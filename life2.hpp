/*
 * life2.hpp
 *
 *  Created on: May 3, 2015
 *      Author: michael
 */

#ifndef LIFE2_HPP_
#define LIFE2_HPP_

#include <vector>
#include <iostream>

/*----------------------------------------------------------
                          MACROS
----------------------------------------------------------*/
#define MAX_STATES 10 /* maximum number of cell states */

/*----------------------------------------------------------
                          TYPES
----------------------------------------------------------*/
typedef unsigned char uint8_t;
typedef unsigned short  uint16_t;
typedef unsigned int  uint32_t;
typedef unsigned long long  uint64_t;

typedef uint8_t game_type;    /* Game type (conways, wireworld, etc.) */
enum{
	CONWAYS_LIFE,
	BRIANS_BRAIN,
	WIRE_WORLD,
	LANGTONS_ANT
};

enum{
	COLOR_RED = 0,
	COLOR_GREEN = 1,
	COLOR_BLUE = 2,
	NUM_COLORS
};

enum{
	STATE_0 = 0,
	STATE_1,
	STATE_2,
	STATE_3,
	STATE_4,
	STATE_5,
	STATE_6,
	STATE_7,
	STATE_8,
	STATE_9,
	NUM_STATES
};

typedef struct {
	int x;
	int y;

} cartesian;

typedef struct {
	int l_x;
	int l_y;
	int u_x;
	int u_y;

} cartesian_range;

class board_preferences {
public:
	board_preferences( std::string _name, game_type _gametype, cartesian_range _terrain );
	~board_preferences();
	uint8_t getNumStates();
	uint8_t* getCharacters();
	uint8_t** getColors();
	uint32_t getHeight();
	uint32_t getWidth();
	int getLowerXRange();
	int getLowerYRange();
	int getUpperXRange();
	int getUpperYRange();


private:
	std::string& name; /* simulation name */
	game_type gametype;  /* gametype */
	uint8_t characters[NUM_STATES];/* characters */
	uint8_t colors[NUM_STATES][NUM_COLORS];/* colors */
	cartesian_range terrain;
	const uint8_t num_states;

};

/*----------------------------------------------------------
                         CLASSES
----------------------------------------------------------*/
class Cell {                 /* Cell object type */

public:
	Cell( uint8_t _state ); /* Constructor */
	virtual ~Cell(); /* Destructor */

	/* Methods */
	uint8_t getState();
	void setState( uint8_t _state );

private:
	uint8_t state;
};

class Board {                /* board object type */

private:
	std::string name;
	board_preferences preferences;
	std::vector<std::vector<Cell>> cells;

public:
	Board( board_preferences& _preferences );
	virtual ~Board();

	void generateAut( std::ostream& output );
	void generateASCII( std::ostream& output, cartesian_range& window );
};

/*----------------------------------------------------------
                        PROCEDURES
----------------------------------------------------------*/


#endif /* LIFE2_HPP_ */
