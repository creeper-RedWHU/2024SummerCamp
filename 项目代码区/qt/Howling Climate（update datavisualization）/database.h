#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>

class database {
public:
    bool connectToDatabase();

private:
    QSqlDatabase db;
};

#endif // DATABASE_H
