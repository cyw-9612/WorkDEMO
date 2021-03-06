#include "winmain.h"
#include "ui_winmain.h"
#include <QMessageBox>
#include <QGridLayout>
#include <QDir>
#include <QProcessEnvironment>
#include <QSettings>
#include <QCryptographicHash>
#include <QHostInfo>
#include <QNetworkInterface>
#include <QUdpSocket>
#include <QMutex>
#include <QMouseEvent>
#include <QFuture>
#include <QMetaType>
#include <QtConcurrent>
#include <QtXlsx/QtXlsx>
#include <QFileDialog>
#include <QProcess>
#include <windows.h>
#include "libcontrol.h"
#include "myAESTool/qaesencryption.h"
#include "mytool.h"

#define GB (1024*1024*1024)

//初始化类的静态成员变量
QMutex winMain::m_Mutex;
QSharedPointer<winMain> winMain::m_pInstance=nullptr;

winMain::winMain(QWidget *parent)
    : BaseView(parent)
    , ui(new Ui::winMain)
{
    ui->setupUi(this);

    /*
     * 设置主对话框样式
     */
    setWindowTitle("脚本管理平台 v1.0.0.0");
    setWindowFlags( Qt::Window|Qt::MSWindowsFixedSizeDialogHint | Qt::FramelessWindowHint |Qt::WindowSystemMenuHint|Qt::WindowMinimizeButtonHint|Qt::WindowMaximizeButtonHint);

    InitUI();

    InitSignal();

    //获取软件配置信息
    QSettings *configIniRead = new QSettings("config.ini", QSettings::IniFormat);
    //将读取到的ini文件保存在QString中，先取值，然后通过toString()函数转换成QString类型
    QString ip = configIniRead->value("/ip/first").toString();
    int port = configIniRead->value("/port/open").toInt();

    //读入入完成后删除指针
    delete configIniRead;

    //建立数据库连接
    m_pDB = new CDataBase("sqlConn_winMain");

    updataANNInfo();

    qRegisterMetaType<QMap<QVariant,QVariant>>("QMap<QVariant,QVariant>");//注册diskInformation类型

    deleteOver7daysLog();
}

winMain::~winMain()
{
    delete ui;

    //删除数据库指针
    delete m_pDB;

    if(m_menu != nullptr)
    {
        delete  m_menu;
        m_menu = nullptr;
    }

    if(m_messageBox != nullptr)
    {
        delete  m_messageBox;
        m_messageBox = nullptr;
    }

    if(m_threadInfo != nullptr)
    {
        delete  m_threadInfo;
        m_threadInfo = nullptr;
    }

    // 停止电脑信息获取线程。
    if (Thread_PCInfo)
    {
        Thread_PCInfo->quit();
        Thread_PCInfo->wait();
        Thread_PCInfo = NULL;
    }

    qDebug() << "~winMain";
}

void winMain::InitUI()
{
    //设置需要遮罩的父窗体
    RtMaskLayer::Instance()->SetMainWidget(this);
//    RtMaskLayer::Instance()->setGeometry(0, 0, 1024, 768);
    //遮蔽罩设置大小(尽量大，避免全屏情况下遮蔽不全)
    RtMaskLayer::Instance()->setGeometry(0, 0, 4096, 2160);
    RtMaskLayer::Instance()->hide();

    //设置哪些弹窗窗体需要被遮罩
    QStringList dialogNames;
    dialogNames << "RTDeviceStatusClassWindow"
                           << "myMessageDialog"
                           << "currentThreadDialog"
                           << "miniMessageDialog"
                           << "myProgressBox";

    RtMaskLayer::Instance()->SetDialogNames(dialogNames);

    m_menu = new QMenu("其他"); // 创建菜单项

    m_optionOne = new QAction("查看电脑进程");
    m_optionTwo = new QAction("功能二");
    m_optionThree = new QAction("功能三");

    m_menu->addAction(m_optionOne);
    m_menu->addAction(m_optionTwo);
    m_menu->addAction(m_optionThree);

    QString munuStyle = "QMenu {\
            background-color : rgb(55, 63, 81);\
            padding:5px;\
            border-radius:15px;}\
            QMenu::item {\
            font-size:10pt;\
            color: rgb(255,255,255);\
            background-color:rgb(55, 63, 81);\
            padding: 8px 25px 6px 10px;\
            margin: 4px 1px;}\
            QMenu::item:selected {\
            background-color : rgb(1, 160, 165);}";
    m_menu->setStyleSheet(munuStyle);

    ui->btn_setting->setMenu(m_menu);

    m_messageBox = new miniMessageDialog(this);

    m_functionWidget = new myFunctionListwidget(ui->bottomPanel);
    ui->bottomLayout->addWidget(m_functionWidget);

    m_PCInfo = new myComputerInfoDialog(ui->bottomPanel);
    ui->bottomLayout->addWidget(m_PCInfo);

    ui->bottomLayout->setStretch(0,4);
    ui->bottomLayout->setStretch(1,3);

    m_threadInfo = new currentThreadDialog(this);

    Thread_PCInfo = new QThread;
    m_PCInfo->moveToThread(Thread_PCInfo);
    m_threadInfo->moveToThread(Thread_PCInfo);
    Thread_PCInfo->start();
}

// 信号初始化
void winMain::InitSignal()
{
    connect(ui->btnMeasure, SIGNAL(clicked()), this, SLOT(SlotMeasure()));// 显示测量数据
    connect(ui->btnMin, SIGNAL(clicked()), this, SLOT(SlotMin()) );//最小化按钮
    connect(ui->btnMax, SIGNAL(clicked()), this, SLOT(SlotMax()) );//最大化按钮
    connect(ui->btnClose, SIGNAL(clicked()), this, SLOT(SlotClose()));//关闭按钮
    connect(ui->btn_setting, SIGNAL(clicked()), this, SLOT(SlotSetting()));//设置按钮
    connect(ui->btnHelp, SIGNAL(clicked()), this, SLOT(SlotHelp()));//帮助按钮

    connect(m_optionOne, &QAction::triggered, this, &winMain::OnOptionOneSlot);
    connect(m_optionTwo, &QAction::triggered, this, &winMain::OnOptionTwoSlot);
    connect(m_optionThree, &QAction::triggered, this, &winMain::OnOptionThreeSlot);

//    connect(m_functionWidget, SIGNAL(sigHFSSBuildClicked()), this, SLOT(slotHFSSBuildClicked()));
//    connect(m_functionWidget, SIGNAL(sigCSTBuildClicked()), this, SLOT(slotCSTBuildcliCked()));
//    connect(m_functionWidget, SIGNAL(sigADSBuildcliCked()), this, SLOT(slotADSBuildcliCked()));
//    connect(m_functionWidget, SIGNAL(sigCOMSOLBuildClicked()), this, SLOT(slotCOMSOLBuildClicked()));
//    connect(m_functionWidget, SIGNAL(sigANNPretreatmentClicked()), this, SLOT(slotANNPretreatmentClicked()));
//    connect(m_functionWidget, SIGNAL(sigANNTrainClicked()), this, SLOT(slotANNTrainClicked()));
//    connect(m_functionWidget, SIGNAL(sigANNAutoOptClicked()), this, SLOT(slotANNAutoOptClicked()));
//    connect(m_functionWidget, SIGNAL(sigPythonCPUClicked()), this, SLOT(slotPythonCPUClicked()));
//    connect(m_functionWidget, SIGNAL(sigPythongGPUClicked()), this, SLOT(slotPythongGPUClicked()));
//    connect(m_functionWidget, SIGNAL(sigMATLABTaskClicked()), this, SLOT(slotMATLABTaskClicked()));
//    connect(m_functionWidget, SIGNAL(sigTaskMonitorClicked()), this, SLOT(slotTaskMonitorClicked()));

    connect(m_functionWidget, SIGNAL(sigCSTbuild()), this, SLOT(slotCSTbuild()));
    connect(m_functionWidget, SIGNAL(sigANNtrain()), this, SLOT(slotANNtrain()));
    connect(m_functionWidget, SIGNAL(sigANNused()), this, SLOT(slotANNused()));

    connect(m_functionWidget, SIGNAL(sigANNTrainCancal()), this, SLOT(slotANNTrainCancal()));
    connect(m_functionWidget, SIGNAL(sigANNTrainComfirm(QString)), this, SLOT(slotANNTrainComfirm(QString)));
}

void winMain::deleteOver7daysLog()
{
    //如果主窗体实例指针获取不到立刻返回
//    if(winMain::getInstance()==nullptr)
//        return;

    QDateTime now = QDateTime::currentDateTime();

    //日志存储目录
    //QString logPath = "c:/logs/";
    //获取程序当前运行目录
    QString fileDir = QCoreApplication::applicationDirPath();
    //日志目录
    fileDir +="//Log";

    //如果日志目录不存在,立刻返回
    QDir* dirx = new QDir();
    if(!dirx->exists(fileDir)){
        return;
    }

    //7天前
    QDateTime dateTime1 = now.addDays(-7);
    QDateTime dateTime2;

    QDir dir(fileDir);
    QFileInfoList fileList = dir.entryInfoList();
    foreach (QFileInfo f, fileList )
    {
        //"."和".."跳过
        if (f.baseName() == "")
            continue;

        dateTime2 = QDateTime::fromString(f.baseName(), "yyyy-MM-dd");
        if (dateTime2 < dateTime1)
        {
            dir.remove(f.absoluteFilePath());
        }
    }
}

// 鼠标按键点击事件
void winMain::mousePressEvent(QMouseEvent *qevent)
{
    if(qevent->button() == Qt::LeftButton)//处理对话框拖动, 鼠标左键点击, 记录当前位置
    {
        if(ui->topPanel->rect().contains(qevent->pos()))
        {
            //如果是全屏状态则不让移动
            if(!this->isMaximized())
            {
                m_mousePress = true;
                m_mousePos = qevent->pos();//鼠标相对于窗体的位置（或者使用event->globalPos() - this->pos()）
            }
        }
    }
}

// 鼠标移动事件
void winMain::mouseMoveEvent(QMouseEvent *qevent)
{
    if(m_mousePress)//处理对话框拖动, 鼠标左键被按下，移动对话框
    {
        QPoint pos = qevent->globalPos();//鼠标相对于屏幕的位置
        this->move(pos - m_mousePos);//移动主窗体位置
    }
}

// 鼠标双击事件
void winMain::mouseDoubleClickEvent(QMouseEvent *event)
{
    if(ui->topPanel->rect().contains(event->pos()))
    {
        SlotMax();
    }
}

// 鼠标按键释放事件
void winMain::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);

    m_mousePress = false;//设置鼠标为未被按下
}

// 显示测量数据
void winMain::SlotMeasure()
{

}

//设置按钮
void winMain::SlotSetting()
{

}

//帮助按钮
void winMain::SlotHelp()
{
    m_messageBox->showMeaasge("显示使用说明书");
}

void winMain::OnOptionOneSlot()
{
    qDebug() << "OnOptionOneSlot()";
    m_threadInfo->MoveToCenter();
    m_threadInfo->show();
}

void winMain::OnOptionTwoSlot()
{
    qDebug() << "OnOptionTwoSlot()";
    m_messageBox->showMeaasge("功能未部署");
}

void winMain::OnOptionThreeSlot()
{
    qDebug() << "OnOptionThreeSlot()";
    m_messageBox->showMeaasge("功能未部署");
}

void winMain::slotHFSSBuildClicked()
{
    m_messageBox->showMeaasge("功能未部署");
}

void winMain::slotCSTBuildcliCked()
{
    m_messageBox->showMeaasge("功能未部署");
}

void winMain::slotADSBuildcliCked()
{
    m_messageBox->showMeaasge("功能未部署");
}

void winMain::slotCOMSOLBuildClicked()
{
    m_messageBox->showMeaasge("功能未部署");
}

void winMain::slotANNPretreatmentClicked()
{
    m_messageBox->showMeaasge("功能未部署");
}

void winMain::slotANNTrainClicked()
{
    m_messageBox->showMeaasge("功能未部署");
}

void winMain::slotANNAutoOptClicked()
{
    m_messageBox->showMeaasge("功能未部署");
}

void winMain::slotPythonCPUClicked()
{
    m_messageBox->showMeaasge("功能未部署");
}

void winMain::slotPythongGPUClicked()
{
    m_messageBox->showMeaasge("功能未部署");
}

void winMain::slotMATLABTaskClicked()
{
    m_messageBox->showMeaasge("功能未部署");
}

void winMain::slotTaskMonitorClicked()
{
    m_messageBox->showMeaasge("功能未部署");
}

void winMain::slotCSTbuild()
{
    m_messageBox->showMeaasge("功能未部署");
}

void winMain::slotANNtrain()
{
    setLayoutMini(true);
}

void winMain::slotANNused()
{
    m_messageBox->showMeaasge("功能未部署");
}

void winMain::slotANNTrainCancal()
{
    setLayoutMini(false);
    m_messageBox->showMeaasge("取消参数设置");
}

void winMain::slotANNTrainComfirm(QString jsonText)
{
    Q_UNUSED(jsonText);
    setLayoutMini(false);

    if(m_pDB)
    {
        QString strSql=QLatin1String("update ANNdataInfo set "
                                     "dataJson=:dataJson"
                                     " where dataIndex=:dataIndex");


        //要绑定的字段添加到map
        QMap<QVariant,QVariant> map;
        map.insert(":dataIndex","baseInfo");
        map.insert(":dataJson",jsonText);

        //执行SQL
        m_pDB->ExecuteSql(strSql,map);

        qDebug()<<"success to update picStr INTO table dataInfo!";

        m_messageBox->showMeaasge("参数设置成功");
        return;
    }

    m_messageBox->showMeaasge("参数设置失败");
}

/**查询电脑信息*/
QString winMain::getWMIC(const QString &cmd)
{
    //可以根据要求添加指令函数
    //获取cpu名称：wmic cpu get Name
    //获取cpu核心数：wmic cpu get NumberOfCores
    //获取cpu线程数：wmic cpu get NumberOfLogicalProcessors
    //查询cpu序列号：wmic cpu get processorid
    //查询主板序列号：wmic baseboard get serialnumber
    //查询BIOS序列号：wmic bios get serialnumber
    //查看硬盘：wmic diskdrive get serialnumber
    //查看网卡信息: wmic nicconfig get macaddress

//    查询cpu信息：wmic cpu get DeviceID,name,NumberOfCores,NumberOfLogicalProcessors
//    获取cpu名称：wmic cpu get Name
//    查询cpu序列号：wmic cpu get processorid
//    获取cpu核心数：wmic cpu get NumberOfCores
//    获取cpu线程数：wmic cpu get NumberOfLogicalProcessors
//    查询主板序列号：wmic baseboard get serialnumber
//    获取主板信息：wmic csproduct
//    查询BIOS序列号：wmic bios get serialnumber
//    查询磁盘信息：wmic logicaldisk get name,Description,filesystem,size,freespace
//    查看硬盘：wmic diskdrive get serialnumber
//    查询第一个磁盘序列号：wmic diskdrive where index=0 get serialnumber
//    查询不为空的MAC地址：wmic nic where netconnectionid!=NULL get MacAddress
//    查询内存信息： wmic memorychip get Capacity,TAG
//    查询系统信息： wmic os get Caption,CSDVersion,CSName,SerialNumber,Version

    QProcess p;
    p.start(cmd);
    p.waitForFinished();
    QString result = QString::fromLocal8Bit(p.readAllStandardOutput());
    QStringList list = cmd.split(" ");
    result = result.remove(list.last(), Qt::CaseInsensitive);
    result = result.replace("\r", "");
    result = result.replace("\n", "");
    result = result.simplified();
    return result;
}

QString winMain::getBaseboardNum()
{
     return getWMIC("wmic baseboard get serialnumber");
}

QString winMain::getBIOSNum()
{
     return getWMIC("wmic bios get serialnumber");
}

QString winMain::getCpuId()
{
    return getWMIC("wmic cpu get processorid");
}

QString winMain::getDiskNum()
{
    return getWMIC("wmic diskdrive where index=0 get serialnumber");
}

QString winMain::getIpAddress()
{
    QString localHostName = QHostInfo::localHostName();
    QHostInfo info = QHostInfo::fromName(localHostName);
    foreach(QHostAddress address,info.addresses())
   {

        if(address.protocol() == QAbstractSocket::IPv4Protocol)

            return address.toString();
   }
    return 0;
}

QString winMain::getMAC()
{
    QList<QNetworkInterface> nets = QNetworkInterface::allInterfaces();// 获取所有网络接口列表
    int nCnt = nets.count();
    QString strMacAddr = "";
    for(int i = 0; i < nCnt; i ++)
    {
       // 如果此网络接口被激活并且正在运行并且不是回环地址，则就是我们需要找的Mac地址
       if(nets[i].flags().testFlag(QNetworkInterface::IsUp) && nets[i].flags().testFlag(QNetworkInterface::IsRunning) && !nets[i].flags().testFlag(QNetworkInterface::IsLoopBack))
       {
           strMacAddr = nets[i].hardwareAddress();
           break;
       }
    }
    return strMacAddr;
}

qint32 winMain::getWebTime()
{
    QUdpSocket udpSocket;
    udpSocket.connectToHost("time.windows.com", 123);
    if(udpSocket.waitForConnected(3000)){
        qint8 LI=0;
        qint8 VN=3;
        qint8 MODE=3;
        qint8 STRATUM=0;
        qint8 POLL=4;
        qint8 PREC=-6;
        QDateTime epoch(QDate(1900,1,1));
        qint32 second=quint32(epoch.secsTo(QDateTime::currentDateTime()));
        qint32 temp=0;
        QByteArray timeRequest(48, 0);
        timeRequest[0]=(LI <<6) | (VN <<3) | (MODE);
        timeRequest[1]=STRATUM;
        timeRequest[2]=POLL;
        timeRequest[3]=PREC & 0xff;
        timeRequest[5]=1;
        timeRequest[9]=1;
        timeRequest[40]=(temp=(second&0xff000000)>>24);
        temp=0;
        timeRequest[41]=(temp=(second&0x00ff0000)>>16);
        temp=0;
        timeRequest[42]=(temp=(second&0x0000ff00)>>8);
        temp=0;
        timeRequest[43]=((second&0x000000ff));
        udpSocket.flush();
        udpSocket.write(timeRequest);
        udpSocket.flush();
        if(udpSocket.waitForReadyRead(3000)){
            QByteArray newTime;
            QDateTime epoch(QDate(1900, 1, 1));
            QDateTime unixStart(QDate(1970, 1, 1));
            do
            {
                newTime.resize(udpSocket.pendingDatagramSize());
                udpSocket.read(newTime.data(), newTime.size());
            }while(udpSocket.hasPendingDatagrams());
            QByteArray TransmitTimeStamp ;
            TransmitTimeStamp=newTime.right(8);
            quint32 seconds=TransmitTimeStamp[0];
            quint8 temp=0;
            for(int j=1;j<=3;j++)
            {
                seconds=seconds<<8;
                temp=TransmitTimeStamp[j];
                seconds=seconds+temp;
            }
            quint32 t = seconds-epoch.secsTo(unixStart);
            qDebug() << __FUNCTION__ << "song" << t;
            return t;
//            time.setTime_t(seconds-epoch.secsTo(unixStart));
        }
    }

    return -1;
}

//获取磁盘容量
quint64 winMain::getDiskSpace(QString iDriver, bool flag)
{
    //iDriver为盘符(例如"C\"),flag为true则求磁盘总容量，为false则求磁盘剩余容量
    LPCWSTR strDriver=(LPCWSTR)iDriver.utf16();
    ULARGE_INTEGER freeDiskSpaceAvailable, totalDiskSpace, totalFreeDiskSpace;

    //调用函数获取磁盘参数(单位为字节Byte),转化为GB，需要除以(1024*1024*1024)
    GetDiskFreeSpaceEx( strDriver, &freeDiskSpaceAvailable, &totalDiskSpace, &totalFreeDiskSpace) ;
    if(flag)
    {
        return (quint64) totalDiskSpace.QuadPart/GB;
    }
    else
    {
        return (quint64) totalFreeDiskSpace.QuadPart/GB;
    }
}

//获取电脑所有盘符名
QStringList winMain::getDiskName()
{
    QFileInfoList list = QDir::drives();
    QStringList diskNameList(NULL);

    for (int i=0; i<list.count(); i++)
    {

        QString str = list.at(i).absoluteDir().absolutePath();
        diskNameList.append(str);
    }
    return diskNameList;
}

//设置电脑信息栏改变布局
void winMain::setLayoutMini(bool isMini)
{
    if(isMini)
    {
        ui->bottomLayout->setStretch(0,5);
        ui->bottomLayout->setStretch(1,1);
    }
    else
    {
        ui->bottomLayout->setStretch(0,4);
        ui->bottomLayout->setStretch(1,3);
    }

    m_PCInfo->setMiniMode(isMini);
}

//更新ANN参数设置界面基本设置
void winMain::updataANNInfo()
{
    if(m_pDB)
    {
        //获取需要导出的数据表名
        QString strSql=QLatin1String("select * from ANNdataInfo where dataIndex=:dataIndex");

        //要绑定的字段添加到map
        QMap<QVariant,QVariant> map;
        map.insert(":dataIndex","baseInfo");

        //执行SQL
        QSqlQuery query=m_pDB->searchData(strSql,map);

        //按行遍历
        while(query.next())
        {
            QString dataJson = query.value("dataJson").toString();
            if(dataJson.length() > 10)
                m_functionWidget->setANNInfo(dataJson);
        }
    }
}
