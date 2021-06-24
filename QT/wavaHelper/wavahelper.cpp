#include "wavahelper.h"
#include "ui_wavahelper.h"
#include <QMutex>
#include <QMouseEvent>
#include <QDebug>
#include <QFileDialog>
#include <QScrollBar>
#include <QTextCodec>
#include <QSettings>
#include <QFuture>
#include <QtConcurrent>
#include <qvalidator.h>
#include <math.h>
#include "rtmasklayer.h"

//初始化类的静态成员变量
QMutex wavaHelper::m_Mutex;
QSharedPointer<wavaHelper> wavaHelper::m_pInstance=nullptr;

wavaHelper::wavaHelper(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::wavaHelper)
{
    ui->setupUi(this);
    //设置程序标题
    setWindowTitle(tr("录音转换工具"));

    setWindowFlags(Qt::Window|Qt::MSWindowsFixedSizeDialogHint | Qt::FramelessWindowHint |Qt::WindowSystemMenuHint|Qt::WindowMinimizeButtonHint|Qt::WindowMaximizeButtonHint);
    this->setStyleSheet("border:1px");

    //初始化 QFuture 监视器
    myWatcher = new QFutureWatcher<void>();
    //绑定监视器结束信号和处理槽
    connect(myWatcher, SIGNAL(finished()), this, SLOT(slotReadFinish()));

    //设置程序图标
    setWindowIcon(QIcon(":/images/icon.png"));

    m_message = new miniMeaasgeBox();

    //UI初始化
    initUI();

    //连接信号槽
    connectSlots();
}

wavaHelper::~wavaHelper()
{
    if(m_message != nullptr)
    {
        delete m_message;
        m_message = nullptr;
    }

    delete ui;

    qDebug() << "~wavaHelper";
}

//UI初始化
void wavaHelper::initUI()
{
    //设置需要遮罩的父窗体
    RtMaskLayer::Instance()->SetMainWidget(this);
    RtMaskLayer::Instance()->setGeometry(0, 0, 800, 600);
    RtMaskLayer::Instance()->hide();

    //设置哪些弹窗窗体需要被遮罩
    QStringList dialogNames;
    dialogNames << "miniMeaasgeBox";

    RtMaskLayer::Instance()->SetDialogNames(dialogNames);

    ui->btn_saveFile->setEnabled(false);
    ui->btn_start->setEnabled(false);

    ui->edit_decline->setEnabled(false);

    QValidator* validator = new QIntValidator( 1, 200, this );

    ui->edit_enlarge->setValidator(validator);
    ui->edit_decline->setValidator(validator);

    //读取默认配置
    m_configIni = new QSettings("configurationFile.ini", QSettings::IniFormat);
   //将读取到的ini文件保存在QString中
    bool isEnlarge = m_configIni->value("/checkSet/isEnlarge").toBool();
    QString setNum = m_configIni->value("/number/setValue").toString();
    ui->checkBox_enlarge->setChecked(isEnlarge);
    ui->checkBox_decline->setChecked(!isEnlarge);
    slotChoseEnlarge(isEnlarge);
    if(isEnlarge)
    {
        ui->edit_enlarge->setText(setNum);
    }
    else
    {
        ui->edit_decline->setText(setNum);
    }

}

//连接信号槽
void wavaHelper::connectSlots()
{
    connect(ui->btnMin, SIGNAL(clicked()), this, SLOT(slotMin()) );//最小化按钮
    connect(ui->btnClose, SIGNAL(clicked()), this, SLOT(slotClose()));//关闭按钮

    connect(ui->btn_choseFile, SIGNAL(clicked()), this, SLOT(slotChoseFile()));
    connect(ui->btn_saveFile, SIGNAL(clicked()), this, SLOT(slotSaveFile()));

    connect(ui->checkBox_enlarge, SIGNAL(clicked(bool)), this, SLOT(slotChoseEnlarge(bool)));
    connect(ui->checkBox_decline, SIGNAL(clicked(bool)), this, SLOT(slotChoseEnlarge(bool)));

    connect(ui->btn_start, SIGNAL(clicked()), this, SLOT(slotStartToHandle()));
}

//鼠标按下事件
void wavaHelper::mousePressEvent(QMouseEvent *event)
{
    //鼠标左键按下
    if(event->button()==Qt::LeftButton)
    {
        //顶层布局矩形区域
        QRect rectTop=ui->widget_top->rect();
        //鼠标按下点落在顶层标题区域
        if(rectTop.contains(event->pos()))
        {
            this->m_windowPos = this->pos();           // 目前控件在父窗口中的位置
            this->m_mousePos = event->globalPos();     // 获得鼠标位置
            this->m_dPos = m_mousePos - m_windowPos;       // 移动后部件所在的位置

            m_moveFlag=true;
        }
    }
}

//鼠标移动事件
void wavaHelper::mouseMoveEvent(QMouseEvent *event)
{
    if(m_moveFlag)
    {
        //拖动窗体
        this->move(event->globalPos() - this->m_dPos);
    }
}

//释放鼠标
void wavaHelper::mouseReleaseEvent(QMouseEvent *event)
{Q_UNUSED(event)
    m_moveFlag=false;
}

//槽函数:最小化
void wavaHelper::slotMin()
{
    this->showMinimized();
}

//槽函数:关闭
void wavaHelper::slotClose()
{
    close();
}

//选择音频文件
void wavaHelper::slotChoseFile()
{

    //打开文件操作
    m_Filename = QFileDialog::getOpenFileName(
                this, tr("open WAV file"),
                "./", tr("WAV files(*.WAV *.wav);;All files (*.*)"));

    if(m_Filename.isEmpty())
    {
        m_message->setWidgetMessage(tr("错误提示"), tr("未选择文件！"), Colortype::red);
        m_message->show();
        return;
    }

    m_message->setWidgetMessage(tr("提示"), tr("正在解析文件,请稍后"), Colortype::black);
    m_message->show();

    //显示文件路径
    ui->edit_oldFile->setText(m_Filename);

    //处理文件名 Qt 中默认的编码为UTF-8，故在windows下需要先转码才能打正确打开。
    QTextCodec *code = QTextCodec::codecForName("GB2312");
    std::string name = code->fromUnicode(m_Filename).data();

    //置标志位
    isReadFile = true;

    //使用 QFuture 开启子线程处理耗时业务
    QFuture<void> future = QtConcurrent::run(&m_Wavefile, &WaveFile::WavRead,name);
    //监视器监视 QFuture
    myWatcher->setFuture(future);

}

//选择文件存储路径
void wavaHelper::slotSaveFile()
{
    QFileInfo info(m_Filename);
    QString file_suffix = info.suffix();
    QString newfile_path = info.absoluteFilePath().left(info.absoluteFilePath().length() - file_suffix.length() - 1) + "_new.WAV";
    m_saveFilename = QFileDialog::getSaveFileName(this,
            tr("Save WAV file"),
            newfile_path,tr("WAV files(*.WAV *.wav);;All files (*.*)")); //选择路径

    if(m_saveFilename.isEmpty())
    {
        m_message->setWidgetMessage(tr("错误提示"), tr("选择路径为空!"), Colortype::red);
        m_message->show();
        return;
    }

    //显示文件路径
    ui->edit_newFile->setText(m_saveFilename);

    ui->btn_start->setEnabled(true);
}

//选择放大倍数
void wavaHelper::slotChoseEnlarge(bool checked)
{
    Q_UNUSED(checked);
    bool isEnlarge = ui->checkBox_enlarge->isChecked();

    ui->edit_enlarge->setEnabled(isEnlarge);
    ui->edit_decline->setEnabled(!isEnlarge);

    QString canEdit = "QWidget{font: large /'SourceHanSansK-Regular/';\
            background-color: rgb(238,238,238);\
        font-size:16px;border-width:1px;border-color:rgba(10,152,182,1);\
        border-style: solid;border-radius:2px;padding: 1px 1px}";
    QString canntEdit = "QWidget{font: large /'SourceHanSansK-Regular/';\
            background-color: rgb(255,255,255);\
        font-size:16px;border-width:1px;border-color:rgba(238,238,238,1);\
        border-style: solid;border-radius:2px;padding: 1px 1px}";

    if(isEnlarge)
    {
        ui->edit_enlarge->setStyleSheet(canEdit);
        ui->edit_decline->setStyleSheet(canntEdit);
        ui->edit_decline->clear();
    }
    else
    {
            ui->edit_enlarge->setStyleSheet(canntEdit);
            ui->edit_decline->setStyleSheet(canEdit);
            ui->edit_enlarge->clear();
    }
}

//开始进行音频处理
void wavaHelper::slotStartToHandle()
{
    m_message->setWidgetMessage(tr("提示"), tr("正在生成文件,请稍等"), Colortype::black);
    m_message->show();

    ui->btn_start->setEnabled(false);

    //置标志位
    isReadFile = false;

    //使用 QFuture 开启子线程处理耗时业务
    QFuture<void> future1 = QtConcurrent::run(this,&wavaHelper::writeNewWavFile);
    //监视器监视 QFuture
    myWatcher->setFuture(future1);

}

//根据设置生成新的文件
void wavaHelper::writeNewWavFile()
{
    //标志位复位
    isErrorForHandle = false;
    //记录放大的倍数
    QString strMultiple;
    double multiple;
    if(ui->checkBox_enlarge->isChecked())
    {
        if(ui->edit_enlarge->text().isEmpty())
        {
            m_message->setWidgetMessage(tr("错误提示"), tr("修改倍数不能为空!"), Colortype::red);
            m_message->show();
            isErrorForHandle = true;
            return;
        }

        strMultiple = ui->edit_enlarge->text();
        multiple = ui->edit_enlarge->text().toDouble();
        if(multiple < 1 || multiple > 200)
        {
            m_message->setWidgetMessage(tr("错误提示"), tr("修改倍数设置异常!"), Colortype::red);
            m_message->show();
            isErrorForHandle = true;
            return;
        }
    }
    else
    {
        if(ui->edit_decline->text().isEmpty())
        {
            m_message->setWidgetMessage(tr("错误提示"), tr("修改倍数不能为空!"), Colortype::red);
            m_message->show();
            isErrorForHandle = true;
            return;
        }

        strMultiple = ui->edit_decline->text();
        multiple = ui->edit_decline->text().toDouble();
        if(multiple < 1 || multiple > 200)
        {
            m_message->setWidgetMessage(tr("错误提示"), tr("修改倍数设置异常!"), Colortype::red);
            m_message->show();
            isErrorForHandle = true;
            return;
        }
        multiple = 1/multiple;
    }

    if(ui->textEdit_fileInfo->toPlainText().contains("6228+"))
    {
        if(ui->checkBox_decline->isChecked())
        {
            multiple = 1/multiple;
            multiple = qPow(10,multiple/1000);
            multiple = 1/multiple;
        }
        else
        {
          multiple = qPow(10,multiple/1000);
        }

    }

    qDebug() << multiple;

    //创建文件
    m_file = new QFile(m_saveFilename);
    if(!m_file->open(QIODevice::WriteOnly))
    {
        delete m_file;
        m_file = 0;
        //以只写的形式打开文件失败
        m_message->setWidgetMessage(tr("错误提示"), tr("以只写的形式打开文件失败!"), Colortype::red);
        m_message->show();
        isErrorForHandle = true;
        return ;
    }

    //写数据到文件中
    WaveFile::AWA6292Wavehead pack = m_Wavefile.head;
    if(m_file)
    {
        //写入文件头
        QByteArray byteArray;
        //分配结构大小的空间，不可缺少
        byteArray.resize(sizeof(pack));

        //结构体转QByteArray
        memcpy(byteArray.data(), &pack, sizeof(pack));

        //转成十六进制字符串
        QString strHex=byteArray.toHex();
        //qDebug()<<"结构体数据为:"<<strHex;
        //从十六进制字符串转成字符串数组
        QByteArray byteHex = QByteArray::fromHex (strHex.toLatin1());
        m_file->write(byteHex);

        QByteArray data("data");
        m_file->write(data);

        QByteArray dataLeng;
        //分配结构大小的空间，不可缺少
        dataLeng.resize(sizeof(m_Wavefile.datalength));
        memcpy(dataLeng.data(), &m_Wavefile.datalength, sizeof(m_Wavefile.datalength));
        m_file->write(dataLeng);
    }

    int count = 0;
    //遍历文件内容进行编辑再存储
    if(m_Wavefile.bitpersample==32)
    {
        for(int i=0; i<m_Wavefile.datanum;i++)  //读入数据
        {
            qint32 data = m_Wavefile.Data[i] * multiple;
            if(data >= 2147483646)
            {
                data = 2147483646;
                count++;
            }
            if(data <= -2147483646)
            {
                data = -2147483646;
                count++;
            }

            QByteArray oneBitData;
            //分配结构大小的空间，不可缺少
            oneBitData.resize(sizeof(data));
            memcpy(oneBitData.data(), &data, sizeof(data));
            m_file->write(oneBitData);
            //qDebug() << "已解析第" << i << "位数据,还有" << m_Wavefile.datanum - i << "位待写入";
        }
    }
    else if(m_Wavefile.bitpersample==16)
    {
        for(int i=0; i<m_Wavefile.datanum;i++)  //读入数据
        {
            qint16 data = m_Wavefile.Data[i] * multiple;
            QByteArray oneBitData;
            //分配结构大小的空间，不可缺少
            oneBitData.resize(sizeof(data));
            memcpy(oneBitData.data(), &data, sizeof(data));
            m_file->write(oneBitData);
        }
    }
    else if(m_Wavefile.bitpersample==8)
    {
        for(int i=0; i<m_Wavefile.datanum;i++)  //读入数据
        {
            qint8 data = m_Wavefile.Data[i] * multiple;
            QByteArray oneBitData;
            //分配结构大小的空间，不可缺少
            oneBitData.resize(sizeof(data));
            memcpy(oneBitData.data(), &data, sizeof(data));
            m_file->write(oneBitData);
        }
    }

    //关闭文件
    if(m_file)
    {
        m_file->close();
        delete  m_file;
        m_file = 0 ;
    }

    //保存此次设置参数
    m_configIni->setValue("/checkSet/isEnlarge",ui->checkBox_enlarge->isChecked());
    m_configIni->setValue("/number/setValue",strMultiple);

    if(count > 50000)
    {
        m_message->setWidgetMessage(tr("提示"), tr("倍数设置过大，建议重新设置!"), Colortype::red);
        m_message->show();
        isErrorForHandle = true;
    }
}

//分析文件信息
void wavaHelper::analyFileInfo(QString Text)
{
    QStringList titleList;
    titleList << tr("仪器型号：") << tr("测量时间：") << tr("校准记录：") << tr("仪器序列号：") << tr("传声器信息：") << tr("") << tr("");

    ui->textEdit_fileInfo->clear();
    ui->textEdit_fileInfo->append(tr("采样频率：") + QString::number(m_Wavefile.head.samplerate));
    ui->textEdit_fileInfo->append(tr("通道数：") + QString::number(m_Wavefile.head.channelnum));
    ui->textEdit_fileInfo->append(tr("数据格式：短整型"));
    //qDebug() << Text;
    if(Text.contains("AWA6292"))
    {
        QStringList textList = Text.split("\\r\\n");
        m_type = textList[0];
        for (int i = 0; i < textList.count(); i++)
        {
            ui->textEdit_fileInfo->append(titleList[i] + textList[i]);
        }
    }
    else if(Text.contains("AWA6228+"))
    {
        //由于数据分块信息不明确，测出做特殊处理
        QString type = Text.left(8);
        ui->textEdit_fileInfo->append(titleList[0] + type);
        //6228+ 仪器信息与时间之间有8位乱码
        QString lestText = Text.right(Text.length() - 8 - 8);

        QString time = lestText.left(19);
        ui->textEdit_fileInfo->append(titleList[1] + time);
        //减去19位时间数据加一个空格
        lestText = Text.right(lestText.length() - 20);

        QStringList textList = lestText.split("\r\n");
        for (int i = 0; i < textList.count(); i++)
        {
            ui->textEdit_fileInfo->append(titleList[i+2] + textList[i]);
        }
    }
    else
    {
        m_message->setWidgetMessage(tr("错误提示"), tr("解析错误，请选择正确的文件"), Colortype::red);
        m_message->show();
        ui->btn_saveFile->setEnabled(false);
        ui->btn_start->setEnabled(false);
        return;
    }
    //获取窗口滚动条
    QScrollBar* pScroll=ui->textEdit_fileInfo->verticalScrollBar();
    pScroll->setValue(0);
    //开启保存按钮
    ui->btn_saveFile->setEnabled(true);
    ui->btn_start->setEnabled(false);
    //清空保存文件空格信息
    ui->edit_newFile->clear();
}

//完成读取文件的分析
void wavaHelper::slotReadFinish()
{
    if(isReadFile)
    {
        //显示解析的文件
        //qDebug() << m_Wavefile.head.factText;
        QString text;
        for (int i = 0; i < sizeof(m_Wavefile.head.factText); i++)
        {
            text.append(m_Wavefile.head.factText[i]);
        }
        m_message->hide();
        //QString text = QString::fromUtf8(m_Wavefile.head.factText);
        //QString text(m_Wavefile.head.factText);
        //qDebug() << text;
        analyFileInfo(text);
    }
    else
    {
        //如果没有错误则显示解析完成
        if(!isErrorForHandle)
        {
            m_message->hide();
            m_message->setWidgetMessage(tr("提示"), tr("解析完成!"), Colortype::black);
            m_message->show();
        }

        ui->btn_start->setEnabled(true);
    }

}

