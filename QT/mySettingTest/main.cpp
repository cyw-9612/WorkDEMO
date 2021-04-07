#include "widget.h"

#include <QApplication>
#include <QtCore/QCoreApplication>
#include <QSettings>
#include <QString>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

//    QSettings *configIniWrite = new QSettings("hahaya.ini", QSettings::IniFormat);
//       //向ini文件中写入内容,setValue函数的两个参数是键值对
//       //向ini文件的第一个节写入内容,ip节下的第一个参数
//       configIniWrite->setValue("/ip/first", "192.168.0.1");
//       //向ini文件的第一个节写入内容,ip节下的第二个参数
//       configIniWrite->setValue("ip/second", "127.0.0.1");
//       //向ini文件的第二个节写入内容,port节下的第一个参数
//       configIniWrite->setValue("port/open", "2222");
//       //写入完成后删除指针
//       delete configIniWrite;

    QSettings *configIniRead = new QSettings("serverAddress.ini", QSettings::IniFormat);
   //将读取到的ini文件保存在QString中，先取值，然后通过toString()函数转换成QString类型
   Widget::ipServer = configIniRead->value("/ip/first").toString();
   Widget::portServer = configIniRead->value("/port/open").toInt();
   //打印得到的结果
   qDebug() << Widget::ipServer;
   qDebug() << Widget::portServer;
   //读入入完成后删除指针
   delete configIniRead;

    Widget w;
    w.show();
    return a.exec();
}
