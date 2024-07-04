/********************************************************************************
** Form generated from reading UI file 'todayweather.ui'
**
** Created by: Qt User Interface Compiler version 6.7.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TODAYWEATHER_H
#define UI_TODAYWEATHER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_todayweather
{
public:

    void setupUi(QWidget *todayweather)
    {
        if (todayweather->objectName().isEmpty())
            todayweather->setObjectName("todayweather");
        todayweather->resize(400, 300);

        retranslateUi(todayweather);

        QMetaObject::connectSlotsByName(todayweather);
    } // setupUi

    void retranslateUi(QWidget *todayweather)
    {
        todayweather->setWindowTitle(QCoreApplication::translate("todayweather", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class todayweather: public Ui_todayweather {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TODAYWEATHER_H
