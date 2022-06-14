//syteminfowindowsimpl.cpp
//#include "syteminfowindowsimpl.h"
#include "sysinfowindowsimpl.h"
#include <windows.h>

SysInfoWindowsImpl::SysInfoWindowsImpl() : SysInfo(),mCpuLoadLastValues()
{
}
double SysInfoWindowsImpl::memoryUsed()
{
    //获取内存的使用率
    MEMORYSTATUSEX memoryStatus;
    memoryStatus.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&memoryStatus);
    qulonglong memoryPhysicalUsed = memoryStatus.ullTotalPhys - memoryStatus.ullAvailPhys;
    return (double)memoryPhysicalUsed / (double)memoryStatus.ullTotalPhys * 100.0;
}
void SysInfoWindowsImpl::init()
{
    mCpuLoadLastValues = cpuRawData();
}
QVector<qulonglong> SysInfoWindowsImpl::cpuRawData()
{
    //获取CPU的占用率
    //闲置时间
    FILETIME idleTime;
    //内核使用时间
    FILETIME kernelTime;
    //用户使用时间
    FILETIME userTime;
    GetSystemTimes(&idleTime, &kernelTime, &userTime);
    QVector<qulonglong> rawData;
    rawData.append(convertFileTime(idleTime));
    rawData.append(convertFileTime(kernelTime));
    rawData.append(convertFileTime(userTime));
    return rawData;
}
qulonglong SysInfoWindowsImpl::convertFileTime(const FILETIME& filetime) const
{
    ULARGE_INTEGER largeInteger;
    largeInteger.LowPart = filetime.dwLowDateTime;
    largeInteger.HighPart = filetime.dwHighDateTime;
    return largeInteger.QuadPart;
}

double SysInfoWindowsImpl::cpuLoadAverage()
{
    QVector<qulonglong> firstSample = mCpuLoadLastValues;
    QVector<qulonglong> secondSample = cpuRawData();
    mCpuLoadLastValues = secondSample;

    //获取两个时间点之间的CPU时间
    qulonglong currentIdle = secondSample[0] - firstSample[0];
    qulonglong currentKernel = secondSample[1] - firstSample[1];
    qulonglong currentUser = secondSample[2] - firstSample[2];
    qulonglong currentSystem = currentKernel + currentUser;

    //（总的时间 - 空闲时间）/ 总的时间 = 占用cpu的时间，也就是占用率
    double percent = (currentSystem - currentIdle) * 100.0 /
    currentSystem ;
    return qBound(0.0, percent, 100.0);
}
