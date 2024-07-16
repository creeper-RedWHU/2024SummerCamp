#ifndef LIFEINDEXWIDGET_H
#define LIFEINDEXWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>

class LifeIndexWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LifeIndexWidget(const QString &iconPath, const QString &labelText, QWidget *parent = nullptr);
    ~LifeIndexWidget();

private:
    QLabel *iconLabel;  //图片标签
    QLabel *textLabel;  //文本标签
    QHBoxLayout *layout;  //布局设置
};

#endif // LIFEINDEXWIDGET_H
