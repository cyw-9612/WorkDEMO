#ifndef TASKCHOISEDIALOG_H
#define TASKCHOISEDIALOG_H

#include <QDialog>
#include "childview.h"
#include "LibControl_global.h"

/***********************************
 *
 * 功 能: 参数设置对话框
 *
 * 编 制: 陈宇葳
 *
 * 日 期: 2022/06/21
 *
 **********************************/

namespace Ui {
class taskChoiseDialog;
}

class LIBCONTROL_EXPORT taskChoiseDialog : public ChildView
{
    Q_OBJECT

public:
    explicit taskChoiseDialog(QWidget *parent = nullptr);
    ~taskChoiseDialog();

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
    void sigCancal();

    void sigComfirm(QString dataJson);

private slots:
     void on_btn_clearDate_clicked();

     void on_btn_export_clicked();

     void on_edit_function1_editingFinished();

     void on_edit_function2_editingFinished();

     void on_edit_function3_editingFinished();

     void on_edit_function4_editingFinished();

     void on_edit_function5_editingFinished();

     void on_edit_function6_editingFinished();

private:
    Ui::taskChoiseDialog *ui;
    QString m_UIInfo = "";
};

#endif // TASKCHOISEDIALOG_H
