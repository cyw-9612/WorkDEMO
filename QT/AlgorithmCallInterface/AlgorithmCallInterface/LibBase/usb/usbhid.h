/*************************************************
  Copyright (C), 2020-2021, AIHua Tech. Co., Ltd.
  File name:    usbhid.h
  Author:       yangchun
  Version:      0.0.01
  Date:         2020-6-1
  Description:  usb hid类
  History:
*************************************************/
#ifndef USBHID_H
#define USBHID_H

#include <string>
#include "hidapi.h"
#include "base_global.h"

class LIBBASE_EXPORT  UsbHid
{
public:
    UsbHid();

    bool Open(int vid, int pid);// 打开usb设备
    void Close();// 关闭usb设备
    bool IsOpen();// 设备是否打开
    int  Recv(unsigned char *data, size_t length);// 接收数据
    int  Send(unsigned char *data, size_t length);// 发送数据
    std::string GetLastError();// 获取错误信息

private:
    hid_device *m_handle;// 设备句柄
    bool m_isOpen;// 是否打开
};

#endif // USBHID_H
