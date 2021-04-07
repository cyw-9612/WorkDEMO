#include "webwidget.h"
#include "ui_webwidget.h"
#include <QHBoxLayout>
#include <QDebug>

WebWidget::WebWidget(const QString& url,QWidget *parent) : QWidget(parent)
{
    ui->setupUi(this);

    webView= wkeCreateWebWindow(WKE_WINDOW_TYPE_TRANSPARENT, (HWND)this->winId(), 0, 0, this->width(),this->height());
//    webView= wkeCreateWebWindow(WKE_WINDOW_TYPE_CONTROL, (HWND)this->winId(), 0, 0, this->width(),this->height());
    wkeShowWindow(webView, TRUE);
    wkeLoadURL(webView,url.toLocal8Bit().data());

}

WebWidget::~WebWidget()
{
    qDebug()<<"in del";
    delete ui;
}

void WebWidget::loadUrl(QString& url)
{
    wkeLoadURL(webView,url.toLocal8Bit().data());
    onMove();
}

void WebWidget::resizeEvent(QResizeEvent *event)
{
    qDebug() << "尺寸变换。";
    Q_UNUSED(event);
    wkeResize(webView, this->width(),this->height());
    QPoint pos=this->mapToGlobal(QPoint(0,0));
    wkeMoveWindow(webView,pos.x(),pos.y(),this->width(),this->height());
}

void WebWidget::moveEvent(QMoveEvent *event)
{
    Q_UNUSED(event);
    onMove();
}

void WebWidget::onMove()
{
    qDebug() << "窗口移动。";
    QPoint pos=this->mapToGlobal(QPoint(0,0));
    wkeMoveWindow(webView,pos.x(),pos.y(),this->width(),this->height());
}

void WebWidget::runJS(QString& js)
{
    wkeRunJS(webView, js.toUtf8());
}

bool WebWidget::iswkeIsLoadComplete()
{
    return wkeIsLoadComplete(webView);
}
