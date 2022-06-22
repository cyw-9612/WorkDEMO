#ifndef MYFUNCTIONDISPLAYDIALOG_H
#define MYFUNCTIONDISPLAYDIALOG_H

#include <QDialog>
#include "childview.h"
#include "LibControl_global.h"

/***********************************
 *
 * 功 能: 功能展示对话框类二
 *
 * 编 制: 陈宇葳
 *
 * 日 期: 2022/06/20
 *
 **********************************/

namespace Ui {
class myFunctionDisplayDialog;
}

class LIBCONTROL_EXPORT myFunctionDisplayDialog : public ChildView
{
    Q_OBJECT

public:
    explicit myFunctionDisplayDialog(QWidget *parent = nullptr);
    ~myFunctionDisplayDialog();

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

signals:
     void sigCSTbuild();

     void sigANNtrain();

     void sigANNused();

private slots:
     void on_btn_CSTbuild_clicked();

     void on_btn_ANNtrain_clicked();

     void on_btn_ANNused_clicked();

private:
    Ui::myFunctionDisplayDialog *ui;
};

#endif // MYFUNCTIONDISPLAYDIALOG_H
