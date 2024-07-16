#ifndef SECTIONWIDGET_H
#define SECTIONWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>

class SectionWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SectionWidget(const QString &weather, const QString &maxt, const QString &mint, const QString &winddir, const QString &windstr, QWidget *parent = nullptr);

private:
    void drawWeather(const QString &weatherType1, const QString &weatherType2);

    QLabel *weatherLabelpic1;
    QLabel *weatherLabelpic2;
    QLabel *weatherLabel;
    QLabel *maxtLabel;
    QLabel *mintLabel;
    QLabel *winddLabel;
    QLabel *windsLabel;
    QVBoxLayout *mainLayout;
};

#endif // SECTIONWIDGET_H
