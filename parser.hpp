/*
 * parser.hpp
 *
 *  Created on: May 4, 2015
 *      Author: michael
 */

#ifndef PARSER_HPP_
#define PARSER_HPP_

#include "life2.hpp"
#include <string>

class Parser {

public:
	Parser();
	virtual ~Parser();
	bool parse( std::string input );

private:
	board_preferences preferences;

};




#endif /* PARSER_HPP_ */
