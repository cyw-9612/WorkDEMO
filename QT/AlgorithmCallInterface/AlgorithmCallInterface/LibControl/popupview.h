/*************************************************
  Copyright (C), 2020-2021, AIHua Tech. Co., Ltd.
  File name:    popupview.h
  Author:       yangchun
  Version:      0.0.01
  Date:         2020-5-15
  Description:  弹出内嵌对话框
  History:
*************************************************/
#ifndef POPUPVIEW_H
#define POPUPVIEW_H

#include <QObject>
#include "LibControl_global.h"

class QPushButton;
class QLabel;

#include "baseview.h"

class LIBCONTROL_EXPORT PopupView: public BaseView
{
    Q_OBJECT
public:
    PopupView(QWidget *parent = nullptr);
    void MoveToCenter();// 居中显示
    void SetTitle(QString title) override;
    void SetSize(int width, int height);// 设置界面大小

private:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

signals:
    void CloseSignals();

public slots:
    void OnCloseSlots();

public:
    bool m_drag;
    QPoint m_mouseStartPoint;//获得鼠标的初始位置
    QPoint m_windowTopLeftPoint;//获得窗口的初始位置
    QPushButton *m_btnClose;
    QLabel *m_lbTitle;
};

#endif // POPUPVIEW_H
