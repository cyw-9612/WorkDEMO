#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork/QUdpSocket>
#include <QAudio>     //这五个是QT处理音频的库
#include <QAudioFormat>
#include <QAudioInput>
#include <QAudioOutput>
#include <QIODevice>
#include <QFile>
#include <QTimer>
#include <QDir>
#include "fft.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    void SetStyle(const QString &qssFile);
    QFile sourceFile;   // class member.
    QFile destinationFile;   // Class member
    QAudioFormat auido_input_format;
    QTimer timer_progressBar;
    int progressBar_val;
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QAudioInput *audio_in;
    QAudioOutput *audio_out;
    void Log_Display(QString text);
    qint64 CreateWavFile(QString catheFileName , QString wavFileName);
    void processPCM(QString catheFileName);
    QList<QAudioDeviceInfo> input_list;
    QList<QAudioDeviceInfo> output_list;
private slots:
    void update_progressBar();
    void on_pushButton_clicked();
    void stopRecording();
    void handleStateChanged_input(QAudio::State newState);
    void handleStateChanged_out(QAudio::State newState);
    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void slotRead();

private:
    void processingData(qint16* numList, int leng);

private:
    Ui::MainWindow *ui;

    qint64 m_startTime = 0;
    qint64 m_stopTime = 0;
    qint64 m_triggerTime = 0;
    qint64 m_elapasedUSecs = 0;
    double m_triggerAvgCount = 0;
    double m_triggerCycle = 0;
    QIODevice *m_steamIn;

    QTimer m_timer;
};
#endif // MAINWINDOW_H
