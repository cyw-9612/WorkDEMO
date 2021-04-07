/********************************************************************************
** Form generated from reading UI file 'rsa.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RSA_H
#define UI_RSA_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_rsa
{
public:

    void setupUi(QWidget *rsa)
    {
        if (rsa->objectName().isEmpty())
            rsa->setObjectName(QString::fromUtf8("rsa"));
        rsa->resize(800, 600);

        retranslateUi(rsa);

        QMetaObject::connectSlotsByName(rsa);
    } // setupUi

    void retranslateUi(QWidget *rsa)
    {
        rsa->setWindowTitle(QCoreApplication::translate("rsa", "rsa", nullptr));
    } // retranslateUi

};

namespace Ui {
    class rsa: public Ui_rsa {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RSA_H
