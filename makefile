# Makefile for Michael Johnson's CS229 Project 2

#Compiler
CC = g++

#Compiler Flags
CFLAGS = -Wall -O2 -g
OFLAGS = -Wall -O2 -c

#Qmake Command (sometimes varies on versions/machines)
QT = qmake


all: showgen sim-tui sim-gui

showgen: showgen.o lifefunctions.o lifeclasses.o
	$(CC) $(CFLAGS) showgen.o lifefunctions.o lifeclasses.o -o showgen 

sim-tui: sim-tui.o lifefunctions.o lifeclasses.o
	$(CC) $(CFLAGS) -lncurses sim-tui.o lifefunctions.o lifeclasses.o -o sim-tui

showgen.o: showgen.cpp
	$(CC) $(OFLAGS) showgen.cpp

sim-tui.o: sim-tui.cpp
	$(CC) $(OFLAGS) sim-tui.cpp

lifefunctions.o: lifefunctions.cpp
	$(CC) $(OFLAGS) lifefunctions.cpp

lifeclasses.o: lifeclasses.cpp
	$(CC) $(OFLAGS) lifeclasses.cpp

sim-gui: lifefunctions.o lifeclasses.o
	cd guisource/; \
	$(QT) -project -o sim-gui.pro ../lifefunctions.cpp ../lifeclasses.cpp; \
	$(QT) sim-gui.pro -o Makefile; \
	make; \
	cp sim-gui ..

clean: 
	rm -f *.o
	rm showgen
	rm sim-tui
	cd guisource/; \
	make clean; \
	rm -f Makefile; \
	rm -f sim-gui.pro; \
	rm sim-gui
	rm sim-gui

tarball:
	tar -cvzf Michael-Johnson-Project-2.tar.gz README.txt makefile showgen.cpp sim-tui.cpp life.h lifefunctions.cpp lifeclasses.cpp guisource/
