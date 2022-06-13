#ifndef BASEPROTOCAL_H
#define BASEPROTOCAL_H

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include "LibCommunicate_global.h"
#include "filter/ireceivefilter.h"
#include "memory/buffer.h"
#include "iprotocal.h"

//数据类型
enum measureType
{
    measureType_TolData=0,  //总值
    measureType_StaData=1,  //统计
    measureType_OCT1=2,     //1/1OCT
    measureType_OCT3=3,     //1/3OCT
    measureType_FFT=4,       //FFT
    measureType_Expose=5,  //声暴露计
    measureType_H24=6,       //24小时
    measureType_INDOOR=7,       //室内测量
};

class LIBCOMMUNICATE_EXPORT BaseProtocal : public IProtocal
{
public:
    BaseProtocal(IReceiveFilter *filter, ILogger *logger = nullptr);

    void SetLogger(ILogger *logger) override;// 日志
    IReceiveFilter *GetFilter() override;//获取协议过滤器
    void SetFilter(IReceiveFilter *filter) override;//设置协议过滤器
    bool Check(Buffer* buffer) override;// 校验协议数据
    Slice GetHead(Buffer* buffer) override;// 返回协议头
    Slice GetTail(Buffer* buffer) override;// 返回协议尾巴
    void Analysis(Buffer* buffer) override;// 解析数据
    void Package(Buffer* buffer,  QVariant &para) override;// 打包数据

    std::string GetDeviceName() override;// 获取设备名
    void SetDeviceName(std::string deviceName) override;// 设置设备名

    bool isSerialOnline(QByteArray buffer) override; //检测设备是否在线

    void setFilterClosed(bool isclosed)  override; //设置过滤器是否关闭

    void stopReadFile(bool isSendMessage) override; //停止读取文件
private:
    IReceiveFilter *m_filer;// 协议过滤器
    ILogger *m_logger;

    std::string m_name;// 设备名
};

#endif // BASEPROTOCAL_H
