#include "widget.h"

#include <QApplication>
#include <qDebug>
#include "wke.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    wkeSetWkeDllPath(L"node.dll");
    QString nodePath = QApplication::applicationDirPath() + "/node.dll";
    if(!QFile::exists(nodePath))
    {
        qDebug()<<QStringLiteral("请将node.dll拷贝到运行目录");
        return 0;
    }
    qDebug()<<nodePath;
    std::vector<wchar_t> tempPath;
    tempPath.resize(MAX_PATH);
    nodePath.toWCharArray(&tempPath[0]);
    wkeSetWkeDllPath(&tempPath[0]);
    wkeInitialize();
    Widget w;
    w.show();
    wkeFinalize();
    return a.exec();
//    QString nodePath = QApplication::applicationDirPath() + "/node.dll";
//        if(!QFile::exists(nodePath))
//        {
//            qDebug()<<QStringLiteral("请将node.dll拷贝到运行目录");
//            return 0;
//        }
//        qDebug()<<nodePath;
//        std::vector<wchar_t> tempPath;
//        tempPath.resize(MAX_PATH);
//        nodePath.toWCharArray(&tempPath[0]);
//        wkeSetWkeDllPath(&tempPath[0]);
//        wkeInitialize();
//        wkeWebView view = wkeCreateWebWindow(WKE_WINDOW_TYPE_TRANSPARENT, NULL, 0, 0, 640, 480);
//        if (!view)
//            return 0;
//        wkeResizeWindow(view, 1024, 720);
//        wkeLoadURLW(view, L"http://121.196.124.117:8088/");
//        wkeShowWindow(view, true);
//        wkeMoveToCenter(view);

//        int result = a.exec();
//        wkeFinalize();
//        return result;
}
