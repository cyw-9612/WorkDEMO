/*************************************************
  Copyright (C), 2020-2021, AIHua Tech. Co., Ltd.
  File name:    baseview.cpp
  Author:       yangchun
  Version:      0.0.01
  Date:         2020-5-15
  Description:  自定义内嵌对话框
  History:
*************************************************/
#include "childview.h"

ChildView::ChildView(QWidget *parent) :
    BaseView(parent)
{
    setWindowFlags( Qt::CustomizeWindowHint| Qt::FramelessWindowHint);
}
