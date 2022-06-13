/*************************************************
  Copyright (C), 2020-2021, AIHua Tech. Co., Ltd.
  File name:    ComChannel.h
  Author:       yangchun
  Version:      0.0.01
  Date:         2021-4-6
  Description:  串口通道
  History:
*************************************************/
#ifndef COMCHANNEL_H
#define COMCHANNEL_H

#include "ichannel.h"
#include "serialport/serialport.h"
#include "LibCommunicate_global.h"

#define MaxBuffer 1024

class LIBCOMMUNICATE_EXPORT ComChannel : public IChannel
{
public:
    ComChannel(ChannelOption hanneloption, ILogger *logger = nullptr);

    void SetLogger(ILogger *logger) override;// 日志
    bool Open() override;// 打开通道
    void Close() override;// 关闭通道
    bool Write(char *buffer, int &length) override;// 写数据
    std::list<Buffer *>* Read(IReceiveFilter *receiveFilter, int timeout) override;// 读数据
    bool isError() override;// 读是否出错
    bool clearBuffer() override;// 清除缓存

private:
    ChannelOption m_channelOption;// 通信参数
    SerialPort m_serialPort;//串口
    ILogger *m_logger;
    Buffer m_Buffer;
    bool m_isError;
};

#endif // COMCHANNEL_H
