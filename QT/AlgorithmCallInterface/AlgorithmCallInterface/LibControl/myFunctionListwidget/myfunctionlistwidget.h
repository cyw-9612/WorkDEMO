#ifndef MYFUNCTIONLISTWIDGET_H
#define MYFUNCTIONLISTWIDGET_H

#include <QDialog>
#include "childview.h"
#include "LibControl_global.h"

/***********************************
 *
 * 功 能: 功能展示对话框类
 *
 * 编 制: 陈宇葳
 *
 * 日 期: 2022/06/014
 *
 **********************************/

namespace Ui {
class myFunctionListwidget;
}

class LIBCONTROL_EXPORT myFunctionListwidget : public ChildView
{
    Q_OBJECT

public:
    explicit myFunctionListwidget(QWidget *parent = nullptr);
    ~myFunctionListwidget();

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
     void sigHFSSBuildClicked();

     void sigCSTBuildClicked();

     void sigADSBuildcliCked();

     void sigCOMSOLBuildClicked();

     void sigANNPretreatmentClicked();

     void sigANNTrainClicked();

     void sigANNAutoOptClicked();

     void sigPythonCPUClicked();

     void sigPythongGPUClicked();

     void sigMATLABTaskClicked();

     void sigTaskMonitorClicked();

private slots:
    void on_btn_HFSSBuild_clicked();

    void on_btn_CSTBuild_clicked();

    void on_btn_ADSBuild_clicked();

    void on_btn_COMSOLBuild_clicked();

    void on_btn_ANNPretreatment_clicked();

    void on_btn_ANNTrain_clicked();

    void on_btn_ANNAutoOpt_clicked();

    void on_btn_PythonCPU_clicked();

    void on_btn_PythongGPU_clicked();

    void on_btn_MATLABTask_clicked();

    void on_btn_TaskMonitor_clicked();

private:
    Ui::myFunctionListwidget *ui;
};

#endif // MYFUNCTIONLISTWIDGET_H
