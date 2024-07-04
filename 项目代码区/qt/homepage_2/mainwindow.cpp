#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "todayweather.h"
#include "datavisualization.h"
#include "historyweather.h"
#include "forecast.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowTitle("主页");
    // 设置默认占满屏幕并最大化显示
    QScreen *screen = QApplication::primaryScreen();
    QRect screenG = screen->geometry();   // 获取主屏幕的几何信息
    this->setGeometry(screenG);
    this->showMaximized();

    // 创建按钮
    btn1 = new QPushButton("当天天气", this);
    btn2 = new QPushButton("历史天气", this);
    btn3 = new QPushButton("数据分析和可视化", this);
    btn4 = new QPushButton("气温预测", this);
    btn5 = new QPushButton("", this);

    btn1->setFixedSize(300, 100);
    btn2->setFixedSize(300, 100);
    btn3->setFixedSize(300, 100);
    btn4->setFixedSize(300, 100);
    btn5->setFixedSize(300, 100);

    // 创建垂直布局管理器
    QVBoxLayout *btnlayout = new QVBoxLayout;

    // 将按钮添加到布局中
    btnlayout->addWidget(btn1);
    btnlayout->addWidget(btn2);
    btnlayout->addWidget(btn3);
    btnlayout->addWidget(btn4);
    btnlayout->addWidget(btn5);

    // 创建右侧功能页面的 QStackedWidget
    stackedwidget = new QStackedWidget(this);

    // 创建页面1
    QWidget *page1 = new todayweather;
    QLabel *hlabel1 = new QLabel("界面1");
    QVBoxLayout *layout1 = new QVBoxLayout(page1);
    layout1->addWidget(hlabel1);
    stackedwidget->addWidget(page1);

    // 创建页面2
    QWidget *page2 = new historyweather;
    QLabel *hlabel2 = new QLabel("界面2");
    QVBoxLayout *layout2 = new QVBoxLayout(page2);
    layout2->addWidget(hlabel2);
    stackedwidget->addWidget(page2);

    // 创建页面3
    QWidget *page3 = new datavisualization;
    QLabel *hlabel3 = new QLabel("界面3");
    QVBoxLayout *layout3 = new QVBoxLayout(page3);
    layout3->addWidget(hlabel3);
    stackedwidget->addWidget(page3);

    // 创建页面4
    QWidget *page4 = new forecast;
    QLabel *hlabel4 = new QLabel("界面4");
    QVBoxLayout *layout4 = new QVBoxLayout(page4);
    layout4->addWidget(hlabel4);
    stackedwidget->addWidget(page4);

    // 创建页面5
    QWidget *page5 = new QWidget;
    QLabel *hlabel5 = new QLabel("界面5");
    QVBoxLayout *layout5 = new QVBoxLayout(page5);
    layout5->addWidget(hlabel5);
    stackedwidget->addWidget(page5);

    // 将 QStackedWidget 添加到主窗口的布局中
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addLayout(btnlayout); // 左侧按钮布局
    mainLayout->addWidget(stackedwidget); // 右侧功能页面

    // 创建一个 QWidget 作为主窗口的中心部件
    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);

    // 连接按钮的点击事件到槽函数，用于切换 QStackedWidget 页面
    connect(btn1, &QPushButton::clicked, this, [this]() {
        stackedwidget->setCurrentIndex(0); // 切换到第一页
    });
    connect(btn2, &QPushButton::clicked, this, [this]() {
        stackedwidget->setCurrentIndex(1); // 切换到第二页
    });
    connect(btn3, &QPushButton::clicked, this, [this]() {
        stackedwidget->setCurrentIndex(2); // 切换到第三页
    });
    connect(btn4, &QPushButton::clicked, this, [this]() {
        stackedwidget->setCurrentIndex(3); // 切换到第四页
    });
    connect(btn5, &QPushButton::clicked, this, [this]() {
        stackedwidget->setCurrentIndex(4); // 切换到第五页
    });

// Apply QSS stylesheet
QString styleSheet = R"(
        /* General Widget Background */
        MainWindow {
            background-color: #F7F9FC;
        }

        /* PushButton Styling */
        QPushButton {
            background-color: #8B8989;
            color: #FFFFFF;
            border: 1px solid #8B8989;
            border-radius: 5px;
            padding: 20px;
            font-family: "Arial", sans-serif;
            font-size: 18px;
            font-weight: bold;
        }

        QPushButton:hover {
            background-color: #CDC5BF;
            border: 1px solid #CDC5BF;
        }

        QPushButton:pressed {
            background-color: #8B8682;
            border: 1px solid #8B8682;
        }

        /* QLabel Styling */
        QLabel {
            font-family: "Arial", sans-serif;
            font-size: 24px;
            color: #333333;
            padding: 10px;
        }

        /* QStackedWidget Styling */
        QStackedWidget {
            background-color: #FFFFFF;
            border: 1px solid #E1E8ED;
            border-radius: 5px;
            padding: 20px;
        }
    )";
    this->setStyleSheet(styleSheet);
}


MainWindow::~MainWindow()
{
    delete ui;
}
