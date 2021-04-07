﻿

#include "work.h"

#include "../utils/byteutil.h"


Work::Work()
{
    failed_times = 0;
}

bool Work::CheckResponse(quint8 *return_pkg) {
    //此处比对返回值 data.at(4)是去掉包头和数据长度的指令类型，此处包头只有3位，数据长度1位
    quint8 src_pkg_bit_1 = ByteUtil::Char2Byte(data.at(4));//command type
    quint8 src_pkg_bit_2 = ByteUtil::Char2Byte(data.at(5));//command code

   if (return_pkg[0] == src_pkg_bit_1 && return_pkg[1] == src_pkg_bit_2) {
       return true;
   } else {
       return false;
   }
}



