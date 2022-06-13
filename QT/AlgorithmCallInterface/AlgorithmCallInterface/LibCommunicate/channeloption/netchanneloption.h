/*************************************************
  Copyright (C), 2020-2021, AIHua Tech. Co., Ltd.
  File name:    ChannelOption.h
  Author:       yangchun
  Version:      0.0.01
  Date:         2021-4-6(编制) 2021-6-11（更新）
  Description:  网口通道参数
  History:
*************************************************/
#ifndef NETCHANNELOPTION_H
#define NETCHANNELOPTION_H
#include <string>
#include "LibCommunicate_global.h"

class LIBCOMMUNICATE_EXPORT NetChannelOption
{
public:
    NetChannelOption();

    std::string IP() const;// 获取服务器IP
    void SetIP(const std::string &ip);// 设置服务器IP

    int Port() const;// 获取端口号
    void SetNetPort(int port);// 设置端口号

private:
    std::string m_IP;// 服务器IP
    int m_port;// 端口号
};

#endif // NETCHANNELOPTION_H
