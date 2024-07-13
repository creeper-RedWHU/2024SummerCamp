#ifndef API_H
#define API_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class api : public QObject {
    Q_OBJECT

public:
    api();
    bool today_weather(const QString& city);
    bool machine_learning(const QString& city, double &data);
    bool fetch_data(const QString& city,const QString& start_year,const QString& start_month,const QString& end_year,const QString& end_month);

signals:
    void predictionReady(double data);

private slots:
    void handleToday_weatherReply();
    void handlemlReply();
    void handlefetch_dataReply();


private:
    QNetworkAccessManager* networkManager;
    double* dataPtr;
};

#endif // API_H
