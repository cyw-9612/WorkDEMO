
#ifndef WORK_H
#define WORK_H

#include <QByteArray>

class Work
{
public:
    Work();

    int failed_times;//用于记录此条指令发送失败的重试次数
    QByteArray data;//用于记录指令封包的数据

    bool CheckResponse(quint8 * return_pkg);//用于检测本指令对应的“反馈指令”是否和本指令匹配

};

#endif // WORK_H
