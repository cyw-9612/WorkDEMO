#ifndef CLIENTTCP_H
#define CLIENTTCP_H
#include <QObject>
#include <QTcpSocket>
#include "memory/buffer.h"
#include "base_global.h"

/***********************************
 *
 * 功 能: TCP通信类
 *
 * 编 制: 陈宇葳
 *
 * 日 期: 2020/06/11
 *
 **********************************/

class LIBBASE_EXPORT clientTCP: public QObject
{
   Q_OBJECT

public:
    clientTCP();
    ~clientTCP();

    bool Open(std::string ip,int port);// 打开TCP客户端
    void Close();// 关闭TCP客户端
    bool IsOpen();// TCP是否打开
    int  Recv(unsigned char *data, size_t length);// 接收数据
    int  Send(unsigned char *data, size_t length);// 发送数据

private:
    //连接服务器 参数:ip 地址,port 端口 默认6666，返回值：true 成功,false失败
    bool connectServer(QString ip,int port=6666);

    //断开连接槽函数
    void disconnectSocket();

private:
    /**创建客户端对象*/
    QTcpSocket *m_pSocket = nullptr;
    bool m_isOpen;// 是否打开

    /**目标服务器地址*/
    std::string m_ip;

    /**端口*/
    int m_port;
};

#endif // CLIENTTCP_H
