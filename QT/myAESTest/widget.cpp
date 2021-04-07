#include "widget.h"
#include "ui_widget.h"
#include <QCryptographicHash>
#include <QDebug>
#include "qaesencryption.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_pushButton_clicked()
{
    QAESEncryption encryption(QAESEncryption::AES_128, QAESEncryption::CBC, QAESEncryption::ZERO);
    QByteArray KEY = ui->textKey->toPlainText().toUtf8();
    QByteArray hashKey = QCryptographicHash::hash(KEY, QCryptographicHash::Md5);
    QByteArray IV = ui->textIV->toPlainText().toUtf8();
    QByteArray hashIV = QCryptographicHash::hash(IV, QCryptographicHash::Md5);
    //QByteArray encodedText = encryption.encode(ui->textOrigin->toPlainText().toUtf8(), KEY,IV);
    QByteArray AES_TEXT= QAESEncryption::Crypt(QAESEncryption::AES_128, QAESEncryption::CBC, ui->textOrigin->toPlainText().toUtf8(), KEY,IV, QAESEncryption::ZERO);

    qDebug() << KEY << "  " << IV;
    if(ui->checkBox->isChecked())
    {
        ui->textEncode->setText(QString::fromLatin1(AES_TEXT.toHex()));
    }
    else
    {
      ui->textEncode->setText(QString::fromLatin1(AES_TEXT.toBase64()));
    }

}

void Widget::on_pushButton_2_clicked()
{
    QAESEncryption encryption(QAESEncryption::AES_128, QAESEncryption::CBC, QAESEncryption::ZERO);
    QByteArray KEY = ui->textKey->toPlainText().toUtf8();
    QByteArray hashKey = QCryptographicHash::hash(KEY, QCryptographicHash::Md5);
    QByteArray IV = ui->textIV->toPlainText().toUtf8();
    QByteArray hashIV = QCryptographicHash::hash(IV, QCryptographicHash::Md5);
    QByteArray decodedText;
    if(ui->checkBox->isChecked())
    {
        decodedText = encryption.decode(QByteArray::fromHex(ui->textEncode->toPlainText().toLatin1()), KEY,IV);
    }
    else
    {
        decodedText = encryption.decode(QByteArray::fromBase64(ui->textEncode->toPlainText().toLatin1()), KEY,IV);
    }
    ui->textDecode->setText(QString::fromUtf8(decodedText));
}
