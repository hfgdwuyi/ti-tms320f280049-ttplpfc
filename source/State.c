#include "State.h"

//-----------------------------------------------------------------------------
// 变量定义

UINT8 g_u8BMSChrgCmd = 0;
UINT8 g_u8BatterySOC = 0;

UINT8 g_u8ACDCFaultSubState = 0;
UINT8 g_u8DCACFaultSubState = 0;

UINT16 g_u16SelfTestDlay = 0;
UINT16 g_u16FaultBackDlay = 0;
UINT16 g_u16SecAucEnOffDlay = 0;
UINT16 g_u16BypssRlyCloseDlay = 0;
UINT16 g_u16SysStatusDlay = 0;
UINT16 g_u16ACDCSubStateDlay = 0;
UINT16 g_u16VgridRMSUVDlay = 0;

T_S_DCSTATUS g_strDCStatus;
T_S_PFCSTATUS g_strPFCStatus;
T_S_DEVCFG g_strDevCfg;

void State_SelfTest(void);

void ACDC_SubState(void);
void ACDC_FaultSubState(void);

void ACDC_SubState_SoftStart(void);
void DCAC_SubState_SoftStart(void);

void State_Var_Init(void)
{
    g_u16SecAucEnOffDlay = 0;
    g_u16FaultBackDlay = 500;
    g_u16SelfTestDlay = 0;// 1ms * 1000 = 1S
    g_strPFCStatus.SysStatus = SYSSTATE_INIT;
    g_strPFCStatus.SubStatus_ACDC = SUBSTATE_ACDC_INIT;
    g_strPFCStatus.SubStatus_DCAC = SUBSTATE_DCAC_INIT;
    g_strPFCStatus.StateFlg.all = 0x00;
    g_strPFCStatus.StateFlg.bits.b1ACDrop = 1;
    g_strPFCStatus.FaultFlg.all = 0x00;
    g_strPFCStatus.FaultFlg.bits.b1VgridDrop = 1;

    g_strDevCfg.CfgFlg.bits.b1UpsModeFlg = 0;

//    g_strDevCfg.VoltPi_Kp = _IQ15(1.0);
//    g_strDevCfg.VoltPi_Ki = _IQ15(0.0);
//
//    g_strDevCfg.CurrPi_Kp = _IQ15(1.0);
//    g_strDevCfg.CurrPi_Ki = _IQ15(0.0);
//
//    g_lq12BusVoltSet.lData = _IQ12(400.0);
}

void State_Manage(void)
{
    if(1 == g_strSysTimeFlg.TB1ms.bits.b1StateMachine)
    {
        g_strSysTimeFlg.TB1ms.bits.b1StateMachine = 0;

        switch(g_strPFCStatus.SysStatus)
        {
            case SYSSTATE_INIT:
                State_SelfTest();
                if(g_strPFCStatus.StateFlg.bits.b1SelfTestOK)
                {
                    g_strPFCStatus.SysStatus = SYSSTATE_SWITCH;
                    g_u16SysStatusDlay = 0;
                }
                break;

            case SYSSTATE_SWITCH:
                if(g_strPFCStatus.StateFlg.bits.b1VgridOK)
                {
                    g_strPFCStatus.SysStatus = SYSSTATE_ACDC;
                    g_strPFCStatus.StateFlg.bits.b1SoftStartDone = 0;
                }
                break;

            case SYSSTATE_ACDC:
                ACDC_SubState();
                ACDC_FaultSubState();
                break;

            default :
                break;
        }
    }
}

void State_SelfTest(void)
{
    if(0 == g_strPFCStatus.StateFlg.bits.b1SelfTestOK)
    {
        if(g_u16SelfTestDlay++ >= 1000)
        {
            if(g_strPFCStatus.FaultFlg.bits.b1Temp1OTP
               && g_strPFCStatus.FaultFlg.bits.b1Temp2OTP
               && g_strPFCStatus.FaultFlg.bits.b1VgridPeakOVP
               && g_strPFCStatus.FaultFlg.bits.b1VgridRmsOVP
               && g_strPFCStatus.FaultFlg.bits.b1IacRmsOCP
               && g_strPFCStatus.FaultFlg.bits.b1VbusHWOVP
               && g_strPFCStatus.FaultFlg.bits.b1VbusOVP)
            {
                g_strPFCStatus.StateFlg.bits.b1SelfTestOK = 1;
            }

            g_u16SelfTestDlay = 0;
        }
    }
}

void ACDC_SubState(void)
{
    switch(g_strPFCStatus.SubStatus_ACDC)
    {
        case SUBSTATE_ACDC_INIT:
            if((0 == g_strPFCStatus.FaultFlg.bits.b1VgridRmsUVP) && (0 == g_strPFCStatus.FaultFlg.bits.b1VgridRmsOVP))
            {
//                EN_INPUT_RELAY();
                if(g_u16ACDCSubStateDlay++ >= 200) // 200ms
                {
                    g_strPFCStatus.SubStatus_ACDC = SUBSTATE_ACDC_SOFTSTART;
                }
            }
            break;

        case SUBSTATE_ACDC_SOFTSTART:
            if(0 != g_strPFCStatus.FaultFlg.all)
            {
                g_strPFCStatus.SubStatus_ACDC = SUBSTATE_ACDC_SHUTDOWN;
            }
            else
            {
                if(g_strPFCStatus.StateFlg.bits.b1SoftStartDone && (g_lq12BusVolt.lData > IQ12_VBUS_380V))
                {
                    g_strPFCStatus.SubStatus_ACDC = SUBSTATE_ACDC_NORMAL;
                }
            }
            break;

        case SUBSTATE_ACDC_NORMAL:
            if(0 != g_strPFCStatus.FaultFlg.all)
            {
                g_strPFCStatus.SubStatus_ACDC = SUBSTATE_ACDC_SHUTDOWN;
            }
            break;

        case SUBSTATE_ACDC_SHUTDOWN:
            if(0 == g_strPFCStatus.FaultFlg.all)
            {
                g_strPFCStatus.SubStatus_ACDC = SUBSTATE_ACDC_SOFTSTART;
                g_strPFCStatus.SysStatus = SYSSTATE_SWITCH;
                EPWM_setActionQualifierContSWForceAction(EPWM1_BASE,
                                                         EPWM_AQ_OUTPUT_A,
                                                         EPWM_AQ_SW_DISABLED);
                EPWM_setActionQualifierContSWForceAction(EPWM1_BASE,
                                                         EPWM_AQ_OUTPUT_B,
                                                         EPWM_AQ_SW_DISABLED);
            }
            else
            {
//                EPWM1_A_LOW_B_LOW();
//                EPWM2_DISABLE_OUTPUT();
                EPWM_setActionQualifierContSWForceAction(EPWM1_BASE,
                                                         EPWM_AQ_OUTPUT_A,
                                                         EPWM_AQ_SW_OUTPUT_LOW);
                EPWM_setActionQualifierContSWForceAction(EPWM1_BASE,
                                                         EPWM_AQ_OUTPUT_B,
                                                         EPWM_AQ_SW_OUTPUT_LOW);
            }
            break;


        default :
            break;
    }

    if((1 == g_strPFCStatus.StateFlg.bits.b1InputRelay)
        && (0 == g_strPFCStatus.FaultFlg.bits.b1VgridRmsUVP)
        && (0 == g_strPFCStatus.FaultFlg.bits.b1VgridRmsOVP))
    {
        if(g_u16BypssRlyCloseDlay++ >= 20)
        {
            g_u16BypssRlyCloseDlay = 20;
//            if(1 == g_strPFCCommuCmd.CmdFlg.bits.b1ACOutput)
            {
//                EN_BYPASS_RELAY();
            }
        }
    }
    else
    {
        g_u16BypssRlyCloseDlay = 0;
    }

    if((g_strPFCStatus.FaultFlg.all & 0x0001FF1F) && (g_strPFCStatus.SubStatus_ACDC != SUBSTATE_ACDC_FAULT))
    {
        g_strPFCStatus.SubStatus_ACDC = SUBSTATE_ACDC_FAULT;
        g_u8ACDCFaultSubState = 0;
    }
}

void ACDC_FaultSubState(void)
{
    switch(g_u8ACDCFaultSubState)
    {
        case 0:
            if(g_strPFCStatus.FaultFlg.all & 0x0001FF03)   // VgridRmsUVP || VgridRmsOVP || Temp1OTP || Temp1OTP || VbusHWOVP || VbusOVP
            {
                g_u8ACDCFaultSubState = 1;

            }
            else if(g_strPFCStatus.FaultFlg.all & 0x0001FF03)   // VbusUVP || IacRmsOCP
            {
                g_u8ACDCFaultSubState = 2;
//                SetFaultBackTimer(1000);
            }
            else
            {
                if(0 == (g_strPFCStatus.FaultFlg.all & 0x0001FF03))
                {
                    g_strPFCStatus.SubStatus_ACDC = SUBSTATE_ACDC_SOFTSTART;
                }
            }
            break;

        case 1:
            if(g_strPFCStatus.FaultFlg.bits.b1VgridRmsUVP)
            {
                if(g_u16VgridRMSUVDlay++ >= 200) // AC 欠压200ms
                {
                    g_u16VgridRMSUVDlay = 0;
                    g_strPFCStatus.SubStatus_ACDC = SUBSTATE_ACDC_INIT;
                    g_strPFCStatus.SysStatus = SYSSTATE_SWITCH;

//                    EPWM1_A_LOW_B_LOW();
//                    EPWM2_DISABLE_OUTPUT();
//                    DIS_INPUT_RELAY();

                    EPWM_setActionQualifierContSWForceAction(EPWM1_BASE,
                                                             EPWM_AQ_OUTPUT_A,
                                                             EPWM_AQ_SW_OUTPUT_LOW);
                    EPWM_setActionQualifierContSWForceAction(EPWM1_BASE,
                                                             EPWM_AQ_OUTPUT_B,
                                                             EPWM_AQ_SW_OUTPUT_LOW);
                }
            }
            else
            {
                if(0 == (g_strPFCStatus.FaultFlg.all & 0x0001FF03)) // VgridRmsUVP || VgridRmsOVP || Temp1OTP || Temp1OTP || VbusHWOVP || VbusOVP
                {
                    g_strPFCStatus.SubStatus_ACDC = SUBSTATE_ACDC_SOFTSTART;
                }
            }
            break;

        case 2:
//            if(GetFaultBackStatus())
//            {
//                g_strPFCStatus.SubStatus_ACDC = SUBSTATE_ACDC_SOFTSTART;
//            }
            break;

        default :
            break;
    }
}


void SoftStart_Handler(void)
{
    g_strPFCStatus.SysStatus = SYSSTATE_ACDC;
    if(g_strPFCStatus.SysStatus == SYSSTATE_ACDC)
    {
//        if(g_strPFCStatus.StateFlg.bits.b1SoftStartDone)
//        {
//            return;
//        }

        if(g_lq12BusVoltSet.lData > IQ12_VBUS_400V)
        {
            g_lq12BusVoltSet.lData = IQ12_VBUS_400V;
        }

        if(g_lq12BusVoltSet.lData < IQ12_VBUS_20V)
        {
            g_lq12BusVoltSet.lData = IQ12_VBUS_20V;
        }

        if(g_lq12BusVoltRef.lData < (g_lq12BusVoltSet.lData - IQ12_VBUS_SOFT_STEP))
        {
            g_lq12BusVoltRef.lData = g_lq12BusVoltRef.lData + IQ12_VBUS_SOFT_STEP;
        }
        else if (g_lq12BusVoltRef.lData > (g_lq12BusVoltSet.lData + IQ12_VBUS_SOFT_STEP))
        {
            g_lq12BusVoltRef.lData = g_lq12BusVoltRef.lData - IQ12_VBUS_SOFT_STEP;
        }
        else
        {
            g_lq12BusVoltRef.lData = g_lq12BusVoltSet.lData;
            g_strPFCStatus.StateFlg.bits.b1SoftStartDone = 1;
        }
    }
}
