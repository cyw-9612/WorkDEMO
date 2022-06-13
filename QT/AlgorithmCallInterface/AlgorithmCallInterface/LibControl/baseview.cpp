/*************************************************
  Copyright (C), 2020-2021, AIHua Tech. Co., Ltd.
  File name:    baseview.cpp
  Author:       yangchun
  Version:      0.0.01
  Date:         2020-5-15
  Description:  自定义弹出对话框
  History:
*************************************************/
#include <QLineEdit>
#include <QDebug>
#include "rtmessagebox.h"
#include "baseview.h"
#include "logger/ilogger.h"

BaseView::BaseView(QWidget *parent) : QDialog(parent)
{
    setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    m_initUI = false;
    m_modify = false;
    m_timer = new QTimer(this);
    m_timer->setInterval(500);
    m_maskLayer = new RtMaskLayer(this);
    m_maskLayer->hide();
    m_enableMove = true;
}

// 初始化界面信号
void BaseView::InitSignal()
{

}

// 初始化界面
void BaseView::InitUI()
{

}

// 获取界面标题
QString BaseView::GetTitle()
{
    return m_title;
}

// 设置界面标题
void BaseView::SetTitle(QString title)
{
    this->m_title = title;
}

// 页导航
void BaseView::Navigate(int pageID)
{
    Q_UNUSED(pageID);
}

// 加载数据
void BaseView::LoadDate()
{

}

// 显示界面
void BaseView::Show()
{
    this->show();
}

// 隐藏界面
void BaseView::Hide()
{
    this->hide();
}

// 设置日志
void BaseView::SetLogger(ILogger *logger)
{
    m_logger = logger;
}

void BaseView::HandleException(exception &ex)
{
    m_logger->WriteExpection(ex);
}

//显示等待框
void BaseView::ShowWaiting()
{
    return;
}

//隐藏等待框
void BaseView::HideWaiting()
{
    return;
}

QWidget *BaseView::GetQWidget()
{
    return this;
}

// 根据不同事件，修改界面状态
void BaseView::ModifyControlStyle(UIEventType_E event, int error)
{
    Q_UNUSED(event);
    Q_UNUSED(error);

    return;
}

// 设置下拉框对齐方式
void BaseView::SetComboBoxAlignment(QComboBox *control, Qt::AlignmentFlag flag)
{
    if(control == nullptr)
    {
        return;
    }

    QLineEdit *edtPower = new QLineEdit(control);
    edtPower->setReadOnly(true);
    edtPower->setStyleSheet("background-color: rgb(255, 255, 255); font: 18px '微软雅黑';border: 0px solid;");

    edtPower->setAlignment(flag);
    control->setLineEdit(edtPower);
}

// 显示提示
int BaseView::ShowTotast(TotastType_E type, QString title, QString msg, QString des)
{
    Q_UNUSED(title);

    m_maskLayer->Show(this);

    RTMessageBox messageBox(type, title, msg, des);
    int res = messageBox.exec();

    m_maskLayer->hide();
    return res;
}

// 处理按键事件
void BaseView::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
        case Qt::Key_Escape://ESC按键点击,直接返回, 防止界面直接关闭
            break;
        default:
            QDialog::keyPressEvent(event);
    }
}

//最小化
void BaseView::SlotMin()
{
    this->showMinimized();
}

//最大化/还原
void BaseView::SlotMax()
{
    if(!this->isMaximized())
    {
        //最大化
        this->showMaximized();
    }
    else
    {
        //还原
        this->showNormal();
    }
}

//关闭
void BaseView::SlotClose()
{
    close();
}
