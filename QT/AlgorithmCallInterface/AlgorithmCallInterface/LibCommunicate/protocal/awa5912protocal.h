#ifndef AWA5912PROTOCAL_H
#define AWA5912PROTOCAL_H

#include <QObject>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include "LibCommunicate_global.h"
#include "filter/ireceivefilter.h"
#include "memory/buffer.h"
#include "baseprotocal.h"
#include "packParam.h"

class LIBCOMMUNICATE_EXPORT AWA5912Protocal : public QObject, public BaseProtocal
{
    Q_OBJECT
public:
    AWA5912Protocal(IReceiveFilter *filter, ILogger *logger = nullptr);

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

public: //AWA5912通信相关
    //发送读控制命令  参数numRead: 读取第num组数据（1-16）,0 读基本信息,默认0
    static QByteArray getCmdRead(int numRead=0);

    //发送清空命令
    static QByteArray getCmdClear();

    //发送删除命令  参数numDelete:要删除的组号(1-16),0 清空
    static QByteArray sendCmdDelete(int numDelete);

    //发送复位命令
    static QByteArray sendCmdReset();

    //下发模板指令
    static QByteArray sendCmdWriteTemplate(QJsonObject dataJson);

    //基于授权协议的仪器信息读取命令
    static QByteArray sendCurrencyCmdReadInfo();

    //发送设置仪器中时间列表指令
    static QByteArray sendCmdSetTimeList(QJsonObject dataJson);

    //发送读取仪器中现有时间列表指令
    static QByteArray sendCmdReadTimeList();

    //接收数据解析
    void recDataAnalysis(QByteArray buffer);

signals:
    //仪器连接成功获取的仪器数据信息
    void connectDeviceInfo(QString deviceType, QString deviceNum);

    //获取一条数据(返回用于数据库写入的map 和 数据导出用的结构体receive)
    void sigGetData(QMap<QVariant,QVariant> dataMap,packRecvAll *receive);

    //获取仪器软授权信息
    void sigGetAuthInfo(QJsonObject dataJson);

    //获取仪器模板信息
    void sigGetTemplateInfo(QJsonObject templateJson);

    //仪器设置结果
    void sigSetResult(bool isSuccess);

    //仪器设置时间列表成功
    void sigSetTimeListResult();

    //读取仪器时间列表
    void sigGetTimeListResult(QJsonObject templateJson);

    //设备串口连接正常信号
    void sigSerialIsOnline();

private:
    //提取协议包中的参数数据:返回修改后的参数结构体 packRecvAll
    packRecvAll& extractPack(packRecvAll * packRecAll);

private:
    IReceiveFilter *m_filer;// 协议过滤器
    ILogger *m_logger;

    std::string m_name;// 设备名

    bool isFirstAskInfo = false;

    //设备发送指令响应响应标志位
    bool isReadMeasureDataReact = false;
    bool isReadDeviceInfoReact = false;
    bool isReadDeviceAutgInfoReact = false;
};

#endif // AWA5912PROTOCAL_H
