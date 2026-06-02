#ifndef _CONSTANT_H_
#define _CONSTANT_H_

#include "DataType.h"


#define PFC_SOFT_VERSION                "V1.00.00"

//------------------------------------------------------------------------------
// define constant for time
//------------------------------------------------------------------------------
// 1us时基
#define DELAYB1US_5MS                   ((UINT16)5500)          // 5ms

// 1ms时基
#define DELAYB1MS_2MS                   ((UINT16)2)             // 2ms
#define DELAYB1MS_2S                    ((UINT16)2000)          // 2s

// 2ms时基
#define DELAYB2MS_2MS                   ((UINT16)1)             // 2ms
#define DELAYB2MS_4MS                   ((UINT16)2)             // 4ms
#define DELAYB2MS_10MS                  ((UINT16)5)             // 10ms
#define DELAYB2MS_20MS                  ((UINT16)10)            // 20ms
#define DELAYB2MS_50MS                  ((UINT16)25)            // 50ms
#define DELAYB2MS_100MS                 ((UINT16)50)            // 100ms
#define DELAYB2MS_500MS                 ((UINT16)250)           // 500ms
#define DELAYB2MS_1S                    ((UINT16)500)           // 1s
#define DELAYB2MS_2S                    ((UINT16)1000)          // 2s
#define DELAYB2MS_5S                    ((UINT16)2500)          // 5s
#define DELAYB2MS_10S                   ((UINT16)5000)          // 10s
#define DELAYB2MS_30S                   ((UINT16)15000)         // 30s
#define DELAYB2MS_60S                   ((UINT16)30000)         // 60s

//------------------------------------------------------------------------------
// STATE
//------------------------------------------------------------------------------
#define SYSSTATE_INIT                   ((UINT8)0x00)
#define SYSSTATE_SWITCH                 ((UINT8)0x01)
#define SYSSTATE_STANDBY                ((UINT8)0x02)
#define SYSSTATE_ACDC                   ((UINT8)0x03)
#define SYSSTATE_DCAC                   ((UINT8)0x04)
#define SYSSTATE_UPDATE                 ((UINT8)0x05)


#define SUBSTATE_ACDC_INIT              ((UINT8)0x00)
#define SUBSTATE_ACDC_STANDBY           ((UINT8)0x01)
#define SUBSTATE_ACDC_SOFTSTART         ((UINT8)0x02)
#define SUBSTATE_ACDC_NORMAL            ((UINT8)0x03)
#define SUBSTATE_ACDC_SHUTDOWN          ((UINT8)0x04)
#define SUBSTATE_ACDC_FAULT             ((UINT8)0x05)

#define SUBSTATE_DCAC_INIT              ((UINT8)0x00)
#define SUBSTATE_DCAC_STANDBY           ((UINT8)0x01)
#define SUBSTATE_DCAC_SOFTSTART         ((UINT8)0x02)
#define SUBSTATE_DCAC_NORMAL            ((UINT8)0x03)
#define SUBSTATE_DCAC_SHUTDOWN          ((UINT8)0x04)
#define SUBSTATE_DCAC_FAULT             ((UINT8)0x05)

#define PWM_SWSTATE_NORM_OPT            ((UINT8)0x00)
#define PWM_SWSTATE_POS_HALF            ((UINT8)0x01)
#define PWM_SWSTATE_NEG_ZC1             ((UINT8)0x02)
#define PWM_SWSTATE_NEG_ZC2             ((UINT8)0x03)
#define PWM_SWSTATE_NEG_ZC3             ((UINT8)0x04)
#define PWM_SWSTATE_NEG_HALF            ((UINT8)0x05)
#define PWM_SWSTATE_POS_ZC1             ((UINT8)0x06)
#define PWM_SWSTATE_POS_ZC2             ((UINT8)0x07)
#define PWM_SWSTATE_POS_ZC3             ((UINT8)0x08)
#define PWM_SWSTATE_DEFSTATUS           ((UINT8)0x09)

//------------------------------------------------------------------------------
// Epwm
//------------------------------------------------------------------------------
#define EPWM_TBPRD_300KHZ               ((UINT16)100)       // 配置周期值 60M/2/300k = 100
#define EPWM_TBPRD_100KHZ               ((UINT16)500)       // 配置周期值 60M/2/300k = 100
#define EPWM_TBPRD_65KHZ                ((UINT16)923)       // 配置周期值 60M/2/65k = 461
#define EPWM_TBPRD_10KHZ                ((UINT16)3000)      // 配置周期值 60M/2/10k = 3000

#define PWM_DUTY_DEF                    ((UINT16)0x0000)
#define PWM_DUTY_MAX                    450//((UINT16)0x0000)
#define PWM_DUTY_MIN                    ((UINT16)0x0000)

#define EPWM1_DBT_300NS                 ((UINT16)18)        // 死区 18*1/100MHz = 180ns
#define EPWM1_DBT_200NS                 ((UINT16)12)        // 死区 12*1/100MHz = 120ns
#define EPWM1_DBT_FULL_TBPRD            ((UINT16)923)

#define ISR_EPWM_FREQ                   100000
#define ISR_EPWM_PRD                    0.00001  // ts = 1 / 100000

//------------------------------------------------------------------------------
// Sample Coeff
//------------------------------------------------------------------------------
#define ADC_ARRAY_SIZE                  ((UINT8)4)

#define ADC_VAC_VOLT_ZERO_1V6           ((INT32)2048)      // 1.65V
#define ADC_VAC_VOLT_ZERO_1V5           ((INT32)1893)      // 1.525V
#define ADC_VAC_CURR_ZERO_1V5           ((INT32)1893)      // 1.541V

#define IQ12_INNAC_VOLT_FACTOR          ((INT32)4150077)      // 0.003257
#define IQ12_GRID_VOLT_FACTOR           ((INT32)5190783)      // 0.002604
#define IQ12_INV_VOLT_FACTOR            ((INT32)4811961)      // 0.002809
#define IQ12_BUS_VOLT_FACTOR            674//((INT32)2759093)   // 0.004899

#define IQ12_INNAC_CURR_FACTOR          ((INT32)943054)     // 0.014333
#define IQ12_INV_CURR_FACTOR            ((INT32)901120)     // 0.015

#define ADC_CAL_RMS_CNT                 ISR_EPWM_FREQ / 100
//------------------------------------------------------------------------------
// define constant for ac
//------------------------------------------------------------------------------
#define IQ12_VACRMS_SOFT_STEP           ((INT32)6554)    // IQ15 0.2 BUS 输出软启步进

#define CYCLE_NONE                      ((UINT8)0x00)
#define CYCLE_POSITIVE                  ((UINT8)0x01)
#define CYCLE_NEGATIVE                  ((UINT8)0x02)

#define GRID_AC_FREQ                    60
#define F_PI                            3.14159265f
#define F_2PI                           2 * F_PI
//------------------------------------------------------------------------------
//define constant for bus
//------------------------------------------------------------------------------
#define IQ12_VBUS_20V                   (((INT32)20) << 12)
#define IQ12_VBUS_380V                  (((INT32)380) << 12)
#define IQ12_VBUS_400V                  (((INT32)400) << 12)

#define IQ12_VBUS_SOFT_STEP             ((INT32)819)    // IQ15 0.2 BUS 输出软启步进

#define SOFTSTART_UP_COUNT              ((UINT16)200)


//------------------------------------------------------------------------------
// PLL
//------------------------------------------------------------------------------

#define PLL_B0_LPF                      (166.7618719)   // (222.3540051)//
#define PLL_B1_LPF                      (-166.4785831)  // (-221.9666016)//



//------------------------------------------------------------------------------
// define constant for sci
//------------------------------------------------------------------------------
#define SCI_BAUDRATE_4800               ((INT16)0x186)  // 60M / 4 / ((0x186 + 1) * 8) = 4800
#define SCI_BAUDRATE_9600               ((INT16)0x0C3)  // 60M / 4 / ((0x0C3 + 1) * 8) = 9600

//SCI通信地址定义
#define SCI_MASTER_ADDR_U2              1
#define SCI_SLAVE_ADDR_U1               2
#define SCI_SLAVE_ADDR_PC               3

#define SCI_RX_BUFF_LEN                 32
#define SCI_TX_BUFF_LEN                 4

#define SCI_BUFF_LEN                    64

//信息帧类型
#define SCI_FRAME_RERUPDATE             0x79
#define SCI_FRAME_UPDATECOMMAND         0x80
#define SCI_FRAME_HANDSHAKE             0x81
#define SCI_FRAME_CSMUNLOCKSTATUS       0x82
#define SCI_FRAME_APIVERSIONSTATUS      0x83
#define SCI_FRAME_ERASE                 0x84
#define SCI_FRAME_PROGRAM               0x85
#define SCI_FRAME_BLOCKHEAD             0x86
#define SCI_FRAME_BLOCKDATA             0x87
#define SCI_FRAME_CHECKSUMBLOCK         0x88
#define SCI_FRAME_PROGRAMSTATUS         0x89
#define SCI_FRAME_CHECKSUMALL           0x8A
#define SCI_FRAME_UPDATEEND             0x8B

//DSP接收缓冲区大小
#define DSP_FLASH_BUFFER_SIZE           ((UINT16)0x200)     // 512 Words
#define DSP_FLASH_DATA_ADDR             ((UINT32)0x20000)

//SCI bootloader标志存储起始地址
#define FLASH_SCIUPFLAG_START_ADDR      ((UINT32)0x30200)


//------------------------------------------------------------------------------
// define constant for fault
//------------------------------------------------------------------------------
#define VGRID_LOSS_CHK_CNT              130     // 130 * (60M / 65k) = 0.002
#define VGRID_PEAK_OVP_CHK_CNT          3
#define VBUS_PEAK_OVP_CHK_CNT           6

#define IGRID_OC_CHK_CNT                13000   // 130 * (60M / 65k) = 0.2
#define IGRID_OC_CHK_BACK_CNT           65000   // 65000 * (60M / 65k) = 1

#define TEMPERAT_OTP_CHK_CNT            2000   // 100 * 1MS = 2S
#define TEMPERAT_OTP_CHK_BACK_CNT       2000   // 100 * 1MS = 2S
#define TEMPERAT_UTP_CHK_CNT            2000   // 100 * 1MS = 2S
#define TEMPERAT_UTP_CHK_BACK_CNT       2000   // 100 * 1MS = 2S

#define IQ15_VGRID_LOSS_H_LMT           _IQ15(20.0)
#define IQ15_VGRID_LOOS_L_LMT           _IQ15(-20.0)
#define IQ15_VGRID_PEAK_OVP_LMT         _IQ15(500.0)
#define IQ15_VBUS_FAST_OVP_LMT         _IQ15(500.0)

#define IQ15_VGRID_H_LMT                _IQ15(265.0)
#define IQ15_VGRID_L_LMT                _IQ15(80.0)

#define IQ15_VGRID_H_BACK_LMT           (((INT32)265) << 15)
#define IQ15_VGRID_L_BACK_LMT           (((INT32)80) << 15)


#define DEV_ERROR_NONE                  ((UINT16)0x0000)
#define DEV_ERROR_NO_VAC                ((UINT16)0x0001)
#define DEV_ERROR_VAC_ZERO              ((UINT16)0x0002)
#define DEV_ERROR_OV_AC                 ((UINT16)0x0004)
#define DEV_ERROR_UV_AC                 ((UINT16)0x0008)
#define DEV_ERROR_HZ                    ((UINT16)0x0010)    // 电网频率异常
#define DEV_ERROR_OV_DC                 ((UINT16)0x0020)
#define DEV_ERROR_OTP1                  ((UINT16)0x0040)
#define DEV_ERROR_UTP1                  ((UINT16)0x0080)
#define DEV_ERROR_OTP2                  ((UINT16)0x0100)
#define DEV_ERROR_UTP2                  ((UINT16)0x0200)
#define DEV_ERROR_COMMU                 ((UINT16)0x0400)

//------------------------------------------------------------------------------
// end
//------------------------------------------------------------------------------

#endif
