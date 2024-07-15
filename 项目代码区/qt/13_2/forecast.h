#ifndef FORECAST_H
#define FORECAST_H

#include <QWidget>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>

class forecast : public QWidget
{
    Q_OBJECT

public:
    explicit forecast(QWidget *parent = nullptr);
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
