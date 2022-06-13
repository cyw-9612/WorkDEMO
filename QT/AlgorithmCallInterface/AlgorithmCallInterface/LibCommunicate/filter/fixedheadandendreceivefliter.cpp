#include "fixedheadandendreceivefliter.h"

FixedHeadAndEndReceiveFliter::FixedHeadAndEndReceiveFliter(char const* headBytes,  char const* endBytes)
{
    m_headIndex = -1;
    m_tailIndex = -1;
    m_headBytes = headBytes;
    m_endBytes = endBytes;
}

std::list<Buffer *> *FixedHeadAndEndReceiveFliter::Filter(Buffer *receiveBuffer, int offset, int length, int &lastByteOffset)
{
    std::list<Buffer* > *listBytes = nullptr;
    int lastIndex = -1;
    int maxOffset = offset + length;
    int loopIndex = offset;
    int headLen = strlen(m_headBytes);
    int tailLen = strlen(m_endBytes);

    m_headIndex = -1;
    m_tailIndex = -1;

    //如果关闭过滤器，则直接返回接收结果
    if(isCloseFilter)
    {
        Buffer *buffer = new Buffer(length, 0);
        if(listBytes == nullptr)
        {
            listBytes = new std::list<Buffer* >();
        }
        buffer->Write(receiveBuffer->data() + lastByteOffset, length);
        listBytes->push_back(buffer);
        lastByteOffset += length;
        return listBytes;
    }

    while (loopIndex < maxOffset)
    {
        if (m_headIndex == -1)/*查找协议头*/
        {
            if (loopIndex <= maxOffset - headLen)
            {
                if (receiveBuffer->findMark(offset, length, loopIndex, m_headBytes))
                {
                    m_headIndex = loopIndex;
                    loopIndex += headLen;
                }
                else
                {
                    loopIndex++;
                }
            }
            else
            {
                loopIndex++;
            }
        }
        else if (m_headIndex >= 0 && m_tailIndex == -1)/*查找协议尾*/
        {
            if (loopIndex <= maxOffset - tailLen)
            {
                if (receiveBuffer->findMark(offset, length, loopIndex, m_endBytes))
                {
                    m_tailIndex = loopIndex;
                    loopIndex += tailLen;
                }
                else
                {
                    loopIndex++;
                }
            }
            else
            {
                loopIndex++;
            }
        }

        if (m_headIndex >= 0 && m_tailIndex > m_headIndex)
        {
            /*
             * 保持有效数据
            */
            int count = m_tailIndex - m_headIndex + tailLen;
            Buffer *buffer = new Buffer(count, 0);
            if(listBytes == nullptr)
            {
                listBytes = new std::list<Buffer* >();
            }

            buffer->Write(receiveBuffer->data() + m_headIndex, count);
            listBytes->push_back(buffer);

            lastIndex = m_tailIndex + tailLen;//设置下一个数据包开始查找位置

            /*
             * 重置协议头尾位置
            */
            m_tailIndex = -1;
            m_headIndex = -1;
        }
    }

    if (lastIndex != -1)
    {
        lastByteOffset = lastIndex;//设置下一个数据包开始查找位置
    }

    return listBytes;
}

void FixedHeadAndEndReceiveFliter::setFilterClosed(bool isClosed)
{
    isCloseFilter = isClosed;
}
