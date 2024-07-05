#include "datavisualization.h"
#include "ui_datavisualization.h"
#include <QtCharts>
#include <QDebug>


datavisualization::datavisualization(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::datavisualization)
{
    ui->setupUi(this);

    //先连接数据库
    if (!connectToDatabase()) {
        QMessageBox::critical(this, "数据库连接失败", "数据可视化界面：无法连接到数据库，请检查配置。");
    }
    else
    {
        qDebug()<<"数据可视化界面界面成功连接数据库";
    }
    QSqlDatabase db=QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("127.0.0.1");
    db.setPort(3306);
    db.setDatabaseName("data"); // 设置数据库名
    db.setUserName("root"); // 设置用户名
    db.setPassword("123456"); // 设置密码
    db.open();

    QHBoxLayout *controlLayout = new QHBoxLayout();

    cityComboBox = new QComboBox(this);
    cityComboBox->addItems({"北京", "上海", "长沙","南京","杭州","武汉"});
    controlLayout->addWidget(new QLabel("选择城市:", this));
    controlLayout->addWidget(cityComboBox);

    YearComboBox = new QComboBox(this);
    YearComboBox->addItems({"2011","2012","2013","2014","2015","2016","2017","2018","2019","2020", "2021", "2022","2023","2024"});
    controlLayout->addWidget(new QLabel("选择年份:", this));
    controlLayout->addWidget(YearComboBox);

    MonthComboBox = new QComboBox(this);
    MonthComboBox->addItems({"1","2","3","4","5","6","7","8","9","10","11","12"});
    controlLayout->addWidget(new QLabel("选择月份:", this));
    controlLayout->addWidget(MonthComboBox);

    startDayComboBox = new QComboBox(this);
    startDayComboBox->addItems({"1","2","3","4","5","6","7","8","9","10","11","12","13","14","15","16","17","18","19","20","21","22","23","24","25","26","27","28","29","30","31"});
    controlLayout->addWidget(new QLabel("选择起始日期:", this));
    controlLayout->addWidget(startDayComboBox);

    endDayComboBox = new QComboBox(this);
    endDayComboBox->addItems({"1","2","3","4","5","6","7","8","9","10","11","12","13","14","15","16","17","18","19","20","21","22","23","24","25","26","27","28","29","30","31"});
    controlLayout->addWidget(new QLabel("选择中止日期:", this));
    controlLayout->addWidget(endDayComboBox);

    QPushButton *drawButton = new QPushButton("绘制", this);
    controlLayout->addWidget(drawButton);
    connect(drawButton, &QPushButton::clicked, this, &datavisualization::onDrawButtonClicked);  // Modify this line
    // connect(drawButton, &QPushButton::clicked, this, );

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
            background-color: #8B8989;
            color: #FFFFFF;
            font-size: 14px;
            cursor: pointer;
        }
        QPushButton:hover {
            background-color: #EEE5DE;
        }
        QPushButton:pressed {
            background-color: #CDC5BF;
        }
    )");



    // 将查询结果添加到数据系列中

    QString Qcity = cityComboBox->currentText();
    int Qyear = YearComboBox->currentText().toInt();
    int Qmonth = MonthComboBox->currentText().toInt();
    int Qstartday = startDayComboBox->currentText().toInt();
    int Qendday = endDayComboBox->currentText().toInt();


    //最高气温折线图
    QSqlQuery query;
    query.bindValue(":city", Qcity);
    query.bindValue(":year", Qyear);
    query.bindValue(":month", Qmonth);
    query.bindValue(":sday", Qstartday);
    query.bindValue(":eday", Qendday);


    query.exec("SELECT day, max_temperature FROM climate where city =:city and year =:year  and month = :month and day<=:sday and day>=:sday");
    series1 = new QLineSeries;
    while (query.next()) {
        int x_value = query.value(0).toInt();
        int y_value = query.value(1).toInt();
        series1->append(x_value, y_value);
    }



    //天气状况饼状图
    series2 = new QPieSeries;
    QSqlQuery query1;
    query1.bindValue(":city", Qcity);
    query1.bindValue(":year", Qyear);
    query1.bindValue(":month", Qmonth);
    query1.bindValue(":sday", Qstartday);
    query1.bindValue(":eday", Qendday);
    query1.exec("SELECT weather FROM climate  where city =:city and year =:year  and month = :month and day<=:sday and day>=:sday");
    int count1=0;  //阴
    int count2=0;  //小雨
    int count3=0;  //晴
    int count4=0;  //多云
    int count5=0;  //中雨
    int count6=0;  //雾~多云
    int count7=0;  //雾~晴
    int count8=0;  //晴~多云
    int count9=0;  //多云~晴
    int count10=0; //小雨~阴
    int count11=0; //雷阵雨~阵雨
    int count12=0; //雷阵雨~多云
    int count13=0; //多云~小雨
    int count14=0;//雷阵雨
    while (query1.next()) {

        QString weather = query1.value(0).toString();
        // 根据天气情况更新计数器
        if (weather == "阴") {
            count1++;
        } else if (weather == "小雨") {
            count2++;
        } else if (weather == "晴") {
            count3++;
        } else if (weather == "多云") {
            count4++;
        } else if (weather == "中雨") {
            count5++;
        } else if (weather == "雾~多云") {
            count6++;
        } else if (weather == "雾~晴") {
            count7++;
        } else if (weather == "晴~多云") {
            count8++;
        } else if (weather == "多云~晴") {
            count9++;
        } else if (weather == "小雨~阴") {
            count10++;
        } else if (weather == "雷阵雨~阵雨") {
            count11++;
        } else if (weather == "雷阵雨~多云") {
            count12++;
        }else if (weather == "多云~小雨") {
            count13++;
        }else if (weather == "雷阵雨") {
            count14++;
        }

    }
    series2->append("阴", count1);
    series2->append("小雨", count2);
    series2->append("晴", count3);
    series2->append("多云", count4);
    series2->append("中雨", count5);
    series2->append("雾~多云", count6);
    series2->append("雾~晴", count7);
    series2->append("晴~多云", count8);
    series2->append("多云~晴", count9);
    series2->append("小雨~阴", count10);
    series2->append("雷阵雨~阵雨", count11);
    series2->append("雷阵雨~多云", count12);
    series2->append("多云~小雨", count13);
    series2->append("雷阵雨", count14);


    //风向柱状图

    series3 = new QBarSeries;

    QSqlQuery query2;
    query2.bindValue(":city", Qcity);
    query2.bindValue(":year", Qyear);
    query2.bindValue(":month", Qmonth);
    query2.bindValue(":sday", Qstartday);
    query2.bindValue(":eday", Qendday);
    query2.exec("SELECT wind_direction FROM climate WHERE city = :city AND year = :year AND month = :month AND day BETWEEN :sday AND :eday");

    // 初始化八个风向的 QBarSet
    barset1 = new QBarSet("东风");
    barset2 = new QBarSet("西风");
    barset3 = new QBarSet("南风");
    barset4 = new QBarSet("北风");   
    barset5 = new QBarSet("东南风");
    barset6 = new QBarSet("东北风");
    barset7 = new QBarSet("西南风");
    barset8 = new QBarSet("西北风");

    // 初始化八个风向的风力计数器
    int wind1 = 0;  // 东风
    int wind2 = 0;  // 西风
    int wind3 = 0;  // 南风
    int wind4 = 0;  // 北风
    int wind5 = 0;  // 东南风
    int wind6 = 0;  // 东北风
    int wind7 = 0;  // 西南风
    int wind8 = 0;  // 西北风

    while (query2.next()) {
        QString wind_direction = query2.value(0).toString();
        // 根据风向更新计数器
        if (wind_direction == "东风") {
            wind1++;
        } else if (wind_direction == "西风") {
            wind2++;
        } else if (wind_direction == "南风") {
            wind3++;
        } else if (wind_direction == "北风") {
            wind4++;
        } else if (wind_direction == "东南风") {
            wind5++;
        } else if (wind_direction == "东北风") {
            wind6++;
        } else if (wind_direction == "西南风") {
            wind7++;
        } else if (wind_direction == "西北风") {
            wind8++;
        }
    }

    // 将风力计数器的值添加到对应的 QBarSet 中
    *barset1 << wind1;
    *barset2 << wind2;
    *barset3 << wind3;
    *barset4 << wind4;
    *barset5 << wind5;
    *barset6 << wind6;
    *barset7 << wind7;
    *barset8 << wind8;

    // 将八个风向的 QBarSet 添加到 QBarSeries 中
    series3->append(barset1);
    series3->append(barset2);
    series3->append(barset3);
    series3->append(barset4);
    series3->append(barset5);
    series3->append(barset6);
    series3->append(barset7);
    series3->append(barset8);


    //最低气温折线图

    QSqlQuery query3;
    query3.bindValue(":city", Qcity);
    query3.bindValue(":year", Qyear);
    query3.bindValue(":month", Qmonth);
    query3.bindValue(":sday", Qstartday);
    query3.bindValue(":eday", Qendday);


    query3.exec("SELECT day, min_temperature FROM climate where city =:city and year =:year  and month = :month and day<=:sday and day>=:sday");
    series4 = new QLineSeries;
    while (query3.next()) {
        int x_value = query.value(0).toInt();
        int y_value = query.value(1).toInt();
        series4->append(x_value, y_value);
    }




    // 创建图表并添加数据系列
    chart1 = new QChart();
    chart1->legend()->hide();
    chart1->addSeries(series1);
    chart1->createDefaultAxes();
    chart1->setTitle("最高气温折线图");

    chart2 = new QChart();
    chart2->legend()->hide();
    chart2->addSeries(series2);
    chart2->createDefaultAxes();
    chart2->setTitle("天气饼状图");

    chart3 = new QChart();
    chart3->legend()->hide();
    chart3->addSeries(series3);
    chart3->createDefaultAxes();
    chart3->setTitle("风向柱状图");

    chart4 = new QChart();
    chart4->legend()->hide();
    chart4->addSeries(series4);
    chart4->createDefaultAxes();
    chart4->setTitle("最低气温折线图");

    // 创建图表视图并设置渲染提示
    chartview1 = new QChartView(chart1);
    chartview1->setRenderHint(QPainter::Antialiasing);

    chartview2 = new QChartView(chart2);
    chartview2->setRenderHint(QPainter::Antialiasing);

    chartview3 = new QChartView(chart3);
    chartview3->setRenderHint(QPainter::Antialiasing);

    chartview4 = new QChartView(chart4);
    chartview4->setRenderHint(QPainter::Antialiasing);

    // 连接鼠标悬停事件信号与槽函数
    connect(series1, &QLineSeries::hovered, this, &datavisualization::updateTooltip1);
    connect(series2, &QPieSeries::hovered, this, &datavisualization::updateTooltip2);
    connect(series3, &QBarSeries::hovered, this, &datavisualization::updateTooltip3);

    // 布局设置
    QHBoxLayout *hlayout1 = new QHBoxLayout;
    hlayout1->addWidget(chartview1);
    hlayout1->addWidget(chartview2);

    QHBoxLayout *hlayout2 = new QHBoxLayout;
    hlayout2->addWidget(chartview3);
    hlayout2->addWidget(chartview4);

    QVBoxLayout *vlayout = new QVBoxLayout;
    vlayout->addLayout(controlLayout);
    vlayout->addLayout(hlayout1);
    vlayout->addLayout(hlayout2);
    // controlLayout

    this->setLayout(nullptr);
    this->setLayout(vlayout);


    // QObject::connect(cityComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
    //                  [=](int index) mutable {
    //                      cityidx = index;
    //                  });
    // QObject::connect(endYearComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
    //                  [=](int index) mutable {
    //                      endyearidx = index;
    //                  });
    // QObject::connect(startYearComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
    //                  [=](int index) mutable {
    //                      startyearidx = index;
    //                  });

    // 点击查询按钮时连接到槽函数
    // QObject::connect(drawButton, &QPushButton::clicked, this, &datavisualization::mydraw);
}

datavisualization::~datavisualization()
{
    delete ui;

}

//连接数据库
bool datavisualization::connectToDatabase()
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

// 点击绘制按钮时触发的槽函数
void datavisualization::onDrawButtonClicked() {
    int startDay = startDayComboBox->currentText().toInt();
    int endDay = endDayComboBox->currentText().toInt();

    if (startDay > endDay) {
        QMessageBox::warning(this, "日期错误", "起始日期不能大于中止日期，请重新选择！");
        return;
    }

    mydraw();
}

//根据数据绘制图表
void datavisualization::mydraw()
{
    // qDebug()<<cityComboBox->currentIndex()<<" "<<startYearComboBox->currentText()<<" "<<endYearComboBox->currentText();
}
//处理折现图的悬停信号
void datavisualization::updateTooltip1(QPointF point, bool state)
{
    if (state) {
        // 格式化提示信息
        QString tooltip = QString("X: %1, Y: %2").arg(point.x()).arg(point.y());
        // 设置提示信息
        QToolTip::showText(QCursor::pos(), tooltip);
    } else {
        // 如果鼠标不在数据点上，则隐藏提示信息
        QToolTip::hideText();
    }
}
//处理饼状图的悬停信号
void datavisualization::updateTooltip2(QPieSlice *slice, bool state)
{
    if (state && slice) {
        QString tooltip = QString("饼状图 - %1: %2").arg(slice->label()).arg(slice->value());
        // 显示提示信息
        QToolTip::showText(QCursor::pos(), tooltip);
    } else {
        // 如果鼠标不在数据点上，则隐藏提示信息
        QToolTip::hideText();
    }
}
//处理柱状图
void datavisualization::updateTooltip3(bool hovered, int index, QBarSet* barset)
{
    if (hovered && barset && index != -1) {
        QString tooltip = QString("柱状图 - %1: %2").arg(barset->label()).arg(barset->at(index));
        QToolTip::showText(QCursor::pos(), tooltip);
    } else {
        QToolTip::hideText();
    }
}


