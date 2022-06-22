#ifndef CDATABASE_H
#define CDATABASE_H

#include "LibControl_global.h"
#include<QtSql/QSqlDatabase>
#include<QtSql/QSqlQuery>
#include<QtSql/QSqlError>
#include<QtSql/QSqlRecord>
#include<QTime>
#include <QElapsedTimer>

/**********************************************
 *
 *  功 能: 数据库操作类
 *
 *  编 制: 陈宇葳
 *
 *  日 期: 2021/05/28
 *
 **********************************************/

class  LIBCONTROL_EXPORT CDataBase
{
public:

    /**
     * @brief 构造函数
     * @param strConnName 数据库连接名称
     */
    CDataBase(const QString strConnName);

    //析构函数
    ~CDataBase();


    /**
     * @brief 初始化数据库
     * @param 无
     * @return true 成功,false 失败
     */
    bool initDataBase();

    /**
     * @brief 执行SQL语句[不带参]
     * @param 无
     * @return true 成功,false 失败
     */
    QSqlRecord ExecuteSql(const QString& strSql);

    /**
     * @brief 执行SQL语句[带参]
     * @param strSql sql语句
     * @param mapParameter 要绑定的字段名和字段值,key value形式
     * @return QSqlRecord
     */
    QSqlRecord ExecuteSql(const QString& strSql, QMap<QVariant,QVariant>& mapParameter);


    /**
     * @brief 批量执行SQL语句[带参]
     * @param strSql sql语句
     * @param mapList 要绑定的字段名和字段值,key value形式
     * @return
     */
    void ExecuteSqls(const QString& strSql, QList<QMap<QVariant,QVariant>>* mapList);

    /**
     * @brief 批量执行SQL语句[带参]
     * @param strSql sql语句
     * @param mapList 要绑定的字段名和字段值,key value形式
     * @return QSqlRecord
     */
    QSqlRecord ExecuteSqlx(const QString& strSql, QMap<QVariant,QVariant>& mapList);

    /**
     * @brief 查询数据
     * @param strSql sql语句
     * @param mapParameter 要查询的字段名列表
     * @return QSqlQuery
     */
    QSqlQuery searchData(const QString& strSql,QMap<QVariant,QVariant>& mapParameter);

    /**
     * @brief 获取数据库连接
     * @return
     */
    QSqlDatabase& getSqlConn();

private:

    //数据库连接名称
    QString m_strConnName="";

    //数据库连接
    QSqlDatabase m_sqlConn;

    /**
     * @brief 初始化表
     * @return
     */
    bool initTable();

    /**
     * @brief 初始化仪器开启功能信息表
     * @return
     */
    bool initTabFunction();

    /**
     * @brief 初始化ANN参数信息表
     * @return
     */
    bool initANNInfo();

    /**
     * @brief 获取数据库路径
     * @return
     */
    QString getDbPath();

};

#endif // CDATABASE_H
