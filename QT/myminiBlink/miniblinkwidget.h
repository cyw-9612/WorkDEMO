#ifndef MINIBLINKWIDGET_H
#define MINIBLINKWIDGET_H

#include <QWidget>
#include "wke.h"

class miniBlinkWidget : public QWidget
{
    Q_OBJECT
public:
    explicit miniBlinkWidget(const QString& url="", QWidget *parent = nullptr);

    //初始化函数
    void initialize();

    //释放接口
    void finalize();

    wkeWebView webView;

    //用户判断是否初始化
    static bool _isInit;

    //用户判断当前有多少个QMiniblinkWebView实例
    static int _viewCount;

signals:

private:
    miniBlinkWidget & operator == (const miniBlinkWidget&){}
    miniBlinkWidget(const miniBlinkWidget&){}

};

#endif // MINIBLINKWIDGET_H
