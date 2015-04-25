#include "mainwindow.h"
#include "ui_mainwindow.h"

/* Constructs the main window from the given ui:  Ui::MainWindow.  The mainwindow's
 responsibility is solely for displaying, so there are no signals/slots defined here*/
MainWindow::MainWindow(QWidget *parent):QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
