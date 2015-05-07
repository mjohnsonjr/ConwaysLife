#ifndef BOARD
#define BOARD
#if 0
using namespace std;

/* This class is a single cell contained in a board, the cell has a couple of methods to change its state, including:
setting alive, or dead state, and checking the state. */
class Cell 
{
	public:
		Cell();
		~Cell();
		Cell(char);

		char getState();
		void setState(char);
		void setDead();

	private:
		char state; /* Can be from 0 - 9 in our supported gametypes */
		
};

/* Board class contains the dimensions, methods to modify and check the dimensions, and it contains a
single pointer array to all the cells contained in the board.  These cells are a faux 2d array. (Cell**) */
class Board
{
	public:
		Board(string, char, char*, unsigned char**, int, int, int, int, Cell**);
		~Board(); 
		Cell** cells; /* Holds a 2d array of Cell objects */

		/* Prototypes for board methods */

		bool generate(int); /* Generates to specified generation n */
		void printASCII(int, int, int, int, bool, bool); /* Prints the game board in ascii text. */
														/* Parameters are windows ranges, NOT terrain ranges, and bools are whether we use them */
		bool createAut(); /* Creates an .aut of this board object to stdout */


		/* inline helper methods to access private vars (getters) */
		inline int width()
		{
			return (upperX - lowerX);
		}

		inline int height()
		{
			return (upperY - lowerY);
		}

		inline int upperXrange()
		{
			return upperX;
		} 
		
		inline int lowerXrange()
		{
			return lowerX;
		}

		inline int upperYrange()
		{
			return upperY;
		}

		inline int lowerYrange()
		{
			return lowerY;
		}
		inline char* getChars()
		{
			return chars;
		}
		
		inline unsigned char** getColors()
		{
			return colors;
		}

		inline string getName()
		{
			return simulationname;
		}

	private:
		int lowerX, upperX, lowerY, upperY;
		string simulationname;
		char gametype;
		char* chars;
		unsigned char** colors;
};

/* This method creates a board pointer filled with information read in from a .aut file
Takes in a string pathname to the .aut file we will use to construct the board */
Board* createBoard(istream& file, int i_txl, int i_txh, int i_tyl, int i_tyh, bool b_tx, bool b_ty);



#endif
#endif
