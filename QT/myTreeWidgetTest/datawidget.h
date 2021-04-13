#ifndef DATAWIDGET_H
#define DATAWIDGET_H
#include <QLabel>
#include <QString>

class dataWidget : public QWidget
{
public:
    dataWidget(QString dataName);

    //设置数据的数量
    void setDateCount(int count);

    //获取数据类的名称
    QString getDataName();

private:
    QString m_dateName;

    int m_dataCount = 0;
};

#endif // DATAWIDGET_H
