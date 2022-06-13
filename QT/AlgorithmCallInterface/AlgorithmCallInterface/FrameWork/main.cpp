#include <QApplication>
#include <QDateTime>
#include <QDir>
#include <QSystemSemaphore>
#include <QSharedMemory>
#include <QFontDatabase>
#include "winmain.h"
#include "rtmessagebox.h"

#include <QApplication>

void outputMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    static QMutex mutex;
    mutex.lock();
    QString text;
    switch(type)
    {
    case QtDebugMsg:
        text = QString("Debug:");
        break;
    case QtWarningMsg:
        text = QString("Warning:");
        break;
    case QtCriticalMsg:
        text = QString("Critical:");
        break;
    case QtFatalMsg:
        text = QString("Fatal:");
    }

    QString context_info = QString("File:(%1) Line:(%2)").arg(QString(context.file)).arg(context.line);
    QString current_date_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss ddd");
    QString current_date = QString("(%1)").arg(current_date_time);
    QString message = QString("%1  %2  %3  %4").arg(text).arg(context_info).arg(msg).arg(current_date);

    //获取程序当前运行目录
    QString fileDir = QCoreApplication::applicationDirPath();
    fileDir +="//Log";

    //如果日志目录不存在,立刻返回
    QDir* dirx = new QDir();
    if(!dirx->exists(fileDir)){
         dirx->mkpath(fileDir);
    }

    //每天产生一个.txt日志文件
    QString currentDay = QDateTime::currentDateTime().toString("yyyy-MM-dd");
    //文件名
    QString fileName=QString("%1.txt").arg(currentDay);

    //路径名+文件名
    QString strPath=QString("%1//%2").arg(fileDir).arg(fileName);
    //QFile file("log.txt");
    QFile file(strPath);

    //QIODevice::ReadWrite 可读写,QIODevice::Text 换行符生效,QIODevice::Append 追加写入,QFile::Truncate 表示将原文件清空
    file.open(QIODevice::Text | QIODevice::WriteOnly | QIODevice::Append);

    QTextStream text_stream(&file);

    //输出到文件流,自动换行空行
    text_stream << message <<"\r\n\r\n";
    //text_stream << texta << endl;

    file.flush();
    file.close();

//    QFile file("log.txt");
//    file.open(QIODevice::WriteOnly | QIODevice::Append);
//    QTextStream text_stream(&file);
//    text_stream << message << "\r\n\r\n";
//    file.flush();
//    file.close();

    mutex.unlock();
}

bool CheckProgramOnly()
{
    // 创建信号量
    static QSystemSemaphore semaphore("ProgrmOnlySemaphore", 1, QSystemSemaphore::Open);
    // 启用信号量，禁止其他实例通过共享内存一起工作
    semaphore.acquire();

    // 创建一个共享内存  “SingleAppTest2”表示一段内存的标识key 可作为唯一程序的标识
    static QSharedMemory sharedMemory("ProgrmOnly");
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
        RTMessageBox box(TOTAST_WARNING, "程序已运行" );
        box.exec();
        return 0;
    }

#ifndef QT_DEBUG
        //注册MessageHandler 用于日志记录
        qInstallMessageHandler(outputMessage);
#endif

    //配置自定义字体文件
    int nIndex = QFontDatabase::addApplicationFont(":/font/Font/SourceHanSansK-Bold.TTF");
    if (nIndex != -1)
    {
      QStringList strList(QFontDatabase::applicationFontFamilies(nIndex));
      if (strList.count() > 0)
      {
         QFont fontThis(strList.at(0));
         fontThis.setPointSize(9);
         a.setFont(fontThis);
      }
    }

    winMain w;
    w.show();

    return a.exec();
}
