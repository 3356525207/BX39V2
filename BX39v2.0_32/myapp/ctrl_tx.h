#ifndef __CTRL_TX_H
#define __CTRL_TX_H

#include "main.h"

/*==============================================================================
 * API 函数
 *============================================================================*/
void CTRL_TX_Init(void);
void CTRL_TX_Process(void);
uint8_t CTRL_TX_RequestFaultRecovery(void);

#endif /* __CTRL_TX_H */
