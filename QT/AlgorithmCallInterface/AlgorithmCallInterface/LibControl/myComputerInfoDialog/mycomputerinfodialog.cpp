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
    connect(&mCpuWidget, SIGNAL(sigCpuData(double)), m_miniInfoWIdget, SLOT(updateCPULab(double)));
    connect(&mMemoryWidget, SIGNAL(sigMemoryData(double)), m_miniInfoWIdget, SLOT(updateMemoryLab(double)));
}

//初始化界面
void myComputerInfoDialog::initUI()
{
    SysInfo::instance().init();
    ui->Layou_info->addWidget(&mCpuWidget);
    ui->Layou_info->addWidget(&mMemoryWidget);
    ui->Layou_info->setStretch(0, this->height());
    ui->Layou_info->setStretch(1, this->width() - this->height());

    m_miniInfoWIdget = new sysInfoMiniWidgat(ui->widget_Info);
    ui->Layou_info->addWidget(m_miniInfoWIdget);
    ui->Layou_info->setStretch(2,0);
    m_miniInfoWIdget->hide();
}

//设置控件输入范围
void myComputerInfoDialog::setCtrlRange()
{

}

void myComputerInfoDialog::setMiniMode(bool isMini)
{
    if(isMini)
    {
        mCpuWidget.hide();
        mMemoryWidget.hide();
        m_miniInfoWIdget->show();
        ui->Layou_info->setStretch(0, 0);
        ui->Layou_info->setStretch(1, 0);
        ui->Layou_info->setStretch(2, 1);

        ui->gridLayout_2->setRowStretch(3,30);
    }
    else
    {
        mCpuWidget.show();
        mMemoryWidget.show();
        m_miniInfoWIdget->hide();
        ui->Layou_info->setStretch(0, this->height());
        ui->Layou_info->setStretch(1, this->width() - this->height());
        ui->Layou_info->setStretch(2, 0);

        ui->gridLayout_2->setRowStretch(3,100);
    }
}
