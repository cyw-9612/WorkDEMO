#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>

//设置录音的时间--ms
#define AUDIO_INPUT_TIME 2000


//#define ANDROID_DEVICE


#ifdef ANDROID_DEVICE
//设置保存文件的路径
#define SAVE_FILE_PATH "/sdcard/DS_XIAOLONG/test.raw"
#else
//设置保存文件的路径
#define SAVE_FILE_PATH "test.pcm"
#define SAVE_WAV_FILE_PATH "test.wav"
#endif


/*
 * 设置QT界面的样式
*/
void MainWindow::SetStyle(const QString &qssFile)
{
    QFile file(qssFile);
    if (file.open(QFile::ReadOnly)) {
        QString qss = QLatin1String(file.readAll());
        qApp->setStyleSheet(qss);
        QString PaletteColor = qss.mid(20,7);
        qApp->setPalette(QPalette(QColor(PaletteColor)));
        file.close();
    }
    else
    {
        qApp->setStyleSheet("");
    }
}

//日志信息显示
void MainWindow::Log_Display(QString text)
{
    ui->plainTextEdit->insertPlainText(text);
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->SetStyle(":/images/blue.css" );        //设置样式表
    this->setWindowIcon(QIcon(":/images/log.ico")); //设置图标
    this->setWindowTitle("录音机");

    //创建工作目录
#ifdef ANDROID_DEVICE
    QDir dir;
    if(!dir.exists("/sdcard/DS_XIAOLONG"))
    {
        if(dir.mkdir("/sdcard/DS_XIAOLONG"))
        {
            Log_Display("/sdcard/DS_XIAOLONG目录创建成功.\n");
        }
        else
        {
            Log_Display("/sdcard/DS_XIAOLONG目录创建失败.\n");
        }
    }
#endif
    //进度条更新
    progressBar_val=0;
    ui->progressBar->setRange(0,AUDIO_INPUT_TIME);
    ui->progressBar->setValue(0);
    connect(&timer_progressBar, SIGNAL(timeout()), this, SLOT(update_progressBar()));

    connect(&m_timer, SIGNAL(timeout()), this, SLOT(slotRead()));
}


void MainWindow::stopRecording()
{
  Log_Display("停止录音.\n");
  m_stopTime = QDateTime::currentMSecsSinceEpoch();

  m_elapasedUSecs = audio_in->elapsedUSecs();
  qint64 processedUSecs = audio_in->processedUSecs();

  audio_in->stop();
  destinationFile.close();

  Log_Display(tr("完成录音:%1,用时:%2\n").arg(QDateTime::fromMSecsSinceEpoch(m_stopTime).toString("hh:mm:ss.zzz")).arg(m_stopTime - m_startTime));
  Log_Display(tr("完成录音用时:%1us,处理数据量:%2\n").arg(m_elapasedUSecs).arg(processedUSecs));

}

MainWindow::~MainWindow()
{
    delete ui;
}

//录音状态
void MainWindow::handleStateChanged_input(QAudio::State newState)
{
 switch (newState) {
     case QAudio::StoppedState:
         if (audio_in->error() != QAudio::NoError) {
             // Error handling
             Log_Display("录音出现错误.\n");
         } else {
             // Finished recording
//             m_stopTime = QDateTime::currentMSecsSinceEpoch();
//             Log_Display(tr("完成录音:%1,用时:%2\n").arg(QDateTime::fromMSecsSinceEpoch(m_stopTime).toString("hh:mm:ss.zzz")).arg(m_stopTime - m_startTime));
//             //将PCM文件转为WAV文件
//             CreateWavFile(SAVE_FILE_PATH,SAVE_WAV_FILE_PATH);
             //处理PCM文件
             processPCM(SAVE_FILE_PATH);
         }
         break;
     case QAudio::ActiveState:
         // Started recording - read from IO device
         Log_Display("开始从IO设备读取PCM声音数据.\n");
         break;
     default:
         // ... other cases as appropriate
         break;
 }
}

//开始采集音频数据
void MainWindow::on_pushButton_clicked()
{
    static bool flag1=1;
    if(flag1) //只需要运行一次
    {
        flag1=0;
        //设置录音的格式
        auido_input_format.setSampleRate(96000); //设置采样率以对赫兹采样。 以秒为单位，每秒采集多少声音数据的频率.
//        auido_input_format.setSampleRate(48000); //设置采样率以对赫兹采样。 以秒为单位，每秒采集多少声音数据的频率.
        auido_input_format.setChannelCount(2);   //将通道数设置为通道。
        auido_input_format.setSampleSize(16);     /*将样本大小设置为指定的sampleSize（以位为单位）通常为8或16，但是某些系统可能支持更大的样本量。*/
        auido_input_format.setCodec("audio/pcm"); //设置编码格式
        auido_input_format.setByteOrder(QAudioFormat::LittleEndian); //样本是小端字节顺序
        auido_input_format.setSampleType(QAudioFormat::SignedInt); //样本类型

        //选择默认设备作为输入源
        //QAudioDeviceInfo info = QAudioDeviceInfo::defaultInputDevice();
        //选择声卡输入设备
        QAudioDeviceInfo info=input_list.at(ui->comboBox_input->currentIndex());

        Log_Display(tr("当前的录音设备的名字:%1\n").arg(info.deviceName()));

        //判断输入的格式是否支持，如果不支持就使用系统支持的默认格式
        if(!info.isFormatSupported(auido_input_format))
        {
          auido_input_format=info.nearestFormat(auido_input_format);
          /*
           * 返回与系统支持的提供的设置最接近的QAudioFormat。
             这些设置由所使用的平台/音频插件提供。
             它们还取决于所使用的QAudio :: Mode。
          */
        }
        //当前设备支持的编码
        Log_Display("当前设备支持的编码格式:\n");
        QStringList list=info.supportedCodecs();
        for(int i=0;i<list.size();i++)
        {
            Log_Display(list.at(i)+"\n");
        }

        Log_Display(tr("当前录音的采样率=%1\n").arg(auido_input_format.sampleRate()));
        Log_Display(tr("当前录音的通道数=%1\n").arg(auido_input_format.channelCount()));
        Log_Display(tr("当前录音的样本大小=%1\n").arg(auido_input_format.sampleSize()));
        Log_Display(tr("当前录音的编码格式=%1\n").arg(auido_input_format.codec()));
        audio_in = new QAudioInput(info,auido_input_format, this);
        connect(audio_in,SIGNAL(stateChanged(QAudio::State)), this, SLOT(handleStateChanged_input(QAudio::State)));
    }

    if(audio_in->state()==QAudio::StoppedState)
    {
       // qDebug()<<"没有处理任何数据.\n";
        //设置采集的时间
        QTimer::singleShot(AUDIO_INPUT_TIME,this,SLOT(stopRecording()));
        destinationFile.setFileName(SAVE_FILE_PATH);
        destinationFile.open( QIODevice::WriteOnly | QIODevice::Truncate);
        audio_in->start(&destinationFile);
//        m_steamIn = audio_in->start();
        m_startTime = QDateTime::currentMSecsSinceEpoch();
//        m_timer.start(200);
//        connect(m_steamIn, &QIODevice::readyRead,this,&MainWindow::slotRead);

        progressBar_val=0;
        ui->progressBar->setFormat("录音进度%p");
        timer_progressBar.start(500); //开始定时器--显示进度条
    }
}

//更新进度条
void MainWindow::update_progressBar()
{
    progressBar_val+=500; //500ms

    if(progressBar_val>=AUDIO_INPUT_TIME)
    {
        timer_progressBar.stop();
        m_timer.stop();
    }

    ui->progressBar->setValue(progressBar_val);
}

//开始播放音频
void MainWindow::on_pushButton_2_clicked()
{
    static bool flag=1;
    if(flag)
    {
        flag=0;
        //QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());
        //选择声卡输出设备
        QAudioDeviceInfo info=output_list.at(ui->comboBox_output->currentIndex());

        if(!info.isFormatSupported(auido_input_format))
        {
          Log_Display("后端不支持原始音频格式，无法播放音频.\n");
          return;
        }
        //当前设备支持的编码
        Log_Display("当前设备支持的编码格式:\n");
        QStringList list=info.supportedCodecs();
        for(int i=0;i<list.size();i++)
        {
            Log_Display(list.at(i)+"\n");
        }
        audio_out = new QAudioOutput(info,auido_input_format,this);
        connect(audio_out,SIGNAL(stateChanged(QAudio::State)), this, SLOT(handleStateChanged_out(QAudio::State)));
    }
    sourceFile.setFileName(SAVE_FILE_PATH);
    sourceFile.open(QIODevice::ReadOnly);
    audio_out->start(&sourceFile);
    progressBar_val=0;
    ui->progressBar->setFormat("播放进度%p");
    timer_progressBar.start(500); //开始定时器--显示进度条
}

//播放音频的反馈信息
void MainWindow::handleStateChanged_out(QAudio::State newState)
{
    switch (newState){
          case QAudio::IdleState:
              // Finished playing (no more data)
              audio_out->stop();
              sourceFile.close();
              Log_Display("音频播放完成.\n");
              break;

          case QAudio::StoppedState:
              // Stopped for other reasons
              if (audio_out->error() != QAudio::NoError) {
                 Log_Display("播放音频出现错误.\n");
              }
              break;
          default:
              // ... other cases as appropriate
              break;
      }
}

//查询可用的音频设备列表
void MainWindow::on_pushButton_3_clicked()
{
    output_list.clear();
    ui->comboBox_output->clear();
    foreach(const QAudioDeviceInfo &deviceInfo, QAudioDeviceInfo::availableDevices(QAudio::AudioOutput))
    {
         Log_Display(tr("声音输出设备:%1\n").arg(deviceInfo.deviceName()));
         output_list.append(deviceInfo);
         ui->comboBox_output->addItem(deviceInfo.deviceName());
    }

    input_list.clear();
    ui->comboBox_input->clear();
    foreach(const QAudioDeviceInfo &deviceInfo, QAudioDeviceInfo::availableDevices(QAudio::AudioInput))
    {
         Log_Display(tr("声音输入设备:%1\n").arg(deviceInfo.deviceName()));
         input_list.append(deviceInfo);
         ui->comboBox_input->addItem(deviceInfo.deviceName());
    }

}

void MainWindow::slotRead()
{
    int len1 = audio_in->bytesReady();
    if(len1 < 2048)
    {
        Log_Display(tr("数据长度不够=%1\n").arg(len1/2));
        return;
    }
    else
        Log_Display(tr("获取数据数据长度=%1\n").arg(len1/2));

    QByteArray byteArray(len1,0);
    m_steamIn->read(byteArray.data(),len1);
    Log_Display(tr("数据读取时间=%1,当前时间=%2\n").arg(m_startTime).arg(QDateTime::currentMSecsSinceEpoch()));
    m_startTime = QDateTime::currentMSecsSinceEpoch();
    int leng = len1 / 2;

    qint16 list[leng] = {0};
    memcpy(list, byteArray, sizeof(list));

    processingData(list, leng);

    //停止录音
//    stopRecording();

}

void MainWindow::processingData(qint16* numList, int leng)
{
//    int fftNum = 4096;
//    if(leng < fftNum)
//    {
//        fftNum = 2048;
//        if(leng < fftNum)
//            return;
//    }

//    fft* m_fft = new fft();

//    QVector<double> x_,y_;
//    for (int x = 0; x < fftNum; x++)
//    {
//        x_ << x;
//        y_ << numList[x];
//    }

//    //用上面的原始数据的初始化这个结构体类
//    QVector<Complex> in_(x_.size());
//    QVector<Complex> out_(x_.size());

//    for(auto i = 0; i< x_.size();i++)
//    {
//        in_[i].im = i;
//        in_[i].rl = y_[i];
//    }

    //调用接口 生成频域的 out_ 数据
//    if(m_fft->fft1(in_,in_.size(),out_))
//    {
//        QVector<double> x1,y1;

//        int maxIndex = 0;
//        double max = 0;

//        for(auto i = 0;i<out_.size()/2;i++)
//        {
//            x1<<i;
//            //用计算出的实部当做y轴画图
//            y1<<out_[i].rl;

//            if(max < out_[i].rl)
//            {
//                max = out_[i].rl;
//                maxIndex = i;
//            }
//        }

//        Log_Display(tr("数据分析FFT最大值=%1,所在坐标=%2\n").arg(max).arg(maxIndex));
//    }

    /**使用暂停录音时的时间节点减去多媒体设备的录音时长最为实际的开始录音时间
        实际使用中存在2ms误差
    */
    m_startTime = m_stopTime - (m_elapasedUSecs/1000);

    qint16 max_left = 0, max_right = 0;
    qint16 min_left = 0, min_right = 0;
    int channelcount = auido_input_format.channelCount();
    for (int x = 0; x < leng; x = x + channelcount)
    {
        if(max_left < numList[x])
            max_left = numList[x];

        if(min_left > numList[x])
            min_left = numList[x];
    }

    for (int x = 1; x < leng; x = x + channelcount)
    {
        if(max_right < numList[x])
            max_right = numList[x];

        if(min_right > numList[x])
            min_right = numList[x];
    }

    int leftIndex = 0;
    QList<int> peekList;

    for (int x = channelcount; x < leng; x = x + channelcount)
    {
        if(x == leng - channelcount)
            continue;

        double gate = 0.5;
        if(numList[x] > max_left*gate)
        {
//            if(numList[x-1] < numList[x] && numList[x] > numList[x+1])
//                peekList << x;

            //找到峰值信号在门限值的左右坐标求中间坐标
            if(numList[x - channelcount] <= max_left*gate /*&& numList[x+1] > max*gate*/)
                leftIndex = x;

            if(/*numList[x - 1] > max*gate && */numList[x + channelcount] <= max_left*gate)
            {
                int index = (leftIndex + x) / (2 * channelcount);
                peekList << index;
                leftIndex = x;

                m_triggerTime = m_startTime + (1000*index)/96000;
                QString triggerTime = QDateTime::fromMSecsSinceEpoch(m_triggerTime).toString("hh:mm:ss.zzz");
                Log_Display(tr("当前坐标：%5。前一个数据=%1，当前数据=%2,后一个数据=%3。触发时间：%6。门限值=%4\n").arg(numList[x - 1]).arg(numList[x]).arg(numList[x + 1]).arg(max_left*0.9).arg(x).arg(triggerTime));
            }
        }
    }

    int countNum = 0;
    int peekLeng = peekList.length();
    int sum = 0;
    int minDif = 0.005 * auido_input_format.sampleRate();
    int lastIndex = 0;
    for (int i = 1; i < peekLeng; i++)
    {
        int dif = peekList[i] - peekList[i-1];
        if(minDif < dif )
        {
            if(0 == lastIndex)
            {
                lastIndex = peekList[i];
                continue;
            }

            dif = peekList[i] - lastIndex;
            sum += dif;
            Log_Display(tr("差值：%1\n").arg(dif));
            countNum++;
            lastIndex = peekList[i];
        }
    }

    if(countNum == 0)
        countNum++;

    m_triggerAvgCount = sum / countNum;
    m_triggerCycle = auido_input_format.sampleRate() / m_triggerAvgCount;
    if((max_left - min_left) > 10)
        Log_Display(tr("\n左声道：当前最大值=%3数据每两个峰值之间相差坐标：%1,估算周期为%2HZ\n\n").arg(m_triggerAvgCount).arg(m_triggerCycle).arg(max_left));
    else
        Log_Display("左声道无信号\n\n");

    if(1 == channelcount)
        return;

    //双声道才考虑右通道的分析
    int index_right = 0;
    QList<int> peekListRight;
    for (int x = 3; x < leng; x = x + channelcount)
    {
        if(x == leng - 1)
            continue;

        if(numList[x] > max_right*0.9)
        {
            //找到峰值信号在门限值的左右坐标求中间坐标
            if(numList[x - channelcount] <= max_right*0.9)
                index_right = x;

            if(numList[x+ channelcount] <= max_right*0.9)
            {
                //因为右声道的数组坐标有偏移量一，所以实际的时间坐标要减一
                int index = (index_right + x) / (2 * channelcount);
                index--;
                peekListRight << index;
                index_right = x;
            }
        }
    }

    countNum = 0;
    peekLeng = peekListRight.length();
    sum = 0;
    for (int i = 1; i < peekLeng; i++)
    {
        sum += peekListRight[i] - peekListRight[i-1];
        countNum++;
    }

    if(countNum == 0)
        countNum++;

    m_triggerAvgCount = sum / countNum;
    m_triggerCycle = auido_input_format.sampleRate() / m_triggerAvgCount;

    if((max_right - min_right) > 10)
        Log_Display(tr("\n右声道: 当前最大值=%3数据每两个峰值之间相差坐标：%1,估算周期为%2HZ\n\n").arg(m_triggerAvgCount).arg(m_triggerCycle).arg(max_right));
    else
        Log_Display("右声道无信号\n\n");

}

struct WAVFILEHEADER
{
    // RIFF 头
    char RiffName[4];
    unsigned long nRiffLength;

    // 数据类型标识符
    char WavName[4];

    // 格式块中的块头
    char FmtName[4];
    unsigned long nFmtLength;

    // 格式块中的块数据
    unsigned short nAudioFormat;
    unsigned short nChannleNumber;
    unsigned long nSampleRate;
    unsigned long nBytesPerSecond;
    unsigned short nBytesPerSample;
    unsigned short nBitsPerSample;

    // 数据块中的块头
    char    DATANAME[4];
    unsigned long   nDataLength;
};


// 将生成的.raw文件转成.wav格式文件;
qint64 MainWindow::CreateWavFile(QString PcmFileName,QString wavFileName)
{
    // 开始设置WAV的文件头
    WAVFILEHEADER WavFileHeader;
    qstrcpy(WavFileHeader.RiffName,"RIFF");
    qstrcpy(WavFileHeader.WavName, "WAVE");
    qstrcpy(WavFileHeader.FmtName, "fmt ");
    qstrcpy(WavFileHeader.DATANAME,"data");

    // 表示 FMT块 的长度
    WavFileHeader.nFmtLength = 16;
    // 表示 按照PCM 编码;
    WavFileHeader.nAudioFormat = 1;
    // 声道数目;
    WavFileHeader.nChannleNumber = 2;
    // 采样频率;
    WavFileHeader.nSampleRate = 96000;
//    WavFileHeader.nSampleRate = 16000;

    // nBytesPerSample 和 nBytesPerSecond这两个值通过设置的参数计算得到;
    // 数据块对齐单位(每个采样需要的字节数 = 通道数 × 每次采样得到的样本数据位数 / 8 )
    WavFileHeader.nBytesPerSample = 2;
    // 波形数据传输速率
    // (每秒平均字节数 = 采样频率 × 通道数 × 每次采样得到的样本数据位数 / 8  = 采样频率 × 每个采样需要的字节数 )
//    WavFileHeader.nBytesPerSecond = 32000;
    WavFileHeader.nBytesPerSecond = 192000;

    // 每次采样得到的样本数据位数;
    WavFileHeader.nBitsPerSample = 16;

    QFile cacheFile(PcmFileName);
    QFile wavFile(wavFileName);

    if (!cacheFile.open(QIODevice::ReadWrite))
    {
        return -1;
    }

    if (!wavFile.open(QIODevice::WriteOnly))
    {
        return -2;
    }

    int nSize = sizeof(WavFileHeader);
    qint64 nFileLen = cacheFile.bytesAvailable();

    WavFileHeader.nRiffLength = static_cast<unsigned long>(nFileLen - 8 + nSize);
    //static_cast<类型>(变量);  //新式的强制转换
    WavFileHeader.nDataLength = static_cast<unsigned long>(nFileLen);

    // 先将wav文件头信息写入，再将音频数据写入;
    wavFile.write((const char *)&WavFileHeader,nSize);
    wavFile.write(cacheFile.readAll());

    cacheFile.close();
    wavFile.close();

    return nFileLen;
}

void MainWindow::processPCM(QString PcmFileName)
{
    QFile cacheFile(PcmFileName);

    if (!cacheFile.open(QIODevice::ReadWrite))
    {
        return;
    }

    qint64 nFileLen = cacheFile.bytesAvailable();

    QByteArray byteArray(nFileLen,0);

    int leng = nFileLen / 2;

    qint16 list[leng] = {0};
    memcpy(list, cacheFile.readAll(), sizeof(list));

    processingData(list, leng);
}
