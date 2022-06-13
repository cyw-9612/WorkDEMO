/*************************************************
  Copyright (C), 2020-2021, AIHua Tech. Co., Ltd.
  File name:    rtbaselogger.h
  Author:       yangchun
  Version:      0.0.01
  Date:         2020-6-1
  Description:  基础日志类
  History:
*************************************************/
#ifndef DEFULTLOGGER_H
#define DEFULTLOGGER_H

#include  "ilogger.h"

class LIBBASE_EXPORT RTBaseLogger : public ILogger
{
public:
    RTBaseLogger();
    void Init(string name) override;// 初始化日志文件
    void LoggerWrite(LoggerLevel_E logLevel, string msg) override;// 写日志
    void LoggerWrite(LoggerLevel_E logLevel, QString msg) override;// 写日志
    void LoggerWrite(LoggerLevel_E logLevel, const char *format, ...) override;// 写日志
    void WriteExpection(exception &ex) override;//写异常
    void SetLogLevel(LoggerLevel_E level) override;// 设置日志等级

private:
    LoggerLevel_E _loggerLevel;// 日志级别
};

#endif // DEFULTLOGGER_H
