#include "netchanneloption.h"

NetChannelOption::NetChannelOption()
{
    m_port = 7050;    //常用默认端口
}

std::string NetChannelOption::IP() const
{
    return m_IP;
}

void NetChannelOption::SetIP(const std::string &ip)
{
    m_IP = ip;
}

int NetChannelOption::Port() const
{
    return m_port;
}

void NetChannelOption::SetNetPort(int port)
{
    m_port = port;
}
