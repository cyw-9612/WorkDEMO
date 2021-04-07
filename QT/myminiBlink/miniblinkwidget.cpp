#include "miniblinkwidget.h"
#include <QApplication>
#include <QFile>
#include <QDebug>

miniBlinkWidget::miniBlinkWidget(const QString& url,QWidget *parent) : QWidget(parent)
{
    webView= wkeCreateWebWindow(WKE_WINDOW_TYPE_TRANSPARENT, (HWND)this->winId(), 0, 0, this->width(),this->height());
//    webView= wkeCreateWebWindow(WKE_WINDOW_TYPE_CONTROL, (HWND)this->winId(), 0, 0, this->width(),this->height());
//    wkeShowWindow(webView, TRUE);
//    wkeLoadURL(webView,url.toLocal8Bit().data());
    SetParent(wkeGetWindowHandle(webView), (HWND)this->winId());
}

//初始化函数
void miniBlinkWidget::initialize()
{
    QString nodePath = QApplication::applicationDirPath() + "/node.dll";
        if(!QFile::exists(nodePath))
        {
            qDebug()<<QStringLiteral("请将node.dll拷贝到运行目录");
            return;
        }
        std::vector<wchar_t> tempPath;
        tempPath.resize(MAX_PATH);
        nodePath.toWCharArray(&tempPath[0]);
        wkeSetWkeDllPath(&tempPath[0]);

        wkeInitialize();
//        _isInit = true;
//        ++miniBlinkWidget::_viewCount;
}

//释放接口
void miniBlinkWidget::finalize()
{
//    if(--miniBlinkWidget::_viewCount == 0)
//    {
//        wkeFinalize();
//        _isInit = false;
//    }
}
