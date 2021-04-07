#include "pluginwidget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PluginWidget w;
    w.show();
    return a.exec();
}
