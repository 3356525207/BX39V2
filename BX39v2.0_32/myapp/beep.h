#ifndef __BEEP_H__
#define __BEEP_H__

#include "n32g003.h"

#define BEEP_FREQ_1KHZ    BEEPER_FREQ_1KHZ
#define BEEP_FREQ_2KHZ    BEEPER_FREQ_2KHZ
#define BEEP_FREQ_4KHZ    BEEPER_FREQ_4KHZ
#define BEEP_FREQ_8KHZ    BEEPER_FREQ_8KHZ

#define BEEP_LONG   400
#define BEEP_SHORT  200


extern uint16_t Beep_Tem;

void Beep_Init(void);
void Beep(uint16_t TIME);
void Beep_Freq(uint16_t TIME, uint32_t freq);

#endif /* __BEEP_H__ */
