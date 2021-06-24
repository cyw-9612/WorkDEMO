#ifndef WAVEFILE_H
#define WAVEFILE_H

#include <string>
#include <QWidget>
#include <QDebug>
using namespace std;

/***********************************
 *
 * 功 能: 录音音频文件处理软件:音频文件读取类
 *
 * 编 制: 陈宇葳
 *
 * 日 期: 2021/06/17
 *
 **********************************/


class WaveFile
{
public:
    WaveFile(QWidget *parent = nullptr);

    struct AWA6292Wavehead
        {
            char sign[4];         //"RIFF"标志 4
            unsigned long int flength;     //文件长度 8
            char wavesign[4];     //"WAVE"标志 12
            char fmtsign[4];      //"fmt"标志 16
            unsigned long int unused;      // 过渡字节（不定）20
            unsigned short formattype;     //格式类别（10H为PCM形式的声音数据) 22
            unsigned short  channelnum;    //通道数，单声道为1，双声道为2 24
            unsigned long int  samplerate; //采样率（每秒样本数），表示每个通道的播放速度 28
            unsigned long int transferrate; //传输速率，每秒字节数
            unsigned short int adjustnum;   //数据调整数，一个数据单位所占的字节
            unsigned short int databitnum; //每样本的数据位数，调整数*8 36
            char fact[4];   //“fact”标志
            unsigned long int factLength; //自定义数据块长度
            char factText[460];
        }head;

    unsigned long int datalength;   //采样数据总数
    unsigned long int totalsample;  //采样点数
    unsigned long int bitpersample; //采样位数
    unsigned long int datanum;      //数据块大小，若采样位数为16，开辟数据总数的大小，若为8，每个short型高低位可存储两个数据，开辟1/2大小即可

    qint32  *Data;

    void WavRead(string filename)
    {
        FILE *fp;
        if((fp=fopen(filename.c_str(),"rb"))==NULL)
        {
            //printf("cannot read wave file\n");
            exit(0);
        }
        fread(&head,sizeof(head),1,fp);
        char datasign[4];
        fread(datasign,4,1,fp);
        fread(&datalength,4,1,fp);

        totalsample = datalength / head.adjustnum;
        bitpersample = head.databitnum / head.channelnum;
        datanum = totalsample;

        try
        {
            //分配内存
            Data = new qint32[datanum+10];//开辟数据块，若采样位数为16，开辟数据总数的大小，若为8，每个short型高地位可存储两个数据，开辟1/2大小即可
        }
        catch(std::bad_alloc)
        {
            //异常处理
            qDebug() << "内存溢出";
            // 返回
            return;
        }

        if(bitpersample==32)
        {
            for(int i=0;!feof(fp) && i<datanum;i++)  //读入数据
            {
                fread(&Data[i],4,1,fp);
                //qDebug() << datax[i];
                Data[i] = Data[i];
                //qDebug() << datax[i];
                if(head.channelnum==2)  //若是双声道，跳过第二个声道
                    fseek(fp,4,SEEK_CUR);
            }
        }
        else if (bitpersample==16)
        {
            for(int i=0;!feof(fp) && i<datanum;i++)  //读入数据
            {
                fread(&Data[i],2,1,fp);
                //qDebug() << datax[i];
                Data[i] = Data[i];
                //qDebug() << datax[i];
                if(head.channelnum==2)  //若是双声道，跳过第二个声道
                    fseek(fp,2,SEEK_CUR);
            }
        }
        else if (bitpersample==8)
        {
            for(int i=0;!feof(fp) && i<datanum;i++)  //读入数据
            {
                fread(&Data[i],1,1,fp);
                //qDebug() << datax[i];
                Data[i] = Data[i];
                //qDebug() << datax[i];
                if(head.channelnum==2)  //若是双声道，跳过第二个声道
                    fseek(fp,1,SEEK_CUR);
            }
        }

        fclose(fp);
    }

};

#endif // WAVEFILE_H
