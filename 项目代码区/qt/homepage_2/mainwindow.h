#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QScreen>  //Qt6后用QScreen代替QDesktopWidget
#include <QPushButton>
#include <QVBoxLayout>  //垂直排版组件
#include <QStackedWidget>
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QPushButton *btn1;
    QPushButton *btn2;
    QPushButton *btn3;
    QPushButton *btn4;
    QPushButton *btn5;
    QVBoxLayout *btnlayout;  //按钮的排版

    // 各个界面
    QVBoxLayout *layout1;
    QVBoxLayout *layout2;
    QVBoxLayout *layout3;
    QVBoxLayout *layout4;
    QVBoxLayout *layout5;

    QLabel *hlabel1;
    QLabel *hlabel2;
    QLabel *hlabel3;
    QLabel *hlabel4;
    QLabel *hlabel5;

    QStackedWidget *stackedwidget;

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
