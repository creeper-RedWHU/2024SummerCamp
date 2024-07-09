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
    QString password = settings.value("Database/Password", "123456").toString();

    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName(hostName);
    db.setDatabaseName(databaseName);
    db.setUserName(userName);
    db.setPassword(password);

    // 尝试打开数据库连接
    if (!db.open()) {
        qDebug() << "Database error occurred:" << db.lastError().text();
        return false;
    }

    qDebug() << "Database connected successfully.";
    return true;
}
