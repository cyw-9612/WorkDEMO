﻿

#include "crc.h"


Crc::Crc()
{

}

Crc::~Crc()
{

}

void Crc::GetCRC(quint8 *data, int len, quint16 &crc) {
    quint16 i,j,carry_flag, temp;

    crc = 0xffff;
    for (i = 0; i < len; i++) {
        crc = crc ^ data[i];
        for (j = 0; j < 8; j ++) {
            temp = crc;
            carry_flag = temp & 0x0001;
            crc = crc >> 1;

            if (carry_flag == 1) {
                 crc = crc ^ 0xa001;
            }
         }
    }
}


bool Crc::CheckCRC(quint8 *data, int len) {
    if (len < 2) return false;

    /*
    printf("------------CheckCRC-----------------\n");
    for(int a = 0; a < len; a ++) {
        printf("%d dst 333  hex=%02x \n", a, data[a]);
    }*/

    quint16 crcLow = data[len - 1];
    quint16 crcHigh = data[len - 2];
    quint16 crc_received = static_cast<quint16>(crcHigh << 8 | crcLow);
    quint16 crc_new;

    GetCRC(data, len - 2, crc_new);

    if (crc_new == crc_received) {
        return true;
    }

    return false;
}
