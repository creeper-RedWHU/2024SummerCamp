/********************************************************************************
** Form generated from reading UI file 'analysis.ui'
**
** Created by: Qt User Interface Compiler version 6.7.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ANALYSIS_H
#define UI_ANALYSIS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_datavisualization
{
public:

    void setupUi(QWidget *datavisualization)
    {
        if (datavisualization->objectName().isEmpty())
            datavisualization->setObjectName("datavisualization");
        datavisualization->resize(400, 300);

        retranslateUi(datavisualization);

        QMetaObject::connectSlotsByName(datavisualization);
    } // setupUi

    void retranslateUi(QWidget *datavisualization)
    {
        datavisualization->setWindowTitle(QCoreApplication::translate("datavisualization", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class datavisualization: public Ui_datavisualization {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ANALYSIS_H
