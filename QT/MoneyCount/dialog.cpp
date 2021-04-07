#include "dialog.h"
#include "ui_dialog.h"
#include <QFileInfo>
#include <QDebug>
#include <QTimer>
#include <QMouseEvent>

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    setWindowFlags(Qt::FramelessWindowHint);
    ui->setupUi(this);
    //设置窗体透明
//    this->setAttribute(Qt::WA_TranslucentBackground);
    QString configFileName = QCoreApplication::applicationDirPath() + "/Config.ini";
    m_psetting = new QSettings(configFileName, QSettings::IniFormat);
    QFileInfo fileInfo(configFileName);
    if(!fileInfo.exists())
    {
        //打印创建文件的路径
        qDebug("%s is not exists.", configFileName.toLatin1().data());
        //写入默认值
        SetConfigData("money_counter", "oneSecond", x);
    }
    money_counter_oneSecond = GetConfigData("money_counter", "oneSecond").toDouble();

    if (money_counter_oneSecond <= 0)
    {
        money_counter_oneSecond = x;
    }

    readAuthHistoryFile();
    m_timer = new QTimer(this);
    m_timer->start(1000);
    //连接超时信号和槽函数
    connect(m_timer, SIGNAL(timeout()), this, SLOT(slotTimtOut()));
    this->setStyleSheet("#Dialog{border-image: url(:/pictus/t010d8cda7193fa4099.jpg);}");
    //最小化
    this->showMinimized();
    this->move(0,0);
}

Dialog::~Dialog()
{
    saveAuthHistory();
    delete ui;
}

void Dialog::slotTimtOut()
{
    double y = ui->lcdNumber->value();
    y = y + money_counter_oneSecond;
    ui->lcdNumber->display(y);
}

/**读取本地文件*/
void Dialog::readAuthHistoryFile()
{
    //阅读本地历史记录文件
    //获取程序当前运行目录
    QString pathApp = QCoreApplication::applicationDirPath();
    QString pathFile=pathApp + "/History.ini";
    //如果文件不存在则返回，如果有记录则上传记录，上传成功则删除文件，反之保留
    QFileInfo fileInfo(pathFile);
    if(fileInfo.isFile())
    {
        QFile file(pathFile);
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        QByteArray history = file.readAll();
        double num = history.toDouble();
        ui->lcdNumber->display(num);
        file.close();
        //读取完成删除文件
        QFile::remove(pathFile);
    }

}

/**记录历史到文件*/
void Dialog::saveAuthHistory()
{
    //上传授权历史失败，则把历史信息保存到本地文件
    QString pathApp = QCoreApplication::applicationDirPath();
    QString pathFile=pathApp + "/History.ini";
    //当下次启动时先检测文件，如果有内容则上传，上传完成删除文件
    QFile file(pathFile);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    double count = ui->lcdNumber->value();
    qDebug() << count;
    QByteArray count1 =  QString("%1").arg(count).toLocal8Bit();
    file.write(count1);
    file.close();
}

//设置配置文件
void Dialog::SetConfigData(QString qstrnodename, QString qstrkeyname, QVariant qvarvalue)
{
    if (m_psetting)
    {
        m_psetting->setValue(QString("/%1/%2").arg(qstrnodename).arg(qstrkeyname), qvarvalue);
    }
}

//读配置文件
QVariant Dialog::GetConfigData(QString qstrnodename, QString qstrkeyname)
{
    QVariant qvar = -1;
    if (m_psetting)
    {
        qvar = m_psetting->value(QString("/%1/%2").arg(qstrnodename).arg(qstrkeyname));
    }
    return qvar;
}


void Dialog::on_pushButton_clicked()
{
    if(m_timer->isActive())
    {
        m_timer->stop();
    }
    else
    {
        m_timer->start(1000);
    }
}

void Dialog::on_pushButton_2_clicked()
{
    QApplication::exit();
}

void Dialog::mousePressEvent(QMouseEvent *event)
{
     this->m_windowPos = this->pos();                // 获得部件当前位置
     this->m_mousePos = event->globalPos();     // 获得鼠标位置
     this->m_dPos = m_mousePos - m_windowPos;       // 移动后部件所在的位置
}

void Dialog::mouseMoveEvent(QMouseEvent *event)
{
     this->move(event->globalPos() - this->m_dPos);
}
