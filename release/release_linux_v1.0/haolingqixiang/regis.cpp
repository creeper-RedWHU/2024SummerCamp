#include "regis.h"
#include <QCryptographicHash>
#include "ui_regis.h"

//构造函数
regis::regis(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::regis)
    , networkManager(new QNetworkAccessManager(this))
{
    ui->setupUi(this);
    this->setWindowTitle("注册");

    QVBoxLayout *vbox = new QVBoxLayout(this);

    //创建按钮
    btn1 = new QPushButton("注册", this);
    le1 = new QLineEdit(this);
    le2 = new QLineEdit(this);
    le3 = new QLineEdit(this);
    la1 = new QLabel("账号：", this);
    la2 = new QLabel("密码：", this);
    la3 = new QLabel("密码：", this);
    showPasswordCheckBox = new QCheckBox("显示密码", this);

    le2->setEchoMode(QLineEdit::Password);
    le3->setEchoMode(QLineEdit::Password);

    le1->setPlaceholderText("请输入账号");
    le2->setPlaceholderText("请输入密码");
    le3->setPlaceholderText("请再次输入密码");

    //将按钮等添加到布局
    vbox->addWidget(la1);
    vbox->addWidget(le1);
    vbox->addWidget(la2);
    vbox->addWidget(le2);
    vbox->addWidget(la3);
    vbox->addWidget(le3);
    vbox->addWidget(showPasswordCheckBox);
    vbox->addWidget(btn1);


    //QSS样式表
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

    //连接按钮点击事件
    connect(btn1, &QPushButton::clicked, this, &regis::on_registerButton_clicked);
    connect(showPasswordCheckBox, &QCheckBox::stateChanged, this, &regis::onShowPasswordChecked);
}


regis ::~regis()
{
    delete ui;
}

//点击注册后触发的槽函数
void regis::on_registerButton_clicked()
{
    QString account = le1->text();
    QString password = le2->text();

    if (account.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "注册失败", "账号或密码不能为空，请重试。");
        return;
    }

    QByteArray hash = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256);
    QString hashedPassword = QString(hash.toHex());

    QNetworkRequest request(QUrl("http://60.205.232.122:5000/register"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject json;
    json["account"] = account;
    json["password"] = hashedPassword;

    QNetworkReply* reply = networkManager->post(request, QJsonDocument(json).toJson());
    connect(reply, &QNetworkReply::finished, this, &regis::handleRegisterReply);
}
//对注册的处理
void regis::handleRegisterReply()
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    if (reply->error() == QNetworkReply::NoError)
    {
        QJsonDocument jsonResponse = QJsonDocument::fromJson(reply->readAll());
        QJsonObject jsonObject = jsonResponse.object();
        bool success = jsonObject["success"].toBool();
        if (success && isSame())
        {
            QMessageBox::information(this, "注册成功", "请登录！");
            this->close();
            login_register *w = new login_register;
            w->show();
        }
        //两次密码不同
        else if(!isSame())
        {
            QMessageBox::warning(this, "注册失败", "请确保两次密码输入相同");
        }
        //账号已存在
        else
        {
            QString errorMessage = jsonObject["error_message"].toString();
            QMessageBox::warning(this, "注册失败", "请更换账号重新注册！" + errorMessage);
        }
    }
    else
    {
        QString errorString = reply->errorString();
        if(errorString == "Error transferring http://60.205.232.122:5000/register - server replied: INTERNAL SERVER ERROR")
        {
            QMessageBox::warning(this, "注册失败", "该账号已存在");
        }
        else
        {
            QMessageBox::warning(this, "注册失败", "网络错误，请重试。错误信息：" + errorString);
        }
    }
    reply->deleteLater();
}

//显示密码的功能
void regis::onShowPasswordChecked(int state)
{
    if (state == Qt::Checked) {
        le2->setEchoMode(QLineEdit::Normal);
        le3->setEchoMode(QLineEdit::Normal);
    } else {
        le2->setEchoMode(QLineEdit::Password);
        le3->setEchoMode(QLineEdit::Password);
    }
}

//判断两次输入的密码是否相同
bool regis::isSame()
{
    QString password1 = le2->text();
    QString password2 = le3->text();
    return (password1==password2);
}
