
#ifndef WORKMANAGER_H
#define WORKMANAGER_H

#include <QList>
#include "work.h"


class WorkManager
{
public:
    WorkManager();
    ~WorkManager();

    void AddWork(QByteArray &pkg);  //给列表增加一条待发送指令
    void InsertWorkAtHead(QByteArray &pkg); //在开头添加一条指令
    void CleanWorkList();   //清空指令列表
    void FinishAWork(); //删除列表首位的指令，表示这条指令发送完成
    Work* GetCurrentWork(); //获取列表首位的那一条指令
    bool IsHasWork();   //查询列表中是否有指令待发
    int GetWorkCount(); //获取指令数量


private:
    QList<Work*> work_list_;    //用于记录待发送指令的列表,一个work链表

};

#endif // WORKMANAGER_H
