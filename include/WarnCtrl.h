#ifndef _WARN_CTRL_H_
#define _WARN_CTRL_H_

#include "DataType.h"
#include "Calculare.h"
#include "State.h"
#include "TimeBase.h"
#include "InitDevice.h"

typedef union
{
    INT32 Data[20];
    struct
    {
        INT32 VgridRmsOVPLmt;
        INT32 VgridRmsOVPBackLmt;
        INT32 VgridRmsUVPLmt;
        INT32 VgridRmsUVPBackLmt;

        INT32 IgridRmsOCPLmt;
        INT32 IgridRmsOCPBackLmt;

        INT32 VgridLossHLmt;
        INT32 VgridLossLLmt;

        INT32 VbusOVPLmt;
        INT32 VbusUVPLmt;
        INT32 VbusOVPBackLmt;
        INT32 VbusUVPBackLmt;

        INT32 Temp1OTPLmt;
        INT32 Temp1OTPBackLmt;
        INT32 Temp1UTPLmt;
        INT32 Temp1UTPBackLmt;
        INT32 Temp2OTPLmt;
        INT32 Temp2OTPBackLmt;
        INT32 Temp2UTPLmt;
        INT32 Temp2UTPBackLmt;
    }Limit;
}T_U_PFCPROTECT;

extern T_U_PFCPROTECT g_PFCProtect;


void ACDC_GridVoltFastLossCheck(void);
void ACDC_GridACPeakVoltOVPCheck(void);
void ACDC_BusVoltHWOVPCheck(void);

void WarnCtrl_Var_Init(void);
void WarnCtrl_Manage(void);
void WarnCtrl_Handler(void);

#endif

