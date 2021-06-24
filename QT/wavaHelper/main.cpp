#include "wavahelper.h"

#include <QApplication>
#include <QSystemSemaphore>
#include <QSharedMemory>
#include <QMessageBox>
#include <QtDebug>

bool CheckProgramOnly()
{
    // 创建信号量
    static QSystemSemaphore semaphore("ProgrmOnlySemaphore001", 1, QSystemSemaphore::Open);
    // 启用信号量，禁止其他实例通过共享内存一起工作
    semaphore.acquire();

    // 创建一个共享内存  “SingleAppTest2”表示一段内存的标识key 可作为唯一程序的标识
    static QSharedMemory sharedMemory("ProgrmOnly001");
    // 测试是否已经运行
    bool isRunning = false;
    // 试图将共享内存的副本附加到现有的段中。
    if (sharedMemory.attach())
    {
        // 如果成功，则确定已经存在运行实例
        isRunning = true;
    }
    else
    {
        // 否则申请一字节内存
        sharedMemory.create(1);
        // 确定不存在运行实例
        isRunning = false;
    }
    semaphore.release();

    return !isRunning;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    if(CheckProgramOnly() == false)
    {
        QMessageBox::information(NULL, "提示", "程序已运行");
        return 0;
    }

    wavaHelper::getInstance()->show();// 显示主界面

    return a.exec();
}
