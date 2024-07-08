/********************************************************************************
** Form generated from reading UI file 'forecast.ui'
**
** Created by: Qt User Interface Compiler version 6.5.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FORECAST_H
#define UI_FORECAST_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_forecast
{
public:

    void setupUi(QWidget *forecast)
    {
        if (forecast->objectName().isEmpty())
            forecast->setObjectName("forecast");
        forecast->resize(400, 300);

        retranslateUi(forecast);

        QMetaObject::connectSlotsByName(forecast);
    } // setupUi

    void retranslateUi(QWidget *forecast)
    {
        forecast->setWindowTitle(QCoreApplication::translate("forecast", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class forecast: public Ui_forecast {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FORECAST_H
