#ifndef LINK_PY_H
#define LINK_PY_H

#include <QObject>
#include <QProcess>
// #include <Python.h>

class link_py : public QObject
{
    Q_OBJECT

public:
    explicit link_py(QObject *parent = nullptr);
    void fetchData(const QString &ct, int startYear, int startMonth, int endYear, int endMonth);

private slots:
    void onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);

private:
    QProcess *process;
};

#endif // LINK_PY_H
