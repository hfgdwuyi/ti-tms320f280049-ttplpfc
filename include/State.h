#ifndef _STATE_H_
#define _STATE_H_

#include "DataType.h"
#include "Constant.h"
#include "Macro.h"
#include "Calculare.h"


typedef union
{
    UINT16   all;
    struct
    {
        UINT16 b1VgridRmsUVP        :1; // 电网输入AC欠压(1:欠压)
        UINT16 b1VgridRmsOVP        :1; // 电网输入AC过压(1:过压)
        UINT16 b1VgridDrop          :1; // 电网输入AC掉电
        UINT16 b1VgridPeakOVP       :1; // BUS 过压(1:过压)
        UINT16 b1IinvRmsOCP         :1; // 逆变输出AC过流(1:过流)
        UINT16 b1VinvRmsUVP         :1; // 逆变输出AC欠压(1:欠压)
        UINT16 b1VinvRmsOVP         :1; // 逆变输出AC过压(1:过压)
        UINT16 b1VinvPeakOVP        :1; // 逆变输出AC掉电

        UINT16 b1IacRmsOCP          :1; // AC过流(1:过流)
        UINT16 b1Rvd0               :1; // 保留
        UINT16 b1SCIFault           :1; // SCI通信故障
        UINT16 b1Temp1OTP           :1; // temp1过温故障
        UINT16 b1Temp2OTP           :1; // temp2过温故障
        UINT16 b1VbusUVP            :1; // BUS 欠压(1:欠压)
        UINT16 b1VbusOVP            :1; // BUS 过压(1:过压)
        UINT16 b1VbusHWOVP          :1; // BUS 过压(1:过压)
    }bits;
}T_U_PFCFAULTSTATUS;

typedef union
{
    UINT16   all;
    struct
    {
        UINT16 bRvd1                :4; // 保留

        UINT16 b1ACDrop             :1; // 原边关机标志位  PFC关机(1:关机)
        UINT16 b1ShutDown           :1; // 原边关机标志位  PFC关机(1:关机)

        UINT16 b1BypassRelay        :1; // 逆变输出继电器状态机(0:断开 1:闭合)
        UINT16 b1InputRelay         :1; // 电网输入继电器状态机(0:断开 1:闭合)
        UINT16 b1CaliRcvd           :1; // 校准参数接收完成标志(1:收到)
        UINT16 b1SoftStartDone      :1; // 软启完成
        UINT16 b1AuxSoftStart       :1; // 软启完成
        UINT16 b1FanVccOK           :1; // 风扇辅源输出
        UINT16 b1FanVccEn           :1; // 低压辅源使能
        UINT16 b1LVAuxEn            :1; // 低压辅源使能
        UINT16 b1VgridOK            :1; // 低压辅源使能
        UINT16 b1SelfTestOK         :1; // 自检正常
    }bits;
}T_U_PFCSUBSTATUS;

typedef struct
{
    UINT8 SysStatus;            // 工作状态
    UINT8 SubStatus_ACDC;       // ACDC工作状态
    UINT8 SubStatus_DCAC;       // DCAC工作状态

    T_U_PFCSUBSTATUS StateFlg;
    T_U_PFCFAULTSTATUS FaultFlg;
}T_S_PFCSTATUS;

extern T_S_PFCSTATUS g_strPFCStatus;

typedef union
{
    UINT16   all;
    struct
    {
        UINT16 bRvd1                :8; // 保留
        UINT16 bRvd0                :1; // 保留
        UINT16 b1SCIFault           :1; // SCI通信故障
        UINT16 b1TempSROTP          :1; // tempSR过温故障
        UINT16 b1IbatOCP            :1; // AC过流(1:过流)
        UINT16 b1VbatUVP            :1; // 电网输入AC欠压(1:欠压)
        UINT16 b1VbatOVP            :1; // 电网输入AC过压(1:过压)
        UINT16 b1VbusUVP            :1; // 逆变输出AC欠压(1:欠压)
        UINT16 b1VbusOVP            :1; // 逆变输出AC过压(1:过压)
    }bits;
}T_U_DCFAULTSTATUS;

// 系统状态标志位定义
typedef union
{
    UINT16   all;
    struct
    {
        UINT16 bRsvd1               :5; // 保留
        UINT16 b1ShutDown           :1; // 原边关机标志位  PFC关机(1:关机)
        UINT16 b1TempDerating       :1; // 降额标志（1：正在降额）-> U2
        UINT16 b1PFCCaliRcvd        :1; // 是否收到校准系数(1:收到) -> U2

        UINT16 bRsvd0               :5; // 保留
        UINT16 b1SoftStartDone      :1; // 软启完成
        UINT16 b1VgridOK            :1; // 电网输入AC电压正常
        UINT16 b1SelfTestOK         :1; // 自检正常
    }bits;
}T_U_DCSUBSTATUS;

typedef struct
{
    T_U_DCSUBSTATUS StateFlg;
    T_U_DCFAULTSTATUS FaultFlg;
}T_S_DCSTATUS;

extern T_S_DCSTATUS g_strDCStatus;

typedef union
{
    UINT16  all;
    struct
    {
        UINT16 b8Rsvd1              :8;     // 保留
        UINT16 b7Rsvd0              :7;     // 保留
        UINT16 b1UpsModeFlg         :1;     // UPS 功能标志位 0 - 禁止UPS功能 1 - 使能UPS功能
    }bits;
}T_U_DEVCFGFLG;

typedef struct
{
    T_U_DEVCFGFLG   CfgFlg;

    INT32 VoltPi_Kp;
    INT32 VoltPi_Ki;

    INT32 CurrPi_Kp;
    INT32 CurrPi_Ki;
}T_S_DEVCFG;

extern T_S_DEVCFG g_strDevCfg;





void State_Var_Init(void);
void State_Manage(void);

void SoftStart_Handler(void);

#endif

