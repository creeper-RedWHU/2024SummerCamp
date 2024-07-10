#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , networkManager(new QNetworkAccessManager(this))
{
    ui->setupUi(this);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_registerButton_clicked() {
    QString account = ui->accountLineEdit->text();
    QString password = ui->passwordLineEdit->text();

    QNetworkRequest request(QUrl("http://60.205.232.122:5000/register"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject json;
    json["account"] = account;
    json["password"] = password;

    QNetworkReply* reply = networkManager->post(request, QJsonDocument(json).toJson());
    connect(reply, &QNetworkReply::finished, this, &MainWindow::handleRegisterReply);
}

void MainWindow::on_loginButton_clicked() {
    QString account = ui->accountLineEdit->text();
    QString password = ui->passwordLineEdit->text();

    QNetworkRequest request(QUrl("http://60.205.232.122:5000/login"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject json;
    json["account"] = account;
    json["password"] = password;

    QNetworkReply* reply = networkManager->post(request, QJsonDocument(json).toJson());
    connect(reply, &QNetworkReply::finished, this, &MainWindow::handleLoginReply);
}

void MainWindow::handleRegisterReply() {
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    if (reply->error() == QNetworkReply::NoError) {
        ui->messageLabel->setText("User registered successfully");
    } else {
        ui->messageLabel->setText("Failed to register user: " + reply->errorString());
    }
    reply->deleteLater();
}

void MainWindow::handleLoginReply() {
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    if (reply->error() == QNetworkReply::NoError) {
        QJsonDocument jsonResponse = QJsonDocument::fromJson(reply->readAll());
        QJsonObject jsonObject = jsonResponse.object();
        bool success = jsonObject["success"].toBool();
        int identity = jsonObject["identity"].toInt();
        if (success) {
            ui->messageLabel->setText("Login successful, identity: " + QString::number(identity));
        } else {
            ui->messageLabel->setText("Login failed");
        }
    } else {
        ui->messageLabel->setText("Failed to login: " + reply->errorString());
    }
    reply->deleteLater();
}
