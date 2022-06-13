#include "cfile.h"
#include <QDir>

CFile::CFile()
{

}

bool CFile::isDirExist(QString fullPath)
{
    QDir dir(fullPath);

    //判断路径是否存在
    if(dir.exists())
    {
        //存在,立刻返回
        return true;
    }
    else
    {
        //不存在,则创建并返回创建结果:true 创建成功,false创建失败
        return dir.mkpath(fullPath);
    }
}

void CFile::writeIni(QString filePath,QString key,QString value)
{
    //参考 https://doc.qt.io/qt-5/qsettings.html#details
    //参考 https://blog.csdn.net/gaoenyang760525/article/details/82192205
    //读写Ini参考 https://blog.csdn.net/yaowangII/article/details/78733022

    QSettings *configIni = new QSettings(filePath, QSettings::IniFormat);

    //设置值
    configIni->setValue(key, value);

    delete  configIni;
}

QString CFile::readIni(QString filePath,QString key)
{
    //读写Ini参考 https://blog.csdn.net/yaowangII/article/details/78733022
    QSettings *configIni = new QSettings(filePath, QSettings::IniFormat);

     //获取值
    QString strValue=configIni->value(key).toString();
    qDebug() << strValue;

    delete  configIni;

    return strValue;
}

QString CFile::getIniPath(QString fileName)
{
    //获取程序当前运行目录
    QString strDir = QCoreApplication::applicationDirPath();
    //INI目录
    strDir +="/config";

    //路径存在,返回true;路径不存在,则创建并返回创建结果:true 成功,false失败
    if(CFile::isDirExist(strDir)==false)
    {
        qDebug()<<"create config path failed!";
        return QStringLiteral("");
    }

    //数据库完整路径=路径名+文件名
    QString strIniPath=QString("%1//%2.ini").arg(strDir).arg(fileName);

    return strIniPath;
}
