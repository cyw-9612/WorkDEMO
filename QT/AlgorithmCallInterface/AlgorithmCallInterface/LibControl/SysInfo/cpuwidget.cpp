//cpuwidget.cpp
#include "cpuwidget.h"
#include "sysinfo.h"
using namespace QtCharts;
CpuWidget::CpuWidget(QWidget* parent) :
    SysInfoWidget(parent),
    mSeries(new QPieSeries(this))
{
    //以饼状图显示CPU的使用率
    mSeries->setPieSize(0.9);//饼图的大小
    mSeries->setHoleSize(0.5); //设置空心圆
    mSeries->append("CPU Load", 30.0);
    mSeries->append("CPU Free", 70.0);

    QChart* chart = chartView().chart();
    QMargins m(-10,-10,-10,-10);
    chart->setMargins(m);
    chart->addSeries(mSeries);
//    chart->setTitle("CPU average load");
    chart->setTitle("CPU 占用率");

    m_centerLabel = new QLabel(this);
    m_centerLabel->setAlignment(Qt::AlignHCenter);
    m_centerLabel->setStyleSheet(QString("background:transparent;font-family:\"微软雅黑\";\
    font-size:14px; color:#209fdf; font-weight:900;"));
    m_centerLabel->hide();
}
void CpuWidget::updateSeries()
{
    //动态刷新CPU的使用率
    double cpuLoadAverage = SysInfo::instance().cpuLoadAverage();
    mSeries->clear();
    mSeries->append("Load", cpuLoadAverage);
    mSeries->append("Free", 100.0 - cpuLoadAverage);

    QString text = QString::number(cpuLoadAverage, 'f', 1) + "%";
    m_centerLabel->setText(text);
    m_centerLabel->show();
    resizeEvent(NULL);
    // 设置块标签
//        for(int sNum=0;sNum<mSeries->slices().count();sNum++)
//        {
//            QPieSlice *slice =mSeries->slices().at(sNum);
//            QString tagText = QString("%1%2%3")
//                    .arg(slice->label())
//                    .arg(QStringLiteral("占比"))
//                    .arg(QString::number(slice->percentage()*100));
//            slice->setLabel(tagText);
//        }

//        // 设置标签显示位置
//        mSeries->setLabelsPosition(QPieSlice::LabelPosition::LabelOutside);

//        // 显示标签
//        mSeries->setLabelsVisible(true);
}

void CpuWidget::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    m_centerLabel->move((this->width()-m_centerLabel->width())/2,
                        (this->height()-m_centerLabel->height())/2-5 + 20);
}
