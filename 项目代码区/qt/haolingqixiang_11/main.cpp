#include "login_register.h"
#include "sectionwidget.h"

#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    login_register w;
    w.show();
    // SectionWidget test("晴","23","21","西风","微风");
    // test.show();

    // w.show();
    return a.exec();
}
