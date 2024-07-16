#ifndef ADMIN_H
#define ADMIN_H

#include <QWidget>
#include <QPushButton>
#include <QStackedWidget>
#include <QSqlTableModel>
#include <QSqlDatabase>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTableView>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QLineEdit>
#include <QFormLayout>
#include <QDebug>
#include <QDialog>
#include <QDialogButtonBox>
#include <QItemSelectionModel>
#include <QHeaderView>
#include <QLabel>
#include <QSqlRecord>
#include <QCryptographicHash>
#include <QInputDialog>

class admin : public QWidget
{
    Q_OBJECT

public:
    explicit admin(QWidget *parent = nullptr);
    ~admin();

private slots:
    void showUsers();  //展示用户管理界面
    void showAdmins();  //管理管理员界面
    void returnToInitial();  //返回
    void addUser();  //添加用户
    void deleteUser();  //删除用户
    void addAdmin();  //添加管理员
    void deleteAdmin();  //删除用户
    void resetPassword();  ///重置密码
    void showResetPasswordButton(const QModelIndex &index);  //展示重置密码按钮

private:
    QVBoxLayout *mainLayout;  //主要布局
    QHBoxLayout *buttonLayout;  //按钮布局
    QPushButton *buttons[2];  //按钮组
    QPushButton *returnButton;  //返回按钮
    QStackedWidget *stackedWidget;  //小窗口
    QWidget *initialWidget;  //初始窗口
    QWidget *userWidget;  //用户管理窗口
    QWidget *adminWidget;  //管理员窗口
    QTableView *userTableView;  //用户表
    QTableView *adminTableView;  //管理员表
    QSqlTableModel *userModel;  //用户数据
    QSqlTableModel *adminModel;  //管理员数据
    QPushButton *deleteUserButton;  //删除用户按钮
    QPushButton *deleteAdminButton;  //删除管理员按钮
    QPushButton *resetPasswordButton;  //重置密码按钮

    QModelIndex selectedUserIndex;  //选择用户的索引
    QModelIndex selectedAdminIndex;  //选择管理员的索引

    QString hashPassword(const QString &password);  //哈密加密后的密码
};

#endif // ADMIN_H
