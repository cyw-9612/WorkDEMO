#ifndef MYTOOLTIP_H
#define MYTOOLTIP_H

#include <QDialog>
#include <QLabel>
#include <QBasicTimer>

namespace Ui {
class myToolTip;
}

/***********************************
 *
 * 功 能: 自定义消息框提示类
 *
 * 编 制: 陈宇葳
 *
 * 日 期: 2020/04/27
 *
 **********************************/

class myToolTip : public QDialog
{
    Q_OBJECT
//    //qss右移1px:qproperty-rightOffset:1;
//    Q_PROPERTY(int rightOffset READ getRightOffset WRITE setRightOffset)
//    //qss上移1px:qproperty-bottomOffset:"-1";
//    Q_PROPERTY(int bottomOffset READ getBottomOffset WRITE setBottomOffset)

public:
    /**
     * @brief 自定义消息提示框
     * @param colorList:颜色图标列表
     * @param dataNameList:数据名称列表
     * @note
     */
    explicit myToolTip(const QList<QColor> colorList, const QList<QString>dataNameList);
    ~myToolTip();

    //初始化界面
    void IniUI();

    //设置信号
    void setConnect();

    //右侧偏移
    int getRightOffset();
    void setRightOffset(int offset);

    //底部偏移
    int getBottomOffset();
    void setBottomOffset(int offset);

    //显示内容设置
    QList<QColor> getColor() const;
    void setColor(QList<QColor> colorList);
    QList<QString> getTextList() const;
    void setTextList(QList<QString> textList,QList<float> dataList);
    QList<float> getData() const;
    void setData(QList<float> dataList);

    //设置锚定窗口，鼠标放上去时显示tooltip
    void anchorTarget(QWidget *target);

    //显示tip在widget的左上角
    void showTip(const QWidget *obj);

    //显示tip在点的左上角
    void showTip(const QPoint &rightBottom);

    //隐藏tip
    void hideTip();

protected:
    //过滤锚定窗口的enter和leave事件
    bool eventFilter(QObject *target, QEvent *event) override;
    void timerEvent(QTimerEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    //清除布局中的子对象
    void clearUI();

    //刷新窗口数据
    void flashDialog();

private:
    Ui::myToolTip *ui;

    QList<QColor> m_colorList;
    QList<QString> m_dataNameList;
    QList<float> m_dataList;

    //默认显示为point的左上角，通过属性设置偏移
    //以右下角为起点，左减右加，上减下加
    int rightOffset = -5;
    int bottomOffset = -5;

    //锚定的窗口
    QWidget *targetWidget=nullptr;
    QPoint targetPoint;
    //show和hide延迟
    QBasicTimer showTimer;
    QBasicTimer hideTimer;
};

#endif // MYTOOLTIP_H
