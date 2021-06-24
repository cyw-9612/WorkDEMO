#include "minimeaasgebox.h"
#include "ui_minimeaasgebox.h"

#include <QDebug>
#include <QGraphicsDropShadowEffect>
#include <QPainter>
#include <QMouseEvent>
#include <QtCore/qmath.h>

miniMeaasgeBox::miniMeaasgeBox(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::miniMeaasgeBox)
{
    ui->setupUi(this);

    //initUI();
    setStyle();
    connectSlots();

    //设置窗口为模态,在前
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    setWindowModality(Qt::ApplicationModal);
}

miniMeaasgeBox::~miniMeaasgeBox()
{
    delete ui;
}

//界面初始化
void miniMeaasgeBox::initUI()
{
    ui->lab_title->setText(m_title);
    ui->label->setText(messageMark);
}

//设置风格
void miniMeaasgeBox::setStyle()
{
    //退出风格
    ui->btn_exit->setToolTip(tr("关闭消息框"));
    //设置正常和滑过两种状态
    QString styleExit=("QToolTip{border:1px solid rgb(118, 118, 118); background-color: #ffffff; color:#484848; font-size:12px;}"
                "QPushButton:hover{background-image:url(:/images/btnClose_mini.png);}"
                      "QPushButton{background-image: url(:/images/btnClose_mini_norm.png);}" );
    ui->btn_exit->setStyleSheet(styleExit);

}

//设置空间输入范围
void miniMeaasgeBox::setCtrlRange()
{

}

//关联信号槽
void miniMeaasgeBox::connectSlots()
{
    //关闭按钮
    connect(ui->btn_exit, SIGNAL(clicked()), this, SLOT(slotClose()) );
    //确定取消按钮
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(slotSendSignal()));
}

//槽函数:关闭
void miniMeaasgeBox::slotClose()
{
    //QApplication::exit();
    Show = false;
    this->hide();
}

//发送信号用槽函数
void miniMeaasgeBox::slotSendSignal()
{
    emit messageBoxCheck();
    Show = false;
    this->hide();
}

//鼠标按下事件
void miniMeaasgeBox::mousePressEvent(QMouseEvent *event)
{
    //鼠标左键按下
    if(event->button()==Qt::LeftButton)
    {
        //顶层布局矩形区域
        QRect rectTop=ui->widget->rect();
        //鼠标按下点落在顶层标题区域
        if(rectTop.contains(event->pos()))
        {
            this->m_windowPos = this->pos();           // 目前控件在父窗口中的位置
            this->m_mousePos = event->globalPos();     // 获得鼠标位置
            this->m_dPos = m_mousePos - m_windowPos;       // 移动后部件所在的位置

            m_moveFlag=true;
        }
    }
}

//鼠标移动事件
void miniMeaasgeBox::mouseMoveEvent(QMouseEvent *event)
{
    if(m_moveFlag)
    {
        //拖动窗体
        this->move(event->globalPos() - this->m_dPos);
    }
}

//释放鼠标
void miniMeaasgeBox::mouseReleaseEvent(QMouseEvent *event)
{Q_UNUSED(event)
    m_moveFlag=false;
}

//设置提示窗信息
void miniMeaasgeBox::setWidgetMessage(QString titile, QString message, int colorType)
{
    m_title = titile;
    messageMark = message;

    switch (colorType)
    {
    case Colortype::black:
        ui->label->setStyleSheet("color: rgb(0, 0, 0);\
                                 font: 12pt \"黑体\";\
                                 border-width:0;\
                                 border-style:outset;");
        break;
    default:
        ui->label->setStyleSheet("color: rgb(255, 0, 0);\
                                 font: 12pt \"黑体\";\
                                 border-width:0;\
                                 border-style:outset;");
        break;
    }

    //刷新界面
    initUI();
}
