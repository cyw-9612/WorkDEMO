#ifndef MYTABLEWIDGET_H
#define MYTABLEWIDGET_H

#include <QDialog>
#include "LibControl_global.h"
#include "childview.h"
#include "PageWidget.h"

namespace Ui {
class myTableWidget;
}

struct UserData
{
    std::string instrumentType; //仪器型号
    std::string instrumentNum;  //仪器串号
    std::string calibrationTime; //校准时间
    std::string humidity;   //湿度
    std::string temperature; //温度
    std::string atmosphericPressure; //大气压
    std::string microphoneModel; //传声器型号
    std::string microphoneNum; //传声器串号
    std::string freeFieldCorrection; //自由场修正量
    std::string microphoneSensitivityLevel; //传声器灵敏度级别
    std::string measureRange; //测量范围
    std::string calibrationSoundLevel;  //校准器声级
    std::string calibrationFrequent; //校准器频率
};

class LIBCONTROL_EXPORT myTableWidget : public ChildView
{
    Q_OBJECT

public:
    explicit myTableWidget(QWidget *parent = nullptr);
    ~myTableWidget();

    /**
     * @brief  初始化校准数据界面UI
     * @return 无
     */
    void InitUI();

    /**
     * @brief  更新校准表格数据
     * @param  data ：一条数据结构体
     * @param row ：该条数据所在的行
     * @return 无
     */
    void UpdateRows(UserData *data, int row);

    /**
     * @brief  设置分页栏的信息
     * @param rows ：一页占有的信息条数
     * @param totalRows ：总的信息条数
     * @return 无
     */
    void SetPages(int rows, int totalRows);

    /**
     * @brief  更新表格信息
     * @param startRow ：本页开始的信息条数
     * @param endRow ：本页结束的信息条数
     * @return 无
     */
    void UpdateTable(int startRow, int endRow);

    /**
     * @brief  清理表格信息
     * @return 无
     */
    void ClearTable();

    /**
     * @brief  初始化表格数据
     * @param rows ：一页占有的信息条数
     * @param totalRows ：总的信息条数
     * @return 无
     */
    void InitTable(int rows, int totalRows);

private:   //排序比较规则
    //机号比较大小
    static bool compareBarData(const UserData *barAmount1, const UserData *barAmount2);
    //日期排序规则
    static bool compareDayData(const UserData *barAmount1, const UserData *barAmount2);
    //湿度排序
    static bool compareHumidityData(const UserData *barAmount1, const UserData *barAmount2);
    //温度排序
    static bool compareTemperatureData(const UserData *barAmount1, const UserData *barAmount2);
    //大气压排序
    static bool compareAtmosphericPressureData(const UserData *barAmount1, const UserData *barAmount2);
    //传声器型号比较大小
    static bool compareStringData(const UserData *barAmount1, const UserData *barAmount2);
    //传声器串号
    static bool compareMicrophoneNumData(const UserData *barAmount1, const UserData *barAmount2);
    //传声器灵敏度级
    static bool compareMicrophoneSensitivityLevelData(const UserData *barAmount1, const UserData *barAmount2);
    //校准器声级
    static bool compareCalibrationSoundLevelData(const UserData *barAmount1, const UserData *barAmount2);
    //校准器频率
    static bool compareCalibrationFrequentData(const UserData *barAmount1, const UserData *barAmount2);

private slots:
    /**
     * @brief  设置当前显示页
     * @param page ：页号
     * @return 无
     */
    void SlotCurrentPageChanged(int page);

    /**
     * @brief  排序
     * @param col ：需要排序的列号
     * @return 无
     */
    void sort(int col);

    /**
     * @brief  根据输入信息条件筛选数据
     * @param screenKey ：筛选信息
     * @return 无
     */
    void slotScreenData(QString screenKey);

    /**
     * @brief  根据日期条件筛选数据
     * @return 无
     */
    void slotScreenData();

private:
    Ui::myTableWidget *ui;

    //分页栏对象
    PageWidget * m_pageWidget;

    //计算总页数
    int Pages_ ;

    //总行数
    int TotalRows_ ;

    //每页行数
    int EachRows_ ;

    //数据链表
    QList<UserData*> DataList_;
};

#endif // MYTABLEWIDGET_H
