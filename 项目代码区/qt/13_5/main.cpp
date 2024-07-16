#include "login_register.h"
#include "mainwindow.h"
// #include "lifeindexwidget.h"

#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    login_register w;
    w.show();
    // MainWindow demo(2);
    // demo.show();
    return a.exec();
}
