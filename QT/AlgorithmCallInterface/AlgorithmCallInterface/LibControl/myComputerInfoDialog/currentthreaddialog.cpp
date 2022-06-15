#include "currentthreaddialog.h"
#include "ui_currentthreaddialog.h"
#include <QMouseEvent>
#include <QDebug>
#include "SysInfo/sysinfo.h"

currentThreadDialog::currentThreadDialog(QWidget *parent) :
    ChildView(parent),
    ui(new Ui::currentThreadDialog)
{
    ui->setupUi(this);

    //且无边框，保持最前
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::FramelessWindowHint);

    initUI();

    setStyle();

    setCtrlRange();

    connectSlots();

    startTimer(1000);

    this->hide();
}

currentThreadDialog::~currentThreadDialog()
{
    delete ui;
}

//设置风格
void currentThreadDialog::setStyle()
{
    //退出风格
    ui->btn_close->setToolTip(tr("关闭消息框"));
}

//关联信号槽
void currentThreadDialog::connectSlots()
{
    //连接按钮信号
    connect(ui->btn_close, SIGNAL(clicked()), this, SLOT(slotCancelClicked()));
}

//初始化界面
void currentThreadDialog::initUI()
{
    //初始化鼠标移动标志位
    m_drag = false;

    m_thread = new threadCheck;

    QStringList headList;
    headList << "进程ID" << "     进程名     " << "父类进程ID" << "进程句柄" << "优先级";
    ui->tableWidget->setColumnCount(5);
    ui->tableWidget->setHorizontalHeaderLabels(headList);
//    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->setColumnWidth(1, 150);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);


    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setShowGrid(false);
}

//置控件输入范围
void currentThreadDialog::setCtrlRange()
{

}

/**取消按钮响应槽函数*/
void currentThreadDialog::slotCancelClicked()
{
    this->hide();
}

void currentThreadDialog::addNew()
{
    QVector<ProcessInfo> vec = m_thread->getVec();


    for(ProcessInfo info : vec){

        bool have = false;
        for(int i = 0; i < ui->tableWidget->rowCount(); i++){

            if(ui->tableWidget->item(i, 0)->text() == info.processID){

               have = true;
            }
        }
        if(!have){

            ui->tableWidget->insertRow(0);
            ui->tableWidget->setItem(0, 0, new QTableWidgetItem(info.processID));
            ui->tableWidget->setItem(0, 1, new QTableWidgetItem(info.processName));
            ui->tableWidget->setItem(0, 2, new QTableWidgetItem(info.parentProcessID));
            ui->tableWidget->setItem(0, 3, new QTableWidgetItem(info.processHandle));
            ui->tableWidget->setItem(0, 4, new QTableWidgetItem(info.processPri));
        }
    }
}

void currentThreadDialog::removeOld()
{
    QVector<ProcessInfo> vec = m_thread->getVec();


    for(int i = 0; i < ui->tableWidget->rowCount(); i++){

        bool have = false;
        for(ProcessInfo info : vec){

            if(info.processID == ui->tableWidget->item(i, 0)->text()){

                have = true;
                continue;
            }
        }

        if(!have){

            ui->tableWidget->removeRow(i);
        }
    }
}

void currentThreadDialog::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event)
    m_thread->clearVec();
    m_thread->getSnapshot();
    addNew();
    removeOld();
}

// 居中显示
void currentThreadDialog::MoveToCenter()
{
    if(this->parent() != nullptr)
    {
        QWidget *frame =  static_cast<QWidget *>(this->parent());// 获取主界面
        QPoint pos = frame->mapToGlobal(QPoint(0, 0));// 客户区坐标转屏幕坐标
        this->move(pos.rx() + ((frame->width() - this->width()) / 2), pos.ry() + ((frame->height() - this->height()) / 2));// 移动当前界面位置
    }
}

//实现移动标题栏功能
void currentThreadDialog::mousePressEvent(QMouseEvent *event)
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
void currentThreadDialog::mouseMoveEvent(QMouseEvent *event)
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
void currentThreadDialog::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        m_drag = false;
    }
}
