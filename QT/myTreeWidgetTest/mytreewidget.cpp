#include "mytreewidget.h"
#include <QGridLayout>

myTreeWidget::myTreeWidget()
{
    initTreeWidgetItem();

    //连接单击开启列表
    QObject::connect(
        this, SIGNAL(clicked(const QModelIndex &)),
        this, SLOT(expand(const QModelIndex &))
    );
}

void myTreeWidget::initTreeWidgetItem()
{
    //隐藏下拉三角
    this->setRootIsDecorated(false);

    //隐藏表头
    this->setHeaderHidden(true);

    //Item自适应大小
    this->resizeColumnToContents(0);

    QTreeWidgetItem *integralData = new QTreeWidgetItem(this); // 子项 积分数据
    integralData->setText(0,"积分数据");
    //integralData->setIcon();

     QTreeWidgetItem *integralData00 = new QTreeWidgetItem(integralData);
     dataWidget *tem0 = new dataWidget("总值数据");
     this->setItemWidget(integralData00,0,tem0);

     QTreeWidgetItem *integralData01 = new QTreeWidgetItem(integralData);
     dataWidget *tem1 = new dataWidget("统计数据");
     this->setItemWidget(integralData01,0,tem1);

    dataWidget *tem2 = new dataWidget("1/1 OCT");
    QTreeWidgetItem *integralData02 = new QTreeWidgetItem(integralData);
    this->setItemWidget(integralData02,0,tem2);

    dataWidget *tem3 = new dataWidget("1/3 OCT");
    QTreeWidgetItem *integralData03= new QTreeWidgetItem(integralData);
    this->setItemWidget(integralData03,0,tem3);

    dataWidget *tem4 = new dataWidget("FFT 数据");
    QTreeWidgetItem *integralData04= new QTreeWidgetItem(integralData);
    this->setItemWidget(integralData04,0,tem4);

    dataWidget *tem5 = new dataWidget("声暴露计数据");
    QTreeWidgetItem *integralData05= new QTreeWidgetItem(integralData);
    this->setItemWidget(integralData05,0,tem5);

    dataWidget *tem6 = new dataWidget("24小时数据");
    QTreeWidgetItem *integralData06= new QTreeWidgetItem(integralData);
    this->setItemWidget(integralData06,0,tem6);


    QTreeWidgetItem *instantaneousData = new QTreeWidgetItem(this); // 子项 瞬时数据
    instantaneousData->setText(0,"瞬时数据");
    //instantaneousData->setIcon();

    QTreeWidgetItem *instantaneousData0= new QTreeWidgetItem(instantaneousData);
    dataWidget *tem10 = new dataWidget("总值数据");
    this->setItemWidget(instantaneousData0,0,tem10);
    QTreeWidgetItem *instantaneousData1= new QTreeWidgetItem(instantaneousData);
    dataWidget *tem11 = new dataWidget("统计数据");
    this->setItemWidget(instantaneousData1,0,tem11);
    QTreeWidgetItem *instantaneousData2= new QTreeWidgetItem(instantaneousData);
    dataWidget *tem12 = new dataWidget("1/1 OCT");
    this->setItemWidget(instantaneousData2,0,tem12);
    QTreeWidgetItem *instantaneousData3= new QTreeWidgetItem(instantaneousData);
    dataWidget *tem13 = new dataWidget("1/3 OCT");
    this->setItemWidget(instantaneousData3,0,tem13);

    QTreeWidgetItem *soundRecording = new QTreeWidgetItem(this); // 子项 录音
    soundRecording->setText(0,"录音数据");
    QTreeWidgetItem *integralData5 = new QTreeWidgetItem(soundRecording);
    integralData5->setText(0,"录音数据");
    //soundRecording->setIcon();
//    dataWidget *tem20 = new dataWidget("录音");
//    this->setItemWidget(soundRecording,3,tem20);

//    this->setItemWidget()

    QString styleTwo = "QTreeView{\
                border: 1px solid lightgray;\
        }\
        QTreeView::item {\
                height: 40px;\
                border-radius: 2px;\
                border: 1px solid transparent;\
                background: transparent;\
                color: black;\
        }\
        QTreeView::item:has-children {\
                border: none;\
                border-bottom: 1px solid lightgray;\
        }\
        QTreeView::item:hover {\
                border: 1px solid rgb(170, 190, 230);\
                background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,stop: 0 rgb(210, 220, 230), stop: 0.5 rgb(220, 235, 255), stop: 1.0 rgb(210, 230, 255));\
        }\
        QTreeView::item:selected {\
                border: 1px solid rgb(170, 190, 230);\
                background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,stop: 0 rgb(230, 240, 250), stop: 0.5 rgb(220, 235, 255), stop: 1.0 rgb(210, 230, 255));\
        }\
        QTreeView::branch:open:has-children {\
                image: url(./Resources/Images/branchOpen2.png);\
        }\
        QTreeView::branch:closed:has-children {\
                image: url(./Resources/Images/branchClose2.png);\
        }";
    this->setStyleSheet(styleTwo);
//    this->setExpandsOnDoubleClick(true);
    this->expandAll();
    //设置缩进为0
    this->setIndentation(0);

}

void myTreeWidget::setInstrumentType(int type)
{
    m_instrumentType = type;
}

////利用事件过滤器实现导航栏按钮图标的转换
//bool winMain::eventFilter(QObject *obj, QEvent *event)
//{
//    switch (event->type())
//    {
//        case QEvent::HoverEnter:
//            if(obj == ui->btnAuth && false == ui->btnAuth->isChecked())
//                ui->btnAuth->setIcon(QIcon(":/images/images/auth_checked.png"));
//            else if(obj == ui->btnUpgrade && false == ui->btnUpgrade->isChecked())
//                ui->btnUpgrade->setIcon(QIcon(":/images/images/updata_checked.png"));
//            else if(obj == ui->btnAuthCode && false == ui->btnAuthCode->isChecked())
//                ui->btnAuthCode->setIcon(QIcon(":/images/images/authcode_checked.png"));
//            else if(obj == ui->btnPrint && false == ui->btnPrint->isChecked())
//                ui->btnPrint->setIcon(QIcon(":/images/images/print_checked.png"));
//            break;
//        case QEvent::HoverLeave:
//            if(obj == ui->btnAuth)
//                ui->btnAuth->setIcon(QIcon(":/images/images/auth_unchecked.png"));
//            else if(obj == ui->btnUpgrade)
//                ui->btnUpgrade->setIcon(QIcon(":/images/images/updata_unchecked.png"));
//            else if(obj == ui->btnAuthCode)
//                ui->btnAuthCode->setIcon(QIcon(":/images/images/authcode_unchecked.png"));
//            else if(obj == ui->btnPrint)
//                ui->btnPrint->setIcon(QIcon(":/images/images/print_unchecked.png"));
//            break;
//        case QEvent::MouseButtonPress:
//            if(obj == ui->btnAuth)
//                ui->btnAuth->setIcon(QIcon(":/images/images/auth_unchecked.png"));
//            else if(obj == ui->btnUpgrade)
//                ui->btnUpgrade->setIcon(QIcon(":/images/images/updata_unchecked.png"));
//            else if(obj == ui->btnAuthCode)
//                ui->btnAuthCode->setIcon(QIcon(":/images/images/authcode_unchecked.png"));
//            else if(obj == ui->btnPrint)
//                ui->btnPrint->setIcon(QIcon(":/images/images/print_unchecked.png"));
//            break;
//            //    case QEvent::MouseButtonRelease:
//            //        if(obj == ui->btnAuth)
//            //            ui->btnAuth->setIcon(QIcon(":/images/images/auth_checked.png"));
//            //        else if(obj == ui->btnUpgrade)
//            //            ui->btnUpgrade->setIcon(QIcon(":/images/images/updata_checked.png"));
//            //        else if(obj == ui->btnAuthCode)
//            //            ui->btnAuthCode->setIcon(QIcon(":/images/images/authcode_checked.png"));
//            //        else if(obj == ui->btnPrint)
//            //            ui->btnPrint->setIcon(QIcon(":/images/images/print_checked.png"));
//            //        break;
//        default:
//            break;
//    }
//    return QWidget::eventFilter(obj, event);
//}
