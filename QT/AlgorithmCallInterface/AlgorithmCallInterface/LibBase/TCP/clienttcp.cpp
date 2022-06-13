#include "clienttcp.h"

clientTCP::clientTCP()
{
    //初始化客户端对象
    m_pSocket = new QTcpSocket();
    m_isOpen = false;
}

clientTCP::~clientTCP()
{
    if(m_pSocket != nullptr)
    {
        delete m_pSocket;
        m_pSocket = nullptr;
    }
}

// 打开TCP客户端
bool clientTCP::Open(std::string ip,int port)
{
    //记录信息
    m_ip = ip;
    m_port = port;

    QString strIp = QString::fromStdString(m_ip);
    return connectServer(strIp,m_port);
}

// 关闭TCP客户端
void clientTCP::Close()
{
    disconnectSocket();
}

// TCP是否打开
bool clientTCP::IsOpen()
{
    return m_isOpen;
}

// 接收数据
int  clientTCP::Recv(unsigned char *data, size_t length)
{
    int count = -1;

    if(data == nullptr || length == 0)
    {
        return 0;// 如何接收缓存无效，直接返回0
    }

    if(!IsOpen())
    {
        Open(m_ip,m_port);
        if(!IsOpen())
        {
            qDebug()<<"m_pSocket.c_str err:open failed!";
            return -1;
        }

    }

    count = m_pSocket->read((char *)data, length);

    m_pSocket->waitForReadyRead(50);

//    if(count>0)
//    {
//        qDebug() << data;
//    }

    return count;
}

// 发送数据
int  clientTCP::Send(unsigned char *data, size_t length)
{
    int count = 0;

    if(data == nullptr || length == 0)
    {
        return 0;// 如何接收缓存无效，直接返回0
    }

    if(!IsOpen())
    {
        Open(m_ip, m_port);
        if(!IsOpen())
        {
            qDebug()<<m_pSocket->errorString()<<"err:open failed!";
            return -1;
        }
    }

    //将数据转换类型
    QByteArray byteHex((char*)data, length);

    //发送指令
    count = m_pSocket->write(byteHex);

    bool BoolFlush = m_pSocket->flush();
    if(count!=-1 && BoolFlush==1)
    {
        //qDebug().noquote() <<"clientTCP::Send byteHex:"<<byteHex;
    }
    else
    {
        qDebug()<<"send failed!"<<endl;
    }

    return count;
}

/**连接服务器 参数:ip 地址,port 端口 默认6666，返回值：true 成功,false失败*/
bool clientTCP::connectServer(QString ip,int port)
{
    m_pSocket->abort(); //取消已有连接
    m_pSocket->connectToHost(ip,port);  //连接服务器

    if(!m_pSocket->waitForConnected(3000))
    {
        qDebug()<<tr("连接服务器失败，请重新操作！");
        m_isOpen = false;
        //QMessageBox::warning(this,tr("连接提示"),tr("连接服务器失败！"));
        return m_isOpen;
    }
    else
    {
        qDebug()<<tr("连接服务器成功！");
        m_isOpen = true;
        return m_isOpen;
        //QMessageBox::warning(this,tr("连接提示"),tr("连接服务器成功！"));
    }
}

/**断开连接槽函数*/
void clientTCP::disconnectSocket()
{
    m_pSocket->abort(); //取消已有连接
    m_isOpen = false;
}
