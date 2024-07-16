#include "database.h"
#include <QSettings>

// 连接数据库
bool database::connectToDatabase()
{
    // 检查驱动程序是否已加载
    if (!QSqlDatabase::isDriverAvailable("QMYSQL")) {
        qDebug() << "MySQL driver not available.";
        return false;
    }

    // 加载数据库配置
    QSettings settings("config.ini", QSettings::IniFormat);
    QString hostName = settings.value("Database/HostName", "60.205.232.122").toString();
    QString databaseName = settings.value("Database/DatabaseName", "data").toString();
    QString userName = settings.value("Database/UserName", "root").toString();
    QString password = settings.value("Database/Password", "QAZ123wsx").toString();

    QString connectionName = "unique_connection_name";

    // 如果连接已经存在，则获取现有连接
    if (QSqlDatabase::contains(connectionName)) {
        db = QSqlDatabase::database(connectionName);
    } else {
        db = QSqlDatabase::addDatabase("QMYSQL", connectionName);
        db.setHostName(hostName);
        db.setDatabaseName(databaseName);
        db.setUserName(userName);
        db.setPassword(password);
    }

    // 尝试打开数据库连接
    if (!db.open()) {
        qDebug() << "Database error occurred:" << db.lastError().text();
        return false;
    }

    qDebug() << "Database connected successfully.";
    return true;
}

// // 关闭和移除数据库连接
// void database::disconnectDatabase()
// {
//     QString connectionName = "unique_connection_name";
//     if (QSqlDatabase::contains(connectionName)) {
//         QSqlDatabase db = QSqlDatabase::database(connectionName);
//         if (db.isOpen()) {
//             db.close();
//         }
//         QSqlDatabase::removeDatabase(connectionName);
//     }
// }
