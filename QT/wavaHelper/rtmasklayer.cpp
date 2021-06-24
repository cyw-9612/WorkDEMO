#include "rtmasklayer.h"
#include <QEvent>
#include <QDebug>
#include <QDialog>
#include "qapplication.h"
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

     }

     return QObject::eventFilter(obj, event);
 }
