#ifndef ABSTRACTINTERFACE_H
#define ABSTRACTINTERFACE_H

#include <QObject>
class QWidget;
class AbstractInterface
{
public:
    virtual ~AbstractInterface() {}
    virtual QWidget *createPluginWidget(QWidget *parent) = 0;
};

#define AbstractInterface_iid "Welcome to use this API"
Q_DECLARE_INTERFACE(AbstractInterface, AbstractInterface_iid)

#endif // ABSTRACTINTERFACE_H
