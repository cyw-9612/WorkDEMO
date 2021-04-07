

#include "protocolmanager.h"
#include <QString>
#include <QDebug>

ProtocolManager *ProtocolManager::instance_p = nullptr;

ProtocolManager::ProtocolManager()
{
    is_sending_ = false;
    wait_response_ = false;
    pos = 0;
    package_on_ = false;
    comm_p = nullptr;

    //be note that caller-thread and creater-thread must be one thread for object of this class


    //线程转换
    //qDebug() << "ProtocolManager::ProtocolManager connect ";
    //每次线程发送完成则发送callSend信号，连接doSend槽函数开启下一个线程
    connect(this, &ProtocolManager::callSend, this, &ProtocolManager::doSend);
}

ProtocolManager::~ProtocolManager()
{
    //qDebug() << "ProtocolManager::~ProtocolManager dis connect ";
    disconnect(this, &ProtocolManager::callSend, this, &ProtocolManager::doSend);
}

//获取一个实例  没有则new一个协议管理类并返回
ProtocolManager* ProtocolManager::GetInstance() {
    if (instance_p == nullptr) {
        instance_p = new ProtocolManager();
    }
    return instance_p;
}



/******************************* connection *****************************/

//发送数据
qint64 ProtocolManager::Send(const QByteArray &data) {
   // qDebug() << "**************    ProtocolManager::Send";
    return comm_p->Send(data);
}

void ProtocolManager::onRecv(QByteArray &data, void *extra_data) {
    Q_UNUSED(extra_data)

//  qDebug() << "ProtocolManager::onRecv = " << data;

    //安位将数据发送至Process函数进行拆包判断处理
    for(int a = 0; a < data.length(); a ++) {
        Process(static_cast<quint8>(data.at(a)));
    }
}

/******************************* thread *****************************/

void ProtocolManager::StartThread() {
    is_sending_ = true;
    start();
}

void ProtocolManager::StopThread() {
    is_sending_ = false;
}

bool ProtocolManager::IsThreadWorking() {
    return is_sending_;
}

//线程开始运行
void ProtocolManager::run() {
    while(is_sending_) {
        //获取工作列表中的第一个当前工作
        current_work_p = wm_.GetCurrentWork();
        if (current_work_p != nullptr) {
            //工作存在，将等待回复标志位致1
            wait_response_ = true;
            //qDebug() << "emit callSend();";
            //发送数据信号，关联doSend()
            emit callSend();

            //记录工作开始时间，用于回复超时判断
            start_work_secs_ = QDateTime::currentSecsSinceEpoch();

        } else {
            //停止所有工作
            StopThread();
            //等待回复标志位致0
            wait_response_ = false;
        }

        //等待回复标志位为1时进行超时判断
        while (wait_response_) {
             msleep(10);
             //定时 当发送和响应时间之差大于设定值 则重新发送
            qint64 current_secs = QDateTime::currentSecsSinceEpoch();
            if(current_secs - start_work_secs_ > TIME_OUT_SECONDS) {
                ReSendPkg();
            }

        }
    }
}

//发送数据
void ProtocolManager::doSend() {
    //qDebug() << "ProtocolManager::doSend";
    Send(current_work_p->data);
}

/********************** work process **************************/

//添加一个工作，使用工作管理类在工作列表后部添加一个工作类
void ProtocolManager::AddWork(QByteArray &pkg) {
   // qDebug() << "ProtocolManager::AddWork";
    wm_.AddWork(pkg);
     if (IsThreadWorking() == false) {
         StartThread();
     }
}

//获取当前工作列表中工作计数
int ProtocolManager::GetCachedWorkCount() {
    return wm_.GetWorkCount();
}

//清除工作列表
void ProtocolManager::CleanWorkList() {
    wm_.CleanWorkList();
}

//接收数据拆包，找到包头有包尾，截取中间数据
void ProtocolManager::Process(quint8 b) {
    cache[pos++] = b;
    if (package_on_ == true)
    {
        if (pos > CACHE_LENGTH) {
            package_on_ = false;
            pos = 0;
            return;
        }

//        cache[pos++] = b;

        //检查包尾，此处设定包尾为4位,找到包尾表示包pkg结束
        if (pos > 4 &&
            cache[pos - 4] == tail[0] &&    //a pkg is finished
            cache[pos - 3] == tail[1] &&
            cache[pos - 2] == tail[2] &&
            cache[pos - 1] == tail[3] &&
            cache[sizeof (head)] + sizeof (head) + sizeof (tail)  == pos //check pkg len 检查包长  cache[sizeof (head)]处就是data中的len
                ) {
            package_on_ = false;

            //根据定义包头加包长为4位
            int skip_head_and_pkg_len = 4;
            //包尾长度位4
            int skip_tail = 4;
            quint8* pkg = &cache[skip_head_and_pkg_len]; //remove head and pkg-len 移除包头和数据长度位
            int pkg_len = pos - skip_head_and_pkg_len - skip_tail;//skip index of tail byte 定义数据长等于总长减去头长，数据位和尾长

            //检测CRC是否正确，正确为true
            if (Crc::CheckCRC(pkg, pkg_len) == true) {
                //将拆包得到的数据发出
                ProcessMetaPkg(pkg, pkg_len-2);//-2 : remove crc byte 移除末尾两位CRC的长度
                pos = 0;
            } else {
                //CRC校验错误，清除工作列表
                CleanWorkList();
                emit onProtocolError(CrcCheckError, "crc check error");
                pos = 0;
            }
        }
    } else if (package_on_ == false &&          //判断包头
               pos > 2  &&
               cache[pos - 3] == head[0] &&
               cache[pos - 2] == head[1] &&
               cache[pos - 1] == head[2]) { //找到包头，开始一个包的读取（标志位）
        package_on_ = true;
//        pos = 0;
//        cache[pos++] = b;
    }
}

//由于等待回复定时超时，表示发送失败，重新发送当前数据
void ProtocolManager::ReSendPkg() {
    //qDebug() << "ProtocolManager::ReSendPkg";
     ProcessMetaPkg(nullptr, 0);
}

//线程和接收数据反馈会唤醒此函数，因此需要互斥锁保证安全
void ProtocolManager::ProcessMetaPkg(quint8 *pkg, int len) {
    //互斥锁
    mutex_.lock();

    //根据指令类型码判断，如果是下位机主动反馈指令，在这里拦截判断
    if(pkg != nullptr && Command::IsCommandIsReportingType(pkg)) {
        emit onDispatchPackage(pkg, len);
        mutex_.unlock();
        return;
    }

    if (wait_response_ == false) {
        mutex_.unlock();
        return;
    }

//    qDebug() << "recv a package";

    //检验下位机对上位机命令的反馈指令是否正确
    if (pkg != nullptr && current_work_p->CheckResponse(pkg) == true) {
//        qDebug() << "finished a package";
        //完成了一个包的发送（得到了正确的反馈）
        wm_.FinishAWork();
        emit onDispatchPackage(pkg, len);//没有包头 包长 CRC 包尾的纯指令数据
    } else {
        ProcessTimeOutError();
//       qDebug() << "recv error";
    }
    wait_response_ = false;

    mutex_.unlock();
}

//进程超时错误处理（增加错误次数，没有超过3次则重发）
void ProtocolManager::ProcessTimeOutError() {
    if (current_work_p != nullptr)
    {
        current_work_p->failed_times++;
        //重发次数超过三次了，发送协议错误信号
        if (current_work_p->failed_times >= 3)
        {
            CleanWorkList();
            StopThread();
            wait_response_ = false;
           // Disconnect();
            emit onProtocolError(ReTry3TimesError, "retry 3 times error");
        }
    }
}

/*
//对于接收下位反馈给上位机数据的指令，在此函数中获取数据
void ProtocolManager::DispatchPackage(quint8 *pkg, int len) {
    if (len < 2) return;

    if(Command::IsCommandReportValue(pkg))  {
        Send(*Command::ResponseForReportValue()); //give response immediately
        emit onReportValue(Command::GetReportValue(pkg, len));
        //wm_.InsertWorkAtHead(*Command::ResponseForReportValue());
    }

    //根据不同指令，调用对应的处理函数，并调用其回调函数，如果有必要的时候
    if(Command::IsCommandGetMachinetemperature(pkg)) {
        emit onGetMachinetemperature(Command::GetValueOfGetMachinetemperature(pkg, len));
    }
}*/

/********************** comm **************************/
//为了避免以下报错，因此为了实现 通信设备实例 和此 协议处理模块能 相互解耦。因此这里
//设计了这个 设备初始化回调函数
//QObject: Cannot create children for a parent that is in a different thread.

//设置协议和指令的连接
void ProtocolManager::SetProtocolComm(CommInterface* comm) {
    comm_p = comm;
    disconnect(comm_p, &CommInterface::onRecv, this, &ProtocolManager::onRecv);
    connect(comm_p, &CommInterface::onRecv, this, &ProtocolManager::onRecv);
}

//指令初始化信号
void ProtocolManager::CommInit() {
    emit onCommInitCallback();
}

bool ProtocolManager::CommOpen() {
    return comm_p->Open();
}

void ProtocolManager::CommClose() {
    if(comm_p == nullptr) return;
    comm_p->Close();
}

bool ProtocolManager::CommIsOpen() {
    return comm_p->IsOpen();
}

//bool ProtocolManager::GetIsSending()
//{
//    return is_sending_;
//}
