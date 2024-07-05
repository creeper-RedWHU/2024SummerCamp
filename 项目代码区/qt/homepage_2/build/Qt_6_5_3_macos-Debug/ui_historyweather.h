/********************************************************************************
** Form generated from reading UI file 'historyweather.ui'
**
** Created by: Qt User Interface Compiler version 6.5.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HISTORYWEATHER_H
#define UI_HISTORYWEATHER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_historyweather
{
public:

    void setupUi(QWidget *historyweather)
    {
        if (historyweather->objectName().isEmpty())
            historyweather->setObjectName("historyweather");
        historyweather->resize(400, 300);

        retranslateUi(historyweather);

        QMetaObject::connectSlotsByName(historyweather);
    } // setupUi

    void retranslateUi(QWidget *historyweather)
    {
        historyweather->setWindowTitle(QCoreApplication::translate("historyweather", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class historyweather: public Ui_historyweather {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HISTORYWEATHER_H
