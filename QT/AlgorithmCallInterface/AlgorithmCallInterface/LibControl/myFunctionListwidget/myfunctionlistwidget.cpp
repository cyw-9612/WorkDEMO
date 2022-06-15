#include "myfunctionlistwidget.h"
#include "ui_myfunctionlistwidget.h"

myFunctionListwidget::myFunctionListwidget(QWidget *parent) :
    ChildView(parent),
    ui(new Ui::myFunctionListwidget)
{
    ui->setupUi(this);

    initUI();

    setStyle();

    setCtrlRange();

    connectSlots();
}

myFunctionListwidget::~myFunctionListwidget()
{
    delete ui;
}

//设置风格
void myFunctionListwidget::setStyle()
{

}

//关联信号槽
void myFunctionListwidget::connectSlots()
{

}

//初始化界面
void myFunctionListwidget::initUI()
{
    ui->btn_HFSSBuild->setEnabled(false);

    ui->btn_ADSBuild->setEnabled(false);

    ui->btn_PythonGPU->setEnabled(false);
}

//设置控件输入范围
void myFunctionListwidget::setCtrlRange()
{

}

void myFunctionListwidget::on_btn_HFSSBuild_clicked()
{
    emit sigHFSSBuildClicked();
}

void myFunctionListwidget::on_btn_CSTBuild_clicked()
{
    emit sigCSTBuildClicked();
}

void myFunctionListwidget::on_btn_ADSBuild_clicked()
{
    emit sigADSBuildcliCked();
}

void myFunctionListwidget::on_btn_COMSOLBuild_clicked()
{
    emit sigCOMSOLBuildClicked();
}

void myFunctionListwidget::on_btn_ANNPretreatment_clicked()
{
    emit sigANNPretreatmentClicked();
}

void myFunctionListwidget::on_btn_ANNTrain_clicked()
{
    emit sigANNTrainClicked();
}

void myFunctionListwidget::on_btn_ANNAutoOpt_clicked()
{
    emit sigANNAutoOptClicked();
}

void myFunctionListwidget::on_btn_PythonCPU_clicked()
{
    emit sigPythonCPUClicked();
}

void myFunctionListwidget::on_btn_PythongGPU_clicked()
{
    emit sigPythongGPUClicked();
}

void myFunctionListwidget::on_btn_MATLABTask_clicked()
{
    emit sigMATLABTaskClicked();
}

void myFunctionListwidget::on_btn_TaskMonitor_clicked()
{
    emit sigTaskMonitorClicked();
}
