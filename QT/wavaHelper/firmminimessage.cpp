#include "firmminimessage.h"
#include "ui_firmminimessage.h"

#include <QDebug>
#include <QGraphicsDropShadowEffect>
#include <QPainter>
#include <QtCore/qmath.h>

firmMiniMessage::firmMiniMessage(QString titile,QString message, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::firmMiniMessage)
{
    ui->setupUi(this);

    m_title = titile;
    messageMark = message;
    initUI();
    setStyle();
    connectSlots();

//    //绘制边框
//    setAttribute(Qt::WA_TranslucentBackground);
//    const int nMargin = 3;//和边距一样的距离
//    QGraphicsDropShadowEffect *shadow_effect = new QGraphicsDropShadowEffect(this);
//    shadow_effect->setOffset(0, 0);
//    shadow_effect->setColor(Qt::black);
//    shadow_effect->setBlurRadius(nMargin);
//    this->setGraphicsEffect(shadow_effect);//一般不用this,用最外层的Frame

    //设置窗口为模态
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);  //且无边框，保持最前
    setWindowModality(Qt::WindowModal);  //修改其为模态窗口

    Show = true;
}

firmMiniMessage::~firmMiniMessage()
{
    delete ui;
}

//界面初始化
void firmMiniMessage::initUI()
{
    ui->lab_title->setText(m_title);
    ui->label->setText(messageMark);
}

//设置风格
void firmMiniMessage::setStyle()
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
void firmMiniMessage::setCtrlRange()
{

}

bool firmMiniMessage::isShow()
{
    return Show;
}
//关联信号槽
void firmMiniMessage::connectSlots()
{
    //关闭按钮
    connect(ui->btn_exit, SIGNAL(clicked()), this, SLOT(slotClose()) );
    //确定取消按钮
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(slotSendSignal()));
}

//槽函数:关闭
void firmMiniMessage::slotClose()
{
    //QApplication::exit();
    emit messageBoxClose();
    emit messageBoxCloseX(true);
    Show = false;
    this->hide();
}

//发送信号用槽函数
void firmMiniMessage::slotSendSignal()
{
    emit messageBoxCheck();
    emit messageBoxClose();
    emit messageBoxCloseX(true);
    Show = false;
    this->hide();
}

//复写paintEvent,绘制窗口边框
void firmMiniMessage::paintEvent(QPaintEvent *event)
{
    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    path.addRect(1, 1, this->width()-2, this->height()-2);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.fillPath(path, QBrush(Qt::white));

    QColor color(0, 0, 0, 50);
    for(int i=0; i<1; i++)
    {
        QPainterPath path;
        path.setFillRule(Qt::WindingFill);
        path.addRect(2-i, 2-i, this->width()-(2-i)*2, this->height()-(2-i)*2);
        color.setAlpha(150 - qSqrt(i)*50);
        painter.setPen(color);
        painter.drawPath(path);
    }
}

//设置提示窗信息
void firmMiniMessage::setWidgetMessage(QString titile, QString message, int colorType)
{
    m_title = titile;
    messageMark = message;

    switch (colorType)
    {
    case QColortype::black:
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
