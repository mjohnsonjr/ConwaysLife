#include <iostream>
#include <cerrno>
#include <fstream>
#include <string>
#include <cstring>
#include <cctype>
#include <cstdlib>
#include <unistd.h>
#include <ncurses.h>
#include "life.h"


/* This program reads a .aut file passed as an argument, and displays a textual user interface 
starting at the initial generation (gen 0).  The TUI can be controlled according to the controls 
listed below, and the controls are also printed at the bottom of the TUI.

See the README for more detailed information and examples.

*/



using namespace std;


/* Prototypes for local functions */
void redraw(Board* board, int gennumber, char** viewable);
void refillViewable(Board* gameboard, char** viewable);
void help();


/* Global Vars */
int maxx, maxy; /* terminal window size */
int curx, cury; /* This coordinate is the bottom left position of the screen */
float scrollposx, scrollposy; /* Current positions of the scrollbars */
int delay = 100; /* Delay in ms of the play mode of the simulation */



int main(int argc, char** argv)
{	

	/* Flags */
	bool helpflag = false; /* Switches set flags (-a, -g, -h)*/
	bool inputfileflag = false; /* Read from a file, or stdin.  If 1, file has been found on cmd line */
	bool playflag = false; /* Sim is in play mode */

	/* C++ style string of char** argv, looks cleaner */
	string command;
	char commandc[100];
	
	/* The terrain size dimensions to generate (tyl -> tyh and txl -> txh) */
	int tyl = 0, tyh = 0, txl = 0, txh = 0;
						

	/* Flags for terrain modified switches. */
	bool b_tx = false, b_ty = false;

	/* Counters/holders */
	int i = 0; 
	size_t pos;
	char* endptr;
	long hold;

	/* String of input file to use, default to stdin */
	string inputfilename;
	ifstream file;

	/* Scroll ratio variables (number of rows moved per scrollbar move) */
	float xratio;
	float yratio;

	/* Game grid object pointer that contains cells and their states */
	Board* gameboard;




	/* Decode Switches */
	for(i =1; i < argc; i++)
	{	
		command = argv[i];

		if(command.compare("-h") == 0 && helpflag == false)  helpflag = true;

		else if(command.compare("-tx") == 0 && b_tx == false)
		{
			if(i + 1 < argc)
			{
				command = argv[i+1];
			 	strcpy(commandc, argv[i+1]);
			}
			else
			{
				cerr << "ERROR!  -tx requires arguments.\n";
				exit(1);
			}

			 if((pos = command.find(",")) == string::npos)
			 {
			 	cerr << "Incorrect syntax for -tx.  Please see help with -h\n";
			 	exit(1);
			 }
			 else
			 {	
			 	errno = 0;
			 	/* First l */
			 	hold = strtol(commandc, &endptr, 10);

				if(endptr == commandc || hold < -2147683648 || hold > 2147683647) {cerr << commandc << ": Error on conversion of tx. Ensure it is between INT_MIN and INT_MAX.\n"; exit(1);}
				else txl = (int)hold;

			 	/* Then h */
			 	command = command.substr(pos + 1);
			 	strcpy(commandc, command.c_str());

			 	errno = 0;
			 	hold = strtol(commandc, &endptr, 10);

			 	if(errno != 0 || txl > hold || endptr == commandc || hold < -2147683648 || hold > 2147683647) {cerr << commandc <<": Error on conversion of tx. Ensure txl < txh and is between INT_MIN and INT_MAX.\n"; exit(1);}
			 	else txh = (int)hold;

			 	b_tx = true;
			 	i++;
			 }
		}

		else if(command.compare("-ty") == 0 && b_ty == false)
		{
			if(i + 1 < argc)
			{
				command = argv[i+1];
			 	strcpy(commandc, argv[i+1]);	
			}
			else
			{
				cerr << "ERROR!  -ty requires arguments.\n";
				exit(1);
			}
			 

			 if((pos = command.find(",")) == string::npos)
			 {
			 	cerr << "Incorrect syntax for -ty.  Please see help with -h.\n";
			 	exit(1);
			 }
			 else 
			 {
			 	errno = 0;
			 	/* First l */
			 	hold = strtol(commandc, &endptr, 10);

			 	if(endptr == commandc || hold < -2147683648 || hold > 2147683647) {cerr << commandc <<": Error on conversion of ty. Ensure it is between INT_MIN and INT_MAX.\n"; exit(1);}
			 	else tyl = (int)hold;

			 	/* Then h */
			 	command = command.substr(pos + 1);
			 	strcpy(commandc, command.c_str());

			 	errno = 0;
			 	hold = strtol(commandc, &endptr, 10);

			 	if(errno != 0 || tyl > hold || endptr == commandc || hold < -2147683648 || hold > 2147683647) {cerr << commandc <<": Error on conversion of ty. Ensure tyl < tyh and is between INT_MIN and INT_MAX.\n"; exit(1);}
			 	else tyh = (int)hold;

			 	b_ty = true;
			 	i++;
			 }
		}

		else if(command.at(0) != '-' && inputfileflag == false)
		{
			inputfilename = argv[i];
			inputfileflag = true;
		}

		/* Any other unrecogznied switches. Don't use here*/
		else 
		{	
			cerr << "\n" << argv[i] << ": Unrecognized/duplicated switch.  Please see help screen with -h.\n\n";
			exit(1);
		}
	}
	/* Switches Done */
	
	if(helpflag)
	{
		help();
	}

	/* Open file, otherwise use input stream (stdin) */
	if(inputfileflag) 
	{
		file.open(inputfilename.c_str());
		
		if(!file.is_open())
		{
			cerr << "\nERROR! Could not open the file: " << inputfilename << ". Aborting.\n\n";
			exit(1);
		} 

		/* Create a board via filestream, and check to make sure it created properly */
		gameboard = createBoard(file, txl, txh, tyl, tyh, b_tx, b_ty); 

	}
	else
	{
		cerr << "\nNo input file detected!  Please specify an .aut file for generation 0. Aborting.\n\n";
		exit(1); 
	}


	if(gameboard == NULL)
	{
		cerr << "ERROR, the file is corrupt or incorrect (usually because of a missing \"Initial\" block or Xrange/Yrange declarations).\n";
		exit(1);
	}

	/* Init starting display as far left and down as possible (min y, min x) */
	curx = gameboard->lowerXrange();//0
	cury = gameboard->lowerYrange();//0


	/* Initialize curses */
	initscr();
	cbreak();
	nodelay(stdscr, TRUE);
	noecho();
    keypad(stdscr, TRUE);

	getmaxyx(stdscr, maxy, maxx);

	if(maxx < 40 || maxy < 20)
	{
		refresh();
		endwin();
		cerr << "\nYour terminal isn't large enough to support this simulation. Please make the window larger. (Minimum of 40 x 20 required).\n\n";
		exit(1);
	}

	xratio = (float)(maxx -4)/(gameboard->width() - maxx + 2);
	yratio = (float)(maxy -8)/(gameboard->height() - maxy + 5);

	/* Init scrollbar positions */
	scrollposx = 1.5; 
	scrollposy = maxy -3.5; 


	char** viewable = new char*[maxx -2];
	for(i = 0; i < maxx -2; i++) viewable[i] = new char[maxy - 5];

	
	/* Generation counter */
	int gencounter = 0;

	/* Input Character from keyboard */
	short inputchar = 0;





	/* While the user does not type Q..*/
	while(inputchar != 'q')
	{

		/* Check for arrow keys to move window */
		if(inputchar == KEY_UP)
		{
			/* Must safety guard these..*/
			if(gameboard->upperYrange() > (cury + maxy - 5))
			{
				cury++;
				scrollposy = scrollposy - yratio;
			} 
		} 

		else if(inputchar == KEY_DOWN)
		{
			if(gameboard->lowerYrange() < cury)
			{
				cury--;
				scrollposy = scrollposy + yratio;	
			} 
		} 

		else if(inputchar == KEY_LEFT)
		{
			if(gameboard->lowerXrange() < curx)
			{
				curx--;	
				scrollposx = scrollposx - xratio;
			} 
		} 

		else if(inputchar == KEY_RIGHT)
		{
			if(gameboard->upperXrange() > (curx + maxx -2))
			{
				curx++;
				scrollposx = scrollposx + xratio;	
			} 
		} 


		else if(inputchar == 's' && playflag == false)
		{
			gameboard->generate(1);
			gencounter++;
		}

		else if(inputchar == '+' || inputchar == '=')
		{
			if(delay < 2000) delay += 50;
		}

		else if(inputchar == '-' || inputchar == '_')
		{
			if(delay > 0) delay -= 50;
		}

		else if(inputchar == 'p')
		{
			if(playflag == false) playflag = true;
			else playflag = false;
		}

		/* Only playmode when no other chars entered and P is toggled */
		if(playflag && (inputchar == ERR || inputchar == KEY_UP || inputchar == KEY_DOWN || inputchar == KEY_RIGHT || inputchar == KEY_LEFT))
		{
				gameboard->generate(1);
				gencounter++;
				
				/* Refill the viewable screen array */
				refillViewable(gameboard, viewable);
				redraw(gameboard, gencounter, viewable);
				refresh();
				timeout(delay);	/* sleep for time_t (ms) */
		}

		/* Refill the viewable screen array */
		refillViewable(gameboard, viewable);
		redraw(gameboard, gencounter, viewable);
		refresh();
		
		/* Get next user input */
		inputchar = tolower(getch());
}

	/* Game over */
	endwin();

	return 0;
}


void redraw(Board* board, int gennumber, char **viewable)
{
	int i, j;
	/* Print the simulation name */
	mvprintw(0, 0, "                                                   "); // this clears leftover chars from contracting numbers
	/* Print current position */
	 mvprintw(0, 0, "Bottom Left Pos: (%d,%d)", curx, cury);
	mvprintw(0, maxx/2 - board->getName().size()/2, "%s", board->getName().c_str());
	
	/* Print the Delay */
	mvprintw(1, 0, "                                                   "); // this clears leftover chars from contracting numbers
	mvprintw(1, 0, "Delay: %dms (+/-)", delay);
	
	/* Print the Generation number */
	mvprintw(1, maxx - 20, "Generation:	%d", gennumber);

	/* Top bar */
	mvprintw(2, 0, "+");
	for(i = 1; i < maxx -1; i++) mvprintw(2, i, "-");
	mvprintw(2, maxx -1, "+");

	/* Side bar */
	for(i = 3; i < maxy -2; i++) mvprintw(i, 0, "|");
	mvprintw(maxy -2, 0, "+");
	mvprintw(maxy -2, maxx -1, "+");

	/* Print Controls */
	mvprintw(maxy -1, 0, "(Q)uit	(P)lay/Pause	(S)tep	Arrows: Scroll");

	/* Print the generation */
	for(i = 1; i < maxx -1; i++)
	 {
	 	for(j = 3; j < maxy -2; j++)
	 	{
	 			mvprintw(j,i,"%c", viewable[i -1][maxy - j - 3]);
	 	}
	 }

	 /* Print out scrollbars */

	 /* Right scrollbar */
	 for(i = 3; i < maxy -2; i++)
	 {
	 	if(maxy - 5 < board->height() && (i == (int)scrollposy || i + 1 == (int) scrollposy || i -1 == (int)scrollposy)) mvprintw(i, maxx -1, "#");
	 	else mvprintw(i, maxx -1, "|");
	 } 

	 /* Bottom Scrollbar */
	 for(i = 1; i < maxx -1; i++)
	 {	 	
	 	if(maxx -2 < board->width() && (i == (int)scrollposx || i + 2 == (int) scrollposx || i + 1 == (int) scrollposx || i -1 == (int)scrollposx || i -2 == (int) scrollposx)) mvprintw(maxy -2, i, "#");
	 	else mvprintw(maxy -2, i, "-");
	 } 

	 /* Scroll arrows on scrollbar limits */
	 mvprintw(3, maxx-1, "^");
	 mvprintw(maxy -3, maxx -1, "v");

	 mvprintw(maxy -2, 1, "<");
	 mvprintw(maxy -2, maxx -2, ">");



	 
	 /* Move cursor so we can see at the end */
	 wmove(stdscr, 0, maxx -1);

}


/* Refill with viewable chars.  **Viewable contains all the cells that CAN BE SEEN on the current TUI.
This will be updated when the user scrolls with arrow keys */
void refillViewable(Board* gameboard, char** viewable)
{
	short state;

	for(int i = 0; i < maxx -2; i++)
	{
		for(int j = 0; j < maxy -5; j++)
		{		
			if(i <= gameboard->width() && j <= gameboard->height()) state = gameboard->cells[i - gameboard->lowerXrange() + curx][j - gameboard->lowerYrange() + cury].getState();
			else state = 0;

				if(i <= gameboard->width() && j <= gameboard->height() && gameboard->cells[i + curx - gameboard->lowerXrange()][j + cury - gameboard->lowerYrange()].getState() && state != 0)
				{ 	
					viewable[i][j] = gameboard->getChars()[state];
				}
				else if(i <= gameboard->width() && j <= gameboard->height()) viewable[i][j] = gameboard->getChars()[0];
				else viewable[i][j] = ' ';
			}
	}
}



void help()
{
	cout << "\nSim-tui Help Screen:\n\n";
	cout << "\tThis program displays the provided .aut file starting at generation 0.\n";
	cout << "\tThe TUI can be controlled with the following controls:\n";
	cout << "\tP: Play/Pause.  S: Step one generation. Q: Quit.  Arrow Keys: Scrolling. +/-: Change Delay Time\n";
	cout << "\nSupported Switches:\n";
	cout << "\t-h : Display the help screen.\n";
	cout << "\t-tx l,h: Set the X terrain range to l,h.  This must be between INT_MIN and INT_MAX.\n";
	cout << "\t-ty l,h: Set the Y terrain range to l,h.  This must be between INT_MIN and INT_MAX.\n";
	cout << "\nSupported Arguments:\n";
	cout << "\tONE .aut file from the command line.\n\n";
	cout << "Please see the included README file for more detailed information.\n\n";
	exit(1);
}







