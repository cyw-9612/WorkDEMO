#ifndef MYTREEWIDGET_H
#define MYTREEWIDGET_H
#include <QTreeWidget>
#include "datawidget.h"

/***********************************
 *
 * 功 能: 自定义数据导航栏
 *
 * 编 制: 陈宇葳
 *
 * 日 期: 2020/04/12
 *
 **********************************/

//消息框类型
enum enum_Type
{
    type_AWA6292=0,                      //0:AWA6292
    type_AWA5912=1,                       //1:AWA5912
    type_AWA5662=2,                      //2:AWA5662
    type_AWA5920=3,                      // 3:AWA5920
};

class myTreeWidget : public QTreeWidget
{
    Q_OBJECT

public:
    //myTreeWidget();
    explicit myTreeWidget(QWidget *parent = 0);

    /**
     * @brief  设置数据导航栏的类型
     * @param  type: 仪器型号
     * @return 无
     */
    void setInstrumentType(int type);

signals:
    //选择数据按钮信号
    void sigTreeWidgetChoseMsg(QString title, QString msg);

private slots:
    //相应点击的项
    void slotClicked(QTreeWidgetItem* item,int count);


private:
    void initTreeWidgetItem();	// 初始化树列表

private:
    //接入仪器类型
    int m_instrumentType;

};

#endif // MYTREEWIDGET_H
