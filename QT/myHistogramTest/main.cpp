#include "datahistogram.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    dataHistogram w;
    w.show();
    return a.exec();
}
