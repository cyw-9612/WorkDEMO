#ifndef WINMAIN_H
#define WINMAIN_H

#include <QMenuBar>
#include <QMenu>
#include <QSharedPointer>
#include <QDebug>
#include "childview.h"
#include "baseview.h"
#include "myAESTool/usetimemark.h"
#include "QPluginLoader"
#include "myDataBase/cdatabase.h"
#include "communicate.h"
#include <logger/rtlogger.h>
#include "protocal/packParam.h"
#include "myFunctionListwidget/myfunctionlistwidget.h"
#include "myFunctionListwidget/myfunctiondisplaydialog.h"
#include "myFunctionListwidget/taskchoisedialog.h"
#include "myComputerInfoDialog/mycomputerinfodialog.h"
#include "myComputerInfoDialog/currentthreaddialog.h"
#include "myMessageDialog/minimessagedialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class winMain; }
QT_END_NAMESPACE

class winMain : public BaseView
{
    Q_OBJECT

public:
    winMain(QWidget *parent = nullptr);
    ~winMain();

public:
    void InitUI() override;// 界面初始化
    void InitSignal() override;// 信号初始化
    void RegisterNavigatePage(QString name, IView *view);
    void NavigatePage(QString name); // 设置当前页

    //获取winMain 唯一实例指针
    static QSharedPointer<winMain>& getInstance()
    {
        if (m_pInstance==nullptr)
        {
            QMutexLocker mutexLocker(&m_Mutex);

            if (m_pInstance==nullptr)
            {
                m_pInstance = QSharedPointer<winMain>(new winMain(nullptr));
            }
        }

        return m_pInstance;
    }

protected:
    //删除超过一周的日志
    void deleteOver7daysLog();

protected:
    void mousePressEvent(QMouseEvent *qevent) override;      // 鼠标按下事件
    void mouseReleaseEvent(QMouseEvent *qevent) override;    // 鼠标释放事件
    void mouseMoveEvent(QMouseEvent *qevent) override;       // 鼠标移动事件
    void mouseDoubleClickEvent(QMouseEvent *event) override; // 鼠标双击事件

signals:

private slots:
    void SlotMeasure();// 显示测量数据
    void SlotSetting();//设置按钮
    void SlotHelp();//帮助按钮

    void OnOptionOneSlot();
    void OnOptionTwoSlot();
    void OnOptionThreeSlot();

    void slotHFSSBuildClicked();
    void slotCSTBuildcliCked();
    void slotADSBuildcliCked();
    void slotCOMSOLBuildClicked();
    void slotANNPretreatmentClicked();
    void slotANNTrainClicked();
    void slotANNAutoOptClicked();
    void slotPythonCPUClicked();
    void slotPythongGPUClicked();
    void slotMATLABTaskClicked();
    void slotTaskMonitorClicked();

    void slotCSTbuild();
    void slotANNtrain();
    void slotANNused();

   void slotANNTrainCancal();
   void slotANNTrainComfirm(QString jsonText);
private:
    //查询电脑硬件信息
    QString getWMIC(const QString &cmd);

    //查询电脑主板信息
    QString getBaseboardNum();

    //查询电脑BIOS信息
    QString getBIOSNum();

    //查询电脑CPUID信息
    QString getCpuId();

    //查询电脑硬盘信息
    QString getDiskNum();

    //查询电脑IP地址
    QString getIpAddress();

    //获取电脑MAC地址
    QString getMAC();

    //通过调用windows接口获取当前日期
    qint32 getWebTime();

    //获取磁盘容量
    quint64 getDiskSpace(QString iDriver, bool flag);

    //获取电脑所有盘符名
    QStringList getDiskName();

private:
    //设置电脑信息栏改变布局
    void setLayoutMini(bool isMini);

    //更新ANN参数设置界面基本设置
    void updataANNInfo();

private:
    Ui::winMain *ui;
    static QSharedPointer<winMain> m_pInstance;// 智能指针:主窗体winMain
    QWidget* m_pMaskLayer = nullptr;// 蒙版遮蔽罩
    QPoint m_mousePos;              // 移动的距离
    bool   m_mousePress = false;            // 鼠标按下
    static QMutex m_Mutex;

    QMenu   *m_menu;                   // 菜单
    QAction *m_optionOne;   // 设置一
    QAction *m_optionTwo;   // 设置二
    QAction *m_optionThree; // 设置三

    ILogger *m_logger;  // 日志

    //获取数据库指针
    CDataBase *m_pDB = nullptr;

    //功能按键界面模块
    myFunctionListwidget * m_functionWidget = nullptr;

    //电脑信息界面模块
    myComputerInfoDialog *m_PCInfo = nullptr;

    //系统当前进程信息窗口
    currentThreadDialog *m_threadInfo = nullptr;

    //用于加资电脑信息的线程
    QThread *Thread_PCInfo;

    //消息提示弹窗
    miniMessageDialog *m_messageBox = nullptr;

};
#endif // WINMAIN_H
