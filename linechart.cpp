#include "linechart.h"

linechart::linechart(QObject *parent) : QObject(parent)
{
}

void linechart::line(QVector<float> vectorA, int clectNumber)
{

    QLineSeries *series = new QLineSeries();     //�½�����ͼ��

    for(int i=0;i<clectNumber;)               //��������
    {
        *series << QPointF(i+1, vectorA[i]);
        i++;
    }

    QChart *chart = new QChart();
    chart->legend()->hide();
    chart->addSeries(series);
    //series->setUseOpenGL(true);//���٣�
    chart->createDefaultAxes();    //�ȴ���Ĭ��������

    QValueAxis *axisX= new QValueAxis;

    //  axisX->setTickCount(series->count());// setTickCount���¿���
    axisX->setMinorTickCount(10);
    axisX->setLabelFormat("%.i");
    axisX->applyNiceNumbers();
    axisX->setTitleText("����");
    axisX->setTickCount(10);
    chart->setAxisX(axisX, series);
    chart->setTitle("Line Chart");

    QValueAxis *axisY= new QValueAxis;
    axisY->setMinorTickCount(10);
    axisY->setLabelFormat("%.1f");
    chart->setAxisY(axisY, series);
    axisY->applyNiceNumbers();
    axisY->setTitleText(tr("�ź�"));
    chart->setAnimationOptions(QChart::NoAnimation);  //����ѡ��

    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setRubberBand(QChartView::RectangleRubberBand);  //���ο� �Ŵ���С

}





