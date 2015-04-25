#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "../life.h"
#include "dialog.h"

#ifndef globals_h
#define globals_h
extern int zoomfactor;
extern int delay;
extern bool playpausebreak;
extern int gennumber;
extern MainWindow *w;
extern QApplication *a;
extern Dialog *d;
extern Board *gameboard;

extern void redrawGUI();


#endif
