/********************************************************************************
** Form generated from reading UI file 'dialog.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOG_H
#define UI_DIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSlider>
#include <QtGui/QSpinBox>

QT_BEGIN_NAMESPACE

class Ui_Dialog
{
public:
    QSlider *ZoomSlider;
    QSlider *DelaySlider;
    QPushButton *QuitButton;
    QPushButton *PlayPauseButton;
    QPushButton *StepButton;
    QLabel *Zoom;
    QSpinBox *ZoomSpin;
    QSpinBox *DelaySpin;
    QLabel *Delay;
    QLabel *GenLabel;
    QLabel *GenCounter;

    void setupUi(QDialog *Dialog)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName(QString::fromUtf8("Dialog"));
        Dialog->resize(350, 250);
        Dialog->setMinimumSize(QSize(350, 250));
        Dialog->setMaximumSize(QSize(350, 250));
        Dialog->setModal(false);
        ZoomSlider = new QSlider(Dialog);
        ZoomSlider->setObjectName(QString::fromUtf8("ZoomSlider"));
        ZoomSlider->setGeometry(QRect(200, 60, 111, 22));
        ZoomSlider->setValue(16);
        ZoomSlider->setOrientation(Qt::Horizontal);
        DelaySlider = new QSlider(Dialog);
        DelaySlider->setObjectName(QString::fromUtf8("DelaySlider"));
        DelaySlider->setGeometry(QRect(200, 100, 111, 22));
        DelaySlider->setMaximum(2000);
        DelaySlider->setValue(100);
        DelaySlider->setOrientation(Qt::Horizontal);
        QuitButton = new QPushButton(Dialog);
        QuitButton->setObjectName(QString::fromUtf8("QuitButton"));
        QuitButton->setGeometry(QRect(20, 190, 80, 32));
        QuitButton->setCheckable(false);
        QuitButton->setAutoExclusive(false);
        QuitButton->setAutoDefault(false);
        PlayPauseButton = new QPushButton(Dialog);
        PlayPauseButton->setObjectName(QString::fromUtf8("PlayPauseButton"));
        PlayPauseButton->setGeometry(QRect(120, 190, 91, 32));
        PlayPauseButton->setFocusPolicy(Qt::StrongFocus);
        PlayPauseButton->setCheckable(true);
        PlayPauseButton->setChecked(false);
        PlayPauseButton->setAutoRepeat(false);
        PlayPauseButton->setAutoExclusive(false);
        PlayPauseButton->setAutoRepeatDelay(0);
        PlayPauseButton->setAutoRepeatInterval(0);
        PlayPauseButton->setAutoDefault(false);
        StepButton = new QPushButton(Dialog);
        StepButton->setObjectName(QString::fromUtf8("StepButton"));
        StepButton->setGeometry(QRect(231, 190, 80, 32));
        StepButton->setCheckable(false);
        StepButton->setAutoExclusive(false);
        StepButton->setAutoDefault(false);
        Zoom = new QLabel(Dialog);
        Zoom->setObjectName(QString::fromUtf8("Zoom"));
        Zoom->setGeometry(QRect(20, 60, 101, 20));
        ZoomSpin = new QSpinBox(Dialog);
        ZoomSpin->setObjectName(QString::fromUtf8("ZoomSpin"));
        ZoomSpin->setGeometry(QRect(130, 60, 57, 24));
        ZoomSpin->setMinimum(3);
        ZoomSpin->setValue(10);
        DelaySpin = new QSpinBox(Dialog);
        DelaySpin->setObjectName(QString::fromUtf8("DelaySpin"));
        DelaySpin->setGeometry(QRect(130, 100, 57, 24));
        DelaySpin->setMaximum(2000);
        DelaySpin->setValue(100);
        Delay = new QLabel(Dialog);
        Delay->setObjectName(QString::fromUtf8("Delay"));
        Delay->setGeometry(QRect(20, 100, 101, 20));
        GenLabel = new QLabel(Dialog);
        GenLabel->setObjectName(QString::fromUtf8("GenLabel"));
        GenLabel->setGeometry(QRect(20, 140, 101, 16));
        GenCounter = new QLabel(Dialog);
        GenCounter->setObjectName(QString::fromUtf8("GenCounter"));
        GenCounter->setGeometry(QRect(190, 140, 81, 16));

        retranslateUi(Dialog);
        QObject::connect(ZoomSpin, SIGNAL(valueChanged(int)), ZoomSlider, SLOT(setValue(int)));
        QObject::connect(ZoomSlider, SIGNAL(valueChanged(int)), ZoomSpin, SLOT(setValue(int)));
        QObject::connect(DelaySpin, SIGNAL(valueChanged(int)), DelaySlider, SLOT(setValue(int)));
        QObject::connect(DelaySlider, SIGNAL(valueChanged(int)), DelaySpin, SLOT(setValue(int)));

        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QDialog *Dialog)
    {
        Dialog->setWindowTitle(QApplication::translate("Dialog", "Dialog", 0, QApplication::UnicodeUTF8));
        QuitButton->setText(QApplication::translate("Dialog", "Quit", 0, QApplication::UnicodeUTF8));
        PlayPauseButton->setText(QApplication::translate("Dialog", "Play/Pause", 0, QApplication::UnicodeUTF8));
        StepButton->setText(QApplication::translate("Dialog", "Step", 0, QApplication::UnicodeUTF8));
        Zoom->setText(QApplication::translate("Dialog", "Zoom Factor:", 0, QApplication::UnicodeUTF8));
        Delay->setText(QApplication::translate("Dialog", "Delay (ms):", 0, QApplication::UnicodeUTF8));
        GenLabel->setText(QApplication::translate("Dialog", "Generation:", 0, QApplication::UnicodeUTF8));
        GenCounter->setText(QApplication::translate("Dialog", "0", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Dialog: public Ui_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOG_H
