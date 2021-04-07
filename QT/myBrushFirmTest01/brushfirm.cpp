#include "brushfirm.h"
#include "ui_brushfirm.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QtDebug>

#include "../file/directoryutil.h"

BrushFirm::BrushFirm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::BrushFirm)
{
    ui->setupUi(this);
    //初始化
    initUI();

}

BrushFirm::~BrushFirm()
{
    delete ui;
}

//初始化函数
void BrushFirm::initUI()
{
    //未开始下载，进度条隐藏
    ui->progressBar->hide();

    // 初始化串口
    m_serial = new MySerialPort;
    // 寻找可用串口并显示
    QStringList serialStrList;
    serialStrList = m_serial->scanSerial();
    for (int i=0; i<serialStrList.size(); i++)
    {
        ui->comboBox_port->addItem(serialStrList[i]); // 在comboBox那添加串口号
    }
    //连接进度信号和进度显示槽函数
    connect(&core_, &Core::onDownloadProgress,
            this, &BrushFirm::OnDownloadProgress);
}

//选择文件按钮
void BrushFirm::on_btn_chooseFile_clicked()
{
    QString filename;
    //选择文件，添加./为绝对路径，设置文件类型为*.bin
    filename = QFileDialog::getOpenFileName(this,
        QApplication::translate("IAP固件升级下载窗口", "选择Bin文件"), //title
          "./", //path
          "Video files (*.bin);;All files (*.*)"); //filter
    //如果文件名为空则报错
    if(filename.isEmpty()) {

       OnDownloadProgress(false, false, 0, QApplication::translate("IAP固件升级下载窗口",
                                                                "未选择Bin文件 !"));
       QMessageBox::warning(this, QApplication::translate("IAP固件升级下载窗口", "错误"),
       QApplication::translate("IAP固件升级下载窗口", "没有选择hex或bin文件 !"));
       return;
    }
    //文档框显示文件路径
    ui->textEdit_filePath->setText(tr("选择文件:") + filename);
    //设置文件绝对路径
    core_.SetHexFileAbsPath(filename);
}

//更新进度条
void BrushFirm::OnDownloadProgress(bool is_finish, bool is_successed, int percent, QString note)
{
    ui->progressBar->show();
    Q_UNUSED(is_successed)
    if(is_finish) {
        SetUiDisable(false);
    }
    //设置文件名，更新进度
    ui->textEdit_filePath->setText(note);
    ui->progressBar->setValue(percent);
    //调试用
//    qDebug() << "is_finish=" << is_finish;
//    qDebug() << "percent=" << percent;
//    qDebug() << "note=" << note;
}

//设置面板UI按钮状态
void BrushFirm::SetUiDisable(bool disable)
{
    ui->btn_downlode->setEnabled(disable);
}

//下载按钮，设置打开串口，测试连接成功
//发送开始指令等待设备响应开始下载文件
void BrushFirm::on_btn_downlode_clicked()
{
    //判断文件绝对路径
    if(!DirectoryUtil::IsFileExist(core_.GetHexFileAbsPath()))
    {
        OnDownloadProgress(false, false, 0, QApplication::translate("IAP固件升级下载窗口",
                                                                    "请首先选择一个bin文件!!"));
        return;
    }
     SetUiDisable(true);
    //开启串口
    //启动新的线程来处理数据
    core_.SetBaudRate(QSerialPort::BaudRate::Baud115200);
    //core_.SetQSerialPortInfo(serialPortInfo);
    core_.SetPortName(ui->comboBox_port->currentText());
    core_.StartWorkingThread();

}
