#ifndef MYCOMPUTERINFODIALOG_H
#define MYCOMPUTERINFODIALOG_H

#include <QDialog>
#include "childview.h"
#include "LibControl_global.h"
#include "SysInfo/cpuwidget.h"
#include "SysInfo/memorywidget.h"
#include "SysInfo/sysinfominiwidgat.h"

/***********************************
 *
 * 功 能: 设备信息展示对话框类
 *
 * 编 制: 陈宇葳
 *
 * 日 期: 2022/06/015
 *
 **********************************/

class threadCheck;

namespace Ui {
class myComputerInfoDialog;
}

class LIBCONTROL_EXPORT myComputerInfoDialog : public ChildView
{
    Q_OBJECT

public:
    explicit myComputerInfoDialog(QWidget *parent = nullptr);
    ~myComputerInfoDialog();

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

     void setMiniMode(bool isMini);
private:
    Ui::myComputerInfoDialog *ui;

    CpuWidget mCpuWidget;
    MemoryWidget mMemoryWidget;

    sysInfoMiniWidgat *m_miniInfoWIdget = nullptr;
};

#endif // MYCOMPUTERINFODIALOG_H
