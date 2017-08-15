#ifndef BARCHART_H
#define BARCHART_H

#include <QObject>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLegend>
#include <QtCharts/QBarCategoryAxis>
#include <QHorizontalBarSeries>
#include<QValueAxis>
#include<QDebug>

QT_CHARTS_USE_NAMESPACE

class barchart : public QObject
{
    Q_OBJECT
public:
    explicit barchart(QObject *parent = nullptr);
    void chart(QVector<float> vectorA, int clectNumber);
    QChartView *chartView;
signals:

public slots:
};

#endif // BARCHART_H
