/*************************************************
  Copyright (C), 2020-2021, AIHua Tech. Co., Ltd.
  File name:    logger.cpp
  Author:       yangchun
  Version:      0.0.01
  Date:         2020-6-1
  Description:  日志类
  History:
*************************************************/
#include "rtlogger.h"
#include "log4qt/logger.h"
#include "log4qt/basicconfigurator.h"
#include "log4qt/propertyconfigurator.h"
#include <QDir>
#include <QDebug>

RTLogger::RTLogger()
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
void RTLogger::Init(string name)
{
    //Log4Qt::BasicConfigurator::configure();
    string path(QDir::currentPath().toStdString());

    Log4Qt::PropertyConfigurator::configure(path.append("//log4qt.conf").c_str());
    if (name.length() != 0 && name.empty() == false)// 配置项不为空
    {
        _logger = Log4Qt::Logger::logger(name.c_str());// 使用指定配置文件项
    }
    else
    {
        _logger = Log4Qt::Logger::rootLogger();// 使用根配置项
    }
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
void RTLogger::LoggerWrite(LoggerLevel_E logLevel, string msg)
{
    switch ((int)logLevel)
    {
    case LevelFatal:
        _logger->fatal(msg.c_str());
        qDebug().noquote() << QString::fromStdString(msg);
        break;
    case LevelError:
        _logger->error(msg.c_str());
        qDebug().noquote() << QString::fromStdString(msg);
        break;
    case LevelWarning:
        _logger->warn(msg.c_str());
        qDebug().noquote() << QString::fromStdString(msg);
        break;
    case LevelInfo:
        _logger->info(msg.c_str());
        qDebug().noquote() << QString::fromStdString(msg);
        break;
    case LevelDebug:
        _logger->debug(msg.c_str());
        qDebug().noquote() << QString::fromStdString(msg);
        break;
    case LevelTrace:
        _logger->trace(msg.c_str());
        break;
    }
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
void RTLogger::LoggerWrite(LoggerLevel_E logLevel, QString msg)
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
void RTLogger::LoggerWrite(LoggerLevel_E logLevel, const char *format, ...)
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
void RTLogger::WriteExpection(exception &ex)
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
void RTLogger::SetLogLevel(LoggerLevel_E level)
{
    _loggerLevel = level;
}
