#ifndef _DATA_TYPE_H_
#define _DATA_TYPE_H_


/**********************宏定义*************************/
typedef signed char         INT8;           // 有符号8位数
typedef unsigned char       UINT8;          // 无符号8位数，字节
typedef int                 INT16;          // 有符号16位数
typedef long                INT32;          // 有符号32位数
typedef unsigned int        UINT16;         // 无符号16位数，字
typedef unsigned long       UINT32;         // 无符号32位数，长字
typedef float               FLOAT32;
typedef long double         FLOAT64;

#ifndef GLOBAL_Q
#define GLOBAL_Q            10 /* Q1 to Q29 */
#endif

struct intData
{
    INT16 iLD;
    INT16 iHD;
};
typedef struct intData intStructData;

typedef union
{
    INT32   lData;
    intStructData   iData;

    struct
    {
        UINT16  lword;
        INT16  hword;
    }half;
}longunion;



typedef union
{
    UINT16  id;
    struct  packed_data
    {
        UINT16 highchar    :8;
        UINT16 lowchar     :8;
    }bitdata;
}ubitint;

typedef union
{
    float   fd ;
    ubitint intdata[2];
    long    lData;
    intStructData   iData;
}ubitfloat;


#endif

