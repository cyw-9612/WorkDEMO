#include "wainmain.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    wainMain w;
    w.show();
    return a.exec();
}
