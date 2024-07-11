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

class admin : public QWidget
{
    Q_OBJECT

public:
    explicit admin(QWidget *parent = nullptr);
    ~admin();

private slots:
    void showUsers();
    void showAdmins();
    void returnToInitial();
    void addUser();
    void deleteUser();
    void addAdmin();
    void deleteAdmin();
    void showDeleteButton(const QModelIndex &index);

private:
    QVBoxLayout *mainLayout;
    QHBoxLayout *buttonLayout;
    QPushButton *buttons[5];
    QPushButton *returnButton;
    QStackedWidget *stackedWidget;
    QWidget *initialWidget;
    QWidget *userWidget;
    QWidget *adminWidget;
    QTableView *userTableView;
    QTableView *adminTableView;
    QSqlTableModel *userModel;
    QSqlTableModel *adminModel;
    QPushButton *deleteUserButton;
    QPushButton *deleteAdminButton;
    QSqlDatabase db;

    QModelIndex selectedUserIndex;
    QModelIndex selectedAdminIndex;

};

#endif // ADMIN_H
