#ifndef FORECAST_H
#define FORECAST_H

#include <QWidget>

namespace Ui {
class forecast;
}

class forecast : public QWidget
{
    Q_OBJECT

public:
    explicit forecast(QWidget *parent = nullptr);
    ~forecast();

private:
    Ui::forecast *ui;
};

#endif // FORECAST_H
