#include <qdebug.h>
#include <QMutex>
#include <QWaitCondition>
#include <QSerialPortInfo>
#include "serialport.h"


SerialPort::SerialPort(QObject *parent) : QObject(parent)
{
    m_serial = new QSerialPort();//实例化串口对象
}

SerialPort::~SerialPort()
{
    if(m_serial->isOpen())
    {
        m_serial->close();
    }
}

//刷新串口列表
QStringList SerialPort::GetListPortName()
{

    m_listPortName.clear();

    foreach(const QSerialPortInfo &info,QSerialPortInfo::availablePorts())
    {
        m_listPortName << info.portName();
    }

    return m_listPortName;
}

// 串口是否打开
bool SerialPort::IsOpen()
{
    return m_serial->isOpen();
}

bool SerialPort::Open(std::string portName, int baud)
{

    QMutex mutex;
    QMutexLocker m_lock(&mutex);

    if(IsOpen())
    {
        Close();
    }

    m_baud = baud;//设置波特率
    m_portName = portName;//设置串口号
    m_serial->setPortName(portName.c_str());//设置串口号
    m_serial->setBaudRate(baud, QSerialPort::AllDirections);//设置波特率和读写方向
    m_serial->setDataBits(QSerialPort::Data8);//数据位为8位
    m_serial->setFlowControl(QSerialPort::NoFlowControl);//无流控制
    m_serial->setParity(QSerialPort::NoParity);//无校验位
    m_serial->setStopBits(QSerialPort::OneStop);//一位停止位
    m_serial->open(QIODevice::ReadWrite);
    if(!IsOpen())
    {
        qDebug()<< portName.c_str() <<" err:open failed!";

        return false;
    }

    qDebug()<< portName.c_str() <<"open Success!";

    return true;
}

void SerialPort::Close()
{
    if (m_serial->isOpen())
    {
        m_serial->clear();
        m_serial->close();
    }
}

int SerialPort::Recv(unsigned char *data, size_t length)
{
    int count = -1;

    if(data == nullptr || length == 0)
    {
        return 0;// 如何接收缓存无效，直接返回0
    }

    if(!IsOpen())
    {
        Open(m_portName, m_baud);
        if(!IsOpen())
        {
            qDebug()<<m_portName.c_str()<<"err:open failed!";
            return -1;
        }
    }

    count = m_serial->read((char *)data, length);

    m_serial->waitForReadyRead(50);

    return count;
}

int SerialPort::Send(unsigned char *data, size_t length)
{
    int count = 0;

    if(data == nullptr || length == 0)
    {
        return 0;// 如何接收缓存无效，直接返回0
    }

    if(!IsOpen())
    {
        Open(m_portName);
        if(!IsOpen())
        {
            qDebug()<<m_portName.c_str()<<"err:open failed!";
            return -1;
        }
    }

    count = m_serial->write((char *)data, length);

    m_serial->waitForBytesWritten(10000);//等待数据写入完成

    return count;
}

int SerialPort::ReadAvailableCout()
{
    return m_serial->bytesAvailable();
}

bool SerialPort::WaitForRead(int time)
{
    return m_serial->waitForReadyRead(time);
}
