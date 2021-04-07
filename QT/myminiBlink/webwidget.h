#ifndef WEBWIDGET_H
#define WEBWIDGET_H

#include <QWidget>
#include <QResizeEvent>
#include "wke.h"

namespace Ui {
class WebWidget;
}

class WebWidget : public QWidget
{
    Q_OBJECT

public:
    explicit WebWidget(const QString& url="",QWidget *parent = 0);
    ~WebWidget();
    wkeWebView webView;
    void loadUrl(QString& url);
    void resizeEvent(QResizeEvent *event);
    void moveEvent(QMoveEvent *event);
    void onMove();
    void runJS(QString& js);
    bool iswkeIsLoadComplete();

signals:

private:
    Ui::WebWidget *ui;
};

#endif // WEBWIDGET_H
