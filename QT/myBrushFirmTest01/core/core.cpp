#include "core.h"

#include <QDebug>
#include <QApplication>


Core::Core()
{
    //初始化波特率为115200
    baudRate_ = QSerialPort::BaudRate::Baud115200;

    protocol_manager_p =  ProtocolManager::GetInstance();
    connect(protocol_manager_p, &ProtocolManager::onCommInitCallback, this, &Core::OnCommInitCallback);
    connect(protocol_manager_p, &ProtocolManager::onProtocolError, this, &Core::OnProtocolError);
    connect(protocol_manager_p, &ProtocolManager::onDispatchPackage, this , &Core::OnDispatchPackage);
}

Core::~Core()
{

}

//设置十六进制文件的绝对路径
void Core::SetHexFileAbsPath(QString file_path) {
    hex_file_abs_path_ = file_path;
}

//得到十六进制文件的绝对路径
QString Core::GetHexFileAbsPath() {
    return hex_file_abs_path_;
}

//设置串口的名字
void Core::SetPortName(QString port_name) {
    port_name_ = port_name;
}
//为了辅助移植本软件对应的bootloader到其他芯片上，这里保留了qdebug调试语句（仅做屏蔽处理）
//开始工作线程
void Core::StartWorkingThread() {
    //初始化进程管理类
    protocol_manager_p->CommInit();

    if(protocol_manager_p->CommOpen() != true) {
        qDebug() << "serial port open failed!";
        emit onDownloadProgress(true, false, 0, QApplication::translate("Core", "serial port open failed!"));
        return;
    }
    //开始线程，运行run函数
    start();
}

void Core::run() {

    //开启十六进制文件
    bool is_success = hex_reader_.Open(hex_file_abs_path_);
    if(!is_success) {
        emit onDownloadProgress(true, false, 0, QApplication::translate("Core", "can't open bin file."));
        return;
    }

    //记录文件信息
    quint32 piece_count =  hex_reader_.GetTotalPieceCount();
    quint16 piece_data_len = hex_reader_.GetPieceDataSize();
    quint32 check_sum = 0;

    qDebug() << "piece_count = " << piece_count;
    qDebug() << "piece_data_len = "  << piece_data_len;


    //开始传输下载
    protocol_manager_p->AddWork(*Command::HexDownloadStart());

    for(quint16 a = 0; a < piece_count; a ++) {
        char* piece_data = hex_reader_.GetNextPieceData();
        //a为当前片数减一，a从0开始
//       qDebug() << "---------------------data index = = " << a+1;
        bool is_last_piece = (a == piece_count-1);
        //根据参数填写工作参数
        protocol_manager_p->AddWork(*Command::HexDownloadData(a,is_last_piece, piece_data, piece_data_len));

        //计算完成百分比=（当前传输片数*100）/总数 %
        double percent_in_double = static_cast<double>(a*100) / static_cast<double>(piece_count );  //完成百分比
        int percent = qRound(percent_in_double);    //四舍五入
        //发送进度条更新信号
        emit onDownloadProgress(false, false, percent, QApplication::translate("Core", "downloading."));

        //防止一次性太多加入命令到发送队列，最多添加20片待发送
        while(protocol_manager_p->GetCachedWorkCount() > 20) {
//            qDebug() << "###################### in wait";
            QThread::msleep(10);
        }

        //注意，由于芯片在收到8组数据即，8个 128数据包之后，会整体把缓存数据写入到flash中，
        //因此可以观察到的通信情况是，通信指示灯，快速散8次，然后，停顿一下（写入flash），然后有快速散8次
        //循环次现在 直到下载完成

        //计算校验和，用于最后的检查步骤
        for (int b = 0; b < piece_data_len; b ++) {
            check_sum += ByteUtil::Char2Byte(piece_data[b]);
        }
//        qDebug() << "check_sum = " << check_sum;
    }

    /*
    QByteArray  *haha = Command::HexDownloadDataCheck(check_sum, piece_count);
    for (int j = 0; j < haha->length(); j ++) {
        printf("%02x ", (haha->data()[j])&0xff);
    }*/

    //qDebug() << "send check sum command";
    //数据传输完成，进度条致99，发送和校验和数据片计数
    emit onDownloadProgress(false, false, 99, QApplication::translate("Core", "download finished, start do check sum."));
    protocol_manager_p->AddWork(*Command::HexDownloadDataCheck(check_sum, piece_count));//option command


}

//进程错误信息
void Core::OnProtocolError(int error_type, QString error_string) {
    qDebug() << "error_type = " << error_type << " resson: " << error_string;

}

//完成一个包的发送，根据包的内容判断当前状态
void Core::OnDispatchPackage(quint8 *pkg, int len) {
  //  qDebug() << "recv " << pkg[0]  << pkg[1] << " len = " << len;

    //反馈为 01 03 这是发送完成指令
    if(len == 2) {
        if(pkg[0] == 0x01 && pkg[1] == 0x03) {
            // qDebug() << "------------------------app running";
             emit onDownloadProgress(true, true, 100, QApplication::translate("Core", "app running."));
        }
    }

    //反馈为 01 05 01 这是传输完成后校验和和片数核对成功指令
    if(len == 3) {
        if(pkg[0] == 0x01 && pkg[1] == 0x05) { //检验和检查成功
            if(pkg[2] == 0x01) { //success
               // qDebug() << "------------------------checksum success";
                emit onDownloadProgress(false, false, 99, QApplication::translate("Core", "checksum success."));

                //校验完成，文件传输无问题，指示传输完成
                protocol_manager_p->AddWork(*Command::HexDownloadEnd());

            } else { //和校验或片数校验失败，文件传输错误
                 //qDebug() << "-------------------------checksum failed";
                 emit onDownloadProgress(true, false, 100, QApplication::translate("Core", "checksum failed."));
            }
        }
    }

}

//设置串口基本参数
void Core::OnCommInitCallback() {
    MySerialPort* device_p;
    device_p = new MySerialPort();
    device_p->SetBaudRate(baudRate_);
    device_p->SetDataBits(QSerialPort::DataBits::Data8);
    device_p->SetParity(QSerialPort::Parity::NoParity);
    device_p->SetStopBits(QSerialPort::StopBits::OneStop);
    device_p->SetPortName(port_name_);

    protocol_manager_p->SetProtocolComm(device_p);
}



