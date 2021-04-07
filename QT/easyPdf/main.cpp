#include "widget.h"

#include <QApplication>
#include <QStandardPaths>
#include <QWidget>
#include "poppler-qt5.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QString filename;
    filename = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation)
            + "/打印测试用.pdf";//桌面上的test文件
    Poppler::Document* document = Poppler::Document::load(filename);
    if (!document || document->isLocked()) {
        delete document;
        return 1;
    }
    if (document == 0) {
        return 1;
    }
    document->setRenderHint(Poppler::Document::TextAntialiasing);

    for(int i = 0; i<document->numPages(); i++){
        Poppler::Page* pdfPage = document->page(i);
        if (pdfPage == 0) {
            return 1;
        }
        int xres = 72.0, yres = 72.0;
        QImage image = pdfPage->renderToImage(xres*2, yres*2, 0, 0, pdfPage->pageSizeF().width()*2,
                                              pdfPage->pageSizeF().height()*2);
        if (image.isNull()) {
            return 1;
        }else{
            image.save(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation)
                       + QString("/pdf-page%1.png").arg(i), "png");
        }
        delete pdfPage;
    }
    delete document;
    exit(0);
    return a.exec();
}
