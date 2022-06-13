#ifndef AWA6292PROTOCAL_H
#define AWA6292PROTOCAL_H

#include <QObject>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>
#include "LibCommunicate_global.h"
#include "filter/ireceivefilter.h"
#include "memory/buffer.h"
#include "baseprotocal.h"

class LIBCOMMUNICATE_EXPORT AWA6292Protocal : public QObject, public BaseProtocal
{
    Q_OBJECT
public:
    AWA6292Protocal(IReceiveFilter *filter, ILogger *logger = nullptr);

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

    void stopReadFile(bool isSendMessage) override; //停止读取文件

public:
    /**
     * @brief 处理本地数据记录文件
     * @param fileName:文件地址
     * @param measureType:测量类型
     * @param displayText:右上角需要显示的文本数据
     * @param jsonText:绘制左侧柱状图需要的json数据
     * @param jsonKey:绘制左侧柱状图需要的json数据对应的数据KEY
     * @note
     */
    static void analysisFile(QString fileName, int measureType, QString &displayText,QJsonObject &jsonText,QStringList &jsonKey);

    /**
     * @brief 处理分块的数据记录（用于通信仪器时）
     * @param textList:安行分块的数据列
     * @param measureType:测量类型
     * @param displayText:右上角需要显示的文本数据
     * @param jsonText:绘制左侧柱状图需要的json数据
     * @param jsonKey:绘制左侧柱状图需要的json数据对应的数据KEY
     * @note
     */
    static void analysisTextList(QStringList textList, int measureType, QString &displayText,QJsonObject &jsonText,QStringList &jsonKey);

    /**
     * @brief 处理本地统计、总值、声暴露数据记录文件
     * @param dataLine:当前数据在文件中的行数
     * @param jsonText:绘制左侧柱状图需要的json数据
     * @param jsonKey:绘制左侧柱状图需要的json数据对应的数据KEY
     * @return 绘制左侧柱状图需要的json数据
     * @note
     */
    static void analyINIDataLine(QString dataLine,QJsonObject &jsonText,QStringList &jsonKey);

    /**
     * @brief 处理本地OCT FFT数据记录文件
     * @param dataLine:当前数据在文件中的行数
     * @param jsonText:绘制左侧柱状图需要的json数据
     * @param jsonKey:绘制左侧柱状图需要的json数据对应的数据KEY
     * @return 绘制左侧柱状图需要的json数据
     * @note
     */
    static void analyOCTDataLine(QString dataLine,QJsonObject &jsonText,QStringList &jsonKey);

    /**
     * @brief 处理本地24H数据记录文件
     * @param dataLine:当前数据在文件中的行数
     * @param jsonText:绘制左侧柱状图需要的json数据
     * @param jsonKey:绘制左侧柱状图需要的json数据对应的数据KEY
     * @return 绘制左侧柱状图需要的json数据
     * @note
     */
    static void analy24DataLine(QString dataLine,QJsonObject &jsonText,QStringList &jsonKey);

    /**
     * @brief 处理本地文件生成瞬时图使用的json数据,最终转成echart能识别的格式
     * @param filePath:本地文件地址
     * @param showText:得到的json文本
     * @return 绘制左侧柱状图需要的json数据
     * @note
     */
    static bool analyInstantFile(QString filePath,QString &showText);

    /**
     * @brief 处理瞬时的一行数据
     * @param isCrossDay:是否跨天标志位
     * @param dataLine:改行数据文本
     * @param cow:当前行号
     * @param dataCow:数据开始的行号
     * @param InstantDataName:每个点的数据列表
     * @param measureData:测量开始日期
     * @return 当前行解析得到的时间点json数据
     * @note
     */
    static QJsonObject analyDataLine(bool isCrossDay,QString dataLine, int cow, int dataCow, QStringList &InstantDataName, QString& measureData);

    /**
     * @brief 生成颜色数据头
     * @param InstantDataName:一个点的数据列表
     * @return 根据每个点数据多少设置的各曲线颜色参数json数据
     * @note
     */
    static QJsonArray getColorHeader(QStringList InstantDataName);

signals:
    //仪器连接成功获取的仪器数据信息
    void connectDeviceInfo(QString deviceType, QString deviceNum);

    //获取仪器软授权信息
    void sigGetAuthInfo(QJsonObject dataJson);

    //获取仪器校准信息的总数
    void sigGetCalibrationInfoNum(int calibrationInfoNum);

    //获取一条仪器校准信息
    void sigGetCalibrationData(QMap<QVariant,QVariant> dataMap, int id_c);

    //获取测量数据总数（文件夹数）
    void sigMeasureInfoNum(int FolderNum);

    //获取指定文件夹下的测量数据数量
    void sigOneMeasureInfoNum(int FolderNo, int FolderNum);

    //获取一条测量数据信息
    void sigGetMeasureData(QMap<QVariant,QVariant> dataMap, int dataType);

    //获取一条瞬时数据信息
    void sigGetInstantData(QMap<QVariant,QVariant> dataMap);

    //获取一条录音数据信息
    void sigGetSoundRecode(QMap<QVariant,QVariant> dataMap);

    //获取一条录音数据信息
    void sigGetRealTimeData(QMap<QVariant,QVariant> dataMap);

    //从仪器处读取到一个文件
    void sigHaveReadFileFromDevice(bool isFail,QString filePath);

    //获取数据无效，则返回该指令复原等待接收标志
    void gitErrorInfo();

    //当前文件传输进度
    void transmissionInfo(QString info);
private slots:
    //主动停止接收文件
    void slotStopToReadAWA6292File(bool isSendMessage);

public:
    /**
     * @brief 连接仪器信息
     * @return 指令对应json转成的字符串（加头尾）
     * @note
     */
    static QByteArray deviceConnectCmd();

    /**
     * @brief 获取授权信息
     * @return 指令对应json转成的字符串（加头尾）
     * @note
     */
    static QByteArray askAuthInfoCmd(QString deviceNum);

    /**
     * @brief 获取校准信息总数指令
     * @return 指令对应json转成的字符串（加头尾）
     * @note
     */
    static QByteArray askCalibrationInfoNumCmd(QString deviceNum);

    /**
     * @brief 获取单条校准信息指令
     * @param count:指定获取第几条校准数据
     * @return 指令对应json转成的字符串（加头尾）
     * @note
     */
    static QByteArray askCalibrationDataCmd(QString deviceNum, int count);

    /**
     * @brief 获取测量信息总数指令(6292中是文件夹是数量)
     * @return 指令对应json转成的字符串（加头尾）
     * @note
     */
    static QByteArray askMeasureInfoNumCmd(QString deviceNum);

    /**
     * @brief 获取指定一条测量信息文件夹中的数据数量指令
     * @param FolderNo ： 文件夹指代参数
     * @return 指令对应json转成的字符串（加头尾）
     * @note
     */
    static QByteArray askOneMeasureInfoNumCmd(QString deviceNum,int FolderNo);

    /**
     * @brief 获取指定一条测量信息文件夹中的一条指定数据数量指令
     * @param FolderNo ： 文件夹指代参数
     * @param FileNo：数据指代参数
     * @return 指令对应json转成的字符串（加头尾）
     * @note
     */
    static QByteArray askMeasureDataCmd(QString deviceNum, int FolderNo, int FileNo);

    /**
     * @brief 获取实时数据指令
     * @param Type ： 需要的数据类型（"Base&Sta&Oct1&Oct3"）
     * @return 指令对应json转成的字符串（加头尾）
     * @note
     */
    static QByteArray askRealTimeDataCmd(QString deviceNum, QString Type);

    /**
     * @brief 读取文件指令
     * @param filePath ： 需要读取的文件路径
     * @return 指令对应json转成的字符串（加头尾）
     * @note
     */
    static QByteArray askFileCmd(QString deviceNum, QString filePath);

    /**
     * @brief 取消读取文件指令
     * @param filePath ： 需要读取的文件路径
     * @return 指令对应json转成的字符串（加头尾）
     * @note
     */
    static QByteArray stopAskFileCmd(QString deviceNum);


private:
    /**分析测量数据结果并生成map数据发送到主界面*/
    void analyOnePieceMeasureData(int id_d, int dataType, QStringList data);

    /**分析瞬时数据结果并生成map数据发送到主界面*/
    void analyOnePieceInstantData(int id_i, QString path);

    /**分析录音数据结果并生成map数据发送到主界面*/
    void analyOnePieceSoundRecodeData(int id_s, QString path);

    /**分析传输的文件数据*/
    void analyFileTrans(Buffer* buffer);

private: //工具类方法
    /**获取flag时间戳*/
    static QString getFlag();

    /**获取仪器是否授权了一个功能*/
    bool isAuthrized(int num);

    /**用于颠倒输入的字符串列表*/
    QStringList ReverseOrder(QStringList strList);

private:
    IReceiveFilter *m_filer;// 协议过滤器
    ILogger *m_logger;

    std::string m_name;// 设备名

    QString m_deviceNum;

    //字符串顺序按：总值,统计,1/1OCT,1/3OCT,FFT,声暴露计,24小时 顺序排（和结构体measureType 保持统一）
    QStringList dataTypeList = {"TOTAL", "STATS", "1/1OCT", "1/3OCT", "FFT", "Expose", "HR24","INDOOR"};

    //设置录音、瞬时文件关联关系
    QString m_id_s_list;
    int m_id_s = -1;
    int m_id_i = -1;
    int m_sound_folder = -1;
    int m_instant_folder = -1;

    //文件传输标志位与数据标识
    bool m_isHead = false;
    bool m_isReadFile = false;
    QFile m_file;//文件对象
    QString m_fileName;//文件名字
    QString m_tempFileName;//文件名字
    qint64 m_fileSize;//文件大小
    qint64 m_receiveSize;//已经收到文件的大小

    bool isCrossDay = false;
};

#endif // AWA6292PROTOCAL_H
