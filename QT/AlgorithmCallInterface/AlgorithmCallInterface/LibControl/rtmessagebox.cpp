/*************************************************
  Copyright (C), 2020-2021, AIHua Tech. Co., Ltd.
  File name:    RTMessageBox.cpp
  Author:       yangchun
  Version:      0.0.01
  Date:         2020-5-22
  Description:
  History:
*************************************************/
#include "rtmessagebox.h"

RTMessageBox::RTMessageBox(int style, QString title, QString text, QString desc)
{
    _mousePress = false;
    int width = 360;
    if(style==1)
    {
        this->resize(width, 197);
        setFixedSize(width, 197);
    }
    else
    {
        this->resize(width, 167);
        setFixedSize(width, 167);
    }

    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog );//设置标题栏隐藏
    this->setStyleSheet("QDialog{border: 3px solid rgb(55, 61, 81);}");
    /*
     * 创建并设置标题栏样式
     */
    _lbTitle = new QLabel(this);
    _lbTitle->setGeometry(0, 0, width, 30);
    _lbTitle->setStyleSheet("QLabel{font-family :微软雅黑;font-size : 15px;color : rgb(233,233,234);background-color: rgb(55,63,81);}");

    if(title.isEmpty() == true)
    {
        if(style==1)
        {
            _lbTitle->setText(QStringLiteral("  警告  "));
        }
        else
        {
            _lbTitle->setText(QStringLiteral("  提示  "));
        }
    }
    else
    {
        _lbTitle->setText(title.insert(0, "  "));
    }

    /*
     * 创建并设置关闭按钮样式
     */
    _btnClose = new QPushButton(this);
    _btnClose->setGeometry(width - 30, 5, 20, 20);
    _btnClose->setText("");
    _btnClose->setStyleSheet("QPushButton{font-family :微软雅黑;font-size : 13px;color : rgb(233,233,234);"
                             "image:url(:/images/messagebox/close_normal);background: transparent;border:none;}"
                             "QPushButton:hover{background-color: rgb(237,28,36);border:none;}");

    /*
     * 创建并设置消息标签样式
     */
    _lbContext = new QLabel(this);
    _lbContext->setGeometry(123, 36, width - 130, 50);
    _lbContext->setWordWrap(true);
    _lbContext->setText(text);
    _lbContext->setStyleSheet("font: 13px 微软雅黑;");
    if(desc.isEmpty() == false)
    {
        _lbContext->setGeometry(123, 26, width - 130, 50);

        _lbDesc = new QLabel(this);
        _lbDesc->setWordWrap(true);
        _lbDesc->setGeometry(123, 66, width - 130, 50);
        _lbDesc->setText(desc);
        _lbDesc->setStyleSheet("font: 13px 微软雅黑; color : rgb(237,28,36);");
    }

    /*
     * 创建并设置消息图标按钮样式
     */
    _picMsg = new QPushButton(this);
    _picMsg->setGeometry(40, 56, 44, 44);
    if(style == 1)
    {
        _picMsg->setStyleSheet("QPushButton{image:url(:/images/messagebox/info.png);background: transparent;border:none;}");
    }
    else if(style == 2)
    {

        _picMsg->setStyleSheet("QPushButton{image:url(:/images/messagebox/warning.png);background: transparent;border:none;}");
    }
    else if(style == 3)
    {
        _picMsg->setStyleSheet("QPushButton{image:url(:/images/messagebox/answer.png);background: transparent;border:none;}");
    }

    /*
     * 创建并设置确认按钮
     */
    _btnOK=new QPushButton(this);
    if(style==3)
    {
        _btnOK->setGeometry(187, 119, 85, 30);
    }
    else if(style==1)
    {
        _btnOK->setGeometry(187, 119, 85, 30);
    }
    else
    {
        _btnOK->setGeometry(135, 119, 85, 30);
    }

    _btnOK->setText(QStringLiteral("确定"));
    _btnOK->setStyleSheet("QPushButton{border:1px rgb(86,164,177);border-radius:4px;border-color: rgb(86,164,177);background-color: rgb(86,164,177);}QPushButton:hover{background-color: rgb(119,146,183);}QPushButton:pressed{background-color: rgb(139,159,185);}");

    /*
     * 创建并设置取消按钮
     */
    _btnCancel=new QPushButton(this);
    _btnCancel->setGeometry(90, 119, 85, 30);
    _btnCancel->setText(QStringLiteral("取消"));
    _btnCancel->setStyleSheet("QPushButton{border:1px rgb(84,91,110);border-radius:4px;border-color: rgb(84,91,110);;background-color: rgb(84,91,110);}QPushButton:hover{background-color: rgb(119,146,183);}QPushButton:pressed{background-color: rgb(139,159,185);}");
    if(style == 3)
    {
        _btnCancel->show();
    }
    else
    {
        _btnCancel->hide();
    }

    if(style == 1)
    {
        this->resize(360, 197);
        _picMsg->setGeometry(width / 2 - 60 / 2, 45, 60, 60);
        _lbContext->setGeometry(0, 110, width, 50);
        _lbContext->setAlignment(Qt::AlignCenter);
        _btnOK->setGeometry(width - 95, 159, 85, 30);
        _picMsg->setStyleSheet("QPushButton{image:url(:/images/messagebox/info.png);background: transparent;border:none;}");
    }

    connect(_btnOK,SIGNAL(clicked()),this,SLOT(OnOkBtnClick()));
    connect(_btnClose,SIGNAL(clicked()),this,SLOT(OnCloseBtnClick()));
    connect(_btnCancel,SIGNAL(clicked()),this,SLOT(OnCancleBtnClick()));
}

RTMessageBox::RTMessageBox(int style, QString title, QString text)
    : RTMessageBox(style, title, text, "")
{

}

RTMessageBox::RTMessageBox(int style, QString text)
    : RTMessageBox(style, "", text, "")
{

}

RTMessageBox::~RTMessageBox(void)
{
}

/*************************************************
  Function: mousePressEvent
  Description: 鼠标按键点击事件
  Input:    qevent：鼠标事件
  Output:   无
  Return:   无
*************************************************/
void RTMessageBox::mousePressEvent(QMouseEvent *qevent)
{
    if(qevent->button() == Qt::LeftButton)//处理对话框拖动, 鼠标左键点击, 记录当前位置
    {
        if(_lbTitle->rect().contains(qevent->pos()))
        {
            _mousePress = true;
            _mousePos = qevent->pos();//鼠标相对于窗体的位置（或者使用event->globalPos() - this->pos()）
        }
    }
}

/*************************************************
  Function: mouseMoveEvent
  Description: 鼠标移动事件
  Input:    qevent：鼠标事件
  Output:   无
  Return:   无
*************************************************/
void RTMessageBox::mouseMoveEvent(QMouseEvent *qevent)
{
    if(_mousePress)//处理对话框拖动, 鼠标左键被按下，移动对话框
    {
        QPoint pos = qevent->globalPos();//鼠标相对于屏幕的位置
        this->move(pos - _mousePos);//移动主窗体位置
    }
}

/*************************************************
  Function: mouseReleaseEvent
  Description: 鼠标按键释放事件
  Input:    qevent：鼠标事件
  Output:   无
  Return:   无
*************************************************/
void RTMessageBox::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);

    _mousePress = false;//设置鼠标为未被按下
}

/*************************************************
  Function: OnOkBtnClick
  Description: 确认按钮按下事件
  Input:    无
  Output:   无
  Return:   无
*************************************************/
void RTMessageBox::OnOkBtnClick()
{
    this->accept();
}

/*************************************************
  Function: OnCancleBtnClick
  Description: 取消按钮按下事件
  Input:    无
  Output:   无
  Return:   无
*************************************************/
void RTMessageBox::OnCancleBtnClick()
{
    this->reject();
}

/*************************************************
  Function: OnCloseBtnClick
  Description: 关闭按钮按下事件
  Input:    无
  Output:   无
  Return:   无
*************************************************/
void RTMessageBox::OnCloseBtnClick()
{
    close();
}
