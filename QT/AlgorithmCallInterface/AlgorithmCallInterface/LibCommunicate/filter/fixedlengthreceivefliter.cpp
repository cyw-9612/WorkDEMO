#include "fixedlengthreceivefliter.h"

FixedLengthReceiveFliter::FixedLengthReceiveFliter(int length)
{
    _fixLength = length;
}


std::list<Buffer *> *FixedLengthReceiveFliter::Filter(Buffer *receiveBuffer, int offset, int length, int &lastByteOffset)
{
    if (length >= _fixLength)
    {
        int num = (int)(length / _fixLength);//计算有效包数量
        std::list<Buffer* > *listBytes=new std::list<Buffer* >();

        /*
         * 分包处理
         */
        for (int i = 0; i < num; i++)
        {
            Buffer *buffer = new Buffer(_fixLength, 0);
            buffer->Write(receiveBuffer->data() + i * _fixLength, _fixLength);
            listBytes->push_back(buffer);
            lastByteOffset = offset + ((i+1)*_fixLength);//设置下一个数据包开始查找位置
        }

        return listBytes;
    }
    else
    {
        return nullptr;
    }
}

void FixedLengthReceiveFliter::setFilterClosed(bool isClosed)
{
    isCloseFilter = isClosed;
}
