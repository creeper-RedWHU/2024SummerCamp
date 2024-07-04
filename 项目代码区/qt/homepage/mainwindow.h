#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMainWindow>
#include <QScreen>  //Qt6后用QScreen代替QDesktopWidget
#include <QPushButton>
#include <QVBoxLayout>  //垂直排版组件
// #include <QGroupBox>  //搞一组按钮
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

    QPushButton * btn1;
    QPushButton * btn2;
    QPushButton * btn3;
    QPushButton * btn4;
    QPushButton * btn5;
    // QGroupBox * btngroup;  //按钮组
    QVBoxLayout* btnlayout;  //按钮的排版


    //下面这一系列是各个界面的
    QVBoxLayout* layout1;
    QVBoxLayout* layout2;
    QVBoxLayout* layout3;
    QVBoxLayout* layout4;
    QVBoxLayout* layout5;

    QLabel * hlabel1;
    QLabel * hlabel2;
    QLabel * hlabel3;
    QLabel * hlabel4;
    QLabel * hlabel5;

    QStackedWidget *stackedwidget;


private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
