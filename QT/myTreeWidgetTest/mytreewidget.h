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
public:
    myTreeWidget();

    /**
     * @brief  设置数据导航栏的类型
     * @param  type: 仪器型号
     * @return 无
     */
    void setInstrumentType(int type);

////事件过滤
//protected:
//    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    void initTreeWidgetItem();	// 初始化树列表

private:
    //接入仪器类型
    int m_instrumentType;

};

#endif // MYTREEWIDGET_H
