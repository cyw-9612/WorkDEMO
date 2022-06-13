/*************************************************
  Copyright (C), 2020-2021, AIHua Tech. Co., Ltd.
  File name:    popupview.cpp
  Author:       yangchun
  Version:      0.0.01
  Date:         2020-5-15
  Description:  自定义内嵌对话框
  History:
*************************************************/
#include <QDebug>
#include <QMouseEvent>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QPushButton>
#include <QLabel>
#include "popupview.h"

PopupView::PopupView(QWidget *parent) :
    BaseView(parent)
{
    int width = 1166;

    setWindowFlags(Qt::Widget | Qt::CustomizeWindowHint| Qt::FramelessWindowHint);

    m_drag = false;
    m_lbTitle = new QLabel(this);
    m_lbTitle->setGeometry(0, 0, width, 40);
    m_lbTitle->setStyleSheet("background-color: rgb(55, 61, 81); color: rgb(255, 255, 255);font: 18px 微软雅黑; padding-left: 10px");

    m_btnClose = new QPushButton(this);
    m_btnClose->setGeometry(width - 30, 10, 20, 20);
    m_btnClose->setText("");
    m_btnClose->setStyleSheet("QPushButton{font-family :微软雅黑;font-size : 5em;color : rgb(233,233,234);"
                           "image:url(:/images/login/close_normal);background: transparent;border:none;}"
                           "QPushButton:hover{background-color: rgb(237,28,36);border:none;}");

    this->setStyleSheet("");

    setFixedSize(1166, 765);

    connect(m_btnClose, &QPushButton::clicked, this, &PopupView::OnCloseSlots);
}

//实现移动标题栏功能
void PopupView::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        if(m_lbTitle->rect().contains(event->pos()))
        {
            m_drag = true;
            m_mouseStartPoint = event->globalPos();//获得鼠标的初始位置
            m_windowTopLeftPoint = this->frameGeometry().topLeft();//获得窗口的初始位置
        }
    }
}

//实现移动标题栏功能
void PopupView::mouseMoveEvent(QMouseEvent *event)
{
    if(m_drag)
    {
        //获得鼠标移动的距离
        QPoint distance = event->globalPos() - m_mouseStartPoint;
        //改变窗口的位置
        this->move(m_windowTopLeftPoint + distance);
    }
}

//实现移动标题栏功能
void PopupView::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        m_drag = false;
    }
}

// 处理关闭按钮事件
void PopupView::OnCloseSlots()
{
    emit this->CloseSignals();
}

// 设置标题栏文字
void PopupView::SetTitle(QString title)
{
    m_lbTitle->setText(title);
}

// 设置界面大小
void PopupView::SetSize(int width, int height)
{
    m_lbTitle->setGeometry(0, 0, width, 40);
    m_btnClose->setGeometry(width - 30, 10, 20, 20);

    setFixedSize(width, height);
}

// 居中显示
void PopupView::MoveToCenter()
{
    if(this->parent() != nullptr)
    {
        QWidget *frame =  static_cast<QWidget *>(this->parent());// 获取主界面
        QPoint pos = frame->mapToGlobal(QPoint(0, 0));// 客户区坐标转屏幕坐标

        this->move(pos.rx() + ((frame->width() - this->width()) / 2), pos.ry() + ((frame->height() - this->height()) / 2));// 移动当前界面位置
    }
}

