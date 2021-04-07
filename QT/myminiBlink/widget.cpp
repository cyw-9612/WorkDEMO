#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window|Qt::MSWindowsFixedSizeDialogHint | Qt::FramelessWindowHint |Qt::WindowSystemMenuHint|Qt::WindowMinimizeButtonHint|Qt::WindowMaximizeButtonHint);

//    m_webWidget = new WebWidget("C:/Users/cyw/Desktop/QT Project/myminiBlink/htmlEcharts/line-simple.html");
//    m_webWidget->show();
    //ui->widget->layout()->addWidget(m_webWidget);
    QString url = "C:/Users/cyw/Desktop/QT Project/myminiBlink/htmlEcharts/line-log.html";
    //QString url = "http://121.196.124.117:8088/";
    //QString url = "C:/Users/cyw/Desktop/QT Project/myminiBlink/htmlEcharts/pie-legend.html";
    ui->widget->loadUrl(url);
//    while(!ui->widget->iswkeIsLoadComplete())
//    {
//        //ui->widget->onMove();
//    }
//    ui->widget->onMove();
}

Widget::~Widget()
{
    delete ui;
}

//鼠标按下事件
void Widget::mousePressEvent(QMouseEvent *event)
{
    //鼠标左键按下
    if(event->button()==Qt::LeftButton)
    {
        //顶层布局矩形区域
        //QRect rectTop=ui->groupTop->rect();
        //鼠标按下点落在顶层标题区域
        //if(rectTop.contains(event->pos()))
        {
            this->m_windowPos = this->pos();           // 目前控件在父窗口中的位置
            this->m_mousePos = event->globalPos();     // 获得鼠标位置
            this->m_dPos = m_mousePos - m_windowPos;       // 移动后部件所在的位置

            m_moveFlag=true;
        }
    }
}

//鼠标移动事件
void Widget::mouseMoveEvent(QMouseEvent *event)
{
    if(m_moveFlag)
    {
        //拖动窗体
        this->move(event->globalPos() - this->m_dPos);
        ui->widget->onMove();
    }
}

//释放鼠标
void Widget::mouseReleaseEvent(QMouseEvent *event)
{Q_UNUSED(event)
    m_moveFlag=false;
}


void Widget::on_pushButton_clicked()
{
    ui->widget->onMove();

    QJsonObject seriesData;
    QJsonArray data1 = {1, 3, 9, 27, 81, 247, 741, 2223, 6669};
    seriesData.insert("data1", data1);
    QJsonArray data2 = {1, 2, 4, 8, 16, 32, 64, 128, 256};
    seriesData.insert("data2", data2);

    QString optionStr = QJsonDocument(seriesData).toJson();
    QString js = QString("init2(%1)").arg(optionStr);
    ui->widget->runJS(js);
}

void Widget::on_pushButton_2_clicked()
{
       QJsonObject sizeData;
       sizeData.insert("width", ui->widget->width() - 20);
       sizeData.insert("height", ui->widget->height() - 20);
       QString sizeStr = QJsonDocument(sizeData).toJson();
       QString js = QString("setSize(%1)").arg(sizeStr);
       ui->widget->runJS(js);
}
