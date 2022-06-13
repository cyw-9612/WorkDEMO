#ifndef CONSTANT_H
#define CONSTANT_H

class Constant
{
public:

    const static int AirCondution = 0;// 气导
    const static int BoneCondution = 1;// 骨导
    const static int SoundCondution = 2;// 声场

    const static int PureTone = 0;// 纯音
    const static int WarbleTone = 1;// 啭音
    const static int PulsePureTone = 2;// 脉冲纯音

    const static int NarrowBandNoise = 0;// 窄带噪音
    const static int WhiteNoise = 1;// 白噪音

    const static int LeftHear = 0;// 左耳听力
    const static int RightHear = 1;// 右耳听力

    const static int SoundChannelSound = 0; // 发声
    const static int SoundChannelMask = 1;  // 遮掩
    const static int SoundChannelSilent = 2;// 无声

    const static int SinglePlay = 0; // 单次播放
    const static int CircelPlay = 0; // 循环播放

    const static int OutputPowerOverLimit = 100; // 信号输出功率过门限

    const static int AIDNoMatch = 0;// 未选配
    const static int AIDHasMatch = 1;// 已选配

    const static int EnablePower = 0;// 功放
    const static int DisablePower = 1;// 非功放

    /*
     * 性别
    */
    const static int Man = 0;// 男
    const static int Woman = 1;// 女

    /*
     * 播放模式
    */
    const static int PlayModeAuto = 0;// 自动
    const static int PlayModeManual = 1;// 手动

    /*
     * 报告单类型
    */
    const static int ReportTypeHear = 0;// 听力测试
    const static int ReportTypeSpeech = 1;// 言语测听

    // 基准声音频率
    const static int BaseHearFreq = 1000;//单位Hz

    /*
     * 声音来源
    */
    const static int Glossary = 0;// 词汇
    const static int MIC = 1;// 麦克风
    const static int ExternalDevice = 2;// 外部设备
};

#endif // CONSTANT_H
