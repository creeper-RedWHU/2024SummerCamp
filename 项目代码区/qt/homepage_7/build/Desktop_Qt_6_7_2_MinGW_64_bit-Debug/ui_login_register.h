/********************************************************************************
** Form generated from reading UI file 'login_register.ui'
**
** Created by: Qt User Interface Compiler version 6.7.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGIN_REGISTER_H
#define UI_LOGIN_REGISTER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_login_register
{
public:

    void setupUi(QWidget *login_register)
    {
        if (login_register->objectName().isEmpty())
            login_register->setObjectName("login_register");
        login_register->resize(400, 300);

        retranslateUi(login_register);

        QMetaObject::connectSlotsByName(login_register);
    } // setupUi

    void retranslateUi(QWidget *login_register)
    {
        login_register->setWindowTitle(QCoreApplication::translate("login_register", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class login_register: public Ui_login_register {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGIN_REGISTER_H
