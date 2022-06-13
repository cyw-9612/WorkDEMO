#ifndef IVIEW_H
#define IVIEW_H

#include <QObject>
#include <QString>

class ILogger;

enum TotastType_E {
    TOTAST_INFO = 1,
    TOTAST_WARNING = 2,
    TOTAST_ANSWER = 3
};

enum UIEventType_E { //定义界面事件
    Event_Type_Init = 1,// 界面初始化
    Event_Type_Check_Error = 2,// 输入项校验错误
};

class IView
{
public:
    virtual void InitSignal() = 0;  // 初始化界面信号
    virtual void InitUI() = 0;      // 初始化界面
    virtual QString GetTitle() = 0;    // 获取界面标题
    virtual void SetTitle(QString title) = 0;// 设置界面标题
    virtual void LoadDate() = 0;    // 加载数据
    virtual void Navigate(int pageID) = 0;// 页导航
    virtual void Show() = 0;        // 显示界面
    virtual void Hide() = 0;        // 隐藏界面
    virtual void SetLogger(ILogger *logger) = 0; // 显示界面
    virtual void ShowWaiting() = 0; // 显示界面
    virtual void HideWaiting() = 0; // 显示界面
    virtual QWidget* GetQWidget() = 0;    // 加载数据
    virtual int ShowTotast(TotastType_E type, QString title, QString msg, QString desc = "") = 0;// 显示提示框
    virtual void ModifyControlStyle(UIEventType_E event, int error = 0) = 0;// 修改控件状态
};

#endif // IVIEW_H
