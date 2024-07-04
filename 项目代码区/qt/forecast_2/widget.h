#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

class MyWidget : public QWidget {
    Q_OBJECT

public:
    MyWidget(QWidget *parent = nullptr);
    ~MyWidget();
};

#endif // WIDGET_H
