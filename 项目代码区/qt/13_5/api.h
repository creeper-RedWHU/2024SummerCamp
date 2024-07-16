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
    bool predict_fake(const QString& min_t,const QString& max_t);
    bool ml_sarima(const QString& city,QPixmap& bpicture);

signals:
    void predictionReady(double data);
    void weatherInfoReady(const QJsonObject& weatherData);
    void predictInfoReady(const QJsonValue& min_t,const QJsonValue& max_t);
    void ml_siramaReady(const QString& city);
    void imageReady(QPixmap& bpicture);

private slots:
    void handleToday_weatherReply();
    void handlemlReply();
    void handlefetch_dataReply();
    void handlepredict_fakeReply();
    void handleml_sarimaReply();





private:
    QNetworkAccessManager* networkManager;
    double* dataPtr;
};

#endif // API_H
