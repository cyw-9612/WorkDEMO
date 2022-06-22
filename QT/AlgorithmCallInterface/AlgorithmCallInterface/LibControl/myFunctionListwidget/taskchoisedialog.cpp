#include "taskchoisedialog.h"
#include "ui_taskchoisedialog.h"
#include "mydoublevalidator.h"
#include <QJsonObject>
#include <QJsonDocument>

taskChoiseDialog::taskChoiseDialog(QWidget *parent) :
    ChildView(parent),
    ui(new Ui::taskChoiseDialog)
{
    ui->setupUi(this);

    initUI();

    setStyle();

    setCtrlRange();

    connectSlots();
}

taskChoiseDialog::~taskChoiseDialog()
{
    delete ui;
}

//设置风格
void taskChoiseDialog::setStyle()
{

}

//关联信号槽
void taskChoiseDialog::connectSlots()
{

}

//初始化界面
void taskChoiseDialog::initUI()
{
    MyDoubleValidator * dv1 = new  MyDoubleValidator(0);
    dv1->setNotation(QDoubleValidator::StandardNotation);
    dv1->setRange(0.0, 300.0, 0);
    ui->edit_function1->setValidator(dv1);

    MyDoubleValidator * dv2 = new  MyDoubleValidator(0);
    dv2->setNotation(QDoubleValidator::StandardNotation);
    dv2->setRange(0.0, 15.0, 2);
    ui->edit_function2->setValidator(dv2);

    MyDoubleValidator * dv3 = new  MyDoubleValidator(0);
    dv3->setNotation(QDoubleValidator::StandardNotation);
    dv3->setRange(0.0, 200.0, 3);
    ui->edit_function3->setValidator(dv3);

    MyDoubleValidator * dv4 = new  MyDoubleValidator(0);
    dv4->setNotation(QDoubleValidator::StandardNotation);
    dv4->setRange(0.0, 100.0, 2);
    ui->edit_function4->setValidator(dv4);

    MyDoubleValidator * dv5 = new  MyDoubleValidator(0);
    dv5->setNotation(QDoubleValidator::StandardNotation);
    dv5->setRange(0.0, 1000.0, 3);
    ui->edit_function5->setValidator(dv5);

    MyDoubleValidator * dv6 = new  MyDoubleValidator(0);
    dv6->setNotation(QDoubleValidator::StandardNotation);
    dv6->setRange(0.0, 50.0, 3);
    ui->edit_function6->setValidator(dv6);
}

//设置控件输入范围
void taskChoiseDialog::setCtrlRange()
{

}

//设置ANN参数列表信息
void taskChoiseDialog::setANNInfo(QString dataJson)
{
    try {
        QJsonParseError error;
        QJsonDocument jsonDocument = QJsonDocument::fromJson(dataJson.toUtf8(), &error);
        if (error.error == QJsonParseError::NoError)
        {
            if (!(jsonDocument.isNull() || jsonDocument.isEmpty()))
            {
                if (jsonDocument.isObject())
                {
                    QVariantMap result = jsonDocument.toVariant().toMap();

                    ui->edit_function1->setText(result["function1"].toString());
                    ui->edit_function2->setText(result["function2"].toString());
                    ui->edit_function3->setText(result["function3"].toString());
                    ui->edit_function4->setText(result["function4"].toString());
                    ui->edit_function5->setText(result["function5"].toString());
                    ui->edit_function6->setText(result["function6"].toString());
                    ui->com_function7->setCurrentText(result["function7"].toString());
                    ui->com_function8->setCurrentText(result["function8"].toString());
                    m_UIInfo = dataJson;
                }
            }
        }
        else
        {
            // 检查错误类型
            qFatal(error.errorString().toUtf8().constData());
            //exit(1);
        }
    } catch (...) {

    }
}

void taskChoiseDialog::on_btn_clearDate_clicked()
{
    //设置还原
    setANNInfo(m_UIInfo);
    emit sigCancal();
}

void taskChoiseDialog::on_btn_export_clicked()
{
    QJsonObject jsonObject;

    jsonObject.insert("function1", ui->edit_function1->text());
    jsonObject.insert("function2", ui->edit_function2->text());
    jsonObject.insert("function3", ui->edit_function3->text());
    jsonObject.insert("function4", ui->edit_function4->text());
    jsonObject.insert("function5", ui->edit_function5->text());
    jsonObject.insert("function6", ui->edit_function6->text());
    jsonObject.insert("function7", ui->com_function7->currentText());
    jsonObject.insert("function8", ui->com_function8->currentText());

    QString stringFull = "";
    stringFull.append((QJsonDocument(jsonObject).toJson()));
    stringFull = stringFull.simplified();
    stringFull.replace(" ", "");

    m_UIInfo = stringFull;
    emit sigComfirm(m_UIInfo);
}

void taskChoiseDialog::on_edit_function1_editingFinished()
{
    double num = ui->edit_function1->text().toDouble();
    QString text = QString::number(num, 'f', 0);
    ui->edit_function1->setText(text);
}

void taskChoiseDialog::on_edit_function2_editingFinished()
{
    double num = ui->edit_function2->text().toDouble();
    QString text = QString::number(num, 'f', 2);
    ui->edit_function2->setText(text);
}

void taskChoiseDialog::on_edit_function3_editingFinished()
{
    double num = ui->edit_function3->text().toDouble();
    QString text = QString::number(num, 'f', 3);
    ui->edit_function3->setText(text);
}

void taskChoiseDialog::on_edit_function4_editingFinished()
{
    double num = ui->edit_function4->text().toDouble();
    QString text = QString::number(num, 'f', 2);
    ui->edit_function4->setText(text);
}

void taskChoiseDialog::on_edit_function5_editingFinished()
{
    double num = ui->edit_function5->text().toDouble();
    QString text = QString::number(num, 'f', 3);
    ui->edit_function5->setText(text);
}

void taskChoiseDialog::on_edit_function6_editingFinished()
{
    double num = ui->edit_function6->text().toDouble();
    QString text = QString::number(num, 'f', 3);
    ui->edit_function6->setText(text);
}
