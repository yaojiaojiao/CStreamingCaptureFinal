#include "linechart.h"
#include "barchart.h"





linechart::linechart(QObject *parent) : QObject(parent)
{

}

void linechart::line(QVector<float> vectorA, int clectNumber)
{

 QLineSeries *series = new QLineSeries();     //新建折线图类

 for(int i=0;i<clectNumber;)               //加入填充点
         {
            *series << QPointF(i, vectorA[i]);
             i++;
          }


 QChart *chart = new QChart();
 chart->legend()->hide();
 chart->addSeries(series);
 chart->createDefaultAxes();    //先创建默认坐标轴

// QStringList categories;
//     for(int i=0;i<clectNumber;)
//     {
//         categories <<QString::number(i+1);
//         i++;
//      }
// QBarCategoryAxis *axis = new QBarCategoryAxis();
// axis->append(categories);

 QValueAxis *axisX= new QValueAxis;
  chart->setAxisX(axisX, series);
  axisX->setTickCount(int(clectNumber/10));
  axisX->setMinorTickCount(10);
//  axisX->setLabelFormat("%.g");
//  axisX->setRange(0,clectNumber);
  axisX->applyNiceNumbers();
 chart->setTitle("Line Chart");

 axisX->setTitleText("点数");



 QValueAxis *axisY= new QValueAxis;
//         axisX->setRange(-5, 10);
//          axisX->setTickCount(16);
 axisY->setMinorTickCount(10);
   axisX->setLabelFormat("%.1f");
  chart->setAxisY(axisY, series);
  axisY->applyNiceNumbers();
  axisY->setTitleText(tr("信号"));


//  chart->setAnimationOptions(QChart::SeriesAnimations);  //动画选项
  chart->setAnimationOptions(QChart::NoAnimation);  //动画选项
//![3]

//![4]
 chartView = new QChartView(chart);
 chartView->setRenderHint(QPainter::Antialiasing);
 chartView->setRubberBand(QChartView::RectangleRubberBand);  //矩形框 放大、缩小
 chartView->showMaximized();
}





