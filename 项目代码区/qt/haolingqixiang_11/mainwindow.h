#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QScreen>
#include <QPushButton>
#include <QVBoxLayout>
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
    explicit MainWindow(int userIdentity, QWidget *parent = nullptr);
    ~MainWindow();

    QPushButton *btn1;
    QPushButton *btn2;
    QPushButton *btn3;
    QPushButton *btn4;
    QPushButton *btn5;
    QVBoxLayout *btnlayout;

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
    int userIdentity;
};

#endif // MAINWINDOW_H
