#include "winmain.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    winMain w;
    w.show();
    return a.exec();
}
