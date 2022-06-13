/*************************************************
  Copyright (C), 2020-2021, AIHua Tech. Co., Ltd.
  File name:    baseview.h
  Author:       yangchun
  Version:      0.0.01
  Date:         2020-5-15
  Description:  自定义弹出对话框
  History:
*************************************************/
#ifndef BASEVIEW_H
#define BASEVIEW_H

#include <QComboBox>
#include <QDialog>
#include <QTimer>
#include "LibControl_global.h"
#include "iview.h"
#include "logger/ilogger.h"
#include "rtmasklayer.h"

class BaseController;

class LIBCONTROL_EXPORT BaseView : public QDialog, public IView
{
    Q_OBJECT
public:
    explicit BaseView(QWidget *parent = nullptr);
    void InitSignal() override; // 初始化界面信号
    void InitUI() override;     // 初始化界面
    virtual QString GetTitle() override;// 获取界面标题
    virtual void SetTitle(QString title) override;// 设置界面标题
    void Navigate(int pageID) override;// 页导航
    void LoadDate() override;   // 加载数据
    void Show() override;       // 显示界面
    void Hide() override;       // 隐藏界面
    void SetLogger(ILogger *logger) override;// 设置日志
    void HandleException(exception &ex);// 处理异常
    void ShowWaiting() override;// 显示等待框
    void HideWaiting() override;// 隐藏对待框
    QWidget *GetQWidget() Q_DECL_OVERRIDE;
    int ShowTotast(TotastType_E type, QString title, QString msg, QString desc = "") override;// 显示提示信息
    void ModifyControlStyle(UIEventType_E event, int error = 0) override;// 修改控件状态
    void SetComboBoxAlignment(QComboBox *control, Qt::AlignmentFlag flag);// 设置下拉框对齐方式

protected:
    void keyPressEvent(QKeyEvent *event) override;// 处理键盘按键

public slots:
    void SlotMin();//界面最小化
    void SlotMax();//界面最大化
    void SlotClose();//界面关闭

public:
     ILogger *m_logger;// 日志
     bool     m_enableMove;// 界面是否能拖动

protected:
    QTimer  *m_timer;// 界面定时器
    QString m_title; // 界面标题
    bool m_modify;   // 界面值修改
    bool m_initUI;   // 界面初始化标记
    RtMaskLayer *m_maskLayer;
};

#endif // BASEVIEW_H
