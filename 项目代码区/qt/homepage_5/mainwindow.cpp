#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "todayweather.h"
#include "datavisualization.h"
#include "historyweather.h"
#include "forecast.h"

MainWindow::MainWindow(int userIdentity, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , userIdentity(userIdentity)
{
    ui->setupUi(this);

    this->setWindowTitle("主页");
    QScreen *screen = QApplication::primaryScreen();
    QRect screenG = screen->geometry();
    this->setGeometry(screenG);
    this->showMaximized();

    btn1 = new QPushButton("当天天气", this);
    btn2 = new QPushButton("历史天气", this);
    btn3 = new QPushButton("数据分析和可视化", this);
    btn4 = new QPushButton("气温预测", this);
    btn5 = new QPushButton("管理员按钮", this);

    btn1->setFixedSize(300, 100);
    btn2->setFixedSize(300, 100);
    btn3->setFixedSize(300, 100);
    btn4->setFixedSize(300, 100);
    btn5->setFixedSize(300, 100);

    QVBoxLayout *btnlayout = new QVBoxLayout;

    btnlayout->addWidget(btn1);
    btnlayout->addWidget(btn2);
    btnlayout->addWidget(btn3);
    btnlayout->addWidget(btn4);

    if (userIdentity == 2) {
        btnlayout->addWidget(btn5);
    } else {
        btn5->hide();
    }

    stackedwidget = new QStackedWidget(this);

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

    QWidget *page5 = new QWidget;
    QLabel *hlabel5 = new QLabel("界面5");
    QVBoxLayout *layout5 = new QVBoxLayout(page5);
    layout5->addWidget(hlabel5);
    stackedwidget->addWidget(page5);

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addLayout(btnlayout);
    mainLayout->addWidget(stackedwidget);

    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);

    connect(btn1, &QPushButton::clicked, this, [this]() {
        stackedwidget->setCurrentIndex(0);
    });
    connect(btn2, &QPushButton::clicked, this, [this]() {
        stackedwidget->setCurrentIndex(1);
    });
    connect(btn3, &QPushButton::clicked, this, [this]() {
        stackedwidget->setCurrentIndex(2);
    });
    connect(btn4, &QPushButton::clicked, this, [this]() {
        stackedwidget->setCurrentIndex(3);
    });
    connect(btn5, &QPushButton::clicked, this, [this]() {
        stackedwidget->setCurrentIndex(4);
    });

    QString styleSheet = R"(
        MainWindow {
            background-color: #F7F9FC;
        }

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
}

MainWindow::~MainWindow()
{
    delete ui;
}
