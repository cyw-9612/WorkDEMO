/*************************************************
  Copyright (C), 2020-2021, AIHua Tech. Co., Ltd.
  File name:    rtmessagebox.h
  Author:       yangchun
  Version:      0.0.01
  Date:         2020-9-8
  Description:  通用蒙板
  History:
*************************************************/
#ifndef RTMASKLAYER_H
#define RTMASKLAYER_H

#include <QWidget>
#include <QMutex>
#include "LibControl_global.h"

class LIBCONTROL_EXPORT RtMaskLayer : public QWidget
{
    Q_OBJECT

public:
    RtMaskLayer(QWidget *parent = Q_NULLPTR);
    ~RtMaskLayer();

    static RtMaskLayer *Instance()
    {
        static QMutex mutex;

        if (!m_instance)
        {
            QMutexLocker locker(&mutex);

            if (!m_instance) {
                m_instance = new RtMaskLayer();
            }
        }

        return m_instance;
    }

    QStringList DialogName() const;//获取遮挡的对话框类名
    void SetDialogNames(const QStringList& DialogName);//设置要遮挡的对话框类名
    void StackUnder(QObject *parent);// 提升控件层级
    void SetMainWidget(QWidget* pWidget);// 设置父容器
    void Show(QDialog *parent);// 显示

protected:
    bool eventFilter(QObject *obj, QEvent *event);
    void paintEvent(QPaintEvent *e);

private:
    static RtMaskLayer* m_instance;
    QStringList m_dialogNames;
    QWidget* m_mainWidget;;

    //设置产生遮蔽罩的类名
    QString m_nowObject = "no";
};

#endif // RTMASKLAYER_H
