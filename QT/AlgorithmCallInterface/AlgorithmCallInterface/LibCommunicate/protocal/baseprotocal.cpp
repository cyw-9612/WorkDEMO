#include "baseprotocal.h"
#include <singleton.h>
#include "logger/rtbaselogger.h"
#include <QDebug>
#include <QFile>
#include "packParam.h"

BaseProtocal::BaseProtocal(IReceiveFilter *filter, ILogger *logger)
{
    m_filer = filter;// 设置过滤器
    m_logger = logger;// 设置日志
    if(m_logger == nullptr)
    {
        m_logger = &Singleton<RTBaseLogger>::getInstance();
    }
}


void BaseProtocal::SetLogger(ILogger *logger)
{
    m_logger = logger;
}

// 获取协议过滤器
IReceiveFilter *BaseProtocal::GetFilter()
{
    return m_filer;
}

// 设置过滤器
void BaseProtocal::SetFilter(IReceiveFilter *filter)
{
    m_filer = filter;
}

// 校验数据是否合法
bool BaseProtocal::Check(Buffer *buffer)
{
    if(buffer != nullptr || buffer->length() > 0)
    {
        return true;// 数据正确
    }

    return false;// 数据错误
}

// 获取协议头
Slice BaseProtocal::GetHead(Buffer *buffer)
{
    if(buffer == nullptr || buffer->length() == 0)
    {
        return Slice();// 数据非法
    }

    return Slice(buffer->data(), 1);
}

// 获取协议尾巴
Slice BaseProtocal::GetTail(Buffer *buffer)
{
    if(buffer == nullptr || buffer->length() == 0)
    {
        return Slice();// 数据非法
    }

    return Slice(buffer->WriteBegin() - 1,  1);
}

// 解析数据
void BaseProtocal::Analysis(Buffer *buffer)
{
    //Q_UNUSED(buffer);
    //将获取到的数据根据既定通信协议进行处理
    QString data = QString::fromStdString(buffer->ToString());
    //去除无效数据
    data = data.left(buffer->length());
    QString deviceName = QString::fromStdString(m_name);
    if("AWA5912" == deviceName)
    {
        qDebug() << "BaseProtocal::Analysis 待处理数据为: AWA5912" << data;
    }
    else if("AWA6292" == deviceName)
    {
        qDebug() << "BaseProtocal::Analysis 待处理数据为: AWA6292" << data;
    }
    else
    {
        qDebug() << "BaseProtocal::Analysis 待处理数据为:" << QString::fromStdString(m_name) << data;
    }

    //更具设备通信进行处理
    return;
}

// 打包数据
void BaseProtocal::Package(Buffer *buffer, QVariant &para)
{
    Q_UNUSED(buffer);
    Q_UNUSED(para);

    return;
}

// 获取设备名
std::string BaseProtocal::GetDeviceName()
{
    return m_name;
}

// 设置设备名
void BaseProtocal::SetDeviceName(std::string name)
{
    m_name = name;
}

//检测设备是否在线
bool BaseProtocal::isSerialOnline(QByteArray buffer)
{
    Q_UNUSED(buffer);
    return true;
}

//设置过滤器是否关闭
void BaseProtocal::setFilterClosed(bool isclosed)
{
    m_filer->setFilterClosed(isclosed);
}

void BaseProtocal::stopReadFile(bool isSendMessage)
{

}
