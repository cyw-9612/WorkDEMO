#ifndef CURRENTTHREADDIALOG_H
#define CURRENTTHREADDIALOG_H

#include <QDialog>
#include "childview.h"
#include "LibControl_global.h"
#include "threadcheck.h"
#include <QVector>

/***********************************
 *
 * 功 能: 显示当前电脑进程对话框
 *
 * 编 制: 陈宇葳
 *
 * 日 期: 2021/06/15
 *
 **********************************/

namespace Ui {
class currentThreadDialog;
}

class LIBCONTROL_EXPORT currentThreadDialog : public ChildView
{
    Q_OBJECT

public:
    explicit currentThreadDialog(QWidget *parent = nullptr);
    ~currentThreadDialog();

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

protected:
    void timerEvent(QTimerEvent *event);
    void addNew();
    void removeOld();

private:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    Ui::currentThreadDialog *ui;
    threadCheck *m_thread;
};

#endif // CURRENTTHREADDIALOG_H
