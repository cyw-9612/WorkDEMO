#include "channeloption.h"

ChannelOption::ChannelOption()
{

}

ComChannelOption ChannelOption::ComOption() const
{
    return m_comOption;
}

void ChannelOption::SetComOption(const ComChannelOption &option)
{
    m_comOption = option;
}

NetChannelOption ChannelOption::NetOption() const
{
    return m_netOption;
}

void ChannelOption::SetNetOption(const NetChannelOption &option)
{
    m_netOption = option;
}

UsbChannelOption ChannelOption::UsbOption() const
{
    return m_usbOption;
}

void ChannelOption::SetUsbOption(const UsbChannelOption &option)
{
    m_usbOption = option;
}
