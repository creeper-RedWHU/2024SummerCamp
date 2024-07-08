#ifndef SECTIONWIDGET_H
#define SECTIONWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>

class SectionWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SectionWidget(const QString &imagePath, const QStringList &texts, QWidget *parent = nullptr);

private:
    QLabel *imageLabel;
    QVBoxLayout *textLayout;
};

#endif // SECTIONWIDGET_H
