#include "historyweather.h"
#include "ui_historyweather.h"
#include <QDebug>

int cityidx=0,yearidx=0,monthidx=0;
historyweather::historyweather(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::historyweather)
{
    ui->setupUi(this);

    vlayout = new QVBoxLayout(this); // 初始化垂直布局

    btn = new QPushButton("查询", this);  // 查询按钮
    cityBox = new QComboBox(this);
    yearBox = new QComboBox(this);
    monthBox = new QComboBox(this);
    weathertable = new QTableView(this);  // 天气表
    model = new QStandardItemModel(this);  // 数据模型

    model->setHorizontalHeaderLabels(QStringList() << "日期" << "天气" << "气温" << "风向" << "风力");  // 设置表头
    weathertable->setModel(model);

    cityBox->addItem("武汉");
    cityBox->addItem("北京");
    cityBox->addItem("上海");

    yearBox->addItem("2024");
    yearBox->addItem("2023");
    yearBox->addItem("2022");

    monthBox->addItem("一月");
    monthBox->addItem("二月");
    monthBox->addItem("三月");
    monthBox->addItem("四月");
    monthBox->addItem("五月");
    monthBox->addItem("六月");
    monthBox->addItem("七月");
    monthBox->addItem("八月");
    monthBox->addItem("九月");
    monthBox->addItem("十月");
    monthBox->addItem("十一月");
    monthBox->addItem("十二月");

    hlayout = new QHBoxLayout;
    hlayout->addWidget(cityBox);
    hlayout->addWidget(yearBox);
    hlayout->addWidget(monthBox);
    hlayout->addWidget(btn);

    vlayout->addLayout(hlayout);  // 添加水平布局到垂直布局
    vlayout->addWidget(weathertable);  // 添加表格到垂直布局

    setLayout(vlayout);  // 设置整体布局

    QObject::connect(cityBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
                     [=](int index) mutable {
                         cityidx = index;
                     });
    QObject::connect(yearBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
                     [=](int index) mutable {
                         yearidx = index;
                     });
    QObject::connect(monthBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
                     [=](int index) mutable {
                         monthidx = index;
                     });

    // 点击查询按钮时连接到槽函数
    QObject::connect(btn, &QPushButton::clicked, this, &historyweather::showweather);
}

void historyweather::showweather()
{
    // 模拟填充数据，实际中需要根据 cityidx、yearidx、monthidx 查询数据并填充 model
    qDebug() << "城市：" << cityBox->currentText() << "年份：" << yearBox->currentText() << "月份：" << monthBox->currentText();

    // 清空现有数据
    model->clear();
    model->setHorizontalHeaderLabels(QStringList() << "日期" << "天气" << "气温" << "风向" << "风力");

    // 模拟填充数据
    for (int row = 0; row < 10; ++row) {
        QList<QStandardItem *> rowData;
        rowData << new QStandardItem(QString("2024-01-%1").arg(row + 1))
                << new QStandardItem("晴")
                << new QStandardItem("25°C")
                << new QStandardItem("东风")
                << new QStandardItem("3级");
        model->appendRow(rowData);
    }
}

historyweather::~historyweather()
{
    delete ui;
}
