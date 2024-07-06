#include "mainwindow.h"
#include "login_register.h"

#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");// 建立和QSQLITE数据库的连接
    db.setHostName("127.0.0.1");  //连接本地主机
    db.setPort(3306);
    db.setUserName("root");
    //设置数据库的密码
    db.setPassword("mt127715318");    //这个就是安装MySQL时设置的密码
    db.setDatabaseName("main");//设置数据库名称
    if(db.open())
    {
        qDebug() << "QSLite数据库连接成功!";
    }
    else
    {
        qDebug() << "QSLite数据库连接失败!";
    }

     login_register w;
     w.show();
    return a.exec();


}
