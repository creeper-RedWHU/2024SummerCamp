#ifndef TODAYWEATHER_H
#define TODAYWEATHER_H

#include <QWidget>
#include <QLabel>
#include <QPixmap>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPainter>
#include <QFont>
#include <QComboBox>  // 组合框控件
#include <QSqlDatabase>  // 数据库控件
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QPushButton>
#include <QDate>
#include <QString>

namespace Ui {
class todayweather;
}

class todayweather : public QWidget
{
    Q_OBJECT

public:
    explicit todayweather(QWidget *parent = nullptr);
    ~todayweather();

    QComboBox *cityBox;  // 城市的组合框控件
    QPushButton * btn;  // 查询按钮

    QLabel *weatherLabel;  // 天气标签
    QLabel *weatherLabel1;  // 天气标签
    QLabel *maxtLabel;  // 最大温度标签
    QLabel *mintLabel;  // 最低温度标签
    QLabel *winddLabel;  // 风向标签
    QLabel *windsLabel;  // 风力标签

    QSqlDatabase db;  // 数据库

    QVBoxLayout *mainLayout;
    QHBoxLayout *topLayout;

    void search();
    bool connectToDatabase();  // 连接数据库
    void showweather(QString maxt, QString mint, QString weather, QString wind_d, QString wind_s);
    void drawWeather(const QString &weatherType);

private:
    Ui::todayweather *ui;
};

#endif // TODAYWEATHER_H
