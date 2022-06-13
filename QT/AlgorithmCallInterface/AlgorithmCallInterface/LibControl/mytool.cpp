#include "mytool.h"
#include <QTime>
#include <QCoreApplication>
#include <QDebug>

myTool::myTool()
{

}

void myTool::Sleep(int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 2);
}

byte* myTool::floatToByte(float value,byte buf[],int buf_len)
{
    qDebug()<<"floatToByte:"<<value;
    //转换float数据到字节数组
    byte* pValue = (byte*)&value;

    for(int i=0;i<buf_len;i++)
    {
        buf[i] = *pValue++; //把相应地址中的数据保存到unsigned char数组中
        qDebug()<<"buf"<<i<<"="<<buf[i];
    }

    return buf;
}

byte* myTool::intToByte(int value,byte buf[],int buf_len)
{
    qDebug()<<"intToByte:"<<value;
    //转换float数据到字节数组
    byte* pValue = (byte*)&value;

    for(int i=0;i<buf_len;i++)
    {
        buf[i] = *pValue++; //把相应地址中的数据保存到unsigned char数组中
        qDebug()<<"buf"<<i<<"="<<buf[i];
    }

    return buf;
}

byte* myTool::shortToByte(short value,byte buf[],int buf_len)
{
    {
        qDebug()<<"shortToByte:"<<value;
        //转换float数据到字节数组
        byte* pValue = (byte*)&value;

        for(int i=0;i<buf_len;i++)
        {
            buf[i] = *pValue++; //把相应地址中的数据保存到unsigned char数组中
            qDebug()<<"buf"<<i<<"="<<buf[i];
        }

        return buf;
    }
}

int myTool::byteToInt(byte* byteBuf)
{
    int var=*((int*)byteBuf);

    //qDebug()<<"CTool::byteToValue var="<<var;

    return var;
}
