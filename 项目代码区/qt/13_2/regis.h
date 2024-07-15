#ifndef REGIS_H
#define REGIS_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include <QCheckBox>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include "login_register.h"

namespace Ui {
class regis;
}

class regis : public QWidget
{
    Q_OBJECT

public:
    explicit regis(QWidget *parent = nullptr);
    ~regis();

    bool userExists(const QString &username);
    void saveUser(const QString &username, const QString &password);
    bool isSame();

private slots:
    void handleRegisterReply();
    void on_registerButton_clicked();
    void onShowPasswordChecked(int state);

private:
    Ui::regis *ui;

    QNetworkAccessManager *networkManager;
    QVBoxLayout *layout;
    QPushButton *btn1;
    QLineEdit *le1, *le2, *le3;
    QLabel *la1, *la2, *la3;

    QCheckBox *showPasswordCheckBox;
    QSqlDatabase db;
};

#endif // REGIS_H

