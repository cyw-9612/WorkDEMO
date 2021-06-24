#ifndef FIRMMINIMESSAGE_H
#define FIRMMINIMESSAGE_H

#include <QWidget>

namespace Ui {
class firmMiniMessage;
}

/***********************************
 *
 * 功 能: 小一点的消息提示消息框[DLL导出]
 *
 * 编 制: 陈宇葳
 *
 * 日 期: 2021/01/21
 *
 **********************************/
enum QColortype
{
    black = 0,
    red = 1,
};

class  firmMiniMessage : public QWidget
{
    Q_OBJECT

public:
    explicit firmMiniMessage(QString titile,QString message, QWidget *parent = nullptr);
    ~firmMiniMessage();

    //设置风格
    void setStyle();

    //关联信号槽
    void connectSlots();

    //初始化界面
    void initUI();

    //设置控件输入范围
     void setCtrlRange();

     //查询改对象有没有关闭
    bool isShow();

    //设置提示窗信息
    void setWidgetMessage(QString titile, QString message , int color);

     void paintEvent(QPaintEvent *event);

signals:
    //消息框点击确认
    void messageBoxCheck();

    //窗口显示消息
    void messageBoxShow();

    //窗口关闭消息
    void messageBoxClose();
    void messageBoxCloseX(bool);

private slots:
    //关闭槽函数
    void slotClose();
    //发送信号用槽函数
    void slotSendSignal();

private:
    Ui::firmMiniMessage *ui;

    QString m_title;
    QString messageMark;

    bool Show = false;
};

#endif // FIRMMINIMESSAGE_H
