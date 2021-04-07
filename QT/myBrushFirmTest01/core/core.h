#ifndef CORE_H
#define CORE_H

#include <QObject>

#include <QThread>
#include <QSerialPortInfo>
#include "../uart/myserialport.h"
#include "../protocol/protocolmanager.h"
#include "hexfilereader.h"

class Core : public QThread
{
    Q_OBJECT
public:
    Core();
    ~Core() override;

    //设置文件路径
    void SetHexFileAbsPath(QString file_path);
    //得到文件绝对路径
    QString GetHexFileAbsPath();

    //设置波特率
    void SetBaudRate(qint32 baudRate) {
        baudRate_ = baudRate;
    }

    //设置串口名称
    void SetPortName(QString port_name);

    //开启工作线程
    void StartWorkingThread();

signals:
    //信号：下载进程（是否完成，是否成功，进度，文件）
    void onDownloadProgress(bool is_finish, bool is_successed, int percent, QString note);

private slots:
    void OnProtocolError(int error_type, QString error_string);
    void OnDispatchPackage(quint8 *pkg, int len);
    void OnCommInitCallback();

private:
    void run() override;


private:
    //定义十六进制文件的绝对路径
    QString hex_file_abs_path_;
   // QSerialPortInfo serial_device_info_;

    ProtocolManager* protocol_manager_p;
    HexFileReader hex_reader_;

    qint32 baudRate_;

    QString port_name_;


};

#endif // CORE_H
