#include "todayweather.h"
#include "ui_todayweather.h"
#include <QDebug>

todayweather::todayweather(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::todayweather)
{
    ui->setupUi(this);

    //先连接到数据库
    if (!connectToDatabase()) {
        QMessageBox::critical(this, "数据库连接失败", "今日天气界面：无法连接到数据库，请检查配置。");
    }
    else
    {
        qDebug()<<"今日天气界面成功连接数据库";
    }


    cityBox = new QComboBox(this);
    cityBox->addItem("武汉");
    cityBox->addItem("北京");
    cityBox->addItem("上海");
    cityBox->setCurrentIndex(0);
    cityBox->setFixedSize(200, 40); // Adjusted size for better look

    QFont font1;
    font1.setPointSize(16); // Adjusted font size for better look
    cityBox->setFont(font1);

    // Custom stylesheet for beautification
    cityBox->setStyleSheet(
        "QComboBox {"
        "    border: 2px solid gray;"
        "    border-radius: 10px;"
        "    padding: 5px;"
        "    background: white;"
        "}"
        "QComboBox::drop-down {"
        "    subcontrol-origin: padding;"
        "    subcontrol-position: top right;"
        "    width: 30px;"
        "    border-left-width: 1px;"
        "    border-left-color: darkgray;"
        "    border-left-style: solid;"
        "    border-top-right-radius: 3px;"
        "    border-bottom-right-radius: 3px;"
        "}"
        "QComboBox::down-arrow {"
        "    image: url(:/icons/down_arrow.png);"
        "}"
        "QComboBox QAbstractItemView {"
        "    border: 2px solid gray;"
        "    selection-background-color: lightgray;"
        "}"
        );

    // 连接城市选择框的信号与槽
    connect(cityBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &todayweather::showweather);

    // Layout management
    topLayout = new QHBoxLayout();
    topLayout->addWidget(cityBox);
    topLayout->addStretch();

    mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(topLayout);
    mainLayout->addStretch();
    setLayout(mainLayout);





}

bool todayweather::connectToDatabase()
{
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setDatabaseName("data");
    db.setUserName("root");
    db.setPassword("123456");

    if (!db.open()) {
        qDebug() << "Database error occurred:" << db.lastError();
        return false;
    }
    return true;
}

void todayweather::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    QPainter painter(this);
    QPixmap weatherp(":/weather/test.png");
    painter.drawPixmap(150, 30, 550, 550, weatherp);
}

void todayweather::showweather(int index)
{
    // 这个函数是根据选择的城市得到对应的index数值，然后利用这个数值在数据库中查找相应的数据并反馈在页面上
    qDebug() << index;
}

todayweather::~todayweather()
{
    delete ui;
}
