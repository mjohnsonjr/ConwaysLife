#include <iostream>
#include <cerrno>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <cstdlib> 
#include "life2.hpp"


/* This program reads a .aut file passed as an argument, and generates to gen N (see -g switch) according 
to Conway's game of life rules to standard output.  This program only writes to standard output, to redirect 
this output to a file use "> filename" on the command line.  Showgen can either create an ASCII representation 
of the output, or it can generate a new .aut file (see -a switch) for generation N. 

See the README for more detailed information and examples.

*/


using namespace std;

/* Prints the help screen */
void help();


int main(int argc, char** argv)
{	

	/* Flags */
	bool helpflag = false, genflag = false, autflag = false; /* Switches set flags (-a, -g, -h)*/
	bool inputfileflag = false; /* Read from a file, or stdin.  If 1, file has been found on cmd line */

	/* C++ style string of char** argv, looks cleaner */
	string command;
	char commandc[100];
	
	/* Generate number, default zero and window dimensions (Only overridden with switches) */
	int gennumber = 0, tyl = 0, tyh = 0, txl = 0, txh = 0, wyl = 0, wyh = 0, wxl = 0, wxh = 0;
						

	/* Flags for terrain modified switches. */
	bool b_tx = false, b_ty = false, b_wx = false, b_wy = false;

	/* Counters */
	int i = 0; 

	/* Error and position detection */
	char* endptr;
	long hold;
	size_t pos;

	/* String and input file to use */
	string inputfilename;
	ifstream file;

	/* The game grid object pointer containing cells and their states */
	Board* gameboard;


	/* Decode Switches */
	for(i =1; i < argc; i++)
	{	
		command = argv[i];

		if(command.compare("-a") == 0 && autflag == false)  autflag = true; /* set aut flag. */

		else if(command.compare("-h") == 0 && helpflag == false)  helpflag = true; /* set help flag. */
		
		else if(command.compare("-g") == 0 && genflag == false)
		{
			 genflag = true; 
			 errno = 0;
			 i++;
			 if(i <  argc) hold = strtol(argv[i], &endptr, 10);
			 else
			 {
			 	cerr << "ERROR!  Generation (-g) switch requires an argument.\n";
			 	exit(1);
			 }
			 if(errno != 0 || hold < 0 || hold > 2147683647 || endptr == argv[i])
			 {
			 	cerr << "\n" << argv[i] << ": Invalid value generation number (must be non-negative and smaller than INT_MAX).  Aborting.\n\n";
			 	exit(1);
			 }
			 else gennumber = (int)hold;
		}

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

			 	if(errno != 0 || txl > hold || endptr == commandc || hold < -2147683648 || hold > 2147683647) {cerr << commandc << ": Error on conversion of tx. Ensure txl < txh and is between INT_MIN and INT_MAX.\n"; exit(1);}
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

			 	if(endptr == commandc || hold < -2147683648 || hold > 2147683647) {cerr << commandc << ": Error on conversion of ty. Ensure it is between INT_MIN and INT_MAX.\n"; exit(1);}
			 	else tyl = (int)hold;

			 	/* Then h */
			 	command = command.substr(pos + 1);
			 	strcpy(commandc, command.c_str());

			 	errno = 0;
			 	hold = strtol(commandc, &endptr, 10);

			 	if(errno != 0 || tyl > hold || endptr == commandc || hold < -2147683648 || hold > 2147683647) {cerr << commandc << ": Error on conversion of ty. Ensure tyl < tyh and is between INT_MIN and INT_MAX.\n"; exit(1);}
			 	else tyh = (int)hold;

			 	b_ty = true;
			 	i++;
			 }
		}

		else if(command.compare("-wx") == 0 && b_wx == false)
		{
			if(i + 1 < argc)
			{
				command = argv[i+1];
			 	strcpy(commandc, argv[i+1]);	
			}
			else
			{
				cerr << "ERROR!  -wx requires arguments.\n";
				exit(1);
			}



			if((pos = command.find(",")) == string::npos)
			 {
			 	cerr << "Incorrect syntax for -wx.  Please see help with -h.\n";
			 	exit(1);
			 }	
			 else 
			 {
			 	errno = 0;
			 	/* First l */
			 	hold = strtol(commandc, &endptr, 10);

			 	if(endptr == commandc || hold < -2147683648 || hold > 2147683647) {cerr << commandc << ": Error on conversion of wx. Ensure it is between INT_MIN and INT_MAX.\n"; exit(1);}
			 	else wxl = (int)hold;

			 	/* Then h */
			 	command = command.substr(pos + 1);
			 	strcpy(commandc, command.c_str());

			 	errno = 0;
			 	hold = strtol(commandc, &endptr, 10);

			 	if(errno != 0 || wxl > hold || endptr == commandc || hold < -2147683648 || hold > 2147683647) {cerr << commandc << ": Error on conversion of wx. Ensure wxl < wxh and is between INT_MIN and INT_MAX.\n"; exit(1);}
			 	else wxh = (int)hold;

			 	b_wx = true;
			 	i++;
			 } 
		}

		else if(command.compare("-wy") == 0 && b_wy == false)
		{
			if(i + 1 < argc)
			{
				command = argv[i+1];
			 	strcpy(commandc, argv[i+1]);	
			}
			else
			{
				cerr << "ERROR!  -wy requires arguments.\n";
				exit(1);
			}



			 if((pos = command.find(",")) == string::npos)
			 {
			 	cerr << "Incorrect syntax for -wy.  Please see help with -h.\n";
			 	exit(1);
			 }
			 else
			 {
			 	errno = 0;
			 	/* First l */
			 	hold = strtol(commandc, &endptr, 10);

			 	if(endptr == commandc || hold < -2147683648 || hold > 2147683647) {cerr << commandc << ": Error on conversion of wy. Ensure it is between INT_MIN and INT_MAX.\n"; exit(1);}
			 	else wyl = (int)hold;

			 	/* Then h */
			 	command = command.substr(pos + 1);
			 	strcpy(commandc, command.c_str());

			 	errno = 0;
			 	hold = strtol(commandc, &endptr, 10);

			 	if(errno != 0 || wyl > hold || endptr == commandc || hold < -2147683648 || hold > 2147683647) {cerr << commandc << ": Error on conversion of wy. Ensure wyl < wyh and is between INT_MIN and INT_MAX.\n"; exit(1);}
			 	else wyh = (int)hold;

			 	b_wy = true;
			 	i++;
			 }
		}

		else if(command.at(0) != '-' && inputfileflag == false)
		{
			inputfilename = argv[i];
			inputfileflag = true;
		}

		/* Any other unrecogznied switches or duplicates. Don't use here*/
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

	if(inputfileflag) 
	{
		file.open(inputfilename.c_str());
		
		if(!file.is_open())
		{
			cerr << "\nERROR! Could not open the file: " << inputfilename << ". Aborting.\n\n";
			exit(1);
		} 

		/* Create a board via a filestream, and check to make sure it created properly */
		gameboard = createBoard(file, txl, txh, tyl, tyh, b_tx, b_ty); 

	}
	else
	{
		/* Create a board via input stream, and check to make sure it created properly */
		cerr << "\nReading .aut format from Standard input: \n\n";
		gameboard = createBoard(std::cin, txl, txh, tyl, tyh, b_tx, b_ty); 
	}


	
	if(gameboard == NULL)
	{
		cerr << "ERROR, the file is corrupt or incorrect (usually because of a missing \"Initial\" block or Xrange/Yrange declarations).\n";
		exit(1);
	}



	/* Gameboard created, now progress it through the specified number generations. */
	gameboard->generate(gennumber); /* If not specified, defaults zero */


	/* If no -a switch, then print ASCII display of generation n */
	if(autflag)
	{
		gameboard->createAut();
		cerr << "\nOutput generated in .aut format to output stream.\n\n";

	}
	
	/* Output of generation n is represented in an .aut file */
	else
	{	
		gameboard->printASCII(wxl, wxh, wyl, wyh, b_wx, b_wy);
	}
	

	delete gameboard;
	return 0;

}


void help()
{
	cout << "\nShowgen Help Screen:\n\n";
	cout << "\tThis program generates the provided .aut file to generation n (default 0) specified by the -g switch.\n";
	cout << "\tThe output can be either in a .aut file or an ASCII representation.  All output is directed to stdout.\n";
	cout << "\nSupported Switches:\n";
	cout << "\t-a : Output to a .aut file (to stdout)\n";
	cout << "\t-h : Display the help screen.\n";
	cout << "\t-g n : Generate to generation n.\n";	
	cout << "\t-tx l,h: Set the X terrain range to l,h.  This must be between INT_MIN and INT_MAX.\n";
	cout << "\t-ty l,h: Set the Y terrain range to l,h.  This must be between INT_MIN and INT_MAX.\n";
	cout << "\t-wx l,h: Set the X window range to l,h.  This must be between INT_MIN and INT_MAX.\n";
	cout << "\t-wy l,h: Set the Y window range to l,h.  This must be between INT_MIN and INT_MAX.\n";
	cout << "\nSupported Arguments:\n";
	cout << "\tONE .aut file from the command line.  If not file is specified, standard input is read.\n\n";
	cout << "For more detailed information please see the included README file.\n\n";
	exit(1);
}
