/*************************************************
  Copyright (C), 2020-2021, AIHua Tech. Co., Ltd.
  File name:    FixedHeadAndEndReceiveFliter.h
  Author:       yangchun
  Version:      0.0.01
  Date:         2021-4-6
  Description:  固定长度帧过滤器
  History:
*************************************************/
#ifndef FIXEDLENGTHRECEIVEFLITER_H
#define FIXEDLENGTHRECEIVEFLITER_H

#include "filter/ireceivefilter.h"
#include "LibCommunicate_global.h"

class LIBCOMMUNICATE_EXPORT FixedLengthReceiveFliter : public IReceiveFilter
{
public:
    FixedLengthReceiveFliter(int length);

    void setFilterClosed(bool isClosed) override;
public:
    std::list<Buffer *> *Filter(Buffer *receiveBuffer, int offset, int length, int &lastByteOffset) override;// 数据过滤器

private:
    int _fixLength;

    //是否关闭过滤器标志位
    bool isCloseFilter = false;
};

#endif // FIXEDLENGTHRECEIVEFLITER_H
