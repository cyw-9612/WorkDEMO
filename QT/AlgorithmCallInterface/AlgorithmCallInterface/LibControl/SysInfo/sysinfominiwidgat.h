#ifndef SYSINFOMINIWIDGAT_H
#define SYSINFOMINIWIDGAT_H

#include <QWidget>
#include <QTimer>
#include <QtCharts/QChartView>
#include "childview.h"
#include "LibControl_global.h"

namespace Ui {
class sysInfoMiniWidgat;
}

class LIBCONTROL_EXPORT sysInfoMiniWidgat : public QWidget
{
    Q_OBJECT

public:
    explicit sysInfoMiniWidgat(QWidget *parent = 0);
    ~sysInfoMiniWidgat();

public slots:
    void updateCPULab(double cpuText);

    void updateMemoryLab(double memoryText);

private:
    Ui::sysInfoMiniWidgat *ui;
};

#endif // SYSINFOMINIWIDGAT_H
