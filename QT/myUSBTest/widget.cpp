#include "widget.h"
#include "ui_widget.h"

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


void Widget::on_pushButton_clicked()
{
    ui->textEdit->append("btn push!");
    int res;
    res = hid_init();
    qDebug() << res;

    //打开指定HID设备，第一个参数是设备的VID，第二个参数是设备的PID，
    //第三个是设备的序列号，一般就指定前两个参数第三个参数填0就可以。返回的类似文件操作的句柄，通过它来读写。
    hid_device *handle;
    handle = hid_open(0xC253, 0x1302, NULL);
    qDebug() << handle;

    //读数据，hid_read()返回值为实际读取到的数据长度，handle是通过hid_open() 得到的。
    //读取到的数据就放在buf里面，这几行可以放到线程中，根据长度判断是否有获取到数据再来处理。
    res = 0;
    unsigned char buf1[128];
    res = hid_read(handle, buf1, 128);
    qDebug() << buf1;

    //写数据，这里需要注意的是数组的第一个字节必须是设备的Report ID，如果只支持一个那么就填0，
    //比如需要发送3个字节，0x01,0x02,0x03，那么就需要按照如下操作：
    res = 0;	// 如果返回-1表示发送失败
    unsigned char buf[65];
     buf[0] = 0x00;	// 这就是Report ID
     buf[1] = 0x53;
     buf[2] = 0;
     buf[3] = 0;
     buf[4] = 0;
     buf[5] = 0;
     buf[6] = 0x0;
     buf[7] = 0x0;
     buf[8] = 0x00;
     res = hid_write(handle, buf, 65);
     qDebug() << "hid_write" << ((res==-1)?("失败"):("成功"));

}
