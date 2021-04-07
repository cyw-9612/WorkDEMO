/********************************************************************************
** Form generated from reading UI file 'brushfirm.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BRUSHFIRM_H
#define UI_BRUSHFIRM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_BrushFirm
{
public:
    QLabel *lab_productVersion;
    QLabel *lab_num;
    QLabel *lab_nowVersion;
    QLabel *lab_hardwareVersion;
    QLabel *lab_softwareVersion;
    QLabel *lab_newVersion;
    QLabel *lab_other;
    QLabel *lab_versoin;
    QLabel *label_commu;
    QLabel *lab_introduce;
    QLabel *lab_port;
    QProgressBar *progressBar;
    QPushButton *btn_downlode;
    QLineEdit *edit_num;
    QLineEdit *edit_softwareVersion;
    QLineEdit *edit_introduce;
    QLineEdit *edit_hardwareVersion;
    QLineEdit *edit_port;
    QComboBox *comboBox_productVersion;
    QComboBox *comboBox_version;
    QComboBox *comboBox_port;
    QPushButton *btn_chooseFile;
    QTextEdit *textEdit_filePath;
    QFrame *frame;
    QFrame *frame_2;
    QFrame *frame_3;

    void setupUi(QWidget *BrushFirm)
    {
        if (BrushFirm->objectName().isEmpty())
            BrushFirm->setObjectName(QString::fromUtf8("BrushFirm"));
        BrushFirm->resize(800, 600);
        BrushFirm->setMinimumSize(QSize(800, 600));
        BrushFirm->setMaximumSize(QSize(800, 600));
        lab_productVersion = new QLabel(BrushFirm);
        lab_productVersion->setObjectName(QString::fromUtf8("lab_productVersion"));
        lab_productVersion->setGeometry(QRect(130, 50, 71, 16));
        lab_num = new QLabel(BrushFirm);
        lab_num->setObjectName(QString::fromUtf8("lab_num"));
        lab_num->setGeometry(QRect(460, 50, 71, 16));
        lab_nowVersion = new QLabel(BrushFirm);
        lab_nowVersion->setObjectName(QString::fromUtf8("lab_nowVersion"));
        lab_nowVersion->setGeometry(QRect(40, 110, 71, 16));
        lab_hardwareVersion = new QLabel(BrushFirm);
        lab_hardwareVersion->setObjectName(QString::fromUtf8("lab_hardwareVersion"));
        lab_hardwareVersion->setGeometry(QRect(130, 160, 71, 16));
        lab_softwareVersion = new QLabel(BrushFirm);
        lab_softwareVersion->setObjectName(QString::fromUtf8("lab_softwareVersion"));
        lab_softwareVersion->setGeometry(QRect(470, 160, 71, 16));
        lab_newVersion = new QLabel(BrushFirm);
        lab_newVersion->setObjectName(QString::fromUtf8("lab_newVersion"));
        lab_newVersion->setGeometry(QRect(40, 220, 81, 16));
        lab_other = new QLabel(BrushFirm);
        lab_other->setObjectName(QString::fromUtf8("lab_other"));
        lab_other->setGeometry(QRect(40, 330, 54, 12));
        lab_versoin = new QLabel(BrushFirm);
        lab_versoin->setObjectName(QString::fromUtf8("lab_versoin"));
        lab_versoin->setGeometry(QRect(130, 280, 54, 12));
        label_commu = new QLabel(BrushFirm);
        label_commu->setObjectName(QString::fromUtf8("label_commu"));
        label_commu->setGeometry(QRect(130, 370, 71, 16));
        lab_introduce = new QLabel(BrushFirm);
        lab_introduce->setObjectName(QString::fromUtf8("lab_introduce"));
        lab_introduce->setGeometry(QRect(470, 280, 61, 16));
        lab_port = new QLabel(BrushFirm);
        lab_port->setObjectName(QString::fromUtf8("lab_port"));
        lab_port->setGeometry(QRect(470, 370, 54, 12));
        progressBar = new QProgressBar(BrushFirm);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setGeometry(QRect(170, 500, 421, 21));
        progressBar->setValue(0);
        btn_downlode = new QPushButton(BrushFirm);
        btn_downlode->setObjectName(QString::fromUtf8("btn_downlode"));
        btn_downlode->setGeometry(QRect(570, 550, 80, 20));
        edit_num = new QLineEdit(BrushFirm);
        edit_num->setObjectName(QString::fromUtf8("edit_num"));
        edit_num->setGeometry(QRect(560, 50, 113, 20));
        edit_softwareVersion = new QLineEdit(BrushFirm);
        edit_softwareVersion->setObjectName(QString::fromUtf8("edit_softwareVersion"));
        edit_softwareVersion->setGeometry(QRect(560, 160, 113, 20));
        edit_introduce = new QLineEdit(BrushFirm);
        edit_introduce->setObjectName(QString::fromUtf8("edit_introduce"));
        edit_introduce->setGeometry(QRect(560, 280, 113, 20));
        edit_hardwareVersion = new QLineEdit(BrushFirm);
        edit_hardwareVersion->setObjectName(QString::fromUtf8("edit_hardwareVersion"));
        edit_hardwareVersion->setGeometry(QRect(230, 160, 113, 20));
        edit_port = new QLineEdit(BrushFirm);
        edit_port->setObjectName(QString::fromUtf8("edit_port"));
        edit_port->setGeometry(QRect(220, 370, 113, 20));
        comboBox_productVersion = new QComboBox(BrushFirm);
        comboBox_productVersion->addItem(QString());
        comboBox_productVersion->setObjectName(QString::fromUtf8("comboBox_productVersion"));
        comboBox_productVersion->setGeometry(QRect(230, 50, 111, 21));
        comboBox_version = new QComboBox(BrushFirm);
        comboBox_version->addItem(QString());
        comboBox_version->addItem(QString());
        comboBox_version->addItem(QString());
        comboBox_version->setObjectName(QString::fromUtf8("comboBox_version"));
        comboBox_version->setGeometry(QRect(230, 280, 111, 21));
        comboBox_port = new QComboBox(BrushFirm);
        comboBox_port->setObjectName(QString::fromUtf8("comboBox_port"));
        comboBox_port->setGeometry(QRect(560, 360, 111, 21));
        btn_chooseFile = new QPushButton(BrushFirm);
        btn_chooseFile->setObjectName(QString::fromUtf8("btn_chooseFile"));
        btn_chooseFile->setGeometry(QRect(600, 440, 80, 20));
        textEdit_filePath = new QTextEdit(BrushFirm);
        textEdit_filePath->setObjectName(QString::fromUtf8("textEdit_filePath"));
        textEdit_filePath->setGeometry(QRect(200, 410, 361, 81));
        frame = new QFrame(BrushFirm);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setGeometry(QRect(120, 90, 621, 51));
        frame->setFrameShape(QFrame::HLine);
        frame->setFrameShadow(QFrame::Raised);
        frame_2 = new QFrame(BrushFirm);
        frame_2->setObjectName(QString::fromUtf8("frame_2"));
        frame_2->setGeometry(QRect(130, 200, 621, 51));
        frame_2->setFrameShape(QFrame::HLine);
        frame_2->setFrameShadow(QFrame::Raised);
        frame_3 = new QFrame(BrushFirm);
        frame_3->setObjectName(QString::fromUtf8("frame_3"));
        frame_3->setGeometry(QRect(120, 310, 621, 51));
        frame_3->setFrameShape(QFrame::HLine);
        frame_3->setFrameShadow(QFrame::Raised);

        retranslateUi(BrushFirm);

        QMetaObject::connectSlotsByName(BrushFirm);
    } // setupUi

    void retranslateUi(QWidget *BrushFirm)
    {
        BrushFirm->setWindowTitle(QCoreApplication::translate("BrushFirm", "BrushFirm", nullptr));
        lab_productVersion->setText(QCoreApplication::translate("BrushFirm", "\344\272\247\345\223\201\345\236\213\345\217\267\357\274\232", nullptr));
        lab_num->setText(QCoreApplication::translate("BrushFirm", "\344\270\262\345\217\267\357\274\232", nullptr));
        lab_nowVersion->setText(QCoreApplication::translate("BrushFirm", "\347\216\260\346\234\211\347\211\210\346\234\254\345\217\267\357\274\232", nullptr));
        lab_hardwareVersion->setText(QCoreApplication::translate("BrushFirm", "\347\241\254\344\273\266\347\211\210\346\234\254\345\217\267\357\274\232", nullptr));
        lab_softwareVersion->setText(QCoreApplication::translate("BrushFirm", "\350\275\257\344\273\266\347\211\210\346\234\254\345\217\267\357\274\232", nullptr));
        lab_newVersion->setText(QCoreApplication::translate("BrushFirm", "\346\226\260\347\211\210\346\234\254\345\217\267\351\200\211\346\213\251\357\274\232", nullptr));
        lab_other->setText(QCoreApplication::translate("BrushFirm", "\345\205\266\344\273\226\357\274\232", nullptr));
        lab_versoin->setText(QCoreApplication::translate("BrushFirm", "\347\211\210\346\234\254\345\217\267\357\274\232", nullptr));
        label_commu->setText(QCoreApplication::translate("BrushFirm", "\351\200\232\344\277\241\346\226\271\345\274\217\357\274\232", nullptr));
        lab_introduce->setText(QCoreApplication::translate("BrushFirm", "\347\211\210\346\234\254\346\217\217\350\277\260\357\274\232", nullptr));
        lab_port->setText(QCoreApplication::translate("BrushFirm", "\344\270\262\345\217\243\345\217\267\357\274\232", nullptr));
        btn_downlode->setText(QCoreApplication::translate("BrushFirm", "\344\270\213\350\275\275", nullptr));
        edit_num->setText(QCoreApplication::translate("BrushFirm", "123456", nullptr));
        edit_softwareVersion->setText(QCoreApplication::translate("BrushFirm", "V1.0.0", nullptr));
        edit_introduce->setText(QCoreApplication::translate("BrushFirm", "\346\227\240", nullptr));
        edit_hardwareVersion->setText(QCoreApplication::translate("BrushFirm", "V1.0.0", nullptr));
        edit_port->setText(QCoreApplication::translate("BrushFirm", "\344\270\262\345\217\243(\351\273\230\350\256\244115200)", nullptr));
        comboBox_productVersion->setItemText(0, QCoreApplication::translate("BrushFirm", "AWA....", nullptr));

        comboBox_version->setItemText(0, QCoreApplication::translate("BrushFirm", "V1.0.1", nullptr));
        comboBox_version->setItemText(1, QCoreApplication::translate("BrushFirm", "V1.0.2", nullptr));
        comboBox_version->setItemText(2, QCoreApplication::translate("BrushFirm", "V1.1.0", nullptr));

        btn_chooseFile->setText(QCoreApplication::translate("BrushFirm", " \351\200\211\346\213\251\346\226\207\344\273\266", nullptr));
    } // retranslateUi

};

namespace Ui {
    class BrushFirm: public Ui_BrushFirm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BRUSHFIRM_H
