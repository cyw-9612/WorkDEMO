#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QSettings>

QT_BEGIN_NAMESPACE
namespace Ui { class Dialog; }
QT_END_NAMESPACE

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();

private:
    void readAuthHistoryFile();
    void saveAuthHistory();
private:
    //鼠标按下事件
    void mousePressEvent(QMouseEvent *event);
    //鼠标移动事件
    void mouseMoveEvent(QMouseEvent *event);
    //目前控件在父窗口中的位置
    QPoint m_windowPos;
    //鼠标位置
    QPoint m_mousePos;
    //移动后位置
    QPoint m_dPos;

private:
    void SetConfigData(QString qstrnodename, QString qstrkeyname, QVariant qvarvalue);
    QVariant GetConfigData(QString qstrnodename, QString qstrkeyname);

private slots:
    void slotTimtOut();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::Dialog *ui;
    const double x = (7100.00/(22*8*60*60));
    double money_counter_oneSecond;
    QTimer * m_timer;
    QSettings *m_psetting = nullptr;
};
#endif // DIALOG_H
