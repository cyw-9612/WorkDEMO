#include "widget.h"
#include "ui_widget.h"

#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <windows.h>
#include <QDebug>
#include <QPrinter>



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
    filename = QFileDialog::getOpenFileName(this,
                                                QApplication::translate("选择文件窗口", "选择PDF文件"), //title
                                                  "./", //path
                                                  "Video files (*.PDF);;All files (*.*)"); //filter
                                            //如果文件名为空则报错
    if(filename.isEmpty()) {

       QMessageBox::warning(this, QApplication::translate("选择文件窗口", "错误"),
       QApplication::translate("选择文件窗口", "没有选择PDF文件 !"));
       return;
    }
    qDebug() << filename;
}


void Widget::on_pushButton_2_clicked()
{
    if(printPDF(filename))
    {
        qDebug() << "success print" ;
    } else
    {
        qDebug() << "fail print" ;
    }

}

bool Widget::printPDF(QString fileName)
{
    QPrinter printer;
    printer.setPageSize(QPrinter::A4); //页面设置
    printer.setOrientation(QPrinter::Portrait); //方向
    QString printerName = printer.printerName();
    // no default printer or no file return false
    if( printerName.size() == 0 || fileName.size() == 0)
        return false;
    QFileInfo fi(fileName);
    QString filePath = fi.absoluteFilePath();
    if(!fi.exists())
        return false;
    int ret = 0;
    ret = (int)ShellExecuteW(NULL,
                             QString("print").toStdWString().c_str(),
                             filePath.toStdWString().c_str(),
                             NULL,
                             NULL,
                             SW_HIDE);
    // if return number bigger than 31 indicate succ
    qDebug()<<"ret:"<<ret<<"--SE_ERR_NOASSOC:"<<SE_ERR_NOASSOC;
    qDebug()<<"filePath:"<<filePath.toStdWString().c_str();
    if (ret > SE_ERR_NOASSOC)
        return true;
    return false;

}



void Widget::on_horizontalSlider_sliderMoved(int position)
{
    ui->progressBar->setValue(position);
}
