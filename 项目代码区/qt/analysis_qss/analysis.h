#ifndef ANALYSIS_H
#define ANALYSIS_H

#include <QWidget>
#include <QComboBox>
#include <QPushButton>
#include <QGridLayout>
#include <QLabel>
#include <QImage>

class ImageWidget : public QWidget {
    Q_OBJECT

public:
    explicit ImageWidget(QWidget *parent = nullptr);
    void updateImages(const QList<QImage> &images);

private:
    QGridLayout *imageLayout;
    QList<QLabel*> imageLabels;
};

class MainWindow : public QWidget {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void drawChart();

private:
    QImage generateImage1(const QString &city, const QString &startYear, const QString &endYear);
    QImage generateImage2(const QString &city, const QString &startYear, const QString &endYear);
    QImage generateImage3(const QString &city, const QString &startYear, const QString &endYear);
    QImage generateImage4(const QString &city, const QString &startYear, const QString &endYear);

    QComboBox *cityComboBox;
    QComboBox *startYearComboBox;
    QComboBox *endYearComboBox;
    ImageWidget *imageWidget;
};

#endif // ANALYSIS_H
