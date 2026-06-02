#include "WarnCtrl.h"


UINT16 g_u16VgridLossCnts = 0;
UINT16 g_u16VgridPeakOVPCnts = 0;

UINT16 g_u16VgridRmsOVPCnts = 0;
UINT16 g_u16VgridRmsOVPBackCnts = 0;

UINT16 g_u16VgridRmsUVPCnts = 0;
UINT16 g_u16VgridRmsUVPBackCnts = 0;

UINT16 g_u16IgridRmsOCPCnts = 0;
UINT16 g_u16IgridRmsOCPBackCnts = 0;

UINT16 g_u16VbusHWOVPCnts = 0;
UINT16 g_u16VbusHWOVPBackCnts = 0;

UINT16 g_u16VbusPeakOVPCnts = 0;
UINT16 g_u16VbusPeakOVPBackCnts = 0;

UINT16 g_u16VbusPeakUVPCnts = 0;
UINT16 g_u16VbusPeakUVPBackCnts = 0;

UINT16 g_u16Temp1UTPCnts = 0;
UINT16 g_u16Temp1UTPBackCnts = 0;
UINT16 g_u16Temp1OTPCnts = 0;
UINT16 g_u16Temp1OTPBackCnts = 0;

UINT16 g_u16Temp2UTPCnts = 0;
UINT16 g_u16Temp2UTPBackCnts = 0;
UINT16 g_u16Temp2OTPCnts = 0;
UINT16 g_u16Temp2OTPBackCnts = 0;


T_U_PFCPROTECT g_PFCProtect;

UINT16 uiOverLevelChk(INT32 lCompareData, INT32 lHighLever, UINT16 u16Filter, UINT16 *pCounter)
{
    if(lCompareData > lHighLever)
    {
        (*pCounter) ++;
    }

    else
    {
        (*pCounter) = 0;
    }

    if((*pCounter) >= u16Filter)
    {
        (*pCounter) = 0;
        return(1);
    }

    else
    {
        return(0);
    }
}

UINT16 uiUnderLevelChk(INT32 lCompareData, INT32 lLowLever, UINT16 u16Filter, UINT16 *pCounter)
{
    if(lCompareData < lLowLever)
    {
        (*pCounter) ++;
    }

    else
    {
        (*pCounter) = 0;
    }

    if((*pCounter) >= u16Filter)
    {
        (*pCounter) = 0;
        return(1);
    }

    else
    {
        return(0);
    }
}


void ACDC_GridVoltFastDropCheck(void)
{
//        if((g_lq12GridVolt.lData < _IQ12(20.0))
//            && (g_lq12GridVolt.lData > _IQ12(-20.0))
//            && (0 == g_strPFCStatus.FaultFlg.bits.b1VgridDrop))
//        {
//            g_u16VgridLossCnts++;
//            if(g_u16VgridLossCnts >= 195)
//            {
//                g_u16VgridLossCnts = 195;
//                g_strPFCStatus.FaultFlg.bits.b1VgridDrop = 1;
//
//                EPWM_setActionQualifierContSWForceAction(EPWM1_BASE,
//                                                         EPWM_AQ_OUTPUT_A,
//                                                         EPWM_AQ_SW_OUTPUT_LOW);
//                EPWM_setActionQualifierContSWForceAction(EPWM1_BASE,
//                                                         EPWM_AQ_OUTPUT_B,
//                                                         EPWM_AQ_SW_OUTPUT_LOW);
//
//                g_u8PFCPWMSwState = PWM_SWSTATE_NORM_OPT;
//            }
//        }
//        else
//        {
//            g_u16VgridLossCnts = 0;
//        }

//    if((g_lq12GridVolt.lData < IQ12_VGRID_LOSS_H_LMT)
//        && (g_lq12GridVolt.lData > IQ12_VGRID_LOOS_L_LMT)
//        && (0 == g_strPFCStatus.FaultFlg.bits.b1VgridDrop))
//    {
//        g_u16VgridLossCnts++;
//        if(g_u16VgridLossCnts >= VGRID_LOSS_CHK_CNT)
//        {
//            g_u16VgridLossCnts = VGRID_LOSS_CHK_CNT;
//            g_strPFCStatus.FaultFlg.bits.b1VgridDrop = 1;
//            IO_O_P_AC_OK_INACTIVE();
//        }
//    }
//    else
//    {
//        g_u16VgridLossCnts = 0;
//    }
}

void ACDC_GridACPeakVoltOVPCheck(void)
{
    if( 0 == g_strPFCStatus.FaultFlg.bits.b1VgridPeakOVP)
    {
//        if(uiOverLevelChk(g_lq12GridVolt.lData, IQ15_VGRID_PEAK_OVP_LMT, 3, &g_u16VgridPeakOVPCnts))
        {
            g_strPFCStatus.FaultFlg.bits.b1VgridPeakOVP = 1;
        }
    }
}

void ACDC_GridFreqCheck(void)
{

}

void ACDC_GridRMSVoltOVPCheck(void)
{
    if(g_strPFCStatus.FaultFlg.bits.b1VgridRmsOVP == 0)
    {
        if(uiOverLevelChk(g_lq12GridVoltRms.lData, g_PFCProtect.Limit.VgridRmsOVPLmt, 3, &g_u16VgridRmsOVPCnts))
        {
            g_strPFCStatus.FaultFlg.bits.b1VgridRmsOVP = 1;
        }
    }
    else
    {
        if(uiUnderLevelChk(g_lq12GridVoltRms.lData, g_PFCProtect.Limit.VgridRmsOVPBackLmt, 300, &g_u16VgridRmsOVPBackCnts))
        {
            g_strPFCStatus.FaultFlg.bits.b1VgridRmsOVP = 0;
        }
    }
}

void ACDC_GridRMSVoltUVPCheck(void)
{
    if((g_strPFCStatus.FaultFlg.bits.b1VgridDrop == 1))
    {
        if(uiOverLevelChk(g_lq12GridVoltRms.lData, 6600, 1000, &g_u16VgridRmsUVPBackCnts))
        {
            g_strPFCStatus.FaultFlg.bits.b1VgridRmsUVP = 0;
            g_strPFCStatus.FaultFlg.bits.b1VgridDrop = 0;
            g_strPFCStatus.StateFlg.bits.b1ACDrop = 0;
        }
    }



//    if((g_strPFCStatus.FaultFlg.bits.b1VgridRmsUVP == 0) && (g_strPFCStatus.FaultFlg.bits.b1VgridDrop == 0))
//    {
//        if(uiUnderLevelChk(g_lq12GridVoltRms.lData, g_PFCProtect.Limit.VgridRmsUVPLmt, 3, &g_u16VgridRmsUVPCnts))
//        {
//            g_strPFCStatus.FaultFlg.bits.b1VgridRmsUVP = 1;
//        }
//    }
//
//    if((g_strPFCStatus.FaultFlg.bits.b1VgridRmsUVP == 1) || (g_strPFCStatus.FaultFlg.bits.b1VgridDrop == 1))
//    {
//        if(uiOverLevelChk(g_lq12GridVoltRms.lData, g_PFCProtect.Limit.VgridRmsUVPBackLmt, 300, &g_u16VgridRmsUVPBackCnts))
//        {
//            g_strPFCStatus.FaultFlg.bits.b1VgridRmsUVP = 0;
//            g_strPFCStatus.FaultFlg.bits.b1VgridDrop = 0;
//            g_strPFCStatus.StateFlg.bits.b1ACDrop = 0;
//        }
//    }
}

void ACDC_GridRMSCurrOCPCheck(void)
{
    if(0 == g_strPFCStatus.FaultFlg.bits.b1IacRmsOCP)
    {
        if(uiOverLevelChk(g_lq12InnACCurrRms.lData, g_PFCProtect.Limit.IgridRmsOCPLmt, 3, &g_u16IgridRmsOCPCnts))
        {
            g_strPFCStatus.FaultFlg.bits.b1IacRmsOCP = 1;
        }
    }
    else
    {
        if(uiUnderLevelChk(g_lq12InnACCurrRms.lData, g_PFCProtect.Limit.IgridRmsOCPBackLmt, 300, &g_u16IgridRmsOCPBackCnts))
        {
            g_strPFCStatus.FaultFlg.bits.b1IacRmsOCP = 0;
        }
    }
}

void ACDC_BusVoltHWOVPCheck(void)
{
//    if(IO_I_P_PF_OVP())
//    {
//        g_u16VbusHWOVPCnts++;
//        if(g_u16VbusHWOVPCnts > 1)
//        {
//            g_u16VbusHWOVPCnts = 1;
//            g_strPFCStatus.FaultFlg.bits.b1VbusHWOVP = 1;
//        }
//        g_u16VbusHWOVPBackCnts = 0;
//    }
//    else
//    {
//        g_u16VbusHWOVPBackCnts++;
//        if(g_u16VbusHWOVPBackCnts > 1)
//        {
//            g_strPFCStatus.FaultFlg.bits.b1VbusHWOVP = 0;
//        }
//        g_u16VbusHWOVPCnts = 0;
//    }
}

void ACDC_BusPeakVoltOVPCheck(void)
{
    if (g_strPFCStatus.FaultFlg.bits.b1VbusOVP == 0)
    {
        if(uiOverLevelChk(g_lq12BusVolt.lData, 819200, 2, &g_u16VbusPeakOVPCnts))
        //if(uiOverLevelChk(g_lq12BusVolt.lData, 286720, 2, &g_u16VbusPeakOVPCnts))
        {
            g_strPFCStatus.FaultFlg.bits.b1VbusOVP = 1;

//            EPWM1_A_LOW_B_LOW();

//            EALLOW;
//            EPwm2Regs.CMPCTL.bit.SHDWAMODE = CC_IMMEDIATE;     // CMPA寄存器使用镜像模式
//            EPwm2Regs.CMPCTL.bit.SHDWBMODE = CC_IMMEDIATE;     // CMPB寄存器使用镜像模式
//            EPwm2Regs.DBFED = EPWM2_DBT_FULL_TBPRD;  // 死区 300ns
//            EPwm2Regs.DBRED = EPWM2_DBT_FULL_TBPRD;  // 死区 300ns
//            EPwm2Regs.DBCTL.bit.OUT_MODE = DB_DISABLE;
//            EPwm2Regs.DBCTL.bit.POLSEL = DB_ACTV_LOC;
//            EPWM2_DISABLE_OUTPUT();
//            EDIS;
            EPWM_setActionQualifierContSWForceAction(EPWM1_BASE,
                                                     EPWM_AQ_OUTPUT_A,
                                                     EPWM_AQ_SW_OUTPUT_LOW);
            EPWM_setActionQualifierContSWForceAction(EPWM1_BASE,
                                                     EPWM_AQ_OUTPUT_B,
                                                     EPWM_AQ_SW_OUTPUT_LOW);
            g_u8PFCPWMSwState = PWM_SWSTATE_NORM_OPT;
        }
    }
//    else
//    {
//        if(uiUnderLevelChk(g_lq12BusVolt.lData, g_PFCProtect.Limit.VbusOVPBackLmt, 1000, &g_u16VbusPeakOVPBackCnts))
//        {
//            g_strPFCStatus.FaultFlg.bits.b1VbusOVP = 0;
//        }
//    }



//    if (g_strPFCStatus.FaultFlg.bits.b1VbusOVP == 0)
//    {
//        if(uiOverLevelChk(g_lq12BusVolt.lData, g_PFCProtect.Limit.VbusOVPLmt, 2, &g_u16VbusPeakOVPCnts))
//        {
//            g_strPFCStatus.FaultFlg.bits.b1VbusOVP = 1;
//        }
//    }
//    else
//    {
//        if(uiUnderLevelChk(g_lq12BusVolt.lData, g_PFCProtect.Limit.VbusOVPBackLmt, 20, &g_u16VbusPeakOVPBackCnts))
//        {
//            g_strPFCStatus.FaultFlg.bits.b1VbusOVP = 0;
//        }
//    }
}

void ACDC_BusPeakVoltUVPCheck(void)
{
    if (g_strPFCStatus.FaultFlg.bits.b1VbusUVP == 0)
    {
        if(uiUnderLevelChk(g_lq12BusVolt.lData, g_PFCProtect.Limit.VbusUVPLmt, 20, &g_u16VbusPeakUVPCnts))
        {
            g_strPFCStatus.FaultFlg.bits.b1VbusUVP = 1;
        }
    }
    else
    {
        if(uiOverLevelChk(g_lq12BusVolt.lData, g_PFCProtect.Limit.VbusUVPBackLmt, 300, &g_u16VbusPeakUVPBackCnts))
        {
            g_strPFCStatus.FaultFlg.bits.b1VbusUVP = 0;
        }
    }
}

void TemperatCheck(void)
{
    if((uiOverLevelChk(g_lq15Temperat1.lData, g_PFCProtect.Limit.Temp1OTPLmt, TEMPERAT_OTP_CHK_CNT, &g_u16Temp1OTPCnts) == 0)
        && (uiUnderLevelChk(g_lq15Temperat1.lData, g_PFCProtect.Limit.Temp1OTPBackLmt, TEMPERAT_OTP_CHK_BACK_CNT, &g_u16Temp1OTPBackCnts) == 0))
    {
        g_strPFCStatus.FaultFlg.bits.b1Temp1OTP = 0;
    }
    else
    {
        g_strPFCStatus.FaultFlg.bits.b1Temp1OTP = 1;
    }

    if((uiOverLevelChk(g_lq15Temperat2.lData, g_PFCProtect.Limit.Temp2OTPLmt, TEMPERAT_OTP_CHK_CNT, &g_u16Temp2OTPCnts) == 0)
        && (uiUnderLevelChk(g_lq15Temperat2.lData, g_PFCProtect.Limit.Temp2OTPBackLmt, TEMPERAT_OTP_CHK_BACK_CNT, &g_u16Temp2OTPBackCnts) == 0))
    {
        g_strPFCStatus.FaultFlg.bits.b1Temp2OTP = 0;
    }
    else
    {
        g_strPFCStatus.FaultFlg.bits.b1Temp2OTP = 1;
    }
}

void WarnCtrl_Var_Init(void)
{
//    g_PFCProtect.Limit.VgridRmsOVPLmt = _IQ15(275.0);
//    g_PFCProtect.Limit.VgridRmsOVPBackLmt = _IQ15(265.0);
//    g_PFCProtect.Limit.VgridRmsUVPLmt = _IQ15(85.0);
//    g_PFCProtect.Limit.VgridRmsUVPBackLmt = _IQ15(90.0);
//
//    g_PFCProtect.Limit.IgridRmsOCPLmt = _IQ15(12.0);
//    g_PFCProtect.Limit.IgridRmsOCPBackLmt = _IQ15(10.0);
//
//    g_PFCProtect.Limit.VgridLossHLmt = _IQ15(20.0);
//    g_PFCProtect.Limit.VgridLossLLmt = _IQ15(-20.0);
//
//    g_PFCProtect.Limit.VbusOVPLmt = _IQ15(450.0);
//    g_PFCProtect.Limit.VbusUVPLmt = _IQ15(440.0);
//    g_PFCProtect.Limit.VbusOVPBackLmt = _IQ15(350.0);
//    g_PFCProtect.Limit.VbusUVPBackLmt = _IQ15(350.0);
//
//    g_PFCProtect.Limit.Temp1OTPLmt = _IQ15(85.0);
//    g_PFCProtect.Limit.Temp1OTPBackLmt = _IQ15(80.0);
//    g_PFCProtect.Limit.Temp1UTPLmt = _IQ15(-30.0);
//    g_PFCProtect.Limit.Temp1UTPBackLmt = _IQ15(-20.0);
//    g_PFCProtect.Limit.Temp2OTPLmt = _IQ15(85.0);
//    g_PFCProtect.Limit.Temp2OTPBackLmt = _IQ15(80.0);
//    g_PFCProtect.Limit.Temp2UTPLmt = _IQ15(-30.0);
//    g_PFCProtect.Limit.Temp2UTPBackLmt = _IQ15(-20.0);
}

void WarnCtrl_Manage(void)
{
    if(0 == g_strSysTimeFlg.TB2ms.bits.b1WarnCtrl)
    {
        return;
    }
    g_strSysTimeFlg.TB2ms.bits.b1WarnCtrl = 0;

//    if(g_strPFCStatus.SysStatus == SYSSTATE_ACDC)
//    {
//        ACDC_GridRMSVoltOVPCheck();
        ACDC_GridRMSVoltUVPCheck();
//        ACDC_GridRMSCurrOCPCheck();
        ACDC_BusPeakVoltOVPCheck();
//        ACDC_BusPeakVoltUVPCheck();
//    }
//    else if(g_strPFCStatus.SysStatus == SYSSTATE_DCAC)
//    {
//
//    }
    TemperatCheck();
}

void WarnCtrl_Handler(void)
{
    //if(g_strPFCStatus.SysStatus == SYSSTATE_ACDC)
    {
        ACDC_GridVoltFastDropCheck();
        //ACDC_GridACPeakVoltOVPCheck();
        //ACDC_BusVoltHWOVPCheck();
    }
}
