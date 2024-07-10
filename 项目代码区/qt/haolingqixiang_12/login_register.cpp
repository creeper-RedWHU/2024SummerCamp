#include "login_register.h"
#include "ui_login_register.h"
#include "mainwindow.h"
#include "database.h"

login_register::login_register(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::login_register)
{
    ui->setupUi(this);
    database thisdb;
    this->setWindowTitle("登录");

    QVBoxLayout *vbox = new QVBoxLayout(this);

    btn1 = new QPushButton("登录", this);
    btn2 = new QPushButton("注册", this);
    le1 = new QLineEdit(this);
    le2 = new QLineEdit(this);
    la1 = new QLabel("账号：", this);
    la2 = new QLabel("密码：", this);
    showPasswordCheckBox = new QCheckBox("显示密码", this);

    le2->setEchoMode(QLineEdit::Password);

    vbox->addWidget(la1);
    vbox->addWidget(le1);
    vbox->addWidget(la2);
    vbox->addWidget(le2);
    vbox->addWidget(showPasswordCheckBox);
    vbox->addWidget(btn1);
    vbox->addWidget(btn2);

    QString styleSheet = R"(
        QWidget {
            background-color: #F7F9FC;
        }

        QLineEdit {
            background-color: #FFFFFF;
            border: 1px solid #E1E8ED;
            border-radius: 5px;
            padding: 5px;
            font-family: "Arial", sans-serif;
            font-size: 14px;
            color: #333333;
        }

        QLabel {
            font-family: "Arial", sans-serif;
            font-size: 14px;
            color: #333333;
            padding: 5px;
        }

        QCheckBox {
            font-family: "Arial", sans-serif;
            font-size: 14px;
            color: #333333;
            padding: 5px;
        }

        QPushButton {
            background-color: #d6e6f2;
            color: #769fcd;
            border: 1px solid #F7F9FC;
            border-radius: 5px;
            padding: 10px 20px;
            font-family: "Arial", sans-serif;
            font-size: 14px;
            font-weight: bold;
        }

        QPushButton:hover {
            background-color: #d6e6f2;
            border: 1px solid #d6e6f2;
        }

        QPushButton:pressed {
            background-color: #b9d7ea;
            border: 1px solid #b9d7ea;
        }
    )";
    this->setStyleSheet(styleSheet);

    setLayout(vbox);
    this->setFixedSize(400, 300);

    connect(btn1, &QPushButton::clicked, this, &login_register::onLoginClicked);
    connect(btn2, &QPushButton::clicked, this, &login_register::onRegisterClicked);
    connect(showPasswordCheckBox, &QCheckBox::stateChanged, this, &login_register::onShowPasswordChecked);

    if (!thisdb.connectToDatabase()) {
        QMessageBox::critical(this, "登录界面数据库连接失败", "无法连接到数据库，请检查配置。");
    }
}

login_register::~login_register()
{
    delete ui;
}

void login_register::onLoginClicked()
{
    QString username = le1->text();
    QString password = le2->text();
    int identity = 1;

    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "登录失败", "账号或密码不能为空，请重试。");
        return;
    }

    if (authenticate(username, password, identity)) {
        QMessageBox::information(this, "登录成功", "欢迎，" + username + "！");
        this->close();
        MainWindow *w = new MainWindow(identity);
        w->show();
    } else {
        QMessageBox::warning(this, "登录失败", "账号或密码错误，请重试。");
    }
}

void login_register::onRegisterClicked()
{
    QString username = le1->text();
    QString password = le2->text();

    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "注册失败", "账号或密码不能为空，请重试。");
        return;
    }

    if (userExists(username)) {
        QMessageBox::warning(this, "注册失败", "账号已存在，请选择其他账号。");
        return;
    }

    saveUser(username, password);
    QMessageBox::information(this, "注册成功", "账号已成功注册，请登录。");
}

void login_register::onShowPasswordChecked(int state)
{
    if (state == Qt::Checked) {
        le2->setEchoMode(QLineEdit::Normal);
    } else {
        le2->setEchoMode(QLineEdit::Password);
    }
}

bool login_register::authenticate(const QString &username, const QString &password, int &identity)
{
    QSqlQuery query;
    query.prepare("SELECT password, identity FROM users WHERE account = :username");
    query.bindValue(":username", username);

    if (!query.exec()) {
        qDebug() << "Error executing query:" << query.lastError();
        return false;
    }

    if (query.next()) {
        QString storedPassword = query.value(0).toString();
        if (storedPassword == password) {
            identity = query.value(1).toInt();
            return true;
        }
    }

    return false;
}

bool login_register::userExists(const QString &username)
{
    QSqlQuery query;
    query.prepare("SELECT account FROM users WHERE account = :username");
    query.bindValue(":username", username);

    if (!query.exec()) {
        qDebug() << "Error executing query:" << query.lastError();
        return false;
    }

    if (query.next()) {
        return true;
    }

    return false;
}

void login_register::saveUser(const QString &username, const QString &password)
{
    QSqlQuery query;
    query.prepare("INSERT INTO users (account, password) VALUES (:username, :password)");
    query.bindValue(":username", username);
    query.bindValue(":password", password);

    if (!query.exec()) {
        qDebug() << "Error executing query:" << query.lastError();
    }
}
