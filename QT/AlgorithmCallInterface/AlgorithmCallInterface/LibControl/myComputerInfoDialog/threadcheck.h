#ifndef THREADCHECK_H
#define THREADCHECK_H

#include <QVector>
#include <QString>

/***********************************
 *
 * 功 能: 电脑现有进程监控类
 *
 * 编 制: 陈宇葳
 *
 * 日 期: 2022/06/015
 *
 **********************************/

struct ProcessInfo{

    QString processID;              //进程ID
    QString threadNum;              //该进程开启的线程数
    QString parentProcessID;        //父进程ID
    QString processPri;             //线程优先权
    QString processHandle;          //进程句柄
    QString processName;            //名称
};

class threadCheck
{
public:
    threadCheck();
    void getSnapshot();
    const QVector<ProcessInfo> &getVec();
    void clearVec();

protected:
    void insertInfo(const QString &pID, const QString &thrNum,
                    const QString &parePID, const QString &processPr,
                    const QString &handle, const QString &processName);

private:
    QVector<ProcessInfo> m_vec;
};

#endif // THREADCHECK_H
