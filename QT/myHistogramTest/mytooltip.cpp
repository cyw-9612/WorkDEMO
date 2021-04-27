#include "mytooltip.h"
#include "ui_mytooltip.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QEvent>
#include <QTimerEvent>
#include <QResizeEvent>
//#include <QStyle>
#include <QHBoxLayout>
#include <QDebug>

myToolTip::myToolTip(const QList<QColor> colorList, const QList<QString>dataNameList)
    : QDialog(nullptr),
    ui(new Ui::myToolTip)
{
    ui->setupUi(this);
    m_colorList = colorList;
    m_dataNameList = dataNameList;

    IniUI();

    setConnect();
}

myToolTip::~myToolTip()
{
    clearUI();

    delete ui;
}

void myToolTip::IniUI()
{
    for (int x = 0 ; x < m_dataNameList.count() ; x++)
    {
        if(m_dataList.count() < m_dataNameList.count())
        {
            //初始化数据列表
            m_dataList << 0.0;
        }

        //设置标签的颜色
        QString color = QString("background-color: rgb(%1, %2, %3);")
                .arg(m_colorList[x].red())
                .arg(m_colorList[x].green())
                .arg(m_colorList[x].blue());

        QLabel *lab_color = new QLabel;
        QLabel *lab_dataName = new QLabel(m_dataNameList[x]);
        QLabel *lab_data = new QLabel(QString("%1dB").arg(m_dataList[x]));
        lab_color->setMaximumSize(8,8);
        lab_color->setMinimumSize(8,8);
        lab_color->setStyleSheet(color);
        lab_dataName->resize(37,12);
        lab_data->resize(42,12);
        //水平弹簧
        QSpacerItem* hSpacerLeft= new QSpacerItem(7, 7, QSizePolicy::Fixed, QSizePolicy::Fixed);
        QSpacerItem* hSpacerRight= new QSpacerItem(14, 7, QSizePolicy::Fixed, QSizePolicy::Fixed);
        //向布局中添加对象
        QGridLayout* topLayout=new QGridLayout;
        topLayout->setContentsMargins(0,0,0,0);
        topLayout->addWidget(lab_color,0,0,1,1);
        topLayout->addItem(hSpacerLeft,0,1,1,1);
        topLayout->addWidget(lab_dataName,0,2,1,1);
        topLayout->addItem(hSpacerRight,0,3,1,1);
        topLayout->addWidget(lab_data,0,4,1,1);

        //设置各行比例系数
        topLayout->setRowStretch(0,8);
        topLayout->setRowStretch(1,7);
        topLayout->setRowStretch(2,37);
        topLayout->setRowStretch(3,14);
        topLayout->setRowStretch(4,42);

        QWidget *widget = new QWidget();
        widget->setObjectName(m_dataNameList[x]);
        widget->setLayout(topLayout);

        ui->layout_data->addWidget(widget);
    }
}

void myToolTip::setConnect()
{

}

int myToolTip::getRightOffset()
{
    return rightOffset;
}

void myToolTip::setRightOffset(int offset)
{
    if(rightOffset!=offset){
        rightOffset=offset;
        //没有动态样式的处理，可自行添加
        //style()->unpolish(this);
        //style()->polish(this);
    }
}

int myToolTip::getBottomOffset()
{
    return bottomOffset;
}

void myToolTip::setBottomOffset(int offset)
{
    if(bottomOffset!=offset){
        bottomOffset=offset;
    }
}

QList<QColor> myToolTip::getColor() const
{
    return m_colorList;
}

void myToolTip::setColor(QList<QColor> colorList)
{
    if(m_colorList != colorList)
    {
        m_colorList = colorList;
        flashDialog();
    }
}
QList<QString> myToolTip::getTextList() const
{
    return m_dataNameList;
}

void myToolTip::setTextList(QList<QString> textList,QList<float> dataList)
{
    if(m_dataNameList != textList || m_dataList != dataList)
    {
        m_dataNameList = textList;
        m_dataList = dataList;
        flashDialog();
    }
}

QList<float> myToolTip::getData() const
{
    return m_dataList;
}

void myToolTip::setData(QList<float> dataList)
{
    if(m_dataList != dataList)
    {
        m_dataList = dataList;
        flashDialog();
    }
}

void myToolTip::anchorTarget(QWidget *target)
{
    if(target&&target!=targetWidget){
        if(targetWidget){
            targetWidget->removeEventFilter(this);
        }
        targetWidget=target;
        targetWidget->installEventFilter(this);
        targetWidget->setMouseTracking(true);
        //如果是随窗口关闭的，看不到析构的打印，难道此时事件循环已停止？
        connect(targetWidget,&QObject::destroyed,this,&QObject::deleteLater);
    }
}

void myToolTip::showTip(const QWidget *obj)
{
    if(!obj)
        return;
    showTip(cursor().pos());
    //showTip(QPoint(cursor().pos().x(),obj->mapToGlobal(QPoint(300, 20)).y()));
    //showTip(obj->mapToGlobal(QPoint(300, 20)));
}

void myToolTip::showTip(const QPoint &rightBottom)
{
    targetPoint=rightBottom;
    //move(rightBottom.x() - width() + rightOffset,
    //     rightBottom.y() - height() + bottomOffset);
    //直接用size+point得到的位置可能显示不全，这里计算下

    int rect_left=rightBottom.x()- width()+rightOffset;
    int rect_top=rightBottom.y()-height()+bottomOffset;
    if(rect_left<0)
        rect_left=0;
    if(rect_top<0)
        rect_top=0;
    //要考虑多个显示器情况，【待测试】
    //根据当前所在屏幕尺寸计算
    QDesktopWidget * desktop=QApplication::desktop();
    if(desktop){
        QRect desk_rect=desktop->screenGeometry(targetWidget?targetWidget:this);
        if(rect_left+width()>desk_rect.width())
            rect_left=desk_rect.width()-width();
        if(rect_top+height()>desk_rect.height())
            rect_top=desk_rect.height()-height();
    }

    move(rect_left,rect_top);
    if(!showTimer.isActive())
        showTimer.start(200,this);
}

void myToolTip::hideTip()
{
    if(!hideTimer.isActive())
        hideTimer.start(300,this);
}

bool myToolTip::eventFilter(QObject *target, QEvent *event)
{
    if(target==targetWidget){
        switch (event->type()) {
        case QEvent::Enter:
            //showTip(QCursor::pos());
            showTip(targetWidget);
            break;
        case QEvent::Leave:
            hideTip();
            break;
        default:
            break;
        }
    }
    return QWidget::eventFilter(target,event);
}

void myToolTip::timerEvent(QTimerEvent *event)
{
    if(event->timerId()==showTimer.timerId()) {
        showTimer.stop();
        //hideTimer.stop();
        if(!hideTimer.isActive()&&isHidden()){
            show();
        }
    }else if(event->timerId()==hideTimer.timerId()){
        showTimer.stop();
        hideTimer.stop();
        if(!isHidden()){
            hide();
        }
    }else{
        QWidget::timerEvent(event);
    }
}

void myToolTip::resizeEvent(QResizeEvent *event)
{
    //初次show的时候可能size可能还没计算好
    showTip(targetPoint);
    QWidget::resizeEvent(event);
}

void myToolTip::clearUI()
{
    //清空horizontalLayout布局内的所有元素
    QLayoutItem *child;
     while ((child = ui->layout_data->takeAt(0)) != 0)
     {
            //setParent为NULL，防止删除之后界面不消失
            if(child->widget())
            {
                child->widget()->setParent(NULL);
            }

            delete child;
     }
}

void myToolTip::flashDialog()
{
    //清空原有数据
    clearUI();
    //重置UI界面
    IniUI();
    //移动窗口
    showTip(targetWidget);
}
