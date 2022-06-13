#include "cdatabase.h"
#include<QDebug>
#include<QMessageBox>
#include<QCoreApplication>
#include<QDir>
#include "cfile.h"


CDataBase::CDataBase(const QString strConnName)
{
    //数据库连接名称
    m_strConnName=strConnName;

    initDataBase();
}

CDataBase::~CDataBase()
{
    m_sqlConn.close();

    qDebug()<<"释放数据库";
}

QString CDataBase::getDbPath()
{
    //获取程序当前运行目录
    QString strDir = QCoreApplication::applicationDirPath();
    //数据库目录
    strDir +="/DataBase";

    //路径存在,返回true;路径不存在,则创建并返回创建结果:true 成功,false失败
    if(CFile::isDirExist(strDir)==false)
    {
        qDebug()<<"create db path failed!";
        return QStringLiteral("");
    }

    //数据库名称
    const QString strDbName="dataBase.db";

    //数据库完整路径=路径名+文件名
    QString strDbPath=QString("%1//%2").arg(strDir).arg(strDbName);
    return strDbPath;
}

bool CDataBase::initDataBase()
{
    //获取数据库驱动列表
    QStringList listDriver = QSqlDatabase::drivers();
//    foreach(QString driver, listDriver)
//        qDebug() <<"/t" << driver;

    //检查SQLITE驱动是否存在
    if(listDriver.contains("QSQLITE")==false)
    {
        qDebug()<<"未检测到SQLITE驱动!";
        QMessageBox::information(0, QObject::tr("Tips"), QObject::tr("未检测到SQLITE驱动!"));
        return false;
    }

    //获取数据库连接,没有则创建
    //if (QSqlDatabase::contains(QSqlDatabase::defaultConnection))
    if (QSqlDatabase::contains(m_strConnName))
    {
        m_sqlConn = QSqlDatabase::database(m_strConnName);
    }
    else
    {
        //使用SQLite数据库驱动创建数据库连接 m_strConnName
        m_sqlConn = QSqlDatabase::addDatabase("QSQLITE",m_strConnName);
    }

    //获取数据库路径
    QString dbPath=getDbPath();

    m_sqlConn.setDatabaseName(dbPath); //设置数据库名称
    m_sqlConn.setHostName("127.0.0.1");//设置主机
    m_sqlConn.setUserName("admin"); //设置用户名
    m_sqlConn.setPassword("123"); //设置密码

    bool ok = m_sqlConn.open(); //连接数据库

    if(!ok)
    {
        //错误信息
        QSqlError error;
        error=m_sqlConn.lastError();

        QString strError="";

        if(error.isValid())//发生错误时isValid()返回true
        {
            switch (error.type())
            {
            case QSqlError::NoError:
                strError="无错误";
                break;
            case QSqlError::ConnectionError://连接错语
                strError=error.text();
                break;
            case QSqlError::StatementError://语句错语
                strError=error.text();
                break;
            case QSqlError::TransactionError://事务错误
                qDebug()<<strError;
                break;
            default://未知错误
                qDebug()<<strError;
                break;
            }
        }

        //输出错误信息
        qDebug()<<strError;
        QMessageBox::critical(0, QObject::tr(" 连接数据库失败!"),strError);
        return false;
    }
    else
    {
        qDebug()<<"连接数据库成功!";
        //QMessageBox::information(0, QObject::tr("Tips"), QObject::tr("连接数据库成功!"));
    }


    //初始化表
    initTable();

    return true;
}

QSqlDatabase& CDataBase::getSqlConn()
{
    m_sqlConn = QSqlDatabase::database(m_strConnName);

    return m_sqlConn;
}


QSqlRecord CDataBase::ExecuteSql(const QString& strSql)
{
    if(!m_sqlConn.isOpen())
    {
        initDataBase();
    }

    //计时器
    QElapsedTimer tmpTime;
    //开始事务
    m_sqlConn.transaction();
    tmpTime.start();

    QSqlQuery query(m_sqlConn);
    if(!query.prepare(strSql))
    {
        qDebug()<<"prepare sql failed!";
    }

    if(!query.exec())
    {
        qDebug()<<"exec sql failed!"<<query.lastError();
    }

    //提交事务
    m_sqlConn.commit();

    //qDebug()<<QString("执行%1次操作耗时:").arg(count)<<(tmpTime.elapsed())/1000.0f<<"s"<<endl;
    qDebug()<<QString("执行耗时:")<<tmpTime.elapsed()<<"ms"<<endl;

    return query.record();
}

QSqlRecord CDataBase::ExecuteSql(const QString& strSql, QMap<QVariant,QVariant>& mapParameter)
{
    if(!m_sqlConn.isOpen())
    {
        initDataBase();
    }

    //计时器
    QElapsedTimer  tmpTime;
    //开始事务
    m_sqlConn.transaction();
    tmpTime.start();

    QSqlQuery query(m_sqlConn);

    if(!query.prepare(strSql))
    {
        qDebug()<<"prepare sql failed!"<<query.lastError();        
    }

    //打印sql语句,方便调试跟踪
    qDebug()<<"prepare strSql:"<<strSql;

    //提取参数中的字段名
    QList<QVariant> keyList=mapParameter.keys();

    //绑定字段值
    for(int i = 0; i < keyList.count(); i++)
    {
        //字段名
        QString name=keyList.at(i).toString();

        query.bindValue(name, mapParameter.value(name));
    }

    if(!query.exec())
    {
        qDebug()<<"exec sql failed!"<<query.lastError();
    }

    //提交事务
    m_sqlConn.commit();

    //qDebug()<<QString("执行%1次操作耗时:").arg(count)<<(tmpTime.elapsed())/1000.0f<<"s"<<endl;
    qDebug()<<QString("执行耗时:")<<tmpTime.elapsed()<<"ms"<<endl;

    return query.record();
}

void CDataBase::ExecuteSqls(const QString& strSql, QList<QMap<QVariant,QVariant>>* mapList)
{
    if(!m_sqlConn.isOpen())
    {
        initDataBase();
    }

    //计时器
    QElapsedTimer  tmpTime;
    //开始事务
    m_sqlConn.transaction();
    tmpTime.start();

    for(int i =0 ; i < mapList->count(); i++)
    {
        QMap<QVariant,QVariant> map = mapList->value(i);
        ExecuteSqlx(strSql, map);
    }

    //提交事务
    m_sqlConn.commit();

    //qDebug()<<QString("执行%1次操作耗时:").arg(count)<<(tmpTime.elapsed())/1000.0f<<"s"<<endl;
    qDebug()<<QString("执行数量:")<<mapList->count()<<QString("执行耗时:")<<tmpTime.elapsed()<<"ms"<<endl;
    return;
}

QSqlRecord CDataBase::ExecuteSqlx(const QString& strSql, QMap<QVariant,QVariant>& mapParameter)
{

    QSqlQuery query(m_sqlConn);

        if(!query.prepare(strSql))
        {
            qDebug()<<"prepare sql failed!"<<query.lastError();
        }

        //打印sql语句,方便调试跟踪
        qDebug()<<"prepare strSql:"<<strSql;

        //提取参数中的字段名
        QList<QVariant> keyList=mapParameter.keys();

        //绑定字段值
        for(int i = 0; i < keyList.count(); i++)
        {
            //字段名
            QString name=keyList.at(i).toString();

            query.bindValue(name, mapParameter.value(name));
        }

        if(!query.exec())
        {
            qDebug()<<"exec sql failed!"<<query.lastError();
        }
    return query.record();
}

//查询数据
QSqlQuery CDataBase::searchData(const QString& strSql,QMap<QVariant,QVariant>& mapParameter)
{
    if(!m_sqlConn.isOpen())
    {
        initDataBase();
    }

    //计时器
    QElapsedTimer  tmpTime;
    //开始事务
    m_sqlConn.transaction();
    tmpTime.start();

    QSqlQuery query(m_sqlConn);

    if(!query.prepare(strSql))
    {
        qDebug()<<"prepare sql failed!";
    }

    query.setForwardOnly(true);

    //提取参数中的字段名
    QList<QVariant> keyList=mapParameter.keys();

    //绑定字段值
    for(int i = 0; i < keyList.count(); i++)
    {
        //字段名
        QString name=keyList.at(i).toString();

        query.bindValue(name, mapParameter.value(name));
    }

    //执行sql语句
    if(!query.exec())
    {
        qDebug()<<"search exec sql failed!"<<query.lastError();
    }

    //提交事务
    m_sqlConn.commit();

    //qDebug()<<QString("执行%1次操作耗时:").arg(count)<<(tmpTime.elapsed())/1000.0f<<"s"<<endl;
    qDebug()<<QString("执行耗时:")<<tmpTime.elapsed()<<"ms"<<endl;

    return query;
}

bool CDataBase::initTable()
{

    //初始化用户信息表
    if(!initTabFunction())
    {
        qDebug()<<"测试信息表初始化失败!";
        return false;
    }

    //初始化传输方式信息表
    if(!initTabTransmission())
    {
        qDebug()<<"传输方式信息表初始化失败!";
        return false;
    }

    //初始化校准记录信息表
    if(!initTabCorrect())
    {
        qDebug()<<"校准记录信息表初始化失败!";
        return false;
    }

    //初始化瞬时数据信息表
    if(!initTabInstantTime())
    {
        qDebug()<<"瞬时数据信息表初始化失败!";
        return false;
    }

    //初始化录音信息表
    if(!initTabSoundRecord())
    {
        qDebug()<<"录音信息表初始化失败!";
        return false;
    }

    //测量数据信息表
    if(!initTabMeasureData())
    {
        qDebug()<<"测量数据信息表初始化失败!";
        return false;
    }

    //模板数据信息表
    if(!initTemplateData())
    {
        qDebug()<<"模板数据信息表初始化失败!";
        return false;
    }

    //功能显示列表
    if(!initFunctionChoiseData())
    {
        qDebug()<<"功能显示列表初始化失败!";
        return false;
    }

    return true;
}

bool CDataBase::initTabFunction()
{
    /**
     * @brief 创建测试信息表的sql
     * instrumentType; //仪器型号，唯一标识一条用户信息记录的字段
     * measureSta;  //统计测量功能
     * measureAll;  //总值测量功能
     * measure1OCt; //1/1OCT测量功能
     * measure3OCt;//1/3OCT测量功能
     * measureExport; //声暴露计测量功能
     * measureFFT; //快速傅里叶测量功能
     * measure24H; //24小时测量功能
     * instantSta; //瞬时统计功能
     * instantALL; //瞬时总值功能
     * instant1OCT; //瞬时1/1OCT
     * instant3OCT; //瞬时1/3OCT
     * realTimeSta; //实时统计功能
     * realTimeALL; //实时总值功能
     * realTime1OCT; //实时1/1OCT
     * realTime3OCT; //实时1/3OCT
     */
    const QString strSql=QLatin1String("create table dataInfo("
                                       "instrumentType varchar(10) PRIMARY KEY NOT NULL,"
                                       "measureSta bool Not Null,"
                                       "measureAll bool Not Null,"
                                       "measure1OCT bool Not Null,"
                                       "measure3OCT bool Not Null,"
                                       "measureExport bool Not Null,"
                                       "measureFFT bool Not Null,"
                                       "measure24H bool Not Null,"
                                       "measureINDOOR bool Not Null,"
                                       "instantSta bool Not Null,"
                                       "instantAll bool Not Null,"
                                       "instant1OCT bool Not Null,"
                                       "instant3OCT bool Not Null,"
                                       "instantINDOOR bool Not Null,"
                                       "realTimeSta bool Not Null,"
                                       "realTimeAll bool Not Null,"
                                       "realTime1OCT bool Not Null,"
                                       "realTime3OCT bool Not Null,"
                                       "sound bool Not Null)");

    QStringList listTable = m_sqlConn.tables();

    QSqlQuery query(m_sqlConn);

    //如果用户信息表不存在则创建
    if(!listTable.contains("dataInfo", Qt::CaseInsensitive))
    {
        //创建表
        if(!query.exec(strSql))
        {
            qDebug()<<"Failed to create table dataInfo:"<<query.lastError();
            return false;
        }

        qDebug()<<"success to create table dataInfo";

    }

    return true;

}

bool CDataBase::initTabTransmission()
{
    /**
     * @brief 创建通信信息表的sql
     * instrumentType; //仪器型号，唯一标识一条用户信息记录的字段
     * isSerial;  //是否设置未串口 true:串口  false:USB
     * comNum;  //串口号
     * BaudRate; //波特率
     */
    const QString strSql=QLatin1String("create table transmissionInfo("
                                       "instrumentType varchar(10) PRIMARY KEY NOT NULL,"
                                       "isSerial bool Not Null,"
                                       "comNum uint,"
                                       "BaudRate uint)");

    QStringList listTable = m_sqlConn.tables();

    QSqlQuery query(m_sqlConn);

    //如果用户信息表不存在则创建
    if(!listTable.contains("transmissionInfo", Qt::CaseInsensitive))
    {
        //创建表
        if(!query.exec(strSql))
        {
            qDebug()<<"Failed to create table transmissionInfo:"<<query.lastError();
            return false;
        }

        qDebug()<<"success to create table transmissionInfo";

    }

    return true;
}

bool CDataBase::initTabCorrect()
{
    /**
     * @brief 创建校准信息表的sql
     * id_c;//记录编号,唯一
     * instrumentType; //仪器型号
     * instrumentNum;  //仪器串号
     * calibrationTime; //校准时间
     * humidity;   //湿度
     * temperature; //温度
     * atmosphericPressure; //大气压
     * microphoneModel; //传声器型号
     * microphoneNum; //传声器串号
     * freeFieldCorrection; //自由场修正量
     *  microphoneSensitivityLevel; //传声器灵敏度级别
     * measureRange; //测量范围
     * soundCalibrationSoundLevel； //声校准器声压级
     *  calibrationSoundLevel;  //校准声压级
     * calibrationFrequent; //校准器频率
     */
    const QString strSql=QLatin1String("create table correctInfo("
                                       "id_c uint PRIMARY KEY NOT NULL,"
                                       "instrumentType varchar(10) NOT NULL,"
                                       "instrumentNum varchar(10) NOT NULL,"
                                       "calibrationTime DATETIME NOT NULL,"
                                       "humidity varchar(10),"
                                       "temperature varchar(10),"
                                       "atmosphericPressure varchar(10),"
                                       "microphoneModel varchar(10),"
                                       "microphoneNum varchar(10),"
                                       "freeFieldCorrection varchar(10),"
                                       "microphoneSensitivityLevel varchar(10),"
                                       "measureRange varchar(10),"
                                       "soundCalibrationSoundLevel varchar(10),"
                                       "calibrationSoundLevel varchar(10),"
                                       "calibrationFrequent varchar(10))");

    QStringList listTable = m_sqlConn.tables();

    QSqlQuery query(m_sqlConn);

    //如果用户信息表不存在则创建
    if(!listTable.contains("correctInfo", Qt::CaseInsensitive))
    {
        //创建表
        if(!query.exec(strSql))
        {
            qDebug()<<"Failed to create table correctInfo:"<<query.lastError();
            return false;
        }

        qDebug()<<"success to create table correctInfo";

    }

    return true;
}

bool CDataBase::initTabInstantTime()
{
    /**
     * @brief 瞬时数据信息表的sql
     * id_i;//记录编号,唯一（用于绑定测量数据与瞬时数据）
     * id_s;//录音记录编号
     * dataType; //数据类型
     * measureTime; //测量时间
     * measurePoint; //测点
     * filePath; //文件指引
     * dataText; //数据原文
     * dataJson;  //处理好后的json数据
     */
    const QString strSql=QLatin1String("create table instantTime("
                                       "id_i uint PRIMARY KEY NOT NULL,"
                                       "id_s varchar(10)," //为空表示没有录音文件
                                       "dataType varchar(10) NOT NULL,"
                                       "measureTime DATETIME,"
                                       "measurePoint varchar(10),"
                                       "filePath varchar(10) NOT NULL,"
                                       "dataText varchar(10),"
                                       "dataJson varchar(10))");

    QStringList listTable = m_sqlConn.tables();

    QSqlQuery query(m_sqlConn);

    //如果用户信息表不存在则创建
    if(!listTable.contains("instantTime", Qt::CaseInsensitive))
    {
        //创建表
        if(!query.exec(strSql))
        {
            qDebug()<<"Failed to create table instantTime:"<<query.lastError();
            return false;
        }

        qDebug()<<"success to create table instantTime";

    }

    return true;

}

bool CDataBase::initTabSoundRecord()
{
    /**
     * @brief 瞬时录音信息表的sql
     * id_s;//记录编号,唯一（用于绑定测量数据与瞬时数据）
     * soundPath; //录音数据记录标志 用于在6292处获取录音信息
     * soundInfo; //其他信息备注
     */
    const QString strSql=QLatin1String("create table soundRecord("
                                       "id_s uint PRIMARY KEY NOT NULL,"
                                       "soundPath varchar(10) NOT NULL,"
                                       "measureTime DATETIME,"
                                       "measurePoint varchar(10),"
                                       "soundInfo varchar(10))");

    QStringList listTable = m_sqlConn.tables();

    QSqlQuery query(m_sqlConn);

    //如果用户信息表不存在则创建
    if(!listTable.contains("soundRecord", Qt::CaseInsensitive))
    {
        //创建表
        if(!query.exec(strSql))
        {
            qDebug()<<"Failed to create table soundRecord:"<<query.lastError();
            return false;
        }

        qDebug()<<"success to create table soundRecord";

    }

    return true;
}

bool CDataBase::initTabMeasureData()
{
    /**
     * @brief 测量信息表的sql  （不同功能中的相同名称数据采用同一个节点记录，减少数量）
     * id_d;//记录编号,唯一（用于绑定测量数据与瞬时数据）
     * id_i; //当前记录匹配的瞬时数据编号
     * id_s; //当前记录匹配的录音数据编号
     * dataType; //数据的测量类型
     * instrumentType; //仪器型号
     * instrumentNum;  //仪器串号
     * measurePlace;  //测点名称
     * measureModel;  //测量方式
     * measureTime; //测量时间
     * Ts;   //Ts
     * Tm; //Tm
     * CalibratedSoundPressureLevel; //校准声压级
     * calibrationTime; //校准时间
     * sensitivity; //灵敏度
     * measureRange; //测量范围
     * timeWeight; //时间计权
     * frequentWeight; //频率计权
     *  LatitudeAndLongitude; //经纬度
     * Latitude; //经度
     * Longitude; //纬度
     * ***总值数据***
     * LAFmax;
     * LCFmax;
     * LZFmax;
     * LAFmin;
     * LCFmin;
     * LZFmin;
     * LASmax;
     * LCSmax;
     * LZSmax;
     * LASmin;
     * LCSmin;
     * LZSmin;
     * LAImax;
     * LCImax;
     * LZImax;
     * LAImin;
     * LCImin;
     * LZImin;
     * LAeq,T;
     * LCeq,T;
     * LZeq,T;
     * LApeek;
     * LCpeek;
     * LZpeek;
     * SEL_A;
     * SEL_C;
     * SEL_Z;
     * ***统计数据***
     * LeqT;
     * L5;
     * L10;
     * L50;
     * L90;
     * L95;
     * Lmax;
     * Lmin;
     * SEL;
     * SD;
     * ***声暴露数据***
     * TWA;
     * LEX8h;
     * LAVG;
     * DOSE;
     * E;
     * ***OCT数据***
     * ***FFT数据***
     * ***24H数据***
     * ***室内测***
     * roomType;
     * dataText; //下位机发送的数据原文
     * dataJson;  //处理好后的json数据（绘制图形或表格用）
     * jsonKey; // 处理好后的json数据的数据key（绘制图形用）
     */
    const QString strSql=QLatin1String("create table measureData("
                                       "id_d uint PRIMARY KEY NOT NULL,"
                                       "id_i uint," //为空表示没有瞬时数据
                                       "id_s varchar(10)," //为空表示没有录音文件
                                       "dataType varchar(10) NOT NULL,"
                                       "instrumentType varchar(10) NOT NULL,"
                                       "instrumentNum varchar(10) NOT NULL,"
                                       "measurePlace varchar(10) ,"
                                       "measureModel varchar(10),"
                                       "measureTime DATETIME NOT NULL,"
                                       "Ts varchar(10),"
                                       "Tm varchar(10),"
                                       "CalibratedSoundPressureLevel varchar(10),"
                                       "calibrationTime DATETIME,"
                                       "sensitivity varchar(10),"
                                       "measureRange varchar(10),"
                                       "timeWeight varchar(10),"
                                       "frequentWeight varchar(10),"
                                       "LatitudeAndLongitude varchar(10),"
                                       "Latitude varchar(10),"
                                       "Longitude varchar(10),"
                                       "LAFmax varchar(10),"
                                       "LCFmax varchar(10),"
                                       "LZFmax varchar(10),"
                                       "LAFmin varchar(10),"
                                       "LCFmin varchar(10),"
                                       "LZFmin varchar(10),"
                                       "LASmax varchar(10),"
                                       "LCSmax varchar(10),"
                                       "LZSmax varchar(10),"
                                       "LASmin varchar(10),"
                                       "LCSmin varchar(10),"
                                       "LZSmin varchar(10),"
                                       "LAImax varchar(10),"
                                       "LCImax varchar(10),"
                                       "LZImax varchar(10),"
                                       "LAImin varchar(10),"
                                       "LCImin varchar(10),"
                                       "LZImin varchar(10),"
                                       "LAeqT varchar(10),"
                                       "LCeqT varchar(10),"
                                       "LZeqT varchar(10),"
                                       "LApeek varchar(10),"
                                       "LCpeek varchar(10),"
                                       "LZpeek varchar(10),"
                                       "SELA varchar(10),"
                                       "SELC varchar(10),"
                                       "SELZ varchar(10),"
                                       "LeqT varchar(10),"
                                       "L5 varchar(10),"
                                       "L10 varchar(10),"
                                       "L50 varchar(10),"
                                       "L90 varchar(10),"
                                       "L95 varchar(10),"
                                       "Lmax varchar(10),"
                                       "Lmin varchar(10),"
                                       "SEL varchar(10),"
                                       "SD varchar(10),"
                                       "TWA varchar(10),"
                                       "LEX8h varchar(10),"
                                       "LAVG varchar(10),"
                                       "DOSE varchar(10),"
                                       "E varchar(10),"
                                       "roomType varchar(10),"
                                       "dataText varchar(10) NOT NULL,"
                                       "dataJson varchar(10) NOT NULL,"
                                       "jsonKey varchar(10) NOT NULL"
                                       ")");

    QStringList listTable = m_sqlConn.tables();

    QSqlQuery query(m_sqlConn);

    //如果用户信息表不存在则创建
    if(!listTable.contains("measureData", Qt::CaseInsensitive))
    {
        //创建表
        if(!query.exec(strSql))
        {
            qDebug()<<"Failed to create table measureData:"<<query.lastError();
            return false;
        }

        qDebug()<<"success to create table measureData";

    }

    return true;
}

bool CDataBase::initTemplateData()
{
    /**
     * @brief 模板信息表的sql  （目前只有AWA5912的数据类型）
     * templateName; //用户设置是模板名称
     * templateWriteTime; //模板创建的时间
     * deviceType; //模板对应的仪器名称
     * templateJson; //存储模板的json数据
     * timeListJson; //存储模板时间列表的json数据
     */
    const QString strSql=QLatin1String("create table templateData("
                                       "templateName varchar(10) PRIMARY KEY NOT NULL,"
                                       "templateWriteTime DATETIME NOT NULL,"
                                       "deviceType varchar(10) NOT NULL,"
                                       "templateJson varchar(10) NOT NULL,"
                                       "timeListJson varchar(10) NOT NULL)");

    QStringList listTable = m_sqlConn.tables();

    QSqlQuery query(m_sqlConn);

    //如果用户信息表不存在则创建
    if(!listTable.contains("templateData", Qt::CaseInsensitive))
    {
        //创建表
        if(!query.exec(strSql))
        {
            qDebug()<<"Failed to create table templateData:"<<query.lastError();
            return false;
        }

        qDebug()<<"success to create table templateData";

    }

    return true;

}

bool CDataBase::initFunctionChoiseData()
{
    /**
     * @brief 功能显示项目选择记录记录表的sql
     * deviceType; //仪器的型号
     * functionName；//功能名称
     * displayList; //显示的项目
     * hideList; //隐藏的项目
     */
    const QString strSql=QLatin1String("create table functionChoiseData("
                                       "functionType uint PRIMARY KEY NOT NULL,"
                                       "displayList varchar(10),"
                                       "hideList varchar(10))");

    QStringList listTable = m_sqlConn.tables();

    QSqlQuery query(m_sqlConn);

    //如果用户信息表不存在则创建
    if(!listTable.contains("functionChoiseData", Qt::CaseInsensitive))
    {
        //创建表
        if(!query.exec(strSql))
        {
            qDebug()<<"Failed to create table functionChoiseData:"<<query.lastError();
            return false;
        }

        qDebug()<<"success to create table functionChoiseData";

    }

    return true;
}
