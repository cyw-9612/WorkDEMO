/*************************************************
  Copyright (C), 2020-2021, AIHua Tech. Co., Ltd.
  File name:    ilogger.h
  Author:       yangchun
  Version:      0.0.01
  Date:         2020-6-1
  Description:  日志类
  History:
*************************************************/
#ifndef LOGGER_H
#define LOGGER_H

#include  <QString>
#include  <exception>
#include  "ilogger.h"
#include "../base_global.h"

namespace  Log4Qt {
    class Logger;
}

class LIBBASE_EXPORT RTLogger : public ILogger
{
public:
    RTLogger();
    void Init(string name) override;// 初始化日志文件
    void LoggerWrite(LoggerLevel_E logLevel, string msg) override;// 写日志
    void LoggerWrite(LoggerLevel_E logLevel, QString msg) override;// 写日志
    void LoggerWrite(LoggerLevel_E logLevel, const char *format, ...) override;// 写日志
    void WriteExpection(exception &ex) override;//写异常
    void SetLogLevel(LoggerLevel_E level) override;// 设置日志等级

private:
    Log4Qt::Logger *_logger;// 日志类
    LoggerLevel_E _loggerLevel;// 日志级别
};

#endif // LOGGER_H
