/*************************************************
  Copyright (C), 2020-2021, AIHua Tech. Co., Ltd.
  File name:    FixedHeadAndEndReceiveFliter.h
  Author:       yangchun
  Version:      0.0.01
  Date:         2021-4-6
  Description:  数据帧接口
  History:
*************************************************/
#ifndef IRECEIVEFILTER_H
#define IRECEIVEFILTER_H

#include <memory/buffer.h>
#include <list>
#include "LibCommunicate_global.h"

class LIBCOMMUNICATE_EXPORT IReceiveFilter
{
public:
     virtual std::list<Buffer *>* Filter(Buffer *receiveBuffer, int offset, int length, int &lastByteOffset) = 0;// 缓存过滤器，从接收缓存里跳出有效数据

    virtual void setFilterClosed(bool isClosed) = 0; //设置过滤器关闭（直接返回接收到的信息）
};

#endif // IRECEIVEFILTER_H
