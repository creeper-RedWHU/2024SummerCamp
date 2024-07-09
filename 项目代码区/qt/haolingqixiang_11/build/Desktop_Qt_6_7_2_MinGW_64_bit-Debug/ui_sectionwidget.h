/********************************************************************************
** Form generated from reading UI file 'sectionwidget.ui'
**
** Created by: Qt User Interface Compiler version 6.7.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SECTIONWIDGET_H
#define UI_SECTIONWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_sectionwidget
{
public:

    void setupUi(QWidget *sectionwidget)
    {
        if (sectionwidget->objectName().isEmpty())
            sectionwidget->setObjectName("sectionwidget");
        sectionwidget->resize(400, 300);

        retranslateUi(sectionwidget);

        QMetaObject::connectSlotsByName(sectionwidget);
    } // setupUi

    void retranslateUi(QWidget *sectionwidget)
    {
        sectionwidget->setWindowTitle(QCoreApplication::translate("sectionwidget", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class sectionwidget: public Ui_sectionwidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SECTIONWIDGET_H
