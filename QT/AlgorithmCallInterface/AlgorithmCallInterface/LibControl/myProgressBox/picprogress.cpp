#include "picprogress.h"
#include <QPainter>

picProgress::picProgress(QWidget *parent) :
    QProgressBar(parent)
{
    setMinimum(0);
    //为增强进度条显示效果，总进度设置为1000（1000对应100%）
    setMaximum(1000);
    setValue(0);
}

void picProgress::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    QRect rect = QRect(0, 0, width(), height());
    //计算进度比例对应的宽x
    const double k = (double)(value() - minimum()) / (maximum()-minimum());
    int x = (int)(rect.width() * k);
    QRect fillRect = rect.adjusted(0, 0, x-rect.width(), 0);

    //设置图形
    QString valueStr = QString("%1%").arg(QString::number(value()));
    QPixmap buttomMap = QPixmap(":/images/radius_back.png");
    QPixmap fillMap = QPixmap(":/images/radius_front.png");

    //画进度条
    p.drawPixmap(rect, buttomMap);
    p.save();
    p.translate(1,1);
    p.drawPixmap(fillRect, fillMap, fillRect);
    p.restore();

//    //画文字
//    QFont f = QFont("Microsoft YaHei", 15, QFont::Bold);
//    p.setFont(f);
//    p.setPen(QColor("#58A4B0"));
//    p.drawText(textRect, Qt::AlignCenter, valueStr);
}

void picProgress::setIsFull(bool isFull)
{
    if(isFull)
    {
        setValue(this->maximum());
    }
    else
    {
        setValue(this->minimum());
    }
}
