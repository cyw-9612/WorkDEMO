//memorywidget.h
#ifndef MEMORYWIDGET_H
#define MEMORYWIDGET_H
#include <QtCharts/QLineSeries>
#include "sysinfowidget.h"
#include "LibControl_global.h"

class LIBCONTROL_EXPORT MemoryWidget : public SysInfoWidget
{
    Q_OBJECT
public:
    explicit MemoryWidget(QWidget *parent = 0);

signals :
    void sigMemoryData(double num);

protected slots:
    void updateSeries() override;
    //以曲线图动态显示内存的占用率
private:
    QtCharts::QLineSeries* mSeries;
    qint64 mPointPositionX;
};
#endif // MEMORYWIDGET_H
