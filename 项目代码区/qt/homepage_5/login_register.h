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
    void onLoginClicked();
    void onRegisterClicked();
    void onShowPasswordChecked(int state);

private:
    Ui::login_register *ui;

    QPushButton *btn1;
    QPushButton *btn2;
    QVBoxLayout *layout;
    QLineEdit *le1, *le2;
    QLabel *la1, *la2;
    QCheckBox *showPasswordCheckBox;

    QSqlDatabase db;

    bool connectToDatabase();
    bool authenticate(const QString &username, const QString &password, int &identity);
    bool userExists(const QString &username);
    void saveUser(const QString &username, const QString &password);
};

#endif // LOGIN_REGISTER_H
