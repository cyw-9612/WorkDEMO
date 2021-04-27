#include "datahistogram.h"
#include "ui_datahistogram.h"

dataHistogram::dataHistogram(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::dataHistogram)
{
    ui->setupUi(this);
    for(int i = 1; i<=27 ; i++)
    {
        ticks << i; //初始化要显示的刻度
    }

    //设置柱状图颜色
    m_listColor << QColor(210,90,90) << QColor (255, 127, 80) << QColor (30, 144, 255)
                         << QColor (218, 165, 32) << QColor (255, 0, 255) << QColor (147, 112, 219);

    labels <<QObject::tr("LAFmax  ")
          <<QObject::tr("LCFmax  ")
          <<QObject::tr("LZFmax  ")
          <<QObject::tr("LAFmin  ")
          <<QObject::tr("LCFmin  ")
          <<QObject::tr("LZFmin  ")
          <<QObject::tr("LASmax  ")
          <<QObject::tr("LCSmax  ")
          <<QObject::tr("LZSmax  ")
          <<QObject::tr("LASmin  ")
          <<QObject::tr("LCSmin  ")
          <<QObject::tr("LZSmin  ")
          <<QObject::tr("LAImax  ")
          <<QObject::tr("LCImax  ")
          <<QObject::tr("LZImax  ")
          <<QObject::tr("LAImin  ")
          <<QObject::tr("LCImin  ")
          <<QObject::tr("LZImin  ")
          <<QObject::tr("LAeqT  ")
          <<QObject::tr("LCeqT  ")
          <<QObject::tr("LZeqT  ")
          <<QObject::tr("LApeek  ")
          <<QObject::tr("LCpeek  ")
          <<QObject::tr("LZpeek  ")
          <<QObject::tr("SEL_A  ")
          <<QObject::tr("SEL_C  ")
          <<QObject::tr("SEL_Z  ");

    //设置X轴参数
    QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
    textTicker->addTicks(ticks, labels);
    ui->Plot->xAxis->setTicker(textTicker);
    ui->Plot->xAxis->setTickLabelRotation(60);//旋转角度
    ui->Plot->xAxis->setSubTicks(false);
    ui->Plot->xAxis->setTickLength(0, 4);
    ui->Plot->xAxis->setRange(0, 10);
    ui->Plot->xAxis->setBasePen(QPen(Qt::black));
    ui->Plot->xAxis->setTickPen(QPen(Qt::black));
    ui->Plot->xAxis->grid()->setVisible(false);
//    ui->Plot->xAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));
    ui->Plot->xAxis->setTickLabelColor(Qt::black);
    ui->Plot->xAxis->setLabelColor(Qt::black);
    ui->Plot->xAxis->setTickLength(0,0);//设置刻度长度，第一个是向内的长度，第二个是向外的长度
    ui->Plot->xAxis->setRange(0,28); //设置坐标轴范围

    //设置Y轴参数
    ui->Plot->yAxis->setNumberFormat("f");
    ui->Plot->yAxis->setNumberPrecision(1);//坐标轴 数据保留1位小数
    ui->Plot->yAxis->setPadding(5); //给左侧边框留距离
    ui->Plot->yAxis->setLabel("(dB)");
    QFont ft;
    ft.setBold(true);
    ft.setPointSize(12);
    ui->Plot->yAxis->setLabelFont(ft);
    ui->Plot->yAxis->setBasePen(QPen(Qt::black));
    ui->Plot->yAxis->setTickPen(QPen(Qt::black));
    ui->Plot->yAxis->setSubTickPen(QPen(Qt::black));
    ui->Plot->yAxis->grid()->setSubGridVisible(false);
    ui->Plot->yAxis->setTickLabelColor(Qt::black);
    ui->Plot->yAxis->setLabelColor(Qt::black);
//    ui->Plot->yAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::SolidLine)); //设置 格子的颜色
//    ui->Plot->yAxis->grid()->setSubGridPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));
    ui->Plot->yAxis->ticker()->setTickCount(8);//8个主刻度
    ui->Plot->yAxis->setSubTickLength(0,2);
    ui->Plot->yAxis->setTickLength(0,5);
    ui->Plot->yAxis->setRange(0,140);

    //实例化QCPBars 先实例的摆在后面
    QCPBars* m_Bar = new QCPBars(ui->Plot->xAxis,ui->Plot->yAxis);
    m_listBar << m_Bar;
    m_mousebar = new QCPBars(ui->Plot->xAxis,ui->Plot->yAxis);
    m_listBar << m_mousebar;
    qDebug() << m_listBar.count();
    m_mousebar->setAntialiased(false); //提供更清晰、像素对齐的条形图边框
    m_Bar->setAntialiased(false);
    m_mousebar->setStackingGap(1);
    m_Bar->setStackingGap(1);
    m_mousebar->setName("testData");
    m_Bar->setName("museTrack");
    m_mousebar->setPen(QPen(QColor(1,140,145).lighter(150)));
    m_Bar->setPen(QPen(m_listColor[0].lighter(150)));
    m_mousebar->setBrush(QColor(0,0,0,0));
    m_Bar->setBrush(m_listColor[0]);

    //准备数据
    //QVector<double> v_index;
    QVector<double> v_data;

    //QVector<QColor> v_color;

    float mm = 10.0;
    for(int i=0;i<27;i++)
    {
        //v_index<<i+1;
        v_data<<  75 + 32* qSin(i * (15*3.14f) / mm);
        v_Mousedata << 0;
        //v_color<<Qt::green;
    }

    //填充数据
    m_mousebar->setData(ticks,v_Mousedata);
    m_Bar->setData(ticks,v_data);

    QCPGraph *pGraph = ui->Plot->addGraph();
    //        m_customPlot->graph(0)->setData(x, y0);
    pGraph->setName("数据曲线");
    pGraph->setData(ticks,v_data);
    pGraph->setPen(QPen(QColor(255,0,0,0))); //设置折线为透明

    //设置消息提示框
    QList<QString> dataNameList;
    dataNameList << "L5";
    tip=new myToolTip(m_listColor,dataNameList);
    tip->anchorTarget(ui->Plot);

    connect(ui->Plot,SIGNAL(mouseChange(int, float)), this, SLOT(slotMouseChange(int, float)));
}

dataHistogram::~dataHistogram()
{
    if(tip != nullptr)
    {
        delete tip;
        tip = nullptr;
    }

    delete ui;
}

void dataHistogram::slotMouseChange(int x_val, float y_val)
{
    //qDebug() << x_val << y_val;
    if(0 > x_val || 26 < x_val)
    {
        return;
    }

    v_Mousedata.clear();
    for(int i = 0; i < 27 ; i++)
    {
        v_Mousedata << 0;
    }

    QList<QString> dataName;
    QList<float> data;
    dataName << labels[x_val] ;
    data << y_val;
    tip->setTextList(dataName,data);
//    QString strToolTip;
//    strToolTip += "详情：";
//    strToolTip += "\n";
//    strToolTip += labels[x_val];
//    strToolTip += QString::number(y_val);
//    strToolTip += "dB";

//    QToolTip::showText(cursor().pos(), strToolTip, ui->Plot);

    v_Mousedata[x_val] = y_val;
    m_mousebar->setData(ticks,v_Mousedata);
}
