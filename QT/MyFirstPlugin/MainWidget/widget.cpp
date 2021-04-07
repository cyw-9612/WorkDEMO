#include "widget.h"
#include "ui_widget.h"
#include <QPluginLoader>
#include <QDir>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    foreach(QString fileName, pluginsDir.entryList(QDir::Files)) {
           QPluginLoader pluginLoader(pluginsDir.absoluteFilePath(fileName));
           QObject *plugin = pluginLoader.instance();

           if(plugin)
           {
               m_pluginInterface = qobject_cast<AbstractInterface *>(plugin);
               if(m_pluginInterface)
               {
                   m_pluginInterface->createPluginWidget(ui->pluginWidget);
                   ok = true;
               }
           }
       }
}

Widget::~Widget()
{
    delete ui;
}

