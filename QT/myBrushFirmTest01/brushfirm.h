#ifndef BRUSHFIRM_H
#define BRUSHFIRM_H

#include <QWidget>
#include <QThread>
#include <QSerialPortInfo>
#include "../core/core.h"
#include "../uart/myserialport.h"

QT_BEGIN_NAMESPACE
namespace Ui { class BrushFirm; }
QT_END_NAMESPACE

class BrushFirm : public QWidget
{
    Q_OBJECT

public:
    BrushFirm(QWidget *parent = nullptr);
    ~BrushFirm();

private slots:
    void on_btn_chooseFile_clicked();

    void on_btn_downlode_clicked();

private:
    //更新进度条
    void OnDownloadProgress(bool is_finish, bool is_successed, int percent, QString note);
    //设置按键操作性
    void SetUiDisable(bool disable);
    //定义初始化操作
    void initUI();

private:
    Ui::BrushFirm *ui;

    //定义处理核心
    Core core_;
    //定义一个串口类
    MySerialPort *m_serial;
};
#endif // BRUSHFIRM_H
