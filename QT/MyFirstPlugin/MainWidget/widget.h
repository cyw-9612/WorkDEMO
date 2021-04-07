#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "abstractinterface.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    bool loadPlugin();

private:
    Ui::Widget *ui;

    AbstractInterface * m_pluginInterface;
};
#endif // WIDGET_H
