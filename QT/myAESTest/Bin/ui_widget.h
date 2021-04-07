/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QTextEdit *textOrigin;
    QTextEdit *textKey;
    QTextEdit *textEncode;
    QTextEdit *textDecode;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QTextEdit *textIV;
    QCheckBox *checkBox;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QString::fromUtf8("Widget"));
        Widget->resize(783, 555);
        textOrigin = new QTextEdit(Widget);
        textOrigin->setObjectName(QString::fromUtf8("textOrigin"));
        textOrigin->setGeometry(QRect(50, 50, 691, 111));
        textKey = new QTextEdit(Widget);
        textKey->setObjectName(QString::fromUtf8("textKey"));
        textKey->setGeometry(QRect(50, 180, 331, 61));
        textEncode = new QTextEdit(Widget);
        textEncode->setObjectName(QString::fromUtf8("textEncode"));
        textEncode->setGeometry(QRect(50, 270, 691, 111));
        textDecode = new QTextEdit(Widget);
        textDecode->setObjectName(QString::fromUtf8("textDecode"));
        textDecode->setGeometry(QRect(50, 410, 691, 131));
        pushButton = new QPushButton(Widget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(360, 245, 80, 20));
        pushButton_2 = new QPushButton(Widget);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setGeometry(QRect(360, 385, 80, 20));
        label = new QLabel(Widget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(60, 30, 54, 12));
        label_2 = new QLabel(Widget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(60, 165, 54, 12));
        label_3 = new QLabel(Widget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(60, 250, 54, 12));
        label_4 = new QLabel(Widget);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(60, 390, 54, 12));
        label_5 = new QLabel(Widget);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(390, 165, 54, 12));
        textIV = new QTextEdit(Widget);
        textIV->setObjectName(QString::fromUtf8("textIV"));
        textIV->setGeometry(QRect(390, 180, 351, 61));
        checkBox = new QCheckBox(Widget);
        checkBox->setObjectName(QString::fromUtf8("checkBox"));
        checkBox->setGeometry(QRect(90, 250, 73, 18));

        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QCoreApplication::translate("Widget", "Widget", nullptr));
        textOrigin->setHtml(QCoreApplication::translate("Widget", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'SimSun'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Microsoft YaHei','Helvetica','Meiryo UI','Malgun Gothic','Segoe UI','Trebuchet MS','Monaco','monospace','Tahoma','STXihei','\345\215\216\346\226\207\347\273\206\351\273\221','STHeiti','Helvetica Neue','Droid Sans','wenquanyi micro hei','FreeSans','Arimo','Arial','SimSun','\345\256\213\344\275\223','Heiti','\351\273\221\344\275\223','sans-serif'; font-size:14px; color:#333333; background-color:#ffffff;\">admin*admin</span></p></body></html>", nullptr));
        textKey->setHtml(QCoreApplication::translate("Widget", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'SimSun'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Microsoft YaHei','Helvetica','Meiryo UI','Malgun Gothic','Segoe UI','Trebuchet MS','Monaco','monospace','Tahoma','STXihei','\345\215\216\346\226\207\347\273\206\351\273\221','STHeiti','Helvetica Neue','Droid Sans','wenquanyi micro hei','FreeSans','Arimo','Arial','SimSun','\345\256\213\344\275\223','Heiti','\351\273\221\344\275\223','sans-serif'; font-size:14px; color:#333333; background-color:#ffffff;\">awa147369abcdefa</span></p></body></html>", nullptr));
        pushButton->setText(QCoreApplication::translate("Widget", "\345\212\240\345\257\206", nullptr));
        pushButton_2->setText(QCoreApplication::translate("Widget", "\350\247\243\345\257\206", nullptr));
        label->setText(QCoreApplication::translate("Widget", "\345\216\237\346\226\207", nullptr));
        label_2->setText(QCoreApplication::translate("Widget", "\345\257\206\345\214\231", nullptr));
        label_3->setText(QCoreApplication::translate("Widget", "\345\257\206\346\226\207", nullptr));
        label_4->setText(QCoreApplication::translate("Widget", "\350\247\243\345\257\206", nullptr));
        label_5->setText(QCoreApplication::translate("Widget", "\345\201\217\347\247\273\351\207\217", nullptr));
        textIV->setHtml(QCoreApplication::translate("Widget", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'SimSun'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Microsoft YaHei','Helvetica','Meiryo UI','Malgun Gothic','Segoe UI','Trebuchet MS','Monaco','monospace','Tahoma','STXihei','\345\215\216\346\226\207\347\273\206\351\273\221','STHeiti','Helvetica Neue','Droid Sans','wenquanyi micro hei','FreeSans','Arimo','Arial','SimSun','\345\256\213\344\275\223','Heiti','\351\273\221\344\275\223','sans-serif'; font-size:14px; color:#333333; background-color:#ffffff;\">0102030405147369</span></p></body></html>", nullptr));
        checkBox->setText(QCoreApplication::translate("Widget", "HEX", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
