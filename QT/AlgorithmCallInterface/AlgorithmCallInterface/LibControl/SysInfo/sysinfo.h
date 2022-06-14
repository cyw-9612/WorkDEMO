#ifndef SYSINFO_H
#define SYSINFO_H
#include "LibControl_global.h"

//单例
class LIBCONTROL_EXPORT SysInfo
{
public:
    //返回对象的实例
    static SysInfo& instance();
    //虚析构
    //我此析构函数必须是虚拟的，以确保从基类指针删除派生类的实例时
    //将调用派生类析构函数，而不仅仅是基类析构函数
    virtual ~SysInfo();
    //虚函数接口
    //init()：该函数允许派生类根据操作系统平台执行任何初始化过程
    virtual void init() = 0;
    //该函数调用一些特定于操作系统的代码来检索平均 CPU 负载并将其作为百分比值返回
    virtual double cpuLoadAverage() = 0;
    //该函数调用一些特定于操作系统的代码来检索使用的内存并将其作为百分比值返回
    virtual double memoryUsed() = 0;
    //virtual 关键字表示该函数可以在派生类中被覆盖。
    //= 0 语法意味着这个函数是纯虚拟的，并且必须在任何具体的派生类中被覆盖
    //此外，这使得 SysInfo 成为无法实例化的抽象类
protected:
    explicit SysInfo();
private:
    //禁止拷贝
    SysInfo(const SysInfo& rhs);
    SysInfo& operator=(const SysInfo& rhs);
};
#endif // SYSINFO_H
