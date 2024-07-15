#ifndef FORECAST_H
#define FORECAST_H

#include <QWidget>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QPixmap>
#include <QIntValidator>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QtCharts>

class forecast : public QWidget
{
    Q_OBJECT

public:
    explicit forecast(QWidget *parent = nullptr);

    QLineSeries *series1;  // 数据系列最高气温
    QScatterSeries *scatterSeries1;  // 最高气温
    QScatterSeries *predictionSeries;  // 预测点
    QSqlDatabase db;
    QChartView *chartview1;
    QChart *chart1;
    QLabel *imageLabel;

    void updateTooltip4(QPointF, bool);
    void draw(double prediction);  // 修改为接受预测值作为参数
    ~forecast();

private slots:
    void onSelectCityClicked();
    void onPredictClicked();
    void onPredictionReady(double data);

private:
    QComboBox *cityBox;
    QPushButton *btnPredict;
    QLabel *imagePlaceholder;
};

#endif // FORECAST_H
