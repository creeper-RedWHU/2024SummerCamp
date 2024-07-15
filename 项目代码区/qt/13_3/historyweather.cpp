#include "historyweather.h"
#include "ui_historyweather.h"
#include "api.h"
#include <QtSql/QSqlError>
#include <QtSql/QSqlRecord>
#include "database.h"
#include <QHeaderView>

historyweather::historyweather(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::historyweather)
{
    ui->setupUi(this);

    this->setLayout(nullptr);
    QVBoxLayout *vlayout = new QVBoxLayout(this); // 初始化垂直布局

    btn = new QPushButton("查询", this);  // 查询按钮
    cityBox = new QComboBox(this);
    yearBox = new QComboBox(this);
    monthBox = new QComboBox(this);

    weathertable = new QTableView(this);  // 天气表
    model = new QStandardItemModel(this);  // 数据模型

    model->setHorizontalHeaderLabels(QStringList() << "日期" << "天气" << "最高气温" << "最低气温" << "风向" << "风力");  // 设置表头

    weathertable->setModel(model);

    cityBox->addItem("武汉");
    cityBox->addItem("北京");
    cityBox->addItem("南京");
    cityBox->addItem("长沙");
    cityBox->addItem("杭州");
    cityBox->addItem("上海");

    yearBox->addItem("2024");
    yearBox->addItem("2023");
    yearBox->addItem("2022");
    yearBox->addItem("2021");
    yearBox->addItem("2020");
    yearBox->addItem("2019");
    yearBox->addItem("2018");
    yearBox->addItem("2017");
    yearBox->addItem("2016");
    yearBox->addItem("2015");
    yearBox->addItem("2014");
    yearBox->addItem("2013");
    yearBox->addItem("2012");
    yearBox->addItem("2011");

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

    QHBoxLayout *hlayout1 = new QHBoxLayout;
    hlayout1->addWidget(cityBox);
    hlayout1->addWidget(yearBox);
    hlayout1->addWidget(monthBox);
    hlayout1->addWidget(btn);
    QHBoxLayout *hlayout2 = new QHBoxLayout;
    hlayout2->addWidget(weathertable);

    vlayout->addLayout(hlayout1);  // 添加水平布局到垂直布局
    vlayout->addLayout(hlayout2);  // 添加水平布局到垂直布局

    this->setLayout(vlayout);  // 设置整体布局

    // 点击查询按钮时连接到槽函数
    QObject::connect(btn, &QPushButton::clicked, this, &historyweather::showweather);

    // Apply QSS stylesheet
    QString styleSheet = R"(
        /* General Widget Background */
        historyweather {
            background-color: #F7F9FC;
        }

        /* ComboBox Styling */
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

        /* PushButton Styling */
        QPushButton {
            background-color: #F7F9FC; /* 更改按钮颜色 */
            color: #769fcd;
            border: 1px solid #F7F9FC; /* 更改按钮边框颜色 */
            border-radius: 5px;
            padding: 10px 20px;
            font-family: "Arial", sans-serif;
            font-size: 14px;
            font-weight: bold;
        }

        QPushButton:hover {
            background-color: #d6e6f2; /* 更改按钮悬停颜色 */
            border: 1px solid #d6e6f2;
        }

        QPushButton:pressed {
            background-color: #b9d7ea; /* 更改按钮按下颜色 */
            border: 1px solid #b9d7ea;
        }

        /* TableView Styling */
        QTableView {
            background-color: #FFFFFF;
            border: 1px solid #E1E8ED;
            gridline-color: #E1E8ED;
            font-family: "Arial", sans-serif;
            font-size: 14px;
            color: #333333;
        }

        QHeaderView::section {
            background-color: #F1F3F5;
            color: #333333;
            padding: 5px;
            border: none;
            border-bottom: 1px solid #E1E8ED;
            border-right: 1px solid #E1E8ED;
        }

        QHeaderView::section:last {
            border-right: none;
        }

        QTableView::item {
            border-bottom: 1px solid #E1E8ED;
        }

        QTableView::item:selected {
            background-color: #E6EDF5;
            color: #333333;
        }
    )";
    this->setStyleSheet(styleSheet);

    // 设置表头占满整个行
    QHeaderView *header = weathertable->horizontalHeader();
    header->setSectionResizeMode(QHeaderView::Stretch);
}

void historyweather::showweather()
{
    database thisdb;
    //先连接数据库
    if (!thisdb.connectToDatabase()) {
        QMessageBox::critical(this, "数据库连接失败", "历史天气界面：无法连接到数据库，请检查配置。");
    }
    else
    {
        qDebug() << "历史天气界面成功连接数据库";
    }

    QString month;
    if(monthBox->currentText()=="一月")
    {
        month="1";
    }
    else if(monthBox->currentText()=="二月")
    {
        month="2";
    }
    else if(monthBox->currentText()=="三月")
    {
        month="3";
    }
    else if(monthBox->currentText()=="四月")
    {
        month="4";
    }
    else if(monthBox->currentText()=="五月")
    {
        month="5";
    }
    else if(monthBox->currentText()=="六月")
    {
        month="6";
    }
    else if(monthBox->currentText()=="七月")
    {
        month="7";
    }
    else if(monthBox->currentText()=="八月")
    {
        month="8";
    }
    else if(monthBox->currentText()=="九月")
    {
        month="9";
    }
    else if(monthBox->currentText()=="十月")
    {
        month="10";
    }
    else if(monthBox->currentText()=="十一月")
    {
        month="11";
    }
    else if(monthBox->currentText()=="十二月")
    {
        month="12";
    }
    qDebug()<<cityBox->currentText()<<yearBox->currentText()<<month<<yearBox->currentText()<<month;


    api *historyapi=new api;
    historyapi->fetch_data(cityBox->currentText(),yearBox->currentText(),month,yearBox->currentText(),month);


    // 清空现有数据
    model->clear();
    model->setHorizontalHeaderLabels(QStringList() << "日期" << "天气" << "最高气温" << "最低气温" << "风向" << "风力");

    // 查询数据库
    QSqlQuery query;
    query.prepare("SELECT year, month, day, weather, max_temperature, min_temperature, wind_direction, wind_strength "
                  "FROM climate WHERE city = :city AND year = :year AND month = :month");
    query.bindValue(":city", cityBox->currentText());
    query.bindValue(":year", yearBox->currentText().toInt());
    query.bindValue(":month", monthBox->currentIndex() + 1);  // monthBox的索引从0开始，实际月份从1开始

    if (!query.exec()) {
        qDebug() << "查询失败：" << query.lastError().text();
        return;
    }

    while (query.next()) {
        QList<QStandardItem *> rowData;
        rowData << new QStandardItem(QString("%1-%2-%3").arg(query.value("year").toInt())
                                         .arg(query.value("month").toInt(), 2, 10, QLatin1Char('0'))
                                         .arg(query.value("day").toInt(), 2, 10, QLatin1Char('0')));
        rowData << new QStandardItem(query.value("weather").toString());
        rowData << new QStandardItem(query.value("max_temperature").toString());
        rowData << new QStandardItem(query.value("min_temperature").toString());
        rowData << new QStandardItem(query.value("wind_direction").toString());
        rowData << new QStandardItem(query.value("wind_strength").toString());

        model->appendRow(rowData);
    }
}

historyweather::~historyweather()
{
    delete ui;
}
