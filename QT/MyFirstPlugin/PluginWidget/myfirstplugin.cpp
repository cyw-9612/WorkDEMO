#include "myfirstplugin.h"
#include "pluginwidget.h"

MyFirstPlugin::MyFirstPlugin(QObject *parent) : QObject(parent)
{

}

QWidget *MyFirstPlugin::createPluginWidget(QWidget *parent)
{
    PluginWidget *pluginWidget = new PluginWidget(parent);
    return pluginWidget;
}
