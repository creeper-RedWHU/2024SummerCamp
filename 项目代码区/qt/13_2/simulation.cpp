#include "simulation.h"
#include <QDebug>
#include "database.h"
#include "api.h"

Simulation::Simulation(QWidget *parent)
    : QWidget(parent)
{

    lowerTempLineEdit = new QLineEdit(this);
    upperTempLineEdit = new QLineEdit(this);
    simulateButton = new QPushButton("模拟", this);
    imageLabel = new QLabel(this);
    modelComboBox = new QComboBox(this);

    tempValidator = new QIntValidator(-30, 50, this);
    lowerTempLineEdit->setValidator(tempValidator);
    upperTempLineEdit->setValidator(tempValidator);

    modelComboBox->addItem("线性回归算法");
    modelComboBox->addItem("Holt-Winters方法");
    modelComboBox->addItem("自回归模型");
    modelComboBox->addItem("前馈神经网路预测");
    modelComboBox->addItem("SARIMA模型");

    modelComboBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    modelComboBox->setMinimumWidth(100);
    modelComboBox->setMinimumHeight(30);

    QHBoxLayout *tempLayout = new QHBoxLayout;
    tempLayout->addWidget(new QLabel("下限温度:"));
    tempLayout->addWidget(lowerTempLineEdit);
    tempLayout->addWidget(new QLabel("上限温度:"));
    tempLayout->addWidget(upperTempLineEdit);

    QHBoxLayout *topLayout = new QHBoxLayout;
    topLayout->addLayout(tempLayout);
    topLayout->addWidget(new QLabel("选择模型:"));
    topLayout->addWidget(modelComboBox);
    topLayout->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    topLayout->addWidget(simulateButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(topLayout);
    mainLayout->addWidget(imageLabel);

    setLayout(mainLayout);

    connect(simulateButton, &QPushButton::clicked, this, &Simulation::onSimulateClicked);

    // QSS样式
    QString styleSheet = R"(
        QLineEdit {
            background-color: #f7fbfc;
            color: #769fcd;
            border: 1px solid #8B8989;
            border-radius: 5px;
            padding: 5px;
            font-family: "Arial", sans-serif;
            font-size: 18px;
            font-weight: bold;
        }

        QPushButton {
            background-color: #f7fbfc;
            color: #769fcd;
            border: 1px solid #8B8989;
            border-radius: 5px;
            padding: 20px;
            font-family: "Arial", sans-serif;
            font-size: 18px;
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
            background-color: #FFFFFF;
            border: 1px solid #E1E8ED;
            border-radius: 5px;
            padding: 10px;
            font-family: "Arial", sans-serif;
            font-size: 14px;
            color: #333333;
        }
    )";

    setStyleSheet(styleSheet);

    series1 = new QLineSeries;
    scatterSeries1 = new QScatterSeries();

    series2 = new QLineSeries;
    scatterSeries2 = new QScatterSeries();

    // 创建图表并添加数据系列
    chart = new QChart();
    chart->legend()->hide();

    chartview = new QChartView(chart);
    chartview->setRenderHint(QPainter::Antialiasing);

    // connect(scatterSeries1, &QScatterSeries::hovered, this, &Simulation::updateTooltip);
    connect(scatterSeries2, &QScatterSeries::hovered, this, &Simulation::updateTooltip);

    mainLayout->addWidget(chartview);
}

Simulation::~Simulation()
{
}


void Simulation::draw(int modelId)
{
    //
    QString modelname;
    switch (modelId) {
    case 1:
        modelname="线性回归算法";
        break;
    case 2:
        modelname="Holt-Winters方法";
        break;
    case 3:
        modelname="自回归模型";
        break;
    case 4:
        modelname="前馈神经网路预测";
        break;
    case 5:
        modelname="SARIMA模型";
        break;
    default:
        modelname="模型获取失败";
        break;
    }

    database thisdb;

    qDebug() << modelId;
    if (!thisdb.connectToDatabase()) {
        QMessageBox::critical(this, "数据库连接失败", "数据模拟无法连接到数据库，请检查配置。");
    } else {
        qDebug() << "成功连接数据库";
    }

    // 确保使用正确的数据库连接名称
    QString connectionName = "connection_1";


    //先获取前40天的数据
    QSqlQuery query1(QSqlDatabase::database(connectionName));  // 使用现有的数据库连接
    // 生成SQL查询字符串
    // OR id=%1 .arg(modelId)
    QString queryString1 = QString("SELECT time, temperature, id FROM analog_data WHERE id=0");
    qDebug() << "Query1 String:" << queryString1;  // 打印查询字符串以调试


    query1.prepare(queryString1);

    if (!query1.exec()) {
        qDebug() << "Query execution failed:" << query1.lastError().text();
    }

    //再查后面的数据
    QSqlQuery query2(QSqlDatabase::database(connectionName));  // 使用现有的数据库连接
    QString queryString2 = QString("SELECT time, temperature, id FROM analog_data WHERE id=%1").arg(modelId);
    qDebug() << "Query2 String:" << queryString2;  // 打印查询字符串以调试
    query2.prepare(queryString2);
    if (!query2.exec()) {
        qDebug() << "Query execution failed:" << query1.lastError().text();
    }


    // 清空原有数据
    series1->clear();
    scatterSeries1->clear();
    series2->clear();
    scatterSeries2->clear();

    // 清空原有散点数据
    foreach (QAbstractSeries *s, chart->series()) {
        chart->removeSeries(s);
    }
    // 填充新数据，同时添加散点数据
    scatterSeries1->setMarkerShape(QScatterSeries::MarkerShapeCircle); // 设置散点形状为圆形
    scatterSeries1->setMarkerSize(9); // 设置散点大小为 4.5 像素
    scatterSeries2->setMarkerShape(QScatterSeries::MarkerShapeCircle); // 设置散点形状为圆形
    scatterSeries2->setMarkerSize(9); // 设置散点大小为 4.5 像素
    QValueAxis *xAxis = new QValueAxis;
    QValueAxis *yAxis = new QValueAxis();


    int minY = 100;  // 初始化为一个非常大的数，作为最小值
    int maxY = -100;  // 初始化为一个非常小的数，作为最大值



    int i=0;
    while (query1.next()) {
        i++;
        int time = query1.value("time").toInt(); // 获取天数
        double temperature = query1.value("temperature").toInt();
        series1->append(time, temperature); // 添加折线图数据
        scatterSeries1->append(time, temperature); // 添加散点数据
        //如果是最后一个 还需要添加到第二个series中
        if(i==40)
        {
            series2->append(time, temperature); // 添加折线图数据
            scatterSeries2->append(time, temperature); // 添加散点数据
        }
        // 更新最小和最大温度值
        if (temperature < minY) {
            minY = temperature;
        }
        if (temperature > maxY) {
            maxY = temperature;
        }
    }
    while (query2.next()) {
        int time = query2.value("time").toInt(); // 获取天数
        double temperature = query2.value("temperature").toInt();
        series2->append(time, temperature); // 添加折线图数据
        scatterSeries2->append(time, temperature); // 添加散点数据
        // 更新最小和最大温度值
        if (temperature < minY) {
            minY = temperature;
        }
        if (temperature > maxY) {
            maxY = temperature;
        }
    }

    xAxis->setRange(0, 50);
    yAxis->setRange(minY-2, maxY+2);

    series1->setName(QString("原始数据折线").arg(modelId));
    scatterSeries1->setName(QString("原始数据散点").arg(modelId));
    series2->setName(QString("预测数据折线").arg(modelId));
    scatterSeries2->setName(QString("预测数据散点").arg(modelId));

    QPen TempPen1(Qt::blue); // 折线颜色为蓝色
    QPen TempPen2(Qt::red); // 预测折线颜色为红色

    QPen TempPointpen(Qt::green);

    scatterSeries1->setPen(TempPointpen);
    series1->setPen(TempPen1);
    series2->setPen(TempPen2);

    // 将折线图系列添加到图表中
    chart->addSeries(series1);
    chart->addSeries(series2);

    chartview->setRenderHint(QPainter::Antialiasing); // 可选的抗锯齿渲染设置
    // 如果需要，可以显示图例
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom); // 设置图例的位置，例如底部对齐


    // 将散点系列添加到图表中，并绑定轴
    chart->addSeries(scatterSeries1);
    chart->addSeries(scatterSeries2);


    chart->setTitle(modelname + "预测结果");
    chart->removeAxis(chart->axisX());
    chart->removeAxis(chart->axisY());

    // 修改图表1的 x 轴信息
    xAxis->setLabelFormat("%d"); // 设置轴标签格式为整数（天数）
    xAxis->setTitleText("日期（天）"); // 设置轴标题
    chart->addAxis(xAxis, Qt::AlignBottom); // 将 x 轴添加到图表的底部
    series1->attachAxis(xAxis); // 将 series1 与 x 轴绑定
    scatterSeries1->attachAxis(xAxis); // 将散点系列与 x 轴绑定
    series2->attachAxis(xAxis); // 将 series1 与 x 轴绑定
    scatterSeries2->attachAxis(xAxis); // 将散点系列与 x 轴绑定

    // 修改图表1的 y 轴信息

    yAxis->setLabelsVisible(true); // 设置轴标签可见
    yAxis->setTitleText("气温/°C"); // 设置轴标题
    chart->addAxis(yAxis, Qt::AlignLeft); // 将 y 轴添加到图表的左侧
    series1->attachAxis(yAxis); // 将 series1 与 y 轴绑定
    scatterSeries1->attachAxis(yAxis); // 将散点系列与 y 轴绑定
    series2->attachAxis(yAxis); // 将 series1 与 y 轴绑定
    scatterSeries2->attachAxis(yAxis); // 将散点系列与 y 轴绑定

    // 刷新图表1显示
    chartview->update();  // 或者 chartview1->repaint();
}
void Simulation::onSimulateClicked()
{
    //这边需要调用api
    // onPredictRequest();
    // qDebug()<<"按钮点击";

    bool lowerOk, upperOk;
    int min_temperature = lowerTempLineEdit->text().toInt(&lowerOk);
    int max_temperature = upperTempLineEdit->text().toInt(&upperOk);
    int selectedModel = modelComboBox->currentIndex();

    if (!lowerOk || !upperOk) {
        QMessageBox::warning(this, "输入错误", "请输入有效的温度值（-30到50之间的整数）。");
        return;
    }

    if (max_temperature - min_temperature > 50) {
        QMessageBox::warning(this, "温差错误", "温差不能超过50摄氏度。");
        return;
    }

    QString min_tem=lowerTempLineEdit->text();
    QString max_tem=upperTempLineEdit->text();



    draw(selectedModel+1);
}


void Simulation::updateTooltip(QPointF point, bool state)
{

    if (state) {
        // 放大散点
        scatterSeries2->setMarkerSize(12); // 设置散点大小为10（放大）

        // 格式化提示信息
        QString tooltip = QString("第%1天,气温:%2°C").arg(point.x()).arg(point.y());

        // 显示提示信息
        QToolTip::showText(QCursor::pos(), tooltip);
    } else {
        // 鼠标移出散点范围，恢复原始大小
        scatterSeries2->setMarkerSize(9); // 设置散点大小为9（原始大小）

        // 隐藏提示信息
        QToolTip::hideText();
    }
}


void Simulation::onPredictRequest() {
    api *predict_api = new api;

    connect(predict_api, &api::predictInfoReady, this, &Simulation::onPredictInfoReady);

    //用用户输入的信息调用api
    predict_api->predict_fake(lowerTempLineEdit->text(),upperTempLineEdit->text());
}

void Simulation::onPredictInfoReady(const QJsonValue& min_t,const QJsonValue& max_t)
{
    // 在这里处理获取到的天气数据
    qDebug() << min_t<<" "<<max_t;
    // Example: 更新界面上的标签或者其他控件
    // weatherLabel->setText(weatherData["temperature"].toString());
}
