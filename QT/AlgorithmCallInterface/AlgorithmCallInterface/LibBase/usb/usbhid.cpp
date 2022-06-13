#include <QString>
#include "usbhid.h"

UsbHid::UsbHid()
{
    m_handle = nullptr;
    m_isOpen = false;
}

bool UsbHid::Open(int vid, int pid)
{
    if(m_isOpen)// 已打开，先关闭
    {
        Close();
        m_isOpen = false;
    }

    m_handle = hid_open(vid, pid, nullptr);// 根据vid和pid打开设备
    if(m_handle != nullptr)
    {
        m_isOpen = true;
    }

    return m_isOpen;
}

void UsbHid::Close()
{
    if(m_handle != nullptr)// 判断设备句柄是否有效
    {
        hid_close(m_handle);
        m_isOpen = false;
        m_handle = nullptr;
    }
}

bool UsbHid::IsOpen()
{
    return m_isOpen;
}

int UsbHid::Recv(unsigned char *data, size_t length)
{
    int count = -1;

    if(data == nullptr || length == 0)
    {
        return 0;// 如何接收缓存无效，直接返回0
    }

    if(m_isOpen && m_handle != nullptr)
    {
        count = hid_read(m_handle, data, length);
    }

    return count;
}

int UsbHid::Send(unsigned char *data, size_t length)
{
    int count = -1;

    if(data == nullptr || length == 0)
    {
        return 0;// 如何待发送数据无效，直接返回0
    }

    if(m_isOpen && m_handle != nullptr)
    {
        count = hid_write(m_handle, data, length);
    }

    return count;
}

std::string UsbHid::GetLastError()
{
    const wchar_t *err = nullptr;
    std::string result = "";

    if(m_handle != nullptr)
    {
        err = hid_error(m_handle);
        QString temp= QString::fromWCharArray(err);
        result = temp.toStdString();
    }

    return result;
}
