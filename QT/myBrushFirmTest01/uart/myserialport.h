﻿#ifndef MYSERIALPORT_H
#define MYSERIALPORT_H

#include <QSerialPort>
#include <QSerialPortInfo>
#include "../protocol/comm_interface.h"

class MySerialPort : public CommInterface
{
    Q_OBJECT
public:
    MySerialPort();
    ~MySerialPort() override;

   public:
       static QList<QSerialPortInfo> GetAvailablePorts();
       bool Open() override;
       void Close() override;
       bool IsOpen() override;
       qint64 Send(const QByteArray &data) override;

       //设置串口名称
       void SetPortName(const QString &name);
       void SetPort(const QSerialPortInfo &serialPortInfo);
       QString GetPortName();

       //设置波特率
       bool SetBaudRate(qint32 baudRate);
       static QList<QString> GetBaudRateStrList();
       static qint32 GetBaudRateValueByIndex(int index);

       //设置数据位
       bool SetDataBits(QSerialPort::DataBits dataBits);
       static QList<QString> GetDatabitsStrList();
       static QSerialPort::DataBits GetDatabitsValueByIndex(int index);

       //设置检验位
       bool SetParity(QSerialPort::Parity parity);
       static QList<QString> GetParityStrList();
       static QSerialPort::Parity GetParityValueByIndex(int index);

       //设置停止位
       bool SetStopBits(QSerialPort::StopBits stopBits);
       static QList<QString> GetStopbitsStrList();
       static QSerialPort::StopBits GetStopbitsValueByIndex(int index);

       //扫描可用串口
       QStringList scanSerial();


private slots:
       void ReadyRead();
private:

       QSerialPort *serial_port_p;
       QSerialPortInfo port_info_;

       const static QList<QString> baudrate_str_list_;
       const static QList<QString> databits_str_list_;
       const static QList<QString> parity_str_list_;
       const static QList<QString> stopbits_str_list_;

};

#endif // MYSERIALPORT_H
