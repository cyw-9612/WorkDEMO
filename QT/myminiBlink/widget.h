#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "webwidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    //鼠标按下事件
    void mousePressEvent(QMouseEvent *event);
    //鼠标移动事件
    void mouseMoveEvent(QMouseEvent *event);
    //释放鼠标
    void mouseReleaseEvent(QMouseEvent *event);

    //目前控件在父窗口中的位置
    QPoint m_windowPos;
    //鼠标位置
    QPoint m_mousePos;
    //移动后位置
    QPoint m_dPos;
    //是否移动窗体标志位： true 移动,false 不动
    bool m_moveFlag=false;

private:
    Ui::Widget *ui;

    WebWidget* m_webWidget;
};
#endif // WIDGET_H
