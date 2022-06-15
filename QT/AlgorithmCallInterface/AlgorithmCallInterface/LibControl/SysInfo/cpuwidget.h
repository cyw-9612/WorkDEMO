//cpuwidget.h
#ifndef CPUWIDGET_H
#define CPUWIDGET_H
#include <QtCharts/QPieSeries>
#include "sysinfowidget.h"
#include "LibControl_global.h"
#include <QLabel>

class LIBCONTROL_EXPORT CpuWidget : public SysInfoWidget
{
    Q_OBJECT
public:
    explicit CpuWidget(QWidget* parent = 0);

protected:
    void resizeEvent(QResizeEvent *event) override;

protected slots:
    void updateSeries() override;
private:
    QtCharts::QPieSeries* mSeries;

    QLabel *m_centerLabel;
};

#endif // CPUWIDGET_H
