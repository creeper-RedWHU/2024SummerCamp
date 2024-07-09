#ifndef HISTORYWEATHER_H
#define HISTORYWEATHER_H

#include <QWidget>
#include <QComboBox>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTableView>
#include <QStandardItemModel>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include "link_py.h"


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
    link_py *linkpy;


    void showweather();

private:
    Ui::historyweather *ui;
    QSqlDatabase db;
    void initDB();
};

#endif // HISTORYWEATHER_H
