#include "forecast.h"
#include "ui_forecast.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPixmap>

forecast::forecast(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::forecast)
{
    ui->setupUi(this);
    QVBoxLayout *layout = new QVBoxLayout(this);

    // 创建左上角的文本标签
    QLabel *title = new QLabel("基于机器学习的气温趋势预测", this);
    title->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    title->setStyleSheet("font-size: 18px; font-weight: bold; color: #1F2937; margin-top: 10px; margin-left: 10px;"); // Adjust style here
    layout->addWidget(title);

    // 创建用于显示算法生成图像的占位符标签
    QLabel *imagePlaceholder = new QLabel(this);
    imagePlaceholder->setFixedSize(400, 300);
    imagePlaceholder->setAlignment(Qt::AlignCenter);
    imagePlaceholder->setStyleSheet("border: 1px solid #E5E7EB; background-color: #F3F4F6; color: #4B5563; font-size: 16px;"); // Adjust style here
    imagePlaceholder->setText("图像区域");

    layout->addWidget(imagePlaceholder, 0, Qt::AlignHCenter | Qt::AlignVCenter);

    setLayout(layout);

    setStyleSheet(R"(
        QWidget {
            background-color: #F9FAFB;
            color: #1F2937;
            font-family: "Segoe UI", Roboto, "Helvetica Neue", Arial, sans-serif;
        }
    )");
}

forecast::~forecast()
{
    delete ui;
}
