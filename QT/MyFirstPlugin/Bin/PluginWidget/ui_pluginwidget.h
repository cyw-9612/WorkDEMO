/********************************************************************************
** Form generated from reading UI file 'pluginwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PLUGINWIDGET_H
#define UI_PLUGINWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PluginWidget
{
public:
    QPushButton *pushButton;
    QLineEdit *lineEdit;

    void setupUi(QWidget *PluginWidget)
    {
        if (PluginWidget->objectName().isEmpty())
            PluginWidget->setObjectName(QString::fromUtf8("PluginWidget"));
        PluginWidget->resize(800, 600);
        pushButton = new QPushButton(PluginWidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(340, 270, 80, 20));
        lineEdit = new QLineEdit(PluginWidget);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setGeometry(QRect(240, 210, 281, 21));

        retranslateUi(PluginWidget);

        QMetaObject::connectSlotsByName(PluginWidget);
    } // setupUi

    void retranslateUi(QWidget *PluginWidget)
    {
        PluginWidget->setWindowTitle(QCoreApplication::translate("PluginWidget", "PluginWidget", nullptr));
        pushButton->setText(QCoreApplication::translate("PluginWidget", "PushButton", nullptr));
    } // retranslateUi

};

namespace Ui {
    class PluginWidget: public Ui_PluginWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PLUGINWIDGET_H
