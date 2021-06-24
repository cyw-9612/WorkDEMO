#ifndef MINIMEAASGEBOX_H
#define MINIMEAASGEBOX_H

#include <QDialog>

/***********************************
 *
 * 功 能: 自定义消息提示消息框
 *
 * 编 制: 陈宇葳
 *
 * 日 期: 2021/01/21
 *
 **********************************/

namespace Ui {
class miniMeaasgeBox;
}

enum Colortype
{
    black = 0,
    red = 1,
};

class miniMeaasgeBox : public QDialog
{
    Q_OBJECT

public:
    explicit miniMeaasgeBox(QWidget *parent = nullptr);
    ~miniMeaasgeBox();

    //设置风格
    void setStyle();

    //关联信号槽
    void connectSlots();

    //初始化界面
    void initUI();

    //设置控件输入范围
     void setCtrlRange();

    //设置提示窗信息
    void setWidgetMessage(QString titile, QString message , int color);

signals:
    //消息框点击确认
    void messageBoxCheck();

private slots:
    //关闭槽函数
    void slotClose();
    //发送信号用槽函数
    void slotSendSignal();

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
    Ui::miniMeaasgeBox *ui;

    QString m_title;
    QString messageMark;

    bool Show = false;
};

#endif // MINIMEAASGEBOX_H
