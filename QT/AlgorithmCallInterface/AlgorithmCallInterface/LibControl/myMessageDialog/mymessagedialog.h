#ifndef MYMESSAGEDIALOG_H
#define MYMESSAGEDIALOG_H

#include <QDialog>
#include "childview.h"
#include "LibControl_global.h"

/***********************************
 *
 * 功 能: 自定义消息提示对话框类
 *
 * 编 制: 陈宇葳
 *
 * 日 期: 2021/06/08
 *
 **********************************/

//窗口风格类型
enum enum_dialogStyle
{
    type_redIconTwoBtn=0,  //警告标签两个按钮
    type_redIconOneBtn=1,  //警告标签一个按钮
    type_blueIconOneBtn=2,  //完成标签一个按钮
};

//窗口信息类型
enum enum_messageStyle
{
    type_readMessage=0,  //取消去读取窗口
    type_changeEquipment=1,  //切换仪器窗口
    type_cancelConnect=2,  //取消连接窗口
    type_other=3, //其他一般窗口
};

namespace Ui {
class myMessageDialog;
}

class LIBCONTROL_EXPORT myMessageDialog : public ChildView
{
    Q_OBJECT

public:
    explicit myMessageDialog(QWidget *parent = nullptr);
    ~myMessageDialog();

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
      * @brief 设置进度条窗口风格
      * @param dialogStyle ;  //风格编号
      * @param messageStyle; //信息类型
      * @param title; //对话框的标题
      * @param message; //对话框中需要显示的信息
      * @note
      */
     void setDialogStyle(int dialogStyle,int messageStyle, QString title, QString message);

     /**
      * @brief 居中显示
      * @note
      */
     void MoveToCenter();

     bool m_drag = false;
     QPoint m_mouseStartPoint;//获得鼠标的初始位置
     QPoint m_windowTopLeftPoint;//获得窗口的初始位置

signals:
     /**取消读取信号*/
     void sigCancelRead(const bool& );

    /**切换设备信号*/
    void sigChangeEquipment(const bool& );

    /**取消连接信号*/
    void sigCancelConnect(const bool& );

    /**一般模式下的按钮信号*/
    void sigCancelBtnCheck(const bool& );

private slots:
    /**取消按钮响应槽函数*/
    void slotCancelClicked();

    /**确认按钮响应槽函数*/
    void slotComfirmClicked();

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    Ui::myMessageDialog *ui;

    //窗口初始化标志位
    bool isIni = true;

    /**消息框确定按钮*/
    QPushButton * btn_comfirm;

    /**息框取消对话框*/
    QPushButton * btn_cancel;

    /**串口布局类型*/
    int m_dialogStyle = -1;

    /**当前窗口类型*/
    int m_messageStyle = -1;

    //红色标签
    QString edit_redStyle = "background-color: rgb(255, 255, 255);\
            color: rgb(255, 0, 0);\
            font: 12pt \"黑体\";\
            border-width:0;\
            border-style:outset;" ;

    //黑色标签
    QString edit_blackStyle = "background-color: rgb(255, 255, 255);\
            color: rgb(0, 0, 0);\
            font: 12pt \"黑体\";\
            border-width:0;\
            border-style:outset;" ;

};

#endif // MYMESSAGEDIALOG_H
