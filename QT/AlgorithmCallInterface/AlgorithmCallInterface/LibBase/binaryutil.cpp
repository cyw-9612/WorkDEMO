#include <QDataStream>
#include "binaryutil.h"

// 16进制字符串转字节数组
QByteArray BinaryUtil::HexStringToByteArray(QString HexString)
{
    bool ok;
    QByteArray ret;
    HexString = HexString.trimmed();
    HexString = HexString.simplified();
    QStringList sl = HexString.split(" ");

    foreach (QString s, sl)
    {
        if(!s.isEmpty()) {
            char c = s.toInt(&ok,16)&0xFF;
            if(ok){
                ret.append(c);
            }
        }
    }
    return ret;
}

// 字节数组转16进制字符串
QString BinaryUtil::ByteArrayToHexString(QByteArray &ba)
{
    QDataStream out(&ba,QIODevice::ReadWrite);   //将str的数据 读到out里面去
    QString buf;
    while(!out.atEnd())
    {
        qint8 outChar = 0;
        out >> outChar;   //每次一个字节的填充到 outchar
        QString str = QString("%1").arg(outChar&0xFF,2,16,QLatin1Char('0')).toUpper() + QString(" ");   //2 字符宽度
        buf += str;
    }
    return buf;
}

// 字节数组转16进制字符串
QString BinaryUtil::ByteArrayToHexString(char *buffer, int bufferLength)
{
    QByteArray arr(buffer, bufferLength);
    return  ByteArrayToHexString(arr);
}

