#include "winmain.h"
#include "ui_winmain.h"

winMain::winMain(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::winMain)
{
    ui->setupUi(this);
}

winMain::~winMain()
{
    delete ui;
}

