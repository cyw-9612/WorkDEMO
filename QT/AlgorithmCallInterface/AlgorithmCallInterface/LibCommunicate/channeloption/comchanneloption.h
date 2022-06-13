/*************************************************
  Copyright (C), 2020-2021, AIHua Tech. Co., Ltd.
  File name:    ChannelOption.h
  Author:       yangchun
  Version:      0.0.01
  Date:         2021-4-6
  Description:  串口通道参数
  History:
*************************************************/
#ifndef COMCHANNELOPTION_H
#define COMCHANNELOPTION_H

#include <string>
#include "LibCommunicate_global.h"

class LIBCOMMUNICATE_EXPORT ComChannelOption
{
public:
    ComChannelOption();

    std::string Com() const;// 获取串口号
    void SetCom(const std::string &com);// 设置串口号

    int Baud() const;// 获取波特率
    void SetBaud(int baud);// 设置波特率

    int DataBit() const;// 获取数据位
    void SetDataBit(int dataBit);// 设置数据位

    int FlowControl() const;// 获取流控制
    void SetFlowControl(int flowControl);// 设置流控制

    int Parity() const;// 获取校验位
    void SetParity(int parity);// 设置校验位

    int StopBits() const;// 获取停止位
    void SetStopBits(int stopBits);// 设置停止位

private:
    std::string m_com;// 串口号
    int m_baud;// 波特率
    int m_dataBit;// 数据位为
    int m_flowControl;// 流控制
    int m_parity;// 校验位
    int m_stopBits;// 停止位
};

#endif // COMCHANNELOPTION_H
