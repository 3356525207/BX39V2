#ifndef VOFA_H
#define VOFA_H

#include "wk_usart.h"

extern float tempFloat[8];
extern uint8_t tempData[28];

//发送数组
void send_array(uint8_t *array, uint8_t len);
void vofa_send(void);


#endif // VOFA_H
