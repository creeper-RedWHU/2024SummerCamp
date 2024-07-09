#include <QApplication>
#include <QPushButton>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>

void handleButtonClicked() {
    QNetworkAccessManager *manager = new QNetworkAccessManager();
    QObject::connect(manager, &QNetworkAccessManager::finished, [](QNetworkReply *reply) {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray response = reply->readAll();
            QMessageBox::information(nullptr, "Response", response);
        } else {
            QMessageBox::critical(nullptr, "Error", reply->errorString());
        }
        reply->deleteLater();
    });

    QNetworkRequest request(QUrl("http://60.205.232.122:5000/hello"));
    manager->get(request);
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QPushButton button("Click me!");
    QObject::connect(&button, &QPushButton::clicked, handleButtonClicked);
    button.show();

    return app.exec();
}
