#include "minimessagedialog.h"
#include "ui_minimessagedialog.h"
#include <QMouseEvent>

miniMessageDialog::miniMessageDialog(QWidget *parent) :
    ChildView(parent),
    ui(new Ui::miniMessageDialog)
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

miniMessageDialog::~miniMessageDialog()
{
    delete ui;
}

//设置风格
void miniMessageDialog::setStyle()
{
    //退出风格
    ui->btn_close->setToolTip(tr("关闭消息框"));
}

//关联信号槽
void miniMessageDialog::connectSlots()
{
    //连接按钮信号
    connect(ui->btn_close, SIGNAL(clicked()), this, SLOT(slotCancelClicked()));
}

//初始化界面
void miniMessageDialog::initUI()
{
    //初始化鼠标移动标志位
    m_drag = false;

}

//置控件输入范围
void miniMessageDialog::setCtrlRange()
{

}

//设置窗口信息
void miniMessageDialog::showMeaasge(QString message)
{
    ui->label->setText(message);
    MoveToCenter();
    this->show();
}

//居中显示
void miniMessageDialog::MoveToCenter()
{
    if(this->parent() != nullptr)
    {
        QWidget *frame =  static_cast<QWidget *>(this->parent());// 获取主界面
        QPoint pos = frame->mapToGlobal(QPoint(0, 0));// 客户区坐标转屏幕坐标
        this->move(pos.rx() + ((frame->width() - this->width()) / 2), pos.ry() + ((frame->height() - this->height()) / 2));// 移动当前界面位置
    }
}

/**取消按钮响应槽函数*/
void miniMessageDialog::slotCancelClicked()
{
    //取消按钮总是取消当前操作直接隐藏当前窗口即可
    this->hide();
}

//实现移动标题栏功能
void miniMessageDialog::mousePressEvent(QMouseEvent *event)
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
void miniMessageDialog::mouseMoveEvent(QMouseEvent *event)
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
void miniMessageDialog::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        m_drag = false;
    }
}

void miniMessageDialog::on_btn_determine_clicked()
{
    this->hide();
}
