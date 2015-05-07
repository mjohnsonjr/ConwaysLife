/*
 * lifeclasses2.cpp
 *
 *  Created on: May 3, 2015
 *      Author: michael
 */
#include <string>
#include "life2.hpp"

/*----------------------------------------------------------
             BOARD CONNSTRUCTORS/DESTRUCTORS
----------------------------------------------------------*/
Board::Board( board_preferences& _preferences ) : preferences( _preferences ) {

}

Board::~Board() {

}

/*----------------------------------------------------------
                     BOARD METHODS
----------------------------------------------------------*/
void Board::generateAut( std::ostream& output ) {
	bool lineflag = false;
	uint8_t state = 1;

	/* File is open, now write initial comments */
	output << "# This file is an auto generated .aut of a Board object.\n#\n";
	output << "# Generation created by Michael Johnson's 229 Project 2\n#\n\n";

	/* Add Keywords and corresponding values */
	output << "Name \"" << this->name << "\";\n\n";
	output << "Xrange " << this->preferences.getLowerXRange() << " "
			<< this->preferences.getUpperXRange() << ";\n\n";
	output << "Yrange " << this->preferences.getLowerYRange() << " "
			<< this->preferences.getUpperYRange() << ";\n\n";

	/* Specify game_type */
	switch( this->preferences.getGameType() ){
		case CONWAYS_LIFE:
			output << "Rules ConwaysLife;\n\n";
			break;
		case BRIANS_BRAIN:
			output << "Rules BriansBrain;\n\n";
			break;
		case WIRE_WORLD:
			output << "Rules WireWorld;\n\n";
			break;
		case LANGTONS_ANT:
			output << "Rules LangtonsAnt;\n\n";
			break;
	}

	/* Specify state chars */
	output << "Chars ";
	for( uint8_t i = 0; i < this->preferences.getNumStates(); i++) {
		output << this->preferences.getCharacters()[i];
		output << ( ( ( i + 1 ) != this->preferences.getNumStates() ) ? ";\n\n" : "" );
	}

	/* Specify state colors */
	output << "Colors ";
	for( uint8_t i = 0; i < this->preferences.getNumStates(); i++) {
		output << "(" << this->preferences.getColors().red[i] << ", "
			   << this->preferences.getColors().green[i] << ", "
			   << this->preferences.getColors().blue[i] << ")";
		output << ( ( ( i + 1 ) != this->preferences.getNumStates() ) ? ";\n\n" : ")" );
	}

	/* Keywords done, now add data TODO: Clean this up a bit. */
	output << "Initial{\n";
	for(int y = 0; y <= this->preferences.getHeight(); y++) {
		lineflag = true; /* Reset every line to re-add Y=.. */

		for(int x = 0; x <= this->preferences.getWidth(); x++) {
			if(cells[x][y].getState() != 0) {
				if(cells[x][y].getState() != state) {
					state = cells[x][y].getState();
					if(lineflag == false) output << ";\n";
					output << "State " << state << ";\n";
					lineflag = true;
				}

				if(lineflag == true){
				lineflag = false;
				output << "Y = " << y + this->preferences.getLowerYRange() << ": " << x + this->preferences.getLowerXRange();
				}
				else{
					output << ", " << x + this->preferences.getLowerXRange();
				}
			}
		}
		if(lineflag == false) output << ";\n"; /* Only newline if we put something on this line */
	}

	/* Now that datapoints have been added, finalize the file with }; */
	output << "};\n\n";

}

void Board::generateASCII( std::ostream& output, cartesian_range& window ) {

	for( int y = window.u_y - this->preferences.getLowerYRange();
			y >= window.l_y - this->preferences.getLowerYRange(); y-- ) {
		for( int x = window.l_x - this->preferences.getLowerXRange();
				x <= window.u_x - this->preferences.getLowerXRange(); x++ ) {
			if( x <= this->preferences.getWidth() && x >= 0
					&& y <= this->preferences.getHeight() && y >= 0 ) {
					output << this->preferences.getCharacters()[ this->cells[x][y].getState() ];
			}
			else {
				output << this->preferences.getCharacters()[ STATE_0 ];
			}
		}
		/* Start next line */
		output << "\n";
	}

}

/*----------------------------------------------------------
              CELL CONSTRUCTORS/DESTRUCTORS
----------------------------------------------------------*/
Cell::Cell( uint8_t _state ) {
	this->state = _state;

}

Cell::~Cell() {

}

/*----------------------------------------------------------
                     CELL METHODS
----------------------------------------------------------*/


uint8_t Cell::getState() {
	return this->state;
}

void Cell::setState( uint8_t _state ) {
	this->state = _state;
}

/*----------------------------------------------------------
           BOARD PREFS CONNSTRUCTORS/DESTRUCTORS
----------------------------------------------------------*/
board_preferences::board_preferences( std::string _name, game_type _gametype, cartesian_range _terrain ) {
	this->name = _name;
	this->gametype = _gametype;
	this->terrain = _terrain;
	switch( gametype ) {
		case CONWAYS_LIFE:
			this->num_states = 2;
			break;
		case BRIANS_BRAIN:
			this->num_states = 3;
			break;
		case WIRE_WORLD:
			this->num_states = 4;
			break;
		case LANGTONS_ANT:
			this->num_states = 10;
			break;
	}

}

/*----------------------------------------------------------
                  BOARD PREFS METHODS
----------------------------------------------------------*/
uint8_t board_preferences::getNumStates() {
	return this->num_states;
}

uint8_t* board_preferences::getCharacters() {
	return this->characters;
}

color_type& board_preferences::getColors() {
	return this->colors;
}

uint32_t board_preferences::getHeight() {
	return( this->terrain.u_y - this->terrain.l_y );
}

uint32_t board_preferences::getWidth() {
	return( this->terrain.u_x - this->terrain.l_x );
}

int board_preferences::getLowerXRange() {
	return this->terrain.l_x;
}

int board_preferences::getLowerYRange() {
	return this->terrain.l_y;
}

int board_preferences::getUpperXRange() {
	return this->terrain.u_x;
}

int board_preferences::getUpperYRange() {
	return this->terrain.u_y;
}

uint8_t board_preferences::getGameType() {
	return this->gametype;
}
