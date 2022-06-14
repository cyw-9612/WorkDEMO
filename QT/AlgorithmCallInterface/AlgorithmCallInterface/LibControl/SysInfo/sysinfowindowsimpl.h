//syteminfowindowsimpl.h
//针对windows平台的接口
#ifndef SYTEMINFOWINDOWSIMPL_H
#define SYTEMINFOWINDOWSIMPL_H
#include <QtGlobal>
#include <QVector>
#include "sysinfo.h"
#include "LibControl_global.h"

typedef struct _FILETIME FILETIME;
class LIBCONTROL_EXPORT SysInfoWindowsImpl : public SysInfo
{
public:
    SysInfoWindowsImpl();
    void init() override;
    //windows下的实现
    double cpuLoadAverage() override;

    //windows下的实现
    double memoryUsed() override;
private:
    //获取并记录CPU的原始数据
    QVector<qulonglong> cpuRawData();
    //转换时间格式
    qulonglong convertFileTime(const FILETIME& filetime) const;
private:
    QVector<qulonglong> mCpuLoadLastValues;
};

#endif // SYTEMINFOWINDOWSIMPL_H
