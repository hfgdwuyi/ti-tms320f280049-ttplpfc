#include "Calculare.h"
#include "Table.h"
#include "math.h"

#define DEBUG_ONLY

UINT8 g_u8ADCArrayCnts = 0;

UINT32 g_u32VinADCTest = 0;
UINT32 g_u32IinADCTest = 0;

UINT16 g_u16RMSSumCnts = 0;
UINT16 g_u16RMSCalCnts = 0;
float g_i32SumReciCnts = 0;
INT32 g_i32SumReciCntsQValue = 0;

UINT32 g_u32HFPWMCMP = 0;
UINT32 g_u32HFPWMDuty = 0;
UINT32 g_u32HFPWMDutyMax = 0;
UINT32 g_u32HFPWMDutyMin = 0;

INT32 g_i32VgridFeeb = 0;
UINT16 g_u16PFCStateCnts = 0;


UINT8 g_u8IsZeroCrossingFlg = 0;
UINT8 g_u8IsPositivalHalfFlg = 0;
UINT8 g_u8PFCACSignFiltered = 0;
UINT8 g_u8PFCPWMSwState = 0;
UINT16 g_u16PFCSoftStartdbFED = 0;
UINT16 g_u16PFCdbFEDSetValue = 0;
UINT16 g_u16PFCSoftStartdbRED = 0;
UINT16 g_u16PFCdbREDSetValue = 0;
UINT16 g_u16PFCSoftStartDuty = 0;
UINT16 g_u16PFCSoftStartDutydbFED = 0;
UINT8 g_u8GridStatus;
UINT8 g_u8EnvelopeStatus;

UINT16 g_u16PFCPWMDriverStateSlew = 0;
INT32 g_i32PLLSine = 0;
INT32 g_i32PLLSinePrev = 0;

UINT16 g_u16AuxDBRef = 0;
UINT16 g_u16AuxSoftStartDB = 0;
UINT16 g_u16AuxSoftStartdbRED = 0;
UINT16 g_u16AuxSoftStartdbFED = 0;

float g_lq2GridVoltRmsSum;

//_iq2 g_lq2InvVoltRmsSum;
//_iq2 g_lq2GridVoltRmsSum;
//_iq2 g_lq2GridVoltRmsSumQValue;
//_iq2 g_lq2InnACVoltRmsSum;
//_iq4 g_lq4InvCurrRmsSum;
//_iq4 g_lq4InnACCurrRmsSum;
//
//_iq2 g_lq2InvVoltRmsCalSum;
//_iq2 g_lq2GridVoltRmsCalSum;
//_iq2 g_lq2InnACVoltRmsCalSum;
//_iq4 g_lq4InvCurrRmsCalSum;
//_iq4 g_lq4InnACCurrRmsCalSum;

ubitfloat g_lq12GridFreq;
ubitfloat g_lq12GridVolt;
ubitfloat g_lq12InputCurrent;
ubitfloat g_lq12OutCurrent1;
ubitfloat g_lq12BusVoltage;
ubitfloat g_lq12OutCurrent;

ubitfloat g_lq12GridVoltRms;
ubitfloat g_lq12GridVoltRmsQValue;
ubitfloat g_lq12GridVoltRmsToUp;
ubitfloat g_lq12InnACVoltRms;
ubitfloat g_lq12InvVoltRms;
ubitfloat g_lq12InnACCurrRms;
ubitfloat g_lq12InvCurrRms;

ubitfloat g_lq12InvVoltRmsRef;
ubitfloat g_lq12InvVoltRmsSet;

ubitfloat g_lq12BusVolt;
ubitfloat g_lq12BusVoltRef;
ubitfloat g_lq12BusVoltSet;

ubitfloat g_lq15Temperat1;
ubitfloat g_lq15Temperat2;

UINT16 g_u16Temp1_Array[ADC_ARRAY_SIZE] = {0};
UINT16 g_u16Temp2_Array[ADC_ARRAY_SIZE] = {0};
UINT16 g_u16Vref1v5_Array[ADC_ARRAY_SIZE] = {0};

T_S_ADC g_strADC;
T_S_PLL g_strPLL;
T_S_SOGI g_strSOGI;
T_S_PID g_strVLoop;
T_S_PID g_strILoop;
T_U_PWMDriverFlg g_unPWMDriverFlg;

typedef struct
{
    float vbusSoftRefPu;
    float iRefPu;
    float iRefMaxPu;
    float iacPu;
    float vacNorm;
    float vacAbsFilt;
    float vbusPu;
    UINT16 vLoopDiv;
    UINT16 vLoopDivCnt;
} T_S_LoopCtrl;

static T_S_LoopCtrl g_strLoopCtrl;

static inline float clampf(float val, float minVal, float maxVal)
{
    if(val > maxVal)
    {
        return maxVal;
    }
    if(val < minVal)
    {
        return minVal;
    }
    return val;
}

static inline float PI_RunAw(T_S_PID *pPI, float ref, float fb)
{
    float err = ref - fb;
    float uUnsat = (pPI->Kp * err) + pPI->Inter;
    float uSat = clampf(uUnsat, pPI->Out_Min, pPI->Out_Max);

    // Back-calculation anti-windup: when saturated, pull integrator back.
    pPI->Inter += (pPI->Ki * err) + (0.3f * (uSat - uUnsat));

    pPI->Err = err;
    pPI->DErr = err - pPI->Err_Prev;
    pPI->DOut = uUnsat;
    pPI->Out = uSat;
    pPI->Err_Prev = err;
    pPI->Out_Prev = uSat;
    return uSat;
}




void Temperature_Cal_Init(void)
{

}

void Temperature_Cal(void)
{
//    UINT8 i = 0;
//    UINT16 u16Temp1 = 0;
//    UINT16 u16Temp2 = 0;
//    INT32 i32temp = 0;
//    INT32 avg1 = 0;
//    INT32 avg2 = 0;
//
//    if(0 == g_strSysTimeFlg.TB10ms.bits.b1TemperatureCal)
//    {
//        return;
//    }
//
//    g_strSysTimeFlg.TB10ms.bits.b1TemperatureCal = 0;
//    for (i = (ADC_ARRAY_SIZE - 1); i >= 1; i--)
//    {
//        g_u16Temp1_Array[i] = g_u16Temp1_Array[i - 1];
//        g_u16Temp2_Array[i] = g_u16Temp2_Array[i - 1];
//    }
////    g_strADC.Temperat1_ADC = TMEP1_DET_FB;
////    g_strADC.Temperat2_ADC = TMEP2_DET_FB;
////    g_u16Temp1_Array[0] = TMEP1_DET_FB;
////    g_u16Temp2_Array[0] = TMEP2_DET_FB;
//
//    avg1 = g_u16Temp1_Array[0];
//    avg2 = g_u16Temp2_Array[0];
//    for (i = 1; i < ADC_ARRAY_SIZE; i++)
//    {
//        avg1 = avg1 + g_u16Temp1_Array[i];
//        avg2 = avg2 + g_u16Temp2_Array[i];
//    }
//
//    u16Temp1 = (UINT16)(avg1 >> 2);
//    u16Temp2 = (UINT16)(avg2 >> 2);
//
//    for(i = 150; i > 0; i--)
//    {
//        if ((u16Temp1 < g_i16NTC_Tab[i]) && (u16Temp1 > g_i16NTC_Tab[i + 2]))
//        {
//            i32temp = (INT32)(i - 40);
//            break;
//        }
//    }
//    UPDNLMT32(i32temp, -40, 110);
//    i32temp = i32temp << 12;
//    g_lq15Temperat1.fd = _IQ12toF(i32temp);
//
//    i32temp = 0;
//    for(i = 150; i > 0; i--)
//    {
//        if ((u16Temp2 < g_i16NTC_Tab[i]) && (u16Temp2 > g_i16NTC_Tab[i + 2]))
//        {
//            i32temp = (INT32)(i - 40);
//            break;
//        }
//    }
//    UPDNLMT32(i32temp, -40, 110);
//    i32temp = i32temp << 12;
//
//    g_lq15Temperat2.fd = _IQ12toF(i32temp);
}



//
// Adc_Cal_100KHz - return ADC value of voltage and current
// It used for control loop, so it have to allocate to RAM
// It runs every 10us
//

#pragma CODE_SECTION( Adc_Cal_100KHz, ".TI.ramfunc" );
void inline Adc_Cal_100KHz(void)  // 100Khz 10us
{
    g_strADC.GridVolt_ADC = (long)VAC_FB - ADC_VAC_VOLT_ZERO_1V6;
    g_strADC.Iin_ADC = (long)(IAC_FB) - ADC_VAC_CURR_ZERO_1V5;
    g_strADC.Vbus_ADC = (long)(VDC_FB);
    g_strADC.Iout_ADC = (long)(IDC_FB);

    g_u8IsPositivalHalfFlg = (g_strADC.GridVolt_ADC >= 0) ? 1 : 0;

//    if(g_strADC.GridVolt_ADC >= 10)
//    {
//
//        g_u8IsPositivalHalfFlg = 1;
//    }
//    else
//    {
//        g_u8IsPositivalHalfFlg = 0;
//    }

//    g_lq12GridVolt.lData = _IQmpy(1267, g_strADC.GridVolt_ADC) >> 12;

//    g_lq12GridVolt.lData = _IQ12mpy(IQ12_GRID_VOLT_FACTOR, g_strADC.GridVolt_ADC);
//    g_lq12InputCurrent.lData = _IQ12mpy(IQ12_INNAC_VOLT_FACTOR, g_strADC.Iin_ADC);
//    g_lq12BusVoltage.lData = _IQ12mpy(IQ12_BUS_VOLT_FACTOR, g_strADC.Vbus_ADC);
//    g_lq12OutCurrent.lData = _IQ12mpy(IQ12_INV_CURR_FACTOR, g_strADC.Iout_ADC);

}
//
// Adc_Cal_10KHz - return ADC value of temperate
// It runs every 100us.
//
void Adc_Cal_10KHz(void)
{

//    g_strADC.Temperat1_ADC = (long)(TEMP1_FB);
//    g_strADC.Temperat2_ADC = (long)(TEMP2_FB);

}

//
// GridVoltRmsCal - Calculate the AC voltage RMS
// First evolution, then filter it
// It runs every 1ms.
//
void GridVoltRmsCal(void)
{
    long iq12Temp = 0;
    long iq12TempAccum = 0;
    long iq12TempAvg = 0;

    g_lq12GridVoltRms.fd = sqrtf(g_i32SumReciCnts);

    iq12Temp = (long)g_lq12GridVoltRms.fd;
    iq12TempAccum -= (iq12TempAvg);
    iq12TempAccum += iq12Temp;
    iq12TempAvg = iq12TempAccum;
    g_lq12GridVoltRms.lData = iq12TempAvg;
}

//
// GridPeriodRmsCal - Calculate the AC voltage RMS
// First square of the ADC, then sum of them in one cycle
// It runs every 1ms.
//

void GridPeriodRmsCal(void)
{

    if(0 == g_strSysTimeFlg.TB1ms.bits.b1GridPeriodCal)
    {
        return;
    }

    g_strSysTimeFlg.TB1ms.bits.b1GridPeriodCal = 0;

    g_lq12GridVolt.fd = g_strADC.GridVolt_ADC;
    if(g_u8IsPositivalHalfFlg == 1)
    {
        g_lq2GridVoltRmsSum += g_lq12GridVolt.fd * g_lq12GridVolt.fd;
        g_u16RMSSumCnts++;
    }
    else
    {
        if(g_u16RMSSumCnts > 0)
        {
            g_i32SumReciCnts = g_lq2GridVoltRmsSum / g_u16RMSSumCnts;
        }
        g_u16RMSSumCnts = 0;
        g_lq2GridVoltRmsSum = 0;
    }

    GridVoltRmsCal();

}

//
// VoltLoop_Var_Init - initialize all the various for voltage loop
// It includes Kp and Ki and the limit value of the PI loop
// It runs when DSP initialize
//

void VoltLoop_Var_Init(void)
{
    g_strVLoop.Err = 0;
    g_strVLoop.Out = 0;
    g_strVLoop.DErr = 0;
    g_strVLoop.DOut = 0;
    g_strVLoop.Err_Prev = 0;
    g_strVLoop.Out_Prev = 0;
    // Outer loop works at 10kHz equivalent (100kHz ISR with /10 decimation).
    g_strVLoop.Kp = 0.06f;
    g_strVLoop.Ki = 0.0015f;
    g_strVLoop.Inter = 0;
    g_strVLoop.Out_Max = 0.90f;
    g_strVLoop.Out_Min = 0.0;

    g_strLoopCtrl.vLoopDiv = 10;
    g_strLoopCtrl.vLoopDivCnt = 0;
    g_strLoopCtrl.vbusSoftRefPu = 0.10f;
    g_strLoopCtrl.iRefMaxPu = 0.90f;
    g_strLoopCtrl.iRefPu = 0.0f;
    g_strLoopCtrl.iacPu = 0.0f;
    g_strLoopCtrl.vacNorm = 0.0f;
    g_strLoopCtrl.vacAbsFilt = 100.0f;
    g_strLoopCtrl.vbusPu = 0.0f;
}

float g_i32Vref = 0.8;
float g_i32Vadc = 0.0;

//
// VoltLoopCtrl - runs voltage loop
// Transfer the ADC of bus voltage to PI loop and compute the result value
// It used for control loop, so it have to allocate to RAM
// It runs every 10us
//

#pragma CODE_SECTION( VoltLoopCtrl, ".TI.ramfunc" );
void VoltLoopCtrl(void)
{

//    g_strADC.RefVolt_ADC = (((g_strADC.RefVolt_ADC * 205) + ((UINT32)VREF_1V5_FB * 819)) >> 10);

//    g_lq12BusVoltRef.lData = g_i32Vref;//_IQ12(400);
    //It means g_lq12BusVolt.lData must to convert to real voltage then left move 12 bit
    //In this project, Vbus/AD = 0.208221
    //So IQ12_BUS_VOLT_FACTOR = 0.208221 << 12 << 12 = 3493374
    g_strADC.BusVolt_ADC = VDC_FB;
    g_lq12BusVolt.lData = g_strADC.BusVolt_ADC;
    g_strLoopCtrl.vbusPu = clampf(((float)g_strADC.BusVolt_ADC) / 4000.0f, 0.0f, 1.2f);
    if(g_lq12BusVoltRef.lData > 0)
    {
        g_i32Vref = clampf(((float)g_lq12BusVoltRef.lData) / ((float)IQ12_VBUS_400V), 0.20f, 0.95f);
    }
//    g_i32Vadc = g_strADC.BusVolt_ADC / 4000;

    if(g_unPWMDriverFlg.bits.b1LoopClose)
    {
        g_strLoopCtrl.vLoopDivCnt++;
        if(g_strLoopCtrl.vLoopDivCnt >= g_strLoopCtrl.vLoopDiv)
        {
            g_strLoopCtrl.vLoopDivCnt = 0;

            // Soft ramp bus reference to reduce inrush and overshoot in startup.
            if(g_strLoopCtrl.vbusSoftRefPu < g_i32Vref)
            {
                g_strLoopCtrl.vbusSoftRefPu += 0.0005f;
            }
            else
            {
                g_strLoopCtrl.vbusSoftRefPu = g_i32Vref;
            }

            g_strVLoop.Out = PI_RunAw(&g_strVLoop,
                                       g_strLoopCtrl.vbusSoftRefPu,
                                       g_strLoopCtrl.vbusPu);
        }

    }

//    g_strVLoop.Out = g_strVLoop.Out >> 15;
}

float g_i32Iref = 0;
INT32 g_i32Iref1 = 0;
INT32 g_i32Iref2 = 0;
float g_i32Iref3 = 0;
float g_i32Iref4 = 0;
float g_i32Iref5 = 11.6f;
float Arg = 0;//_IQ15(0.0);

//
// Iref_Cal - compute the AC input current loop referense
// Transfer the result of voltage loop, and multiplied by AC sinx
// Then it can be followed by the AC voltage wave
// It used for control loop, so it have to allocate to RAM
// It runs every 10us
//

#pragma CODE_SECTION( Iref_Cal, ".TI.ramfunc" );
void Iref_Cal(void)
{
    g_unPWMDriverFlg.bits.b1LoopClose = 1;
    if(g_unPWMDriverFlg.bits.b1LoopClose)
    {
        float vacAbs;

        vacAbs = fabsf((float)g_strADC.GridVolt_ADC);
        g_strLoopCtrl.vacAbsFilt += 0.01f * (vacAbs - g_strLoopCtrl.vacAbsFilt);
        g_strLoopCtrl.vacNorm = clampf(((float)g_strADC.GridVolt_ADC) / (g_strLoopCtrl.vacAbsFilt + 1.0f),
                                       -1.0f,
                                       1.0f);

        g_strLoopCtrl.iRefPu = g_strVLoop.Out * g_strLoopCtrl.vacNorm;
        g_strLoopCtrl.iRefPu = clampf(g_strLoopCtrl.iRefPu,
                                      -g_strLoopCtrl.iRefMaxPu,
                                      g_strLoopCtrl.iRefMaxPu);

        g_i32Iref4 = g_strLoopCtrl.iRefPu;
    }
}

//
// CurrLoop_Var_Init - initialize all the various for current loop
// It includes Kp and Ki and the limit value of the PI loop
// It runs when DSP initialize
//

void CurrLoop_Var_Init(void)
{
    g_strILoop.Err = 0;
    g_strILoop.Out = 0;
    g_strILoop.DErr = 0;
    g_strILoop.DOut = 0;
    g_strILoop.Err_Prev = 0;
    g_strILoop.Out_Prev = 0;
    g_strILoop.Kp = 0.25f;
    g_strILoop.Ki = 0.008f;
    g_strILoop.Inter = 0;
    g_strILoop.Out_Max = 0.95f;
    g_strILoop.Out_Min = 0.05f;
}

float g_i32Iadc = 0.0;

//
// CurrLoopCtrl - runs current loop
// Transfer the ADC of inductance current to PI loop and compute the result value
// It used for control loop, so it have to allocate to RAM
// It runs every 10us
//

#pragma CODE_SECTION( CurrLoopCtrl, ".TI.ramfunc" );
void CurrLoopCtrl(void)
{
    g_unPWMDriverFlg.bits.b1LoopClose = 1;
    if(g_unPWMDriverFlg.bits.b1LoopClose)
    {
        g_strADC.Iin_ADC = (long)(IAC_FB) - ADC_VAC_CURR_ZERO_1V5;
        g_strLoopCtrl.iacPu = clampf(((float)g_strADC.Iin_ADC) / 3000.0f, -1.2f, 1.2f);
        g_lq12InputCurrent.fd = g_strLoopCtrl.iacPu;

        g_i32Iadc = g_strLoopCtrl.iacPu;
        g_strILoop.Out = PI_RunAw(&g_strILoop, g_strLoopCtrl.iRefPu, g_strLoopCtrl.iacPu);
    }
}

//
// PWMDriver_Var_Init - initialize all the various for PWM driver
// It runs when DSP initialize
//

void PWMDriver_Var_Init(void)
{
    g_unPWMDriverFlg.bits.b1LoopClose = 0;
    g_u8PFCPWMSwState = PWM_SWSTATE_NORM_OPT;
}


//
// PWMDriver_Ctrl - state machine for PWN logic
// It include the cross zero control and duty of position and negative half cycle
// It used for control loop, so it have to allocate to RAM
// It runs every 10us

#pragma CODE_SECTION( PWMDriver_Ctrl, ".TI.ramfunc" );
void PWMDriver_Ctrl(void)
{
#if  PFC_CLOSE_LOOP
    g_u32HFPWMDuty = PWM_DUTY_MAX * g_strILoop.Out;
#else

#endif

//#ifdef DEBUG_ONLY

//    static uint32_t u32ACCycle = 0;

    switch(g_u8PFCPWMSwState)
        {
            case PWM_SWSTATE_NORM_OPT:
                if(g_strADC.GridVolt_ADC >= 20 && g_u8GridStatus == POSITION_CROSS_ZERO)
                {
                    g_unPWMDriverFlg.bits.b1LoopClose = 1;
                    g_u8PFCPWMSwState = PWM_SWSTATE_POS_HALF;
                }
                break;

            case PWM_SWSTATE_POS_HALF:
                EPWM_setActionQualifierContSWForceAction(EPWM2_BASE,
                                                         EPWM_AQ_OUTPUT_A,
                                                         EPWM_AQ_SW_OUTPUT_HIGH);
                EPWM_setActionQualifierContSWForceAction(EPWM2_BASE,
                                                         EPWM_AQ_OUTPUT_B,
                                                         EPWM_AQ_SW_OUTPUT_LOW);
                EPWM_setCounterCompareValue(EPWM1_BASE, EPWM_COUNTER_COMPARE_A, g_u32HFPWMDuty);
//                EPWM_setCounterCompareValue(EPWM1_BASE, EPWM_COUNTER_COMPARE_B, 250);
//                EPWM_setDeadBandDelayMode(EPWM1_BASE, EPWM_DB_RED, true);
//                EPWM_setDeadBandDelayMode(EPWM1_BASE, EPWM_DB_FED, true);
//                u32ACCycle++;
////                if(u32ACCycle >= 1000)
//                if(g_u8IsZeroCrossingFlg == 1)
//                {
//                    u32ACCycle = 0;
//                    g_u8PFCPWMSwState = PWM_SWSTATE_NEG_ZC1;
//
//                }

                if(g_strADC.GridVolt_ADC < 20)
                {
                    g_u8PFCPWMSwState = PWM_SWSTATE_NEG_ZC1;
                }
                break;

            case PWM_SWSTATE_NEG_ZC1:
                EPWM_setActionQualifierContSWForceAction(EPWM2_BASE,
                                                         EPWM_AQ_OUTPUT_A,
                                                         EPWM_AQ_SW_OUTPUT_LOW);
                EPWM_setActionQualifierContSWForceAction(EPWM1_BASE,
                                                         EPWM_AQ_OUTPUT_A,
                                                         EPWM_AQ_SW_OUTPUT_LOW);
                EPWM_setActionQualifierContSWForceAction(EPWM1_BASE,
                                                         EPWM_AQ_OUTPUT_B,
                                                         EPWM_AQ_SW_OUTPUT_LOW);
                EPWM_setRisingEdgeDelayCount(EPWM1_BASE, 0);
                EPWM_setFallingEdgeDelayCount(EPWM1_BASE, 0);
                EPWM_setDeadBandDelayMode(EPWM1_BASE, EPWM_DB_RED, false);
                EPWM_setDeadBandDelayMode(EPWM1_BASE, EPWM_DB_FED, false);
                {
                    g_u8PFCPWMSwState = PWM_SWSTATE_NEG_ZC2;
                }
                break;

            case PWM_SWSTATE_NEG_ZC2:
                {

                    EPWM_setCounterCompareValue(EPWM1_BASE, EPWM_COUNTER_COMPARE_A, 500);
                    EPWM_setActionQualifierContSWForceAction(EPWM1_BASE,
                                                             EPWM_AQ_OUTPUT_A,
                                                             EPWM_AQ_SW_DISABLED);
                    g_u16PFCPWMDriverStateSlew = 0;
                    g_u16PFCSoftStartDuty = 0;
                    g_u8PFCPWMSwState = PWM_SWSTATE_NEG_ZC3;
                }
                break;

            case PWM_SWSTATE_NEG_ZC3:
                g_u16PFCPWMDriverStateSlew++;
                g_u16PFCSoftStartDuty = 500 - g_u16PFCPWMDriverStateSlew;
//                if(g_u16PFCPWMDriverStateSlew > 10)
                if(g_strADC.GridVolt_ADC >= 20 && g_u8GridStatus == NEGTIVE_CROSS_ZERO)
                {
                    EPWM_setActionQualifierContSWForceAction(EPWM1_BASE,
                                                             EPWM_AQ_OUTPUT_B,
                                                             EPWM_AQ_SW_DISABLED);

                    g_u8PFCPWMSwState = PWM_SWSTATE_NEG_HALF;
                    g_u16PFCPWMDriverStateSlew = 0;
                }
                else
                {
//                    EPWM_setDeadBandDelayMode(EPWM1_BASE, EPWM_DB_RED, false);
//                    EPWM_setDeadBandDelayMode(EPWM1_BASE, EPWM_DB_FED, false);
                    EPWM_setCounterCompareValue(EPWM1_BASE, EPWM_COUNTER_COMPARE_A, g_u16PFCSoftStartDuty);


                }
                break;

            case PWM_SWSTATE_NEG_HALF:
                EPWM_setActionQualifierContSWForceAction(EPWM2_BASE,
                                                         EPWM_AQ_OUTPUT_A,
                                                         EPWM_AQ_SW_OUTPUT_LOW);
                EPWM_setActionQualifierContSWForceAction(EPWM2_BASE,
                                                         EPWM_AQ_OUTPUT_B,
                                                         EPWM_AQ_SW_OUTPUT_HIGH);

                EPWM_setCounterCompareValue(EPWM1_BASE, EPWM_COUNTER_COMPARE_A, g_u32HFPWMDuty);
//                EPWM_setCounterCompareValue(EPWM1_BASE, EPWM_COUNTER_COMPARE_B, 250);

//                u32ACCycle++;
//                if(u32ACCycle >= 500)
//                {
//                    u32ACCycle = 0;
//                    g_u8PFCPWMSwState = PWM_SWSTATE_POS_ZC1;
//
//                }
                if(g_strADC.GridVolt_ADC < 20)
                {
                    g_u8PFCPWMSwState = PWM_SWSTATE_POS_ZC1;
                }

                break;

            case PWM_SWSTATE_POS_ZC1:
                EPWM_setActionQualifierContSWForceAction(EPWM1_BASE,
                                                         EPWM_AQ_OUTPUT_A,
                                                         EPWM_AQ_SW_OUTPUT_LOW);
                EPWM_setActionQualifierContSWForceAction(EPWM1_BASE,
                                                         EPWM_AQ_OUTPUT_B,
                                                         EPWM_AQ_SW_OUTPUT_LOW);
                EPWM_setRisingEdgeDelayCount(EPWM1_BASE, 0);
                EPWM_setFallingEdgeDelayCount(EPWM1_BASE, 0);
                EPWM_setDeadBandDelayMode(EPWM1_BASE, EPWM_DB_RED, false);
                EPWM_setDeadBandDelayMode(EPWM1_BASE, EPWM_DB_FED, false);
//                if(u32ACCycle == 26)
                {
                    g_u8PFCPWMSwState = PWM_SWSTATE_POS_ZC2;
                    g_u16PFCSoftStartDuty = 0;
                }
                break;

            case PWM_SWSTATE_POS_ZC2:
//                if(u32ACCycle == 27)
                {
                    EPWM_setCounterCompareValue(EPWM1_BASE, EPWM_COUNTER_COMPARE_A, 0);
                    EPWM_setActionQualifierContSWForceAction(EPWM1_BASE,
                                                             EPWM_AQ_OUTPUT_B,
                                                             EPWM_AQ_SW_DISABLED);
                    g_u8PFCPWMSwState = PWM_SWSTATE_POS_ZC3;
                    g_u16PFCPWMDriverStateSlew = 0;
                    g_u16PFCSoftStartDuty = 0;
                }
                break;

            case PWM_SWSTATE_POS_ZC3:
                g_u16PFCPWMDriverStateSlew++;
                g_u16PFCSoftStartDuty = g_u16PFCPWMDriverStateSlew;
//                if(g_u16PFCPWMDriverStateSlew > 10 && g_strADC.GridVolt_ADC >= 20)
                if(g_strADC.GridVolt_ADC >= 20 && g_u8GridStatus == POSITION_CROSS_ZERO)
                {
                    EPWM_setActionQualifierContSWForceAction(EPWM1_BASE,
                                                             EPWM_AQ_OUTPUT_A,
                                                             EPWM_AQ_SW_DISABLED);

                    g_u8PFCPWMSwState = PWM_SWSTATE_POS_HALF;
                    g_u16PFCPWMDriverStateSlew = 0;
//                    u32ACCycle = 0;
                }
                else
                {
//                    EPWM_setDeadBandDelayMode(EPWM1_BASE, EPWM_DB_RED, false);
//                    EPWM_setDeadBandDelayMode(EPWM1_BASE, EPWM_DB_FED, false);
                    EPWM_setCounterCompareValue(EPWM1_BASE, EPWM_COUNTER_COMPARE_A, g_u16PFCSoftStartDuty);
                }
                break;

            default:
                g_u8PFCPWMSwState = PWM_SWSTATE_DEFSTATUS;
                break;
        }

}





