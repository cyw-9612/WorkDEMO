#ifndef MYFUNCTIONLISTWIDGET_H
#define MYFUNCTIONLISTWIDGET_H

#include <QDialog>
#include "childview.h"
#include "LibControl_global.h"
#include "myfunctiondisplaydialog.h"
#include "taskchoisedialog.h"

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

     //设置ANN参数列表信息
     void setANNInfo(QString dataJson);

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

     void sigCSTbuild();

     void sigANNtrain();

     void sigANNused();

     void sigANNTrainCancal();

     void sigANNTrainComfirm(QString text);

private slots:
     void slotCSTbuild();
     void slotANNtrain();
     void slotANNused();

     void slotANNTrainCancal();
     void slotANNTrainComfirm(QString dataJson);
private:
    Ui::myFunctionListwidget *ui;

    //功能展示界面模块
    myFunctionDisplayDialog *m_functionWidget = nullptr;

    //ANN参数测试列表
    taskChoiseDialog *m_taskParameterDialog = nullptr;
};

#endif // MYFUNCTIONLISTWIDGET_H
