#ifndef TODAYWEATHER_H
#define TODAYWEATHER_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QPushButton>
#include <QDate>
#include <QString>
#include <QDateTime>
#include <QStackedWidget>
#include <QtCharts>
#include "sectionwidget.h"

namespace Ui {
class todayweather;
}

class todayweather : public QWidget
{
    Q_OBJECT

public:
    explicit todayweather(QWidget *parent = nullptr);
    ~todayweather();

    QLineSeries *series;  //数据系列气温
    QScatterSeries *scatterSeries;//气温
    QChartView *chartview;
    QChart *chart;
    QComboBox *cityBox;  // 城市的组合框控件
    QPushButton *btn;    // 查询按钮

    QLabel *weatherLabel;  // 天气标签


    QSqlDatabase db;       // 数据库

    QVBoxLayout *mainLayout;


    void search();
    void showweather(QString maxt, QString mint, QString weather, QString wind_d, QString wind_s);
    void drawWeather(const QString &weatherType);
    void clearWeatherDisplay();
    void getweather(int month, int day, int hour, QString &temp, QString &hum, QString &wea, QString &wd, QString &ws);
    void updateTooltip(QPointF point, bool state);
private:
    Ui::todayweather *ui;
    QStackedWidget *weatherwidget;  // 用于切换不同天气显示的堆栈控件
};

#endif // TODAYWEATHER_H
