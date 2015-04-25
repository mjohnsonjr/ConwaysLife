#include <iostream>
#include <fstream>
#include "life.h"

/**** CLASS IMPLEMENTATIONS FOR THE GAME OF LIFE/OTHER GAMES ******/

/* This file contains implementations of all classes (Board, Cell) used in the game of life. */
/* Classes are declared in lifeclasses.h */


/* Constructors for a Cell.  The Cell class contains the cell's state, and has methods that are called to
get and set the state.  The cell's state can also be specified when constructed.  If no state is specified, 
the default state is 0 */
Cell::Cell()
{
	state = 0;
}

Cell::Cell(char in_state)
{
	state = in_state;
}



/* Methods (getter and setters).. Self Explanitory. */
char Cell::getState()
{
	return state;
}

void Cell::setState(char istate)
{
	state = istate;
}

void Cell::setDead()
{
	state = 0;
}

/* Destructor */
Cell::~Cell()
{

}
/* Cell Class Done */




/* Board Class */

/* Contructs a board.  The Board class has a lot of parameters, but they are all handled by other functions
(namely createBoard().  The board object is a very flexible object that contains all variables required to run
any of the supported simulations (Conways, BriansBrain, etc..) The code is very portable in that each method determines
the type of simulation for the user.  The board contains a double array of Cell objects all with unique states.  */
Board::Board(string sname, char igametype, char* ichars, unsigned char** icolors, int ilowerX, int iupperX, int ilowerY, int iupperY, Cell** icells)
{
	simulationname = sname;
	chars = ichars;
	upperX = iupperX;
	lowerX = ilowerX;
	upperY = iupperY;
	lowerY = ilowerY;
	cells = icells;
	colors = icolors;
	gametype = igametype;


}

/* Destruct board */
Board::~Board()
{
	for(int i = 0; i <= width(); i++)
		delete[] cells[i];
	delete[] cells;
}



/* Board Methods */

/* This method modifies the arrays as it goes through generations.  The parameter, n, is
the number of generations that the data structure will go through on this method call. The method
determines the simulation type (Conways, BriansBrain, etc.) automatically.  This returns true on success */
bool Board::generate(int n)
{
	/* number of neighbors for Cell (x,y), and counters */
	int numneighbors = 0, i = 0, j = 0; 

	/* New array of cells for generation + 1 */
	Cell** newCells = NULL; 	

	/* Used for Langton's Ant to protect already accessed cells */
	bool** locked = NULL;

	/* Loop for n times generations */
	for(int numgen = 0; numgen < n; numgen++)
	{
		/* Reconstruct every iteration to clear and default to all dead cells (default constructor) */
		newCells = new Cell*[upperXrange() - lowerXrange() + 1];
		for(i = 0; i < upperXrange() - lowerXrange() + 1; i++)
			newCells[i] = new Cell[upperYrange() - lowerYrange() + 1];


		/* Conways Life */
		if(gametype == 0)
		{

			/*Check the neighbor cells to determine if this lives or dies. */
			for(i = 0; i <= width(); i++)
			{
				for(j = 0; j <= height(); j++)
				{
					numneighbors = 0;

						if(j < height() && cells[i][j+1].getState())
							numneighbors++;
						if(j > 0 && cells[i][j-1].getState())
							numneighbors++;
						if(i < width() && cells[i+1][j].getState())
							numneighbors++;
						if(i > 0 && cells[i-1][j].getState())
							numneighbors++;
						if(i < width() && j < height() && cells[i+1][j+1].getState())
							numneighbors++;
						if(i > 0 && j > 0 && cells[i-1][j-1].getState())
							numneighbors++;
						if(i > 0 && j < height() && cells[i-1][j+1].getState())
							numneighbors++;
						if(i < width() && j > 0 && cells[i+1][j-1].getState())
						numneighbors++;

					/* Rules (Conways):
						- more than 3 neighbors, cell dies
						- 3 neighbors comes to life
						- 2 or 3 neighbors stays alive
						- less than 2 neighbors dies
					*/	
						if(numneighbors == 3) newCells[i][j].setState(1); /* Cell gains life in this condition */
						else if(numneighbors == 2) /* Cell maintains state */
						{
							if(cells[i][j].getState()) newCells[i][j].setState(1);
						} 
						else if(numneighbors < 2) newCells[i][j].setDead(); /* Condition for cell to die. */
						else if(numneighbors > 3) newCells[i][j].setDead(); /* Starvation condition */
				
				}
			}

		}


		/* Brians Brain */
		else if(gametype == 1)
		{
			for(i = 0; i <= width(); i++)
			{
				for(j = 0; j <= height(); j++)
				{
					numneighbors = 0;

						if(j < height() && cells[i][j+1].getState() == 2)
							numneighbors++;
						if(j > 0 && cells[i][j-1].getState() == 2)
							numneighbors++;
						if(i < width() && cells[i+1][j].getState() == 2)
							numneighbors++;
						if(i > 0 && cells[i-1][j].getState() == 2)
							numneighbors++;
						if(i < width() && j < height() && cells[i+1][j+1].getState() == 2)
							numneighbors++;
						if(i > 0 && j > 0 && cells[i-1][j-1].getState() == 2)
							numneighbors++;
						if(i > 0 && j < height() && cells[i-1][j+1].getState() == 2)
							numneighbors++;
						if(i < width() && j > 0 && cells[i+1][j-1].getState() == 2)
							numneighbors++;

					/* Rules (BB):
						- 2 neighbors, of cell -> on
						- Alive cell -> Dying
						- Dying -> off
					*/	
						if(numneighbors == 2 && cells[i][j].getState() == 0) newCells[i][j].setState(2);
						else if(cells[i][j].getState() == 1) newCells[i][j].setDead();
						else if(cells[i][j].getState() == 2) newCells[i][j].setState(1);
				}
			} 
		}

		/* Wire World */
		else if(gametype == 2)
		{
			for(i = 0; i <= width(); i++)
			{
				for(j = 0; j <= height(); j++)
				{
					numneighbors = 0;

						if(j < height() && cells[i][j+1].getState() == 1)
							numneighbors++;
						if(j > 0 && cells[i][j-1].getState() == 1)
							numneighbors++;
						if(i < width() && cells[i+1][j].getState() == 1)
							numneighbors++;
						if(i > 0 && cells[i-1][j].getState() == 1)
							numneighbors++;
						if(i < width() && j < height() && cells[i+1][j+1].getState() == 1)
							numneighbors++;
						if(i > 0 && j > 0 && cells[i-1][j-1].getState() == 1)
							numneighbors++;
						if(i > 0 && j < height() && cells[i-1][j+1].getState() == 1)
							numneighbors++;
						if(i < width() && j > 0 && cells[i+1][j-1].getState() == 1)
							numneighbors++;

					/* Rules (WW):
						- empty -> empty
						- Electron head -> Electron tail
						- Electron tail -> Wire
						- Wire -> Electron head (if 1 or 2 neighbors are electron heads), otherwise Wire.
					*/	
						if(cells[i][j].getState() == 1) newCells[i][j].setState(2);
						else if(cells[i][j].getState() == 2) newCells[i][j].setState(3);
						else if(cells[i][j].getState() == 3) 
						{
							if(numneighbors == 1 || numneighbors == 2) newCells[i][j].setState(1);
							else newCells[i][j].setState(3);
						}
				}
			}
		}

		/* Langton's Ant */
		else if(gametype == 3)
		{
			/* Determines if cell has already been accessed, I only alloc this if Langdon's Ant */
			if(locked == NULL)
			{
				locked = new bool*[upperXrange() - lowerXrange() + 1];
				for(i = 0; i < upperXrange() - lowerXrange() + 1; i++)
					locked[i] = new bool[upperYrange() - lowerYrange() + 1];
			}

			/* Fill with false, (no cells accessed) */
			for(i = 0; i <= width(); i++)
				for(j = 0; j <= height(); j++)
					locked[i][j] = false;

			for(i = 0; i <= width(); i++)
			{
				for(j = 0; j <= height(); j++)
				{
					/* Rules (LA):
						- If its cell is white, turn right (e.g., if facing north, change to east), otherwise turn left.
						- Change the color of its cell.
						- Move forward one step (based on the direction it is facing).
					*/	
						if(cells[i][j].getState() == 0 && locked[i][j] == false) newCells[i][j].setState(0);
						else if(cells[i][j].getState() == 1 && locked[i][j] == false) newCells[i][j].setState(1);

						/* White ant facing north */
						else if(cells[i][j].getState() == 2 && locked[i][j] == false) 
						{
							if(i < width())
							{
								newCells[i][j].setState(1);
								if(cells[i+1][j].getState() == 0)
									newCells[i+1][j].setState(3);
								else if(cells[i+1][j].getState() == 1)
									newCells[i+1][j].setState(7);
								locked[i+1][j] = true;
							}
							else newCells[i][j].setState(0);

						} 
						/* White ant facing east */
						else if(cells[i][j].getState() == 3 && locked[i][j] == false)
						{
							if(j > 0)
							{
								newCells[i][j].setState(1);
								if(cells[i][j-1].getState() == 0)
									newCells[i][j-1].setState(4);
								else if(cells[i][j-1].getState() == 1)
									newCells[i][j-1].setState(8);
							}
							else newCells[i][j].setState(0);

						}
						/* White ant facing south */
						else if(cells[i][j].getState() == 4 && locked[i][j] == false)
						{
							if(i > 0)
							{
								newCells[i][j].setState(1);
								if(cells[i-1][j].getState() == 0)
									newCells[i-1][j].setState(5);
								else if(cells[i-1][j].getState() == 1)
									newCells[i-1][j].setState(9);
							}
							else newCells[i][j].setState(0);
							
						}
						/* White ant facing west */
						else if(cells[i][j].getState() == 5 && locked[i][j] == false) 
						{
							if(j < height())
							{
								newCells[i][j].setState(1);
								if(cells[i][j+1].getState() == 0)
									newCells[i][j+1].setState(2);
								else if(cells[i][j+1].getState() == 1)
									newCells[i][j+1].setState(6);
								locked[i][j+1] = true;
								
							}
							else newCells[i][j].setState(0);
							
						}
						/* Black ant facing North */
						else if(cells[i][j].getState() == 6 && locked[i][j] == false)
						{
							if(i > 0)
							{
								newCells[i][j].setState(0);
								if(cells[i-1][j].getState() == 0)
									newCells[i-1][j].setState(3);
								else if(cells[i-1][j].getState() == 1)
									newCells[i-1][j].setState(7);
							}
							else newCells[i][j].setState(1);
							
						}
						/* Black ant facing East */
						else if(cells[i][j].getState() == 7 && locked[i][j] == false)
						{
							if(j < height())
							{
								newCells[i][j].setState(0);
								if(cells[i][j+1].getState() == 0)
									newCells[i][j+1].setState(4);
								else if(cells[i][j+1].getState() == 1)
									newCells[i][j+1].setState(8);
								locked[i][j+1] = true;	
							}
							else newCells[i][j].setState(1);
							
						}
						/* Black facing south */
						else if(cells[i][j].getState() == 8 && locked[i][j] == false)
						{
							if(i < width())
							{
								newCells[i][j].setState(0);
								if(cells[i+1][j].getState() == 0)
									newCells[i+1][j].setState(5);
								else if(cells[i+1][j].getState() == 1)
									newCells[i+1][j].setState(9);
								locked[i+1][j] = true;
							}
							else newCells[i][j].setState(1);
							
						}

						/* Black ant is facing West */
						else if(cells[i][j].getState() == 9 && locked[i][j] == false)
						{
							if(j > 0)
							{
								newCells[i][j].setState(0);
								if(cells[i][j-1].getState() == 0)
									newCells[i][j-1].setState(2);
								else if(cells[i][j-1].getState() == 1)
									newCells[i][j-1].setState(6);
							}
							else newCells[i][j].setState(1);
							
						}
						/* Cell was accessed, don't reaccess */
						locked[i][j] = true;
				}
			}
		}

		/* After new grid of cells is built, destroy the old one and set the new on to the board */
		for(i = 0; i <= width(); i++)
			delete[] cells[i];
		delete[] cells;
		cells = newCells;
	}

		if(locked != NULL)
		{
			for(i = 0; i <= width(); i++)
				delete[] locked[i];
			delete[] locked;
		}

	return true;
}

/* Print board in ASCII to stdout */
/* The parameters are a window size in the following order (note you will be
unable to distinguish terrain from window in this instance).  Parameters are 
(x lowbound, x upperbound, y lowerbound, y upperbound).
Prints to stdout, an ASCII representation of a .aut */
void Board::printASCII(int i_xl, int i_xh, int i_yl, int i_yh, bool b_wx = false, bool b_wy = false)
{
	short state;

	/* Check passed in window range, if the booleans aren't set, ignore the passed values and use .aut terrain range */
	if(b_wx == false)
	{
		i_xl = lowerXrange();
		i_xh = upperXrange();
	}

	if(b_wy == false)
	{
		i_yl = lowerYrange();
		i_yh = upperYrange();
	}

	/* Loop through 2d grid of board, printing each cell */
	for(int y = i_yh - lowerYrange(); y >= i_yl - lowerYrange(); y--)
	{
		for(int x = i_xl - lowerXrange(); x <= i_xh - lowerXrange(); x++)
		{	
			
			if(x <= width() && x >= 0 && y <= height() && y >= 0)
			{	
				state = cells[x][y].getState();
				cout << getChars()[state];
			}
			else cout << getChars()[0];

		}
		cout << "\n";
	}
}



/* Creates an .aut file of this board object at the current generation.  This is written to stdout, 
so redirect stdout to save it to a file. */
bool Board::createAut()
{
	/*Used to properly format the output file */
	char lineflag = 1;
	int i = 0; short state = 1;

	/* File is open, now write initial comments */
	cout << "# This file is an auto generated .aut of a Board object.\n#\n";
	cout << "# Generation created by Michael Johnson's 229 Project 2\n#\n\n";

	/* Add Keywords and corresponding values */
	cout << "Name \"" << simulationname << "\";\n\n";
	cout << "Xrange " << lowerXrange() << " " << upperXrange() << ";\n\n";
	cout << "Yrange " << lowerYrange() << " " << upperYrange() << ";\n\n";
	
	/* Rules */
	if(gametype == 0) cout << "Rules ConwaysLife;\n\n";
	if(gametype == 1) cout << "Rules BriansBrain;\n\n";
	if(gametype == 2) cout << "Rules WireWorld;\n\n";
	if(gametype == 3) cout << "Rules LangtonsAnt;\n\n";

	/* Chars */
	if(gametype == 0)
		cout << "Chars " << (int)chars[0] << ", " << (int)chars[1] << ";\n\n";
	else if(gametype == 1)
		cout << "Chars " << (int)chars[0] << ", " << (int)chars[1] << ", " << (int)chars[2] << ";\n\n";
	else if(gametype == 2)
		cout << "Chars " << (int)chars[0] << ", " << (int)chars[1] << ", " << (int)chars[2] << ", " << (int)chars[3] << ";\n\n";
	else if(gametype == 3)
		cout << "Chars " << (int)chars[0] << ", " << (int)chars[1] << ", " << (int)chars[2] << ", " << (int)chars[3] << ", " << (int)chars[4] << ", " << (int)chars[5] << ", " << (int)chars[6] << ", " << (int)chars[7] << ", " << (int)chars[8] << ", " << (int)chars[9] <<";\n\n";

	/* Colors */
	if(gametype == 0)
	{
		cout << "Colors ";
		for(i = 0; i < 2; i++)
		{
			cout << "(" << (int)colors[i][0] << ", " << (int)colors[i][1] << ", " << (int)colors[i][2] << ")";
			if(i + 1 != 2) cout << ", ";
		} 
		cout << ";\n\n";
	}

	if(gametype == 1)
	{
		cout << "Colors ";
		for(i = 0; i < 3; i++)
		{
			cout << "(" << (int)colors[i][0] << ", " << (int)colors[i][1] << ", " << (int)colors[i][2] << ")";
			if(i + 1 != 3) cout << ", ";
		} 
		cout << ";\n\n";
	}

	if(gametype == 2)
	{
		cout << "Colors ";
		for(i = 0; i < 4; i++)
		{
			cout << "(" << (int)colors[i][0] << ", " << (int)colors[i][1] << ", " << (int)colors[i][2] << ")";
			if(i + 1 != 4) cout << ", ";
		} 
		cout << ";\n\n";
	}

	if(gametype == 3)
	{
		cout << "Colors ";
		for(i = 0; i < 10; i++)
		{
			cout << "(" << (int)colors[i][0] << ", " << (int)colors[i][1] << ", " << (int)colors[i][2] << ")";
			if(i + 1 != 10) cout << ", ";
		} 
		cout << ";\n\n";
	}



	/* Keywords done, now add data */
	cout << "Initial{ \n";

	for(int y = 0; y <= height(); y++)
	{
		lineflag = 1; /* Reset every line to re-add Y=.. */

		for(int x = 0; x <= width(); x++)
		{
			if(cells[x][y].getState() != 0)
			{
				if(cells[x][y].getState() != state)
				{
					state = cells[x][y].getState();
					if(lineflag == 0) cout << ";\n";
					cout << "State " << state << ";\n";
					lineflag = 1;
				}

				if(lineflag == 1)
				{	lineflag = 0;
					cout << "Y = " << y + lowerYrange() << ": " << x + lowerXrange();
				}
				else cout << ", " << x + lowerXrange();
			}
		}
		if(lineflag == 0) cout << ";\n"; /* Only newline if we put something on this line */
	}

	/* Now that datapoints have been added, finalize the file with }; */
	cout << "};\n\n";


	return true; /* Success */
}
