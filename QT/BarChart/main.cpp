#include "mainwindow.h"
#include <QApplication>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLegend>
#include <QtCharts/QBarCategoryAxis>

QT_CHARTS_USE_NAMESPACE

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    //创建线条
    QBarSet *set0 = new QBarSet("华为");
    QBarSet *set1 = new QBarSet("苹果");
    QBarSet *set2 = new QBarSet("三星");
    QBarSet *set3 = new QBarSet("小米");
    QBarSet *set4 = new QBarSet("OPPO");
    set0->setBorderColor(QColor(255,128,255));//没什么效果
    set0->setColor(QColor(255,0,255));//没什么效果
    //set0->setLabel("F");//F等效于Jane
    set0->setLabelColor(QColor(0,255,0));//没什么效果
    //set0->setLabelFont(QFont("微软雅黑"));//字体变大

    *set0 << 5420 << 3850 << 2789 << 2543 << 2167 << 1568;
    *set1 << 4130 << 4100 << 4578 << 3789 << 3256 << 2985;
    *set2 << 7150 << 7980 << 7345 << 6865 << 5784 << 4657;
    *set3 << 3190 << 2140 << 2865 << 2045 << 1836 << 1268;
    *set4 << 1137 << 2800 << 2645 << 1926 << 1369 << 1280;

    QBarSeries *series = new QBarSeries();
    series->append(set0);
    series->append(set1);
    series->append(set2);
    series->append(set3);
    series->append(set4);

    series->setVisible(true);//设置条形图是否可视化
    series->setBarWidth(0.8);//设置Bar宽
    series->setLabelsVisible(true);//设置标签是否可视化
    series->setLabelsAngle(30);//设置标签的角度
    series->setLabelsFormat("@value u");//设置标签显示的格式
    series->setLabelsPosition(QAbstractBarSeries::LabelsOutsideEnd);//设置标签的位置
    series->setLabelsPrecision(5);//标签中显示的最大有效位数
    //series->setName("BLUE");//当创建多个Bar时才有效


    QChart *chart = new QChart();
    chart->setTitle("全球手机品牌销量");
    chart->addSeries(series);
    chart->setAnimationOptions(QChart::SeriesAnimations);
    //chart->setTheme(QChart::ChartThemeBlueCerulean);//设置系统主题
    chart->setAnimationOptions(QChart::AllAnimations);//设置启用或禁用动画
    //chart->setBackgroundBrush(QBrush(QColor(170,170,255)));//设置背景色,主题和背景二选一
    //chart->setDropShadowEnabled(true);//是否背景阴影
    chart->setLocalizeNumbers(true);//数字是否本地化
    //chart->legend()->show();//legend是否显示，show和hide
    chart->createDefaultAxes();//创建默认轴
    chart->setTitleBrush(QBrush(QColor(255,170,255)));//设置标题Brush
    chart->setTitleFont(QFont("微软雅黑"));//设置标题字体

    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignTop);//对齐
    //chart->legend()->detachFromChart();
    //chart->legend()->setBackgroundVisible(true);//设置背景是否可视
    //chart->legend()->setAutoFillBackground(true);//设置背景自动填充
    //chart->legend()->setColor(QColor(255,128,128,128));//设置颜色
    //chart->legend()->setContentsMargins(10,10,10,10);//设置边距left,top,right,bottom
    chart->legend()->setLabelColor(QColor(255,128,255));//设置标签颜色
    chart->legend()->setVisible(true);//设置是否可视
    //chart->legend()->setMaximumHeight(50);
    //chart->legend()->setMaximumWidth(120);
    //chart->legend()->setMaximumSize(10000);
    //chart->legend()->setGeometry(50,50,120,50);//设置几何位置x,y,w,h
    //chart->legend()->setBrush(QBrush(QColor(128,128,128,128)));
    //chart->legend()->setPen(QPen(QColor(192,192, 192,192)));
    chart->legend()->setBorderColor(QColor(255,255,170,185));//设置边框颜色
    QFont font = chart->legend()->font();
    font.setItalic(!font.italic());
    chart->legend()->setFont(font);//设置字体为斜体
    font.setPointSizeF(12);
    chart->legend()->setFont(font);//设置字体大小
    chart->legend()->setFont(QFont("微软雅黑"));//设置字体类型

    QStringList categories;
    categories << "2018" << "2017" << "2016" << "2015" << "2014" << "2013";
    QBarCategoryAxis *axis = new QBarCategoryAxis();
    axis->append(categories);
    chart->setAxisX(axis, series);//设置X轴
    /*X轴*/
    chart->axisX(series)->setVisible(true);
    chart->axisX(series)->setRange(1000,10000);//设置Y轴范围
    //X轴标题
    chart->axisX(series)->setTitleText("年份（年）");//设置Y轴标题
    chart->axisX(series)->setTitleBrush(QBrush(QColor(255,170,255)));
    chart->axisX(series)->setTitleFont(QFont("微软雅黑"));
    chart->axisX(series)->setTitleVisible(true);
    //X轴网格
    chart->axisX(series)->setGridLineColor(QColor(50,255,50));//设置Y轴网格线颜色
    chart->axisX(series)->setGridLineVisible(true);
    //chart->axisY(series)->setGridLinePen(QPen(QColor(50,255,50)));
    //X轴标签
    chart->axisX(series)->setLabelsAngle(30);
    //chart->axisY(series)->setLabelsBrush(QBrush(QColor(50,255,50)));
    chart->axisX(series)->setLabelsColor(QColor(255,170,255));
    chart->axisX(series)->setLabelsFont(QFont("Arial"));
    chart->axisX(series)->setLabelsVisible(true);
    //X轴轴线
    //chart->axisY(series)->setLinePen(QPen(QColor(50,255,50)));
    chart->axisX(series)->setLinePenColor(QColor(255,170,255));
    chart->axisX(series)->setLineVisible(true);
    chart->axisX(series)->setMax(90);
    chart->axisX(series)->setMin(10);
    //X轴子网格线
    chart->axisX(series)->setMinorGridLineColor(QColor(255,255,255));
    //chart->axisY(series)->setMinorGridLinePen(QPen(QColor(50,255,50)));
    chart->axisX(series)->setMinorGridLineVisible(true);

    chart->axisX(series)->setReverse(true);//Y轴值反向
    //X轴阴影
    //chart->axisX(series)->setShadesVisible(true);//默认false
    //chart->axisX(series)->setShadesBorderColor(QColor(0,255,0));
    //chart->axisX(series)->setShadesColor(QColor(0,0,0));
    //chart->axisY(series)->setShadesBrush(QColor(0,0,0));
    //chart->axisY(series)->setShadesPen(QColor(0,0,0));

    /*Y轴*/
    chart->axisY(series)->setVisible(true);
    chart->axisY(series)->setRange(1000,10000);//设置Y轴范围
    //Y轴标题
    chart->axisY(series)->setTitleText("手机销量（部）");//设置Y轴标题
    chart->axisY(series)->setTitleBrush(QBrush(QColor(255,170,255)));
    chart->axisY(series)->setTitleFont(QFont("微软雅黑"));
    chart->axisY(series)->setTitleVisible(true);
    //Y轴网格
    chart->axisY(series)->setGridLineColor(QColor(50,255,50));//设置Y轴网格线颜色
    chart->axisY(series)->setGridLineVisible(true);
    //chart->axisY(series)->setGridLinePen(QPen(QColor(50,255,50)));
    //Y轴标签
    chart->axisY(series)->setLabelsAngle(30);
    //chart->axisY(series)->setLabelsBrush(QBrush(QColor(50,255,50)));
    chart->axisY(series)->setLabelsColor(QColor(255,170,255));
    chart->axisY(series)->setLabelsFont(QFont("Arial"));
    chart->axisY(series)->setLabelsVisible(true);
    //Y轴轴线
    //chart->axisY(series)->setLinePen(QPen(QColor(50,255,50)));
    chart->axisY(series)->setLinePenColor(QColor(255,170,255));
    chart->axisY(series)->setLineVisible(true);
    //最大最小值相当于设置范围
    //chart->axisY(series)->setMax(90);
    //chart->axisY(series)->setMin(10);
    //Y轴子网格线
    chart->axisY(series)->setMinorGridLineColor(QColor(255,255,255));
    //chart->axisY(series)->setMinorGridLinePen(QPen(QColor(50,255,50)));
    chart->axisY(series)->setMinorGridLineVisible(true);

    //chart->axisY(series)->setReverse(true);//Y轴值反向
    //Y轴阴影
    //chart->axisY(series)->setShadesVisible(true);//默认false
    //chart->axisY(series)->setShadesBorderColor(QColor(0,255,0));
    //chart->axisY(series)->setShadesColor(QColor(0,0,0));
    //chart->axisY(series)->setShadesBrush(QColor(0,0,0));
    //chart->axisY(series)->setShadesPen(QColor(0,0,0));




    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    w.setCentralWidget(chartView);
    w.resize(960, 720);
    w.show();

    return a.exec();
}
