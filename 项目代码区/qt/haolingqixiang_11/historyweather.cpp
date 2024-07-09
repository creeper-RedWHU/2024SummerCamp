#include "historyweather.h"
#include "ui_historyweather.h"
#include <QDebug>
#include <QtSql/QSqlError>
#include <QtSql/QSqlRecord>
#include "database.h"

int cityidx=0, yearidx=0, monthidx=0;

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

    QHBoxLayout *hlayout = new QHBoxLayout;
    hlayout->addWidget(cityBox);
    hlayout->addWidget(yearBox);
    hlayout->addWidget(monthBox);
    hlayout->addWidget(btn);

    vlayout->addLayout(hlayout);  // 添加水平布局到垂直布局
    vlayout->addWidget(weathertable);  // 添加表格到垂直布局

    this->setLayout(vlayout);  // 设置整体布局

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

    // 初始化数据库连接
    // initDB();

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
            background-color: #8B8989;
            color: #FFFFFF;
            border: 1px solid #8B8989;
            border-radius: 5px;
            padding: 10px 20px;
            font-family: "Arial", sans-serif;
            font-size: 14px;
            font-weight: bold;
        }

        QPushButton:hover {
            background-color: #EEE5DE;
            border: 1px solid #EEE5DE;
        }

        QPushButton:pressed {
            background-color: #CDC5BF;
            border: 1px solid #CDC5BF;
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
}

// void historyweather::initDB()
// {
//     // db = QSqlDatabase::addDatabase("QMYSQL");
//     // db.setHostName("60.205.232.122");  // 根据实际情况设置
//     // db.setDatabaseName("data");  // 替换为你的数据库名
//     // db.setUserName("root");  // 替换为你的数据库用户名
//     // db.setPassword("123456");  // 替换为你的数据库密码

//     QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");// 建立和QSQLITE数据库的连接
//     db.setHostName("127.0.0.1");  //连接本地主机
//     db.setPort(3306);
//     db.setUserName("root");
//     //设置数据库的密码
//     db.setPassword("mt127715318");    //这个就是安装MySQL时设置的密码
//     db.setDatabaseName("/Users/motao/demo1.db");//设置数据库名称
//     if (!db.open()) {
//         qDebug() << "历史天气无法连接到数据库：" << db.lastError().text();
//     } else {
//         qDebug() << "历史天气成功连接到数据库";
//     }
// }

void historyweather::showweather()
{
    // initDB();
    database thisdb;
    //先连接数据库
    if (!thisdb.connectToDatabase()) {
        QMessageBox::critical(this, "数据库连接失败", "历史天气界面：无法连接到数据库，请检查配置。");
    }
    else
    {
        qDebug()<<"历史天气界面成功连接数据库";
    }

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
