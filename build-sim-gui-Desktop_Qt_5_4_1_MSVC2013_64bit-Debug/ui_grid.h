/********************************************************************************
** Form generated from reading UI file 'grid.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GRID_H
#define UI_GRID_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Grid
{
public:

    void setupUi(QWidget *Grid)
    {
        if (Grid->objectName().isEmpty())
            Grid->setObjectName(QStringLiteral("Grid"));
        Grid->setEnabled(true);
        Grid->resize(661, 491);
        Grid->setFocusPolicy(Qt::NoFocus);

        retranslateUi(Grid);

        QMetaObject::connectSlotsByName(Grid);
    } // setupUi

    void retranslateUi(QWidget *Grid)
    {
        Grid->setWindowTitle(QApplication::translate("Grid", "Form", 0));
    } // retranslateUi

};

namespace Ui {
    class Grid: public Ui_Grid {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GRID_H