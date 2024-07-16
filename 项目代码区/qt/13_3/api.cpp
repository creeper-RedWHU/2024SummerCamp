#include "api.h"
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

// 构造函数，初始化网络管理器
api::api() {
    networkManager = new QNetworkAccessManager(this);
}


// 获取今天的天气信息
// today_weather
// 参数为 city = request.args.get('city')
// 返回值为return jsonify({'success': True, 'message': 'successfully'})
// 返回值: 成功返回 true
bool api::today_weather(const QString& city) {
    // 检查networkManager是否已经初始化
    if (!networkManager) {
        qDebug() << "networkManager未初始化";
        return false;
    }

    // 设置请求URL
    const QUrl url("http://60.205.232.122:5000/today_weather");
    QNetworkRequest request(url);

    // 设置请求头为JSON格式
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // 创建JSON对象并设置城市名称
    QJsonObject json;
    json["city"] = city;

    // 发送POST请求并获取回复
    QNetworkReply* reply = networkManager->post(request, QJsonDocument(json).toJson());

    // 连接信号槽，处理回复
    connect(reply, &QNetworkReply::finished, this, &api::handleToday_weatherReply);

    return true;
}

// 处理天气信息的回复
void api::handleToday_weatherReply() {
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());

    if (reply->error() == QNetworkReply::NoError) {
        QByteArray response_data = reply->readAll();
        QJsonDocument jsonResponse = QJsonDocument::fromJson(response_data);

        if (!jsonResponse.isNull()) {
            QJsonObject jsonObject = jsonResponse.object();
            bool success = jsonObject["success"].toBool();

            if (success) {
                qDebug() << "today_api调用成功";
                // 这里可以处理具体的天气数据
                QJsonObject dataObject = jsonObject["data"].toObject();
                // 发射信号通知天气数据
                emit weatherInfoReady(dataObject);
            } else {
                qDebug() << "调用失败1 ";
            }
        } else {
            qDebug() << "解析JSON失败";
        }
    } else {
        QString errorString = reply->errorString();
        qDebug() << "调用失败: " << errorString;
    }

    reply->deleteLater();
}




// 机器学习的API端口
// ml
// 参数 city: 城市名称
// 返回值: 成功返回 true
bool api::machine_learning(const QString& city, double &data)
{
    // 设置请求URL
    QNetworkRequest request(QUrl("http://60.205.232.122:5000/ml"));
    // 设置请求头为JSON格式
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // 创建JSON对象并设置参数
    QJsonObject json;
    json["city"] = city;

    // 将data的地址存储到成员变量中，以便在handlemlReply中使用
    dataPtr = &data;

    // 发送POST请求并获取回复
    QNetworkReply* reply = networkManager->post(request, QJsonDocument(json).toJson());
    // 连接信号槽，处理回复
    connect(reply, &QNetworkReply::finished, this, &api::handlemlReply);
    return true;
}

// 处理机器学习API的回复
void api::handlemlReply() {
    // 获取发送请求的对象
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    double prediction = 0.0;

    // 检查是否有错误
    if (reply->error() == QNetworkReply::NoError) {
        // 读取回复并解析为JSON文档
        QJsonDocument jsonResponse = QJsonDocument::fromJson(reply->readAll());
        QJsonObject jsonObject = jsonResponse.object();

        // 提取预测值
        if (jsonObject.contains("predictions")) {
            QJsonArray predictionsArray = jsonObject["predictions"].toArray();

            // 检查二维数组是否有效
            if (predictionsArray.size() > 0 && predictionsArray[0].isArray()) {
                QJsonArray innerArray = predictionsArray[0].toArray();

                // 确保内层数组包含一个元素
                if (innerArray.size() > 0) {
                    prediction = innerArray[0].toDouble();
                } else {
                    qDebug() << "内层数组为空";
                }
            } else {
                qDebug() << "返回的预测值格式不正确";
            }
        } else {
            qDebug() << "返回的JSON不包含预测值";
        }
        // 打印结果
        qDebug() << "predictions:" << prediction;
    } else {
        // 如果请求过程中有错误，打印错误信息
        QString errorString = reply->errorString();
        qDebug() << "调用失败: " << errorString;
    }
    // 发射信号通知预测值
    emit predictionReady(prediction);

    // 清理回复对象
    reply->deleteLater();
}





// fetch_data
// 参数 city: 城市名称
// start_year 开始年
// start_month 开始月
// end_year 结束年
// end_month 结束月
// 返回值: 成功返回 true
bool api::fetch_data(const QString& city,const QString& start_year,const QString& start_month,const QString& end_year,const QString& end_month)
{
    // 设置请求URL
    QNetworkRequest request(QUrl("http://60.205.232.122:5000//fetch_weather"));
    // 设置请求头为JSON格式
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // 创建JSON对象并设置城市名称
    QJsonObject json;
    json["city"] = city;
    json["start_year"]=start_year;
    json["start_month"]=start_month;
    json["end_year"]=end_year;
    json["end_month"]=end_month;


    // 发送POST请求并获取回复
    QNetworkReply* reply = networkManager->post(request, QJsonDocument(json).toJson());
    // 连接信号槽，处理回复
    connect(reply, &QNetworkReply::finished, this, &api::handlefetch_dataReply);

    return true;
}

// 处理天气信息的回复
void api::handlefetch_dataReply()
{
    // 获取发送请求的对象
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    // 检查是否有错误
    if (reply->error() == QNetworkReply::NoError) {
        // 读取回复并解析为JSON文档
        QJsonDocument jsonResponse = QJsonDocument::fromJson(reply->readAll());
        QJsonObject jsonObject = jsonResponse.object();
        // 获取成功标志
        bool success = jsonObject["success"].toBool();
        if (success) {
            // 如果成功，打印成功信息
            qDebug() << "fetch_data调用成功";
        } else {
            // 如果失败，打印错误信息
            QString errorMessage = jsonObject["error_message"].toString();
            qDebug() << "调用失败: " << errorMessage;
        }
    } else {
        // 如果请求过程中有错误，打印错误信息
        QString errorString = reply->errorString();
        qDebug() << "调用失败: " << errorString;
    }
    // 清理回复对象
    reply->deleteLater();
}

// 获取模拟数据
// predict_fake
// 参数为两个温度
// 返回值为 true 表示成功
bool api::predict_fake(const QString& min_t, const QString& max_t) {
    // 检查 networkManager 是否已经初始化
    if (!networkManager)
    {
        qDebug() << "networkManager 未初始化";
        return false;
    }
    // 设置请求 URL
    const QUrl url("http://60.205.232.122:5000/ml_fake");
    QNetworkRequest request(url);

    // 设置请求头为 JSON 格式
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // 创建 JSON 对象并设置温度值
    QJsonObject json;
    json["max_temperature"] = max_t;
    json["min_temperature"] = min_t;

    // 发送 POST 请求并获取回复
    QNetworkReply* reply = networkManager->post(request, QJsonDocument(json).toJson());

    // 连接信号槽，处理回复
    connect(reply, &QNetworkReply::finished, this, &api::handlepredict_fakeReply);

    return true;
}

// 处理 predict_fake 的回复
void api::handlepredict_fakeReply() {
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());

    if (reply->error() == QNetworkReply::NoError) {
        QByteArray response_data = reply->readAll();
        QJsonDocument jsonResponse = QJsonDocument::fromJson(response_data);

        if (!jsonResponse.isNull()) {
            QJsonObject jsonObject = jsonResponse.object();
            bool success = jsonObject["success"].toBool();

            if (success) {
                qDebug() << "predict_fake 调用成功";
                // 处理具体的数据（这里假设返回的 JSON 结构）
                QJsonValue min_temp = jsonObject["min_temp"];
                QJsonValue max_temp = jsonObject["max_temp"];
                // 发射信号通知数据已准备好
                emit predictInfoReady(min_temp, max_temp);
            }
            else {
                QString message = jsonObject["message"].toString();
                qDebug() << "2";
                qDebug() << "调用失败: " << message;
            }
        } else {
            qDebug() << "3";
            qDebug() << "解析 JSON 失败";
        }
    }
    else {
        QString errorString = reply->errorString();
        qDebug() << "1";
        qDebug() << "调用失败: " << errorString;
    }

    reply->deleteLater();
}


//sarima算法接口，替代机器学习ml
//接口名为/sarima 传递参数为城市名
//返回布尔类型的返回值sarima
bool api::ml_sarima(const QString& city)
{
    // 检查networkManager是否已经初始化
    if (!networkManager) {
        qDebug() << "networkManager未初始化";
        return false;
    }

    // 设置请求URL
    const QUrl url("http://60.205.232.122:5000/sarima");
    QNetworkRequest request(url);

    // 设置请求头为JSON格式
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // 创建JSON对象并设置城市名称
    QJsonObject json;
    json["city"] = city;

    // 发送POST请求并获取回复
    QNetworkReply* reply = networkManager->post(request, QJsonDocument(json).toJson());

    // 连接信号槽，处理回复
    connect(reply, &QNetworkReply::finished, this, &api::handleml_sarimaReply);

    return true;
}

// 处理天气信息的回复
void api::handleml_sarimaReply() {
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());

    if (reply->error() == QNetworkReply::NoError) {
        QByteArray response_data = reply->readAll();
        QJsonDocument jsonResponse = QJsonDocument::fromJson(response_data);

        if (!jsonResponse.isNull()) {
            QJsonObject jsonObject = jsonResponse.object();
            bool success = jsonObject["success"].toBool();

            if (success) {
                qDebug() << "sarima_api调用成功";
                // 这里可以处理具体的天气数据
                QJsonObject dataObject = jsonObject["data"].toObject();
                // 发射信号通知天气数据
                emit weatherInfoReady(dataObject);
            } else {
                qDebug() << "调用失败1 ";
            }
        } else {
            qDebug() << "解析JSON失败";
        }
    } else {
        QString errorString = reply->errorString();
        qDebug() << "调用失败: " << errorString;
    }

    reply->deleteLater();
}
