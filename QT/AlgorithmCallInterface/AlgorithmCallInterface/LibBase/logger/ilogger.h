/*************************************************
  Copyright (C), 2020-2021, AIHua Tech. Co., Ltd.
  File name:    ilogger.h
  Author:       yangchun
  Version:      0.0.01
  Date:         2020-6-1
  Description:  日志接口头文件
  History:
*************************************************/
#ifndef ILOGGER_H
#define ILOGGER_H

#include <string>
#include <QString>
#include "../base_global.h"
using namespace std;

//定义日志等级
enum LoggerLevel_E {
    LevelFatal = 8,
    LevelError = 7,
    LevelWarning = 6,
    LevelInfo = 5,
    LevelDebug = 4,
    LevelTrace = 3,
    LevelAll = 2,
};

class ILogger
{
public:
    virtual ~ILogger() { };
    virtual void Init(string name) = 0;  //初始化日志文件
    virtual void LoggerWrite(LoggerLevel_E logLevel, string msg) = 0;     //写日志
    virtual void LoggerWrite(LoggerLevel_E logLevel, QString msg) = 0;    //写日志
    virtual void LoggerWrite(LoggerLevel_E logLevel, const char * format, ...) = 0;//写日志
    virtual void WriteExpection(exception &ex) = 0; //写异常
    virtual void SetLogLevel(LoggerLevel_E level) = 0;     //设置日志等级

protected:
    LoggerLevel_E _loggerLevel;   //日志等级
    string _logName;            //日志文件名
};

#endif // ILOGGER_H
