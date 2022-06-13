#include "fixheadandlengthreceivefilter.h"

FixHeadAndLengthReceiveFilter::FixHeadAndLengthReceiveFilter(const char *headBytes, int length)
{
    m_headBytes = headBytes;
    m_fixLength = length;
}

std::list<Buffer *> *FixHeadAndLengthReceiveFilter::Filter(Buffer *receiveBuffer, int offset, int length, int &lastByteOffset)
{
    std::list<Buffer* > *listBytes = nullptr;
    int available = length;
    int headLen = strlen(m_headBytes);

    if (available >= m_fixLength)
    {
        int curMaxIndex = available + offset - headLen;
        int loopIndex = offset;
        listBytes = new std::list<Buffer* >();

        while (loopIndex <= curMaxIndex)
        {
            if (receiveBuffer->findMark(offset, length, loopIndex, m_headBytes))
            {
                if(loopIndex + m_fixLength > offset + length)
                {
                    break;// 数据不足，退出循环
                }

                Buffer *buffer = new Buffer(m_fixLength, 0);
                buffer->Write(receiveBuffer->data() + loopIndex, m_fixLength);
                listBytes->push_back(buffer);

                lastByteOffset = loopIndex + m_fixLength;
                loopIndex += m_fixLength;                    // 下标移到截取数据最后数据位的下一个下标
            }
            else
            {
                loopIndex++;
            }
        }
    }

    return listBytes;
}

void FixHeadAndLengthReceiveFilter::setFilterClosed(bool isClosed)
{
    isCloseFilter = isClosed;
}
