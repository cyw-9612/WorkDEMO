#include "widget.h"
#include "ui_widget.h"
#include <QUdpSocket>
#include <QDateTime>
#include <QDebug>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}

qint32 Widget::getWebTime()
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

void Widget::on_pushButton_clicked()
{
    QDateTime now = QDateTime::fromSecsSinceEpoch(getWebTime());
    QString time = now.toString("yyyy年MM月dd日 hh:mm:ss ddd");
    qDebug() << "the time is :" << time;
}
