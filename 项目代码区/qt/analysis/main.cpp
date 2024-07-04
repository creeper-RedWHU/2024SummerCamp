#include <QApplication>
#include "analysis.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow mainWindow;
    mainWindow.showMaximized(); // 窗口最大化显示
    return app.exec();
}
