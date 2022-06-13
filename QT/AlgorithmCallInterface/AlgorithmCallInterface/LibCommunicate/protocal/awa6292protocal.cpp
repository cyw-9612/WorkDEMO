#include "awa6292protocal.h"
#include <singleton.h>
#include "logger/rtbaselogger.h"
#include <QDebug>
#include <QFile>
#include <QDateTime>
#include <QCoreApplication>
#include <QDir>
#include "packParam.h"

 AWA6292Protocal::AWA6292Protocal(IReceiveFilter *filter, ILogger *logger) :
     BaseProtocal(filter, logger)
 {
     m_filer = filter;// 设置过滤器
     m_logger = logger;// 设置日志
     if(m_logger == nullptr)
     {
         m_logger = &Singleton<RTBaseLogger>::getInstance();
     }

     //初始化文件传输标志位
     m_isHead = false;
     m_isReadFile = false;
 }

 void AWA6292Protocal::SetLogger(ILogger *logger)
 {
     m_logger = logger;
 }

 // 获取协议过滤器
 IReceiveFilter *AWA6292Protocal::GetFilter()
 {
     return m_filer;
 }

 // 设置过滤器
 void AWA6292Protocal::SetFilter(IReceiveFilter *filter)
 {
     m_filer = filter;
 }

 // 校验数据是否合法
 bool AWA6292Protocal::Check(Buffer *buffer)
 {
     if(buffer != nullptr || buffer->length() > 0)
     {
         return true;// 数据正确
     }

     return false;// 数据错误
 }

 // 获取协议头
 Slice AWA6292Protocal::GetHead(Buffer *buffer)
 {
     if(buffer == nullptr || buffer->length() == 0)
     {
         return Slice();// 数据非法
     }

     return Slice(buffer->data(), 1);
 }

 // 获取协议尾巴
 Slice AWA6292Protocal::GetTail(Buffer *buffer)
 {
     if(buffer == nullptr || buffer->length() == 0)
     {
         return Slice();// 数据非法
     }

     return Slice(buffer->WriteBegin() - 1,  1);
 }

 // 解析数据
 void AWA6292Protocal::Analysis(Buffer *buffer)
 {
     //Q_UNUSED(buffer);

     //将获取到的数据根据既定通信协议进行处理
     QString data = QString::fromStdString(buffer->ToString());
     //去除无效数据
     data = data.left(buffer->length());

     //qDebug() << "AWA6292Protocal::Analysis get data: " << data.left(10);

     if(!m_isReadFile && data.contains("##") && !data.contains("Class"))
     {
         if(data.indexOf("##") > 50)
             return;

         m_isReadFile = true;
         m_isHead = true;
     }

     if(data.contains("fileEnd"))
     {
         //给服务器发送（接收文件完成）
         //tcpSocket->write("file done");

         //标志位复原
         m_isReadFile = false;
         m_file.close();

         qDebug() << "AWA6292Protocal::Analysis get m_fileName:" << m_fileName;

         //检验传输文件大小是否正确
         if(m_fileSize != m_file.size())
         {
             qDebug() << "文件尺寸不对";
             //发送文件读取失败信号
             emit sigHaveReadFileFromDevice(true , "");
             return;
         }

         //发送文件读取完成信号
         emit sigHaveReadFileFromDevice(false , m_fileName);
         return;
     }

     if(m_isReadFile)
     {
        //此处是传输文件处理
        analyFileTrans(buffer);
        return;
     }

     //qDebug() << "AWA6292Protocal::Analysis 待处理数据为: AWA6292" << data;

     //帧头<N>索引
     int indexBegin=data.indexOf("<N>");
     indexBegin+=3;
     //帧尾</N>索引
     int indexEnd=data.indexOf("</N>");

 //    qDebug()<<"indexBegin:"<<indexBegin;
 //    qDebug()<<"indexEnd:"<<indexEnd;

     //条件判断，排除数据接收不完全情况
     if( 3!=indexBegin || indexEnd-indexBegin < 20)
     {
         return;
     }
     else
     {
         //提取<N>和</N>之间的Json字符串
         QString strJson=data.mid(indexBegin,indexEnd-indexBegin);
         //qDebug().noquote() <<"AWA6292Protocal::readSocket strJson:"<<strJson;

         QJsonParseError error;
         QJsonDocument jsonDocument  = QJsonDocument::fromJson(strJson.toUtf8(), &error);

         if (error.error == QJsonParseError::NoError)
         {
             if (!(jsonDocument.isNull() || jsonDocument.isEmpty()))
             {
                 if (jsonDocument.isObject())
                 {
                     QMap<QString, QVariant> m_result = jsonDocument.toVariant().toMap();

                     //首先判断仪器连接信息 "101"
                     if(QString::number(AWAJSON_cmd::DeviceConnect) == m_result["Class"].toString())
                     {
                         //记录仪器的型号
                         QString deviceType = m_result["DeviceType"].toString();
                         if("" == deviceType)
                             deviceType = "AWA6292";

                         //记录仪器的串号
                         QString deviceNum = m_result["DeviceAWAID"].toString();

                         m_deviceNum = deviceNum;

                         //仪器连接成功获取的仪器数据信息
                         emit connectDeviceInfo(deviceType, deviceNum);
                     }

                     //获取仪器授权信息 "130"
                     if(QString::number(AWAJSON_cmd::AuthInfoCmd) == m_result["Class"].toString())
                     {
                         QJsonObject dataJson;
                         dataJson.insert("instrumentSerialType", m_result["DeviceType"].toString());
                         dataJson.insert("instrumentSerialNumber", m_result["DeviceAWAID"].toString());
                         //返回的传声器型号和串号由“+”连接
                         QString MicSN = m_result["MicSN"].toString();
                         int leftCount = MicSN.indexOf("-");
                         QString microphoneSerialType = MicSN.left(leftCount);
                         QString microphoneSerialNumber = MicSN.right(MicSN.length() - 1 - leftCount);
//                         QStringList microphoneSerialInfo = m_result["MicSN"].toString().split("+");
//                         //如果返回结果没有两项
//                         if(microphoneSerialInfo.count() < 2)
//                         {
//                             microphoneSerialInfo << microphoneSerialInfo[0];
//                         }
                         dataJson.insert("microphoneSerialType", microphoneSerialType);
                         dataJson.insert("microphoneSerialNumber", microphoneSerialNumber);
                         dataJson.insert("hardwareVersion",m_result["HardwareVersion"].toString());
                         dataJson.insert("softwareVersion",m_result["SoftwareVersion"].toString());

                         //装填功能 总计8个功能
                         dataJson.insert("total",m_result["Base"].toBool());
                         dataJson.insert("sta",m_result["Sta"].toBool());
                         dataJson.insert("OCT1",m_result["Oct1"].toBool());
                         dataJson.insert("OCT3",m_result["Oct3"].toBool());
                         dataJson.insert("FFT",m_result["Fft"].toBool());
                         dataJson.insert("expose",m_result["expose"].toBool());
                         dataJson.insert("Sti",m_result["Sti"].toBool());
                         dataJson.insert("H24",m_result["Hr24"].toBool());
                         dataJson.insert("indoor",m_result["Oct1Id"].toBool());
                         dataJson.insert("reverberation",m_result["Rever"].toBool());

                         emit sigGetAuthInfo(dataJson);
                     }

                     //获取仪器校准记录 "131"
                     if(QString::number(AWAJSON_cmd::CalibrationInfoCmd) == m_result["Class"].toString())
                     {
                         //根据返回信息的CMD处理
                         switch (m_result["CMD"].toInt())
                         {
                            //返回的是校准信息的总数
                            case CalibrationInfo_cmd::ReplyCalibrationInfoNum :
                             {
                                 int calibrationInfoNum = m_result["InfoCount"].toInt();
                                 //返回校准信息的总数
                                 emit sigGetCalibrationInfoNum(calibrationInfoNum);
                             }
                             break;
                             //返回的是一条校准数据
                             case CalibrationInfo_cmd::ReplyCalibrationData :
                             {
                             qDebug() << strJson;
                                //获取当前数据的编号
                                int calibrationDataCount = m_result["NUM"].toInt();
                                //装填数据库写入所需map数据
                                QMap<QVariant , QVariant> mapData;
                                mapData.insert(":id_c", calibrationDataCount);
                                mapData.insert(":instrumentType", m_result["DeviceType"].toString());//仪器型号
                                mapData.insert(":instrumentNum", m_result["DeviceAWAID"].toString());//仪器串号
                                QString a = m_result["CaliTime"].toString();
                                QDateTime calibrationTime = QDateTime::fromString(a,"yyyyMMddhhmmss");
                                mapData.insert(":calibrationTime", calibrationTime);//校准时间
                                mapData.insert(":temperature", QString::number(m_result["CaliTemp"].toFloat(), 'f', 1));//校准温度 ℃
                                mapData.insert(":humidity", QString::number(m_result["CaliHumi"].toFloat(), 'f', 1));//校准湿度 %
                                mapData.insert(":atmosphericPressure", QString::number(m_result["CaliP"].toFloat(), 'f', 1));//校准气压 单位 百帕
//                                QStringList microphone = m_result["MicSN"].toString().split("+");//传声器序列号可以为传送器型号+序列号
//                                //如果返回结果没有两项
//                                if(microphone.count() < 2)
//                                {
//                                    microphone << microphone[0];
//                                }

                                QString MicSN = m_result["MicSN"].toString();
                                int leftCount = MicSN.indexOf("-");
                                QString microphoneSerialType = MicSN.left(leftCount);
                                QString microphoneSerialNumber = MicSN.right(MicSN.length() - 1 - leftCount);
                                mapData.insert(":microphoneModel", microphoneSerialType);//传声器型号
                                mapData.insert(":microphoneNum", microphoneSerialNumber);//传声器串号
                                mapData.insert(":freeFieldCorrection",QString::number(m_result["FreeFieldCorrection"].toFloat(), 'f', 1));//自由场修正量dB
                                mapData.insert(":microphoneSensitivityLevel", QString::number(m_result["MicSensitivityLevel"].toFloat(), 'f', 1));//传声器灵敏度级别
                                QString RangeLow = QString::number(m_result["RangeLow"].toFloat(), 'f', 1);
                                QString RangeHigh = QString::number(m_result["RangeHigh"].toFloat(), 'f', 1);
                                QString measureRange = RangeLow + "-" + RangeHigh;
                                mapData.insert(":measureRange", measureRange);//测量范围
                                mapData.insert(":soundCalibrationSoundLevel", m_result["VoiceMicSensitivityLevel"].toString().replace("dB",""));//声校准声压级
                                mapData.insert(":calibrationSoundLevel", m_result["AdjValue"].toString().replace("dB",""));//校准声压级
                                mapData.insert(":calibrationFrequent", m_result["CaliFreq"].toString());//校准器频率

                                emit sigGetCalibrationData(mapData, calibrationDataCount);
                             }
                              break;
                         default:;

                         }

                     }

                     //获取仪器测量信息记录 "132"
                     if(QString::number(AWAJSON_cmd::MeasureInfoCmd) == m_result["Class"].toString())
                     {
                         //qDebug() << "收到132数据回复：" << QDateTime::currentMSecsSinceEpoch();
                         //根据返回信息的CMD处理
                         switch (m_result["CMD"].toInt())
                         {
                            //返回的是测量信息的总数(文件夹数)
                            case MeasureInfo_cmd::ReplyMeasureInfoNum :
                             {
                                //获取文件夹总数
                                int FolderNum = m_result["FolderNum"].toInt();
                                emit sigMeasureInfoNum(FolderNum);
                             }
                             break;
                             //返回的是一个测量信息中测量数据的数量(一个文件夹下的数据数量)
                             case MeasureInfo_cmd::ReplyMeasureOneDataCount :
                             {
                                //记录问询的文件夹编号
                                int FolderNo = m_result["folderNo"].toInt();
                                //获取一个文件夹下的数据数量
                                int FileNum = m_result["FileNum"].toInt();
                                emit sigOneMeasureInfoNum(FolderNo, FileNum);
                             }
                              break;
                             //返回的是一个指定的测量数据
                             case MeasureInfo_cmd::ReplyMeasureOneData :
                             {
                                 //记录问询的文件夹编号
                                 int FolderNo = m_result["folderNo"].toInt();
                                 //获取一个文件夹下的数据数量
                                 int FileNo = m_result["FileNo"].toInt();
                                 //计算当前测量数据的唯一标号
                                 int id = FolderNo*100 + FileNo;
                                 //qDebug() <<"FolderNo: " << FolderNo << "FileNo: " << FileNo << "id:" << id;
                                 //获取当前数据内容(ByteArray格式防止非字符数据丢失)
                                 QString Value = m_result["Value"].toString();
                                //分析返回的数据类型
                                QString dataType = m_result["Type"].toString();
                                if("Date" == dataType) //测量数据
                                {
                                    QStringList valueLine = Value.split("\n");
                                    int dataType = -1;
                                    //判断数据类型
                                    for(int i = 0; i < dataTypeList.count(); i++)
                                    {
                                        if(Value.contains("-" + dataTypeList[i]))
                                        {
                                            //记录数据类型，跳出循环
                                            dataType = i;
                                            break;
                                        }
                                    }

                                    if(-1 == dataType)
                                    {
                                        emit gitErrorInfo();
                                        return;
                                    }

                                    //处理数据
                                    analyOnePieceMeasureData(id, dataType, valueLine);

                                }
                                else if("Log" == dataType) //数字记录数据
                                {
                                    //记录当前数字记录数据的所属文件夹和数据库id
                                    m_id_i = id;
                                    m_instant_folder = FolderNo;
                                    //处理数据
                                    analyOnePieceInstantData(id, Value);
                                }
                                else if("Rec" == dataType) //录音文件
                                {
                                    //记录当前录音文件的所属文件夹和数据库id
                                    m_id_s = id;

                                    if(0 == FileNo)
                                    {
                                        m_sound_folder = FolderNo;
                                        m_id_s_list = QString::number(id);
                                    }
                                    else
                                    {
                                        if(m_sound_folder == FolderNo)
                                            m_id_s_list = m_id_s_list + ";" + QString::number(id);
                                    }
                                    //处理数据
                                    analyOnePieceSoundRecodeData(id, Value);
                                }
                             }
                              break;
                         default:;

                         }
                     }

                     //获取仪器实时数据记录 "133"
                     if(QString::number(AWAJSON_cmd::RealTimeInfoCmd) == m_result["Class"].toString())
                     {
                         QMap<QVariant, QVariant> realTimeData;
                         bool haveData = m_result["haveData"].toBool();
                         realTimeData.insert("haveData",haveData);
                         if(haveData)
                         {
                             qDebug() << m_result;
                             QString dataType = m_result["Type"].toString();
                             realTimeData.insert("dataType", dataType);

                             //基本数据
                             QStringList dataName = {"LAFp","LASp","LAIp","LCFp","LCSp","LCIp","LZFp","LZSp","LZIp","LAeq1s","LCeq1s","LZeq1s"};
                             for (int i =0; i < dataName.count(); i++)
                             {
                                 realTimeData.insert(dataName[i], m_result[dataName[i]].toString());
                             }

                             //统计数据
                             QString StaValue = m_result["Lp"].toString();
                             realTimeData.insert("StaKey", "Lp");
                             realTimeData.insert("StaValue", StaValue);

                             //OCT1数据
                             QStringList FreqOct1,DataOct1 ;
                             FreqOct1 << "SPL(A)" << "SPL(C)" << "SPL(Z)";
                             FreqOct1 <<  ReverseOrder(m_result["FreqOct1"].toString().split("*"));
                             QStringList DataACZOct1 = m_result["DataAczOct1"].toString().split("*");
                             DataOct1 << DataACZOct1;
                             DataOct1 << ReverseOrder(m_result["DataOct1"].toString().split("*"));
                             realTimeData.insert("FreqOct1", FreqOct1);
                             realTimeData.insert("DataOct1", DataOct1);

                             //OCT3数据
                             QStringList FreqOct3,DataOct3;
                             FreqOct3 << "SPL(A)" << "SPL(C)" << "SPL(Z)" << "SPL(T)" << "SPL(U)";
                             FreqOct3 << ReverseOrder(m_result["FreqOct3"].toString().split("*"));
                             QStringList DataACZOct3 = m_result["DataAczOct3"].toString().split("*");
                             DataOct3 << DataACZOct3;
                             DataOct3 << ReverseOrder(m_result["DataOct3"].toString().split("*"));
                             realTimeData.insert("FreqOct3", FreqOct3);
                             realTimeData.insert("DataOct3", DataOct3);
                         }
                         emit sigGetRealTimeData(realTimeData);

                     }

                    //获取文件
                     if(QString::number(AWAJSON_cmd::RealFileCmd) == m_result["Class"].toString())
                     {
                         //qDebug() << "AWA6292Protocal::Analysis 待处理数据为: AWA6292" << data;

                        if(1 ==m_result["CMD"].toInt())
                        {
                            //发送文件读取完成信号
                            emit sigHaveReadFileFromDevice(true , "");
                        }
                        return;
                     }
                 }
             }
         }
         else
         {
             // 检查错误类型
             qFatal(error.errorString().toUtf8().constData());
             //exit(1);
         }
     }
 }

 // 打包数据
 void AWA6292Protocal::Package(Buffer *buffer, QVariant &para)
 {
     Q_UNUSED(buffer);
     Q_UNUSED(para);

     return;
 }

 // 获取设备名
 std::string AWA6292Protocal::GetDeviceName()
 {
     return m_name;
 }

 // 设置设备名
 void AWA6292Protocal::SetDeviceName(std::string name)
 {
     m_name = name;
 }

 //停止读取文件
 void AWA6292Protocal::stopReadFile(bool isSendMessage)
 {
    slotStopToReadAWA6292File(isSendMessage);
 }

 //处理本地数据记录文件
 void AWA6292Protocal::analysisFile(QString fileName, int measureType, QString &displayText,QJsonObject &jsonText,QStringList &jsonKey)
 {
     //读取文件
     QByteArray byteData;
     QFile file(fileName);
     displayText.clear();
     jsonKey.clear();
     int cow = 0,dataCow = 5000;
     file.open(QIODevice::ReadOnly | QIODevice::Text);
     while(!file.atEnd())
     {
         byteData = file.readLine();
         //解码
         QString currentLine = QString::fromLocal8Bit(byteData);



         if(currentLine.contains("peripheral"))
         {
             displayText.append("\n");
             continue;
         }

         if(currentLine.contains("data"))
         {
             dataCow = cow;
             //qDebug() << dataCow;
         }

         //根据6292的数据存储格式进行数据分析
         switch (measureType)
         {
         case measureType::measureType_TolData:
         case measureType::measureType_StaData:
         case measureType::measureType_Expose:
             analyINIDataLine(currentLine, jsonText,jsonKey);
         break;
         case measureType::measureType_OCT1:
         case measureType::measureType_OCT3:
         case measureType::measureType_FFT:
         case measureType::measureType_INDOOR:
             analyOCTDataLine(currentLine, jsonText,jsonKey);
         break;
         case measureType::measureType_H24:
             analy24DataLine(currentLine, jsonText,jsonKey);
         break;
         default: ;
         }

         cow++;

         if(currentLine.contains("hide"))
         {
             continue;
         }

         if(currentLine.contains("Ld") || currentLine.contains("Ln") || currentLine.contains("Ldn"))
         {
             //自定义记权
            displayText.append(currentLine);
         }

         //OCT , FFT 和 H24 的文本部分只显示测量信息 数值在表格中显示
         if((measureType == measureType::measureType_OCT1 || measureType == measureType::measureType_OCT3 ||
             measureType == measureType::measureType_FFT || measureType == measureType::measureType_H24 ||
             measureType == measureType::measureType_INDOOR)
                 && cow >=dataCow)
         {
             continue;
         }

         if(!currentLine.contains("checkcode"))
         {
             //currentLine.replace(" \t","");
             //if(currentLine.contains("测量方式"))
             //    currentLine = currentLine.left(currentLine.indexOf("-"));

             //if(currentLine.contains("经度"))
             //    currentLine = currentLine.replace(" 纬度","\n纬度");

             //处理总值数据的显示
             if(currentLine.contains("=\t"))
                 currentLine.replace("\t"," ");
             else
                currentLine.replace(" \t","");

             if(currentLine.contains("测量方式"))
                 currentLine = currentLine.left(currentLine.indexOf("-"));

             if(currentLine.contains("最大风速风向"))
                 currentLine = currentLine.replace("\t","");

             if(currentLine.contains("经度"))
                 currentLine = currentLine.replace(" 纬度","\n纬度");

             if(measureType == measureType::measureType_TolData)
             {
                 QStringList text;
                 text << "LCFmax" << "LZFmax" << "LCSmax" << "LZSmax" << "LCImax" << "LZImax"
                          << "LCFmin" << "LZFmin" << "LCSmin" << "LZSmin" << "LCImin" << "LZImin"
                          << "LCeq,T" << "LZeq,T" << "LCpeak" << "LZpeak" << "SELC" << "SELZ";
                 for(int i = 0; i < text.count(); i++)
                 {
                     if(currentLine.contains(text[i]))
                         currentLine = currentLine.replace(text[i],"  " + text[i]);
                 }
             }

             displayText.append(currentLine);
         }

     }
 }

 void AWA6292Protocal::analysisTextList(QStringList textList, int measureType, QString &displayText,QJsonObject &jsonText,QStringList &jsonKey)
 {
     //初始化
     displayText.clear();
     jsonKey.clear();
     int cow = 0,dataCow = 5000, peripheralCow = 0;

     //循环遍历文件数据
     for(int i = 0; i < textList.count(); i++)
     {
         QString currentLine = textList[i];

         if(currentLine.contains("peripheral"))
         {
             displayText.append("\n");
             peripheralCow = cow;
             continue;
         }

         if(currentLine.contains("data"))
         {
             dataCow = cow;
             currentLine.replace("data","\n数据-Data");
             //qDebug() << dataCow;
         }

         //根据6292的数据存储格式进行数据分析
         switch (measureType)
         {
         case measureType::measureType_TolData:
         case measureType::measureType_StaData:
         case measureType::measureType_Expose:
             analyINIDataLine(currentLine, jsonText,jsonKey);
         break;
         case measureType::measureType_OCT1:
         case measureType::measureType_OCT3:
         case measureType::measureType_FFT:
         case measureType::measureType_INDOOR:
             analyOCTDataLine(currentLine, jsonText,jsonKey);
         break;
         case measureType::measureType_H24:
             analy24DataLine(currentLine, jsonText,jsonKey);
         break;
         default: ;
         }

         cow++;

         //分属于因此数据内容
         if(currentLine.contains("hide"))
         {
             continue;
         }

         if(currentLine.contains("Ld") || currentLine.contains("Ln") || currentLine.contains("Ldn"))
         {
             //自定义记权
            displayText.append(currentLine + "\n");
         }

         if(0 == peripheralCow)
         {
             //OCT , FFT 和 H24 的文本部分只显示测量信息 数值在表格中显示  但是需要显示气象数据
             if((measureType == measureType::measureType_OCT1 || measureType == measureType::measureType_OCT3 ||
                 measureType == measureType::measureType_FFT || measureType == measureType::measureType_H24 ||
                 measureType == measureType::measureType_INDOOR)&& cow >=dataCow)
             {
                    continue;
             }
         }

         if(!currentLine.contains("checkcode"))
         {
             //处理总值数据的显示
             if(currentLine.contains("=\t"))
                 currentLine.replace("\t"," ");
             else
                currentLine.replace(" \t","");

             if(currentLine.contains("测量方式"))
                 currentLine = currentLine.left(currentLine.indexOf("-"));

             if(currentLine.contains("最大风速风向"))
                 currentLine = currentLine.replace("\t","");

             if(currentLine.contains("经度"))
                 currentLine = currentLine.replace(" 纬度","\n纬度");

             if(measureType == measureType::measureType_TolData)
             {
                 QStringList text;
                 text << "LCFmax" << "LZFmax" << "LCSmax" << "LZSmax" << "LCImax" << "LZImax"
                          << "LCFmin" << "LZFmin" << "LCSmin" << "LZSmin" << "LCImin" << "LZImin"
                          << "LCeq,T" << "LZeq,T" << "LCpeak" << "LZpeak" << "SELC" << "SELZ";
                 for(int i = 0; i < text.count(); i++)
                 {
                     if(currentLine.contains(text[i]))
                     {
                         currentLine = currentLine.replace(text[i],"  " + text[i]);
                     }
                 }
             }

             displayText.append(currentLine + "\n");
         }
     }
 }

 void AWA6292Protocal::analyINIDataLine(QString dataLine,QJsonObject &ReadDatajsonObject,QStringList &jsonKey)
 {
     try
     {
         QString dataName = dataLine.left(dataLine.indexOf("\t"));
         //去除空格，= 和 ：
         dataName.replace(QRegExp("\\s{1,}"), "");
         dataName.replace(":", "");

         if(dataName.contains("hide"))
         {
             //获取分布图名称
            dataName = dataLine.mid(5,5);
            //数据分布最后一位为空格
            QString dataList = dataLine.mid(11,dataLine.length() - 12).replace(" ",";");
            ReadDatajsonObject.insert(dataName, dataList);
            return;
         }

         if(dataName == "")
         {
             return;
         }

         if(dataName.contains("经度"))
         {
             QString data = dataLine.mid(dataLine.indexOf("\t") + 1 , dataLine.indexOf("\n") - dataLine.indexOf("\t") -1 );
             //经纬度数据特殊处理
            QString jindu = data.left(data.indexOf("纬度"));
            QString weidu = data.right(data.length() - data.indexOf("\t") - 1);
            QString str = "(" + jindu + "," + weidu +")";
            ReadDatajsonObject.insert("经纬度", str);
            ReadDatajsonObject.insert("测点经度", jindu);
            ReadDatajsonObject.insert("测点纬度", weidu);
         }
         else if(dataName.contains("="))
         {
             dataLine.replace("=\t","=");
             dataLine.replace("= \t","=");
             QStringList shortDataList = dataLine.split("\t");
             for (int i = 0;i < shortDataList.length(); i++)
             {
                 if(shortDataList[i].contains("="))
                 {
                     int index = shortDataList[i].indexOf("=");
                     QString key = shortDataList[i].left(index).replace(QRegExp("\\s{1,}"), "");
                     QString value = shortDataList[i].right(shortDataList[i].length() - index -1 ).replace(QRegExp("\\s{1,}"), "");
                     ReadDatajsonObject.insert(key, value);
                     jsonKey.append(key);
                 }
             }
         }
         else
         {
             QString data = dataLine.mid(dataLine.indexOf("\t") + 1 , dataLine.indexOf("\n") - dataLine.indexOf("\t") -1 );
             if(data.contains("="))
             {
                 //自定义计权
                 data = data.mid(data.indexOf("\t") + 1 , 5 );
                 jsonKey.append(dataName);
             }
             ReadDatajsonObject.insert(dataName, data);
         }

     } catch (exception& e)
     {
         return;
     }
 }

 void AWA6292Protocal::analyOCTDataLine(QString dataLine,QJsonObject &m_ReadDatajsonObject,QStringList &jsonKey)
 {
     try
     {
         QString dataName = dataLine.left(dataLine.indexOf("\t"));
         //去除空格和 ：
         dataName.replace(QRegExp("\\s{1,}"), "");
         dataName.replace(":", "");
         if(dataName == "")
         {
             return;
         }
         QString data = dataLine.mid(dataLine.indexOf("\t") + 1 , dataLine.indexOf("\n") - dataLine.indexOf("\t") -1 );
         if(data.contains("纬度"))
         {
             //经纬度数据特殊处理
            QString jindu = data.left(data.indexOf("纬度"));
            QString weidu = data.right(data.length() - data.indexOf("\t") - 1);
            QString str = "(" + jindu + "," + weidu +")";
            m_ReadDatajsonObject.insert("经纬度", str);
            m_ReadDatajsonObject.insert("测点经度", jindu);
            m_ReadDatajsonObject.insert("测点纬度", weidu);
         }
         else if(data.contains("\t"))
         {
             QStringList dataList = data.split("\t");
             //qDebug() << dataList.length();
             QStringList dataNameList = {"Lmax","Lmin","Leq,T"};
             QJsonObject paraPack;
             for (int y = 0; y < dataNameList.length() ; y++)
             {
                 paraPack.insert(dataNameList[y],dataList[y].replace(QRegExp("\\s{1,}"), ""));
             }

             m_ReadDatajsonObject.insert(dataName, paraPack);
             if( !dataName.contains("频率"))
             {
                 jsonKey.append(dataName);
             }
         }
         else
         {
             m_ReadDatajsonObject.insert(dataName, data);
         }
     } catch (exception& e)
     {
         return;
     }
 }

 void AWA6292Protocal::analy24DataLine(QString dataLine,QJsonObject &m_ReadDatajsonObject,QStringList &jsonKey)
 {
     try
     {
         QString dataName = dataLine.left(dataLine.indexOf("\t"));
         //去除空格和 ：
         dataName.replace(QRegExp("\\s{1,}"), "");
         dataName.replace(":", "");
         if(dataName == "")
         {
             return;
         }
         QString data = dataLine.mid(dataLine.indexOf("\t") + 1 , dataLine.indexOf("\n") - dataLine.indexOf("\t") -1 );

         if(data.contains("纬度"))
         {
             //经纬度数据特殊处理
            QString jindu = data.left(data.indexOf("纬度")).replace("\t","");
            QString weidu = data.right(data.length() - data.indexOf("\t") - 1).replace("纬度:","").replace("\t","");
            QString str = "(" + jindu + "," + weidu +")";
            m_ReadDatajsonObject.insert("经纬度", str);
            m_ReadDatajsonObject.insert("测点经度", jindu);
            m_ReadDatajsonObject.insert("测点纬度", weidu);
         }
         else if(dataName.contains("="))
         {
             QString customName = dataName.left(dataName.indexOf("="));
             QString customData = dataName.right(dataName.length() - dataName.indexOf("=") - 1);
             m_ReadDatajsonObject.insert(customName, customData);
         }
         else if(data.contains("\t"))
         {
             QStringList dataList = data.split("\t");
             //qDebug() << dataList.length();
             QStringList dataNameList = {"Lmax","Lmin","Leq,T","L5","L10","L50","L90","L95","SD"};
             QJsonObject paraPack;

//             if(dataList.length() != dataNameList.length())
//             {
//                return;
//             }

             for (int y = 0; y < dataNameList.length() ; y++)
             {
                 paraPack.insert(dataNameList[y],dataList[y].replace(QRegExp("\\s{1,}"), ""));
             }

             m_ReadDatajsonObject.insert(dataName, paraPack);
             if( !dataName.contains("Time"))
             {
                 jsonKey.append(dataName);
             }
         }
         else
         {
             m_ReadDatajsonObject.insert(dataName, data);
         }
     } catch (exception& e)
     {
         return;
     }
 }

 bool AWA6292Protocal::analyInstantFile(QString filePath,QString &showText)
 {
     try{
         //QJsonObject jsonData;
         //QByteArray js;
    //     //装填固有数据 数据标签
    //     //jsonData.insert("activeType","sys_back_lp");
         showText.append("{\"activeType\": \"sys_back_lp\",\"list\": [");
         //js.append("{\"activeType\": \"sys_back_lp\",\"list\": [");
         //一个时间点的数据列表
         QStringList InstantDataName;

         qDebug() << "AWA6292Protocal::analyInstantFile start";

         //读取文件
         QByteArray byteData;
         QString text;
         QFile file(filePath);
         QJsonArray dataListArray;
         QString measureData = "";
         QString day = "";
         bool isCrossDay = false;
         bool haveMeasurDate = false;
         int cow = 0,dataCow = 5000;
         if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
         {
             //文件只读开启失败
             return false;
         }
         //int i = 0;
         while(!file.atEnd())
         {
             byteData = file.readLine();
             //解码
             QString currentLine = QString::fromLocal8Bit(byteData);
             if(currentLine.contains("data"))
             {
                 //记录数据开始行
                 dataCow = cow;
                 //qDebug() << dataCow;
             }

             QJsonObject oneLine = analyDataLine(isCrossDay, currentLine,cow++,dataCow,InstantDataName,measureData);

             if("" != measureData && !haveMeasurDate)
             {
                 //记录读取到的测量开始时间，用于做数据跨天处理
                day = measureData;
                haveMeasurDate = true;
             }
             else if(haveMeasurDate && !isCrossDay && day!=measureData)
             {
                 //跨天标志位，跨天只进行一次
                isCrossDay = true;
             }

             if(!(oneLine.value("dateTime").toString() == ""))
             {
                 //dataListArray.append(oneLine);
                 showText.append(QString(QJsonDocument(oneLine).toJson()) + ",");
    //              js.append(QString(QJsonDocument(oneLine).toJson()).replace(" ","").toUtf8());
    //              js.append( + ",");
                 //qDebug().noquote() << i++;
             }

             currentLine.clear();
             //text.append(currentLine);
         }
         //qDebug() << text;

         //装填数据 list
         //jsonData.insert("list",dataListArray);

         QString header = QString(QJsonDocument(getColorHeader(InstantDataName)).toJson()).replace(" ","") + ",";
         showText.append("],\"header\": " + header + "}");
    //     js.append("],\"header\": ");
    //     js.append(header.toUtf8());
    //     js.append("}");
         //装填颜色 header
         //jsonData.insert("header", getColorHeader(InstantDataName));

         //转换得到最终结果
         //showText = QString(QJsonDocument(jsonData).toJson());
        qDebug() << "AWA6292Protocal::analyInstantFile isover";
         return true;
    } catch (exception& e)
    {
         qDebug() << "AWA6292Protocal::analyInstantFile exception";
        return false;
    }
 }

 //分析一行数据
 QJsonObject AWA6292Protocal::analyDataLine(bool isCrossDay, QString dataLine, int cow, int dataCow, QStringList &InstantDataName, QString& measureDay)
 {
     QJsonObject onePoint;

     try
     {
         //瞬时图数据用的json只需要数据记录即可,数据之前的信息不显示,只提取需要的测量方式，测量时间 ，
         if(cow <= dataCow)
         {
             QString dataName = dataLine.left(dataLine.indexOf("\t"));
             //去除空格，= 和 ：
             dataName.replace(QRegExp("\\s{1,}"), "");
             dataName.replace(":", "");

             QString data = dataLine.mid(dataLine.indexOf("\t") + 1 , dataLine.indexOf("\n") - dataLine.indexOf("\t") -1 );

             if(dataLine.contains("开始时间"))
             {
                 QStringList day = dataLine.split("\t");

                 //记录开始时间的日期 只取左边10位 YYYY-MM-DD
                 measureDay = day[1].replace(QRegExp("\\s{1,}"), "").left(10);
             }

             //返回数据
             onePoint.insert(dataName, data);
             return onePoint;
         }

         if(cow == dataCow + 1)
         {
             //此处获取的是数据的名称行
             //以制表符分开数据块
             InstantDataName = dataLine.split("\t");

             for (int i = 0; i < InstantDataName.count(); i++)
             {
                 //去除数据名称中的空格
                 InstantDataName[i].replace(QRegExp("\\s{1,}"), "");
             }

             //返回空数据
             onePoint.insert(QString::number(cow), dataLine);
             return onePoint;
         }

         //开始数据的处理
         QJsonObject oneLineData;
         QStringList dataList = dataLine.split("\t");

         //根据数据格式，最后一个是时间戳，需要放在外面
         for (int i = 0; i < InstantDataName.count() - 1 ; i++)
         {
             if(i >= dataList.count())
                 continue ;

             double count = dataList[i].replace(QRegExp("\\s{1,}"), "").toDouble();
             //保留一位小数
             QString data = QString::number(count, 'f', 1);
             oneLineData.insert(InstantDataName[i],data);
         }

         //插入infoList数据点
         onePoint.insert("infoList",oneLineData);

         //插入时间戳 最后一个数据就是时间戳 20211122133326:015
         QString time = dataList[dataList.count()-1].replace(QRegExp("\\s{1,}"), "");
         QDateTime measureTimex = QDateTime::fromString(time,"yyyyMMddhhmmss:zzz");
         QString newStateTime = measureTimex.toString("yyyy-MM-dd hh:mm:ss:zzz");
         QDateTime day = QDateTime::fromString(measureDay,"yyyy-MM-dd");
         //如果是跨天的日期加一
         if("00:00:0" == time.left(7) && !isCrossDay)
         {
             measureDay = day.addDays(1).toString("yyyy-MM-dd");
         }
         if("" == newStateTime)
             onePoint.insert("dateTime",measureDay + " " + time);
         else
             onePoint.insert("dateTime",newStateTime);

         //返回单个节点数据
         return onePoint;

     } catch (exception& e)
     {
         onePoint.insert(QString::number(cow), dataLine);
         return onePoint;
     }
 }

 //生成颜色数据头
 QJsonArray AWA6292Protocal::getColorHeader(QStringList InstantDataName)
 {
     QJsonArray colorHeader;

     //根据解析获得的数据类型填写颜色
     QStringList colorList;
     colorList << "#C23531"      << "#61A0A8" << "#2F4554"
                    << "#FF0700"     << "#FE9B00" << "#00FF01"
                    << "#02FDFC"     << "#F9FF01" << "#9F00FF"
                    << "#FD01FE"     << "#964A4C" << "#A88A74"
                    << "#81190B"     << "#F9760E" << "#ADB836"
                    << "#49BFA5"     << "#000000" << "#0000FE"
                    << "#AC5890"     << "#577AFC" << "#A97D7D"
                    << "#F05B5B"     << "#E3BB7C" << "#3E740A"
                    << "#1D567F"     << "#E3C656" << "#1B0C59"
                    << "#9B1B6B"     << "#563A0B" << "#767171"
                    << "#B99477"     << "#C07D3E" << "#BDF174"
                    << "#43A2DC"     << "#DAD8B5" << "#8C80BB"
                    << "#1890FF"     << "#733383" << "#3b0FEA"
                    << "#CA8622"     << "#546570" << "#BDA29A";

     for (int i = 0; i < InstantDataName.count(); i++)
     {
         if("time" == InstantDataName[i])
         {
             continue;
         }
         QJsonObject oneColorData;
         oneColorData.insert("color", colorList[i]);
         oneColorData.insert("name", InstantDataName[i]);
         colorHeader.append(oneColorData);
     }
     return colorHeader;
 }

//连接仪器信息
QByteArray AWA6292Protocal::deviceConnectCmd()
{
    QJsonObject jsonObject;

    //指令类型:连接仪器信息
    jsonObject.insert("Class", QString::number(AWAJSON_cmd::DeviceConnect));

    //时间戳
    jsonObject.insert("Flag", getFlag());

    QString stringFull = "<N>";
    stringFull.append((QJsonDocument(jsonObject).toJson()));
    stringFull.append("</N>");
    stringFull = stringFull.simplified();
    stringFull.replace(" ", "");

    return stringFull.toLatin1();
}

 //获取授权信息
QByteArray AWA6292Protocal::askAuthInfoCmd(QString deviceNum)
{
    QJsonObject jsonObject;

    //指令类型:获取授权信息
    jsonObject.insert("Class", QString::number(AWAJSON_cmd::AuthInfoCmd));

    //时间戳
    jsonObject.insert("Flag", getFlag());

    //仪器串号
    jsonObject.insert("DeviceAWAID",deviceNum);

    QString stringFull = "<N>";
    stringFull.append((QJsonDocument(jsonObject).toJson()));
    stringFull.append("</N>");
    stringFull = stringFull.simplified();
    stringFull.replace(" ", "");

    return stringFull.toLatin1();
}

//获取校准信息总数指令
QByteArray AWA6292Protocal::askCalibrationInfoNumCmd(QString deviceNum)
{
    QJsonObject jsonObject;

    //指令类型:获取校准信息
    jsonObject.insert("Class", QString::number(AWAJSON_cmd::CalibrationInfoCmd));

    //时间戳
    jsonObject.insert("Flag", getFlag());

    //cmd 获取校准信息总数指令
    jsonObject.insert("CMD", CalibrationInfo_cmd::AskCalibrationInfoNum);

    //仪器串号
    jsonObject.insert("DeviceAWAID",deviceNum);

    QString stringFull = "<N>";
    stringFull.append((QJsonDocument(jsonObject).toJson()));
    stringFull.append("</N>");
    stringFull = stringFull.simplified();
    stringFull.replace(" ", "");

    return stringFull.toLatin1();
}

//获取单条校准信息指令
QByteArray AWA6292Protocal::askCalibrationDataCmd(QString deviceNum, int count)
{
    QJsonObject jsonObject;

    //指令类型:获取校准信息
    jsonObject.insert("Class", QString::number(AWAJSON_cmd::CalibrationInfoCmd));

    //时间戳
    jsonObject.insert("Flag", getFlag());

    //cmd 获取单条校准信息指令
    jsonObject.insert("CMD", CalibrationInfo_cmd::AskCalibrationData);

    //仪器串号
    jsonObject.insert("DeviceAWAID",deviceNum);

    //指定校准计数
    jsonObject.insert("NUM",count);

    QString stringFull = "<N>";
    stringFull.append((QJsonDocument(jsonObject).toJson()));
    stringFull.append("</N>");
    stringFull = stringFull.simplified();
    stringFull.replace(" ", "");

    return stringFull.toLatin1();
}

//获取测量信息总数指令(6292中是文件夹是数量)
QByteArray AWA6292Protocal::askMeasureInfoNumCmd(QString deviceNum)
{
    QJsonObject jsonObject;

    //指令类型:获取测量信息指令
    jsonObject.insert("Class", QString::number(AWAJSON_cmd::MeasureInfoCmd));

    //时间戳
    jsonObject.insert("Flag", getFlag());

    //cmd 获取测量信息总数指令
    jsonObject.insert("CMD", MeasureInfo_cmd::AskMeasureInfoNum);

    //仪器串号
    jsonObject.insert("DeviceAWAID",deviceNum);

    QString stringFull = "<N>";
    stringFull.append((QJsonDocument(jsonObject).toJson()));
    stringFull.append("</N>");
    stringFull = stringFull.simplified();
    stringFull.replace(" ", "");

    return stringFull.toLatin1();
}

// 获取指定一条测量信息文件夹中的数据数量指令
QByteArray AWA6292Protocal::askOneMeasureInfoNumCmd(QString deviceNum, int FolderNo)
{
    QJsonObject jsonObject;

    //指令类型:获取测量信息指令
    jsonObject.insert("Class", QString::number(AWAJSON_cmd::MeasureInfoCmd));

    //时间戳
    jsonObject.insert("Flag", getFlag());

    //cmd 获取一条测量信息中数据数量指令
    jsonObject.insert("CMD", MeasureInfo_cmd::AskMeasureOneDataCount);

    //仪器串号
    jsonObject.insert("DeviceAWAID",deviceNum);

    //数据文件夹指代参数
    jsonObject.insert("FolderNo",FolderNo);

    QString stringFull = "<N>";
    stringFull.append((QJsonDocument(jsonObject).toJson()));
    stringFull.append("</N>");
    stringFull = stringFull.simplified();
    stringFull.replace(" ", "");

    return stringFull.toLatin1();
}

//获取指定一条测量信息文件夹中的一条指定数据数量指令
QByteArray AWA6292Protocal::askMeasureDataCmd(QString deviceNum, int FolderNo, int FileNo)
{
    QJsonObject jsonObject;

    //指令类型:获取测量信息指令
    jsonObject.insert("Class", QString::number(AWAJSON_cmd::MeasureInfoCmd));

    //时间戳
    jsonObject.insert("Flag", getFlag());

    //cmd 获取指定一条测量数据指令
    jsonObject.insert("CMD", MeasureInfo_cmd::AskMeasureOneData);

    //仪器串号
    jsonObject.insert("DeviceAWAID",deviceNum);

    //数据文件夹指代参数
    jsonObject.insert("FolderNo",FolderNo);

    //数据文件夹下一条数据的指代参数
    jsonObject.insert("FileNo",FileNo);

    QString stringFull = "<N>";
    stringFull.append((QJsonDocument(jsonObject).toJson()));
    stringFull.append("</N>");
    stringFull = stringFull.simplified();
    stringFull.replace(" ", "");

    return stringFull.toLatin1();
}

//获取实时数据指令
QByteArray AWA6292Protocal::askRealTimeDataCmd(QString deviceNum, QString Type)
{
    QJsonObject jsonObject;

    //指令类型:获取实时数据
    jsonObject.insert("Class", QString::number(AWAJSON_cmd::RealTimeInfoCmd));

    //时间戳
    jsonObject.insert("Flag", getFlag());

    //仪器串号
    jsonObject.insert("DeviceAWAID",deviceNum);

    //数据类型
    jsonObject.insert("Type",Type);

    QString stringFull = "<N>";
    stringFull.append((QJsonDocument(jsonObject).toJson()));
    stringFull.append("</N>");
    stringFull = stringFull.simplified();
    stringFull.replace(" ", "");

    return stringFull.toLatin1();
}

//读取文件指令
QByteArray AWA6292Protocal::askFileCmd(QString deviceNum, QString filePath)
{
    QJsonObject jsonObject;

    //指令类型:获取测量信息指令
    jsonObject.insert("Class", QString::number(AWAJSON_cmd::RealFileCmd));

    //时间戳
    jsonObject.insert("Flag", getFlag());

    //cmd 获取一条测量信息中数据数量指令
    jsonObject.insert("CMD", 0);

    //仪器串号
    jsonObject.insert("DeviceAWAID",deviceNum);

    //数据文件夹指代参数
    jsonObject.insert("FilePath",filePath);

    QString stringFull = "<N>";
    stringFull.append((QJsonDocument(jsonObject).toJson()));
    stringFull.append("</N>");
    stringFull = stringFull.simplified();
    //stringFull.replace(" ", "");

    return stringFull.toUtf8();
}

//取消读取文件指令
QByteArray AWA6292Protocal::stopAskFileCmd(QString deviceNum)
{
    QJsonObject jsonObject;

    //指令类型:获取测量信息指令
    jsonObject.insert("Class", QString::number(AWAJSON_cmd::RealFileCmd));

    //时间戳
    jsonObject.insert("Flag", getFlag());

    //cmd 获取一条测量信息中数据数量指令
    jsonObject.insert("CMD", 1);

    //仪器串号
    jsonObject.insert("DeviceAWAID",deviceNum);


    QString stringFull = "<N>";
    stringFull.append((QJsonDocument(jsonObject).toJson()));
    stringFull.append("</N>");
    stringFull = stringFull.simplified();
    stringFull.replace(" ", "");

    return stringFull.toUtf8();
}

/**分析测量数据结果并生成map数据发送到主界面*/
void AWA6292Protocal::analyOnePieceMeasureData(int id_d, int dataType, QStringList data)
{
    QString displayText;
    QJsonObject jsonText;
    QStringList jsonKey;
    //分析数据
    analysisTextList(data, dataType, displayText,jsonText,jsonKey);

    //查看是否有瞬时数据或录音数据(同一个文件夹下的数据进行关联)
    int folderNo = id_d / 100;
    int id_i = -1;
    QString id_s = "-1";
    if(folderNo == m_instant_folder)
    {
        id_i = m_id_i;
    }
    if(folderNo == m_sound_folder)
    {
        id_s = m_id_s_list;
    }

    //处理生成的json结果
    QString jsonData,jsonKeyStr;
    jsonData = QString(QJsonDocument(jsonText).toJson());
    //qDebug() << "解析结果json：" << jsonData;
    for(int i = 0 ; i < jsonKey.count(); i++)
    {
        jsonKeyStr.append(jsonKey[i] + ";");
    }

    //生成数据库写入需要的map数据
    QMap<QVariant, QVariant> mapData;

    //填充通用数据
    mapData.insert(":id_d", id_d);
    mapData.insert(":id_i", id_i);
    mapData.insert(":id_s", id_s);
    mapData.insert(":dataText", displayText.replace("~","～"));
    mapData.insert(":dataJson", jsonData);
    mapData.insert(":jsonKey", jsonKeyStr);
    mapData.insert(":instrumentType", jsonText["仪器型号"].toString());
    mapData.insert(":instrumentNum", jsonText["机号"].toString());
    mapData.insert(":measurePlace", jsonText["测点名"].toString());
    QString measureModel = jsonText["测量方式"].toString();
    measureModel = measureModel.left(measureModel.indexOf("-"));
    mapData.insert(":measureModel", measureModel);
    //数据类型
    //int typeSize = measureModel.length() - measureModel.indexOf("-") - 1;
    QString dataTypeText = dataTypeList[dataType];
    mapData.insert(":dataType", dataTypeText);
    //qDebug() << "收到132数据类型：" << dataTypeText;
    QDateTime  measureTime = QDateTime::fromString( jsonText["测量开始时间"].toString(), "yyyy-MM-dd hh:mm:ss");
    mapData.insert(":measureTime", measureTime);
    mapData.insert(":Ts", jsonText["Ts"].toString());
    mapData.insert(":Tm", jsonText["Tm"].toString());
    mapData.insert(":CalibratedSoundPressureLevel", jsonText["校准声压级"].toString().replace("dB",""));
    mapData.insert(":sensitivity", jsonText["灵敏度级"].toString().replace("dB",""));
    QDateTime  calibrationTime = QDateTime::fromString( jsonText["校准时间"].toString(), "yyyy-MM-dd hh:mm:ss");
    mapData.insert(":calibrationTime", calibrationTime);
    mapData.insert(":measureRange", jsonText["测量范围"].toString().replace("dBA",""));
    //经纬度
    mapData.insert(":LatitudeAndLongitude", jsonText["经纬度"].toString());
    mapData.insert(":Latitude", jsonText["测点经度"].toString());
    mapData.insert(":Longitude", jsonText["测点纬度"].toString());

    //根据6292的数据存储格式进行数据库内容填写
    switch (dataType)
    {
    case measureType::measureType_TolData:
    {
        mapData.insert(":LAFmax", jsonText["LAFmax"].toString());
        mapData.insert(":LCFmax", jsonText["LCFmax"].toString());
        mapData.insert(":LZFmax", jsonText["LZFmax"].toString());
        mapData.insert(":LAFmin", jsonText["LAFmin"].toString());
        mapData.insert(":LCFmin", jsonText["LCFmin"].toString());
        mapData.insert(":LZFmin", jsonText["LZFmin"].toString());
        mapData.insert(":LASmax", jsonText["LASmax"].toString());
        mapData.insert(":LCSmax", jsonText["LCSmax"].toString());
        mapData.insert(":LZSmax", jsonText["LZSmax"].toString());
        mapData.insert(":LASmin", jsonText["LASmin"].toString());
        mapData.insert(":LCSmin", jsonText["LCSmin"].toString());
        mapData.insert(":LZSmin", jsonText["LZSmin"].toString());
        mapData.insert(":LAImax", jsonText["LAImax"].toString());
        mapData.insert(":LCImax", jsonText["LCImax"].toString());
        mapData.insert(":LZImax", jsonText["LZImax"].toString());
        mapData.insert(":LAImin", jsonText["LAImin"].toString());
        mapData.insert(":LCImin", jsonText["LCImin"].toString());
        mapData.insert(":LZImin", jsonText["LZImin"].toString());
        mapData.insert(":LAeqT", jsonText["LAeq,T"].toString());
        mapData.insert(":LCeqT", jsonText["LCeq,T"].toString());
        mapData.insert(":LZeqT", jsonText["LZeq,T"].toString());
        mapData.insert(":LApeek", jsonText["LApeak"].toString());
        mapData.insert(":LCpeek", jsonText["LCpeak"].toString());
        mapData.insert(":LZpeek", jsonText["LZpeak"].toString());
        mapData.insert(":SELA", jsonText["SELA"].toString());
        mapData.insert(":SELC", jsonText["SELC"].toString());
        mapData.insert(":SELZ", jsonText["SELZ"].toString());
    }
        break;
    case measureType::measureType_StaData:
    {
        QStringList Weights = jsonText["计权"].toString().split("/");
        mapData.insert(":frequentWeight", Weights[0]);
        mapData.insert(":timeWeight", Weights[1]);
        mapData.insert(":L5", jsonText["L5"].toString());
        mapData.insert(":L10", jsonText["L10"].toString());
        mapData.insert(":L50", jsonText["L50"].toString());
        mapData.insert(":L90", jsonText["L90"].toString());
        mapData.insert(":L95", jsonText["L95"].toString());
        mapData.insert(":Lmax", jsonText["Lmax"].toString());
        mapData.insert(":Lmin", jsonText["Lmin"].toString());
        mapData.insert(":SEL", jsonText["SEL"].toString());
        mapData.insert(":SD", jsonText["SD"].toString());
        mapData.insert(":LeqT", jsonText["Leq,T"].toString());
    }
        break;
    case measureType::measureType_Expose:
        break;
    case measureType::measureType_OCT1:
    case measureType::measureType_OCT3:
    case measureType::measureType_FFT:
    case measureType::measureType_H24:
    {
        QStringList Weights = jsonText["计权"].toString().split("/");
        mapData.insert(":frequentWeight", Weights[0]);
        mapData.insert(":timeWeight", Weights[1]);
    }
        break;
    case measureType::measureType_INDOOR:
        mapData.insert(":roomType", jsonText["房间类型"].toString());
        break;
    default:;
    }

    //发送数据，写入数据库
    emit sigGetMeasureData(mapData, dataType);
}

/**分析瞬时数据结果并生成map数据发送到主界面*/
void AWA6292Protocal::analyOnePieceInstantData(int id_i, QString path)
{
    //查看是否有录音数据(同一个文件夹下的数据进行关联)
    int folderNo = id_i / 100;
    QString id_s = "";
    if(folderNo == m_sound_folder)
    {
        id_s = m_id_s_list;
    }

    //生成数据库写入需要的map数据
    QMap<QVariant, QVariant> mapData;
    mapData.insert(":id_i", id_i);
    mapData.insert(":id_s", id_s);
    mapData.insert(":filePath", path);
    QString fileName = path.right(path.length() - path.indexOf("ms/") - 3);
    fileName = fileName.left(fileName.indexOf("/"));
    //1628734554174-20210812001_总值积分
    //QDateTime measureTime = QDateTime::fromString(fileName.mid(fileName.indexOf("-") + 1, 8),"yyyyMMdd");
    qint64 timeFlg = fileName.left(fileName.indexOf("-")).toLongLong();;
    QDateTime measureTime = QDateTime::fromMSecsSinceEpoch(timeFlg);
    mapData.insert(":measureTime", measureTime);
    QString measurePoint = fileName.mid(fileName.indexOf("-") + 1, 11);
    mapData.insert(":measurePoint", measurePoint);

    QString dataType;
    //根据名称判断数据类型
    if(path.contains("总值积分"))
    {
        dataType = "TOTAL_DRE";
    }
    else if(path.contains("统计积分"))
    {
        dataType = "STATS_DRE";
    }
    else if(path.contains("OCT11"))
    {
        dataType = "1/1OCT_DRE";
    }
    else if(path.contains("OCT13"))
    {
        dataType = "1/3OCT_DRE";
    }
    else if(path.contains("室内"))
    {
        dataType = "INDOOR_DRE";
    }
    else
    {
        dataType = "";
    }
    mapData.insert(":dataType", dataType);

    emit sigGetInstantData(mapData);
}

/**分析录音数据结果并生成map数据发送到主界面*/
void AWA6292Protocal::analyOnePieceSoundRecodeData(int id_s, QString path)
{
    //生成数据库写入需要的map数据
    QMap<QVariant, QVariant> mapData;
    mapData.insert(":id_s", id_s);
    mapData.insert(":soundPath", path);
    QString fileName = path.right(path.length() - path.indexOf("ms/") - 3);
    fileName = fileName.left(fileName.indexOf("/"));
    //1628734554174-20210812001_总值积分
    //QDateTime measureTime = QDateTime::fromString(fileName.mid(fileName.indexOf("-") + 1, 8),"yyyyMMdd");
    qint64 timeFlg = fileName.left(fileName.indexOf("-")).toLongLong();;
    QDateTime measureTime = QDateTime::fromMSecsSinceEpoch(timeFlg);
    mapData.insert(":measureTime", measureTime);
    QString measurePoint = fileName.mid(fileName.indexOf("-") + 1, 11);
    mapData.insert(":measurePoint", measurePoint);

    emit sigGetSoundRecode(mapData);
}

/**分析传输的文件数据*/
void AWA6292Protocal::analyFileTrans(Buffer* buffer)
{
    if(m_isHead)
    {
        //第一个包时文件头数据
        m_isHead = false;

        QString data = QString::fromStdString(buffer->ToString());

        //帧头<N>索引
        int indexBegin=data.indexOf("<N>");
        indexBegin+=3;
        //帧尾</N>索引
        int indexEnd=data.indexOf("</N>");

    //    qDebug()<<"indexBegin:"<<indexBegin;
    //    qDebug()<<"indexEnd:"<<indexEnd;

        //条件判断，排除数据接收不完全情况
        if( 3!=indexBegin || indexEnd-indexBegin < 10)
        {
            emit sigHaveReadFileFromDevice(true , "");
            //标志位复原
            m_isReadFile = false;
            return;
        }
        else
        {
            //提取<N>和</N>之间的Json字符串
            QString strJson=data.mid(indexBegin,indexEnd-indexBegin);
            //初始化
            m_fileName = strJson.section("##",0,0);
            m_tempFileName = m_fileName;
            m_fileSize = strJson.section("##",1,1).toInt();
            m_receiveSize = 0;
            qDebug() << "m_fileName:" << m_fileName << "m_fileSize" << m_fileSize;
        }

        QString timeFlg = getFlag();
        QString filePath = QCoreApplication::applicationDirPath() + "/tempFile/" + timeFlg;
        QDir dir;
        if (!dir.exists(filePath))
        {
        bool res = dir.mkpath(filePath);
        qDebug() << "创建临时文件目录是否成功" << res;
            if(!res)
            {
                emit sigHaveReadFileFromDevice(true , "");
                //标志位复原
                m_isReadFile = false;
                return;
            }
        }

        m_fileName = filePath + "/" + m_fileName;

        //打开文件
        m_file.setFileName(m_fileName);
        bool isOK = m_file.open(QIODevice::WriteOnly);
        if(false == isOK)
        {
            qDebug() << "WriteOnly error 38";
        }

    }
    else
    {
//        QString dataz = QString::fromStdString(buffer->ToString());

//        //帧头<N>索引
//        int indexBegin=dataz.indexOf("<N>");
//        indexBegin+=3;
//        //帧尾</N>索引
//        int indexEnd=dataz.indexOf("</N>");

//        qDebug()<<"indexBegin:"<<indexBegin;
//        qDebug()<<"indexEnd:"<<indexEnd;

        buffer->Skip(3);
        buffer->Truncate(buffer->length() - 4);
       //QByteArray data = QByteArray::fromStdString(buffer->ToString()).left(buffer->length());
//        QByteArray data = QByteArray(buffer->data(),buffer->length());
//        qDebug() << "analyFileTrans 待写入文件数据：" << data << "-------------------------";
//        data = data.right(buffer->length() - 3); //去头
//        data = data.left(buffer->length() - 3 - 4); //去尾
//        qDebug()  << "analyFileTrans 去除头尾后文件数据："<< data << "-------------------------";

        qint64 len = m_file.write(buffer->data(),buffer->length());
        //清除缓存
        buffer->Skip(buffer->length());
        m_receiveSize += len;
        //qDebug() << "buffer->length()" << buffer->length() << "data.length() : "  <<  data.length() << "写入长度：" << len;
        //qDebug() << "以写入" << m_receiveSize << "byte, 已完成传输" << (100.00*m_receiveSize)/m_fileSize << "%";
        emit transmissionInfo(m_tempFileName + " " + QString::number((100.00*m_receiveSize)/m_fileSize ,'f',2) + "%" );
//        if(m_receiveSize == m_fileSize)
//        {
//            //标志位复原
//            m_isReadFile = false;
//            m_file.close();

//           qDebug() << "已经读取到足够长度了:" << m_fileName;

//            //发送文件读取完成信号
//            emit sigHaveReadFileFromDevice(false , m_fileName);
//            return;
//        }
        //客户端接收了多少，反馈给服务器
        //QString str = QString::number(receiveSize);
        //tcpSocket->write(str.toUtf8().data());

        //更新进度条
        //ui->progressBar->setValue(receiveSize/1024);
    }
}

 /**获取flag时间戳*/
QString AWA6292Protocal::getFlag()
{
    return QDateTime::currentDateTime().toString("yyyyMMddhhmmsszzz");
}

/**获取仪器是否授权了一个功能*/
bool AWA6292Protocal::isAuthrized(int num)
{
    //1为授权 0为未授权
    return (num == 1);
}

/**用于颠倒输入的字符串列表*/
QStringList AWA6292Protocal::ReverseOrder(QStringList strList)
{
    QStringList RorderStr;
    for(int i = (strList.count() -1); i >= 0 ; i--)
    {
        RorderStr << strList[i];
    }
    return RorderStr;
}

//主动停止接收文件
void AWA6292Protocal::slotStopToReadAWA6292File(bool isSendMessage)
{
    //停止文件读取
    qDebug() << "AWA6292Protocal::slotStopToReadAWA6292File 停止文件读取";

    //标志位复原
    if(m_isReadFile)
    {
        m_isReadFile = false;

        if(isSendMessage)
        {
            m_file.close();
            //主动发送文件读取失败的信号
            emit sigHaveReadFileFromDevice(true , "");
        }
    }

}
