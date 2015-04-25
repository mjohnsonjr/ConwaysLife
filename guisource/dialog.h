#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
    
private slots:
    void on_ZoomSpin_valueChanged(int arg1);

    void on_DelaySpin_valueChanged(int arg1);

    void on_QuitButton_clicked();

    void on_StepButton_clicked();

    void on_PlayPauseButton_toggled(bool checked);

    void on_PlayPauseButton_clicked();


private:
    Ui::Dialog *ui;
};

#endif
