#ifndef MINIMESSAGEDIALOG_H
#define MINIMESSAGEDIALOG_H

#include <QDialog>
#include "childview.h"
#include "LibControl_global.h"

/***********************************
 *
 * 功 能: 自定义小型消息提示对话框类
 *
 * 编 制: 陈宇葳
 *
 * 日 期: 2022/06/15
 *
 **********************************/

namespace Ui {
class miniMessageDialog;
}

class LIBCONTROL_EXPORT miniMessageDialog : public ChildView
{
    Q_OBJECT

public:
    explicit miniMessageDialog(QWidget *parent = nullptr);
    ~miniMessageDialog();

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
      * @brief 设置窗口信息
      * @param message; //对话框中需要显示的信息
      * @note
      */
     void showMeaasge(QString message);

     /**
      * @brief 居中显示
      * @note
      */
     void MoveToCenter();

     bool m_drag = false;
     QPoint m_mouseStartPoint;//获得鼠标的初始位置
     QPoint m_windowTopLeftPoint;//获得窗口的初始位置

private slots:
    /**取消按钮响应槽函数*/
    void slotCancelClicked();

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

    void on_btn_determine_clicked();

private:
    Ui::miniMessageDialog *ui;
};

#endif // MINIMESSAGEDIALOG_H
