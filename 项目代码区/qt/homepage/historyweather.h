#ifndef HISTORYWEATHER_H
#define HISTORYWEATHER_H

#include <QWidget>
#include <QComboBox>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTableView>
#include <QStandardItemModel>

namespace Ui {
class historyweather;
}

class historyweather : public QWidget
{
    Q_OBJECT

public:
    explicit historyweather(QWidget *parent = nullptr);
    ~historyweather();


    QComboBox *cityBox,*yearBox,*monthBox;  //三个选择栏
    QPushButton *btn;  //查询按钮
    QTableView *weathertable;  //天气表
    QStandardItemModel *model;

    void showweather();

private:
    Ui::historyweather *ui;
};

#endif // HISTORYWEATHER_H
