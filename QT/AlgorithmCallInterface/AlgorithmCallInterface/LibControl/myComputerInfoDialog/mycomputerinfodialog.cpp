#include "mycomputerinfodialog.h"
#include "ui_mycomputerinfodialog.h"
#include "SysInfo/sysinfo.h"

myComputerInfoDialog::myComputerInfoDialog(QWidget *parent) :
    ChildView(parent),
    ui(new Ui::myComputerInfoDialog),
    mCpuWidget(this),
    mMemoryWidget(this)
{
    ui->setupUi(this);

    initUI();

    setStyle();

    setCtrlRange();

    connectSlots();
}

myComputerInfoDialog::~myComputerInfoDialog()
{
    delete ui;
}

//设置风格
void myComputerInfoDialog::setStyle()
{

}

//关联信号槽
void myComputerInfoDialog::connectSlots()
{

}

//初始化界面
void myComputerInfoDialog::initUI()
{
    SysInfo::instance().init();
    ui->Layou_info->addWidget(&mCpuWidget);
    ui->Layou_info->addWidget(&mMemoryWidget);
    ui->Layou_info->setStretch(0, this->height());
    ui->Layou_info->setStretch(1, this->width() - this->height());
}

//设置控件输入范围
void myComputerInfoDialog::setCtrlRange()
{

}
