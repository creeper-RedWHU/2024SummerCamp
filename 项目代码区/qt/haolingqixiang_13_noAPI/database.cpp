#include "database.h"
#include <QSettings>

// 连接数据库
bool database::connectToDatabase()
{
    // // 检查驱动程序是否已加载
    // if (!QSqlDatabase::isDriverAvailable("QMYSQL")) {
    //     qDebug() << "MySQL driver not available.";
    //     return false;
    // }

    // // 加载数据库配置
    // QSettings settings("config.ini", QSettings::IniFormat);
    // QString hostName = settings.value("Database/HostName", "60.205.232.122").toString();
    // // QString hostName = settings.value("Database/HostName", "localhost").toString();
    // QString databaseName = settings.value("Database/DatabaseName", "data").toString();
    // QString userName = settings.value("Database/UserName", "root").toString();
    // QString password = settings.value("Database/Password", "QAZ123wsx").toString();

    // db = QSqlDatabase::addDatabase("QMYSQL");
    // db.setHostName(hostName);
    // db.setDatabaseName(databaseName);
    // db.setUserName(userName);
    // db.setPassword(password);


    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");// 建立和QSQLITE数据库的连接
    db.setHostName("127.0.0.1");  //连接本地主机
    db.setPort(3306);
    db.setUserName("root");
    //设置数据库的密码
    db.setPassword("mt127715318");    //这个就是安装MySQL时设置的密码
    db.setDatabaseName("/Users/motao/demo1.db");//设置数据库名称

    // 尝试打开数据库连接
    if (!db.open()) {
        qDebug() << "Database error occurred:" << db.lastError().text();
        return false;
    }

    qDebug() << "Database connected successfully.";
    return true;
}
