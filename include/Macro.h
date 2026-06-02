#ifndef _MACRO_H_
#define _MACRO_H_

//#include "DSP28x_Project.h"

#include "State.h"

#define GPIO_DETECT_AC_DIRECTION    28






#define VAL_UPLMT(Var, Max)                 {(Var) = ((Var) >= (Max)) ? (Max) : (Var);}
#define VAL_DWNLMT(Var, Min)                {(Var) = ((Var) <= (Min)) ? (Min) : (Var);}
#define VAL_UPDNLMT(Var, Max, Min)          {(Var) = ((Var) >= (Max)) ? (Max) : (Var); (Var) = ((Var) <= (Min)) ? (Min) : (Var);}
#define MAX2(x,y)                           (((x) >= (y)) ? (x) : (y))
#define MIN2(x,y)                           (((x) <= (y)) ? (x) : (y))
#define UPLMT16(Var, Max)                   {(Var) = ((Var) >= (Max)) ? (Max) : (Var);}
#define DWNLMT16(Var, Min)                  {(Var) = ((Var) <= (Min)) ? (Min) : (Var);}
#define UPDNLMT16(Var, Max, Min)            {(Var) = ((Var) >= (Max)) ? (Max) : (Var); (Var) = ((Var) <= (Min)) ? (Min) : (Var);}

#define UPLMT32(Var, Max)                   {(Var) = ((Var) >= (Max)) ? (Max) : (Var);}
#define DWNLMT32(Var, Min)                  {(Var) = ((Var) <= (Min)) ? (Min) : (Var);}
#define UPDNLMT32(Var, Max, Min)            {(Var) = ((Var) >= (Max)) ? (Max) : (Var); (Var) = ((Var) <= (Min)) ? (Min) : (Var);}

#define SELF_INC_LMT(a, b)                  if(a < b) a++;\
                                                else a = b;
#define SELF_DEC_LMT(a, b)                  if(a > b) a--;\
                                                else a = b;

#define REGBITS_SET(Reg, Mask)              (Reg |= (UINT16)(Mask))
#define REGBITS_GET(Reg, Mask)              (Reg & (UINT16)(Mask))
#define REGBITS_CLR(Reg, Mask)              (Reg &= (UINT16)(~(UINT16)(Mask)))

#define SOFTTIMER_HANDLER(DlyCnts)          {if (DlyCnts > 0){DlyCnts--;}}
#define SOFTTIMER_TIMEOUT_IS(DlyCnts)       {return ((DlyCnts == 0) ? 1 : 0);}
#define STATE_IS(State1, State2)            {return ((State1 == State2) ? 1 : 0);}
#define JUMP2_SYSSTATE(state)               {g_strPFCStatus.SysStatus = state;}
#define JUMP2_DCACSUBSTATE(state)           {g_strPFCStatus.SubStatus_DCAC = state;}
#define JUMP2_ACDCSUBSTATE(state)           {g_strPFCStatus.SubStatus_ACDC = state;}


#endif
