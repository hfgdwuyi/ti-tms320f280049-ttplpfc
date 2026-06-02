#include "TimeBase.h"

UINT8 g_u8Clock2msCnt = 0;                // 2ms计数
UINT8 g_u8Time1msCnt = 0;
UINT8 g_u8Time2msCnt = 0;
UINT16 g_u16TimeBaseCnt = 0;

T_S_SYSTIMEFLAG g_strSysTimeFlg;

void TimeBase_Var_Init(void)
{
    g_u8Clock2msCnt = 0;                // 2ms计数
    g_u8Time1msCnt = 0;
    g_u8Time2msCnt = 0;
    g_u16TimeBaseCnt = 0;

    g_strSysTimeFlg.TB1ms.all = 0x00;
    g_strSysTimeFlg.TB2ms.all = 0x00;
    g_strSysTimeFlg.TB10ms.all = 0x00;
    g_strSysTimeFlg.TB20ms.all = 0x00;
    g_strSysTimeFlg.TB50ms.all = 0x00;
    g_strSysTimeFlg.TB100ms.all = 0x00;
}

void TimeBase_Manage(void)
{
    if(g_u8Time2msCnt != g_u8Clock2msCnt)
    {
        g_u16TimeBaseCnt++;
        g_u8Time2msCnt  = g_u8Clock2msCnt;
        g_strSysTimeFlg.TB2ms.all = 0xFF;
    }

    if(1 == g_strSysTimeFlg.TB2ms.bits.b1TimeBase)
    {
        g_strSysTimeFlg.TB2ms.bits.b1TimeBase = 0;

        if(0 == (g_u16TimeBaseCnt % DELAYB2MS_4MS))
        {
            //g_unSysTimeFlg.bits.b1Timer4ms = 1;          // 置4ms定时标志
        }

        if(0 == (g_u16TimeBaseCnt % DELAYB2MS_10MS))
        {
            g_strSysTimeFlg.TB10ms.all = 0xFF;          // 置10ms定时标志
        }

        if(0 == (g_u16TimeBaseCnt % DELAYB2MS_20MS))
        {
            g_strSysTimeFlg.TB20ms.all = 0xFF;          // 置20ms定时标志
        }

        if(0 == (g_u16TimeBaseCnt % DELAYB2MS_50MS))
        {
            g_strSysTimeFlg.TB50ms.all = 0xFF;          // 置50ms定时标志
        }

        if(0 == (g_u16TimeBaseCnt % DELAYB2MS_100MS))
        {
            g_strSysTimeFlg.TB100ms.all = 0xFF;         // 置100ms定时标志
        }

        if(0 == (g_u16TimeBaseCnt % DELAYB2MS_1S))
        {
            //g_unSysTimeFlg.bits.b1Timer1s = 1;            // 置1s定时标志
        }
    }
}


