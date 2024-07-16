#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "todayweather.h"
#include "datavisualization.h"
#include "historyweather.h"
#include "forecast.h"
#include "simulation.h""
#include "admin.h"

//构造函数
MainWindow::MainWindow(int userIdentity, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , userIdentity(userIdentity)
{
    ui->setupUi(this);

    //QSS样式表
    QString styleSheet = R"(
    MainWindow {
        background-color: #f7fbfc;
    }

    QPushButton {
        background-color: #F7F9FC;
        color: #769fcd;
        border: 1px solid #F7F9FC;
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

    QPushButton:checked {
        background-color: #b9d7ea;
        border: 1px solid #b9d7ea;
    }

    QLabel {
        font-family: "Arial", sans-serif;
        font-size: 24px;
        color: #333333;
        padding: 10px;
    }

    QStackedWidget {
        background-color: #FFFFFF;
        border: 1px solid #E1E8ED;
        border-radius: 5px;
        padding: 20px;
    }
)";

    this->setStyleSheet(styleSheet);

    // 设置窗口标题
    this->setWindowTitle("主页");
    QScreen *screen = QApplication::primaryScreen();
    this->move(100, 50);

    // 创建按钮
    btn1 = new QPushButton("当天天气", this);
    btn2 = new QPushButton("历史天气", this);
    btn3 = new QPushButton("数据分析和可视化", this);
    btn4 = new QPushButton("气温预测", this);
    btn5 = new QPushButton("个人模拟分析",this);
    btn6 = new QPushButton("管理员界面", this);

    // 设置按钮为可选中状态
    btn1->setCheckable(true);
    btn2->setCheckable(true);
    btn3->setCheckable(true);
    btn4->setCheckable(true);
    btn5->setCheckable(true);
    btn6->setCheckable(true);

    // 设置按钮固定大小
    btn1->setFixedSize(300, 100);
    btn2->setFixedSize(300, 100);
    btn3->setFixedSize(300, 100);
    btn4->setFixedSize(300, 100);
    btn5->setFixedSize(300, 100);
    btn6->setFixedSize(300, 100);

    // 创建按钮布局
    QVBoxLayout *btnlayout = new QVBoxLayout;
    btnlayout->addWidget(btn1);
    btnlayout->addWidget(btn2);
    btnlayout->addWidget(btn3);
    btnlayout->addWidget(btn4);
    btnlayout->addWidget(btn5);

    // 根据用户身份决定是否显示管理员界面按钮
    if (userIdentity == 2) {
        btnlayout->addWidget(btn6);
    } else {
        btn6->hide();
    }

    // 创建堆叠窗口部件
    stackedwidget = new QStackedWidget(this);

    // 创建并添加各个页面到堆叠窗口部件

    QWidget *page1 = new todayweather;
    QLabel *hlabel1 = new QLabel("界面1");
    QVBoxLayout *layout1 = new QVBoxLayout(page1);
    layout1->addWidget(hlabel1);
    stackedwidget->addWidget(page1);

    QWidget *page2 = new historyweather;
    QLabel *hlabel2 = new QLabel("界面2");
    QVBoxLayout *layout2 = new QVBoxLayout(page2);
    layout2->addWidget(hlabel2);
    stackedwidget->addWidget(page2);

    QWidget *page3 = new datavisualization;
    QLabel *hlabel3 = new QLabel("界面3");
    QVBoxLayout *layout3 = new QVBoxLayout(page3);
    layout3->addWidget(hlabel3);
    stackedwidget->addWidget(page3);

    QWidget *page4 = new forecast;
    QLabel *hlabel4 = new QLabel("界面4");
    QVBoxLayout *layout4 = new QVBoxLayout(page4);
    layout4->addWidget(hlabel4);
    stackedwidget->addWidget(page4);

    QWidget *page5 = new Simulation;
    QLabel *hlabel5 = new QLabel("界面5");
    QVBoxLayout *layout5 = new QVBoxLayout(page5);
    layout5->addWidget(hlabel5);
    stackedwidget->addWidget(page5);

    QWidget *page6 = new admin;
    QLabel *hlabel6 = new QLabel("界面6");
    QVBoxLayout *layout6 = new QVBoxLayout(page6);
    layout5->addWidget(hlabel6);
    stackedwidget->addWidget(page6);

    // 创建主布局
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addLayout(btnlayout);
    mainLayout->addWidget(stackedwidget);

    // 设置中心部件
    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);

    // 更新按钮样式并连接按钮点击事件
    updateButtonStyles(btn1);
    connect(btn1, &QPushButton::clicked, this, [this]() {
        updateButtonStyles(btn1);
        stackedwidget->setCurrentIndex(0);
    });
    connect(btn2, &QPushButton::clicked, this, [this]() {
        updateButtonStyles(btn2);
        stackedwidget->setCurrentIndex(1);
    });
    connect(btn3, &QPushButton::clicked, this, [this]() {
        updateButtonStyles(btn3);
        stackedwidget->setCurrentIndex(2);
    });
    connect(btn4, &QPushButton::clicked, this, [this]() {
        updateButtonStyles(btn4);
        stackedwidget->setCurrentIndex(3);
    });
    connect(btn5, &QPushButton::clicked, this, [this]() {
        updateButtonStyles(btn5);
        stackedwidget->setCurrentIndex(4);
    });
    connect(btn6, &QPushButton::clicked, this, [this]() {
        updateButtonStyles(btn6);
        stackedwidget->setCurrentIndex(5);
    });
}

// 更新按钮样式的方法
void MainWindow::updateButtonStyles(QPushButton *activeButton)
{
    QList<QPushButton*> buttons = {btn1, btn2, btn3, btn4, btn5, btn6};
    for (QPushButton *btn : buttons) {
        if (btn == activeButton) {
            btn->setChecked(true);
        } else {
            btn->setChecked(false);
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
