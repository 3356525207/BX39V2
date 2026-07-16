#ifndef __FOC_DRV_H
#define __FOC_DRV_H

#include "at32m412_416_wk_config.h"

typedef struct
{		
	float Iu;            // U相电流  
	float Iv;            // V相电流 
	float Iw;            // W相电流 	
	float Ialpha;        // alpha轴电流 
	float Ibeta;	       // beta轴电流 

	float  MCangle;		  // 电机当前角度
	float  ElectricalVal; // 电机电角度
	float  speed;         // 电机速度
	float  speedLPF;      // 电机速度滤波值
	
	int Taposition;

	float  last_anglefloat; // 电机速度计算用上次角度
	float  anglefloat;
	
	float SinVal;        // 正弦值
	float CosVal;        // 余弦值
	float Id;	           // d轴电流 
	float Iq;	           // q轴电流 
	
  float IdLPF; 	       // d轴电流滤波值
  float IqLPF; 		     // q轴电流滤波值
  float IdLPFFactor; 	 // d轴电流滤波系数
  float IqLPFFactor; 	 // q轴电流滤波系数	

  float Taspeed;       // 目标速度
  float Iq_ref;        // q轴电流参考值
	
	float Ud;            // d轴电压 
	float Uq;            // q轴电压 	
	float Ualpha;        // alpha轴电压
	float Ubeta;         // beta轴电压		
	float Ubus;          // 母线电压	
	
  uint16_t   PwmCycle;      // PWM周期
  uint16_t   PwmLimit;	     // 最大占空比
	uint16_t   DutyCycleA;    // A相占空比
	uint16_t   DutyCycleB;    // B相占空比
	uint16_t   DutyCycleC;    // C相占空比	
}FOC_STRUCT;

extern FOC_STRUCT FocStruct;

void Clark_Transform(FOC_STRUCT *p);
void Pack_Transform(FOC_STRUCT *p); 
void IPack_Transform(FOC_STRUCT *p);
void Calculate_SVPWM(FOC_STRUCT *p);


#endif 


