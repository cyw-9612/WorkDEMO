#include "mytreewidget.h"
#include <QGridLayout>
#include <QDebug>

myTreeWidget::myTreeWidget(QWidget *parent)
{
    initTreeWidgetItem();

    //连接单击开启列表
    QObject::connect(
        this, SIGNAL(clicked(const QModelIndex &)),
        this, SLOT(expand(const QModelIndex &))
    );

    QObject::connect(
        this, SIGNAL(itemClicked(QTreeWidgetItem*,int)),
        this, SLOT(slotClicked(QTreeWidgetItem*,int))
    );

}

void myTreeWidget::initTreeWidgetItem()
{
    //隐藏下拉三角
    this->setRootIsDecorated(false);

    //隐藏表头
    this->setHeaderHidden(true);

    //禁用双击
    this->setExpandsOnDoubleClick(false);

    //Item自适应大小
    this->resizeColumnToContents(0);

    QTreeWidgetItem *integralData = new QTreeWidgetItem(this); // 子项 积分数据
    integralData->setText(0,"积分数据");
    //integralData->setIcon();
    QStringList dataName;
    dataName << "总值数据" << "统计数据" << "1/1 OCT" << "1/3 OCT" << "FFT 数据" << "声暴露计数据" << "24小时数据";
    for (int i = 0; i < dataName.count(); i++)
    {
        QTreeWidgetItem *integralDataChild = new QTreeWidgetItem(integralData);
        dataWidget *tem = new dataWidget(dataName[i]);
        this->setItemWidget(integralDataChild,0,tem);
    }
    dataName.clear();

    QTreeWidgetItem *instantaneousData = new QTreeWidgetItem(this); // 子项 瞬时数据
    instantaneousData->setText(0,"瞬时数据");
    //instantaneousData->setIcon();

    dataName << "总值数据" << "统计数据" << "1/1 OCT" << "1/3 OCT" ;
    for (int i = 0; i < dataName.count(); i++)
    {
        QTreeWidgetItem *instantaneousDataChild = new QTreeWidgetItem(instantaneousData);
        dataWidget *tem = new dataWidget(dataName[i]);
        this->setItemWidget(instantaneousDataChild,0,tem);
    }
    dataName.clear();

    QTreeWidgetItem *soundRecording = new QTreeWidgetItem(this); // 子项 录音
    soundRecording->setText(0,"录音数据");
    //soundRecording->setIcon();
    QTreeWidgetItem *soundRecording0= new QTreeWidgetItem(soundRecording);
    dataWidget *tem20 = new dataWidget("录音");
    this->setItemWidget(soundRecording0,0,tem20);

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

    //展开所有项
    this->expandAll();
    //设置缩进为0
    this->setIndentation(0);

}

void myTreeWidget::setInstrumentType(int type)
{
    m_instrumentType = type;
}

void myTreeWidget::slotClicked(QTreeWidgetItem* item,int count)
{
    QTreeWidgetItem* parentItem = item->parent();
    if(parentItem)
    {
        QString parentsName = parentItem->text(0);
        QWidget *itemwidget = itemWidget(item, count);
        dataWidget *dataWidgets = (dataWidget *)itemwidget;
        QString currentItem = dataWidgets->getDataName();
        qDebug() << "点击的是：" << parentsName << "->" << currentItem;
        emit sigTreeWidgetChoseMsg(parentsName,currentItem);
    }
    else
    {
        qDebug() << "点击的是根节点" << item->text(0);
    }

}
