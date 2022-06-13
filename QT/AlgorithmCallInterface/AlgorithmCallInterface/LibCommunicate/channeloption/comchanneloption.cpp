#include <QSerialPort>
#include "comchanneloption.h"

ComChannelOption::ComChannelOption()
{
    m_baud = 115200;// 波特率
    m_dataBit = 8;// 数据位为8位
    m_flowControl = QSerialPort::NoFlowControl;// 无流控制
    m_parity = 0;// 校验位
    m_stopBits = 1;// 停止位
}

std::string ComChannelOption::Com() const
{
    return m_com;
}

void ComChannelOption::SetCom(const std::string &com)
{
    m_com = com;
}

int ComChannelOption::Baud() const
{
    return m_baud;
}

void ComChannelOption::SetBaud(int baud)
{
    m_baud = baud;
}

int ComChannelOption::DataBit() const
{
    return m_dataBit;
}

void ComChannelOption::SetDataBit(int dataBit)
{
    m_dataBit = dataBit;
}

int ComChannelOption::FlowControl() const
{
    return m_flowControl;
}

void ComChannelOption::SetFlowControl(int flowControl)
{
    m_flowControl = flowControl;
}

int ComChannelOption::Parity() const
{
    return m_parity;
}

void ComChannelOption::SetParity(int parity)
{
    m_parity = parity;
}

int ComChannelOption::StopBits() const
{
    return m_stopBits;
}

void ComChannelOption::SetStopBits(int stopBits)
{
    m_stopBits = stopBits;
}
