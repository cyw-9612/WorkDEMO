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
    connect(m_functionWidget, SIGNAL(sigCSTbuild()), this, SLOT(slotCSTbuild()));
    connect(m_functionWidget, SIGNAL(sigANNtrain()), this, SLOT(slotANNtrain()));
    connect(m_functionWidget, SIGNAL(sigANNused()), this, SLOT(slotANNused()));

    connect(m_taskParameterDialog, SIGNAL(sigCancal()),this, SLOT(slotANNTrainCancal()));
    connect(m_taskParameterDialog, SIGNAL(sigComfirm(QString)),this, SLOT(slotANNTrainComfirm(QString)));
}

//初始化界面
void myFunctionListwidget::initUI()
{
    m_functionWidget = new myFunctionDisplayDialog(this);
    ui->gridLayout->addWidget(m_functionWidget);

    m_taskParameterDialog = new taskChoiseDialog(this);
    ui->gridLayout->addWidget(m_taskParameterDialog);
    m_taskParameterDialog->hide();
}

//设置控件输入范围
void myFunctionListwidget::setCtrlRange()
{

}

//设置ANN参数列表信息
void myFunctionListwidget::setANNInfo(QString dataJson)
{
    m_taskParameterDialog->setANNInfo(dataJson);
}

void myFunctionListwidget::slotCSTbuild()
{
    emit sigCSTbuild();
}

void myFunctionListwidget::slotANNtrain()
{
    emit sigANNtrain();
    m_functionWidget->hide();
    m_taskParameterDialog->show();
}

void myFunctionListwidget::slotANNused()
{
    emit sigANNused();
}

void myFunctionListwidget::slotANNTrainCancal()
{
    m_functionWidget->show();
    m_taskParameterDialog->hide();
    emit sigANNTrainCancal();
}

void myFunctionListwidget::slotANNTrainComfirm(QString dataJson)
{
    m_functionWidget->show();
    m_taskParameterDialog->hide();
    emit sigANNTrainComfirm(dataJson);
}
