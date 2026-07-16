#ifndef __ENCODER_H__
#define __ENCODER_H__

#include "wk_system.h"
#include <stdio.h>





extern int32_t speed_rps;

float Gat_Encsingle_float(void);

int32_t Get_Encoder_Speed(void);


float Gat_VFsingle_float(void);

void Enc_Init(void );


#endif 