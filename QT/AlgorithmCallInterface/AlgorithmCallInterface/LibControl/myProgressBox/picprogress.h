#ifndef PICPROGRESS_H
#define PICPROGRESS_H
#include <QProgressBar>

/***********************************
 *
 * 功 能: 自定义动态进度条绘制类
 *
 * 编 制: 陈宇葳
 *
 * 日 期: 2020/11/26
 *
 **********************************/

class picProgress : public QProgressBar
{
    Q_OBJECT
public:
    explicit picProgress(QWidget *parent = 0);

    //设置进度条为全满或全空
    void setIsFull(bool isFull);

signals:

public slots:

protected:
    void paintEvent(QPaintEvent *) Q_DECL_OVERRIDE;
};

#endif // PICPROGRESS_H
