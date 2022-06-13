#ifndef MYTOOL_H
#define MYTOOL_H
#include "LibControl_global.h"

/***********************************
 *
 * 功 能: 自定义工具类
 *
 * 编 制: 陈宇葳
 *
 * 日 期: 2020/04/12
 *
 **********************************/

/**
 * @brief 定义unsigned char为byte
 */
typedef unsigned char byte;

class LIBCONTROL_EXPORT myTool
{
public:
    myTool();

    /**
     * @brief 延迟方法
     * @param msec:延迟毫秒数
     * @note
     */
    static void Sleep(int msec);

    /**
     * @brief float转byte数组
     * @param value 数值
     * @param buf 缓冲区
     * @param buf_len 缓冲区长度,默认4字节
     * return 无
     */
    static byte* floatToByte(float value,byte buf[],int buf_len=4);

    /**
     * @brief int转byte数组
     * @param value 数值
     * @param buf 缓冲区
     * @param buf_len 缓冲区长度,默认4字节
     * return 无
     */
    static byte* intToByte(int value,byte buf[],int buf_len=4);

    /**
     * @brief short转byte数组
     * @param value 数值
     * @param buf 缓冲区
     * @param buf_len 缓冲区长度,默认4字节
     * return 无
     */
    static byte* shortToByte(short value,byte buf[],int buf_len=2);

    /**
     * @brief byte数组转int数值
     * @param byteBuf 字节数组
     * return 无
     */
    static int byteToInt(byte* byteBuf);
};

#endif // MYTOOL_H
