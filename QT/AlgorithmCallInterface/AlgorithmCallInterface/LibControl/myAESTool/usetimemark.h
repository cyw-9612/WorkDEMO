#ifndef USETIMEMARK_H
#define USETIMEMARK_H

#include "LibControl_global.h"

#include <QSettings>
#include <QDateTime>

/***********************************
 *
 * 功 能: 软件使用时间记录工具类
 *
 * 编 制: 陈宇葳
 *
 * 更新日期: 2021/03/12
 *
 **********************************/

class LIBCONTROL_EXPORT useTimeMark
{
public:
    useTimeMark();
    ~useTimeMark();

    /**
    * @brief isHaveTime 软件是否还有使用时间
    * @return 无
    */
    bool isHaveTime();

    /**
    * @brief writeTime 写入使用时间
    * @param day 可使用的天数
    * @return 无
    */
    void writeTime(int day);

    /**
    * @brief switchTime 修改使用时间
    * @param limieDay:序列号的可使用时长
    * @param sec:从注册开始到现在的时间
    * @return 无
    */
    void switchTime(int limieDay,int sec);

private:
    /**
    * @brief readAuthHistoryFile 读取历史数据
    * @return 无
    */
    void readAuthHistoryFile();

    /**
    * @brief saveAuthHistory 保存数据
    * @return 无
    */
    void saveAuthHistory();

    /**
    * @brief SetConfigData 设置配置文件数据
    *  @param qstrnodename 数据类型
    *  @param qstrkeyname 数据名称
    *  @param qvarvalue 数据
    * @return 无
    */
    void SetConfigData(QString qstrnodename, QString qstrkeyname, QVariant qvarvalue);

    /**
    * @brief GetConfigData 设置文件数据
    *  @param qstrnodename 数据类型
    *  @param qstrkeyname 数据名称
    * @return 对应数据
    */
    QVariant GetConfigData(QString qstrnodename, QString qstrkeyname);

private:
    QSettings *m_psetting = nullptr;

    //软件开启时间
    qint64 startTime;

    //软件关闭时间
    qint64 endTime;

    //剩余的时间
    qint64 lastTime;
};

#endif // USETIMEMARK_H
