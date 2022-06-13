/*************************************************
  Copyright (C), 2020-2021, AIHua Tech. Co., Ltd.
  File name:    childview.h
  Author:       yangchun
  Version:      0.0.01
  Date:         2020-5-15
  Description:  自定义内嵌对话框
  History:
*************************************************/
#ifndef BASECHILDVIEW_H
#define BASECHILDVIEW_H

#include "baseview.h"

class LIBCONTROL_EXPORT ChildView : public BaseView
{
    Q_OBJECT
public:
    explicit ChildView(QWidget *parent = nullptr);

signals:

};

#endif // BASECHILDVIEW_H
