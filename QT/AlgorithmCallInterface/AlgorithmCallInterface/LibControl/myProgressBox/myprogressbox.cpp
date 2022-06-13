#include "myprogressbox.h"
#include "ui_myprogressbox.h"
#include <QMouseEvent>
#include <QDebug>

myProgressBox::myProgressBox(QWidget *parent) :
    ChildView(parent),
    ui(new Ui::myProgressBox)
{
    ui->setupUi(this);

    m_messageDialog = new myMessageDialog(this);

    initUI();

    setStyle();

    setCtrlRange();

    connectSlots();

    //且无边框，保持最前
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    this->hide();
}

myProgressBox::~myProgressBox()
{
    if(m_messageDialog!=nullptr)
    {
        delete m_messageDialog;
        m_messageDialog = nullptr;
    }
    delete ui;
}

//设置风格
void myProgressBox::setStyle()
{
    //退出风格
    ui->btn_close->setToolTip(tr("关闭消息框"));

    ui->lab_title->setText(tr("正在读取"));
    ui->lab_info->setText(tr("设备数据读取中请等待..(请勿在读取过程中断开连接!)"));
    ui->lab_read->setText(tr("共需读取:"));
    ui->lab_read_2->setText(tr("条"));
    ui->lab_now->setText(tr("当前读取数据编号:"));

    ui->btn_cancel->setText(tr("取消"));
}

//关联信号槽
void myProgressBox::connectSlots()
{
    //父窗口更新进度信息连接
    //connect(parent() ,SIGNAL(sigUpdateProgress(int,QString)), this, SLOT(slotUpdataProgress(int,QString)));

    //取消按钮
    connect(ui->btn_close, SIGNAL(clicked()),this, SLOT(slotCancelRead()));
    connect(ui->btn_cancel, SIGNAL(clicked()),this, SLOT(slotCancelRead()));

    //绑定对话框信息
     connect(m_messageDialog, SIGNAL(sigCancelRead(const bool& )), this, SLOT(slotCancelRead(const bool& )));
}

//初始化界面
void myProgressBox::initUI()
{
    //鼠标移动标志位初始化
    m_drag = false;

    ui->lab_num->setText("0%");
    ui->widget_MyProgress->setValue(0);

     QString dialogTitle = tr("操作提示");
     QString meaasge = tr("确定要取消读取数据吗?");
    m_messageDialog->setDialogStyle(enum_dialogStyle::type_redIconTwoBtn,enum_messageStyle::type_readMessage,
                                    dialogTitle, meaasge);
     // 隐藏对话框
    m_messageDialog->Hide();
}

//置控件输入范围
void myProgressBox::setCtrlRange()
{

}

//设置进度条窗口风格
void myProgressBox::setDialogStyle(int dialogStyle)
{
    m_progressStyle = dialogStyle;

    switch (dialogStyle)
    {
        case enum_progressStyle::type_connectStyle :
        {
            setLabIsVisable(false);
            ui->lab_title->setText(tr("设备连接"));
            ui->lab_info->setText(tr("设备连接中请等待..(请勿在连接过程中断开连接!)"));
            QString dialogTitle = tr("操作提示");
            QString meaasge = tr("确定要取消连接吗?");
           m_messageDialog->setDialogStyle(enum_dialogStyle::type_redIconTwoBtn,enum_messageStyle::type_readMessage,
                                           dialogTitle, meaasge);
           ui->labInfo->setVisible(false);
        }
        break;
    case enum_progressStyle::type_analyStyle :
        setLabIsVisable(false);
        ui->lab_title->setText(tr("数据解析"));
        ui->lab_info->setText(tr("设备数据解析中请等待..(请勿在解析过程中断开连接!)"));
        //隐藏按钮
        ui->btn_cancel->setVisible(false);
        ui->labInfo->setVisible(true);
        ui->labInfo->setText("");
        break;
    default:
        {
            setLabIsVisable(true);
            ui->lab_title->setText(tr("正在读取"));
            ui->lab_info->setText(tr("设备数据读取中请等待..(请勿在读取过程中断开连接!)"));
            QString dialogTitle = tr("操作提示");
            QString meaasge = tr("确定要取消读取数据吗?");
           m_messageDialog->setDialogStyle(enum_dialogStyle::type_redIconTwoBtn,enum_messageStyle::type_readMessage,
                                           dialogTitle, meaasge);
           ui->labInfo->setVisible(false);
        }

        break;
    }
}


//设置进度条相关信息
void myProgressBox::setDataCount(int piceCount)
{
    //设置信息总数
    ui->lab_readCount->setText(QString::number(piceCount));
    //总数不显示
    ui->lab_readCount->hide();
    ui->lab_read->hide();
    ui->lab_read_2->hide();
    //读取条数不显示
    ui->lab_now->hide();
    ui->lab_nowCount->hide();

    m_maxCount = piceCount;

    //设置进度条参数
    ui->widget_MyProgress->setMaximum(piceCount);
    ui->widget_MyProgress->setMinimum(0);
    ui->widget_MyProgress->setValue(0);
    ui->lab_num->setText("0%");
}

//只更新进度条槽函数
void myProgressBox::slotUpdataProgress(int value)
{
    if(value < 0)
        value = 0;

    if(value > m_maxCount)
        value = m_maxCount;

    QString num = tr("%1%").arg((value*100)/m_maxCount);
    ui->lab_num->setText(num);
    ui->widget_MyProgress->setValue(value);
}

//进度条更新槽函数
void myProgressBox::slotUpdataProgress(int value,QString dataName)
{
    if(value < 0)
        value = 0;

    if(value > m_maxCount)
        value = m_maxCount;

    QString num = tr("%1%").arg((value*100)/m_maxCount);
    ui->lab_num->setText(num);
    ui->widget_MyProgress->setValue(value);

    ui->lab_nowCount->setText(dataName);
}

//用户点击取消按钮
void myProgressBox::slotCancelRead()
{
     m_messageDialog->Show();
}

//取消读取
void myProgressBox::slotCancelRead(const bool& isCancel)
{
    if(isCancel)
    {
        // 进行取消读取操作
        qDebug() << "myProgressBox::slotCancelRead 用户确认取消读取（連接）操作";
        if(m_progressStyle == enum_progressStyle::type_readStyle)
        {
            emit cancelRead(true);
        }
        else if(m_progressStyle == enum_progressStyle::type_connectStyle)
        {
            emit cancelConnect(true);
        }
        else if(m_progressStyle == enum_progressStyle::type_analyStyle)
        {
            emit cancelReadFile(true);
        }
    }
}

// 设置中间标签是否显示
void myProgressBox::setLabIsVisable(bool isVisable)
{
    ui->lab_num->setVisible(isVisable);
    ui->lab_read->setVisible(isVisable);
    ui->lab_readCount->setVisible(isVisable);
    ui->lab_read_2->setVisible(isVisable);
    ui->lab_now->setVisible(isVisable);
    ui->lab_nowCount->setVisible(isVisable);

    //默认连接和解析 进度条为全满状态
    ui->widget_MyProgress->setIsFull(!isVisable);
}

//实现移动标题栏功能
void myProgressBox::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        if(ui->widget->rect().contains(event->pos()))
        {
            m_drag = true;
            m_mouseStartPoint = event->globalPos();//获得鼠标的初始位置
            m_windowTopLeftPoint = this->frameGeometry().topLeft();//获得窗口的初始位置
        }
    }
}

//实现移动标题栏功能
void myProgressBox::mouseMoveEvent(QMouseEvent *event)
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
void myProgressBox::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        m_drag = false;
    }
}

// 居中显示
void myProgressBox::MoveToCenter()
{
    if(this->parent() != nullptr)
    {
        QWidget *frame =  static_cast<QWidget *>(this->parent());// 获取主界面
        QPoint pos = frame->mapToGlobal(QPoint(0, 0));// 客户区坐标转屏幕坐标
        this->move(pos.rx() + ((frame->width() - this->width()) / 2), pos.ry() + ((frame->height() - this->height()) / 2));// 移动当前界面位置
    }
}

void myProgressBox::setInfo(QString info)
{
    ui->labInfo->setText(info);
}
