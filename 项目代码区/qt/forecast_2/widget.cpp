#include "widget.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPixmap>

MyWidget::MyWidget(QWidget *parent)
    : QWidget(parent) {
    QVBoxLayout *layout = new QVBoxLayout(this);

    // 创建左上角的文本标签
    QLabel *title = new QLabel("基于机器学习的气温趋势预测", this);
    title->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    layout->addWidget(title);

    // 创建用于显示算法生成图像的占位符标签,后续可以删了（应该吧）
    QLabel *imagePlaceholder = new QLabel(this);
    imagePlaceholder->setFixedSize(400, 300);
    imagePlaceholder->setAlignment(Qt::AlignCenter);
    imagePlaceholder->setStyleSheet("border: 1px solid black;");
    imagePlaceholder->setText("图像区域");

    layout->addWidget(imagePlaceholder, 0, Qt::AlignHCenter | Qt::AlignVCenter);

    // 在此处添加算法生成的图像到占位符
    // QPixmap pixmap = ...; // 通过算法生成图像
    // imagePlaceholder->setPixmap(pixmap);

    setLayout(layout);
}

MyWidget::~MyWidget() {
}
