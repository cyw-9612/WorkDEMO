#include "mymessagedialog.h"
#include "ui_mymessagedialog.h"
#include <QMouseEvent>
#include <QDebug>

myMessageDialog::myMessageDialog(QWidget *parent) :
    ChildView(parent),
    ui(new Ui::myMessageDialog)
{
    ui->setupUi(this);

    initUI();

    setStyle();

    setCtrlRange();

    connectSlots();

    //且无边框，保持最前
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    this->hide();
}

myMessageDialog::~myMessageDialog()
{
    delete ui;
}

//设置风格
void myMessageDialog::setStyle()
{
    //退出风格
    ui->btn_close->setToolTip(tr("关闭消息框"));

    //设置按钮风格
    QString cancalStyle = "QPushButton{ font: bold large 'SourceHanSansK-Regular';font-size:14px;\
            border: 0px solid;\
            background-color:rgba(255, 30, 30, 1);\
            color:rgba(255, 255, 255,1);\
            border-radius: 2px;\
            }\
            QPushButton:disabled{\
            background-color:#DCDCDC;\
            border-radius:2px;\
            color:#FFFFFF;    \
            }\
            QPushButton:hover{\
            border: 0px solid;\
            background-color:rgba(255, 87, 87,1);\
            color:rgba(255, 255, 255,1);\
            border-radius:2px;\
            opacity:0.2;\
            }\
            QPushButton:pressed{\
            background-color:rgba(56, 60, 73,1);\
            color:rgba(255, 255, 255,1);\
            border-radius:2px;\
            }";

    QString comfirmlStyle = "QPushButton{ font: bold large /'SourceHanSansK-Regular/';font-size:14px;\
            border: 0px solid;\
            background-color:rgba(88, 164, 176,1);\
            color:rgba(255, 255, 255,1);\
            border-radius: 2px;\
            }\
            QPushButton:disabled{\
            background-color:#DCDCDC;\
            border-radius:2px;\
            color:#FFFFFF;    \
            }\
            QPushButton:hover{\
            border: 0px solid;\
            background-color:rgba(116, 208, 222,1);\
            color:rgba(255, 255, 255,1);\
            border-radius:2px;\
            opacity:0.2;\
            }\
            QPushButton:pressed{\
            background-color:rgba(58, 108, 116,1);\
            color:rgba(255, 255, 255,1);\
            border-radius:2px;\
            }";

    btn_cancel->setStyleSheet(cancalStyle);
    btn_comfirm->setStyleSheet(comfirmlStyle);

}

//关联信号槽
void myMessageDialog::connectSlots()
{
    //连接按钮信号
    connect(ui->btn_close, SIGNAL(clicked()), this, SLOT(slotCancelClicked()));
    connect(btn_cancel, SIGNAL(clicked()), this, SLOT(slotCancelClicked()));
    connect(btn_comfirm, SIGNAL(clicked()), this, SLOT(slotComfirmClicked()));
}

//初始化界面
void myMessageDialog::initUI()
{
    //初始化鼠标移动标志位
    m_drag = false;

    //初始化按钮
    btn_cancel = new QPushButton;
    btn_comfirm = new QPushButton;

    btn_cancel->setText(tr("取消"));
    btn_cancel->setFixedSize(100, 35);
    btn_comfirm->setText(tr("确定"));
    btn_comfirm->setFixedSize(100, 35);

}

//置控件输入范围
void myMessageDialog::setCtrlRange()
{

}

//设置进度条窗口风格
void myMessageDialog::setDialogStyle(int dialogStyle,int messageStyle, QString title, QString message)
{
    //设置信息标签文字
    ui->edit_infor->setText(message);
    ui->lab_title->setText(title);

    //记录信息类型
    if(m_messageStyle == messageStyle && dialogStyle == m_dialogStyle)
    {
        //如果类型一致则不再切换风格
        return;
    }

    //记录对话框风格
    m_messageStyle = messageStyle;

    //记录对话框布局风格
    m_dialogStyle = dialogStyle;

    //清空原有布局
    delete ui->widget_btn->layout();

    //水平弹簧
    QSpacerItem* hSpacerCenter= new QSpacerItem(30, 30, QSizePolicy::Fixed, QSizePolicy::Fixed);

    //按钮栏布局
    QHBoxLayout * btnLayout = new QHBoxLayout;
    btnLayout->setContentsMargins(0,0,0,0);

    //标签图像类
    QImage Image;

    //三种情况 蓝色标签一个按钮用default定义
    switch (dialogStyle)
    {
        case enum_dialogStyle::type_redIconOneBtn :
        btn_cancel->setVisible(false);
        Image.load(":/images/exclamatory_mark.png");
        btnLayout->addWidget(btn_comfirm);
        ui->edit_infor->setStyleSheet(edit_redStyle);
        break;
    case enum_dialogStyle::type_redIconTwoBtn :
        Image.load(":/images/exclamatory_mark.png");
        if(!isIni)
            btn_cancel->setVisible(true);
        else
            isIni = false;

        btnLayout->addWidget(btn_cancel);
        btnLayout->addItem(hSpacerCenter);
        btnLayout->addWidget(btn_comfirm);
        ui->edit_infor->setStyleSheet(edit_redStyle);
        break;
    default:
        Image.load(":/images/finish.png");
        btn_cancel->setVisible(false);
        btnLayout->addWidget(btn_comfirm);
        ui->edit_infor->setStyleSheet(edit_blackStyle);
        break;
    }

    //设置标签类型
    QPixmap pixmap = QPixmap::fromImage(Image);
    int width = pixmap.width();
    int height = pixmap.height();
    ui->lab_icon->setFixedSize(width, height);
    ui->lab_icon->setPixmap(pixmap);
    //应用布局
     ui->widget_btn->setLayout(btnLayout);

     //MoveToCenter();
}

/**取消按钮响应槽函数*/
void myMessageDialog::slotCancelClicked()
{
    //取消按钮总是取消当前操作直接隐藏当前窗口即可
    this->hide();
}

/**确认按钮响应槽函数*/
void myMessageDialog::slotComfirmClicked()
{
    switch (m_messageStyle)
    {
        case enum_messageStyle::type_readMessage :
            emit sigCancelRead(true);
            this->hide();
        break;
        case enum_messageStyle::type_changeEquipment :
            emit sigChangeEquipment(true);
            this->hide();
        break;
        case enum_messageStyle::type_cancelConnect :
            emit sigCancelConnect(true);
            this->hide();
        break;
        default:
            if(m_dialogStyle == enum_dialogStyle::type_redIconTwoBtn)
            {
                emit sigCancelBtnCheck(true);
            }
            this->hide();
        break;
    }
}

//实现移动标题栏功能
void myMessageDialog::mousePressEvent(QMouseEvent *event)
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
void myMessageDialog::mouseMoveEvent(QMouseEvent *event)
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
void myMessageDialog::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        m_drag = false;
    }
}

// 居中显示
void myMessageDialog::MoveToCenter()
{
    if(this->parent() != nullptr)
    {
        QWidget *frame =  static_cast<QWidget *>(this->parent());// 获取主界面
        QPoint pos = frame->mapToGlobal(QPoint(0, 0));// 客户区坐标转屏幕坐标
        this->move(pos.rx() + ((frame->width() - this->width()) / 2), pos.ry() + ((frame->height() - this->height()) / 2));// 移动当前界面位置
    }
}
