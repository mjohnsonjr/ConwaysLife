README FILE: The Game of Life (Project 2):  Created by Michael Johnson

Programs: showgen, sim-tui, sim-gui


IMPORTANT:  User messages are pushed to stderr.  If you really want to redirect those, go for it.  
			(To redirect stdout, use > or 1>.  For stderr, use 2> in the unix terminal).

Showgen:
	
	This program reads a .aut file passed as an argument, and generates to gen N (see -g switch) according to Conway's
	game of life rules to standard output.  This program only writes to standard output, to redirect this output to a
	file use "> filename" on the command line.  Showgen can either create an ASCII representation of the output, or it
	can generate a new .aut file (see -a switch) for generation N.


	Switches: 
		-h : opens the help screen
		-g n : generates the game to generation n (n value defaults to 0 if not specified)
		-a : output should be in .aut format rather than an ASCII representation
		-tx l,h : Set the X terrain range to l,h.  This must be between INT_MIN and INT_MAX.
		-ty l,h : Set the Y terrain range to l,h.  This must be between INT_MIN and INT_MAX.
		-wx l,h : Set the X window range to l,h.  This must be between INT_MIN and INT_MAX.
		-wy l,h : Set the Y window range to l,h.  This must be between INT_MIN and INT_MAX.

	Arguments:
		One .aut file representing the initial generation (gen 0).

	Note:  This program also supports reading a .aut file from standard input.  To do this, use "< filename" on the command line. 
	
	Example uses:
		./showgen -g 5 test.aut -a : Outputs a .aut file containing the information for generation 5 of the test.aut file.
		./showgen -g 5 test.aut : Outputs an ASCII representation for generation 5 of the test.aut file.



Sim-tui:
	
	This program reads a .aut file passed as an argument, and displays a textual user interface starting at the initial
	 generation (gen 0).  The TUI can be controlled according to the controls listed below, and the controls are also
	 printed at the bottom of the TUI.


	Switches: 
		-h : opens the help screen
		-tx l,h : Set the X terrain range to l,h.  This must be between INT_MIN and INT_MAX.  THIS OVERRIDES THE .aut VALUES.
		-ty l,h : Set the Y terrain range to l,h.  This must be between INT_MIN and INT_MAX.  THIS OVERRIDES THE .aut VALUES.

	Arguments:
		One .aut file representing the initial generation (gen 0).

	Example uses:
		./sim-tui test.aut : Sets up the TUI with the initialization described in test.aut.
		./sim-tui test.aut -tx 1,2 -ty 0,5 : Sets up the TUI with the initialization described in test.aut, but the X and Y ranges listed in test.aut are overridden by our command line arguments.

	Controls:
		P: Play/Pause.  
		S: Step one generation. 
		Q: Quit.  
		Arrow Keys: Scrolling. 
		+/-: Change Delay Time


Sim-gui:
	
	This program reads a .aut file passed as an argument, and displays a graphical user interface starting at the initial
	 generation (gen 0).  The GUI can be controlled via the control window that appears when the program executes.

	Switches: 
		-h : opens the help screen
		-tx l,h : Set the X terrain range to l,h.  This must be between INT_MIN and INT_MAX.  THIS OVERRIDES THE .aut VALUES.
		-ty l,h : Set the Y terrain range to l,h.  This must be between INT_MIN and INT_MAX.  THIS OVERRIDES THE .aut VALUES.

	Arguments:
		One .aut file representing the initial generation (gen 0).

	Example uses:
		./sim-gui test.aut : Sets up the GUI with the initialization described in test.aut.
		./sim-gui test.aut -tx 1,2 -ty 0,5 : Sets up the GUI with the initialization described in test.aut, but the X and Y ranges listed in test.aut are overridden by our command line arguments.

	Controls:
		All GUI controls are done via the control window that appears.  This window contains buttons that will Step 
		generations, Play/Pause, and Quit.  All of these are self explanitory and do the same thing as the TUI.  The GUI
		also has a slider for Zoom Factor and for delay.  Delay here works the same as the TUI, in Play mode it is how 
		long we wait between generations.  The Zoom factor is a GUI specific function that allows the user to zoom 
		in/out of the GUI.  This will make the cells either larger or smaller.



.aut File Format:

	IMPORTANT: When reading an .aut file please conform to the following order if possible:
		Name before Initial statement.
		Xrange/Yrange before initial statement.
		Rules keyword AFTER Xrange/Yrange statement.
		Chars/Colors statements after Rules statements.

		IF YOU DO NOT CONFORM TO THIS:  The program will fall back to its defaults and WILL warn you.  
		The program will still run, though.  But it may not be what you expect because of the incorrect
		.aut file syntax.


Gametypes:
	
	Conway's Game of Life:

		Using the keyword "Rules ConwaysLife" or no Rules keyword at all will activate Conway's game of life.  These are
		 the rules for CGOL:
			(a) if there are fewer than two alive neighbors in generation i, then the cell will be dead in genera-
				tion i + 1 (from loneliness).
			(b) if there are two or three alive neighbors in generation i, then the cell will be alive in generation i+1.
			(c) if there are more than three alive neighbors in generation i, then the cell will be dead in genera-
			tion i + 1 (from over-population).
			(d) if there are exactly three alive neighbors in generation i, then the cell will be alive in generation i+1
			(from reproduction); otherwise, it will be dead.


	Brian's Brain:

		Using the keyword "Rules BriansBrain" will activate Brian's Brain.  These are the state transitions for BB:

			1. Off -> On, if exactly two neighbors are in state "on";
			2. Dying -> Off.
			3. On -> Dying.


	Wire World:

		Using the keyword "Rules WireWorld" will activate Wire World.  These are the state transitions for WW:

			1. Empty -> Empty
			2. Electron head -> Electron tail
			3. Electron tail -> Wire
			4. Wire -> Electron head if 1 or 2 neighbors are also electron heads, Wire otherwise.


	Langton's Ant:

		Using the keyword "Rules LangtonsAnt" will activate Langton's Ant.  These are the state transitions for LA:	

			1. If the cell is white, turn right (e.g., if facing north, change to east), otherwise turn left.
			2. Change the color of the cell.
			3. Move forward one step (based on the direction it is facing).	


	Other .aut File Keywords:

		Name "Simulation Name"; : Simulation Name will be the title of the simulation for each GUI/TUI.
		
		Chars 32, 64; : ASCII chars 32 will be for state 0, ASCII 64 will be for state 1, etc. (up to 10 states);
		
		Colors (255,255,255); GUI cell colors will be these RGB values.  First is state 0, then state 1...etc.

		Rules GameMode; : Sets up the specified simulation type, explained above.

		Xrange LOW HIGH;  Where LOW is the lowerbound for X, and HIGH is the upperbound for X.

		Yrange LOW HIGH;  Where LOW is the lowerbound for Y, and HIGH is the upperbound for Y.

		Initial Block: Initializes cells for Generation 0.  The State n keyword will change all future cells specified to to state n.

		Initial {
		# Set these to state 1
			Y= 3: -9;
			Y=-3: -9;
		State 2; # The following are set to state 2
			Y= 3: -10;
			Y=-3: -10;
		State 3; # The following are set to state 3
			Y= 3: -8, -7, -6, -5, -4, -3, -2, -1, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10;
			Y= 2: -1, 0;
			Y= 4: -1, 0;
			Y=-3: -8, -7, -6, -5, -4, -3, -2, -1, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10;
			Y=-2: 0, 1;
			Y=-4: 0, 1;
		};




Example .aut File:

# This is an example aut file.
Xrange -30 30;
Yrange -30 30;
Rules LangtonsAnt;
Chars 32, 35, 43, 43, 43, 43, 43, 43, 43, 43;
Colors (255, 255, 255), (64, 64, 64), (255, 63, 63), (255, 63, 63), (255, 63, 63), (255, 63, 63), (255, 63, 63), (255, 63, 63), (255, 63, 63), (255, 63, 63);
Initial {
 Y = -4: 0;
 Y = -3: -1, 1;
 Y = -2: -2;
 State 2;
 Y = -2: 0;
 State 1;
 Y = -2: 2;
 Y = -1: -3, -1, 1, 3;
 Y = 0: -4;
 State 5;
 Y = 0: -2;
 State 1;
 Y = 0: 0;
 State 3;
 Y = 0: 2;
 State 1;
 Y = 0: 4;
 Y = 1: -3, -1, 1, 3;
 Y = 2: -2;
 State 4;
 Y = 2: 0;
 State 1;
 Y = 2: 2;
 Y = 3: -1, 1;
 Y = 4: 0;
};






