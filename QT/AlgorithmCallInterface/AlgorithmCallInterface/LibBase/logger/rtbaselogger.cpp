/*************************************************
  Copyright (C), 2020-2021, AIHua Tech. Co., Ltd.
  File name:    rtbaselogger.cpp
  Author:       yangchun
  Version:      0.0.01
  Date:         2020-6-1
  Description:  日志类
  History:
*************************************************/
#include "rtbaselogger.h"
#include <QDebug>

RTBaseLogger::RTBaseLogger()
{

}

/*************************************************
  Function: OpenAudioDevice
  Description: 初始化日志文件
  Input:
            name：日志文件名
  Output:   无
  Return:   无
*************************************************/
void RTBaseLogger::Init(string name)
{
    Q_UNUSED(name);
}

/*************************************************
  Function: OpenAudioDevice
  Description: 写日志文件
  Input:
            logLevel：日志等级
            msg：日志
  Output:   无
  Return:   无
*************************************************/
void RTBaseLogger::LoggerWrite(LoggerLevel_E logLevel, string msg)
{
   Q_UNUSED(logLevel);

   qDebug().noquote() << QString::fromStdString(msg);
}

/*************************************************
  Function: OpenAudioDevice
  Description: 写日志文件
  Input:
            logLevel：日志等级
            msg：日志
  Output:   无
  Return:   无
*************************************************/
void RTBaseLogger::LoggerWrite(LoggerLevel_E logLevel, QString msg)
{
   this->LoggerWrite(logLevel, msg.toStdString());
}

/*************************************************
  Function: OpenAudioDevice
  Description: 写日志文件
  Input:
            logLevel：日志等级
            format：格式化日志，内容不超过2000字节
  Output:   无
  Return:   无
*************************************************/
void RTBaseLogger::LoggerWrite(LoggerLevel_E logLevel, const char *format, ...)
{
    va_list		ap;
    char	str[2000] = {0};

    va_start(ap, format);
    vsprintf(str, format, ap);
    va_end(ap);

    this->LoggerWrite(logLevel, QString(str));
}

/*************************************************
  Function: WriteExpection
  Description: 写异常
  Input:
  Output:   无
  Return:   无
*************************************************/
void RTBaseLogger::WriteExpection(exception &ex)
{
    this->LoggerWrite(LoggerLevel_E::LevelError, QString(ex.what()));
}

/*************************************************
  Function: SetLogLevel
  Description: 设置日志等级
  Input:
            logLevel：日志等级
  Output:   无
  Return:   无
*************************************************/
void RTBaseLogger::SetLogLevel(LoggerLevel_E level)
{
    _loggerLevel = level;
}
