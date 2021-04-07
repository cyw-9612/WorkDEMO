#include "rsa.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    rsa w;
    w.show();
    return a.exec();
}
