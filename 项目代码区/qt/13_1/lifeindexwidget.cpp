#include "LifeIndexWidget.h"
#include <QFont>

LifeIndexWidget::LifeIndexWidget(const QString &iconPath, const QString &labelText, QWidget *parent)
    : QWidget(parent)
{
    // 图标标签
    iconLabel = new QLabel(this);
    // 文本标签
    textLabel = new QLabel(this);
    // 布局设置
    layout = new QHBoxLayout(this);

    // 设置图标图片，保持宽高比，且高度为25
    QPixmap pixmap(iconPath);
    int height = 25;
    int width = static_cast<int>(height * pixmap.width() / pixmap.height());
    iconLabel->setPixmap(pixmap.scaled(width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    // 设置文本
    textLabel->setText(labelText);
    textLabel->setAlignment(Qt::AlignCenter);

    iconLabel->setAlignment(Qt::AlignCenter);
    textLabel->setAlignment(Qt::AlignCenter);

    // 设置字体大小
    QFont font = textLabel->font();
    font.setPointSize(12); // 适当的字体大小
    textLabel->setFont(font);


    layout->addWidget(iconLabel);
    layout->addWidget(textLabel);

    setLayout(layout);

    // 设置固定大小
}

LifeIndexWidget::~LifeIndexWidget()
{
    delete iconLabel;
    delete textLabel;
    delete layout;
}
