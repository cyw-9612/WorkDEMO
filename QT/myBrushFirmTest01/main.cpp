#include "brushfirm.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    BrushFirm w;
    w.show();
    return a.exec();
}
