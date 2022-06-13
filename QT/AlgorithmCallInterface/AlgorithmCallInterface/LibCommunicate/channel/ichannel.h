/*************************************************
  Copyright (C), 2020-2021, AIHua Tech. Co., Ltd.
  File name:    IChannel.h
  Author:       yangchun
  Version:      0.0.01
  Date:         2021-4-6
  Description:  通道接口
  History:
*************************************************/
#ifndef ICHANNEL_H
#define ICHANNEL_H

#include <memory/buffer.h>
#include "channeloption/channeloption.h"
#include "filter/ireceivefilter.h"
#include "logger/ilogger.h"
#include <list>

class IChannel
{
public:
    virtual void SetLogger(ILogger *logger) = 0;// 日志
    virtual bool Open() = 0;// 打开通道
    virtual void Close() = 0;// 关闭通道
    virtual bool Write(char *buffer, int &length) = 0;// 写数据
    virtual std::list<Buffer *>* Read(IReceiveFilter *receiveFilter, int timeOut = 100) = 0;// 读数据
    virtual bool isError() = 0;// 读是否出错
    virtual bool clearBuffer() = 0;// 清除缓存
};

#endif // ICHANNEL_H
