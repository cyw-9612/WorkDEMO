

#ifndef PROTOCOLSETTINGMANAGER_H
#define PROTOCOLSETTINGMANAGER_H

#include <QObject>
#include <QString>
#include <QDateTime>
#include <QThread>
#include <QMutex>

#include "workmanager.h"
#include "command.h"
#include "comm_interface.h"


class ProtocolManager : public QThread
{
    Q_OBJECT
private:
    ProtocolManager();
    ~ProtocolManager();
public:

    //枚举指令错误类型
    enum ProtocolError {
        CrcCheckError,
        ReTry3TimesError
    };
    static ProtocolManager* GetInstance();

    //设置协议和指令的连接
    void SetProtocolComm(CommInterface* comm);
    //添加一个工作类
    void AddWork(QByteArray &pkg);
    //得到现有工作类的计数
    int GetCachedWorkCount();
//    //获得当前线程状态
//    bool GetIsSending();

    //comm函数实现
    void CommInit();
    bool CommOpen();
    void CommClose();
    bool CommIsOpen();

signals:
    //std signals
    void onCommInitCallback();
    //协议错误信号（错误类型，错误信息）
    void onProtocolError(int error_type, QString error_string);
    //派遣识别一个包（包地址，包长）
    void onDispatchPackage(quint8 *pkg, int len);


private:
    static ProtocolManager *instance_p;

    //连接函数 发送和接收
    qint64 Send(const QByteArray &data);
    void onRecv(QByteArray &data, void *extra_data);

    //进程函数
    void Process(quint8 b);
    //发送原来的包
    void ProcessMetaPkg(quint8 *pkg, int len);
    //清除工作列表
    void CleanWorkList();
    //重发一个包
    void ReSendPkg();
    //进程超时错误
    void ProcessTimeOutError();

    //thread
    void run();
    void StartThread();
    void StopThread();
    bool IsThreadWorking();

    //thread-switch-functions
    void doSend();
signals:
    void callSend();

private:
      CommInterface *comm_p;
      bool is_sending_;

      WorkManager wm_;
      bool wait_response_;
      Work *current_work_p;

      //包的参数设定
      //定义当前是数据是否处于包内
      bool package_on_;
      //定义cach数组长度
      const static int CACHE_LENGTH = 1024 * 5;
      quint8 cache[CACHE_LENGTH];
      //定义当前数据的位置指针
      int pos;

      //QMutex提供的是线程之间的访问顺序化
      QMutex mutex_;
      //开始时间，用于计算是否超时
      qint64 start_work_secs_;
      //等待超时为10S
      const qint64 TIME_OUT_SECONDS = 10;

};

#endif // PROTOCOLSETTINGMANAGER_H
