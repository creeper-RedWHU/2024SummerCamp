#include "datavisualization.h"
#include "ui_datavisualization.h"
#include <QtCharts>
#include <QDebug>

datavisualization::datavisualization(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::datavisualization)
{
    ui->setupUi(this);
    // database thisdb;
    // 初始化数据库连接
    // if (!thisdb.connectToDatabase()) {
    //     QMessageBox::critical(this, "登录界面数据库连接失败", "无法连接到数据库。");
    //     return;
    // }

    connectToDatabase();
    // 先连接数据库
    if (!connectToDatabase()) {
        QMessageBox::critical(this, "数据库连接失败", "数据可视化界面：无法连接到数据库，请检查配置。");
    }

    QHBoxLayout *controlLayout = new QHBoxLayout();

    cityComboBox = new QComboBox(this);
    cityComboBox->addItems({"北京", "上海", "长沙", "南京", "杭州", "武汉"});
    QHBoxLayout *cityLayout = new QHBoxLayout();
    cityLayout->addWidget(new QLabel("选择城市:", this));
    cityLayout->addWidget(cityComboBox);
    controlLayout->addLayout(cityLayout);

    YearComboBox = new QComboBox(this);
    YearComboBox->addItems({"2011", "2012", "2013", "2014", "2015", "2016", "2017", "2018", "2019", "2020", "2021", "2022", "2023", "2024"});
    QHBoxLayout *yearLayout = new QHBoxLayout();
    yearLayout->addWidget(new QLabel("选择年份:", this));
    yearLayout->addWidget(YearComboBox);
    controlLayout->addLayout(yearLayout);

    MonthComboBox = new QComboBox(this);
    MonthComboBox->addItems({"1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12"});
    QHBoxLayout *monthLayout = new QHBoxLayout();
    monthLayout->addWidget(new QLabel("选择月份:", this));
    monthLayout->addWidget(MonthComboBox);
    controlLayout->addLayout(monthLayout);

    startDayComboBox = new QComboBox(this);
    startDayComboBox->addItems({"1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "30", "31"});
    QHBoxLayout *startDayLayout = new QHBoxLayout();
    startDayLayout->addWidget(new QLabel("选择起始日期:", this));
    startDayLayout->addWidget(startDayComboBox);
    controlLayout->addLayout(startDayLayout);

    endDayComboBox = new QComboBox(this);
    endDayComboBox->addItems({"1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "30", "31"});
    QHBoxLayout *endDayLayout = new QHBoxLayout();
    endDayLayout->addWidget(new QLabel("选择中止日期:", this));
    endDayLayout->addWidget(endDayComboBox);
    controlLayout->addLayout(endDayLayout);

    QPushButton *drawButton = new QPushButton("绘制", this);
    controlLayout->addWidget(drawButton);
    connect(drawButton, &QPushButton::clicked, this, &datavisualization::onDrawButtonClicked);  // Modify this line

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
            background-color: #f7fbfc;
            color: #769fcd;
            font-size: 14px;
            cursor: pointer;
        }
        QPushButton:hover {
            background-color: #d6e6f2;
        }
        QPushButton:pressed {
            background-color: #b9d7ea;
        }
    )");

    series1 = new QLineSeries; // 最高气温折线
    series4 = new QLineSeries; // 最低气温折线
    series2 = new QPieSeries; // 天气饼状图
    series3 = new QBarSeries; // 风向柱状图
    series5 = new QLineSeries; // 温差折线
    series6 = new QPieSeries; // 风力饼状图

    scatterSeries1 = new QScatterSeries(); // 最高气温散点
    scatterSeries4 = new QScatterSeries(); // 最低气温散点
    scatterSeries5 = new QScatterSeries(); // 温差

    // 创建图表并添加数据系列
    chart1 = new QChart();
    chart1->legend()->hide();
    chart1->addSeries(series1);
    chart1->createDefaultAxes();
    chart1->setTitle("气温折线图");

    chart4 = new QChart();
    chart4->legend()->hide();
    chart4->addSeries(series5);
    chart4->createDefaultAxes();
    chart4->setTitle("温差折线图");

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

    chart6 = new QChart();
    chart6->legend()->hide();
    chart6->addSeries(series6);
    chart6->createDefaultAxes();
    chart6->setTitle("风力饼状图");

    // 创建图表视图并设置渲染提示
    chartview1 = new QChartView(chart1);
    chartview1->setRenderHint(QPainter::Antialiasing);

    chartview4 = new QChartView(chart4);
    chartview4->setRenderHint(QPainter::Antialiasing);

    chartview2 = new QChartView(chart2);
    chartview2->setRenderHint(QPainter::Antialiasing);

    chartview3 = new QChartView(chart3);
    chartview3->setRenderHint(QPainter::Antialiasing);

    chartview6 = new QChartView(chart6);
    chartview6->setRenderHint(QPainter::Antialiasing);


    // 连接鼠标悬停事件信号与槽函数
    connect(series2, &QPieSeries::hovered, this, &datavisualization::updateTooltip2);
    connect(series3, &QBarSeries::hovered, this, &datavisualization::updateTooltip3);
    connect(scatterSeries1, &QScatterSeries::hovered, this, &datavisualization::updateTooltip4);
    connect(scatterSeries4, &QScatterSeries::hovered, this, &datavisualization::updateTooltip5);
    connect(scatterSeries5, &QScatterSeries::hovered, this, &datavisualization::updateTooltip6);
    connect(series6, &QPieSeries::hovered, this, &datavisualization::updateTooltip7);
    // 添加四个按钮
    btn1 = new QPushButton("气温分析", this);
    btn2 = new QPushButton("温差分析", this);
    btn3 = new QPushButton("天气分析", this);
    btn4 = new QPushButton("风向分析", this);
    btn5 = new QPushButton("风力分析", this);

    // 设置新的按钮垂直布局
    QVBoxLayout *btnlayout = new QVBoxLayout;
    btnlayout->addWidget(btn1);
    btnlayout->addWidget(btn2);
    btnlayout->addWidget(btn3);
    btnlayout->addWidget(btn4);
    btnlayout->addWidget(btn5);

    // 设置图表布局
    stackedWidget = new QStackedWidget(this);
    stackedWidget->addWidget(chartview1);
    stackedWidget->addWidget(chartview2);
    stackedWidget->addWidget(chartview3);
    stackedWidget->addWidget(chartview4);
    stackedWidget->addWidget(chartview6);

    // 添加 QLabel 到图表布局下面
    infoLabel = new QLabel("这是一些描述文本。", this);  // Make infoLabel a member variable

    // 连接按钮点击事件与图表切换和 Qlabel 内容修改
    updateButtonStyle(btn1);
    connect(btn1, &QPushButton::clicked, this, [this]() {
        updateButtonStyle(btn1);
        btn1onclicked();
    });
    connect(btn2, &QPushButton::clicked, this, [this]() {
        updateButtonStyle(btn2);
        btn2onclicked();
    });
    connect(btn3, &QPushButton::clicked, this, [this]() {
        updateButtonStyle(btn3);
        btn3onclicked();
    });
    connect(btn4, &QPushButton::clicked, this, [this]() {
        updateButtonStyle(btn4);
        btn4onclicked();
    });
    connect(btn5, &QPushButton::clicked, this, [this]() {
        updateButtonStyle(btn5);
        btn5onclicked();
    });

    connect(drawButton, &QPushButton::clicked, this, &datavisualization::btn1onclicked);
    connect(drawButton, &QPushButton::clicked, this, &datavisualization::btn2onclicked);
    connect(drawButton, &QPushButton::clicked, this, &datavisualization::btn3onclicked);
    connect(drawButton, &QPushButton::clicked, this, &datavisualization::btn4onclicked);
    connect(drawButton, &QPushButton::clicked, this, &datavisualization::btn5onclicked);
    //再次点击绘制的时候返回界面1
    connect(drawButton, &QPushButton::clicked, this, [this]() {
        updateButtonStyle(btn1);
        btn1onclicked();
    });


    // 布局设置
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addLayout(btnlayout);
    QVBoxLayout *chartLayout = new QVBoxLayout;  // Create a new layout for chart and label
    chartLayout->addWidget(stackedWidget);
    chartLayout->addWidget(infoLabel);
    mainLayout->addLayout(chartLayout);  // Add the new layout to the main layout
    infoLabel->setText(" ");


    QVBoxLayout *vlayout = new QVBoxLayout;
    vlayout->addLayout(controlLayout);
    vlayout->addLayout(mainLayout);

    setLayout(vlayout);
}

void datavisualization::updateChartAndLabel(QChartView *chartview, const QString &text) {
    stackedWidget->setCurrentWidget(chartview);
    infoLabel->setText(text);
}


datavisualization::~datavisualization()
{
    delete ui;

}
void datavisualization::btn1onclicked()
{
    QString Qcity = cityComboBox->currentText();
    int Qyear = YearComboBox->currentText().toInt();
    int Qmonth = MonthComboBox->currentText().toInt();
    int Qstartday = startDayComboBox->currentText().toInt();
    int Qendday = endDayComboBox->currentText().toInt();

    QSqlQuery query;  // 使用现有的数据库连接
    query.prepare("SELECT day,max_temperature,min_temperature FROM climate WHERE city = :city AND year = :year AND month = :month AND day <= :eday AND day >= :sday");
    query.bindValue(":city", Qcity);
    query.bindValue(":year", Qyear);
    query.bindValue(":month", Qmonth);
    query.bindValue(":sday", Qstartday);
    query.bindValue(":eday", Qendday);

    if (!query.exec()) {
        //qDebug() << "Query execution failed:" << query.lastError().text();
    }

    int maxtem_max = -100; // 最高气温的最高值
    QString maxtem_max_days; // 最高气温的最高值对应的日期（多个日期）
    int maxtem_min = 100; // 最高气温的最低值
    QString maxtem_min_days; // 最高气温的最低值对应的日期（多个日期）
    int mintem_max = -100; // 最低气温的最高值
    QString mintem_max_days; // 最低气温的最高值对应的日期（多个日期）
    int mintem_min = 100; // 最低气温的最低值
    QString mintem_min_days; // 最低气温的最低值对应的日期（多个日期）
    int total_max_temperature = 0; // 总最高气温
    int total_min_temperature = 0; // 总最低气温
    int count = 0; // 计数器，用于统计数据数量

    while (query.next()) {
        int day = query.value("day").toInt(); // 获取天数
        int y1_value = query.value("max_temperature").toInt();
        int y2_value = query.value("min_temperature").toInt();

        // 更新最高气温的最高值及其对应日期
        if (y1_value > maxtem_max) {
            maxtem_max = y1_value;
            maxtem_max_days = QString::number(day);
        } else if (y1_value == maxtem_max) {
            maxtem_max_days += ", " + QString::number(day);
        }

        // 更新最高气温的最低值及其对应日期
        if (y1_value < maxtem_min) {
            maxtem_min = y1_value;
            maxtem_min_days = QString::number(day);
        } else if (y1_value == maxtem_min) {
            maxtem_min_days += ", " + QString::number(day);
        }

        // 更新最低气温的最高值及其对应日期
        if (y2_value > mintem_max) {
            mintem_max = y2_value;
            mintem_max_days = QString::number(day);
        } else if (y2_value == mintem_max) {
            mintem_max_days += ", " + QString::number(day);
        }

        // 更新最低气温的最低值及其对应日期
        if (y2_value < mintem_min) {
            mintem_min = y2_value;
            mintem_min_days = QString::number(day);
        } else if (y2_value == mintem_min) {
            mintem_min_days += ", " + QString::number(day);
        }

        // 累加总最高气温和总最低气温
        total_max_temperature += y1_value;
        total_min_temperature += y2_value;
        count++;
    }

    // 计算平均最高气温和平均最低气温
    double average_max_temperature = static_cast<double>(total_max_temperature) / count;
    double average_min_temperature = static_cast<double>(total_min_temperature) / count;

    // 构建显示文本
    QString labelText1 = Qcity + "市" + QString::number(Qyear) + "年" + QString::number(Qmonth) + "月" + QString::number(Qstartday) + "日" + "～" + QString::number(Qendday) + "日气温数据分析：\n"
                         + "最高气温的最高值为 " + QString::number(maxtem_max) + "°C，出现在" + maxtem_max_days + "日\n"
                         + "最高气温的最低值为 " + QString::number(maxtem_min) + "°C，出现在" + maxtem_min_days + "日\n"
                         + "最低气温的最高值为 " + QString::number(mintem_max) + "°C，出现在" + mintem_max_days + "日\n"
                         + "最低气温的最低值为 " + QString::number(mintem_min) + "°C，出现在" + mintem_min_days + "日\n"
                         + "平均最高气温为 " + QString::number(average_max_temperature, 'f', 2) + "°C；"+ "平均最低气温为 " + QString::number(average_min_temperature, 'f', 2) + "°C\n";

    updateChartAndLabel(chartview1, labelText1);



}
void datavisualization::btn2onclicked()
{


    QString Qcity = cityComboBox->currentText();
    int Qyear = YearComboBox->currentText().toInt();
    int Qmonth = MonthComboBox->currentText().toInt();
    int Qstartday = startDayComboBox->currentText().toInt();
    int Qendday = endDayComboBox->currentText().toInt();

    QSqlQuery query;
    query.prepare("SELECT day, max_temperature, min_temperature FROM climate WHERE city = :city AND year = :year AND month = :month AND day <= :eday AND day >= :sday");
    query.bindValue(":city", Qcity);
    query.bindValue(":year", Qyear);
    query.bindValue(":month", Qmonth);
    query.bindValue(":sday", Qstartday);
    query.bindValue(":eday", Qendday);
    query.exec();

    // 初始化变量用于计算温差和日期
    double maxTempDiff = -std::numeric_limits<double>::infinity();
    double minTempDiff = std::numeric_limits<double>::infinity();
    QString maxTempDates;
    QString minTempDates;
    double totalTempDiff = 0.0;
    int count = 0;

    // 遍历查询结果
    while (query.next()) {
        int day = query.value(0).toInt();
        double maxTemp = query.value(1).toDouble();
        double minTemp = query.value(2).toDouble();

        double tempDiff = maxTemp - minTemp;

        // 计算最高温差和对应日期
        if (tempDiff > maxTempDiff) {
            maxTempDiff = tempDiff;
            maxTempDates = QString::number(day);
        } else if (tempDiff == maxTempDiff) {
            maxTempDates += ", " + QString::number(day);
        }

        // 计算最低温差和对应日期
        if (tempDiff < minTempDiff) {
            minTempDiff = tempDiff;
            minTempDates = QString::number(day);
        } else if (tempDiff == minTempDiff) {
            minTempDates += ", " + QString::number(day);
        }

        // 累加温差，用于计算平均值
        totalTempDiff += tempDiff;
        count++;
    }

    // 计算平均温差
    double averageTempDiff = totalTempDiff / count;

    // 构建最终的文本信息
    QString labelText4 = Qcity + "市" + QString::number(Qyear) + "年" + QString::number(Qmonth) + "月" + QString::number(Qstartday) + "～" + QString::number(Qendday) + "日温差数据分析：\n";
    labelText4 += "最高温差为" + QString::number(maxTempDiff) + " °C，出现在 " + maxTempDates + "日\n";
    labelText4 += "最低温差为" + QString::number(minTempDiff) + " °C，出现在 " + minTempDates + "日\n";
    labelText4 += "平均温差为" + QString::number(averageTempDiff) + " °C";



    updateChartAndLabel(chartview4, labelText4);
}
void datavisualization::btn3onclicked()
{
    QString Qcity = cityComboBox->currentText();
    int Qyear = YearComboBox->currentText().toInt();
    int Qmonth = MonthComboBox->currentText().toInt();
    int Qstartday = startDayComboBox->currentText().toInt();
    int Qendday = endDayComboBox->currentText().toInt();

    QSqlQuery query1(db);
    query1.prepare("SELECT weather FROM climate WHERE city = :city AND year = :year AND month = :month AND day <= :eday AND day >= :sday");
    query1.bindValue(":city", Qcity);
    query1.bindValue(":year", Qyear);
    query1.bindValue(":month", Qmonth);
    query1.bindValue(":sday", Qstartday);
    query1.bindValue(":eday", Qendday);



    // 执行查询语句
    if (!query1.exec()) {
        //qDebug() << "Query execution failed. Error:" << query1.lastError().text();
    } else {
        //qDebug() << "Query executed successfully.";

        QMap<QString, int> weatherCounts; // 使用QMap来记录天气及其出现次数

        while (query1.next()) {
            QString weather = query1.value(0).toString();

            // 检查天气是否已经在Map中存在，如果存在，则递增计数，否则添加新记录
            if (weatherCounts.contains(weather)) {
                weatherCounts[weather]++;
            } else {
                weatherCounts.insert(weather, 1);
            }
        }


        int total=Qendday-Qstartday+1;
        // 构造分析结果文本
        QString labelText2 = Qcity + "市" + QString::number(Qyear) + "年" + QString::number(Qmonth) + "月"
                             + QString::number(Qstartday) + "～" + QString::number(Qendday) + "日天气分析：\n";

        labelText2 += "天数大于3天的天气有：\n";
        bool isFirstHighFrequency = true; // 用于控制逗号的输出
        bool havethree=0;
        for (auto it = weatherCounts.constBegin(); it != weatherCounts.constEnd(); ++it) {
            if (it.value() > 3) { // 大于3天的天气类型
                if (!isFirstHighFrequency) {
                    labelText2 += "； ";
                }
                labelText2 += it.key() + "：" + QString::number(it.value()) + "天";
                isFirstHighFrequency = false;
                havethree=1;
            }
        }
        if(!havethree)
        {
            labelText2 += "没有天数大于3的天气 ";
        }

        // 找出出现次数最高的天气类型
        QStringList mostFrequentWeathers;
        int maxCount = 0;

        for (auto it = weatherCounts.constBegin(); it != weatherCounts.constEnd(); ++it) {
            if (it.value() > maxCount) {
                maxCount = it.value();
                mostFrequentWeathers.clear();
                mostFrequentWeathers.append(it.key());
            } else if (it.value() == maxCount) {
                mostFrequentWeathers.append(it.key());
            }
        }

        // 构造出现频率最高的天气类型文本
        labelText2 += "\n出现频率最高的天气类型：\n";
        for (const QString &weather : mostFrequentWeathers) {
            labelText2 += weather+" " ;
        }

        double percentage = (static_cast<double>(maxCount) / total) * 100.0;
        labelText2 += "：" + QString::number(maxCount) + "天，出现频率为"+ QString::number(percentage, 'f', 2) + "%";

        updateChartAndLabel(chartview2, labelText2);

    }
}
void datavisualization::btn4onclicked()
{
    QString Qcity = cityComboBox->currentText();
    int Qyear = YearComboBox->currentText().toInt();
    int Qmonth = MonthComboBox->currentText().toInt();
    int Qstartday = startDayComboBox->currentText().toInt();
    int Qendday = endDayComboBox->currentText().toInt();

    QSqlQuery query2;
    query2.prepare("SELECT wind_direction FROM climate WHERE city = :city AND year = :year AND month = :month AND day BETWEEN :sday AND :eday");
    query2.bindValue(":city", Qcity);
    query2.bindValue(":year", Qyear);
    query2.bindValue(":month", Qmonth);
    query2.bindValue(":sday", Qstartday);
    query2.bindValue(":eday", Qendday);
    query2.exec();

    // 初始化八个风向的风力计数器
    int wind1 = 0;  // 东风
    int wind2 = 0;  // 西风
    int wind3 = 0;  // 南风
    int wind4 = 0;  // 北风
    int wind5 = 0;  // 东南风
    int wind6 = 0;  // 东北风
    int wind7 = 0;  // 西南风
    int wind8 = 0;  // 西北风
    int wind9 = 0;  // 微风

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
        } else if (wind_direction == "微风") {
            wind9++;
        }else if (wind_direction == "无持续风向") {
            wind9++;
        }
        // 统计每种风向的天数
        int maxDays = 0;
        QString maxWindNames;  // 使用 QString 来保存多个风向名称
        QString labelText3 = Qcity + "市" + QString::number(Qyear) + "年" + QString::number(Qmonth) + "月" + QString::number(Qstartday) + "～" + QString::number(Qendday) + "日风向分析：\n";

        // 风向数组，按照你的顺序排列
        QString windNames[9] = {"东风", "西风", "南风", "北风", "东南风", "东北风", "西南风", "西北风", "微风/无持续风向"};

        int windCounts[9] = {wind1, wind2, wind3, wind4, wind5, wind6, wind7, wind8, wind9};

        // 找出出现天数最多的风向
        for (int i = 0; i < 9; ++i) {
            if (windCounts[i] > maxDays) {
                maxDays = windCounts[i];
                maxWindNames = windNames[i];  // 更新最大天数风向名称
            } else if (windCounts[i] == maxDays) {
                maxWindNames += "，" + windNames[i];  // 多个风向名称用逗号隔开
            }
        }

        // 加入出现天数最多的风向的信息
        labelText3 += "出现天数最多的风向是：" + maxWindNames + "，有 " + QString::number(maxDays) + " 天\n";


        if(!maxWindNames.contains("微风")&&Qendday-Qstartday+1>10)
        {
            labelText3 +="这段时间"+ maxWindNames+"较多"+"，气温可能";
            // 加入气温分析的内容
            if (maxWindNames.contains("东风")) {
                labelText3 += "较暖哦";
            }
            if (maxWindNames.contains("西风")) {
                labelText3 += "较凉哦";
            }
            if (maxWindNames.contains("南风")) {
                labelText3 += "较暖湿哦";
            }
            if (maxWindNames.contains("北风")) {
                labelText3 += "较凉哦";
            }
        }

        updateChartAndLabel(chartview3, labelText3);
    }
}


void datavisualization::btn5onclicked()
{
    QString Qcity = cityComboBox->currentText();
    int Qyear = YearComboBox->currentText().toInt();
    int Qmonth = MonthComboBox->currentText().toInt();
    int Qstartday = startDayComboBox->currentText().toInt();
    int Qendday = endDayComboBox->currentText().toInt();

    QSqlQuery query1(db);
    query1.prepare("SELECT wind_strength FROM climate WHERE city = :city AND year = :year AND month = :month AND day <= :eday AND day >= :sday");
    query1.bindValue(":city", Qcity);
    query1.bindValue(":year", Qyear);
    query1.bindValue(":month", Qmonth);
    query1.bindValue(":sday", Qstartday);
    query1.bindValue(":eday", Qendday);



    // 执行查询语句
    if (!query1.exec()) {
        //qDebug() << "Query execution failed. Error:" << query1.lastError().text();
    } else {
        //qDebug() << "Query executed successfully.";

        QMap<QString, int> wind_strengthCounts; // 使用QMap来记录风力及其出现次数
        int windyDaysCount = 0; // 记录风力大于等于3级的天数

        while (query1.next()) {
            QString wind_strength = query1.value(0).toString();

            // 检查风力是否已经在Map中存在，如果存在，则递增计数，否则添加新记录
            if (wind_strengthCounts.contains(wind_strength)) {
                wind_strengthCounts[wind_strength]++;
            } else {
                wind_strengthCounts.insert(wind_strength, 1);
            }

            // 计算风力大于等于3级的天数
            QString windLevel = wind_strength[0];
            if (windLevel >= "3"&&windLevel <= "9") {
                windyDaysCount++;
            }
        }

        int total = Qendday - Qstartday + 1;
        // 构造分析结果文本
        QString labelText6 = Qcity + "市" + QString::number(Qyear) + "年" + QString::number(Qmonth) + "月"
                             + QString::number(Qstartday) + "～" + QString::number(Qendday) + "日风力分析：\n";

        // 找出出现次数最高的风力类型
        QStringList mostFrequentWindStrengths;
        int maxCount = 0;

        for (auto it = wind_strengthCounts.constBegin(); it != wind_strengthCounts.constEnd(); ++it) {
            if (it.value() > maxCount) {
                maxCount = it.value();
                mostFrequentWindStrengths.clear();
                mostFrequentWindStrengths.append(it.key());
            } else if (it.value() == maxCount) {
                mostFrequentWindStrengths.append(it.key());
            }
        }

        // 构造出现频率最高的风力类型文本
        labelText6 += "出现频率最高的风力类型：\n";
        for (const QString &wind_strength : mostFrequentWindStrengths) {
            labelText6 += wind_strength + " ";
        }

        double percentage = (static_cast<double>(maxCount) / total) * 100.0;
        labelText6 += "：" + QString::number(maxCount) + "天，出现频率为" + QString::number(percentage, 'f', 2) + "%\n";

        // 显示风力大于等于3级的天数
        labelText6 += "风力大于等于3级的天数：" + QString::number(windyDaysCount)+"天";

        updateChartAndLabel(chartview6, labelText6);
    }


}


//连接数据库
bool datavisualization::connectToDatabase()
{
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("60.205.232.122");
    db.setDatabaseName("data");
    db.setUserName("root");
    db.setPassword("QAZ123wsx");

    // QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");// 建立和QSQLITE数据库的连接
    // db.setHostName("127.0.0.1");  //连接本地主机
    // db.setPort(3306);
    // db.setUserName("root");
    // //设置数据库的密码
    // db.setPassword("mt127715318");    //这个就是安装MySQL时设置的密码
    // db.setDatabaseName("/Users/motao/demo1.db");//设置数据库名称

    if (!db.open()) {
        //qDebug() << "Database error occurred:" << db.lastError();
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
    // connectToDatabase();


    QString Qcity = cityComboBox->currentText();
    int Qyear = YearComboBox->currentText().toInt();
    int Qmonth = MonthComboBox->currentText().toInt();
    int Qstartday = startDayComboBox->currentText().toInt();
    int Qendday = endDayComboBox->currentText().toInt();

    // 查询数据库获取最高气温数据
    QSqlQuery query;  // 使用现有的数据库连接
    query.prepare("SELECT day,max_temperature,min_temperature FROM climate WHERE city = :city AND year = :year AND month = :month AND day <= :eday AND day >= :sday");
    query.bindValue(":city", Qcity);
    query.bindValue(":year", Qyear);
    query.bindValue(":month", Qmonth);
    query.bindValue(":sday", Qstartday);
    query.bindValue(":eday", Qendday);

    if (!query.exec()) {
        //qDebug() << "Query execution failed:" << query.lastError().text();
    }



    // 清空原有数据
    series1->clear();
    scatterSeries1->clear();
    series4->clear();
    scatterSeries4->clear();


    // 清空原有散点数据
    foreach (QAbstractSeries *s, chart1->series()) {
        chart1->removeSeries(s);
    }

    // 填充新数据，同时添加散点数据

    scatterSeries1->setMarkerShape(QScatterSeries::MarkerShapeCircle); // 设置散点形状为圆形
    scatterSeries1->setMarkerSize(9); // 设置散点大小为 4.5 像素
    QValueAxis *xAxis = new QValueAxis;
    QValueAxis *yAxis = new QValueAxis();

    scatterSeries4->setMarkerShape(QScatterSeries::MarkerShapeCircle); // 设置散点形状为圆形
    scatterSeries4->setMarkerSize(9); // 设置散点大小为 4.5 像素




    int minY = 100;  // 初始化为一个非常大的数，作为最小值
    int maxY = -100;  // 初始化为一个非常小的数，作为最大值
    int minX = 100;  // 初始化为一个非常大的数，作为最小值
    int maxX = -100;  // 初始化为一个非常小的数，作为最大值

    while (query.next()) {
        int day = query.value("day").toInt(); // 获取天数
        int y1_value = query.value("max_temperature").toInt();
        int y2_value = query.value("min_temperature").toInt();
        series1->append(day, y1_value); // 添加折线图数据
        scatterSeries1->append(day, y1_value); // 添加散点数据
        series4->append(day, y2_value); // 添加折线图数据
        scatterSeries4->append(day, y2_value); // 添加散点数据

        // 更新最小和最大温度值
        if (y2_value < minY) {
            minY = y2_value;
        }
        if (y1_value > maxY) {
            maxY = y1_value;
        }

        // 更新最小和最大天数
        if (day < minX) {
            minX = day;
        }
        if (day > maxX) {
            maxX = day;
        }
    }
    xAxis->setRange(minX-0.5, maxX+0.5);
    yAxis->setRange(minY-2, maxY+2);

    series1->setName("最高气温折线");// 设置名称
    series4->setName("最低气温折线");
    scatterSeries1->setName("最高气温散点");
    scatterSeries4->setName("最低气温散点");

    QPen maxTempPen(Qt::red); // 最高气温折线颜色为红色
    QPen minTempPen(Qt::blue); // 最低气温折线颜色为蓝色

    QPen maxTempPointpen(Qt::green);
    QPen minTempPointpen(Qt::green);

    scatterSeries1->setPen(maxTempPointpen);
    scatterSeries4->setPen(minTempPointpen);
    series1->setPen(maxTempPen); // 设置最高气温折线系列的画笔
    series4->setPen(minTempPen); // 设置最低气温折线系列的画笔

    // 将折线图系列添加到图表中
    chart1->addSeries(series1);
    chart1->addSeries(series4);

    chartview1->setRenderHint(QPainter::Antialiasing); // 可选的抗锯齿渲染设置
    // 如果需要，可以显示图例
    chart1->legend()->setVisible(true);
    chart1->legend()->setAlignment(Qt::AlignBottom); // 设置图例的位置，例如底部对齐


    // 将散点系列添加到图表中，并绑定轴
    chart1->addSeries(scatterSeries1);
    scatterSeries1->attachAxis(xAxis);
    scatterSeries1->attachAxis(yAxis);
    chart1->addSeries(scatterSeries4);
    scatterSeries4->attachAxis(xAxis);
    scatterSeries4->attachAxis(yAxis);

    // 更新图表1的标题
    chart1->setTitle("气温折线图（含散点）");
    //qDebug() << "Updated chart1 title to '气温折线图（含散点）'.";

    chart1->removeAxis(chart1->axisX());
    chart1->removeAxis(chart1->axisY());

    // 修改图表1的 x 轴信息
    xAxis->setLabelFormat("%d"); // 设置轴标签格式为整数（天数）
    xAxis->setTitleText("日期（天）"); // 设置轴标题
    chart1->addAxis(xAxis, Qt::AlignBottom); // 将 x 轴添加到图表的底部
    series1->attachAxis(xAxis); // 将 series1 与 x 轴绑定
    scatterSeries1->attachAxis(xAxis); // 将散点系列与 x 轴绑定
    series4->attachAxis(xAxis); // 将 series4 与 x 轴绑定
    scatterSeries4->attachAxis(xAxis); // 将散点系列与 x 轴绑定
    //qDebug() << "Updated chart1 x-axis format and title.";

    // 修改图表1的 y 轴信息

    yAxis->setLabelsVisible(true); // 设置轴标签可见
    yAxis->setTitleText("气温/°C"); // 设置轴标题
    chart1->addAxis(yAxis, Qt::AlignLeft); // 将 y 轴添加到图表的左侧
    series1->attachAxis(yAxis); // 将 series1 与 y 轴绑定
    scatterSeries1->attachAxis(yAxis); // 将散点系列与 y 轴绑定
    series4->attachAxis(yAxis); // 将 series1 与 y 轴绑定
    scatterSeries4->attachAxis(yAxis); // 将散点系列与 y 轴绑定
    //qDebug() << "Updated chart1 y-axis format and title.";

    // 刷新图表1显示
    chartview1->update();  // 或者 chartview1->repaint();
    //qDebug() << "Updated chartview1.";






    QSqlQuery query1(db);
    query1.prepare("SELECT weather FROM climate WHERE city = :city AND year = :year AND month = :month AND day <= :eday AND day >= :sday");
    query1.bindValue(":city", Qcity);
    query1.bindValue(":year", Qyear);
    query1.bindValue(":month", Qmonth);
    query1.bindValue(":sday", Qstartday);
    query1.bindValue(":eday", Qendday);



    // 执行查询语句
    if (!query1.exec()) {
        //qDebug() << "Query execution failed. Error:" << query1.lastError().text();
    } else {
        //qDebug() << "Query executed successfully.";

        QMap<QString, int> weatherCounts;


        while (query1.next()) {
            QString weather = query1.value(0).toString();

            // 检查天气是否已经在Map中存在，如果存在，则递增计数，否则添加新记录
            if (weatherCounts.contains(weather)) {
                weatherCounts[weather]++;
            } else {
                weatherCounts.insert(weather, 1);
            }
        }

        QMap<QString, QColor> weatherColors;
        for (auto it = weatherCounts.begin(); it != weatherCounts.end(); ++it) {
            int red = QRandomGenerator::global()->bounded(256);    // 随机生成 0 到 255 之间的整数
            int green = QRandomGenerator::global()->bounded(256);
            int blue = QRandomGenerator::global()->bounded(256);
            QColor randomColor(red, green, blue);
            weatherColors.insert(it.key(), randomColor);
        }
        series2->clear();

        for (auto it = weatherCounts.constBegin(); it != weatherCounts.constEnd(); ++it) {
            QString weather = it.key();
            int count = it.value();
            int total=Qendday-Qstartday+1;
            if (count > 0) {
                QPieSlice *slice = series2->append(weather, count);

                if (weatherColors.contains(weather)) {
                    slice->setBrush(weatherColors[weather]);
                } else {
                    // Handle unknown weather type color
                    slice->setBrush(Qt::black); // or any default color
                }

                // Set label to show weather type and percentage
                QString label = QString("%1 (%2%)").arg(weather).arg(QString::number(static_cast<double>(count) / total * 100, 'f', 1));
                slice->setLabel(label);
                slice->setLabelVisible(true); // Make label visible
            }
        }

        chart2->setTitle("天气饼状图");
        //qDebug() << "Chart title updated to:" << chart2->title();

        chartview2->update();

    }





    QSqlQuery query2;
    query2.prepare("SELECT wind_direction FROM climate WHERE city = :city AND year = :year AND month = :month AND day BETWEEN :sday AND :eday");
    query2.bindValue(":city", Qcity);
    query2.bindValue(":year", Qyear);
    query2.bindValue(":month", Qmonth);
    query2.bindValue(":sday", Qstartday);
    query2.bindValue(":eday", Qendday);
    query2.exec();

    QBarSet *barset1 = new QBarSet("东风");
    QBarSet *barset2 = new QBarSet("西风");
    QBarSet *barset3 = new QBarSet("南风");
    QBarSet *barset4 = new QBarSet("北风");
    QBarSet *barset5 = new QBarSet("东南风");
    QBarSet *barset6 = new QBarSet("东北风");
    QBarSet *barset7 = new QBarSet("西南风");
    QBarSet *barset8 = new QBarSet("西北风");
    QBarSet *barset9 = new QBarSet("微风/无持续风向");

    barset1->setColor(Qt::red);
    barset2->setColor(Qt::darkBlue);
    barset3->setColor(Qt::green);
    barset4->setColor(Qt::darkGreen);
    barset5->setColor(Qt::cyan);
    barset6->setColor(Qt::magenta);
    barset7->setColor(Qt::darkYellow);
    barset8->setColor(Qt::darkRed);
    barset9->setColor(Qt::gray);

    // 初始化八个风向的风力计数器
    int wind1 = 0;  // 东风
    int wind2 = 0;  // 西风
    int wind3 = 0;  // 南风
    int wind4 = 0;  // 北风
    int wind5 = 0;  // 东南风
    int wind6 = 0;  // 东北风
    int wind7 = 0;  // 西南风
    int wind8 = 0;  // 西北风
    int wind9 = 0;  // 微风

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
        } else if (wind_direction == "微风") {
            wind9++;
        }else if (wind_direction == "无持续风向") {
            wind9++;
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
    *barset9 << wind9;

    // 将八个风向的 QBarSet 添加到 QBarSeries 中
    QBarSeries *series3 = new QBarSeries();
    series3->append(barset1);
    series3->append(barset2);
    series3->append(barset3);
    series3->append(barset4);
    series3->append(barset5);
    series3->append(barset6);
    series3->append(barset7);
    series3->append(barset8);
    series3->append(barset9);
    connect(series3, &QBarSeries::hovered, this, &datavisualization::updateTooltip3);



    // Add space to label to add space between labels and axis在标签和轴之间加空格
    QValueAxis *axisY3 = qobject_cast<QValueAxis*>(chart3->axes(Qt::Vertical).first());
    Q_ASSERT(axisY3);
    axisY3->setLabelFormat("%.2f");
    series3->setLabelsPosition(QAbstractBarSeries::LabelsInsideEnd);  //设置柱状图标签显示的位置
    series3->setLabelsVisible(true);  //设置柱状图数据标签可见



    // 清除旧数据并设置新数据集到柱状图
    chart3->removeAllSeries();
    chart3->addSeries(series3);
    chart3->setTitle("风向统计图表");
    chart3->legend()->setVisible(true);
    chart3->legend()->setAlignment(Qt::AlignBottom);

    QStringList categories;
    categories << "风向" ;  //保存横坐标字符串的列表
    QBarCategoryAxis *axis4 = new QBarCategoryAxis();
    axis4->append(categories);
    chart3->createDefaultAxes();
    chart3->setAxisX(axis4, series3);

    // 释放查询结果和内存
    query2.finish();

    chartview2->update();



    // 查询数据库获取最高气温数据
    QSqlQuery query4;  // 使用现有的数据库连接
    query4.prepare("SELECT day,min_temperature,max_temperature FROM climate WHERE city = :city AND year = :year AND month = :month AND day <= :eday AND day >= :sday");
    query4.bindValue(":city", Qcity);
    query4.bindValue(":year", Qyear);
    query4.bindValue(":month", Qmonth);
    query4.bindValue(":sday", Qstartday);
    query4.bindValue(":eday", Qendday);

    if (!query4.exec()) {
        //qDebug() << "Query execution failed:" << query4.lastError().text();
    }



    series5->clear();
    scatterSeries5->clear();

    QAbstractAxis *oldXAxis = chart4->axisX();
    QAbstractAxis *oldYAxis = chart4->axisY();
    chart4->removeAxis(oldXAxis);
    chart4->removeAxis(oldYAxis);
    delete oldXAxis;
    delete oldYAxis;



    // 填充新数据，同时添加散点数据
    scatterSeries5->setMarkerShape(QScatterSeries::MarkerShapeCircle); // 设置散点形状为圆形
    scatterSeries5->setMarkerSize(9); // 设置散点大小为 9 像素

    QValueAxis *xAxis_diff = new QValueAxis;
    QValueAxis *yAxis_diff = new QValueAxis();

    int minY_diff = 100;  // 初始化为一个非常大的数，作为最小值
    int maxY_diff = -100;  // 初始化为一个非常小的数，作为最大值
    int minX_diff = 100;  // 初始化为一个非常大的数，作为最小值
    int maxX_diff = -100;  // 初始化为一个非常小的数，作为最大值

    while (query4.next()) {
        int day = query4.value("day").toInt(); // 获取天数
        int max_temp = query4.value("max_temperature").toInt();
        int min_temp = query4.value("min_temperature").toInt();
        int temperature_diff = max_temp - min_temp;

        series5->append(day, temperature_diff); // 添加折线图数据
        scatterSeries5->append(day, temperature_diff); // 添加散点数据

        // 更新最小和最大温差值
        if (temperature_diff < minY_diff) {
            minY_diff = temperature_diff;
        }
        if (temperature_diff > maxY_diff) {
            maxY_diff = temperature_diff;
        }

        // 更新最小和最大天数
        if (day < minX_diff) {
            minX_diff = day;
        }
        if (day > maxX_diff) {
            maxX_diff = day;
        }
    }



    xAxis_diff->setRange(minX_diff-0.5, maxX_diff+0.5);
    yAxis_diff->setRange(minY_diff-1, maxY_diff+1);

    series5->setName("温差折线"); // 设置名称
    scatterSeries5->setName("温差散点");

    QPen diffLinePen(Qt::darkBlue); // 温差折线颜色为黑色
    QPen diffPointPen(Qt::green); // 温差散点画笔为绿色

    series5->setPen(diffLinePen); // 设置温差折线系列的画笔
    scatterSeries5->setPen(diffPointPen);

    // 将折线图系列添加到图表中
    chart4->addSeries(series5);

    // 如果需要，可以显示图例
    chart4->legend()->setVisible(true);
    chart4->legend()->setAlignment(Qt::AlignBottom); // 设置图例的位置，例如底部对齐

    // 将散点系列添加到图表中，并绑定轴
    chart4->addSeries(scatterSeries5);
    scatterSeries5->attachAxis(xAxis_diff);
    scatterSeries5->attachAxis(yAxis_diff);

    // 更新图表的标题
    chart4->setTitle("温差折线图（含散点）");
    //qDebug() << "Updated chart4 title to '温差折线图（含散点）'.";

    // 修改图表的 x 轴信息
    xAxis_diff->setLabelFormat("%d"); // 设置轴标签格式为整数（天数）
    xAxis_diff->setTitleText("日期（天）"); // 设置轴标题
    chart4->addAxis(xAxis_diff, Qt::AlignBottom); // 将 x 轴添加到图表的底部
    series5->attachAxis(xAxis_diff); // 将折线系列与 x 轴绑定
    scatterSeries5->attachAxis(xAxis_diff); // 将散点系列与 x 轴绑定
    //qDebug() << "Updated chart4 x-axis format and title.";

    // 修改图表的 y 轴信息
    yAxis_diff->setLabelsVisible(true); // 设置轴标签可见
    yAxis_diff->setTitleText("温差/°C"); // 设置轴标题
    chart4->addAxis(yAxis_diff, Qt::AlignLeft); // 将 y 轴添加到图表的左侧
    series5->attachAxis(yAxis_diff); // 将折线系列与 y 轴绑定
    scatterSeries5->attachAxis(yAxis_diff); // 将散点系列与 y 轴绑定
    //qDebug() << "Updated chart4 y-axis format and title.";

    // 刷新图表显示
    chartview4->update();  // 或者 chartview4->repaint();
    //qDebug() << "Updated chartview4.";


    QSqlQuery query6(db);
    query6.prepare("SELECT wind_strength FROM climate WHERE city = :city AND year = :year AND month = :month AND day <= :eday AND day >= :sday");
    query6.bindValue(":city", Qcity);
    query6.bindValue(":year", Qyear);
    query6.bindValue(":month", Qmonth);
    query6.bindValue(":sday", Qstartday);
    query6.bindValue(":eday", Qendday);

    if (!query6.exec()) {
        //qDebug() << "Error executing query:" << query6.lastError().text();
        // Handle error condition, maybe return or log the error
        return; // or any appropriate action
    }

    QMap<QString, int> wind_strengthCounts;
    while (query6.next()) {
        QString wind_strength = query6.value(0).toString();

        if (wind_strengthCounts.contains(wind_strength)) {
            wind_strengthCounts[wind_strength]++;
        } else {
            wind_strengthCounts.insert(wind_strength, 1);
        }
    }

    QMap<QString, QColor> wind_strengthColors;
    for (auto it = wind_strengthCounts.begin(); it != wind_strengthCounts.end(); ++it) {
        int red = QRandomGenerator::global()->bounded(256);
        int green = QRandomGenerator::global()->bounded(256);
        int blue = QRandomGenerator::global()->bounded(256);
        QColor randomColor(red, green, blue);
        wind_strengthColors.insert(it.key(), randomColor);
    }

    series6->clear();

    int total = Qendday - Qstartday + 1;
    for (auto it = wind_strengthCounts.constBegin(); it != wind_strengthCounts.constEnd(); ++it) {
        QString wind_strength = it.key();
        int count = it.value();

        if (count > 0) {
            QPieSlice *slice = series6->append(wind_strength, count);

            if (wind_strengthColors.contains(wind_strength)) {
                slice->setBrush(wind_strengthColors[wind_strength]);
            } else {
                slice->setBrush(Qt::black); // Default color if not found
            }

            QString label = QString("%1 (%2%)").arg(wind_strength).arg(QString::number(static_cast<double>(count) / total * 100, 'f', 1));
            slice->setLabel(label);
            slice->setLabelVisible(true);
        }
    }

    chart6->setTitle("风力饼状图");
    chartview6->update();


}


//处理折线图的悬停信号
void datavisualization::updateTooltip1(QPointF point, bool state)
{
    if (state) {
        // 格式化提示信息
        QString tooltip = QString("日期: %1, 气温: %2°C")
                              .arg(QString::number(point.x(), 'f', 1)) // x 轴数据保留一位小数
                              .arg(QString::number(point.y(), 'f', 1)); // y 轴数据保留一位小数
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
        QString tooltip = QString("%1: %2天").arg(slice->label()).arg(slice->value());
        QToolTip::showText(QCursor::pos(), tooltip);

        // 放大当前悬停的扇形
        slice->setExploded(true);
        slice->setLabelVisible(true); // 可选：显示扇形的标签

        // 如果有需要，您还可以修改扇形的颜色或其他属性
        // slice->setBrush(...);

    } else {
        QToolTip::hideText();

        // 恢复扇形的正常大小
        if (slice) {
            slice->setExploded(false);
        }
    }
}

//处理柱状图

void datavisualization::updateTooltip3(bool hovered, int index, QBarSet* barset)
{
    static QColor originalColor; // 使用静态变量来保存原始颜色

    if (hovered && barset && index != -1) {
        // 获取柱子的颜色并保存原始颜色
        if (!originalColor.isValid()) {
            originalColor = barset->color(); // 只在第一次保存原始颜色
        }

        // 放大柱子的效果，例如增加柱子的宽度或者修改颜色
        barset->setColor(originalColor.lighter(150));  // 使用稍亮的颜色

        QString tooltip = QString(" %1: %2天").arg(barset->label()).arg(barset->at(index));
        QToolTip::showText(QCursor::pos(), tooltip);
    } else {
        // 恢复原始状态
        if (barset && originalColor.isValid()) {
            barset->setColor(originalColor);  // 恢复原始颜色
        }
        QToolTip::hideText();
        originalColor = QColor(); // 重置原始颜色为无效状态，以便下次重新保存
    }
}






// 处理折线散点图的悬停信号
void datavisualization::updateTooltip4(QPointF point, bool state)
{

    if (state) {
        // 放大散点
        scatterSeries1->setMarkerSize(12); // 设置散点大小为10（放大）

        // 格式化提示信息
        QString tooltip = QString("日期: %1号, 最高气温: %2°C").arg(point.x()).arg(point.y());

        // 显示提示信息
        QToolTip::showText(QCursor::pos(), tooltip);
    } else {
        // 鼠标移出散点范围，恢复原始大小
        scatterSeries1->setMarkerSize(9); // 设置散点大小为9（原始大小）

        // 隐藏提示信息
        QToolTip::hideText();
    }
}

// 处理折线散点图的悬停信号
void datavisualization::updateTooltip5(QPointF point, bool state)
{

    if (state) {
        // 放大散点
        scatterSeries4->setMarkerSize(12); // 设置散点大小为10（放大）

        // 格式化提示信息
        QString tooltip = QString("日期: %1号, 最低气温: %2°C").arg(point.x()).arg(point.y());

        // 显示提示信息
        QToolTip::showText(QCursor::pos(), tooltip);
    } else {
        // 鼠标移出散点范围，恢复原始大小
        scatterSeries4->setMarkerSize(9); // 设置散点大小为9（原始大小）

        // 隐藏提示信息
        QToolTip::hideText();
    }
}

void datavisualization::updateTooltip6(QPointF point, bool state)
{

    if (state) {
        // 放大散点
        scatterSeries5->setMarkerSize(12); // 设置散点大小为10（放大）

        // 格式化提示信息
        QString tooltip = QString("日期: %1号, 温差: %2°C").arg(point.x()).arg(point.y());

        // 显示提示信息
        QToolTip::showText(QCursor::pos(), tooltip);
    } else {
        // 鼠标移出散点范围，恢复原始大小
        scatterSeries5->setMarkerSize(9); // 设置散点大小为9（原始大小）

        // 隐藏提示信息
        QToolTip::hideText();
    }
}

//处理饼状图的悬停信号
void datavisualization::updateTooltip7(QPieSlice *slice, bool state)
{
    if (state && slice) {
        QString tooltip = QString("%1: %2天").arg(slice->label()).arg(slice->value());
        QToolTip::showText(QCursor::pos(), tooltip);

        // 放大当前悬停的扇形
        slice->setExploded(true);
        slice->setLabelVisible(true); // 可选：显示扇形的标签

        // 如果有需要，您还可以修改扇形的颜色或其他属性
        // slice->setBrush(...);

    } else {
        QToolTip::hideText();

        // 恢复扇形的正常大小
        if (slice) {
            slice->setExploded(false);
        }
    }
}
// 更新按钮样式
void datavisualization::updateButtonStyle(QPushButton *activeButton) {
    QList<QPushButton*> buttons = {btn1, btn2, btn3, btn4, btn5};
    for (QPushButton *btn : buttons) {
        if (btn == activeButton) {
            btn->setStyleSheet(R"(
                QPushButton {
                    background-color: #b9d7ea; /* 被点击时的颜色 */
                    color: #769fcd;
                    font-size: 14px;
                    padding: 10px 20px;
                    border: none;
                    border-radius: 4px;
                }
            )");
        } else {
            btn->setStyleSheet(R"(
                QPushButton {
                    background-color: #f7fbfc; /* 默认颜色 */
                    color: #769fcd;
                    font-size: 14px;
                    padding: 10px 20px;
                    border: none;
                    border-radius: 4px;
                }
                QPushButton:hover {
                    background-color: #d6e6f2; /* 悬停时的颜色 */
                }
                QPushButton:pressed {
                    background-color: #b9d7ea; /* 按下时的颜色 */
                }
            )");
        }
    }
}

