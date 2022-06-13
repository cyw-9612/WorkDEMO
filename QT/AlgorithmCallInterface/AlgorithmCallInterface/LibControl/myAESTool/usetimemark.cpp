#include "usetimemark.h"

#include <QFileInfo>
#include <QCoreApplication>
#include <QDebug>
#include <QTextCodec>
#include "qaesencryption.h"
#include "libcontrol.h"

useTimeMark::useTimeMark()
{
    //获取配置文件信息
    m_psetting = new QSettings("translations/serverAddress.ini", QSettings::IniFormat);

    readAuthHistoryFile();
}

useTimeMark::~useTimeMark()
{
    //记录使用时间
    saveAuthHistory();

    if(m_psetting !=nullptr)
    {
        delete m_psetting;
        m_psetting=nullptr;
    }
    qDebug() << "~useTimeMark被调用";
}

/*写入使用时间**/
void useTimeMark::writeTime(int day)
{
    //按照时间一天算24小时
    qint64 secs = day * 24 * 60 * 60;
    if(secs < 0)
        secs = 0;

    if(secs >= lastTime && 0 != lastTime && day != 1000)
    {
        //时间写小不写大
        return;
    }
    qDebug() <<"原有时间：" << lastTime << "写入剩余使用时间还有：" << secs << "秒";

    //加密数据
    QByteArray byteArrFull = QString("%1").arg(secs).toUtf8();
    //qDebug() <<byteArrFull;
   QByteArray AES_TEXT= QAESEncryption::Crypt(QAESEncryption::AES_128, QAESEncryption::CBC, byteArrFull,
                                              AES_KEY, AES_IV, QAESEncryption::ZERO).toBase64();
    //写入剩余时间
    //SetConfigData("limitDay", "third", QString::fromLatin1(AES_TEXT.toBase64()));
    m_psetting->setValue("/limitDay/third", QString::fromLatin1(AES_TEXT));
    //刷新软件开启时间
    startTime = QDateTime::currentSecsSinceEpoch();
    lastTime = secs;
}

//更新使用时间
void useTimeMark::switchTime(int limieDay,int sec)
{
    //按照时间一天算24小时
    qint64 limieSecs = limieDay * 24 * 60 * 60;
    limieSecs = limieSecs - sec;

    if(limieSecs > lastTime || 0 == lastTime)
    {
        return;
    }

    qDebug() <<"原有时间：" << lastTime << "写入剩余使用时间还有：" << limieSecs << "秒";
    //加密数据
    QByteArray byteArrFull = QString("%1").arg(limieSecs).toUtf8();
    //qDebug() <<byteArrFull;
   QByteArray AES_TEXT= QAESEncryption::Crypt(QAESEncryption::AES_128, QAESEncryption::CBC, byteArrFull,
                                              AES_KEY, AES_IV, QAESEncryption::ZERO).toBase64();
    //写入剩余时间
    //SetConfigData("limitDay", "third", QString::fromLatin1(AES_TEXT.toBase64()));
    m_psetting->setValue("/limitDay/third", QString::fromLatin1(AES_TEXT));
    //刷新软件开启时间
    startTime = QDateTime::currentSecsSinceEpoch();
    lastTime = limieSecs;
}

/**读取本地文件*/
void useTimeMark::readAuthHistoryFile()
{
    //记录软件开启时间
    startTime = QDateTime::currentSecsSinceEpoch();

    QString AES_TEXT = GetConfigData("limitDay", "third").toString();
    //qDebug() << AES_TEXT;
    QByteArray enCode = QByteArray::fromBase64(AES_TEXT.toLatin1());
    QByteArray buffer;

    //解密
    buffer = QAESEncryption::Decrypt(QAESEncryption::AES_128, QAESEncryption::CBC, enCode,
                                                     AES_KEY, AES_IV, QAESEncryption::ZERO);
    //QString limitTime;
    //limitTime = QTextCodec::codecForName("UTF-8")->toUnicode(buffer);
    lastTime = buffer.toInt();
    qDebug() << "读取剩余使用时间还有：" << lastTime << "秒";
}

/**记录历史到文件*/
void useTimeMark::saveAuthHistory()
{
    //记录软件关闭时间
    endTime = QDateTime::currentSecsSinceEpoch();

    qDebug() << "原有时间：" << lastTime << "开始"  << startTime << "结束" << endTime;

    qint64 difTime;
    if(endTime > startTime)
    {
        difTime = endTime - startTime;
    }
    else
    {
        difTime = startTime - endTime;
    }

    if(lastTime > difTime)
    {
        lastTime = lastTime - difTime;
    }
    else
    {
        lastTime = 0;
    }

    qDebug() << "写入剩余使用时间还有：" << lastTime << "秒";

    //加密数据
    QByteArray byteArrFull = QString("%1").arg(lastTime).toUtf8();
    //qDebug() <<byteArrFull;
   QByteArray AES_TEXT= QAESEncryption::Crypt(QAESEncryption::AES_128, QAESEncryption::CBC, byteArrFull,
                                              AES_KEY, AES_IV, QAESEncryption::ZERO).toBase64();
    //写入剩余时间
    //SetConfigData("limitDay", "third", QString::fromLatin1(AES_TEXT.toBase64()));
    m_psetting->setValue("/limitDay/third", QString::fromLatin1(AES_TEXT));

    //更新开始时间
    startTime = QDateTime::currentSecsSinceEpoch();
}

//设置配置文件
void useTimeMark::SetConfigData(QString qstrnodename, QString qstrkeyname, QVariant qvarvalue)
{
    if (m_psetting)
    {
        m_psetting->setValue(QString("/%1/%2").arg(qstrnodename).arg(qstrkeyname), qvarvalue);
    }
}

//读配置文件
QVariant useTimeMark::GetConfigData(QString qstrnodename, QString qstrkeyname)
{
    QVariant qvar = -1;
    if (m_psetting)
    {
        qvar = m_psetting->value(QString("/%1/%2").arg(qstrnodename).arg(qstrkeyname));
    }
    return qvar;
}

//软件是否还有使用时间
bool useTimeMark::isHaveTime()
{
    qDebug() << "开始检测软件是否还有使用时间";
    saveAuthHistory();
    //qDebug() << "是否超时：" << lastTime ;
    return (lastTime > 0) ;
}
