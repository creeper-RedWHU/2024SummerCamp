#include "login_register.h"
#include "ui_login_register.h"
#include "mainwindow.h"

login_register::login_register(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::login_register)
{
    ui->setupUi(this);

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
            background-color: #8B8989;
            color: #FFFFFF;
            border: 1px solid #8B8989;
            border-radius: 5px;
            padding: 10px 20px;
            font-family: "Arial", sans-serif;
            font-size: 14px;
            font-weight: bold;
        }

        QPushButton:hover {
            background-color: #EEE5DE;
            border: 1px solid #EEE5DE;
        }

        QPushButton:pressed {
            background-color: #CDC5BF;
            border: 1px solid #CDC5BF;
        }
    )";
    this->setStyleSheet(styleSheet);

    setLayout(vbox);
    this->setFixedSize(400, 300);

    connect(btn1, &QPushButton::clicked, this, &login_register::onLoginClicked);
    connect(btn2, &QPushButton::clicked, this, &login_register::onRegisterClicked);
    connect(showPasswordCheckBox, &QCheckBox::stateChanged, this, &login_register::onShowPasswordChecked);

    if (!connectToDatabase()) {
        QMessageBox::critical(this, "数据库连接失败", "无法连接到数据库，请检查配置。");
    }
}

login_register::~login_register()
{
    delete ui;
}

bool login_register::connectToDatabase()
{
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("60.205.232.122");
    db.setDatabaseName("data");
    db.setUserName("root");
    db.setPassword("123456");

    // QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");// 建立和QSQLITE数据库的连接
    // db.setHostName("127.0.0.1");  //连接本地主机
    // db.setPort(3306);
    // db.setUserName("root");
    //设置数据库的密码
    // db.setPassword("mt127715318");    //这个就是安装MySQL时设置的密码
    // db.setDatabaseName("/Users/motao/demo1.db");//设置数据库名称
    if (!db.open()) {
        qDebug() << "Database error occurred:" << db.lastError();
        return false;
    }
    return true;
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
