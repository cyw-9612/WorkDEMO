#include "tcpchannel.h"
#include "binaryutil.h"
#include <singleton.h>
#include "logger/rtbaselogger.h"

TCPChannel::TCPChannel(ChannelOption hanneloption, ILogger *logger)
{
    m_logger = nullptr;
    m_isError = false;
    m_logger = logger;
    if(m_logger == nullptr)
    {
        m_logger = &Singleton<RTBaseLogger>::getInstance();
    }
    m_channelOption = hanneloption;
}

// 日志
void TCPChannel::SetLogger(ILogger *logger)
{
    m_logger = logger;
}

// 打开通道
bool TCPChannel::Open()
{
    m_isError = false;

    return m_clientTCP.Open(m_channelOption.NetOption().IP(),
                            m_channelOption.NetOption().Port());
}

// 关闭通道
void TCPChannel::Close()
{
    m_clientTCP.Close();
}

// 写数据
bool TCPChannel::Write(char *buffer, int &length)
{
    if(length == 0)
    {
        return true;
    }

    length = m_clientTCP.Send((unsigned char *)buffer, length);

    if(length > 0)
    {
        m_logger->LoggerWrite(LoggerLevel_E::LevelTrace, QString("send data: %1").arg(buffer));
    }

    if(length == -1)
    {
        m_isError = true;
    }
    else
    {
        m_isError = false;
    }

    return length > 0;
}

// 读数据
std::list<Buffer *>* TCPChannel::Read(IReceiveFilter *receiveFilter, int timeOut)
{

    Q_UNUSED(timeOut);
    std::list<Buffer *> *data = nullptr;
    bool hasNewData = false;
    int recvLength = 0;
    unsigned char buffer[MaxBuffer] = {0};

    memset(buffer, 0, MaxBuffer);
    recvLength = m_clientTCP.Recv((unsigned char *)buffer, MaxBuffer);
    if(recvLength > 0)
    {
        m_Buffer.Append(buffer, recvLength);
        hasNewData = true;
        m_isError = false;
        m_logger->LoggerWrite(LoggerLevel_E::LevelTrace, QString("recv data: %1").arg((char *)buffer));
        //qDebug() << "QString::fromUtf8:" << QString::fromUtf8((char *)buffer);
    }
    else if(recvLength == -1)
    {
        m_isError = true;
    }
    else
    {
        m_isError = false;
    }

    if(hasNewData)// 有数据
    {
        int offset = m_Buffer.PrependableBytes();
        int next = 0;
        data = receiveFilter->Filter(&m_Buffer, offset, m_Buffer.length(), next);// 过滤数据包
        if(next > offset)
        {
            //qDebug() << "buffer_skip:" << next - offset;
//            if((next - offset) > 1048576)
//            {
//                qDebug()<< "数据解析异常";
//            }
            m_Buffer.Skip(next - offset);

            //如果有数据读取位附加了偏移量，则去除前面的以读取数据位
            if(m_Buffer.PrependableBytes() > 0)
            {
                Buffer temp(m_Buffer.length(), 0);
                temp.Write(m_Buffer.data(), m_Buffer.length());
                m_Buffer.Swap(temp);
            }
        }
    }


    return data;
}

bool TCPChannel::isError()
{
    return m_isError == true;
}

bool TCPChannel::clearBuffer()
{
    m_Buffer.Reset();
    return true;
}
