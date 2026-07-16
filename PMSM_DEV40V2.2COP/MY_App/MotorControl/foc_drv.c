
/* 包含头文件 ----------------------------------------------------------------*/
#include "foc_drv.h"
#include "at32m412_416_wk_config.h"                            
FOC_STRUCT FocStruct = {0}; // 定义FOC结构体变量并初始化为0


/**
  * 函数功能:Clark变换 
  * 输入参数:
  * 返回参数:
  * 说    明:恒幅值变换 
  */
void Clark_Transform(FOC_STRUCT *p)
{
	p->Ialpha = p->Iu;
	p->Ibeta  = (p->Iu * 0.57735027f) + (p->Iv * 1.1547004f);     
}
	
/**
  * 函数功能:Pack变换 
  * 输入参数:
  * 返回参数:
  * 说    明: 
  */
void Pack_Transform(FOC_STRUCT *p)
{
	p->Id = (p->Ialpha * p->CosVal)  + (p->Ibeta * p->SinVal);
	p->Iq = (-p->Ialpha * p->SinVal) + (p->Ibeta * p->CosVal);	
}

/**
  * 函数功能:反Pack变换 
  * 输入参数:
  * 返回参数:
  * 说    明: 
  */
void IPack_Transform(FOC_STRUCT *p)
{
  p->Ualpha = p->Ud * p->CosVal - p->Uq * p->SinVal;
	p->Ubeta  = p->Uq * p->CosVal + p->Ud * p->SinVal;
}

/**
  * 函数功能:合成空间电压矢量 
  * 输入参数:
  * 返回参数:
  * 说    明:三相电机使用SVPWM 
  */
void Calculate_SVPWM(FOC_STRUCT *p)
{
  float U1,U2,U3 = 0;
	float X ,Y ,Z = 0;
  float T1,T2,T1Temp,T2Temp = 0;	
	uint8_t A,B,C,N = 0;
	uint16_t  Ta,Tb,Tc = 0;
	
	U1 = p->Ubeta;
	U2 = (0.866f * p->Ualpha) - (0.5f * p->Ubeta);
  U3 = (-0.866f * p->Ualpha) - (0.5f * p->Ubeta);
	
	if(U1 > 0){A = 1;} else{A = 0;}
	if(U2 > 0){B = 1;} else{B = 0;}
	if(U3 > 0){C = 1;} else{C = 0;}
	N = 4 * C + 2 * B + A;
	
	X = (1.732f * p->PwmCycle * p->Ubeta) / p->Ubus;
	Y = (1.5f * p->Ualpha * p->PwmCycle + 0.866f * p->Ubeta * p->PwmCycle) / p->Ubus;
	Z = (-1.5f * p->Ualpha * p->PwmCycle + 0.866f * p->Ubeta * p->PwmCycle) / p->Ubus;
			
	switch(N)
	{
	  case 3: {T1 = -Z; T2 =  X;} break;
	  case 1: {T1 =  Z; T2 =  Y;} break;
	  case 5: {T1 =  X; T2 = -Y;} break;
	  case 4: {T1 = -X; T2 =  Z;} break;
	  case 6: {T1 = -Y; T2 = -Z;} break;
	  case 2: {T1 =  Y; T2 = -X;} break;
    default:{T1 = 0;  T2=0;}    break;
	}
	
	T1Temp = T1;
	T2Temp = T2;
	if(T1+T2 > p->PwmLimit)
	{
	  T1 = p->PwmLimit * T1Temp / (T1Temp + T2Temp);
    T2 = p->PwmLimit * T2Temp / (T1Temp + T2Temp);
	}
	
	Ta = (p->PwmCycle - T1 - T2) * 0.25f;
	Tb = Ta + T1 * 0.5f;
	Tc = Tb + T2 * 0.5f;
	
	switch(N)
	{
	  case 3: 
      {
      p->DutyCycleA = Ta;
      p->DutyCycleB = Tb;
      p->DutyCycleC = Tc;
	  } break;
	  case 1: 
    {
      p->DutyCycleA = Tb;
      p->DutyCycleB = Ta;
      p->DutyCycleC = Tc;
		} break;
	  case 5: 
    {
      p->DutyCycleA = Tc;
      p->DutyCycleB = Ta;
      p->DutyCycleC = Tb;
		} break;
	  case 4: 
    {
      p->DutyCycleA = Tc;
      p->DutyCycleB = Tb;
      p->DutyCycleC = Ta;
		} break;
	  case 6: 
    {
      p->DutyCycleA = Tb;
      p->DutyCycleB = Tc;
      p->DutyCycleC = Ta;
		} break;
	  case 2: 
    {
      p->DutyCycleA = Ta;
      p->DutyCycleB = Tc;
      p->DutyCycleC = Tb;
		} break;
    default:
		{			
	    p->DutyCycleA = Ta;
      p->DutyCycleB = Tb;
      p->DutyCycleC = Tc;
		}break;
	}
}
