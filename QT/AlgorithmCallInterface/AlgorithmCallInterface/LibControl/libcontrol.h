#ifndef LIBCONTROL_H
#define LIBCONTROL_H

#include "LibControl_global.h"
#include <QString>

//AES密匙和偏移量
static const QByteArray AES_KEY = QString("awa147369abcdefa").toUtf8();
static const QByteArray AES_IV = QString("0102030405147369").toUtf8();

class LIBCONTROL_EXPORT LibControl
{
public:
    LibControl();

    //服务器信息
    static QString ipServerx;
    static int portServerx;
    static QString currentDayString;
    //用户电脑本机信息
    static QString localIP;
    static QString MAC;
    static QString CPUID;
    static QString BIOS;
    //序列号窗口类型 0:软件第一次启动 1:软件使用时长不足
    static int serialWidgetType;
    //RSA密匙
    static QString strPriKey;
};

#endif // LIBCONTROL_H
