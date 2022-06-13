#include "comchannel.h"
#include "binaryutil.h"
#include <singleton.h>
#include "logger/rtbaselogger.h"
#include <QDebug>

ComChannel::ComChannel(ChannelOption hanneloption, ILogger *logger)
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
void ComChannel::SetLogger(ILogger *logger)
{
    m_logger = logger;
}

// 打开通道
bool ComChannel::Open()
{
    m_isError = false;

    return m_serialPort.Open(m_channelOption.ComOption().Com(),
                      m_channelOption.ComOption().Baud());
}

// 关闭通道
void ComChannel::Close()
{
    m_serialPort.Close();
}

// 写数据
bool ComChannel::Write(char *buffer, int &length)
{
    if(length == 0)
    {
        return true;
    }

    length = m_serialPort.Send((unsigned char *)buffer, length);
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
std::list<Buffer *>* ComChannel::Read(IReceiveFilter *receiveFilter, int timeOut)
{
    Q_UNUSED(timeOut);
    std::list<Buffer *> *data = nullptr;
    bool hasNewData = false;
    int recvLength = 0;
    unsigned char buffer[MaxBuffer] = {0};

    memset(buffer, 0, MaxBuffer);
    recvLength = m_serialPort.Recv((unsigned char *)buffer, MaxBuffer);
    if(recvLength > 0)
    {
        m_Buffer.Append(buffer, recvLength);
        hasNewData = true;
        m_isError = false;
        m_logger->LoggerWrite(LoggerLevel_E::LevelTrace, QString("recv data: %1").arg((char *)buffer));
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
            m_Buffer.Skip(next - offset);
        }
    }

    return data;
}

bool ComChannel::isError()
{
    return m_isError == true;
}

bool ComChannel::clearBuffer()
{
    m_Buffer.Reset();
    return true;
}




