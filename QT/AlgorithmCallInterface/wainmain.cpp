#include "wainmain.h"
#include "ui_wainmain.h"

wainMain::wainMain(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::wainMain)
{
    ui->setupUi(this);
}

wainMain::~wainMain()
{
    delete ui;
}

