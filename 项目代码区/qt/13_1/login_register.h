#ifndef LOGIN_REGISTER_H
#define LOGIN_REGISTER_H

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

namespace Ui {
class login_register;
}

class login_register : public QWidget
{
    Q_OBJECT

public:
    explicit login_register(QWidget *parent = nullptr);
    ~login_register();

private slots:
    void on_loginButton_clicked();
    void handleLoginReply();
    void on_registerButton_clicked();
    void onShowPasswordChecked(int state);
    //void handleRegisterReply();

private:
    Ui::login_register *ui;

    QNetworkAccessManager *networkManager;
    QPushButton *btn1;
    QPushButton *btn2;
    QVBoxLayout *layout;
    QLineEdit *le1, *le2;
    QLabel *la1, *la2;
    QCheckBox *showPasswordCheckBox;

    QSqlDatabase db;

    // bool connectToDatabase();
    bool authenticate(const QString &username, const QString &password, int &identity);
    bool userExists(const QString &username);
    void saveUser(const QString &username, const QString &password);
};

#endif // LOGIN_REGISTER_H
