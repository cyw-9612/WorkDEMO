#include "mycustomplot.h"
#include <QDebug>

mycustomplot::mycustomplot(QWidget *parent)
    :QCustomPlot(parent)
{
}

void mycustomplot::mouseMoveEvent(QMouseEvent *event)
{
    QCustomPlot::mouseMoveEvent(event);

    //当前鼠标位置（像素坐标）
    int x_pos = event->pos().x();
    int y_pos = event->pos().y();

    //像素坐标转成实际的x,y轴的坐标
    qint64 x_val = this->xAxis->pixelToCoord(x_pos) - 0.5;
    qint64 y_val = this->yAxis->pixelToCoord(y_pos);

    QString str,strToolTip;
        str = QString::number(x_val, 10, 3);
        strToolTip += "x: ";
        strToolTip += str;
        strToolTip += "\n";

        float y;
        //获取y轴的值
        for (int i = 0;i < this->xAxis->graphs().count();i++)
        {
            //获得x轴坐标位置对应的曲线上y的值
            y = this->graph(i)->data()->at(x_val)->value;
            str = QString::number(y);
            strToolTip += "y:";
            strToolTip += str;
        }
        //QToolTip::showText(cursor().pos(), strToolTip, this);

    //qDebug() << "x_val" << x_val << "y_val" << y_val;

//    auto iter = this->graph(0)->data()->at(i)->key;
//    double value = iter->mainValue();
//    qDebug() << value;

     //只有鼠标悬浮位置改变且鼠标在柱图上才发送信号
    if(x_val != m_currenyXvalue && y >= y_val)
    {
        //发送坐标变换的信号
        emit mouseChange(x_val, y);
        m_currenyXvalue = x_val;
    }

    this->replot();//曲线重绘
}

