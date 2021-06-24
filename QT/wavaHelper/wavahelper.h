#ifndef WAVAHELPER_H
#define WAVAHELPER_H

#include <QDialog>
#include <QMutex>
#include <QMenu>
#include <QSharedPointer>
#include <QDebug>
#include <QSystemTrayIcon>
#include <QFile>
#include <QFutureWatcher>
#include <QSettings>
#include "wavefile.h"
#include "minimeaasgebox.h"

/***********************************
 *
 * 功 能: 录音音频文件处理软件:主框架窗口
 *
 * 编 制: 陈宇葳
 *
 * 日 期: 2021/06/17
 *
 **********************************/

QT_BEGIN_NAMESPACE
namespace Ui { class wavaHelper; }
QT_END_NAMESPACE

class wavaHelper : public QDialog
{
    Q_OBJECT

public:
    wavaHelper(QWidget *parent = nullptr);
    ~wavaHelper();

    //关联信号槽
    void connectSlots();

    //获取winMain 唯一实例指针
    static QSharedPointer<wavaHelper>& getInstance()
    {
        if (m_pInstance==nullptr)
        {
            QMutexLocker mutexLocker(&m_Mutex);

            if (m_pInstance==nullptr)
            {
                m_pInstance = QSharedPointer<wavaHelper>(new wavaHelper(nullptr));
            }
        }

        return m_pInstance;
    }

private:
    //鼠标按下事件
    void mousePressEvent(QMouseEvent *event);
    //鼠标移动事件
    void mouseMoveEvent(QMouseEvent *event);
    //释放鼠标
    void mouseReleaseEvent(QMouseEvent *event);

    //目前控件在父窗口中的位置
    QPoint m_windowPos;
    //鼠标位置
    QPoint m_mousePos;
    //移动后位置
    QPoint m_dPos;
    //是否移动窗体标志位： true 移动,false 不动
    bool m_moveFlag=false;

private:
    //UI初始化
    void initUI();

    //处理音频文件信息
    void analyFileInfo(QString Text);

    //根据设置生成新的文件
    void writeNewWavFile();

    static QMutex m_Mutex;
    //智能指针:主窗体winMain
    static QSharedPointer<wavaHelper> m_pInstance;

private slots:
    //槽函数:最小化
    void slotMin();

    //槽函数:最大化 (工具窗口大小固定)
    //void slotMax();

    //槽函数:关闭
    void slotClose();

    //选择音频文件
    void slotChoseFile();

    //选择文件存储路径
    void slotSaveFile();

    //选择放大倍数
    void slotChoseEnlarge(bool checked);

    //开始进行音频处理
    void slotStartToHandle();

    //完成读取或生成文件的分析
    void slotReadFinish();

private:
    Ui::wavaHelper *ui;

    //消息提示框
    miniMeaasgeBox *m_message = nullptr;

    //音频文件读取类
    WaveFile m_Wavefile;

    QString m_Filename; //文件选择路径
    QString m_saveFilename; //文件存储路径

    //仪器类型
    QString m_type;

    //保存的文件对象
    QFile * m_file;

    //QFuture本身不带信号槽，可使用QFutureWatcher进行监控
    QFutureWatcher<void> *myWatcher;

    //标志线程执行的是否位读取文件 true为读取文件 false为生成文件
    bool isReadFile = true;

    bool isErrorForHandle = false;

    QSettings *m_configIni;
};
#endif // WAVAHELPER_H
