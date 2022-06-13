/*************************************************
  Copyright (C), 2020-2021, AIHua Tech. Co., Ltd.
  File name:    RunDevice.h
  Author:       yangchun
  Version:      0.0.01
  Date:         2021-4-6
  Description:  设备
  History:
*************************************************/
#ifndef RUNDEVICE_H
#define RUNDEVICE_H

#include <QTimer>
#include <QThread>
#include <QMutex>
#include <QObject>
#include "idevice.h"
#include "LibCommunicate_global.h"

class LIBCOMMUNICATE_EXPORT RunDevice :  public QObject, public IDevice
{
    Q_OBJECT
public:
    RunDevice(IChannel *channel, IProtocal *protocal, ILogger *logger);

    bool Open() override;// 打开设备
    bool Close() override;// 关闭设备
    std::string GetDeviceName() override;// 获取设备名
    void SetDeviceName(std::string deviceName) override;// 设置设备名
    void Run() override;//运行器
    bool Excute(Buffer *buffer) override;//执行命令
    bool AsynExcute(Buffer *buffer) override;//异步执行命令
    bool Analysis(Buffer *buffer) override;//分析结果

    //检测串口设备是否连接
    bool checkDeviceState();

    //设置protocal过滤器是否关闭
    void setFilterClosed(bool isClosed);

    //设置AWA6292目标指令过滤器
    void setTargetCMDFiltering(void (*function)(QString ,IChannel&));

    //设置AWA6292停止读取文件
    void stopReadFile(bool isSendMessage);
signals:
    void DeviceStatus(int status);//设备状态：0：离线 1：在线 2：异常

    void connectDeviceInfo(QString deviceName, QString deviceNum);//设备连接信息 设备型号和设备串号

private:
    IChannel *m_channel;// 通道
    IProtocal *m_protocal;// 设备协议
    ILogger *m_logger;// 日志
    ChannelStatus_E m_channelStatus;// 通道状态
    Buffer *m_sendBuffer;// 发送缓冲
    QTimer m_timer;// 定时器
    QMutex m_writeLock;// 写锁
    std::string m_name;// 设备名

    //心跳问询状态标志
    bool isSendPlaus = false;

    //发送心跳开始时间
    qint64 m_startTime = 0;

    //AWA6292指令过滤函数指针
    void (*isTargetCmd)(QString ,IChannel&);
};

#endif // RUNDEVICE_H
