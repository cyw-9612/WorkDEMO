#ifndef TCPCHANNEL_H
#define TCPCHANNEL_H

#include "ichannel.h"
#include "TCP/clienttcp.h"
#include "LibCommunicate_global.h"

/***********************************
 *
 * 功 能: TCP通道
 *
 * 编 制: 陈宇葳
 *
 * 日 期: 2020/06/11
 *
 **********************************/

#define MaxBuffer 64*1024

class LIBCOMMUNICATE_EXPORT TCPChannel : public IChannel
{
public:
    TCPChannel(ChannelOption hanneloption, ILogger *logger = nullptr);

    void SetLogger(ILogger *logger) override;// 日志
    bool Open() override;// 打开通道
    void Close() override;// 关闭通道
    bool Write(char *buffer, int &length) override;// 写数据
    std::list<Buffer *>* Read(IReceiveFilter *receiveFilter, int timeout) override;// 读数据
    bool isError() override;// 读是否出错
    bool clearBuffer() override;// 清除缓存

private:
    ChannelOption m_channelOption;// 通信参数
    clientTCP m_clientTCP;//TCP客户端
    ILogger *m_logger;
    Buffer m_Buffer;
    bool m_isError;
};

#endif // TCPCHANNEL_H
