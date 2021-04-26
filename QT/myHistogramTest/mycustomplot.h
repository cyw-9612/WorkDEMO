#ifndef MYCUSTOMPLOT_H
#define MYCUSTOMPLOT_H

#include "qcustomplot.h"
#include <QObject>
#include <QList>

class mycustomplot:public QCustomPlot
{
    Q_OBJECT
public:
    mycustomplot(QWidget *parent = nullptr);

protected:
    virtual void mouseMoveEvent(QMouseEvent *event);

signals:
    void mouseChange(int x_val, float y_val);

private:
    int m_currenyXvalue = 0;
};

#endif // MYCUSTOMPLOT_H
