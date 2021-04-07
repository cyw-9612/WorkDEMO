#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <QSettings>

QString Widget::ipServer;
int Widget::portServer;

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    ui->edit_ip->setText(Widget::ipServer);
    ui->edit_port->setText(QString::number(Widget::portServer,10));

    connect(ui->btn_read,SIGNAL(clicked()), this ,SLOT(slotReadIni()));
    connect(ui->btn_write,SIGNAL(clicked()), this ,SLOT(slotWriteIni()));
}

Widget::~Widget()
{
    qDebug() << Widget::ipServer;
    qDebug() <<Widget:: portServer;
    delete ui;
}

void Widget::slotReadIni()
{
    QSettings *configIniRead = new QSettings("serverAddress.ini", QSettings::IniFormat);
   //将读取到的ini文件保存在QString中，先取值，然后通过toString()函数转换成QString类型
   Widget::ipServer = configIniRead->value("/ip/first").toString();
   Widget::portServer = configIniRead->value("/port/open").toInt();
   ui->edit_ip->setText(Widget::ipServer);
   ui->edit_port->setText(QString::number(Widget::portServer,10));
   //读入入完成后删除指针
   delete configIniRead;
}

void Widget::slotWriteIni()
{
    QSettings *configIniWrite = new QSettings("serverAddress.ini", QSettings::IniFormat);
     //向ini文件中写入内容,setValue函数的两个参数是键值对
     //向ini文件的第一个节写入内容,ip节下的第一个参数
     configIniWrite->setValue("/ip/first", ui->edit_ip->text());
     //向ini文件的第二个节写入内容,port节下的第一个参数
     configIniWrite->setValue("port/open", ui->edit_port->text());
     //写入完成后删除指针
    delete configIniWrite;
}

