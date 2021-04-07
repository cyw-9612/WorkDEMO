#ifndef PLUGINWIDGET_H
#define PLUGINWIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class PluginWidget; }
QT_END_NAMESPACE

class PluginWidget : public QWidget
{
    Q_OBJECT

public:
    PluginWidget(QWidget *parent = nullptr);
    ~PluginWidget();

private:
    Ui::PluginWidget *ui;
};
#endif // PLUGINWIDGET_H
