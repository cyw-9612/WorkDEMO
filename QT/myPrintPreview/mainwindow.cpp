#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPrinter>
#include <QPainter>
#include <QTableView>
#include <QStandardItemModel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //设置窗体大小
    this->setMaximumSize(600,800);
    this->setMinimumSize(600,800);
    //生成打印与预览按键
    pushButton = new QPushButton(tr("生成PDF"),this);
    pushButton_printPreview = new QPushButton(tr("打印预览"),this);

    //生成文本框，设置按键大小，文本框大小（大小参考A4纸张）
    textEdit = new QTextEdit(this);
    pushButton->setGeometry(QRect(100,100,100,25));
    pushButton_printPreview->setGeometry(QRect(208,100,100,25));
    textEdit->setGeometry(QRect(100,128,400,500));

    //连接打印浏览信号和保存PDF文件信号
    connect(pushButton,SIGNAL(clicked()),this,SLOT(CreatPdf()) );
    connect(pushButton_printPreview,SIGNAL(clicked()),this,SLOT(PrintPreview()) );
}

MainWindow::~MainWindow()
{
    delete ui;
}

//生成PDF文件
void MainWindow::CreatPdf()
{
    QPrinter printer;
    QFileDialog fileDialog;
    QString str = QFileDialog::getSaveFileName(this,tr("Save Text"),"../",tr("Text Files (*.pdf)"));
    //设置文件输出格式
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setPageSize(QPrinter::A4);
    printer.setOutputFileName(str);
    textEdit->print(&printer);
}

//打印浏览
void MainWindow::PrintPreview()
{
    QPrinter printer;
    QPrintPreviewDialog previewDialog(&printer,this);
    connect(&previewDialog,SIGNAL(paintRequested(QPrinter*)),this,SLOT(Preview(QPrinter*) ) );
    previewDialog.exec(); //打印对话框显示，paintRequest触发
}

void MainWindow::Preview(QPrinter *printer)
{
    textEdit->print(printer);
}

