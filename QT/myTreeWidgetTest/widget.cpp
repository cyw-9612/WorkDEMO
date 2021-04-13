#include "widget.h"
#include "ui_widget.h"
#include "mytreewidget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    myTreeWidget *treeWidget = new myTreeWidget();

    ui->horizontalLayout->addWidget(treeWidget);

}

Widget::~Widget()
{
    delete ui;
}

