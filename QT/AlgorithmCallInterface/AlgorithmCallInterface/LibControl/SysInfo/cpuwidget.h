//cpuwidget.h
#ifndef CPUWIDGET_H
#define CPUWIDGET_H
#include <QtCharts/QPieSeries>
#include "sysinfowidget.h"
#include "LibControl_global.h"

class LIBCONTROL_EXPORT CpuWidget : public SysInfoWidget
{
    Q_OBJECT
public:
    explicit CpuWidget(QWidget* parent = 0);
protected slots:
    void updateSeries() override;
private:
    QtCharts::QPieSeries* mSeries;
};

#endif // CPUWIDGET_H
