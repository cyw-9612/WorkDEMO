#include "Dialog.h"
#include <QVBoxLayout>

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
{
    m_customPlot = new XxwCustomPlot();
    //设置鼠标跟随开
    m_customPlot->showTracer(true);

    // add title layout element:
    m_customPlot->plotLayout()->insertRow(0);
    m_customPlot->plotLayout()->addElement(0, 0, new QCPTextElement(m_customPlot, "标题", QFont("黑体", 12, QFont::Bold)));

    m_customPlot->legend->setVisible(true);
    QFont legendFont = font();  // start out with MainWindow's font..
    legendFont.setPointSize(9); // and make a bit smaller for legend
    m_customPlot->legend->setFont(legendFont);
    m_customPlot->legend->setBrush(QBrush(QColor(255,255,255,230)));
    // 默认情况下，图例位于主轴矩形的插入布局中。所以这就是我们如何访问它来改变图例的位置:
    m_customPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignRight);

    // 使左轴和下轴始终将其范围转移到右轴和上轴:
    connect(m_customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), m_customPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(m_customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), m_customPlot->yAxis2, SLOT(setRange(QCPRange)));

    //允许用户使用鼠标拖动轴范围，使用鼠标滚轮缩放并通过单击选择图形：
    m_customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);

    //设置当前时间
//    QString dateTime = QDateTime::currentDateTime().toString("-MM-dd hh:mm:ss");
//    QDateTime dateTimex = QDateTime::fromString("2021" + dateTime,"yyyy-MM-dd hh:mm:ss");
    QDateTime dateTime = QDateTime::currentDateTime();
    double  now = dateTime.toTime_t();//当前时间转化为秒

    // 制造数据点
    int nCount = 7200;
    QTime time;
    time= QTime::currentTime();
    qsrand(time.msec()+time.second()*1000);

    QVector<double> x(nCount), y0(nCount), y1(nCount); // 用条目初始化 0..5000
    for (int i = 0; i < nCount; ++i)
    {
        x[i] =now + i; // x goes from -1 to 1
        //qDebug() << qrand() % 1000;
//        y0[i] = qrand() % 100;
//        y1[i] = qrand() % 100;
        y0[i] = (nCount - i) * qSin(i * (15*3.14f) / nCount); //sin
        y1[i] = (nCount - i) * qCos(i * (15*3.14f) / nCount); //cos
    }

    // 图例
    QPen pen;
    int i = 1;
    QCPGraph *pGraph = m_customPlot->addGraph();
    //        m_customPlot->graph(0)->setData(x, y0);
    pGraph->setName("sin曲线");
    pGraph->setData(x,y0);
    pGraph->setPen(QPen(Qt::blue));

    pGraph = m_customPlot->addGraph();
    //        m_customPlot->graph(0)->setData(x, y0);
    pGraph->setName("cos曲线");
    pGraph->setData(x,y1);
    pGraph->setPen(QPen(Qt::red));

    // 设置坐标轴名称:
    m_customPlot->xAxis->setLabel("时间轴");
    m_customPlot->yAxis->setLabel("数据");

    //QCPAxisTickerDateTime 时间坐标轴 必须要用智能指针
    QSharedPointer<QCPAxisTickerDateTime> timer(new QCPAxisTickerDateTime);

   //设置时间格式
   timer->setDateTimeFormat("yy-M-d hh:mm:ss");

   //设置时间轴 一共几格
   timer->setTickCount(12);

   //设置label 旋转35° 横着显示可能显示不全
   m_customPlot->xAxis->setTickLabelRotation(35);

   timer->setTickStepStrategy(QCPAxisTicker::tssMeetTickCount);
   m_customPlot->xAxis->setSubTicks(false);
   //m_customPlot->xAxis->setRange(now, now+3600*2);

   //设置坐标轴
   m_customPlot->xAxis->setTicker(timer);

    // set axes ranges, so we see all data:
//    m_customPlot->xAxis->setRange(-1, 1);
//    m_customPlot->yAxis->setRange(0, 1);
    m_customPlot->rescaleAxes(true);

    m_customPlot->replot();

//    m_customPlot->setInteraction(QCP::iRangeDrag, true);
//    m_customPlot->setInteraction(QCP::iRangeZoom, true);

//    QDateTime dateTime = QDateTime::currentDateTime();
//    double  now = dateTime.toTime_t();//当前时间转化为秒

//    //生成时间刻度对象
//    QSharedPointer<QCPAxisTickerDateTime> dateTimeTicker(new QCPAxisTickerDateTime);
//    m_customPlot->xAxis->setTicker(dateTimeTicker);
//    dateTimeTicker->setTickCount(12);
//    dateTimeTicker->setTickStepStrategy(QCPAxisTicker::tssMeetTickCount);
//    m_customPlot->xAxis->setSubTicks(false);
//    m_customPlot->xAxis->setRange(now, now+3600*24);//x轴范围，从当前时间起往后推24小时
//    QVector<double> yData, xData;//生成数据
//    for (int i = 0; i <= 24; i++)
//    {
//        xData.push_back(now + i * 3600.0);
//        yData.push_back(pow(double(i), 2) + 550.0*sin(double(i)/4.0));
//    }
//    dateTimeTicker->setDateTimeFormat("yyyy-M-d h:m");//设置x轴刻度显示格式
//    m_customPlot->xAxis->setTickLabelRotation(30);//设置刻度标签顺时针旋转30度
//    m_customPlot->yAxis->setRange(-1000,10000);
//    m_customPlot->addGraph();
//    m_customPlot->graph(0)->setData(xData, yData);//显示数据

    QVBoxLayout *pLayout = new QVBoxLayout;
    pLayout->addWidget(m_customPlot);
    setLayout(pLayout);
    showMaximized();
}

Dialog::~Dialog()
{

}
