#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_dialog.h"
#include "dialog.h"
#include "grid.h"
#include "globals.h"
#include "../life.h"
#include <QApplication>
#include <iostream>
#include <fstream>
#include <cerrno>



/* This program reads a .aut file passed as an argument, and displays a graphical user interface
 starting at the initial generation (gen 0).  The GUI can be controlled via the control window that
  appears when the program executes.

 See the README for more detailed information and examples.

*/



/* Prototypes */
void help();
void redrawGUI();


/* These global vars are defined here and required for other QT forms.  The only effective
 way to ensure access is to make them global via a defined header.  This is safe since this main
 function terminates after the QApplication is started, then only the dialog window has access to
 these vars */
int zoomfactor = 10;
int delay = 100;
int gennumber = 0;
bool playpausebreak = true;
QApplication *a;
MainWindow *w;
Dialog *d;
Board* gameboard;



int main(int argc, char **argv)
{

        /* Flags */
        bool helpflag = false; /* Switches set flags (-a, -g, -h)*/
        bool inputfileflag = false; /* Read from a file, or stdin.  If 1, file has been found on cmd line */

        /* C++ style string of char** argv, looks cleaner */
        string command;
        char commandc[100];

        /* The terrain size dimensions to generate (tyl -> tyh and txl -> txh) */
        int tyl = 0, tyh = 0, txl = 0, txh = 0;


        /* Flags for terrain modified switches. */
        bool b_tx = false, b_ty = false;

        /* Counters/holders */
        int i = 0;
        char* endptr;
        size_t pos;
        long hold;

        /* String of input file to use, default to stdin */
        string inputfilename;
        ifstream file;


        /* Decode Switches */
        for(i =1; i < argc; i++)
        {
            command = argv[i];

            if(command.compare("-h") == 0 && helpflag == false)  helpflag = true;

            else if(command.compare("-tx") == 0 && b_tx == false)
            {
                if(i + 1 < argc)
                {
                    command = argv[i+1];
                    strcpy(commandc, argv[i+1]);
                }
                else
                {
                    cerr << "ERROR!  -tx requires arguments.\n";
                    exit(1);
                }

                 if((pos = command.find(",")) == string::npos)
                 {
                    cerr << "Incorrect syntax for -tx.  Please see help with -h\n";
                    exit(1);
                 }
                 else
                 {
                    errno = 0;
                    /* First l */
                    hold = strtol(commandc, &endptr, 10);

                    if(endptr == commandc || hold < -2147683648 || hold > 2147683647) {cerr << commandc << ": Error on conversion of tx. Ensure it is between INT_MIN and INT_MAX.\n"; exit(1);}
                    else txl = (int)hold;

                    /* Then h */
                    command = command.substr(pos + 1);
                    strcpy(commandc, command.c_str());

                    errno = 0;
                    hold = strtol(commandc, &endptr, 10);

                    if(errno != 0 || txl > hold || endptr == commandc || hold < -2147683648 || hold > 2147683647) {cerr << commandc << ": Error on conversion of tx. Ensure txl < txh and is between INT_MIN and INT_MAX.\n"; exit(1);}
                    else txh = (int)hold;

                    b_tx = true;
                    i++;
                 }
            }

            else if(command.compare("-ty") == 0 && b_ty == false)
            {
                if(i + 1 < argc)
                {
                    command = argv[i+1];
                    strcpy(commandc, argv[i+1]);
                }
                else
                {
                    cerr << "ERROR!  -ty requires arguments.\n";
                    exit(1);
                }

                 if((pos = command.find(",")) == string::npos)
                 {
                    cerr << "Incorrect syntax for -ty.  Please see help with -h.\n";
                    exit(1);
                 }
                 else
                 {
                    errno = 0;
                    /* First l */
                    hold = strtol(commandc, &endptr, 10);

                    if(endptr == commandc || hold < -2147683648 || hold > 2147683647) {cerr << commandc << ": Error on conversion of ty. Ensure it is between INT_MIN and INT_MAX.\n"; exit(1);}
                    else tyl = (int)hold;

                    /* Then h */
                    command = command.substr(pos + 1);
                    strcpy(commandc, command.c_str());

                    errno = 0;
                    hold = strtol(commandc, &endptr, 10);

                    if(errno != 0 || tyl > hold || endptr == commandc || hold < -2147683648 || hold > 2147683647) {cerr << commandc << ": Error on conversion of ty. Ensure tyl < tyh and is between INT_MIN and INT_MAX.\n"; exit(1);}
                    else tyh = (int)hold;

                    b_ty = true;
                    i++;
                 }
            }

            else if(command.at(0) != '-' && inputfileflag == false)
            {
                inputfilename = argv[i];
                inputfileflag = true;
            }

            /* Any other unrecogznied switches. Don't use here*/
            else
            {
                cerr << "\n" << argv[i] << ": Unrecognized/duplicated switch.  Please see help screen with -h.\n\n";
                exit(1);
            }
        }
        /* Switches Done */

        /* Print the help screen */
        if(helpflag)
        {
            help();
        }

        /* Open file */
        if(inputfileflag)
        {
            file.open(inputfilename.c_str());

            if(!file.is_open())
            {
                cerr << "\nERROR! Could not open the file: " << inputfilename << ". Aborting.\n\n";
                exit(1);
            }
            /* Create a board via filestream, and check to make sure it created properly */
            gameboard = createBoard(file, txl, txh, tyl, tyh, b_tx, b_ty);
        }
        else
        {
            cerr << "\nNo input file detected!  Please specify an .aut file for generation 0. Aborting.\n\n";
            exit(1);
        }

        if(gameboard == NULL)
        {
            cerr << "\nERROR, the file is corrupt or incorrect (usually because of a missing \"Initial\" block or Xrange/Yrange declarations).\n\n";
            exit(1);
        }

        /* File opened and object created, now setup Qt components */
        a = new QApplication(argc, argv);
        w = new MainWindow;
        d = new Dialog;



        /* Set the Titles of each window based on sim name */
        QString name = QString::fromUtf8((gameboard->getName().c_str()));
        w->setWindowTitle(name);
        d->setWindowTitle(name + " Controls");



        /* Draw initial board layout */
        redrawGUI();

    /* Display both dialog window and main window */
    w->show();
    d->show();
    
    return a->exec();
}

/* Redraws the updated alive cell positions to the GUI */
void redrawGUI()
{
    /* reset all cells */
    QImage image = QImage(gameboard->width() + 1, gameboard->height() + 1, QImage::Format_ARGB32);
    image.fill(qRgba(0, 0, 0, 0));
    w->ui->grid->setIconImage(image);

    /* Keeps track of the current state of the cell */
    short state;

    for(int y = 0; y <= gameboard->height(); y++)
    {
        for(int x = 0; x <= gameboard->width(); x++)
            {
                state = gameboard->cells[x][y].getState();
                /* Cell is alive/state > 0 */
                if(state != 0)
                {
                    w->ui->grid->setPenColor( QColor( (gameboard->getColors()[state][0]), (gameboard->getColors()[state][1]), (gameboard->getColors()[state][2]) ) );
                    w->ui->grid->setImagePixel(QPoint(zoomfactor * x, zoomfactor * (gameboard->height() - y) ) , true);
                }
                /* Cell is dead/state 0 */
                else
                {
                    w->ui->grid->setPenColor( QColor( (gameboard->getColors()[0][0]), (gameboard->getColors()[0][1]), (gameboard->getColors()[0][2]) ) );
                    w->ui->grid->setImagePixel(QPoint(zoomfactor * x, zoomfactor * (gameboard->height() - y) ) , true);
                }
            }
     }
}

/* Print the sim-gui help screen and exit */
void help()
{
        cout << "\nSim-gui Help Screen:\n\n";
        cout << "\tThis program displays the provided .aut file starting at generation 0.\n";
        cout << "\tThe GUI can be controlled by using the control window.  \n\tThe window has the following options: \n";
        cout << "\tPlay/Pause.  Step one generation.  Quit.  Scrollwheel/Scrollbars: Scrolling.\n";
        cout << "\nSupported Switches:\n";
        cout << "\t-h : Display the help screen.\n";
        cout << "\t-tx l,h: Set the X terrain range to l,h.  This must be between INT_MIN and INT_MAX.\n";
        cout << "\t-ty l,h: Set the Y terrain range to l,h.  This must be between INT_MIN and INT_MAX.\n";
        cout << "\nSupported Arguments:\n";
        cout << "\tONE .aut file from the command line.\n\n";
        cout << "Please see the included README file for more detailed information.\n\n";
        exit(1);
}


