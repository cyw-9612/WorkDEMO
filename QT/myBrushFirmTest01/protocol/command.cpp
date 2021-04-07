
#include "command.h"

#include "../utils/byteutil.h"


Command::Command()
{

}

Command::~Command()
{

}

//打包函数
QByteArray* Command::Pack(QByteArray &data) {
    QByteArray *ret = new QByteArray();

    //计算CRC
    quint16 crc;
    Crc::GetCRC(reinterpret_cast<quint8*>(data.data()), data.length(), crc);
    quint8 high_byte = (crc >> 8) & 0xff;   //CRC高8位
    quint8 low_byte = crc & 0xff;   //CRC低8位

    char head_char_array[sizeof(head)];
    ByteUtil::ByteArray2CharArray(head, head_char_array, sizeof(head));

    char tail_char_array[sizeof(tail)];
    ByteUtil::ByteArray2CharArray(tail, tail_char_array, sizeof(tail));

    //一个包的数据：包头+数据长度1位+数据+CRC校验位2位+包尾
    ret->append(head_char_array, sizeof(head_char_array));
    ret->append(data);
    ret->append(ByteUtil::Byte2Char(high_byte));
    ret->append(ByteUtil::Byte2Char(low_byte));
    ret->append(tail_char_array, sizeof(tail_char_array));

    int head_len = sizeof(head);
    int tail_len = sizeof(tail);

    //计算数据长度，总长减去包头，包尾长再加自己长度一位
    int pkg_len = ret->length() - head_len - tail_len + 1;
    ret->insert(head_len, ByteUtil::Int2Char(pkg_len));

/*
    quint8 *dst =  new quint8[ret->length()];
    ByteUtil::CharArray2ByteArray(ret->data(), dst, ret->length());
    for(int a = 0; a < ret->length(); a ++) {
        printf("%d dst hex=%02x \n", a,dst[a]);
    }
    delete[] dst;
    */
    return ret;
}

