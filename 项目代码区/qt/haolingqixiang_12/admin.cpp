#include "admin.h"
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
#include "database.h"

admin::admin(QWidget *parent)
    : QWidget(parent)
{
    database thisdb;
    // 初始化数据库连接
    if (!thisdb.connectToDatabase()) {
        QMessageBox::critical(this, "登录界面数据库连接失败", "无法连接到数据库。");
        return;
    }

    mainLayout = new QVBoxLayout(this);
    buttonLayout = new QHBoxLayout();

    // 创建功能按钮
    const char* buttonNames[5] = {"用户", "管理员"};
    for (int i = 0; i < 2; ++i) {
        buttons[i] = new QPushButton(buttonNames[i], this);
        buttons[i]->setFixedHeight(80);
        buttonLayout->addWidget(buttons[i]);
    }

    // 创建返回按钮
    returnButton = new QPushButton("←", this);
    returnButton->setFixedSize(55, 55);
    returnButton->setVisible(false);
    returnButton->setStyleSheet("border-radius: 25px; border: 2px solid #8B8989;");
    returnButton->setStyleSheet("color:#000000");

    // 创建返回按钮的框
    QWidget *returnButtonBox = new QWidget(this);
    QHBoxLayout *returnButtonBoxLayout = new QHBoxLayout(returnButtonBox);
    returnButtonBoxLayout->addWidget(returnButton);
    returnButtonBoxLayout->setContentsMargins(0, 0, 0, 0);
    returnButtonBox->setFixedSize(55, 55);
    returnButtonBox->setStyleSheet("background-color: #FFFFFF; border-radius: 30px;");

    // 将返回按钮添加到右上角
    QHBoxLayout *topLayout = new QHBoxLayout();
    topLayout->addWidget(returnButtonBox, 0, Qt::AlignRight);

    // 连接按钮信号到槽
    connect(buttons[0], &QPushButton::clicked, this, &admin::showUsers);
    connect(buttons[1], &QPushButton::clicked, this, &admin::showAdmins);
    connect(returnButton, &QPushButton::clicked, this, &admin::returnToInitial);

    // 创建堆栈窗口以保存不同视图
    stackedWidget = new QStackedWidget(this);

    // 初始窗口
    initialWidget = new QWidget(this);
    QVBoxLayout *initialLayout = new QVBoxLayout(initialWidget);
    initialLayout->addLayout(buttonLayout);
    stackedWidget->addWidget(initialWidget);

    // 用户窗口
    userWidget = new QWidget(this);
    QVBoxLayout *userLayout = new QVBoxLayout(userWidget);

    userTableView = new QTableView(this);
    userModel = new QSqlTableModel(this);
    userModel->setTable("users");
    userModel->setFilter("identity = 1");
    userModel->select();
    userTableView->setModel(userModel);

    userTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    userTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    userTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    connect(userTableView, &QTableView::clicked, this, &admin::showDeleteButton);

    deleteUserButton = new QPushButton("删除", this);
    deleteUserButton->setVisible(false);
    connect(deleteUserButton, &QPushButton::clicked, this, &admin::deleteUser);

    QPushButton *addUserButton = new QPushButton("添加用户", this);
    connect(addUserButton, &QPushButton::clicked, this, &admin::addUser);

    userLayout->addWidget(userTableView);
    userLayout->addWidget(deleteUserButton);
    userLayout->addWidget(addUserButton);
    stackedWidget->addWidget(userWidget);

    // 管理员窗口
    adminWidget = new QWidget(this);
    QVBoxLayout *adminLayout = new QVBoxLayout(adminWidget);

    adminTableView = new QTableView(this);
    adminModel = new QSqlTableModel(this);
    adminModel->setTable("users");
    adminModel->setFilter("identity = 2");
    adminModel->select();
    adminTableView->setModel(adminModel);

    adminTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    adminTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    adminTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    connect(adminTableView, &QTableView::clicked, this, &admin::showDeleteButton);

    deleteAdminButton = new QPushButton("删除", this);
    deleteAdminButton->setVisible(false);
    connect(deleteAdminButton, &QPushButton::clicked, this, &admin::deleteAdmin);

    QPushButton *addAdminButton = new QPushButton("添加管理员", this);
    connect(addAdminButton, &QPushButton::clicked, this, &admin::addAdmin);

    adminLayout->addWidget(adminTableView);
    adminLayout->addWidget(deleteAdminButton);
    adminLayout->addWidget(addAdminButton);
    stackedWidget->addWidget(adminWidget);

    // 将堆栈窗口添加到主布局
    mainLayout->addLayout(topLayout);
    mainLayout->addWidget(stackedWidget);

    setLayout(mainLayout);
    this->showMaximized(); // 启动时最大化窗口

    QString styleSheet = R"(
        QWidget {
            background-color: #F7F9FC;
        }

        QPushButton {
            background-color: #8B8989;
            color: #FFFFFF;
            border: 1px solid #8B8989;
            border-radius: 5px;
            padding: 20px;
            font-family: "Arial", sans-serif;
            font-size: 18px;
            font-weight: bold;
        }

        QPushButton:hover {
            background-color: #CDC5BF;
            border: 1px solid #CDC5BF;
        }

        QPushButton:pressed {
            background-color: #8B8682;
            border: 1px solid #8B8682;
        }

        QTableView {
            background-color: #FFFFFF;
            border: 1px solid #E1E8ED;
            border-radius: 5px;
            padding: 10px;
            font-family: "Arial", sans-serif;
            font-size: 14px;
            color: #333333;
        }

        QHeaderView::section {
            background-color: #F7F9FC;
            padding: 5px;
            border: none;
            font-family: "Arial", sans-serif;
            font-size: 14px;
            color: #333333;
        }
    )";

    setStyleSheet(styleSheet);
}

admin::~admin()
{
}

void admin::showUsers()
{
    stackedWidget->setCurrentWidget(userWidget);
    returnButton->setVisible(true);
}

void admin::showAdmins()
{
    stackedWidget->setCurrentWidget(adminWidget);
    returnButton->setVisible(true);
}

void admin::returnToInitial()
{
    stackedWidget->setCurrentWidget(initialWidget);
    returnButton->setVisible(false);
}

void admin::addUser()
{
    QDialog dialog(this);
    dialog.setWindowTitle("添加新用户");

    QFormLayout form(&dialog);
    form.setLabelAlignment(Qt::AlignRight);

    QLineEdit *accountEdit = new QLineEdit(&dialog);
    QLineEdit *passwordEdit = new QLineEdit(&dialog);
    accountEdit->setFixedHeight(30);
    passwordEdit->setFixedHeight(30);
    passwordEdit->setEchoMode(QLineEdit::Password);

    form.addRow("账号:", accountEdit);
    form.addRow("密码:", passwordEdit);

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);

    connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        QString account = accountEdit->text();
        QString password = passwordEdit->text();

        QSqlQuery query;
        query.prepare("INSERT INTO users (account, password, identity) VALUES (?, ?, ?)");
        query.addBindValue(account);
        query.addBindValue(password);
        query.addBindValue(1); // 普通用户的身份为 1

        if (!query.exec()) {
            QMessageBox::critical(this, "错误", "添加用户失败: " + query.lastError().text());
        } else {
            userModel->select();
        }
    }
}

void admin::deleteUser()
{
    if (!selectedUserIndex.isValid()) return;

    int row = selectedUserIndex.row();
    QSqlRecord record = userModel->record(row);
    QString account = record.value("account").toString();

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "确认删除", "你确定要删除该用户吗？", QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        QSqlQuery query;
        query.prepare("DELETE FROM users WHERE account = ?");
        query.addBindValue(account);

        if (!query.exec()) {
            QMessageBox::critical(this, "错误", "删除用户失败: " + query.lastError().text());
        } else {
            userModel->select();
            deleteUserButton->setVisible(false);
        }
    }
}

void admin::addAdmin()
{
    QDialog dialog(this);
    dialog.setWindowTitle("添加新管理员");

    QFormLayout form(&dialog);
    form.setLabelAlignment(Qt::AlignRight);

    QLineEdit *accountEdit = new QLineEdit(&dialog);
    QLineEdit *passwordEdit = new QLineEdit(&dialog);
    accountEdit->setFixedHeight(30);
    passwordEdit->setFixedHeight(30);
    passwordEdit->setEchoMode(QLineEdit::Password);

    form.addRow("账号:", accountEdit);
    form.addRow("密码:", passwordEdit);

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);

    connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        QString account = accountEdit->text();
        QString password = passwordEdit->text();

        QSqlQuery query;
        query.prepare("INSERT INTO users (account, password, identity) VALUES (?, ?, ?)");
        query.addBindValue(account);
        query.addBindValue(password);
        query.addBindValue(2); // 管理员的身份为 2

        if (!query.exec()) {
            QMessageBox::critical(this, "错误", "添加管理员失败: " + query.lastError().text());
        } else {
            adminModel->select();
        }
    }
}

void admin::deleteAdmin()
{
    if (!selectedAdminIndex.isValid()) return;

    int row = selectedAdminIndex.row();
    QSqlRecord record = adminModel->record(row);
    QString account = record.value("account").toString();

    if (account == "admin") {
        QMessageBox::warning(this, "删除错误", "无法删除主管理员账号。");
        return;
    }

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "确认删除", "你确定要删除该管理员吗？", QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        QSqlQuery query;
        query.prepare("DELETE FROM users WHERE account = ?");
        query.addBindValue(account);

        if (!query.exec()) {
            QMessageBox::critical(this, "错误", "删除管理员失败: " + query.lastError().text());
        } else {
            adminModel->select();
            deleteAdminButton->setVisible(false);
        }
    }
}

void admin::showDeleteButton(const QModelIndex &index)
{
    if (stackedWidget->currentWidget() == userWidget) {
        selectedUserIndex = index;
        deleteUserButton->setVisible(true);
    } else if (stackedWidget->currentWidget() == adminWidget) {
        selectedAdminIndex = index;
        deleteAdminButton->setVisible(true);
    }
}

// bool admin::connectToDatabase()
// {
//     db = QSqlDatabase::addDatabase("QMYSQL");
//     db.setHostName("localhost");
//     db.setDatabaseName("data");
//     db.setUserName("root");
//     db.setPassword("123456");

//     if (!db.open()) {
//         qDebug() << "数据库连接失败: " << db.lastError().text();
//         return false;
//     }
//     return true;
// }
