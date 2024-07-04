#include "todayweather.h"
#include "ui_todayweather.h"

todayweather::todayweather(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::todayweather)
{
    ui->setupUi(this);

    cityBox = new QComboBox(this);
    cityBox->addItem("武汉");
    cityBox->addItem("北京");
    cityBox->addItem("上海");
    cityBox->setCurrentIndex(0);
    cityBox->setFixedSize(150, 80);
    QFont font1;
    font1.setPointSize(24);
    cityBox->setFont(font1);

    // 连接城市选择框的信号与槽
    QObject::connect(cityBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
                     [=](int index){
                         // 当用户更改选择时，执行显示天气函数
                         showweather(index);
                     });
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
    qDebug()  << index;
}

todayweather::~todayweather()
{
    delete ui;
}
