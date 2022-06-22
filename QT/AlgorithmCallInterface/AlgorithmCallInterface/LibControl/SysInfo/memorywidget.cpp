//memorywidget.cpp
#include "memorywidget.h"
#include <QtCharts/QAreaSeries>
#include <QPen>
#include <QLinearGradient>
#include "sysinfo.h"

using namespace QtCharts;

const int CHART_X_RANGE_COUNT = 50;
const int CHART_X_RANGE_MAX = CHART_X_RANGE_COUNT - 1;

//渐变色的起始色和终止色
const int COLOR_DARK_BLUE = 0x209fdf;
const int COLOR_LIGHT_BLUE = 0xbfdfef;
const int PEN_WIDTH = 3;

MemoryWidget::MemoryWidget(QWidget *parent) :
    SysInfoWidget(parent),
    mSeries(new QLineSeries(this)),
    mPointPositionX(0)
{
    QPen pen(COLOR_DARK_BLUE);
    pen.setWidth(PEN_WIDTH);

    QLinearGradient gradient(QPointF(0, 0), QPointF(0, 1));
    gradient.setColorAt(1.0, COLOR_DARK_BLUE);
    gradient.setColorAt(0.0, COLOR_LIGHT_BLUE);
    gradient.setCoordinateMode(QGradient::ObjectBoundingMode);

     //动态刷线曲线图
    QAreaSeries* areaSeries = new QAreaSeries(mSeries);
    areaSeries->setPen(pen);
    areaSeries->setBrush(gradient);

    QChart* chart = chartView().chart();
    QMargins m(-10,-13,-10,-15);
    chart->setMargins(m);
    chart->addSeries(areaSeries);
//    chart->setTitle("Memory used");
    chart->setTitle("内存使用情况");
    chart->createDefaultAxes();
    chart->axisX()->setRange(0, CHART_X_RANGE_MAX);
    chart->axisX()->setVisible(false);
    chart->axisY()->setRange(0, 100);
}

void MemoryWidget::updateSeries()
{
    //刷线内存使用率
    double memoryUsed = SysInfo::instance().memoryUsed();
    emit sigMemoryData(memoryUsed);
    mSeries->append(mPointPositionX++, memoryUsed);
    //超出显示范围之后往前滚动一个格子
    if (mSeries->count() > CHART_X_RANGE_COUNT) {
        QChart* chart = chartView().chart();
        chart->scroll(chart->plotArea().width() / CHART_X_RANGE_MAX, 0);
        mSeries->remove(0);
    }
}
