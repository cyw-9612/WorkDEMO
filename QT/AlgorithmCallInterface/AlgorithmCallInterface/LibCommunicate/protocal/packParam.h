#ifndef PACKPARAM_H
#define PACKPARAM_H

#include <QByteArray>
#include <QString>

#pragma pack(push) //保存对齐状态
#pragma pack(1)//设定为1字节对齐

/***********************************
 *
 * 功 能: 参数包模块 （现在主要防止5912通信结构体）
 *
 * 编 制: 陈宇葳
 *
 * 更新日期: 2020/06/25
 *
 **********************************/

//自定义数据类型 byte
typedef unsigned char byte;

/************AWA5912仪器信息读取指令*********************/
//上位机发送读取指令,读取数据和读取设置信息都是这个
struct packRead
{
    char head[7];					//头"<N>AUTH"
    int len;  							//整个长度	（整个结构体长度）
    int cmd;							//指令类型  6：读取数据
    int num;							//num：读取第num组数据（1-16 最多16组数据）读取仪器设置信息（0）
    char tail[4];                  //尾"</N>"
};

//接收到的下位机全部信息结构体
struct packRecvAll
{
    char head[7];                                       //头"<N>AUTH"
    int len;                                                   //整个长度	（整个结构体长度）
    int cmd;                                                 //指令类型  7：上传全部数据
    int num;                                                 //num：读取第num组数据（1-16 最多16组数据）读取仪器设置信息（0）
    int state;                                                //1,0 有数据是1 没数据0
    char instrumentType[16];                   //仪器型号
    char instrumentSerialNumber[16];   //仪器序列号(机号)
    char StartTime[32];                             //开始时间
    char EndTime[32];                               //结束时间
    char Exposure [16];                              //声暴露E
    char Dose[16];                                       //噪声剂量
    char Leq_t[16];                                      //Leq,t
    char Lex_8h [16];                                  // Leq,8h
    char Lmax[16];                                      // Lmax
    char Lmin[16];                                       // Lmin
    char NLpeak1 [16];                                // NLpeak1
    char NLpeak2 [16];                                // NLpeak2
    char NLpeak3 [16];                                // NLpeak3
    char LCpeak[16];                                    //LCpeak
    char SEL[16];                                           //SEL
    //以下为2020/06/22 新增 依据产品部需求 < AWA5912 个人噪声剂量计任务书 V1.03 >
    //2020/06/23 经过调试发现22号张自立提供的协议长度16不对,检查下位机程序发现他的数组长度是32，所以此处修改为32
    char SensitivityLevel[32];                  //灵敏度级
    char CALSPL[32];                                  //校准声压级
    char CalibrationDate [32];                  //校准日期
    char TS[32];                                           //TS
    char OverLoad[32];                               //是否过载
    char No2SoftwareName[32];               //暂未用
    char tail[4];                  //尾"</N>"
};

//下位机发送仪器现有模板信息
struct packRecvTemplateInformation
{
    char head[7];					//头"<N>AUTH"
    int len;  						//整个长度	（整个结构体长度）
    int cmd;						//指令类型  4：仪器上传基本信息
    char DateSet[16];				//日期设置     2020-05-18
    char TimeSet[16];			    //时间设置     15:49:00
    char TimeSustain[16];       	//Ts设置	       13h10min30s
    char TimeWeight[16];          //时间计权	   F
    char FrequencyWeight [16];	    //频率计权      A
    char Dose[16];			        //Dose          85
    char ChangeRate[16];		    //交换率        3
    char ThresholdValue [16];		//门限值        70
    char NLpeak1[16];		        // NLpeak1      120
    char NLpeak2 [16];		        // NLpeak2      130
    char NLpeak3 [16];			    // NLpeak3       140
    char lockstate[16];	            // 字符串  “lock”  /   “unlock”
    char No0SoftwareName[16];	    //暂未用
    char No1SoftwareName[16];	    //暂未用
    char tail[4];                  //尾"</N>"
};

//上位机对5912设置模板
struct packSetTemplate
{
    char head[7];					//头"<N>AUTH"
    int len;  						//整个长度	（整个结构体长度）
    int cmd;						//指令类型  4：服务器写入基本信息
    char DateSet[16];				//日期设置     2020-05-18
    char TimeSet[16];			    //时间设置     15:49:00
    char TimeSustain[16];       	//Ts设置	       13h10min30s
    char TimeWeight[16];          //时间计权	   F
    char FrequencyWeight [16];	    //频率计权      A
    char Dose[16];			        //Dose          85
    char ChangeRate[16];		    //交换率        3
    char ThresholdValue [16];		//门限值        70
    char NLpeak1[16];		        // NLpeak1      120
    char NLpeak2 [16];		        // NLpeak2      130
    char NLpeak3 [16];			    // NLpeak3       140
    char lockstate[16];	            // 字符串  “lock”  /   “unlock”
    char No0SoftwareName[16];	    //暂未用
    char No1SoftwareName[16];	    //暂未用
    char tail[4];                  //尾"</N>"
};

//对5912设置模板信息后的返回结果
struct packSetTemplateReply
{
    char head[7];						//头"<N>AUTH"
    int len;  							//整个长度	（整个结构体长度）
    int cmd;							//指令类型  5：仪器设置成功   10：仪器测量中，设置失败
    char tail[4];                  //尾"</N>"
};

//接收到的下位机发送信息结构体
struct packRecvInfo
{
    char head[7];						//头"<N>AUTH"
    int len;  							//整个长度	（整个结构体长度）
    int cmd;							//指令类型  7：上传数据
    int num;							//num：上传第num组数据（1-16）
    int state;							//1,0
    char instrumentType[16];			//仪器型号
    char instrumentSerialNumber[16];  	//仪器序列号(机号)
    char StartTime[32];                //开始时间
    char EndTime[32];			        //结束时间
    char Exposure [16];			        //声暴露E
    char Dose[16];		                //噪声剂量
    char Leq_t[16];		            //Leq,t
    char Leq_8h [16];				    // Leq,8h
    char Lmax[16];		            // Lmax
    char Lmin[16];		                // Lmin
    char NLpeak1 [16];				    // NLpeak1
    char NLpeak2 [16];				    // NLpeak2
    char NLpeak3 [16];				    // NLpeak3
    char LCPEAK [16];	                //LCPEAK
    char SEL [16];	                    //SEL
    char SensitivityLevel[32];            //sensitivity level
    char CALSPL[32];                  //校准声压级
    char CalibrationDate [32];           //calibration dates
    char TS[32];                      //暂未用
    char No1SoftwareName[32];        //暂未用
    char No2SoftwareName[32];         //暂未用
    char tail[4];                  //尾"</N>"
};

//上位机确认接收到上传的信息
struct packRecvInfoSuccess
{
    char head[7];						//头"<N>AUTH"
    int len;  							//整个长度	（整个结构体长度）
    int cmd;							//指令类型  8：数据接收成功
    char tail[4];                  //尾"</N>"
};

//上位机发送清除仪器信息
struct packClearData
{
    char head[7];						//头"<N>AUTH"
    int len;  							//整个长度	（整个结构体长度）
    int cmd;							//指令类型  9：清空数据
    int num;							//num 0：清空数据 num 1-16：清空第n组数据
    char tail[4];                  //尾"</N>"
};

//下位机清除数据反馈
struct packClearReply
{
    char head[7];						//头"<N>AUTH"
    int len;  							//整个长度	（整个结构体长度）
    int cmd;							//指令类型  5：数据删除成功 10：仪器测量中，删除失败
    char tail[4];                  //尾"</N>"
};

//设定Ts列表【上位机发送】
struct packSetTimeList
{
    char head[7];						//头"<N>AUTH"
    int len;  							//整个长度	D4（整个结构体长度）
    int cmd;							//指令类型  0B：写入TS列表数据
    qint32 tslist[50];				//Ts数据
    char tail[4];                  //尾"</N>"
};

//设定Ts列表【下位机发送】
struct packSetTimeReply
{
    char head[7];			//头"<N>AUTH"
    int len;  					//整个长度	0C（整个结构体长度）
    int cmd;					//指令类型  0C：写入TS列表数据成功  其他：失败
    char tail[4];              //尾"</N>"
};

//读取Ts列表【上位机发送】
struct packRedTimeList
{
    char head[7];			//头"<N>AUTH"
    int len;  					//整个长度	0C（整个结构体长度）
    int cmd;					//指令类型  0D：读取TS列表
    char tail[4];              //尾"</N>"
};

//读取Ts列表【下位机发送】
struct packRedTimeReply
{
    char head[7];			//头"<N>AUTH"
    int len;  					//整个长度	D4（整个结构体长度）
    int cmd;					//指令类型  0E：返回TS列表数据
    qint32 tslist[50];		//Ts数据
    char tail[4];              //尾"</N>"
};

/************通用仪器授权信息读取指令*********************/
//获取仪器信息[上位机发送]
struct currencyPackInfo
{
    char head[7];						//头"<N>AUTH"
    int len;  							//整个长度	（整个结构体长度）
    int cmd;							//指令类型  4：获取仪器信息
    char tail[4];                        //尾 </N>
};

//下位机返回仪器信息结构体[下位机发送]
struct currencyPackRecvInfo
{
    char head[7];						//头"<N>AUTH"
    int len;  							//整个长度	（整个结构体长度）
    int cmd;							//指令类型  5：仪器发送信息
    char instrumentSerialNumber[32];		//仪器序列号(机号)
    char hardwareVersion[32];        	//硬件版本
    char softwareVersion[32];			//软件版本
    char tail[4];                        //尾 </N>
};

/************AWAJSON信息读取指令*********************/
//指令类型参数（json中为字符串）
enum AWAJSON_cmd
{
    //该参数放在 Class 值中
    DeviceConnect = 101, //连接仪器
    AuthInfoCmd = 130, //授权信息
    CalibrationInfoCmd = 131, // 校准信息
    MeasureInfoCmd = 132, // 测量数据
    RealTimeInfoCmd = 133, // 读取实时数据
    RealFileCmd = 134, // 读取文件
};

//读取校准记录的CMD指令（json中为数字）
enum CalibrationInfo_cmd
{
    //该参数放在 CMD 值中
    AskCalibrationInfoNum = 0, //上位机读取校准信息数量
    ReplyCalibrationInfoNum = 1, // 下位机回复校准信息数量
    AskCalibrationData = 2, // 上位机读取一条校准记录
    ReplyCalibrationData = 3, // 下位机返回一条校准记录
};

//读取测量数据的CMD指令（json中为数字）
enum MeasureInfo_cmd
{
    //该参数放在 CMD 值中
    AskMeasureInfoNum = 0, //上位机读取总的测量数据数量（6292中为文件夹数）
    ReplyMeasureInfoNum = 1, // 下位机回复总的测量数据数量
    AskMeasureOneDataCount = 2, // 上位机读取某个文件夹下的数据数量
    ReplyMeasureOneDataCount = 3, // 下位机返回某个文件夹下的数据数量
    AskMeasureOneData = 4, // 上位机读取指代文件夹下的一条数据（顺序为数字正序，下位机安数据库排序返回）
    ReplyMeasureOneData = 5, // 下位机返回指代文件夹下的一条数据
};

#pragma pack(pop)//恢复对齐状态

#endif // PACKPARAM_H
