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
