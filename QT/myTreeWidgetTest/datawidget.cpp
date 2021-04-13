#include "datawidget.h"
#include <QLayout>

dataWidget::dataWidget(QString dataName)
{
    m_dateName = dataName;
    QLabel *labDateName = new QLabel;
    QLabel *labDateCount = new QLabel;
    labDateName->setText(m_dateName);
    labDateName->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    labDateCount->setText(QString("(%1)").arg(m_dataCount));
    labDateCount->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    labDateCount->setStyleSheet("color: rgb(255, 0, 0);");

    //设置布局
    QHBoxLayout *horizontalLayout = new QHBoxLayout;
    horizontalLayout->setContentsMargins(20,0,0,0);
    horizontalLayout->addWidget(labDateName,0);
    horizontalLayout->addWidget(labDateCount,0);


    this->setStyleSheet("background: transparent;");
    this->setLayout(horizontalLayout);
}


//设置数据的数量
void dataWidget::setDateCount(int count)
{
    m_dataCount = count;
}

//获取数据类的名称
QString dataWidget::getDataName()
{
    return m_dateName;
}
