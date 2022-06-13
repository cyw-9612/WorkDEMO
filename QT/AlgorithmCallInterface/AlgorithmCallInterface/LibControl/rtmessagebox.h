/*************************************************
  Copyright (C), 2020-2021, AIHua Tech. Co., Ltd.
  File name:    rtmessagebox.h
  Author:       yangchun
  Version:      0.0.01
  Date:         2020-5-22
  Description:
  History:
*************************************************/
#ifndef RTMESSAGEBOX_H
#define RTMESSAGEBOX_H

#include <QDialog>
#include <QPushButton>
#include <QMouseEvent>
#include <QLabel>
#include "LibControl_global.h"
#include "qdialog.h"

class LIBCONTROL_EXPORT RTMessageBox : public QDialog
{
    Q_OBJECT
public:
    RTMessageBox(int style, QString title, QString text, QString desc);
    RTMessageBox(int style, QString title, QString text);
    RTMessageBox(int style, QString text);
    ~RTMessageBox(void);

protected:
    void mousePressEvent(QMouseEvent *qevent);            //鼠标按下事件
    void mouseReleaseEvent(QMouseEvent *qevent);          //鼠标释放事件
    void mouseMoveEvent(QMouseEvent *qevent);             //鼠标移动事件

public slots:
    void OnOkBtnClick();
    void OnCancleBtnClick();
    void OnCloseBtnClick();

private:
    QPushButton *_btnOK;    //确认按钮
    QPushButton *_btnCancel;//取消按钮
    QPushButton *_btnClose; //右上角关闭按钮
    QPushButton *_picMsg;   //提示图标
    QLabel *_lbTitle;       //标题栏
    QLabel *_lbContext;     //提示消息
    QLabel *_lbDesc;        //提示消息描述信息
    QPoint _mousePos;       //移动的距离
    bool _mousePress;       //鼠标按下
};

#endif // RTMESSAGEBOX_H
