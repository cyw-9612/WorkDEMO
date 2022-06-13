#ifndef CFILE_H
#define CFILE_H

#include "LibControl_global.h"
#include <QString>
#include <QSettings>
#include<QDebug>
#include <QCoreApplication>

/**********************************************
 *
 *  功 能: 文件操作类
 *
 *  编 制: 柯鹏飞
 *
 *  日 期: 2020/12/02
 *
 **********************************************/

class LIBCONTROL_EXPORT CFile
{
public:
    CFile();

    /**
     * @brief 判断路径是否存在,不存在则创建
     * @param fullPath
     * @return true:存在或创建成功,false:不存在或创建失败
     */
    static bool isDirExist(QString fullPath);

    /**
     * @brief 写入Ini文件
     * @param filePath: 文件路径
     * @param key:ini键值
     * @param value:待写的值
     */
    static void writeIni(QString filePath,QString key,QString value);

    /**
     * @brief 读取ini文件
     * @param filePath: 文件路径
     * @param key:ini键值
     * @return 读到的字符串
     */
    static QString readIni(QString filePath,QString key);

    /**
     * @brief 获取给定名称的ini文件全路径
     * @param strName:文件名[不含扩展名]
     * @return ini文件全路径
     */
    static QString getIniPath(QString fileName);
};

#endif // CFILE_H
