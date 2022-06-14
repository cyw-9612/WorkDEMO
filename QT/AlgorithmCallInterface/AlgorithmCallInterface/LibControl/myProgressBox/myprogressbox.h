#ifndef MYPROGRESSBOX_H
#define MYPROGRESSBOX_H

#include <QDialog>
#include "childview.h"
#include "LibControl_global.h"
#include "picprogress.h"
#include "myMessageDialog/mymessagedialog.h"

/***********************************
 *
 * 功 能: 自定义进度条框类
 *
 * 编 制: 陈宇葳
 *
 * 日 期: 2021/06/07
 *
 **********************************/

//数据类型
enum enum_progressStyle
{
    type_readStyle=0,  //读取数据进度
    type_connectStyle=1,  //设备连接进度
    type_analyStyle=2,     //数据解析进度
};

namespace Ui {
class myProgressBox;
}

class LIBCONTROL_EXPORT myProgressBox : public ChildView
{
    Q_OBJECT

public:
    explicit myProgressBox(QWidget *parent = nullptr);
    ~myProgressBox();

    /**
     * @brief 设置风格
     * @note
     */
    void setStyle();

    /**
     * @brief 关联信号槽
     * @note
     */
    void connectSlots();

    /**
     * @brief 初始化界面
     * @note
     */
    void initUI();

    /**
     * @brief 设置控件输入范围
     * @note
     */
     void setCtrlRange();


    /**
     * @brief 设置进度条相关信息
     * @param piceCount ;  //总共的数据数量
     * @note
     */
    void setDataCount(int piceCount);

    /**
     * @brief 设置进度条窗口风格
     * @param dialogStyle ;  //风格编号
     * @note
     */
    void setDialogStyle(int dialogStyle);

    /**
     * @brief 居中显示
     * @note
     */
    void MoveToCenter();

    bool m_drag = false;
    QPoint m_mouseStartPoint;//获得鼠标的初始位置
    QPoint m_windowTopLeftPoint;//获得窗口的初始位置

    void setInfo(QString info);
signals:
    //取消读取信号
    void cancelRead(const bool& );

     //取消连接信号
     void cancelConnect(const bool& );

    //取消文件获取信号
     void cancelReadFile(const bool& );
private slots:
     /**
      * @brief 只更新进度条槽函数
      * @param dataNamel; //当前处理数据编号
      * @note
      */
     void  slotUpdataProgress(int value);

    /**
     * @brief 进度条更新槽函数
     * @param value; //以及读取的数据数量
     * @param dataNamel; //当前处理数据编号
     * @note
     */
    void  slotUpdataProgress(int value, QString dataName);

    /**
     * @brief 用户取消读取操作
     * @note
     */
    void  slotCancelRead();

    /**
     * @brief 取消读取
     * @note
     */
    void  slotCancelRead(const bool& isCancel);

private:
    /**
     * @brief 设置中间标签是否显示
     * @param isVisable; //标签是否显示
     * @note
     */
    void  setLabIsVisable(bool isVisable);

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    Ui::myProgressBox *ui;

    //消息对话框
    myMessageDialog *m_messageDialog = nullptr;

    //最大计数
    int m_maxCount = -1;

    //窗口信息类型
    int m_progressStyle = -1;
};

#endif // MYPROGRESSBOX_H
