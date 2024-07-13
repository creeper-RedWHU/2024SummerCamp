#include "login_register.h"
#include "sectionwidget.h"
#include "mainwindow.h"

#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // login_register w;
    // w.show();
    // SectionWidget test("晴转阴","21","西风","微风","湿度信息");
    // test.show();
    MainWindow demo(2);
    demo.show();
    return a.exec();
}
