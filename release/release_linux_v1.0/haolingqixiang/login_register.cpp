#include "login_register.h"
#include "ui_login_register.h"
#include "mainwindow.h"
#include "regis.h"
#include <QCryptographicHash>

// 构造函数，初始化登录注册界面
login_register::login_register(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::login_register)
    , networkManager(new QNetworkAccessManager(this))
{
    ui->setupUi(this);
    this->setWindowTitle("登录");

    // 创建垂直布局
    QVBoxLayout *vbox = new QVBoxLayout(this);

    // 创建背景标签并设置背景图片
    backgroundLabel = new QLabel(this);
    backgroundLabel->setStyleSheet("QLabel { background-image: url(:/logo/7.jpg); background-repeat: no-repeat; background-position: top; }");
    backgroundLabel->setFixedSize(400, 89);

    // 创建按钮和输入框
    btn1 = new QPushButton("登录", this);
    btn2 = new QPushButton("注册", this);
    le1 = new QLineEdit(this);
    le2 = new QLineEdit(this);
    la1 = new QLabel("账号：", this);
    la2 = new QLabel("密码：", this);
    showPasswordCheckBox = new QCheckBox("显示密码", this);

    // 设置密码输入框为密码模式
    le2->setEchoMode(QLineEdit::Password);

    // 设置输入框的占位文本
    le1->setPlaceholderText("请输入账号");
    le2->setPlaceholderText("请输入密码");

    // 将控件添加到垂直布局中
    vbox->addWidget(backgroundLabel);
    vbox->addWidget(la1);
    vbox->addWidget(le1);
    vbox->addWidget(la2);
    vbox->addWidget(le2);
    vbox->addWidget(showPasswordCheckBox);
    vbox->addWidget(btn1);
    vbox->addWidget(btn2);

    // 设置样式表
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

    // 设置布局
    setLayout(vbox);
    this->setFixedSize(420, 380);

    // 连接按钮点击事件
    connect(btn1, &QPushButton::clicked, this, &login_register::on_loginButton_clicked);
    connect(btn2, &QPushButton::clicked, this, &login_register::on_registerButton_clicked);
    connect(showPasswordCheckBox, &QCheckBox::stateChanged, this, &login_register::onShowPasswordChecked);
}

login_register::~login_register()
{
    delete ui;
}

// 处理登录按钮点击事件
void login_register::on_loginButton_clicked()
{
    QString account = le1->text();
    QString password = le2->text();

    // 检查账号和密码是否为空
    if (account.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "登录失败", "账号或密码不能为空，请重试。");
        return;
    }

    // 对密码进行SHA256哈希
    QByteArray hash = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256);
    QString hashedPassword = QString(hash.toHex());

    // 创建网络请求
    QNetworkRequest request(QUrl("http://60.205.232.122:5000/login"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // 创建JSON对象
    QJsonObject json;
    json["account"] = account;
    json["password"] = hashedPassword;

    // 发送POST请求
    QNetworkReply* reply = networkManager->post(request, QJsonDocument(json).toJson());
    connect(reply, &QNetworkReply::finished, this, &login_register::handleLoginReply);
}

// 处理登录回复
void login_register::handleLoginReply()
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    if (reply->error() == QNetworkReply::NoError)
    {
        QJsonDocument jsonResponse = QJsonDocument::fromJson(reply->readAll());
        QJsonObject jsonObject = jsonResponse.object();
        bool success = jsonObject["success"].toBool();
        int identity = jsonObject["identity"].toInt();
        if (success)
        {
            QMessageBox::information(this, "登录成功", "欢迎！");
            this->close();
            MainWindow *w = new MainWindow(identity);
            w->showMaximized();
            w->show();
        }
        else
        {
            QString errorMessage = jsonObject["error_message"].toString();
            QMessageBox::warning(this, "登录失败", "账号或密码错误，请重试。" + errorMessage);
        }
    }
    else
    {
        QString errorString = reply->errorString();
        QMessageBox::warning(this, "登录失败", "网络错误，请重试。错误信息：" + errorString);
    }
    reply->deleteLater();
}

// 处理注册按钮点击事件
void login_register::on_registerButton_clicked()
{
    this->close();
    regis *w = new regis();
    w->show();

    // QString account = le1->text();
    // QString password = le2->text();

    // if (account.isEmpty() || password.isEmpty()) {
    //     QMessageBox::warning(this, "注册失败", "账号或密码不能为空，请重试。");
    //     return;
    // }

    // QByteArray hash = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256);
    // QString hashedPassword = QString(hash.toHex());

    // QNetworkRequest request(QUrl("http://60.205.232.122:5000/register"));
    // request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // QJsonObject json;
    // json["account"] = account;
    // json["password"] = hashedPassword;

    // QNetworkReply* reply = networkManager->post(request, QJsonDocument(json).toJson());
    // connect(reply, &QNetworkReply::finished, this, &login_register::handleRegisterReply);
}

// void login_register::handleRegisterReply()
// {
//     QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
//     if (reply->error() == QNetworkReply::NoError)
//     {
//         QJsonDocument jsonResponse = QJsonDocument::fromJson(reply->readAll());
//         QJsonObject jsonObject = jsonResponse.object();
//         bool success = jsonObject["success"].toBool();
//         if (success)
//         {
//             QMessageBox::information(this, "注册成功", "请登录！");
//         }
//         else
//         {
//             QString errorMessage = jsonObject["error_message"].toString();
//             QMessageBox::warning(this, "注册失败", "请更换账号重新注册！" + errorMessage);
//         }
//     }
//     else
//     {
//         QString errorString = reply->errorString();
//         if(errorString == "Error transferring http://60.205.232.122:5000/register - server replied: INTERNAL SERVER ERROR")
//         {
//             QMessageBox::warning(this, "注册失败", "该账号已存在");
//         }
//         else
//         {
//             QMessageBox::warning(this, "注册失败", "网络错误，请重试。错误信息：" + errorString);
//         }
//     }
//     reply->deleteLater();
// }

// 处理显示密码复选框状态变化事件
void login_register::onShowPasswordChecked(int state)
{
    if (state == Qt::Checked) {
        le2->setEchoMode(QLineEdit::Normal);
    } else {
        le2->setEchoMode(QLineEdit::Password);
    }
}
