/*************************************************
  Copyright (C), 2020-2021, AIHua Tech. Co., Ltd.
  File name:    FixedHeadAndEndReceiveFliter.h
  Author:       yangchun
  Version:      0.0.01
  Date:         2021-4-6
  Description:  固定帧头和长度过滤器
  History:
*************************************************/
#ifndef FIXHEADANDLENGTHRECEIVEFILTER_H
#define FIXHEADANDLENGTHRECEIVEFILTER_H

#include "filter/ireceivefilter.h"
#include "LibCommunicate_global.h"

class LIBCOMMUNICATE_EXPORT FixHeadAndLengthReceiveFilter : public IReceiveFilter
{
public:
    FixHeadAndLengthReceiveFilter(const char * headBytes,  int length);
    std::list<Buffer *> *Filter(Buffer *receiveBuffer, int offset, int length, int &lastByteOffset);

    void setFilterClosed(bool isClosed);
private:
    int m_fixLength;
    const char *m_headBytes;// 协议头

    //是否关闭过滤器标志位
    bool isCloseFilter = false;
};

#endif // FIXHEADANDLENGTHRECEIVEFILTER_H
