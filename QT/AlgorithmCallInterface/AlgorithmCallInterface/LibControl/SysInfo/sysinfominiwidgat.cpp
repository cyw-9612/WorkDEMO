#include "sysinfominiwidgat.h"
#include "ui_sysinfominiwidgat.h"
#include "sysinfo.h"

sysInfoMiniWidgat::sysInfoMiniWidgat(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::sysInfoMiniWidgat)
{
    ui->setupUi(this);
}

sysInfoMiniWidgat::~sysInfoMiniWidgat()
{
    delete ui;
}

void sysInfoMiniWidgat::updateCPULab(double cpuText)
{
    QString text = QString::number(cpuText, 'f', 1) + "%";
    ui->lab_CPU->setText(text);
}

void sysInfoMiniWidgat::updateMemoryLab(double memoryText)
{
    QString text = QString::number(memoryText, 'f', 1) + "%";
    ui->lab_memory->setText(text);
}
