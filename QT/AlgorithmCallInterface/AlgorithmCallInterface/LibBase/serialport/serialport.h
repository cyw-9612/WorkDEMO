/*************************************************
  Copyright (C), 2020-2021, AIHua Tech. Co., Ltd.
  File name:    serialport.h
  Author:       yangchun
  Version:      0.0.01
  Date:         2020-6-1
  Description:  串口类
  History:
*************************************************/
#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <QObject>
#include <QSerialPort>
#include <QTimer>
#include <QThread>
#include "memory/buffer.h"
#include "base_global.h"

class LIBBASE_EXPORT SerialPort: public QObject
{
    Q_OBJECT

public:
    SerialPort(QObject *parent = nullptr);
    ~SerialPort();

public:
    bool IsOpen(); //检测串口是否打开
    bool Open(std::string portName, int baud = 115200);//打开指定串口
    void Close();// 关闭串口
    int Recv(unsigned char *data, size_t length);// 接收数据
    int Send(unsigned char *data, size_t length);// 发送数据
    int ReadAvailableCout();//可读字节数
    bool WaitForRead(int time);// 等待读数据
    QStringList GetListPortName();// 获取串口列表

private:
    QSerialPort *m_serial;// Qt串口类指针
    QStringList m_listPortName;// 可用串口列表
    std::string m_portName;// 当前串口号
    int m_baud;// 波特率
};

#endif // SERIALPORT_H
