#ifndef DATAHISTOGRAM_H
#define DATAHISTOGRAM_H

#include <QWidget>
#include "qcustomplot.h"
#include "mycustomplot.h"
#include "mytooltip.h"

QT_BEGIN_NAMESPACE
namespace Ui { class dataHistogram; }
QT_END_NAMESPACE

class dataHistogram : public QWidget
{
    Q_OBJECT

public:
    dataHistogram(QWidget *parent = nullptr);
    ~dataHistogram();

private slots:
    void slotMouseChange(int x_value, float y_val);

private:
    Ui::dataHistogram *ui;

    QList<QCPBars*> m_listBar; //实际数据对象列表
    QList<QColor> m_listColor; //不同数据的柱状图颜色
    QCPBars *m_mousebar; //鼠标数据对象

    QVector<double> ticks; //坐标轴计数
    QVector<double> v_Mousedata; //鼠标对应的坐标轴数据列表

    QVector<QString> labels; //要显示的刻度的下标

    myToolTip *tip = nullptr;
};
#endif // DATAHISTOGRAM_H
