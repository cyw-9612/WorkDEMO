#ifndef BINARYUTIL_H
#define BINARYUTIL_H

#include <QByteArray>
#include <QString>
#include "base_global.h"

class LIBBASE_EXPORT BinaryUtil
{
public:
    static QByteArray HexStringToByteArray(QString HexString);
    static QString ByteArrayToHexString(char *buffer, int bufferLength);
    static QString ByteArrayToHexString(QByteArray &ba);
};

#endif // BINARYUTIL_H
