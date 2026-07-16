#ifndef __MATH_DRV_H
#define __MATH_DRV_H

#include "at32m412_416_wk_config.h"

#define SIN_RAD     0X0300
#define U0_90       0x0000 
#define U90_180     0x0100
#define U180_270    0x0200
#define U270_360    0x0300

#define MOVE_WINDOW_SIZE 2
typedef struct
{
	u16   DataNum;                    //数据位置
	float InVal;                      //输入数据
	float OutVal;                     //输出平均值	
  float DataBuf[MOVE_WINDOW_SIZE];  //采样数据区
}MOVEAVERAGEFILTER_STRUCT;
   
float my_atan2(float x, float y);
void Calculate_Sin_Cos(s32 angle,float *sinval,float *cosval);
void Amplitude_Limit(s32 *input,s32 min,s32 max);
void Move_Average_Filter(MOVEAVERAGEFILTER_STRUCT *p);

#endif 


