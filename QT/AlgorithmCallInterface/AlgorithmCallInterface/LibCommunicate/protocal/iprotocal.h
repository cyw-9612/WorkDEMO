/*************************************************
  Copyright (C), 2020-2021, AIHua Tech. Co., Ltd.
  File name:    IProtocal.h
  Author:       yangchun
  Version:      0.0.01
  Date:         2021-4-6
  Description:  协议处理接口
  History:
*************************************************/
#ifndef IPROTOCAL_H
#define IPROTOCAL_H

#include <QVariant>
#include "filter/ireceivefilter.h"
#include "memory/buffer.h"
#include "logger/ilogger.h"

class IProtocal
{
public:
    virtual void SetLogger(ILogger *logger) = 0;// 日志
    virtual IReceiveFilter * GetFilter() = 0;//获取协议过滤器
    virtual void SetFilter(IReceiveFilter *filter) = 0;//设置协议过滤器
    virtual bool Check(Buffer* buffer) = 0;// 校验协议数据
    virtual Slice GetHead(Buffer* buffer) = 0;// 返回协议头
    virtual Slice GetTail(Buffer* buffer) = 0;// 返回协议尾巴
    virtual void Analysis(Buffer* buffer) = 0;// 解析数据
    virtual void Package(Buffer* buffer,  QVariant &para) = 0;// 打包数据

    virtual std::string GetDeviceName() = 0;// 获取设备名
    virtual void SetDeviceName(std::string deviceName) = 0;// 设置设备名

    virtual bool isSerialOnline(QByteArray buffer)  = 0; //检测设备是否在线

    virtual void setFilterClosed(bool isclosed)  = 0; //设置过滤器是否关闭

    virtual void stopReadFile(bool isSendMessage) = 0; //停止读取文件
};

#endif // IPROTOCAL_H
