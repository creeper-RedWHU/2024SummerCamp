#include "analysis.h"
#include <QLabel>
#include <QPainter>

ImageWidget::ImageWidget(QWidget *parent) : QWidget(parent), imageLayout(new QGridLayout(this)) {
    setLayout(imageLayout);

    // 初始化4个QLabel以便复用，避免页面闪烁
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

    // 设置窗口最大化并占满屏幕
    setWindowState(Qt::WindowMaximized);
}

void MainWindow::drawChart() {
    QString city = cityComboBox->currentText();
    QString startYear = startYearComboBox->currentText();
    QString endYear = endYearComboBox->currentText();

    // 调用不同的算法生成四张图像
    QImage image1 = generateImage1(city, startYear, endYear);
    QImage image2 = generateImage2(city, startYear, endYear);
    QImage image3 = generateImage3(city, startYear, endYear);
    QImage image4 = generateImage4(city, startYear, endYear);

    QList<QImage> images = {image1, image2, image3, image4};
    imageWidget->updateImages(images);
}

QImage MainWindow::generateImage1(const QString &city, const QString &startYear, const QString &endYear) {
    // 图像生成算法1
    QImage image(600, 400, QImage::Format_RGB32);
    image.fill(Qt::white);
    QPainter painter(&image);
    painter.drawText(image.rect(), Qt::AlignCenter, city + " 图像1 " + startYear + "-" + endYear);
    return image;
}

QImage MainWindow::generateImage2(const QString &city, const QString &startYear, const QString &endYear) {
    // 图像生成算法2
    QImage image(600, 400, QImage::Format_RGB32);
    image.fill(Qt::white);
    QPainter painter(&image);
    painter.drawText(image.rect(), Qt::AlignCenter, city + " 图像2 " + startYear + "-" + endYear);
    return image;
}

QImage MainWindow::generateImage3(const QString &city, const QString &startYear, const QString &endYear) {
    // 图像生成算法3
    QImage image(600, 400, QImage::Format_RGB32);
    image.fill(Qt::white);
    QPainter painter(&image);
    painter.drawText(image.rect(), Qt::AlignCenter, city + " 图像3 " + startYear + "-" + endYear);
    return image;
}

QImage MainWindow::generateImage4(const QString &city, const QString &startYear, const QString &endYear) {
    // 图像生成算法4
    QImage image(600, 400, QImage::Format_RGB32);
    image.fill(Qt::white);
    QPainter painter(&image);
    painter.drawText(image.rect(), Qt::AlignCenter, city + " 图像4 " + startYear + "-" + endYear);
    return image;
}
