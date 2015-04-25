#include "dialog.h"
#include "ui_dialog.h"
#include "globals.h"
#include "../life.h"
#include "globals.h"
#include <cstdlib>
#include <unistd.h>

/* This form, Dialog, is the controls menu that displays and has user controls for sim-gui.
 All of the Slots contained in Dialog are described below */

Dialog::Dialog(QWidget *parent):QDialog(parent),ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}

/* Sets the current Zoom based on spinbox value */
void Dialog::on_ZoomSpin_valueChanged(int arg1)
{
    zoomfactor = arg1;
    w->ui->grid->setZoomFactor(zoomfactor);
}

/* Sets the current delay (ms) based on spinbox */
void Dialog::on_DelaySpin_valueChanged(int arg1)
{
     delay = arg1;
}

/* Quit button handler, closes all windows and exits the program */
void Dialog::on_QuitButton_clicked()
{
    this->ui->PlayPauseButton->toggle();
    w->close();
    this->close();
}

/* Handles the step button being clicked. */
void Dialog::on_StepButton_clicked()
{
    gameboard->generate(1);
    d->ui->GenCounter->setText(QString::number(++gennumber));
    redrawGUI();
}

/* Sets the flag to indicate whether the playpause button has been depressed */
void Dialog::on_PlayPauseButton_toggled(bool checked)
{
    if(checked) playpausebreak = true;
    else playpausebreak = false;
}

void Dialog::on_PlayPauseButton_clicked()
{
    int i;
    while(playpausebreak)
    {

        /* Allows user input between generations in Play/pause mode. */
        for(i = 0; i < delay; i+=10)
        {
            usleep(9000);
            a->processEvents();
        }

         gameboard->generate(1);
         d->ui->GenCounter->setText(QString::number(++gennumber));
         redrawGUI();
         a->processEvents();
    }
}

