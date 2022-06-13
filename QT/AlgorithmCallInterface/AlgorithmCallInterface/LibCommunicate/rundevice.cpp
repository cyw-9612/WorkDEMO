#include "rundevice.h"
#include <singleton.h>
#include <QDebug>
#include <QDateTime>
#include "logger/rtbaselogger.h"

RunDevice::RunDevice(IChannel *channel, IProtocal *protocal, ILogger *logger)
{
    m_protocal = protocal;
    m_channel = channel;
    m_channelStatus = ChannelUnknow;
    m_logger = logger;
    if(m_logger == nullptr)
    {
        m_logger = &Singleton<RTBaseLogger>::getInstance();
    }
    m_sendBuffer = new Buffer(1024);

    isTargetCmd = nullptr;
}

// 打开设备
bool RunDevice::Open()
{
    if (m_channel == nullptr)
    {
        return false;
    }

    bool flag = m_channel->Open();

    if(flag)
    {
        m_channelStatus = ChannelOpen;
        emit DeviceStatus(DeviceOnline);
     }

    return flag;
}

// 关闭设备
bool RunDevice::Close()
{
    if (m_channel == nullptr)
    {
        return false;
    }

    if (m_channel != nullptr)
    {
        m_channel->Close();
    }

    m_channelStatus = ChannelClose;

    emit DeviceStatus(DeviceOffLine);

    return true;
}

// 获取设备名
std::string RunDevice::GetDeviceName()
{
    return m_name;
}

// 设置设备名
void RunDevice::SetDeviceName(std::string name)
{
    m_name = name;
    m_protocal->SetDeviceName(m_name);
}

// 轮询设备
void RunDevice::Run()
{
    int length = 0;

    if(m_channelStatus != ChannelOpen)
    {
        QThread::msleep(500);
        return;
    }

    if(isSendPlaus)
    {
        qint64 currentTime = QDateTime::currentDateTime().toMSecsSinceEpoch();
        if((currentTime - m_startTime) > 1000)
        {
            //心跳回复超时
            m_channelStatus = ChannelClose;
            isSendPlaus = false;
            emit DeviceStatus(DeviceAbnormal);//设置通道异常
        }
    }

    /*
     * 异步发送数据命令
     */
    if (m_sendBuffer->length() > 0)
    {
        m_writeLock.lock();
        length = m_sendBuffer->length();

        if(isTargetCmd)
        {
            isTargetCmd(QString((char *)m_sendBuffer->data()), *m_channel);
        }
//        if(QString((char *)m_sendBuffer->data()).contains("\"Class\":\"134\""))
//        {
//            m_channel->clearBuffer();
//        }

        bool flag = m_channel->Write((char *)m_sendBuffer->data(), length);
        if(flag)
        {
            m_sendBuffer->Skip(length);// 移除已发送数据
        }

        m_writeLock.unlock();
    }

    std::list<Buffer *> *lstBuffer = m_channel->Read(m_protocal->GetFilter(), 5);
    if(lstBuffer != nullptr )
    {

        for(std::list<Buffer *>::iterator item = lstBuffer->begin(); item != lstBuffer->end(); item++)
        {
            //qDebug() << QDateTime::currentDateTime();
            Buffer *response = *item;

            if(m_protocal->Check(response))
            {
                QString data = QString(response->data());
                if(response->length() > 1000)
                {
                    data = QString(response->data()).left(1000);
                    data = data + ".... 数据过长";
                }
                m_logger->LoggerWrite(LoggerLevel_E::LevelTrace, QString("recv package: %1").arg(data));

                Analysis(*item);//数据包合法解析数据-
            }

            //清除过滤器申请缓存
            delete  response;
        }

        //清理通道通信缓存
        //m_channel->clearBuffer();

        delete lstBuffer;
    }
    else
    {
        if(m_channel->isError())// 判断通道是否错误
        {
            emit DeviceStatus(DeviceAbnormal);//设置通道异常
            //等待主线程处理
            QThread::msleep(500);
        }
    }
}

// 同步执行命令
bool RunDevice::Excute(Buffer *request)
{
    if(request == nullptr)
    {
        return false;
    }

    int length = request->length();

    return m_channel->Write((char *)request->data(), length);// 发送命令数据
}

// 异步执行命令
bool RunDevice::AsynExcute(Buffer *buffer)
{
    m_writeLock.lock();
    m_sendBuffer->Append(buffer->data(), buffer->length());
    m_writeLock.unlock();

    return true;
}

// 数据解析
bool RunDevice::Analysis(Buffer *buffer)
{
    if(m_protocal != nullptr)
    {
        if(isSendPlaus)
        {
            //将获取到的数据根据既定通信协议进行处理
            QString data = QString::fromStdString(buffer->ToString());
            //去除无效数据
            data = data.left(buffer->length());

            //如果设备在线，返回true 心跳标志复位为false
            isSendPlaus = !m_protocal->isSerialOnline(data.toLatin1());
        }
        else
        {
            m_protocal->Analysis(buffer);
        }

    }

    return true;
}

//检测串口设备是否连接
bool RunDevice::checkDeviceState()
{
    isSendPlaus = true;

    m_startTime = QDateTime::currentDateTime().toMSecsSinceEpoch();
    return true;
}

//设置protocal过滤器是否关闭
void RunDevice::setFilterClosed(bool isClosed)
{
    m_protocal->setFilterClosed(isClosed);
}

//设置AWA6292目标指令过滤器
void RunDevice::setTargetCMDFiltering(void (*function)(QString ,IChannel&))
{
    isTargetCmd = function;
}

//设置AWA6292停止读取文件
void RunDevice::stopReadFile(bool isSendMessage)
{
    m_protocal->stopReadFile(isSendMessage);
}
