#ifndef FORMWAVFILEPROCESS_H
#define FORMWAVFILEPROCESS_H

#include <QObject>
#include <QMutex>
#include <QMenu>
#include <QDebug>
#include <QSystemTrayIcon>
#include <QFile>
#include <QFileInfo>
#include <QFutureWatcher>
#include "libWavFileProcess_global.h"
#include "wavefile.h"

/***********************************
 *
 * 功 能: 录音音频文件处理库
 *
 * 编 制: 陈宇葳
 *
 * 日 期: 2021/07/08
 *
 **********************************/

class LIBWAVFILEPROCESS_EXPORT formWavFileProcess  : public QObject
{
    Q_OBJECT

public:
    explicit formWavFileProcess(QObject *parent = nullptr);
    ~formWavFileProcess();

    /**
     * @brief 生成新的录音文件
     * @param fileName:选择的文件路径
     * @param Magnification:放大倍数
     * @return 生成的新文件的路径
     */
    std::string ProcessingAudio(std::string fileName, double Magnification);


private slots:
    //完成读取或生成文件的分析
    void slotReadFinish();

private:
    //根据设置生成新的文件
    void writeNewWavFile();

    void Sleep(int msec);

private:
    //音频文件读取类
    WaveFile m_Wavefile;

    QString m_Filename; //文件选择路径
    QString m_saveFilename; //文件存储路径
    double m_Magnification; //放大倍数

    //保存的文件对象
    QFile * m_file;

    //QFuture本身不带信号槽，可使用QFutureWatcher进行监控
    QFutureWatcher<void> *myWatcher;

    //标志线程执行的是否位读取文件 true为读取文件 false为生成文件
    bool isReadFile = true;

    bool isCreatFile = false;

    bool isErrorForHandle = false;
};

#endif // FORMWAVFILEPROCESS_H
