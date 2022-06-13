/*************************************************
  Copyright (C), 2020-2021, AIHua Tech. Co., Ltd.
  File name:    FixedHeadAndEndReceiveFliter.h
  Author:       yangchun
  Version:      0.0.01
  Date:         2021-4-6
  Description:  固定头尾帧过滤器
  History:
*************************************************/
#ifndef FIXEDHEADANDENDRECEIVEFLITER_H
#define FIXEDHEADANDENDRECEIVEFLITER_H

#include "filter/ireceivefilter.h"
#include "LibCommunicate_global.h"

class LIBCOMMUNICATE_EXPORT FixedHeadAndEndReceiveFliter : public IReceiveFilter
{
public:
    FixedHeadAndEndReceiveFliter(const char * headBytes,  const char * endBytes);
    std::list<Buffer *> *Filter(Buffer *receiveBuffer, int offset, int length, int &lastByteOffset);

    void setFilterClosed(bool isClosed);
private:
    int m_headIndex;//协议头位置
    int m_tailIndex;//协议尾位置
    const char *m_headBytes;// 协议头
    const char *m_endBytes;// 协议尾

    //是否关闭过滤器标志位
    bool isCloseFilter = false;
};

#endif // FIXEDHEADANDENDRECEIVEFLITER_H
