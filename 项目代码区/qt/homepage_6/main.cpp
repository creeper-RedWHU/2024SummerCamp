#include "mainwindow.h"
#include "login_register.h"

#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    login_register w;
    w.show();
    return a.exec();
}
