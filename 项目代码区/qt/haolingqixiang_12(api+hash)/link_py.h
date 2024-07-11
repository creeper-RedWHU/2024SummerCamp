#ifndef LINK_PY_H
#define LINK_PY_H

#include <QObject>
#include <QProcess>
#include <QThread>

class link_py : public QObject
{
    Q_OBJECT
public:
    explicit link_py(QObject *parent = nullptr);
    void runPythonScript();  // 连接 hello 函数
    void runFetchData(const QString &str, int a, int b, int c, int d);  // 连接 fetchData 函数

private slots:
    void onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);

private:
    QProcess *process;
};

class PythonRunner : public QThread
{
    Q_OBJECT

public:
    PythonRunner(QObject *parent = nullptr);
    PythonRunner(const QString &str, int a, int b, int c, int d, QObject *parent = nullptr);

protected:
    void run() override;

private:
    bool runFetchData;
    QString str;
    int a, b, c, d;
};

#endif // LINK_PY_H
