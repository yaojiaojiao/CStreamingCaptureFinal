#include "barchart.h"



barchart::barchart(QObject *parent) : QObject(parent)
{

}

void barchart::chart(QVector<float> vectorA, int clectNumber)
{

    QBarSet *set5 = new QBarSet("信号强度");
    for(int i=0;i<clectNumber;)
    {
        *set5 <<vectorA[i];
        i++;
    }

    //![1]

    //![2]
    QBarSeries *series = new QBarSeries();  //QBarSeries类提供了一系列按类别分组的垂直条 （用于创建竖直柱状图）
    //QHorizontalBarSeries *series=new QHorizontalBarSeries();  //QHorizontalBarSeries （用于创建水平柱状图）

    series->append(set5);

    //![2]

    //![3]

    //QChart类管理图表的系列、图例和坐标轴的图形表示
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Bar Chart");
    chart->setAnimationOptions(QChart::NoAnimation);  //动画选项 无动画
    //   chart->setAnimationOptions(QChart::SeriesAnimations);  //动画选项
    //![3]

    //![4]

    QStringList categories;       //QStringList类提供了一个字符串列表。
    for(int i=0;i<clectNumber;)
    {
        categories <<QString::number(i+1);
        i++;
    }
    QBarCategoryAxis *axis = new QBarCategoryAxis();      //    QBarCategoryAxis类将类别添加到图表的坐标轴上
    axis->append(categories);
    chart->createDefaultAxes();//根据已经添加到图表中的系列来创建图表的坐标轴。之前添加到图表中的任何坐标轴都将被删除。

    chart->setAxisX(axis, series);
    QValueAxis *axisY = new QValueAxis;
    // axisX->setRange(-5, 10);
    // axisX->setTickCount(16);
    axisY->setMinorTickCount(10);
    axisY->setLabelFormat("%.2f");
    chart->setAxisY(axisY, series);
    axisY->applyNiceNumbers(); //显示好看的数值
    //![4]
    //chart->setTheme(QChart::ChartThemeHighContrast);  //设置主题

    //![5]
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);//图例是如何与图表对齐的


    //![6]
    //(构建 QChartView，并设置抗锯齿、标题、大小)
    //QChartView是一个独立的小部件，可以显示图表。
    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);//边缘平滑抗锯齿
    chartView->setRubberBand(QChartView::RectangleRubberBand);  //矩形框 放大、缩小
    //chartView->setSizeAdjustPolicy(QAbstractScrollArea::AdjustIgnored);

    chartView->showMaximized();
    //![6]
}
