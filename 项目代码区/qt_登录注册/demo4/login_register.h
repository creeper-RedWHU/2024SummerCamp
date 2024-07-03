#ifndef LOGIN_REGISTER_H
#define LOGIN_REGISTER_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void onLoginClicked();
    void onRegisterClicked();

private:
    Ui::Widget *ui;

    QPushButton *btn1;
    QPushButton *btn2;
    QVBoxLayout *layout;
    QLineEdit *le1,*le2;  //行编辑器
    QLabel *la1,*la2;

    QSqlDatabase db;

    bool connectToDatabase();
    bool authenticate(const QString &username, const QString &password);
    bool userExists(const QString &username);
    void saveUser(const QString &username, const QString &password);
};
#endif // LOGIN_REGISTER_H
