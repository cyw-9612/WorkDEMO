#include "mytablewidget.h"
#include "ui_mytablewidget.h"
#include <QDebug>

myTableWidget::myTableWidget(QWidget *parent) :
    ChildView(parent),
    ui(new Ui::myTableWidget)
{
    ui->setupUi(this);

    InitUI();

    //连接水平表头响应
    connect(ui->tableWidget->horizontalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(sort(int)));

    //连接搜索按钮和搜索函数
    connect(ui->btn_screen, SIGNAL(clicked()), this, SLOT(slotScreenData()));
}

myTableWidget::~myTableWidget()
{
    if(m_pageWidget != nullptr)
    {
        delete m_pageWidget;
        m_pageWidget = nullptr;
    }

    delete ui;

    qDebug() << "~myTableWidget";
}

void myTableWidget::InitUI()
{
    QStringList headers;
        headers<<QObject::tr("仪器型号")
                    <<QObject::tr("仪器串号")
                    <<QObject::tr("校准时间")
                    <<QObject::tr("湿度/%rh")
                    <<QObject::tr("温度/°C")
                    <<QObject::tr("大气压/kPa")
                    <<QObject::tr("传声器型号")
                    <<QObject::tr("传声器串号")
                    <<QObject::tr("自由场修正量/kPa")
                    <<QObject::tr("传声器灵敏度级别")
                    <<QObject::tr("测量范围/dB")
                    <<QObject::tr("校准器声级/dB")
                    <<QObject::tr("校准器频率/HZ");

        QString tableWidgetStyle = "QTableWidget#tableWidget{\
            background-color: rgb(255, 255, 255); \
            border:1px solid #EEF1F7;           \
            font-size:12px;\
            color:#666666; }\
        QTableWidget::item {\
            border-left:none ; \
            border-top:1px solid #dddddd ; \
            border-right:none  ;\
           border-bottom:1px solid #dddddd ;}\
        QTableView::item:selected{\
           color:red;}\
        QHeaderView::section:first{\
            border-image: url(:/images/expand/tempUser_headBg2.png);\
            height: 30px;\
            font: 11pt ;}\
        QHeaderView::section:middle{\
            border-image: url(:/images/expand/tempUser_headBg2.png);\
            height: 30px;\
            font: 11pt;}\
        QHeaderView::section:last{\
            border-image: url(:/images/expand/tempUser_headBg4.png);\
            height: 30px;\
            font: 11pt ;}\
        QScrollBar::handle{\
            background: rgba(255,255,255,20%);\
            border: 0px solid grey;  \
            border-radius:3px; \
            width: 8px;}";

        ui->tableWidget->setColumnCount(headers.size());//列数量
        ui->tableWidget->setHorizontalHeaderLabels(headers);//列名称
        ui->tableWidget->setAlternatingRowColors(true); // 隔行变色
        ui->tableWidget->setPalette(QPalette(QColor(235,235,235))); // 设置隔行变色的颜色  gray灰色
        ui->tableWidget->horizontalHeader()->setDefaultAlignment (Qt::AlignLeft | Qt::AlignVCenter); //表头居左
        ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);//单元格不可编辑
        //ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);//自动列宽
        ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);//表列随着表格变化而自适应变化
        ui->tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);//表行随着表格变化而自适应变化
        ui->tableWidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);//然后设置要根据内容使用宽度的列
        ui->tableWidget->horizontalHeader()->setSectionResizeMode(10, QHeaderView::ResizeToContents);//然后设置要根据内容使用宽度的列
        ui->tableWidget->installEventFilter(this);
        ui->tableWidget->verticalHeader()->setVisible(false);//隐藏列表头
        ui->tableWidget->setShowGrid(false);//隐藏表格线
        ui->tableWidget->setStyleSheet(tableWidgetStyle);

        m_pageWidget = new PageWidget();
            connect(m_pageWidget, &PageWidget::currentPageChanged,
                    this, &myTableWidget::SlotCurrentPageChanged);
        ui->gridLayout->addWidget(m_pageWidget);

        InitTable(10,1000);
}

void myTableWidget::UpdateRows(UserData *data, int row)
{
    if(!data || row < 0)
        return;
    int colindex = 0;

    this->ui->tableWidget->setItem(row, colindex++,
                                   new QTableWidgetItem(QString::fromStdString(data->instrumentType)));
    this->ui->tableWidget->setItem(row, colindex++,
                                   new QTableWidgetItem(QString::fromStdString(data->instrumentNum)));
    this->ui->tableWidget->setItem(row, colindex++,
                                   new QTableWidgetItem(QString::fromStdString(data->calibrationTime)));
    this->ui->tableWidget->setItem(row, colindex++,
                                   new QTableWidgetItem(QString::fromStdString(data->humidity)));
    this->ui->tableWidget->setItem(row, colindex++,
                                   new QTableWidgetItem(QString::fromStdString(data->temperature)));
    this->ui->tableWidget->setItem(row, colindex++,
                                   new QTableWidgetItem(QString::fromStdString(data->atmosphericPressure)));
    this->ui->tableWidget->setItem(row, colindex++,
                                   new QTableWidgetItem(QString::fromStdString(data->microphoneModel)));
    this->ui->tableWidget->setItem(row, colindex++,
                                   new QTableWidgetItem(QString::fromStdString(data->microphoneNum)));
    this->ui->tableWidget->setItem(row, colindex++,
                                   new QTableWidgetItem(QString::fromStdString(data->freeFieldCorrection)));
    this->ui->tableWidget->setItem(row, colindex++,
                                   new QTableWidgetItem(QString::fromStdString(data->microphoneSensitivityLevel)));
    this->ui->tableWidget->setItem(row, colindex++,
                                   new QTableWidgetItem(QString::fromStdString(data->measureRange)));
    this->ui->tableWidget->setItem(row, colindex++,
                                   new QTableWidgetItem(QString::fromStdString(data->calibrationSoundLevel)));
    this->ui->tableWidget->setItem(row, colindex++,
                                   new QTableWidgetItem(QString::fromStdString(data->calibrationFrequent)));
}

//设置分页栏信息
void myTableWidget::SetPages(int rows, int totalRows)
{
    if(rows < 0 || totalRows < 0)
        return;
    int offset = (totalRows % rows) > 0 ? 1 : 0;
    //计算总页数
    Pages_ = totalRows / rows + offset;
    //总行数
    TotalRows_ = totalRows;
    //每页行数
    EachRows_ = rows;
    //设置分页栏最大页面数
    m_pageWidget->setMaxPage(Pages_);
}

void myTableWidget::SlotCurrentPageChanged(int page)
{
    //根据当前页决定要显示数据范围
    int rowstart = (page - 1) * EachRows_;
    int rowend = page * EachRows_ - 1;
    //根据起点和终点行更新表格页面
    UpdateTable(rowstart, rowend);
    m_pageWidget->setCurrentPage(page,true);
}

void myTableWidget::UpdateTable(int startRow, int endRow)
{
    //必须先清空表格显示内容
    this->ClearTable();
    int row = 0;
    for(int i = startRow; i < this->DataList_.size(); ++i)
    {
        if(i > endRow)//超出范围
            break;
        UserData *data = this->DataList_.at(i);
        if(data)
        {
            this->ui->tableWidget->insertRow(row);
            this->UpdateRows(data, row);
            row++;
        }
    }
}

void myTableWidget::ClearTable()
{
    //清空表格数据
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->clearContents();
}

//初始化表格数据
void myTableWidget::InitTable(int rows, int totalRows)
{
    if(rows < 0 || totalRows < 0)
        return;
    //根据条件创建对应的表格数据
    int show = 1,count = 0;
    std::string tmpstr = std::to_string(show);;

    for(int i = 0; i < totalRows; ++i)
    {
        if(count == rows)
        {
            tmpstr = std::to_string(++show);
            count = 0;
        }
        UserData *data = new UserData();
        data->instrumentType = "AWA6292"; //仪器型号
        data->instrumentNum = tmpstr;  //仪器串号
        QString day = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
        data->calibrationTime = day.toStdString(); //校准时间
        data->humidity = "-";   //湿度
        data->temperature = "-"; //温度
        data->atmosphericPressure = tmpstr; //大气压
        data->microphoneModel = tmpstr; //传声器型号
        data->microphoneNum = tmpstr; //传声器串号
        data->freeFieldCorrection = tmpstr; //自由场修正量
        data->microphoneSensitivityLevel = tmpstr; //传声器灵敏度级别
        data->measureRange = tmpstr; //测量范围
        data->calibrationSoundLevel = tmpstr;  //校准器声级
        data->calibrationFrequent = tmpstr; //校准器频率
        DataList_.push_back(data);
        count ++;
    }
    //设置分页栏页面数据
    SetPages(rows, totalRows);
    SlotCurrentPageChanged(1);
    m_pageWidget->setTotalCount(totalRows);
}

void myTableWidget::sort(int col)
{
    //表内排序
    //ui->tableWidget->sortItems(col, Qt::AscendingOrder);

    QString headName = ui->tableWidget->horizontalHeaderItem(col)->text();
    qDebug() << "选择排序类为：" << headName;
    // 对文件按时间排序
//    qSort(DataList_.begin(), DataList_.end(),[](const UserData &infoA,const UserData &infoB){
//                  return infoA.instrumentNum > infoB.instrumentNum;});
    switch (col)
    {
    case 1:
        //机号
        qSort(DataList_.begin(), DataList_.end(),compareBarData);
    break;
    case 2:
        //日期
        qSort(DataList_.begin(), DataList_.end(),compareDayData);
    break;
    case 3:
        //湿度
        qSort(DataList_.begin(), DataList_.end(),compareHumidityData);
    break;
    case 4:
        //温度
        qSort(DataList_.begin(), DataList_.end(),compareTemperatureData);
    break;
    case 5:
        //大气压
        qSort(DataList_.begin(), DataList_.end(),compareAtmosphericPressureData);
    break;
    case 6:
        //传声器型号
        qSort(DataList_.begin(), DataList_.end(),compareStringData);
    break;
    case 7:
        //传声器串号
        qSort(DataList_.begin(), DataList_.end(),compareMicrophoneNumData);
    break;
    case 9:
        //传声器灵敏度级
        qSort(DataList_.begin(), DataList_.end(),compareMicrophoneSensitivityLevelData);
    break;
    case 11:
        //校准器声级
        qSort(DataList_.begin(), DataList_.end(),compareCalibrationSoundLevelData);
    break;
    case 12:
        //校准器频率
        qSort(DataList_.begin(), DataList_.end(),compareCalibrationFrequentData);
    break;
    default:
        qDebug() << headName << "不可排序";
    }

    //排序完成回到第一页
    SlotCurrentPageChanged(1);

}

//机号排列判断
bool myTableWidget::compareBarData(const UserData *barAmount1, const UserData *barAmount2)
{
    int a = QString::fromStdString(barAmount1->instrumentNum).toInt();
    int b = QString::fromStdString(barAmount2->instrumentNum).toInt();
    if (a > b)
    {
        return true;
    }
  return false;
}

//传声器型号排列判断
bool myTableWidget::compareStringData(const UserData *barAmount1, const UserData *barAmount2)
{
    QString a = QString::fromStdString(barAmount1->microphoneModel);
    QString b = QString::fromStdString(barAmount2->microphoneModel);
    if (a > b)
    {
        return true;
    }
  return false;
}

//比较当前日期数据
bool myTableWidget::compareDayData(const UserData *barAmount1, const UserData *barAmount2)
{
    QDateTime day1 = QDateTime::fromString(QString::fromStdString(barAmount1->calibrationTime), "yyyy-MM-dd hh:mm:ss");
    QDateTime day2 = QDateTime::fromString(QString::fromStdString(barAmount2->calibrationTime), "yyyy-MM-dd hh:mm:ss");
    if(day1 > day2)
    {
        return true;
    }
    return false;
}

//比较湿度
bool myTableWidget::compareHumidityData(const UserData *barAmount1, const UserData *barAmount2)
{
    int a = QString::fromStdString(barAmount1->humidity).toInt();
    int b = QString::fromStdString(barAmount2->humidity).toInt();
    if (a > b)
    {
        return true;
    }
  return false;
}

//比较大气压
bool myTableWidget::compareAtmosphericPressureData(const UserData *barAmount1, const UserData *barAmount2)
{
    float a = QString::fromStdString(barAmount1->atmosphericPressure).toFloat();
    float b = QString::fromStdString(barAmount2->atmosphericPressure).toFloat();
    if (a > b)
    {
        return true;
    }
  return false;
}

//比较温度
bool myTableWidget::compareTemperatureData(const UserData *barAmount1, const UserData *barAmount2)
{
    float a = QString::fromStdString(barAmount1->temperature).toFloat();
    float b = QString::fromStdString(barAmount2->temperature).toFloat();
    if (a > b)
    {
        return true;
    }
  return false;
}

//比较传声器串号
bool myTableWidget::compareMicrophoneNumData(const UserData *barAmount1, const UserData *barAmount2)
{
    int a = QString::fromStdString(barAmount1->microphoneNum).toInt();
    int b = QString::fromStdString(barAmount2->microphoneNum).toInt();
    if (a > b)
    {
        return true;
    }
  return false;
}

//传声器灵敏度级
bool myTableWidget::compareMicrophoneSensitivityLevelData(const UserData *barAmount1, const UserData *barAmount2)
{
    float a = QString::fromStdString(barAmount1->microphoneSensitivityLevel).toFloat();
    float b = QString::fromStdString(barAmount2->microphoneSensitivityLevel).toFloat();
    if (a > b)
    {
        return true;
    }
  return false;
}

//校准器声级
bool myTableWidget::compareCalibrationSoundLevelData(const UserData *barAmount1, const UserData *barAmount2)
{
    int a = QString::fromStdString(barAmount1->calibrationSoundLevel).toInt();
    int b = QString::fromStdString(barAmount2->calibrationSoundLevel).toInt();
    if (a > b)
    {
        return true;
    }
  return false;
}

//校准器频率
bool myTableWidget::compareCalibrationFrequentData(const UserData *barAmount1, const UserData *barAmount2)
{
    int a = QString::fromStdString(barAmount1->calibrationFrequent).toInt();
    int b = QString::fromStdString(barAmount2->calibrationFrequent).toInt();
    if (a > b)
    {
        return true;
    }
  return false;
}

//筛选日期数据 等建立好数据库改用数据库筛选数据
void myTableWidget::slotScreenData()
{
    QDateTime beginTime = ui->dateTimeEdit_begin->dateTime();
    QDateTime endTime = ui->dateTimeEdit_end->dateTime();
    if(beginTime > endTime)
    {
        QDateTime a = beginTime;
        beginTime = endTime;
        endTime = a;
    }
    QList<UserData*> screanDataList;
    for (int i = 0; i < DataList_.length(); i ++ )
    {
        UserData *date = DataList_.at(i);
        QDateTime day = QDateTime::fromString(QString::fromStdString(date->calibrationTime), "yyyy-MM-dd hh:mm:ss");
        if(day >= beginTime && day<= endTime)
        {
            screanDataList.push_back(date);
        }

    }

//    DataList_ = screanDataList;
//    //设置分页栏页面数据
//    SetPages(6, DataList_.length());
//     SlotCurrentPageChanged(1);

}

//筛选数据
void myTableWidget::slotScreenData(QString screenKey)
{
    qDebug() << "筛选条件为：" << screenKey;
}
