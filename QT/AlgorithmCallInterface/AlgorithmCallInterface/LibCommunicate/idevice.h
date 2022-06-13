/*************************************************
  Copyright (C), 2020-2021, AIHua Tech. Co., Ltd.
  File name:    IProtocal.h
  Author:       yangchun
  Version:      0.0.01
  Date:         2021-4-6
  Description:  设备接口
  History:
*************************************************/
#ifndef IDEVICE_H
#define IDEVICE_H

#include "channel/ichannel.h"
#include "protocal/iprotocal.h"

enum ChannelStatus_E{
    ChannelOpen = 0,// 打开
    ChannelConnect,// 链接
    ChannelClose,// 关闭
    ChannelUnknow,// 未知
};

enum DeviceStatus_E{
    DeviceOffLine = 0,// 离线
    DeviceOnline,// 在线
    DeviceAbnormal// 异常
};

class IDevice
{
public:
    virtual bool Open() = 0;// 打开设备
    virtual bool Close() = 0;// 关闭设备
    virtual std::string GetDeviceName() = 0;// 获取设备名
    virtual void SetDeviceName(std::string deviceName) = 0;// 设置设备名
    virtual void Run()= 0;//设备运行器
    virtual bool Excute(Buffer *buffer) = 0;//执行命令
    virtual bool AsynExcute(Buffer *buffer) = 0;//异步执行命令
    virtual bool Analysis(Buffer *buffer) = 0;//分析结果
};

#endif // IDEVICE_H
