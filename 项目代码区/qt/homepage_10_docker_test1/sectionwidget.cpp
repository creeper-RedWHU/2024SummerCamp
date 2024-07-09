#include "sectionwidget.h"

SectionWidget::SectionWidget(const QString &imagePath, const QStringList &texts, QWidget *parent)
    : QWidget(parent)
{
    // 创建并设置图像标签
    imageLabel = new QLabel(this);
    QPixmap pixmap(imagePath);
    imageLabel->setPixmap(pixmap.scaled(150, 150, Qt::KeepAspectRatio)); // 调整图片大小

    // 创建并设置文字布局
    textLayout = new QVBoxLayout;
    for (const QString &text : texts) {
        QLabel *textLabel = new QLabel(text, this);
        textLayout->addWidget(textLabel);
    }

    // 创建主布局，并将图像和文字布局添加到主布局中
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->addWidget(imageLabel);
    mainLayout->addLayout(textLayout);
    setLayout(mainLayout);
}
