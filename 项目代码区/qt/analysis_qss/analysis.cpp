#include "analysis.h"
#include <QLabel>
#include <QPainter>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QApplication>

ImageWidget::ImageWidget(QWidget *parent) : QWidget(parent), imageLayout(new QGridLayout(this)) {
    setLayout(imageLayout);

    for (int i = 0; i < 4; ++i) {
        QLabel *imageLabel = new QLabel(this);
        imageLabels.append(imageLabel);
        int row = i / 2;
        int col = i % 2;
        imageLayout->addWidget(imageLabel, row, col);
    }
}

void ImageWidget::updateImages(const QList<QImage> &images) {
    for (int i = 0; i < images.size(); ++i) {
        imageLabels[i]->setPixmap(QPixmap::fromImage(images[i]));
    }
}

MainWindow::MainWindow(QWidget *parent) : QWidget(parent), imageWidget(new ImageWidget(this)) {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QHBoxLayout *controlLayout = new QHBoxLayout();

    cityComboBox = new QComboBox(this);
    cityComboBox->addItems({"北京", "上海", "广州"});
    controlLayout->addWidget(new QLabel("选择城市:", this));
    controlLayout->addWidget(cityComboBox);

    startYearComboBox = new QComboBox(this);
    startYearComboBox->addItems({"2020", "2021", "2022"});
    controlLayout->addWidget(new QLabel("选择起始年份:", this));
    controlLayout->addWidget(startYearComboBox);

    endYearComboBox = new QComboBox(this);
    endYearComboBox->addItems({"2021", "2022", "2023"});
    controlLayout->addWidget(new QLabel("选择中止年份:", this));
    controlLayout->addWidget(endYearComboBox);

    QPushButton *drawButton = new QPushButton("绘制", this);
    controlLayout->addWidget(drawButton);
    connect(drawButton, &QPushButton::clicked, this, &MainWindow::drawChart);

    mainLayout->addLayout(controlLayout);
    mainLayout->addWidget(imageWidget);

    setLayout(mainLayout);

    setWindowState(Qt::WindowMaximized);

    setStyleSheet(R"(
        QWidget {
            background-color: #F7F7F8;
            color: #000000;
            font-family: "Helvetica Neue", Helvetica, Arial, sans-serif;
        }
        QLabel {
            font-size: 14px;
        }
        QComboBox {
            padding: 5px;
            border: 1px solid #CCCCCC;
            border-radius: 4px;
            background-color: #FFFFFF;
            font-size: 14px;
        }
        QComboBox::drop-down {
            border-left: 1px solid #CCCCCC;
        }
        QPushButton {
            padding: 10px 20px;
            border: none;
            border-radius: 4px;
            background-color: #10A37F;
            color: #FFFFFF;
            font-size: 14px;
            cursor: pointer;
        }
        QPushButton:hover {
            background-color: #0E8C6C;
        }
        QPushButton:pressed {
            background-color: #0C7259;
        }
    )");
}

void MainWindow::drawChart() {
    QString city = cityComboBox->currentText();
    QString startYear = startYearComboBox->currentText();
    QString endYear = endYearComboBox->currentText();

    QImage image1 = generateImage1(city, startYear, endYear);
    QImage image2 = generateImage2(city, startYear, endYear);
    QImage image3 = generateImage3(city, startYear, endYear);
    QImage image4 = generateImage4(city, startYear, endYear);

    QList<QImage> images = {image1, image2, image3, image4};
    imageWidget->updateImages(images);

    setWindowState(Qt::WindowMaximized); // Ensure window remains maximized
}


//图像生成的算法在此处添加
QImage MainWindow::generateImage1(const QString &city, const QString &startYear, const QString &endYear) {
    QImage image(600, 400, QImage::Format_RGB32);
    image.fill(Qt::white);
    QPainter painter(&image);
    painter.drawText(image.rect(), Qt::AlignCenter, city + " 图像1 " + startYear + "-" + endYear);
    return image;
}

QImage MainWindow::generateImage2(const QString &city, const QString &startYear, const QString &endYear) {
    QImage image(600, 400, QImage::Format_RGB32);
    image.fill(Qt::white);
    QPainter painter(&image);
    painter.drawText(image.rect(), Qt::AlignCenter, city + " 图像2 " + startYear + "-" + endYear);
    return image;
}

QImage MainWindow::generateImage3(const QString &city, const QString &startYear, const QString &endYear) {
    QImage image(600, 400, QImage::Format_RGB32);
    image.fill(Qt::white);
    QPainter painter(&image);
    painter.drawText(image.rect(), Qt::AlignCenter, city + " 图像3 " + startYear + "-" + endYear);
    return image;
}

QImage MainWindow::generateImage4(const QString &city, const QString &startYear, const QString &endYear) {
    QImage image(600, 400, QImage::Format_RGB32);
    image.fill(Qt::white);
    QPainter painter(&image);
    painter.drawText(image.rect(), Qt::AlignCenter, city + " 图像4 " + startYear + "-" + endYear);
    return image;
}
