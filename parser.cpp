/*
 * parser.cpp

 *
 *  Created on: May 4, 2015
 *      Author: michael
 */

#include <boost/tokenizer.hpp>
#include "life2.hpp"

/*----------------------------------------------------------
             PARSER CONNSTRUCTORS/DESTRUCTORS
----------------------------------------------------------*/
Parser::Parser() {
	/* Default options */
	cartesian_range cart;
	cart.l_x = -10;
	cart.l_y = -10;
	cart.u_x = 10;
	cart.u_y = 10;
	this->preferences = board_preferences( "Unnamed Simulation", CONWAYS_LIFE, cart );
}

Parser::~Parser() {

}

/*----------------------------------------------------------
                     PARSER METHODS
----------------------------------------------------------*/
bool Parser::parse( std::string input ) {

	/* Tokenize based on spaces */
	boost::char_separator< char > separator( " \t\n", 3 );
	boost::tokenizer< boost::char_separator< char > > tokens( input, separator );

	for( std::string& token : tokens ) {
		/* Convert to lowercase */
		std::transform(token.begin(), token.end(), token.begin(), ::tolower);
		switch( token ) {

			case "Chars":
				//some callback handler
				break;

			case "Colors":
				break;
		}
	}

	return true;
}

namespace {

bool handleChars( boost::tokenizer< boost::char_separator< char > >& tokens ) {


}

}

