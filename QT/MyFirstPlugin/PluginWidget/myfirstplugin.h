#ifndef MYFIRSTPLUGIN_H
#define MYFIRSTPLUGIN_H

#include <QObject>
#include "abstractinterface.h"

class MyFirstPlugin : public QObject, public AbstractInterface
{
    Q_OBJECT
public:
    explicit MyFirstPlugin(QObject *parent = nullptr);

    Q_INTERFACES(AbstractInterface)
    Q_PLUGIN_METADATA(IID "Welcome to use this API" FILE "myfirstplugin.json")

    QWidget *createPluginWidget(QWidget *parent);
signals:

};

#endif // MYFIRSTPLUGIN_H
