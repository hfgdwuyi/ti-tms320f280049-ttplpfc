#ifndef _SYS_TIMER_H_
#define _SYS_TIMER_H_

#include "DataType.h"
#include "Constant.h"



#define TIMEBASE_1MS_HANDLER()              {\
                                                g_strSysTimeFlg.TB1ms.all = 0xFFFF;\
                                                if((++g_u8Time1msCnt) >= DELAYB1MS_2MS)\
                                                {\
                                                    g_u8Time1msCnt = 0;\
                                                    g_u8Clock2msCnt ^= 1;\
                                                }\
                                            }\


// 定时标志位
typedef union
{
    UINT8   all;
    struct
    {
        UINT8 bRvd1                 :6; // 保留
        UINT8 b1GridPeriodCal       :1; //
        UINT8 b1StateMachine        :1; // 0 2ms定时标志，主程序中使用
     }bits;
}T_U_TIME1MSFLAG;

typedef union
{
    UINT8   all;
    struct
    {
        UINT8 bRvd1                 :5; // 保留
        UINT8 b1AuxSoftStart        :1; // 0 2ms定时标志，主程序中使用
        UINT8 b1WarnCtrl            :1; // 0 2ms定时标志，主程序中使用
        UINT8 b1TimeBase            :1; // 0 2ms定时标志，主程序中使用
     }bits;
}T_U_TIME2MSFLAG;

typedef union
{
    UINT8   all;
    struct
    {
        UINT8 bRvd1                 :7; // 保留
        UINT8 b1TemperatureCal      :1; // 0 10ms定时标志，温度采样使用
     }bits;
}T_U_TIME10MSFLAG;

typedef union
{
    UINT8   all;
    struct
    {
        UINT8 bRvd1                 :8; // 保留
//        UINT8 b1GridPeriodCal       :1; // 0 10ms定时标志，温度采样使用
     }bits;
}T_U_TIME20MSFLAG;

typedef union
{
    UINT8   all;
    struct
    {
        UINT8 bRvd1                 :7; // 保留
        UINT8 b1SCISend             :1; // 0 2ms定时标志，主程序中使用
    }bits;
}T_U_TIME50MSFLAG;

typedef union
{
    UINT8   all;
    struct
    {
        UINT8 bRvd1                 :7; // 保留
        UINT8 b1SCIFaultChk         :1; // 0 2ms定时标志，主程序中使用
     }bits;
}T_U_TIME100MSFLAG;

typedef struct
{
    T_U_TIME1MSFLAG     TB1ms;
    T_U_TIME2MSFLAG     TB2ms;
    T_U_TIME10MSFLAG    TB10ms;
    T_U_TIME20MSFLAG    TB20ms;
    T_U_TIME50MSFLAG    TB50ms;
    T_U_TIME100MSFLAG   TB100ms;
}T_S_SYSTIMEFLAG;

extern T_S_SYSTIMEFLAG g_strSysTimeFlg;

extern UINT8 g_u8Clock2msCnt;                // 2ms计数
extern UINT8 g_u8Time1msCnt;

void TimeBase_Var_Init(void);
void TimeBase_Manage(void);


#endif

