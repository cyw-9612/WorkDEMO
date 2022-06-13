#include "awa5912protocal.h"
#include <singleton.h>
#include "logger/rtbaselogger.h"
#include <QDebug>
#include <QFile>
#include <QDateTime>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include "packParam.h"

AWA5912Protocal::AWA5912Protocal(IReceiveFilter *filter, ILogger *logger) :
    BaseProtocal(filter, logger)
{
    m_filer = filter;// 设置过滤器
    m_logger = logger;// 设置日志
    if(m_logger == nullptr)
    {
        m_logger = &Singleton<RTBaseLogger>::getInstance();
    }
    isFirstAskInfo = true;
}

void AWA5912Protocal::SetLogger(ILogger *logger)
{
    m_logger = logger;
}

// 获取协议过滤器
IReceiveFilter *AWA5912Protocal::GetFilter()
{
    return m_filer;
}

// 设置过滤器
void AWA5912Protocal::SetFilter(IReceiveFilter *filter)
{
    m_filer = filter;
}

// 校验数据是否合法
bool AWA5912Protocal::Check(Buffer *buffer)
{
    if(buffer != nullptr || buffer->length() > 0)
    {
        return true;// 数据正确
    }

    return false;// 数据错误
}

// 获取协议头
Slice AWA5912Protocal::GetHead(Buffer *buffer)
{
    if(buffer == nullptr || buffer->length() == 0)
    {
        return Slice();// 数据非法
    }

    return Slice(buffer->data(), 1);
}

// 获取协议尾巴
Slice AWA5912Protocal::GetTail(Buffer *buffer)
{
    if(buffer == nullptr || buffer->length() == 0)
    {
        return Slice();// 数据非法
    }

    return Slice(buffer->WriteBegin() - 1,  1);
}

// 解析数据
void AWA5912Protocal::Analysis(Buffer *buffer)
{
//    //Q_UNUSED(buffer);
//    //将获取到的数据根据既定通信协议进行处理
//    QString data = QString::fromStdString(buffer->ToString());
//    //去除无效数据
//    data = data.left(buffer->length());

//    QString deviceName = QString::fromStdString(m_name);

    QByteArray byArr;
    byArr.resize(buffer->length());
    for(int i=0;i<buffer->length();i++)
    {
        byArr[i]=buffer->data()[i];
    }

    qDebug() << "BaseProtocal::Analysis 待处理数据为: AWA5912" << byArr.toHex();

//    if( 0x0e==(unsigned char)byArr.at(11) )
//    {
        recDataAnalysis(byArr);
//    }
//    else
//    {
//        recDataAnalysis(data.toLatin1());
//    }

}

// 打包数据
void AWA5912Protocal::Package(Buffer *buffer, QVariant &para)
{
    Q_UNUSED(buffer);
    Q_UNUSED(para);

    return;
}

// 获取设备名
std::string AWA5912Protocal::GetDeviceName()
{
    return m_name;
}

// 设置设备名
void AWA5912Protocal::SetDeviceName(std::string name)
{
    m_name = name;
}

//发送读取命令 参数numRead: 读取第num组数据（1-16）,0 读基本信息
QByteArray AWA5912Protocal::getCmdRead(int numRead)
{
    try
    {

        QString head="<N>AUTH";
        QString tail="</N>";

        packRead packR;
        memset(&packR,0,sizeof(packR));

        //帧头
        memcpy(packR.head,head.toLatin1(),head.size());

        //长度
        packR.len=sizeof(packRead);
        //命令字：6 读取数据
        packR.cmd=6;
        //读取仪器设置信息
        packR.num=numRead;

        //帧尾
        memcpy(packR.tail,tail.toLatin1(),tail.size());

        QByteArray byteArray;
        //分配结构大小的空间，不可缺少
        byteArray.resize(sizeof(packRead));

        //结构体转QByteArray
        //byteArray.append((char*)&packW, sizeof(packWrite));
        memcpy(byteArray.data(), &packR, sizeof(packRead));

        //转成十六进制字符串
        QString strHex=byteArray.toHex();

        //从十六进制字符串转成字符串数组
        QByteArray byteHex=QByteArray::fromHex (strHex.toLatin1());

        return byteHex;

    }
    catch(std::exception* e)
    {
        QString strTittle=QString("CMainDlg::sendCmdRead");
        qDebug()<<strTittle;

        return 0;
    }
    catch(...)
    {
        QString strTittle=QString("CMainDlg::sendCmdRead");
        qDebug()<<strTittle;

        return 0;
    }
}

//发送清空命令
QByteArray AWA5912Protocal::getCmdClear()
{
    try
    {
        QString head="<N>AUTH";
        QString tail="</N>";

        packClearData pack;
        memset(&pack,0,sizeof(pack));

        //帧头
        memcpy(pack.head,head.toLatin1(),head.size());

        //长度
        pack.len=sizeof(packClearData);
        //命令字：9 清空数据
        pack.cmd=9;
        //清空
        pack.num=0;

        //帧尾
        memcpy(pack.tail,tail.toLatin1(),tail.size());

        QByteArray byteArray;
        //分配结构大小的空间，不可缺少
        byteArray.resize(sizeof(packClearData));

        //结构体转QByteArray
        memcpy(byteArray.data(), &pack, sizeof(packClearData));

        //转成十六进制字符串
        QString strHex=byteArray.toHex();
        qDebug()<<"clear Cmd:"<<strHex;
        //从十六进制字符串转成字符串数组
        QByteArray byteHex=QByteArray::fromHex (strHex.toLatin1());
        //发送指令
        //m_SerialPort->sendData(byteHex);
        //emit sigSend(byteHex);  //2020/07/22 解决QSerialport 跨线程问题

        return byteHex;
    }
    catch(std::exception* e)
    {
        QString strTittle=QString("CMainDlg::sendCmdClear");
        qDebug()<<strTittle;

//        Msg msg(this);
//        msg.setText(strTittle,e->what(),Msg::MsgMode::info);
//        msg.exec();

        return 0;
    }
    catch(...)
    {
        QString strTittle=QString("CMainDlg::sendCmdClear");
        qDebug()<<strTittle;

//        Msg msg(this);
//        msg.setText(strTittle,"error",Msg::MsgMode::info);
//        msg.exec();

        return 0;
    }
}

//发送删除命令:删除指定组号的数据,参数numDelete:要删除的组号(1-16),0 清空
QByteArray AWA5912Protocal::sendCmdDelete(int numDelete)
{
    try
    {
        QString head="<N>AUTH";
        QString tail="</N>";

        packClearData pack;
        memset(&pack,0,sizeof(pack));

        //帧头
        memcpy(pack.head,head.toLatin1(),head.size());

        //长度
        pack.len=sizeof(packClearData);
        //命令字：9 清空数据
        pack.cmd=9;
        //删除指定组号的数据
        pack.num=numDelete;

        //帧尾
        memcpy(pack.tail,tail.toLatin1(),tail.size());

        QByteArray byteArray;
        //分配结构大小的空间，不可缺少
        byteArray.resize(sizeof(packClearData));

        //结构体转QByteArray
        //byteArray.append((char*)&packW, sizeof(packWrite));
        memcpy(byteArray.data(), &pack, sizeof(packClearData));

        //转成十六进制字符串
        QString strHex=byteArray.toHex();
        qDebug()<<"delete Cmd:"<<strHex;
        //从十六进制字符串转成字符串数组
        QByteArray byteHex=QByteArray::fromHex (strHex.toLatin1());
        //发送指令
        //m_SerialPort->sendData(byteHex);
        //emit sigSend(byteHex);  //2020/07/22 解决QSerialport 跨线程问题

        return byteHex;
    }
    catch(std::exception* e)
    {
        QString strTittle=QString("CMainDlg::sendCmdDelete");
        qDebug()<<strTittle;

//        Msg msg(this);
//        msg.setText(strTittle,e->what(),Msg::MsgMode::info);
//        msg.exec();

        return 0 ;
    }
    catch(...)
    {
        QString strTittle=QString("CMainDlg::sendCmdDelete");
        qDebug()<<strTittle;

//        Msg msg(this);
//        msg.setText(strTittle,"error",Msg::MsgMode::info);
//        msg.exec();

        return 0;
    }

}

//发送复位命令
QByteArray AWA5912Protocal::sendCmdReset()
{
    string str="AWAKs";

    QString strShow=QString(str.data());
    qDebug()<<"Reset Cmd:"<<strShow.toLatin1().toHex();

    return strShow.toLatin1();
    //m_SerialPort->sendData(str);
}

//下发模板指令
QByteArray AWA5912Protocal::sendCmdWriteTemplate(QJsonObject dataJson)
{
    //帧头
    QString head="<N>AUTH";
    //提取数据
    QString DateSet = dataJson.value("DateSet").toString();
    QString TimeSet = dataJson.value("TimeSet").toString();
    QString TimeSustain = dataJson.value("TimeSustain").toString();
    QString TimeWeight = dataJson.value("TimeWeight").toString();
    QString FrequencyWeight = dataJson.value("FrequencyWeight").toString();
    QString Dose = dataJson.value("Dose").toString();
    QString ChangeRate = dataJson.value("ChangeRate").toString();
    QString ThresholdValue = dataJson.value("ThresholdValue").toString();
    QString NLpeak1 = dataJson.value("NLpeak1").toString();
    QString NLpeak2 = dataJson.value("NLpeak2").toString();
    QString NLpeak3 = dataJson.value("NLpeak3").toString();
    QString lockstate = dataJson.value("lockstate").toString();
    //帧尾
    QString tail="</N>";

    //创建结构体
    packSetTemplate packW;
    memset(&packW,0,sizeof(packW));

    //帧头
    memcpy(packW.head,head.toLatin1(),head.size());

    //长度
    packW.len=sizeof(packSetTemplate);
    //命令字
    packW.cmd=4;
    //日期
    memcpy(packW.DateSet,DateSet.toLatin1(),DateSet.size());
    //时间
    memcpy(packW.TimeSet,TimeSet.toLatin1(),TimeSet.size());
    //Ts
    memcpy(packW.TimeSustain,TimeSustain.toLatin1(),TimeSustain.size());
    //时间计权
    memcpy(packW.TimeWeight,TimeWeight.toLatin1(),TimeWeight.size());
    //频率计权
    memcpy(packW.FrequencyWeight,FrequencyWeight.toLatin1(),FrequencyWeight.size());
    //Dose
    memcpy(packW.Dose,Dose.toLatin1(),Dose.size());
    //变化率
    memcpy(packW.ChangeRate,ChangeRate.toLatin1(),ChangeRate.size());
    //门限
    memcpy(packW.ThresholdValue,ThresholdValue.toLatin1(),ThresholdValue.size());
    //超过峰值设定
    memcpy(packW.NLpeak1,NLpeak1.toLatin1(),NLpeak1.size());
    memcpy(packW.NLpeak2,NLpeak2.toLatin1(),NLpeak2.size());
    memcpy(packW.NLpeak3,NLpeak3.toLatin1(),NLpeak3.size());

    //状态
    memcpy(packW.lockstate,lockstate.toLatin1(),lockstate.size());

    //帧尾
    memcpy(packW.tail,tail.toLatin1(),tail.size());

    QByteArray byteArray;
    //分配结构大小的空间，不可缺少
    byteArray.resize(sizeof(packSetTemplate));

    //结构体转QByteArray
    memcpy(byteArray.data(), &packW, sizeof(packSetTemplate));

    //转成十六进制字符串
    QString strHex=byteArray.toHex();
    //qDebug()<<"AWA5912Protocal::sendCmdWriteTemplate:"<<strHex;
    //从十六进制字符串转成字符串数组
    QByteArray byteHex=QByteArray::fromHex (strHex.toLatin1());

    return byteHex;
}

//基于授权协议的仪器信息读取命令
QByteArray AWA5912Protocal::sendCurrencyCmdReadInfo()
{
    //定义授权结构体
    currencyPackInfo pack;
    //帧头
    QString head = "<N>AUTH";
    //命令字:4 读取仪器串号信息
    int cmd = 4;
    //帧尾
    QString tail = "</N>";

    memset(&pack,0,sizeof(pack));
    //帧头
    memcpy(pack.head,head.toLatin1(),head.size());
    //长度,下位机授权协议中包长不含有包头尾
    pack.len = 12;
    //命令字
    pack.cmd = cmd;
    //帧尾
    memcpy(pack.tail,tail.toLatin1(),tail.size());

    QByteArray byteArray;
    //分配结构大小的空间，不可缺少
    byteArray.resize(sizeof(pack));

    //结构体转QByteArray
    memcpy(byteArray.data(), &pack, sizeof(pack));

    //转成十六进制字符串
    QString strHex = byteArray.toHex();
    //qDebug()<<"结构体数据为:"<<strHex;
    //从十六进制字符串转成字符串数组
    QByteArray byteHex = QByteArray::fromHex (strHex.toLatin1());
    return  byteHex;
}

//发送设置仪器中时间列表指令
QByteArray AWA5912Protocal::sendCmdSetTimeList(QJsonObject dataJson)
{
    try
    {
        QString head="<N>AUTH";
        QString tail="</N>";

        packSetTimeList pack;
        memset(&pack,0,sizeof(pack));

        //帧头
        memcpy(pack.head,head.toLatin1(),head.size());

        //长度
        pack.len=sizeof(packSetTimeList) - 7;
        //命令字：0b 设置TS列表
        pack.cmd= 0x0b;

         //数据
        for(int i = 0; i< 50; i++)
        {
            QString indexStr = QString("timeList%1").arg(i);
            int num = dataJson.value(indexStr).toInt();
            //到0则暂停
            if(num == 0)
                continue;

            pack.tslist[i] = num;
        }

        //帧尾
        memcpy(pack.tail,tail.toLatin1(),tail.size());

        QByteArray byteArray;
        //分配结构大小的空间，不可缺少
        byteArray.resize(sizeof(packSetTimeList));

        //结构体转QByteArray
        memcpy(byteArray.data(), &pack, sizeof(packSetTimeList));

        //转成十六进制字符串
        QString strHex=byteArray.toHex();
        qDebug()<<"SetTimeList Cmd:"<<strHex;
        //从十六进制字符串转成字符串数组
        QByteArray byteHex=QByteArray::fromHex (strHex.toLatin1());
        //发送指令

        return byteHex;
    }
    catch(std::exception* e)
    {
        QString strTittle=QString("CMainDlg::sendCmdSetTimeList");
        qDebug()<<strTittle;

//        Msg msg(this);
//        msg.setText(strTittle,e->what(),Msg::MsgMode::info);
//        msg.exec();

        return 0;
    }
    catch(...)
    {
        QString strTittle=QString("CMainDlg::sendCmdSetTimeList");
        qDebug()<<strTittle;

//        Msg msg(this);
//        msg.setText(strTittle,"error",Msg::MsgMode::info);
//        msg.exec();

        return 0;
    }

}

//发送读取仪器中现有时间列表指令
QByteArray AWA5912Protocal::sendCmdReadTimeList()
{
    try
    {
        QString head="<N>AUTH";
        QString tail="</N>";

        packRedTimeList pack;
        memset(&pack,0,sizeof(pack));

        //帧头
        memcpy(pack.head,head.toLatin1(),head.size());

        //长度
        pack.len=sizeof(packRedTimeList) - 7;
        //命令字：0d 读取TS列表
        pack.cmd= 0x0d;

        //帧尾
        memcpy(pack.tail,tail.toLatin1(),tail.size());

        QByteArray byteArray;
        //分配结构大小的空间，不可缺少
        byteArray.resize(sizeof(packClearData));

        //结构体转QByteArray
        memcpy(byteArray.data(), &pack, sizeof(packClearData));

        //转成十六进制字符串
        QString strHex=byteArray.toHex();
        qDebug()<<"ReadTimeList Cmd:"<<strHex;
        //从十六进制字符串转成字符串数组
        QByteArray byteHex=QByteArray::fromHex (strHex.toLatin1());
        //发送指令
        //m_SerialPort->sendData(byteHex);
        //emit sigSend(byteHex);  //2020/07/22 解决QSerialport 跨线程问题

        return byteHex;
    }
    catch(std::exception* e)
    {
        QString strTittle=QString("CMainDlg::sendCmdReadTimeList");
        qDebug()<<strTittle;

//        Msg msg(this);
//        msg.setText(strTittle,e->what(),Msg::MsgMode::info);
//        msg.exec();

        return 0;
    }
    catch(...)
    {
        QString strTittle=QString("CMainDlg::sendCmdReadTimeList");
        qDebug()<<strTittle;

//        Msg msg(this);
//        msg.setText(strTittle,"error",Msg::MsgMode::info);
//        msg.exec();

        return 0;
    }

}

//接收数据解析
void AWA5912Protocal::recDataAnalysis(QByteArray buffer)
{
    //排除长度不足的干扰数据
    if(buffer.size() <19)
    {
        return;
    }

    //指令类型4:上传基本信息
    if( 0x04==(unsigned char)buffer.at(11) )
    {
        //qDebug() << "data received:" << buffer;
        packRecvTemplateInformation * receive = (packRecvTemplateInformation *)buffer.data();
        QJsonObject templateJson;
        templateJson.insert("hasData", true);
        templateJson.insert("DateSet", receive->DateSet);
        templateJson.insert("TimeSet", receive->TimeSet);
        templateJson.insert("TimeSustain", receive->TimeSustain);
        templateJson.insert("TimeWeight", receive->TimeWeight);
        templateJson.insert("FrequencyWeight", receive->FrequencyWeight);
        templateJson.insert("Dose", receive->Dose);
        templateJson.insert("ChangeRate", receive->ChangeRate);
        templateJson.insert("ThresholdValue", receive->ThresholdValue);
        templateJson.insert("NLpeak1", receive->NLpeak1);
        templateJson.insert("NLpeak2", receive->NLpeak2);
        templateJson.insert("NLpeak3", receive->NLpeak3);
        templateJson.insert("lockstate", receive->lockstate);

        emit sigGetTemplateInfo(templateJson);
    }

    //指令类型7:上传全部数据
    if( 0x07==(unsigned char)buffer.at(11) )
    {
        packRecvAll *receiveRec = (packRecvAll *)buffer.data();
        packRecvAll *receive=new packRecvAll();
        //提取数据
        receive= &extractPack(receiveRec);
        //qDebug() << "data received:" << ;

        //首次查询是获取仪器本机信息
        if(isFirstAskInfo)
        {
            isFirstAskInfo = false;
            qDebug() << "AWA5912仪器连接成功，串号为:" << receive->instrumentSerialNumber;
            QString deviceNum = receive->instrumentSerialNumber;
            emit connectDeviceInfo("AWA5912", deviceNum);
        }
        else if(1 == receive->state) //如果有数据
        {
            QMap<QVariant,QVariant> map;
            map.insert(":id_d",receive->num); //数据编号
            map.insert(":id_i",-1); //数据编号
            map.insert(":id_s",-1); //数据编号
            map.insert(":dataType","Expose");
            map.insert(":measureModel","声暴露计");
            map.insert(":instrumentType",receive->instrumentType); //仪器信号
            map.insert(":instrumentNum",receive->instrumentSerialNumber); //仪器串号
            QString startTime = receive->StartTime;
            QDateTime start = QDateTime::fromString(startTime,"yyyy-MM-dd hh:mm:ss");
            map.insert(":measureTime",start); //测量开始时间
            map.insert(":E",receive->Exposure); //声暴露量
            map.insert(":DOSE",receive->Dose); //噪声计量
            map.insert(":LeqT",receive->Leq_t); //Leq_t
            map.insert(":LEX8h",receive->Lex_8h); //Lex_8h
            map.insert(":Lmax",receive->Lmax); //Lmax
            map.insert(":Lmin",receive->Lmin); //Lmin
            map.insert(":SEL",receive->SEL); //SEL
            map.insert(":sensitivity",receive->SensitivityLevel); //灵敏度级
            map.insert(":LCpeek",receive->LCpeak); //LCpeak
            map.insert(":CalibratedSoundPressureLevel",receive->CALSPL); //校准声压级
            QString calibrationTime = receive->CalibrationDate;
            QDateTime calibration = QDateTime::fromString(calibrationTime,"yyyy-MM-dd hh:mm:ss");
            map.insert(":calibrationTime",calibration); //校准日期
            QString Ts = receive->TS;
            Ts = Ts.replace("h",":").replace("min",":").replace("s","");
            map.insert(":Ts",Ts); //测量时间
            QString dataText;

            dataText.append("数据组: " + QString::number(receiveRec->num) + "\n");
            dataText.append("仪器型号: " + QString(receiveRec->instrumentType) + "\n");
            dataText.append("仪器串号: " + QString(receiveRec->instrumentSerialNumber) + "\n");
            dataText.append("测量开始时间: " + QString(receiveRec->StartTime) + "\n");
            dataText.append("测量结束时间: " + QString(receiveRec->EndTime) + "\n");
            dataText.append(QString(receiveRec->Exposure) + "\n");
            dataText.append(QString(receiveRec->Dose) + "\n");
            dataText.append(QString(receiveRec->Leq_t) + "\n");
            dataText.append(QString(receiveRec->Lex_8h) + "\n");
            dataText.append(QString(receiveRec->Lmax).left(16) + "\n");
            dataText.append(QString(receiveRec->Lmin).left(16) + "\n");
            QString N1 = QString(receiveRec->NLpeak1);
            QString N2 = QString(receiveRec->NLpeak2);
            QString N3 = QString(receiveRec->NLpeak3);
            dataText.append("N(" + N1.left( N1.indexOf("=")).replace("L","LA") + ")" +N1.right(N1.length() - N1.indexOf("="))  + "\n");
            dataText.append("N(" + N2.left( N2.indexOf("=")).replace("L","LA") + ")" +N2.right(N2.length() - N2.indexOf("="))  + "\n");;
            dataText.append("N(" + N3.left( N3.indexOf("=")).replace("L","LA") + ")" +N3.right(N3.length() - N3.indexOf("="))  + "\n");
            QString lcpeakStr = QString(receiveRec->LCpeak).replace("c", "C");
            if(lcpeakStr.contains("SEL"))
            {
                lcpeakStr = lcpeakStr.left(lcpeakStr.indexOf("SEL"));
            }
            dataText.append(lcpeakStr + "\n");
            dataText.append(QString(receiveRec->SEL) + "\n");
            dataText.append(QString(receiveRec->SensitivityLevel) + "\n");
            dataText.append(QString(receiveRec->CALSPL) + "\n");
            dataText.append(QString(receiveRec->CalibrationDate) + "\n");
            dataText.append(QString(receiveRec->TS).replace("S","s") + "\n");
            dataText.append(QString(receiveRec->OverLoad));

            map.insert(":dataText",dataText); //数据文本

            QString jsonData,jsonKey;
            jsonKey = "E;Dose;Leq,T;Lex,8h;Lmax;Lmin;LCpeak;SEL";
            QJsonObject json;
            json.insert("E",receive->Exposure);
            json.insert("Dose",receive->Dose);
            json.insert("Leq,T",receive->Leq_t);
            json.insert("Lex,8h",receive->Lex_8h);
            json.insert("Lmax",receive->Lmax);
            json.insert("Lmin",receive->Lmin);
            json.insert("LCpeak",receive->LCpeak);
            json.insert("SEL",receive->SEL);
            jsonData = QString(QJsonDocument(json).toJson());
            map.insert(":dataJson",jsonData);
            map.insert(":jsonKey",jsonKey);

            //发送信息
            emit sigGetData(map,receive);
        }

    }

    //基于授权协议的仪器信息读取返回信息
    if( 0x05==(unsigned char)buffer.at(11) && buffer.size() == sizeof (currencyPackRecvInfo))
    {
        currencyPackRecvInfo *receiveRec = (currencyPackRecvInfo *)buffer.data();
        QJsonObject dataJson;
        dataJson.insert("instrumentSerialType", "AWA5912");
        dataJson.insert("instrumentSerialNumber", receiveRec->instrumentSerialNumber);
        dataJson.insert("microphoneSerialType", "");
        dataJson.insert("microphoneSerialNumber", "");
        dataJson.insert("hardwareVersion",receiveRec->hardwareVersion);
        dataJson.insert("softwareVersion",receiveRec->softwareVersion);

        //装填功能 总计8个功能
        dataJson.insert("total",false);
        dataJson.insert("sta",false);
        dataJson.insert("OCT1",false);
        dataJson.insert("OCT3",false);
        dataJson.insert("FFT",false);
        dataJson.insert("expose",true);
        dataJson.insert("Sti",false);
        dataJson.insert("H24",false);
        dataJson.insert("indoor",false);
        dataJson.insert("reverberation",false);

        emit sigGetAuthInfo(dataJson);
    }

    //收到下位机返回的设置信息 05设置成功,0A设置失败
    //下位机返回：设置成功
    if( 0x05==(unsigned char)buffer.at(11) )
    {
        QString str=QString(buffer.at(11));
        //qDebug()<<"str"<<str;
        qDebug()<<"仪器回复: 指令成功";
        emit sigSetResult(true);
    }

    //下位机返回：设置失败
    if( 0x0a==(unsigned char)buffer.at(11) )
    {
        QString str=QString(buffer.at(11));
        //qDebug()<<"str"<<str;
        qDebug()<<"仪器回复: 测量过程中不允许操作,请结束测量";
        emit sigSetResult(false);
    }

    //设置时间列表结果
    if( 0x0c==(unsigned char)buffer.at(11) )
    {
        QString str=QString(buffer.at(11));

        //qDebug()<<"str"<<str;
        qDebug()<<"仪器回复: 设置时间列表成功";
        emit sigSetTimeListResult();
    }

    //读取时间列表结果
    if( 0x0e==(unsigned char)buffer.at(11) )
    {
        QString str=QString(buffer.at(11));

        packRedTimeReply *receiveRec = (packRedTimeReply *)buffer.data();
        QJsonObject dataJson;
        for(int i = 0; i < 50; i++)
        {
            QString str = QString("timeList%1").arg(i);
            dataJson.insert(str, receiveRec->tslist[i]);
        }

        //qDebug()<<"str"<<str;
        qDebug()<<"仪器回复: 获取仪器时间列表";
        emit sigGetTimeListResult(dataJson);
    }
}

//用于心跳检测的指令解析结果
bool AWA5912Protocal::isSerialOnline(QByteArray buffer)
{
    //排除长度不足的干扰数据
    if(buffer.size() <19)
    {
        return false;
    }

    //指令类型4 发送心跳内容就是查询仪器信息，返回其他则为错
    if( 0x04==(unsigned char)buffer.at(11) )
    {
        emit sigSerialIsOnline();
        return true;
    }

    return false;
}

//提取协议包中的参数数据: 返回修改后的参数结构体 packRecvAll
packRecvAll& AWA5912Protocal::extractPack(packRecvAll * packRecAll)
{
    packRecvAll* pack=new packRecvAll();
    //packRecvAll* pack=new packRecvAll(packRecAll);

    QString str="";
    int index1=0;
    int index2=0;

    //首先判断是否有有效数据
    pack->state = packRecAll->state;
    if(!isFirstAskInfo)
    {
        if(0 == pack->state)
        {
            //无有效数据之间返回空
            return *pack;
        }
    }

    //机型
    str.clear();
    str=QString(packRecAll->instrumentType);
    memcpy(pack->instrumentType,str.toLatin1(),str.size());

    //机号
    str.clear();
    str=QString(packRecAll->instrumentSerialNumber);
    memcpy(pack->instrumentSerialNumber,str.toLatin1(),str.size());

    //组号
    pack->num=packRecAll->num;
    //qDebug() << " 解析一组有用的数据 ：数据标号为" << pack->num ;

    //校准日期
    str.clear();
    str=QString(packRecAll->CalibrationDate);
    index1=str.indexOf("=");
    //index2=str.indexOf("d");

    //提取数字再赋值给结构体
    if(index1>0)
    {
        str=str.mid(index1+1);
        memcpy(pack->CalibrationDate,str.toLatin1(),str.size());
    }


    //新灵敏度级
    str.clear();
    str=QString(packRecAll->SensitivityLevel);
    index1=str.indexOf("=");
    index2=str.indexOf("d");

    //提取数字再赋值给结构体
    if(index1>0 && index2>0)
    {
        str=str.mid(index1+1,index2-index1-1);
        memcpy(pack->SensitivityLevel,str.toLatin1(),str.size());
    }

    //校准声压级
    str.clear();
    str=QString(packRecAll->CALSPL);
    index1=str.indexOf("=");
    index2=str.indexOf("d");

    //提取数字再赋值给结构体
    if(index1>0 && index2>0)
    {
        str=str.mid(index1+1,index2-index1-1);
        memcpy(pack->CALSPL,str.toLatin1(),str.size());
    }

    //Ts
    str.clear();
    str=QString(packRecAll->TS);
    index1=str.indexOf("=");
    //index2=str.indexOf("d");

    //提取数字再赋值给结构体
    if(index1>0)
    {
        str=str.mid(index1+1);
        memcpy(pack->TS,str.toLatin1(),str.size());
    }


    //OverLoad 过载标志
    str.clear();
    str=QString(packRecAll->OverLoad);
    index1=str.indexOf("=");
    //index2=str.indexOf("d");

    //提取数字再赋值给结构体
    if(index1>0)
    {
        str=str.mid(index1+1);
        memcpy(pack->OverLoad,str.toLatin1(),str.size());
        //qDebug()<<"overload:"<<str;
        //qDebug()<<"pack->OverLoad:"<<pack->OverLoad;
    }



    //开始时间
    str.clear();
    str=QString(packRecAll->StartTime);
    memcpy(pack->StartTime,str.toLatin1(),str.size());

    //结束时间
    str.clear();
    str=QString(packRecAll->EndTime);
    memcpy(pack->EndTime,str.toLatin1(),str.size());

    //声暴露E
    str.clear();
    str=QString(packRecAll->Exposure);
    index1=str.indexOf("=");
    //忽略大小写
    index2=str.indexOf("p",0,Qt::CaseInsensitive);

    //提取'='和'p'之间的数字再赋值给结构体
    if(index1>0 && index2>0)
    {
        str=str.mid(index1+1,index2-index1-1);
        memcpy(pack->Exposure,str.toLatin1(),str.size());
    }

    //Dose
    str.clear();
    str=QString(packRecAll->Dose);
    index1=str.indexOf("=");
    index2=str.indexOf("%");

    //提取'='和'%'之间的数字再赋值给结构体
    if(index1>0 && index2>0)
    {
        str=str.mid(index1+1,index2-index1-1);
        memcpy(pack->Dose,str.toLatin1(),str.size());
    }

    //Leq_t
    str.clear();
    str=QString(packRecAll->Leq_t);
    index1=str.indexOf("=");
    index2=str.indexOf("d");

    //提取数字再赋值给结构体
    if(index1>0 && index2>0)
    {
        str=str.mid(index1+1,index2-index1-1);
        memcpy(pack->Leq_t,str.toLatin1(),str.size());
    }

    //Lex_8h
    str.clear();
    str=QString(packRecAll->Lex_8h);
    index1=str.indexOf("=");
    index2=str.indexOf("d");

    //提取数字再赋值给结构体
    if(index1>0 && index2>0)
    {
        str=str.mid(index1+1,index2-index1-1);
        memcpy(pack->Lex_8h,str.toLatin1(),str.size());
    }

    //LCpeak
    str.clear();
    str=QString(packRecAll->LCpeak);
    index1=str.indexOf("=");
    index2=str.indexOf("d");

    //提取数字再赋值给结构体  2020/06/23 修改
    if(index1>0 && index2>0)
    {
        str=str.mid(index1+1,index2-index1-1);
        memcpy(pack->LCpeak,str.toLatin1(),str.size());
    }

    //SEL
    str.clear();
    str=QString(packRecAll->SEL);
    index1=str.indexOf("=");
    index2=str.indexOf("d");

    //提取数字再赋值给结构体
    if(index1>0 && index2>0)
    {
        str=str.mid(index1+1,index2-index1-1);
        memcpy(pack->SEL,str.toLatin1(),str.size());
    }

    //Lmax
    str.clear();
    str=QString(packRecAll->Lmax);
    index1=str.indexOf("=") + 0; //不去除F：
    index2=str.indexOf("d");

    //提取数字再赋值给结构体
    if(index1>0 && index2>0)
    {
        str=str.mid(index1+1,index2-index1-1);
        memcpy(pack->Lmax,str.toLatin1(),str.size());
    }

    //Lmin
    str.clear();
    str=QString(packRecAll->Lmin);
    index1=str.indexOf("=") + 0; //不去除F：
    index2=str.indexOf("d");

    //提取数字再赋值给结构体
    if(index1>0 && index2>0)
    {
        str=str.mid(index1+1,index2-index1-1);
        memcpy(pack->Lmin,str.toLatin1(),str.size());
    }


    //为了导出时候方便提取内容，此处不再进行提取，原封不动拷贝
    str.clear();
    str=QString(packRecAll->NLpeak1);
    memcpy(pack->NLpeak1,str.toLatin1(),str.size());

    str.clear();
    str=QString(packRecAll->NLpeak2);
    memcpy(pack->NLpeak2,str.toLatin1(),str.size());

    str.clear();
    str=QString(packRecAll->NLpeak3);
    memcpy(pack->NLpeak3,str.toLatin1(),str.size());

    return *pack;

}
