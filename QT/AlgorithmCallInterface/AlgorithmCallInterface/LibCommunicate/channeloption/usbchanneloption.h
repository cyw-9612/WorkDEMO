/*************************************************
  Copyright (C), 2020-2021, AIHua Tech. Co., Ltd.
  File name:    ChannelOption.h
  Author:       yangchun
  Version:      0.0.01
  Date:         2021-4-6
  Description:  USB通道参数
  History:
*************************************************/
#ifndef USBCHANNELOPTION_H
#define USBCHANNELOPTION_H

#include <string>
#include "LibCommunicate_global.h"

class LIBCOMMUNICATE_EXPORT UsbChannelOption
{
public:
    UsbChannelOption();

    std::string Pid() const;
    void SetPid(const std::string &Pid);

    std::string Vid() const;
    void SetVid(const std::string &Vid);

private:
    std::string m_pid;
    std::string m_vid;
};

#endif // USBCHANNELOPTION_H
