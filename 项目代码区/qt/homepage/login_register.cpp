#include "login_register.h"
#include "ui_login_register.h"
#include "mainwindow.h"

login_register::login_register(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::login_register)
{
    ui->setupUi(this);

    this->setWindowTitle("登录界面");

    // 创建垂直布局管理器
    QVBoxLayout *vbox = new QVBoxLayout(this);

    // 创建按钮和文本框
    btn1 = new QPushButton("登录", this);
    btn2 = new QPushButton("注册", this);
    le1 = new QLineEdit(this);
    le2 = new QLineEdit(this);
    la1 = new QLabel("账号：", this);
    la2 = new QLabel("密码：", this);

    // 将部件添加到布局管理器中，按照需要的顺序添加
    vbox->addWidget(la1);
    vbox->addWidget(le1);
    vbox->addWidget(la2);
    vbox->addWidget(le2);
    vbox->addWidget(btn1);
    vbox->addWidget(btn2);

    // 设置样式表
    le1->setStyleSheet("QLineEdit { "
                       "    height: 30px; "  // 设置高度为 30 像素
                       "    font-size: 14px; "  // 设置字体大小为 14 像素
                       "}");
    le2->setStyleSheet("QLineEdit { "
                       "    height: 30px; "  // 设置高度为 30 像素
                       "    font-size: 14px; "  // 设置字体大小为 14 像素
                       "}");
    btn1->setStyleSheet("QPushButton { "
                        "    height: 30px; "  // 设置按钮高度为 30 像素
                        "    font-size: 14px; "  // 设置字体大小为 14 像素
                        "}");
    btn2->setStyleSheet("QPushButton { "
                        "    height: 30px; "  // 设置按钮高度为 30 像素
                        "    font-size: 14px; "  // 设置字体大小为 14 像素
                        "}");

    // 设置窗口的整体布局
    setLayout(vbox);
    this->setFixedSize(400, 300);

    // 连接按钮点击事件到槽函数
    connect(btn1, &QPushButton::clicked, this, &login_register::onLoginClicked);
    connect(btn2, &QPushButton::clicked, this, &login_register::onRegisterClicked);

    // 连接到数据库
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
    db.setHostName("localhost"); // 设置数据库主机名
    db.setDatabaseName("demouser"); // 设置数据库名
    db.setUserName("root"); // 设置数据库用户名
    db.setPassword("zhjsh666"); // 设置数据库密码

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

    if (authenticate(username, password)) {
        QMessageBox::information(this, "登录成功", "欢迎，" + username + "！");
        this->close();
        MainWindow w = new MainWindow;
        w.show();
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

bool login_register::authenticate(const QString &username, const QString &password)
{
    QSqlQuery query;
    query.prepare("SELECT password FROM users WHERE account = :username");
    query.bindValue(":username", username);

    if (!query.exec()) {
        qDebug() << "Error executing query:" << query.lastError();
        return false;
    }

    if (query.next()) {
        QString storedPassword = query.value(0).toString();
        if (storedPassword == password) {
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
