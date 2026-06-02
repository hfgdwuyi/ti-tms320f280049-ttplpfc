#ifndef _CALCULATION_H_
#define _CALCULATION_H_

//#include "DSP28x_Project.h"
//#include "IQmathLib.h"

#include "DataType.h"
#include "Macro.h"
#include "Constant.h"
#include "TimeBase.h"
#include "State.h"
#include "InitDevice.h"

#define PFC_CLOSE_LOOP              1

typedef union
{
    UINT16   all;
    struct
    {
        UINT16 bRsvd1               :8; // ����
        UINT16 bRsvd0               :6; // ����
        UINT16 b1LoopClose          :1; // �ջ���־λ
    }bits;
}T_U_PWMDriverFlg;

extern T_U_PWMDriverFlg g_unPWMDriverFlg;

typedef struct
{
    INT32 b0;
    INT32 b2;
    INT32 a1;
    INT32 a2;
    INT32 qb0;
    INT32 qb1;
    INT32 qb2;

    INT32 input[3];
    INT32 u[3];
    INT32 q_u[3];
}T_S_SOGI;

typedef struct
{
    INT32 Theta[2];
    INT32 u_d[2];
    INT32 u_q[2];
    INT32 y[2];

    INT32 B1;
    INT32 B0;

    INT32 SinTheta;
    INT32 CosTheta;

    INT32 Freq_G;
    INT32 Freq_O;

    INT32 Ts;
}T_S_PLL;



typedef struct
{
    INT16 Temperat1_ADC;
    INT16 Temperat2_ADC;
    INT32 BusVolt_ADC;
    INT32 RefVolt_ADC;
    INT32 Iout2_ADC;
    INT32 Vbus_ADC;
    INT32 Iout_ADC;
    INT32 Iin_ADC;
    INT32 GridVolt_ADC;
    INT32 GridVolt_ADC2Q;
    INT32 Vac_ADC2PLL;
    INT32 Vac_L_ADC;
    INT32 Vac_N_ADC;
}T_S_ADC;


typedef struct
{
    float Err;
    float Out;
    float DErr;
    float DOut;
    float Err_Prev;
    float Out_Prev;
    float Kp;
    float Ki;
    float Inter;
    float Out_Max;
    float Out_Min;
}T_S_PID;

enum
{
    POSITION_HALF = 1,
    NEGTIVE_HALF = 2,
    POSITION_CROSS_ZERO = 3,
    NEGTIVE_CROSS_ZERO = 4,
};

enum
{
    RESET    = 0,
    DECREASE = 1,
    INCREASE = 2,
};

extern UINT8 g_u8PFCPWMSwState;
extern UINT8 g_u8GridStatus;
extern UINT8 g_u8EnvelopeStatus;

extern ubitfloat g_lq12GridFreq;
extern ubitfloat g_lq12GridVolt;
extern ubitfloat g_lq12InputCurrent;
extern ubitfloat g_lq12BusVoltage;
extern ubitfloat g_lq12OutCurrent1;
extern ubitfloat g_lq12OutCurrent2;

extern ubitfloat g_lq12GridVoltRms;
extern ubitfloat g_lq12InnACVoltRms;
extern ubitfloat g_lq12InnACCurrRms;
extern ubitfloat g_lq12InvVoltRms;
extern ubitfloat g_lq12InvCurrRms;

extern ubitfloat g_lq12InvVoltRmsRef;
extern ubitfloat g_lq12InvVoltRmsSet;

extern ubitfloat g_lq12BusVolt;
extern ubitfloat g_lq12BusVoltRef;
extern ubitfloat g_lq12BusVoltSet;

extern ubitfloat g_lq15Temperat1;
extern ubitfloat g_lq15Temperat2;

extern T_S_ADC g_strADC;

extern INT32 g_i32VgridFeeb;
extern UINT8 g_u8IsPositivalHalfFlg;



void Temperature_Cal_Init(void);
void Temperature_Cal(void);

void inline Adc_Cal_100KHz(void);
void Adc_Cal_10KHz(void);
void GridPeriodRmsCal(void);
void GridDirectionCal(void);

void VoltLoop_Var_Init(void);
void VoltLoopCtrl(void);
void Iref_Cal(void);
void CurrLoop_Var_Init(void);
void CurrLoopCtrl(void);
void PWMDriver_Var_Init(void);
void PWMDriver_Ctrl(void);
void PWM2_Update(void);

#endif
