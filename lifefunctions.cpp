#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <sstream>
#include <cerrno>
#include <algorithm>
#include <cstdlib>
#include "life.h"


using namespace std;

/* This file contains a helper function to setup both the board and cells and do IO. 

	Contains: createBoard(): Creates, allocates, and initializes a Board object from a specified .aut file.
*/




/* Helper function that kills all the whitespace in a string */
void killWhitespace(string &line)
{
		line.erase(std::remove(line.begin(), line.end(), ' '), line.end());
		line.erase(std::remove(line.begin(), line.end(), '\n'), line.end());
		line.erase(std::remove(line.begin(), line.end(), '\t'), line.end());
		line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());
}


/* Creates a board of Cells and returns a pointer to the board.
Board contains pointer array of cells. With the various states of alive/dead based onr
what was read in from either stdin or the specified .aut file.

bool b_tx, and b_ty default to false.  If true, the parameters passed in txl/txh or tyl/tyh will override the .aut file. 

 */

Board* createBoard(istream& file, int i_txl, int i_txh, int i_tyl, int i_tyh, bool b_tx = false, bool b_ty = false)
{

	/* Class Vars, for Board constructor */
	int lowerXrange = 0, upperXrange = 0, lowerYrange = 0, upperYrange = 0;
	Cell** cells = NULL;
	int tempX = 0, tempY = 0;
	string simulationname = "Unnamed Simulation"; /* Defualt name, changed using keyword */
	unsigned char** colors = NULL;
	char* chars = NULL;

	/* 0 indicates ConwaysLife, 1 = BriansBrain, 2 = WireWorld, 3 = LangtonsAnt.  Default Conways */
    char gametype = 0;

    /* Indicates current state to set cells while reading .aut file.  Default state is 1. */
    char state = 1, newstate = 1, numstates = 2;


	/* Misc Vars/Flags */
	string token;
	string line;
	char tokenc[200];

	char Yrangeflag = 0, Xrangeflag = 0, initflag = 0, err = 0, charflag = 0, nameflag = 0, colorflag = 0, rulesflag = 0;
	size_t pos = -1, pos2, pos3, pos4;
	char* endptr;
	int i = 0;
	long holder;




	/* Check inputs, need individual overrides */
	/* This overrides the .aut values for terrain range */
	if(b_tx == true)
	{
		lowerXrange = i_txl;
		upperXrange = i_txh;
		Xrangeflag = 1; 
	}

	if(b_ty == true)
	{
		lowerYrange = i_tyl;
		upperYrange = i_tyh;
		Yrangeflag = 1; 
	}


	/* Get the first line */
	getline(file, line, ';');

	/* Loop until EOF to figure out data from keywords before constructing a board */
	while(!file.eof())
	{	
		line = line + ";";

		/* Find comments */ 
		while((pos = line.find('#', pos + 1)) != string::npos)
		{	
			pos2 = line.find("\n", pos);

			pos3 = line.find("\"", 0);
			pos4 = line.find("\"", pos3 + 1);

			/* Avoid deleting comments in quotes (Name keyword) */
			if(pos > pos3 && pos < pos4 && line.find("Name") != string::npos)
			{
				continue;
			}  

			/* Erases comments starting with '#' and ending with '\n' */
			line.erase(pos, pos2 - pos);
		} 
		

		/* Find all supported keywords */		
		if((pos = line.find("Xrange")) != string::npos && Xrangeflag == 0 && initflag == 0)
		{

			errno = 0; /* Check for errors */

			stringstream ss(line);
			ss >> token;
			if(!(ss >> token)) errno = 1; /* Lower X Range */
			strcpy(tokenc, token.c_str());

			holder = strtol(tokenc, &endptr, 10);

			/* Confirm proper conversion */
			if(errno != 0 || tokenc == endptr || holder < -2147483648 || holder > 2147483647)
			{
				cerr << "ERROR! On Xrange conversions. Ensure X range is between INT_MIN and INT_MAX. Aborting.\n";
				exit(1);
			}
			else lowerXrange = (int)holder;

			if(!(ss >> token)) errno = 1; /* Upper X Range */			
			strcpy(tokenc, token.c_str());

			holder = strtol(tokenc, &endptr, 10);

			/* Confirm proper conversion */
			if(errno != 0 || tokenc == endptr || holder < -2147483648 || holder > 2147483647)
			{
				cerr << "ERROR! On Xrange conversions. Ensure X range is between INT_MIN and INT_MAX. Aborting.\n";
				exit(1);
			}
			else upperXrange = (int)holder;

			/* Success, set flag */
			Xrangeflag = 1;
		}

		else if((pos = line.find("Yrange")) != string::npos && Yrangeflag == 0 && initflag == 0)
		{

			errno = 0; /* Check for errors */

			stringstream ss(line);
			ss >> token;
			if(!(ss >> token)) errno = 1; /* Lower Y Range */
			strcpy(tokenc, token.c_str());

			holder = strtol(tokenc, &endptr, 10);

			/* Confirm proper conversion */
			if(errno != 0 || tokenc == endptr || holder < -2147483648 || holder > 2147483647)
			{
				cerr << "ERROR! On Yrange conversions. Ensure Y range is between INT_MIN and INT_MAX. Aborting.\n";
				exit(1);
			}
			else lowerYrange = (int)holder;

			if(!(ss >> token)) errno = 1; /* Upper Y Range */
			strcpy(tokenc, token.c_str());

			holder = strtol(tokenc, &endptr, 10);

			/* Confirm proper conversion */
			if(errno != 0 || tokenc == endptr || holder < -2147483648 || holder > 2147483647)
			{
				cerr << "ERROR! On Yrange conversions. Ensure Y range is between INT_MIN and INT_MAX. Aborting.\n";
				exit(1);
			}
			else upperYrange = (int)holder;
			
			/* Success, set flag */
			Yrangeflag = 1;

		}

		else if((pos = line.find("Name")) != string::npos && nameflag == 0 && initflag == 0)
		{
			pos3 = line.find("\"") + 1;
			pos4 = line.find("\"", pos3);

			/* safety check */
			if(pos3 != string::npos && pos4 != string::npos) simulationname = line.substr(pos3, pos4 - pos3);

			nameflag = 1;
		}

        else if((pos = line.find("Rules")) != string::npos && rulesflag == 0 && initflag == 0)
		{
			stringstream ss(line);

			ss >> token;
            ss >> token;

            if(Xrangeflag == 0 || Yrangeflag == 0) cerr << "WARNING!  This .aut does not conform to specs!  The \"Rules\" keyword should come after Xrange and Yrange statements.\n";


            if(token.find("ConwaysLife") != string::npos)
            {
                gametype = 0;
                chars = new char[2];
                chars[0] = '~';
                chars[1] = '1';

                colors = new unsigned char*[2];
                for(i = 0; i < 2; i++)
                    colors[i] = new unsigned char[3];
                colors[0][0] = 0;
                colors[0][1] = 0;
                colors[0][2] = 0;

                colors[1][0] = 255;
                colors[1][1] = 255;
                colors[1][2] = 255;

            }

            else if(token.find("BriansBrain") != string::npos)
            {
                gametype = 1;
                chars = new char[3];
                chars[0] = '~';
                chars[1] = '1';
                chars[2] = '2';

                colors = new unsigned char*[3];
                for(i = 0; i < 3; i++)
                    colors[i] = new unsigned char[3];
                colors[0][0] = 0;
                colors[0][1] = 0;
                colors[0][2] = 0;

                colors[1][0] = 255;
                colors[1][1] = 255;
                colors[1][2] = 255;

                colors[2][0] = 255;
                colors[2][1] = 0;
                colors[2][2] = 0;

            }

            else if(token.find("WireWorld") != string::npos)
            {
                gametype = 2;
                chars = new char[4];
                chars[0] = '~';
                chars[1] = '1';
                chars[2] = '2';
                chars[3] = '3';

                colors = new unsigned char*[4];
                for(i = 0; i < 4; i++)
                    colors[i] = new unsigned char[3];
                colors[0][0] = 0;
                colors[0][1] = 0;
                colors[0][2] = 0;

                colors[1][0] = 255;
                colors[1][1] = 255;
                colors[1][2] = 255;

                colors[2][0] = 255;
                colors[2][1] = 0;
                colors[2][2] = 0;

                colors[3][0] = 0;
                colors[3][1] = 255;
                colors[3][2] = 0;
            }

            else if(token.find("LangtonsAnt") != string::npos)
            {
                gametype = 3;
                chars = new char[10];
                chars[0] = '~';
                chars[1] = '1';
                chars[2] = '2';
                chars[3] = '3';
                chars[4] = '4';
                chars[5] = '5';
                chars[6] = '6';
                chars[7] = '7';
                chars[8] = '8';
                chars[9] = '9';
      

                colors = new unsigned char*[10];
                for(i = 0; i < 10; i++)
                    colors[i] = new unsigned char[3];
                colors[0][0] = 0;
                colors[0][1] = 0;
                colors[0][2] = 0;

                colors[1][0] = 255;
                colors[1][1] = 255;
                colors[1][2] = 255;

                colors[2][0] = 0;
                colors[2][1] = 0;
                colors[2][2] = 0;

                colors[3][0] = 0;
                colors[3][1] = 0;
                colors[3][2] = 0;

                colors[4][0] = 0;
                colors[4][1] = 0;
                colors[4][2] = 0;

                colors[5][0] = 0;
                colors[5][1] = 0;
                colors[5][2] = 0;

                colors[6][0] = 255;
                colors[6][1] = 255;
                colors[6][2] = 255;

                colors[7][0] = 255;
                colors[7][1] = 255;
                colors[7][2] = 255;

                colors[8][0] = 255;
                colors[8][1] = 255;
                colors[8][2] = 255;

                colors[9][0] = 255;
                colors[9][1] = 255;
                colors[9][2] = 255;
            }
            else /* If keyword is unrecognized, play Conway's Life. */
            {
            	cerr << "WARNING!  Rules keyword was unrecognized in .aut file.  Defaulting to ConwaysLife.\n";

            	gametype = 0;
                chars = new char[2];
                chars[0] = '~';
                chars[1] = '1';

                colors = new unsigned char*[2];
                for(i = 0; i < 2; i++)
                    colors[i] = new unsigned char[3];
                colors[0][0] = 0;
                colors[0][1] = 0;
                colors[0][2] = 0;

                colors[1][0] = 255;
                colors[1][1] = 255;
                colors[1][2] = 255;

            }

              /* Figure out number of possible states for error checks in Initial block */
            if(gametype == 0) numstates = 2;
            else if(gametype == 1) numstates = 3;
            else if (gametype == 2) numstates = 4;
            else if (gametype == 3) numstates = 10;

			rulesflag = 1;
		}

		else if((pos = line.find("Chars")) != string::npos && charflag == 0 && initflag == 0)
		{
            if(rulesflag == 0)
            {
            	cerr << "WARNING!  No \"Rules\" keyword detected before \"Colors\" keyword.  Defaulting to ConwaysLife.\n";
                chars = new char[2];
                chars[0] = '~';
                chars[1] = '1';

                colors = new unsigned char*[2];
                for(i = 0; i < 2; i++)
                    colors[i] = new unsigned char[3];
                colors[0][0] = 0;
                colors[0][1] = 0;
                colors[0][2] = 0;

                colors[1][0] = 255;
                colors[1][1] = 255;
                colors[1][2] = 255;

                rulesflag = 1;
                numstates = 2;
            }

			/* Delete whitespace for iteration */
			killWhitespace(line);
			i = 0;
			char hold;


			pos = line.find("Chars") + 5;

			while(line.at(pos) != ';' && i < numstates)
			{
				errno = 0;
				strcpy(tokenc, line.substr(pos).c_str());

                hold = (char)(strtol(tokenc, &endptr, 10));
				
				if(errno == 0 && tokenc != endptr && hold >= 32 && hold <= 126)
				{
                    chars[i] = hold;
             
					while(line.at(pos) != ';' && line.at(pos) != ',') pos++;
					if(line.at(pos) != ';')pos++; /* Avoid OOB exception */
                }
                else
                {
                	cerr << "WARNING!  Problem reading Char for State " << i << ".  Make sure it is between ASCII 32 and 126.  Reverting to default char: " << (short)chars[i] << ".\n";
                	while(line.at(pos) != ';' && line.at(pos) != ',') pos++;
                	if(line.at(pos) != ';')pos++;
                } 
                i++; /* Tracks char count */
			}

			charflag = 1;
		}

        else if((pos = line.find("Colors")) != string::npos && colorflag == 0 && initflag == 0)
        {
            if(rulesflag == 0)
            {
            	cerr << "WARNING!  No \"Rules\" keyword detected before \"Colors\" keyword.  Defaulting to ConwaysLife.\n";
                chars = new char[2];
                chars[0] = '~';
                chars[1] = '1';

                colors = new unsigned char*[2];
                for(i = 0; i < 2; i++)
                    colors[i] = new unsigned char[3];
                colors[0][0] = 0;
                colors[0][1] = 0;
                colors[0][2] = 0;

                colors[1][0] = 255;
                colors[1][1] = 255;
                colors[1][2] = 255;

                rulesflag = 1;
                numstates = 2;
            }

			/* Erase spaces */
			killWhitespace(line);
			i = 0;

            while(line.at(pos) != ';' && i < numstates)
			{
				if(line.at(pos) == '(')
				{
					pos++;
					errno = 0;
					
					/* Red */
					strcpy(tokenc, line.substr(pos).c_str());
                    holder = strtol(tokenc, &endptr, 10);
					if(errno != 0 || tokenc == endptr || holder < 0 || holder > 255)
					{
						cerr << "WARNING!  Problem reading color Red for State " << i << ".  Reverting to default value: " << (short)colors[i][0] << ".\n";
					}
					else colors[i][0] = (unsigned char)holder;
					while(line.at(pos) != ',') pos++;
					pos++;

					/* Green */
					errno = 0;
					strcpy(tokenc, line.substr(pos).c_str());
                    holder = strtol(tokenc, &endptr, 10);
					if(errno != 0 || tokenc == endptr || holder < 0 || holder > 255)
					{
						cerr << "WARNING!  Problem reading color Green for State " << i << ".  Reverting to default value: " << (short)colors[i][0] << ".\n";
					}
					else colors[i][1] = (unsigned char)holder;
					while(line.at(pos) != ',') pos++;
					pos++;

					/* Blue */
					errno = 0;
					strcpy(tokenc, line.substr(pos).c_str());
                    holder = strtol(tokenc, &endptr, 10);
					if(errno != 0 || tokenc == endptr || holder < 0 || holder > 255)
					{
						cerr << "WARNING!  Problem reading color Blue for State " << i << ".  Reverting to default value: " << (short)colors[i][0] << ".\n";
					}
					else colors[i][2] = (unsigned char)holder;
					while(line.at(pos) != ')') pos++;

					i++; /* Tracks color count */

				}
				pos++; /* safety */

			}
			colorflag = 1;	
		}

		/* Decode the "alive cells, construct them, and fill data." */
		/* This section is used to for the first line, instantiate the pointer, and deals with the Initial {} section; */
		else if((pos = line.find("Initial")) != string::npos && line.find("{") != string::npos && Yrangeflag == 1 && Xrangeflag == 1 && initflag == 0)
		{
			initflag = 1;

            if(rulesflag == 0)
            {
            	chars = new char[2];
                chars[0] = '~';
                chars[1] = '1';

                colors = new unsigned char*[2];
                for(i = 0; i < 2; i++)
                    colors[i] = new unsigned char[3];
                colors[0][0] = 0;
                colors[0][1] = 0;
                colors[0][2] = 0;

                colors[1][0] = 255;
                colors[1][1] = 255;
                colors[1][2] = 255;

                rulesflag = 1;
                numstates = 2;

            }



			/* Alloc all the space in the arrays */
			//numcells =(upperXrange-lowerXrange + 1) * (upperYrange - lowerYrange + 1);
			cells = new Cell*[upperXrange-lowerXrange + 1];
			for(i = 0; i < upperXrange-lowerXrange + 1; i++)
				cells[i] = new Cell[upperYrange - lowerYrange + 1];


			
			
			if((pos = line.find("State")) != string::npos)
			{
				stringstream ss(line.substr(pos));
				ss >> token;
				errno = 0;

				if(!(ss >> token)) errno = 1; /* Next state */
				
				strcpy(tokenc, token.c_str());

				newstate = (char)strtol(tokenc, &endptr, 10);

				if(newstate < numstates && errno == 0 && tokenc != endptr) state = newstate;
				else cerr << "WARNING!  .aut file attempted to change to an illegal/unknown state (" << (short)newstate << ").  Ignoring and remaining in State " << (short)state <<".\n";


				getline(file, line, ';');
				continue; //Found a state change, continue to handle in next part.
			} 

			/* Kill spaces */			
			killWhitespace(line);

			/* Parse the line further */
			pos = line.find("Y");

			if(pos == string::npos)
			{
				getline(file, line, ';');
				continue; /* Safety check if no Y found */
			} 

			while(line.at(pos) != '=')
			{
				pos++;
				if(line.at(pos) == ';') break;
			} if(line.at(pos) == ';')
				{
					getline(file, line, ';');
					continue;	
				} 
			
				errno = 0;
				strcpy(tokenc, line.substr(pos + 1).c_str());


				holder = strtol(tokenc, &endptr, 10);
				pos++;

				if(holder > upperYrange || holder < lowerYrange || errno != 0 || endptr == tokenc || holder < -2147483648 || holder > 2147483647)
				{
						cerr << "WARNING!  Skipping all cells with Y = " << holder << " because they're outside the terrain or invalid.\n";
						getline(file, line, ';');
						continue;
				}
				else tempY = (int)holder;

				
				/* Now we have the Y val, we need all listed X vals */
				while(line.at(pos) != ':')
				{ 
					pos++; //Ensuring proper formatting.
					if(line.at(pos) == ';') break;
				} if(line.at(pos) == ';')
				
					{
						getline(file, line, ';');
						continue;
					} 

				/* Initial reading after colon: */
					errno = 0;
					strcpy(tokenc, line.substr(pos + 1).c_str());

					holder = strtol(tokenc, &endptr, 10);
					pos++;


					if(holder > upperXrange || holder < lowerXrange || errno != 0 || endptr == tokenc || holder < -2147483648 || holder > 2147483647)
					{
						cerr << "WARNING!  Skipping cell " << "(" << holder << ", " << tempY << ") because it's outside the terrain or invalid.\n";
						err = 1;
					}
					else tempX = (int)holder;


					/* Now that we have an X val, set the cell to alive */
					if(err == 0) cells[tempX - lowerXrange][tempY - lowerYrange].setState(state);
					else err = 0;

				while(line.at(pos) != ';')
				{
					
					while(line.at(pos) != ',')
					{
						pos++; /* increment to next X, or end */
						if(line.at(pos) == ';') break;
					} if(line.at(pos) == ';') break;

					errno = 0;
					strcpy(tokenc, line.substr(pos + 1).c_str());

					holder = strtol(tokenc, &endptr, 10);
					pos++;
					if(holder > upperXrange || holder < lowerXrange || errno != 0 || endptr == tokenc || holder < -2147483648 || holder > 2147483647)
					{
						cerr << "WARNING!  Skipping cell " << "(" << holder << ", " << tempY << ") because it's outside the terrain or invalid.\n";
						continue;
					}
					else tempX = (int)holder;


					/* Set the found cell state to alive */
					cells[tempX - lowerXrange][tempY - lowerYrange].setState(state);
				}
		}


		/* This is for the rest of the lines after Initial{} */
		else if(initflag == 1)
		{	

			if((pos = line.find("State")) != string::npos)
			{
				stringstream ss(line.substr(pos));
				ss >> token;
				errno = 0;

				if(!(ss >> token)) errno = 1; /* Next state */
				
				strcpy(tokenc, token.c_str());

				newstate = (char)strtol(tokenc, &endptr, 10);

				if(newstate < numstates && errno == 0 && tokenc != endptr) state = newstate;
				else cerr << "WARNING!  .aut file attempted to change to an illegal/unknown state (" << (short)newstate << ").  Ignoring and remaining in State " << (short)state <<".\n";


				getline(file, line, ';');
				continue; //Found a state change, continue to handle in next part.
			} 

			/* Destroy whitespace for iteration */
			killWhitespace(line);

			/* Detect if we are done */
			if(line.find("}") != string::npos && line.find(";",line.find("}")) != string::npos) break;

			/* Parse the line further */
			pos = line.find("Y");
			

			if(pos == string::npos) 
			{
				getline(file, line, ';');
				continue; /* Safety check if no Y found */
			}

			while(line.at(pos) != '=')
			{
				pos++;
				if(line.at(pos) == ';') break;
			} if(line.at(pos) == ';') 
				{
					getline(file, line, ';');
					continue;
				}
				errno = 0;
				strcpy(tokenc, line.substr(pos + 1).c_str());
				
				holder = strtol(tokenc, &endptr, 10);
				pos++;


				if(holder > upperYrange || holder < lowerYrange || errno != 0 || tokenc == endptr || holder < -2147483648 || holder > 2147483647)
				{
						cerr << "WARNING!  Skipping all cells with Y = " << holder << " because they're outside the terrain or invalid.\n";
						getline(file, line, ';');
						continue;
				}
				else tempY = (int)holder;
				
				/* Now we have the Y val, we need all listed X vals */
				while(line.at(pos) != ':')
				{ 
					pos++; //Ensuring proper formatting.
					if(line.at(pos) == ';') break;
				} if(line.at(pos) == ';') 

					{
						getline(file, line, ';');
						continue;
					}

				/* Initial reading after colon: */
					errno = 0;
					strcpy(tokenc, line.substr(pos + 1).c_str());
					holder = strtol(tokenc, &endptr, 10);
					pos++;


					if(holder > upperXrange || holder < lowerXrange || errno != 0 || tokenc == endptr || holder < -2147483648 || holder > 2147483647)
					{
						cerr << "WARNING!  Skipping cell " << "(" << holder << ", " << tempY << ") because it's outside the terrain or invalid.\n";
						err = 1;
					}
					else tempX = (int)holder;

					/* Now that we have an X val, set the cell to alive */
					if(err == 0) cells[tempX - lowerXrange][tempY - lowerYrange].setState(state);
					else err = 0;

				while(line.at(pos) != ';')
				{
					while(line.at(pos) != ',')
					{
						pos++; /* increment to next X, or end */
						if(line.at(pos) == ';') break;
					} if(line.at(pos) == ';') break;

					errno = 0;
					strcpy(tokenc, line.substr(pos + 1).c_str());
					holder = strtol(tokenc, &endptr, 10);
					pos++;
					if(holder > upperXrange || holder < lowerXrange || errno != 0 || tokenc == endptr || holder < -2147483648 || holder > 2147483647)
					{	
						cerr << "WARNING!  Skipping cell " << "(" << holder << ", " << tempY << ") because it's outside the terrain or invalid.\n";
						continue;
					}
					else tempX = (int)holder;

					/* Set cell state to alive */
					cells[tempX - lowerXrange][tempY - lowerYrange].setState(state);
				}
		}

		else
		{
			/* Nothing happens, ignored keyword or an empty line. */
		}
		
		/* Gets the next line in the file up to a ';' */
		getline(file, line, ';');
	}	
	/* Construct our Board after EOF. */

	if(initflag == 0 || Xrangeflag == 0 || Yrangeflag == 0) return NULL; /* Something went wrong. Bad .aut file/syntax. */

	Board* board = new Board(simulationname, gametype, chars, colors, lowerXrange, upperXrange, lowerYrange, upperYrange, cells);
	
	return board;
}









