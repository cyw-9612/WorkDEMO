/*************************************************
  Copyright (C), 2020-2021, AIHua Tech. Co., Ltd.
  File name:    ChannelOption.h
  Author:       yangchun
  Version:      0.0.01
  Date:         2021-4-6
  Description:  通道参数
  History:
*************************************************/
#ifndef ICHANNELOPTION_H
#define ICHANNELOPTION_H

#include "comchanneloption.h"
#include "netchanneloption.h"
#include "usbchanneloption.h"
#include "LibCommunicate_global.h"

class LIBCOMMUNICATE_EXPORT ChannelOption
{
public:
    ChannelOption();

    ComChannelOption ComOption() const;// 获取串口参数
    void SetComOption(const ComChannelOption &ComOption);// 设置串口参数

    NetChannelOption NetOption() const;// 获取网口参数
    void SetNetOption(const NetChannelOption &NetOption);// 设置网口参数

    UsbChannelOption UsbOption() const;// 获取USB参数
    void SetUsbOption(const UsbChannelOption &UsbOption);// 设置USB参数

private:
    ComChannelOption m_comOption;// 串口参数
    NetChannelOption m_netOption;// 网口参数
    UsbChannelOption m_usbOption;// usb参数
};

#endif // ICHANNELOPTION_H
