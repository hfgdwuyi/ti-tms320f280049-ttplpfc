#ifndef _INITIALIZE_H_
#define _INITIALIZE_H_


//#include "DSP28x_Project.h"
#include "Macro.h"
#include "Constant.h"
#include "Interrupts.h"
#include "InitDevice.h"
#include "State.h"
#include "device.h"
//#include "F28x_Project.h"

// ADC SOC0-SOC15¿ØÖÆ¼Ä´æÆ÷(ADCSOCxCTL)×Ö¶ÎÃèÊö  ----- TRIGSEL
#define ADCTRIG_SOFT                        0
#define ADCTRIG_CPU_TINT0                   1
#define ADCTRIG_CPU_TINT1                   2
#define ADCTRIG_CPU_TINT2                   3
#define ADCTRIG_XINT2                       4
#define ADCTRIG_EPWM1_SOCA                  5
#define ADCTRIG_EPWM1_SOCB                  6
#define ADCTRIG_EPWM2_SOCA                  7
#define ADCTRIG_EPWM2_SOCB                  8
#define ADCTRIG_EPWM3_SOCA                  9
#define ADCTRIG_EPWM3_SOCB                  10
#define ADCTRIG_EPWM4_SOCA                  11
#define ADCTRIG_EPWM4_SOCB                  12
#define ADCTRIG_EPWM5_SOCA                  13
#define ADCTRIG_EPWM5_SOCB                  14
#define ADCTRIG_EPWM6_SOCA                  15
#define ADCTRIG_EPWM6_SOCB                  16
#define ADCTRIG_EPWM7_SOCA                  17
#define ADCTRIG_EPWM7_SOCB                  18

#define ADC_ACQPS_CLK                       0x8

#define ADCINA0_VAC_FB                      (0x00)          // ADCINA0 -- VAC_SENS_DSP
#define ADCINA1_IDC1_FB                     (0x01)          // ADCINA1 -- IDC_SENS_DSP
#define ADCINA2_IAC_FB                      (0x02)          // ADCINA2 -- IAC_SENS_DSP
#define ADCINA3_VDC_FB                      (0x03)          // ADCINA3 -- VDC_SENS_DSP
#define ADCINA4_IDC2_FB                     (0x04)          // ADCINA4 -- IDC2_SENS_DSP
#define ADCINA5_TEMP1_FB                    (0x05)          // ADCINA5 -- TMEP1_DET_DSP
#define ADCINA6_TEMP2_FB                    (0x06)          // ADCINA6 -- TMEP1_DET_DSP
#define ADCINA8_VAC_L_FB                    (0x08)          // ADCINA8 -- VAC_L_SENS_DSP
#define ADCINA9_VAC_N_FB                    (0x09)          // ADCINA9 -- VAC_N_SENS_DSP

#define VAC_FB                              ADC_readResult(ADCARESULT_BASE, ADC_SOC_NUMBER0)
#define IAC_FB                              ADC_readResult(ADCARESULT_BASE, ADC_SOC_NUMBER2)
#define VDC_FB                              ADC_readResult(ADCARESULT_BASE, ADC_SOC_NUMBER3)
#define IDC_FB                              ADC_readResult(ADCARESULT_BASE, ADC_SOC_NUMBER4)
#define TEMP1_FB                            ADC_readResult(ADCARESULT_BASE, ADC_SOC_NUMBER5)
#define TEMP2_FB                            ADC_readResult(ADCARESULT_BASE, ADC_SOC_NUMBER6)
#define VAC_L_FB                            ADC_readResult(ADCARESULT_BASE, ADC_SOC_NUMBER7)
#define VAC_N_FB                            ADC_readResult(ADCARESULT_BASE, ADC_SOC_NUMBER8)


//-----------------------------------------------------------------------------
// GPIO
// OUTPUT

#define EPWM2_ENABLE()                      {EALLOW; EPwm2Regs.AQCSFRC.all = 0x00; EDIS;}
#define EPWM2_DISABLE()                     {EALLOW; EPwm2Regs.AQCSFRC.all = 0x05; EDIS;}
#define EPWM2_A_DUTY_SET(cmpa)              {EPwm2Regs.CMPA.half.CMPA = cmpa;}
#define EPWM2_B_DUTY_SET(cmpb)              {EPwm2Regs.CMPB = cmpb;}
#define EPWM2_A_HIGH_B_LOW()                {EALLOW; EPwm2Regs.AQCSFRC.all = 0x06; EDIS;}
#define EPWM2_A_LOW_B_HIGH()                {EALLOW; EPwm2Regs.AQCSFRC.all = 0x09; EDIS;}
#define EPWM2_A_LOW_B_LOW()                 {EALLOW; EPwm2Regs.AQCSFRC.all = 0x05; EDIS;}
#define EPWM2_ENABLE_DB()                   {EALLOW; EPwm2Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE; EDIS;}
#define EPWM2_DISABLE_DB()                  {EALLOW; EPwm2Regs.DBCTL.bit.OUT_MODE = DB_DISABLE; EDIS;}

#define EPWM1_ENABLE_OUTPUT()               {EALLOW; EPwm1Regs.AQCSFRC.all = 0x00; EDIS;}
#define EPWM1_DISABLE_OUTPUT()              {EALLOW; EPwm1Regs.AQCSFRC.all = 0x05; EDIS;}
#define EPWM1_A_HIGH_B_LOW()                {EALLOW; EPwm1Regs.AQCSFRC.all = 0x06; EDIS;}
#define EPWM1_A_LOW_B_HIGH()                {EALLOW; EPwm1Regs.AQCSFRC.all = 0x09; EDIS;}
#define EPWM1_A_LOW_B_LOW()                 {EALLOW; EPwm1Regs.AQCSFRC.all = 0x05; EDIS;}
#define EPWM1_A_FORCES_LOW()                {EALLOW; EPwm1Regs.AQCSFRC.bit.CSFA = AQ_CLEAR; EDIS;}
#define EPWM1_B_FORCES_LOW()                {EALLOW; EPwm1Regs.AQCSFRC.bit.CSFB = AQ_CLEAR; EDIS;}
#define EPWM1_A_FORCES_HIGH()               {EALLOW; EPwm1Regs.AQCSFRC.bit.CSFA = AQ_SET; EDIS;}
#define EPWM1_B_FORCES_HIGH()               {EALLOW; EPwm1Regs.AQCSFRC.bit.CSFB = AQ_SET; EDIS;}
#define EPWM1_ENABLE_SWAP_DB_OUTPUT()       {EALLOW; EPwm1Regs.HRCNFG.bit.SWAPAB = 1; EDIS;}
#define EPWM1_DISABLE_SWAP_DB_OUTPUT()      {EALLOW; EPwm1Regs.HRCNFG.bit.SWAPAB = 0; EDIS;}
#define EPWM1_ENABLE_DB()                   {EALLOW; EPwm1Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE; EDIS;}
#define EPWM1_DISABLE_DB()                  {EALLOW; EPwm1Regs.DBCTL.bit.OUT_MODE = DB_DISABLE; EDIS;}
#define EPWM1_A_DUTY_SET(cmpa)              {EPwm1Regs.CMPA.all = ((UINT32)cmpa << 8);}
#define EPWM1_B_DUTY_SET(cmpb)              {EPwm1Regs.CMPB = cmpb;}
#define EPWM1_SET_PWM_DBRED(dbred)          {EPwm1Regs.DBRED.all = dbred;}
#define EPWM1_SET_PWM_DBFED(dbfed)          {EPwm1Regs.DBFED.all = dbfed;}


#define EPWM3_ENABLE()                      {EALLOW; EPwm3Regs.AQCSFRC.all = 0x00; EDIS;}
#define EPWM3_DISABLE()                     {EALLOW; EPwm3Regs.AQCSFRC.all = 0x05; EDIS;}
#define EPWM3_A_DUTY_SET(cmpa)              {EPwm3Regs.CMPA.half.CMPA = cmpa;}
#define EPWM3_SET_PWM_DBRED(dbred)          {EALLOW; EPwm3Regs.DBRED = dbred; EDIS;}
#define EPWM3_SET_PWM_DBFED(dbfed)          {EALLOW; EPwm3Regs.DBFED = dbfed; EDIS;}
#define EPWM3_ENABLE_DB()                   {EALLOW; EPwm3Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE; EDIS;}
#define EPWM3_DISABLE_DB()                   {EALLOW; EPwm3Regs.DBCTL.bit.OUT_MODE = DB_DISABLE; EDIS;}

#define EPWM4_ENABLE()                      {EALLOW; EPwm4Regs.AQCSFRC.all = 0x00; EDIS;}
#define EPWM4_DISABLE()                     {EALLOW; EPwm4Regs.AQCSFRC.all = 0x05; EDIS;}

#define EN_FAN_VCC_PWM()                    {EPWM4_ENABLE(); g_strPFCStatus.StateFlg.bits.b1FanVccEn = 1;}
#define DIS_FAN_VCC_PWM()                   {EPWM4_DISABLE(); g_strPFCStatus.StateFlg.bits.b1FanVccEn = 0;}


#define GPIO_EPWM1A                         0
#define GPIO_EPWM1B                         1
#define GPIO_EPWM2A                         2
#define GPIO_EPWM2B                         3

#define GPIO_OUT_LOW                        0
#define GPIO_OUT_HIGH                       1

// =============================================================================


extern void InitDSP_Device(void);
extern void Kick_WatchDog(void);

#endif

