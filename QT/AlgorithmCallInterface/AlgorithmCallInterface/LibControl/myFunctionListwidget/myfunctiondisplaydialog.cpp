#include "myfunctiondisplaydialog.h"
#include "ui_myfunctiondisplaydialog.h"

myFunctionDisplayDialog::myFunctionDisplayDialog(QWidget *parent) :
    ChildView(parent),
    ui(new Ui::myFunctionDisplayDialog)
{
    ui->setupUi(this);

    initUI();

    setStyle();

    setCtrlRange();

    connectSlots();

}

myFunctionDisplayDialog::~myFunctionDisplayDialog()
{
    delete ui;
}

//设置风格
void myFunctionDisplayDialog::setStyle()
{

}

//关联信号槽
void myFunctionDisplayDialog::connectSlots()
{

}

//初始化界面
void myFunctionDisplayDialog::initUI()
{

}

//设置控件输入范围
void myFunctionDisplayDialog::setCtrlRange()
{

}

void myFunctionDisplayDialog::on_btn_CSTbuild_clicked()
{
    emit sigCSTbuild();
}

void myFunctionDisplayDialog::on_btn_ANNtrain_clicked()
{
    emit sigANNtrain();
}

void myFunctionDisplayDialog::on_btn_ANNused_clicked()
{
    emit  sigANNused();
}
