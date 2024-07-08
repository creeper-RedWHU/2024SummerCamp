#include "link_py.h"
#include <QDebug>

link_py::link_py(QObject *parent)
    : QObject(parent),
    process(new QProcess(this))
{
    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, &link_py::onProcessFinished);
}

void link_py::fetchData(const QString &ct, int startYear, int startMonth, int endYear, int endMonth)
{
    // QString pythonExecutable = "D:/Python/python.exe";
    QString scriptPath = "C:/Users/19272/Desktop/homepage_7/py/GetData.py";

    QStringList arguments;
    arguments << scriptPath
              << ct
              << QString::number(startYear)
              << QString::number(startMonth)
              << QString::number(endYear)
              << QString::number(endMonth);
    qDebug() <<arguments;


    process->start("D:/Python/python.exe", arguments);
}

void link_py::onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    if (exitStatus == QProcess::NormalExit && exitCode == 0) {
        qDebug() << "Python 脚本执行成功";
    } else {
        qDebug() << "Python 脚本执行失败";
    }
}
