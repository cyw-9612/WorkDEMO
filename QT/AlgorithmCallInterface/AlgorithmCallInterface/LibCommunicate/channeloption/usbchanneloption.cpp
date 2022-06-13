#include "usbchanneloption.h"

UsbChannelOption::UsbChannelOption()
{

}

std::string UsbChannelOption::Pid() const
{
    return m_pid;
}

void UsbChannelOption::SetPid(const std::string &pid)
{
    m_pid = pid;
}

std::string UsbChannelOption::Vid() const
{
    return m_vid;
}

void UsbChannelOption::SetVid(const std::string &vid)
{
    m_vid = vid;
}
