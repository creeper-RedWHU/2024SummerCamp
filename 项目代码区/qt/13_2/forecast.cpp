#include "forecast.h"
#include "api.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QPixmap>

forecast::forecast(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);



    // 创建左上角的文本标签
    QLabel *title = new QLabel("基于机器学习的气温趋势预测", this);
    title->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    title->setStyleSheet("font-size: 18px; font-weight: bold; color: #1F2937; margin-top: 10px; margin-left: 10px;");
    mainLayout->addWidget(title);

    // 创建水平布局，用于放置两个按钮
    QHBoxLayout *buttonLayout = new QHBoxLayout();

    // 创建选择城市下拉框
    cityBox = new QComboBox(this);
    cityBox->addItem("武汉");
    cityBox->addItem("北京");
    cityBox->addItem("上海");
    cityBox->addItem("长沙");
    cityBox->addItem("南京");
    cityBox->addItem("杭州");
    buttonLayout->addWidget(cityBox);

    // 创建预测按钮
    btnPredict = new QPushButton("预测", this);
    buttonLayout->addWidget(btnPredict);

    mainLayout->addLayout(buttonLayout);

    // 创建用于显示算法生成图像的占位符标签
    imagePlaceholder = new QLabel(this);
    imagePlaceholder->setFixedSize(800, 600);
    imagePlaceholder->setAlignment(Qt::AlignCenter);
    imagePlaceholder->setStyleSheet("border: 1px solid #E5E7EB; background-color: #F3F4F6; color: #4B5563; font-size: 16px;");
    imagePlaceholder->setText("图像区域");

    mainLayout->addWidget(imagePlaceholder, 0, Qt::AlignHCenter | Qt::AlignVCenter);

    setLayout(mainLayout);

    setStyleSheet(R"(
        QWidget {
            background-color: #F9FAFB;
            color: #1F2937;
            font-family: "Segoe UI", Roboto, "Helvetica Neue", Arial, sans-serif;
        }
        QComboBox {
            background-color: #FFFFFF;
            border: 1px solid #E1E8ED;
            border-radius: 5px;
            padding: 5px;
            font-family: "Arial", sans-serif;
            font-size: 14px;
            color: #333333;
        }
        QComboBox::drop-down {
            subcontrol-origin: padding;
            subcontrol-position: top right;
            width: 15px;
            border-left-width: 1px;
            border-left-color: #E1E8ED;
            border-left-style: solid;
            border-top-right-radius: 3px;
            border-bottom-right-radius: 3px;
        }
        QComboBox::down-arrow {
            image: url(:/icons/down_arrow.png); /* Ensure you have a suitable icon */
            width: 10px;
            height: 10px;
        }
        QComboBox QAbstractItemView {
            background-color: #FFFFFF;
            border: 1px solid #E1E8ED;
            selection-background-color: #E6EDF5;
            selection-color: #333333;
        }
        QPushButton {
            background-color: #f7fbfc;
            color: #769fcd;
            border: 1px solid #8B8989;
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
        QLabel {
            background-color: #F3F4F6;
            color: #4B5563;
            font-size: 16px;
        }
    )");
    connect(btnPredict, &QPushButton::clicked, this, &forecast::onPredictClicked);
}

forecast::~forecast()
{
}

void forecast::onSelectCityClicked()
{
    // 选择对应城市
}

void forecast::onPredictClicked()
{
    api *forecast_api = new api;
    double data;

    connect(forecast_api, &api::predictionReady, this, &forecast::onPredictionReady);

    forecast_api->machine_learning(cityBox->currentText(), data);
}

void forecast::onPredictionReady(double data)
{
    qDebug() << "预测结果:" << data;
    // 在这里实现点击查询之后的预测算法
    // Example: labelImage->setPixmap(QPixmap(":/images/image.png"));
}
