/*************************************************
  Copyright (C), 2020-2021, AIHua Tech. Co., Ltd.
  File name:    rtmasklayer.cpp
  Author:       yangchun
  Version:      0.0.01
  Date:         2020-9-8
  Description:  通用蒙板
  History:
*************************************************/
#include <QEvent>
#include <QDebug>
#include <QDialog>
#include "qapplication.h"
#include "rtmasklayer.h"
#include <QPaintEvent>
#include <QStyleOption>
#include <QPainter>

RtMaskLayer *RtMaskLayer::m_instance = nullptr;
RtMaskLayer::RtMaskLayer(QWidget *parent) :
    QWidget(parent)
{
   QString str("background-color:rgba(138,138,138,0.6);");   //0.6代表透明度，根据需要自行调整
   this->setStyleSheet(str);

   //绑定全局事件,过滤弹窗窗体进行处理
   qApp->installEventFilter(this);
}

RtMaskLayer::~RtMaskLayer()
{

}

void RtMaskLayer::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);

    QStyleOption opt;
    QPainter p(this);

    opt.init(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void RtMaskLayer::SetMainWidget(QWidget *pWidget)
{
    if (this->m_mainWidget != pWidget){
        this->setParent(pWidget);
        this->m_mainWidget = pWidget;
    }

}

void RtMaskLayer::Show(QDialog *parent)
{
    this->setParent(parent);
    setGeometry(0, 0, parent->width(), parent->height());
    StackUnder(parent);
    show();
}

//void RtMaskLayer::SetOpacity(double opacity)
//{
//    this->setWindowOpacity(opacity);
//}

//void RtMaskLayer::SetBgColor(const QColor &bgColor)
//{
//    QPalette palette = this->palette();
//    palette.setBrush(QPalette::Background, bgColor);
//    this->setPalette(palette);
//}

void RtMaskLayer::SetDialogNames(const QStringList& names)
{
     if(m_dialogNames == names)
     {
         return;
     }

     m_dialogNames = names;
}

void RtMaskLayer::StackUnder(QObject *parent)
{
    QObjectList list = parent->children();
    foreach(QObject *obj , list)
    {
         QWidget *widget = qobject_cast<QWidget*>(obj);
         if(widget){
            widget->stackUnder(this);
         }
    }
}

 QStringList RtMaskLayer::DialogName() const
 {
     return m_dialogNames;
 }

 bool RtMaskLayer::eventFilter(QObject *obj, QEvent *event)
 {
     if(event->type() == QEvent::Hide)
     {
         if(m_dialogNames.contains(obj->objectName()))
         {
             if(m_nowObject == obj->objectName())
             {
                 this->hide();
                 m_nowObject = "no";
             }
         }

         return QObject::eventFilter(obj, event);
     }

     if (event->type() == QEvent::Show)
     {
        if (!m_dialogNames.contains(obj->objectName()))
        {
            return QObject::eventFilter(obj, event);
        }

        //记录开启遮蔽罩的类名
        if("no" == m_nowObject)
        {
            //如果此时遮蔽罩没有开启
            m_nowObject = obj->objectName();
        }

        this->show();

//      pWidget->activateWindow();

//         m_mainWidget->stackUnder(this);
//         auto pWidget = dynamic_cast<QWidget*>(obj);     //将object转换为普通QWidget
//         if (Q_NULLPTR == pWidget)
//         {
//             return QObject::eventFilter(obj, event);
//         }
//         pWidget->activateWindow();
//         pWidget->setFocus(Qt::ActiveWindowFocusReason);
//         stackUnder(pWidget);    //将该窗口设置放到弹窗的下面

//         if(Q_NULLPTR == m_mainWidget)
//         {
//             return QObject::eventFilter(obj, event);
//         }

//         m_mainWidget->stackUnder(this);    //将主窗口设置放到该组件界面下方，就能够有一个比较清晰的层次关系

//         //下面是实现将弹窗的位置移动到主程序的正中间，在这边实现的目的是为了减少代码量，毕竟写代码能偷的懒还是一定要偷的
//         QRect screenGeometry = m_mainWidget->geometry();
//         int x = screenGeometry.x() + (screenGeometry.width() - pWidget->width()) / 2;
//         int y = screenGeometry.y() + (screenGeometry.height() - pWidget->height()) / 2;
//         pWidget->move(x, y);
     }

     return QObject::eventFilter(obj, event);
 }


