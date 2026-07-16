#ifndef _RCV315_H
#define _RCV315_H

#include "main.h"

/*--- GPIO Pin Configuration (user-modifiable) ---*/
#define RCV315_GPIO_PORT    GPIOA
#define RCV315_GPIO_PIN     GPIO_PIN_12

#define RCV315_READ_PIN()   ((RCV315_GPIO_PORT->PID & RCV315_GPIO_PIN) ? 1 : 0)

/*--- Timing Constants (based on 0.2ms interrupt period) ---*/
#define T0_2MS          1

/* 0-code: high 410us(2 ticks), low 1190us(6 ticks). ±2 ticks for jitter + sampling */
#define HIGH_0_MIN      1
#define HIGH_0_MAX      4
#define LOW_0_MIN       4
#define LOW_0_MAX       8

/* 1-code: high 1200us(6 ticks), low 400us(2 ticks). ±2 ticks for jitter + sampling */
#define HIGH_1_MIN      4
#define HIGH_1_MAX      8
#define LOW_1_MIN       1
#define LOW_1_MAX       4

/* Period tolerance: 1.60ms (8 ticks) + jitter */
#define PERIOD_MIN      5
#define PERIOD_MAX      11

/* Frame gap low: 10ms (50 ticks) + jitter */
#define SYNC_LOW_MIN    35
#define SYNC_LOW_MAX    65

/* Timeout: >20ms (100 ticks) no change -> re-sync */
#define TIMEOUT_TICKS   100

/*--- Frame Parameters ---*/
#define FRAME_BITS      24
#define ADDR_BYTES      2
#define KEY_BYTES       1

/*--- Receive Status ---*/
#define RCV_IDLE        0
#define RCV_SYNC        1
#define RCV_RECV        2
#define RCV_DONE        3
#define RCV_ERROR       4

/*--- Key Definitions ---*/
#define KEY_START       0x01
#define KEY_SPEED_UP    0x02
#define KEY_MODE        0x04
#define KEY_SPEED_DOWN  0x06

/*--- Speed Control ---*/
#define SPEED_DEFAULT    1.0f
#define SPEED_MAX_DEF    3.8f
#define SPEED_MIN_DEF    1.0f
#define SPEED_STEP_DEF   0.1f

/*--- External Dependencies (defined by application) ---*/
extern uint16_t Beep_Tem;
extern uint8_t  DISPLAY_MOD;
extern uint8_t  DSPY_Tem;
extern uint8_t  RUN_MOD;
extern uint8_t  VIBRATINO_Flag;

/* Run status */
extern uint8_t  re_run_status;
extern uint8_t  re_newdata;
extern uint8_t  re_key;
extern uint8_t  re_addr[2];
extern float    re_speed;

/* Pairing state */
extern uint8_t  re_pairing;
extern uint8_t  re_pairing_done;

/*--- API Functions ---*/
void    RCV315_Init(void);
void    RCV315_Process(void);       /* call in timer ISR (0.2ms period) */

/* Key value output interface */
uint8_t RCV315_GetKey(void);
void    RCV315_GetAddr(uint8_t *addr);
float   RCV315_GetSpeed(void);
void    RCV315_SetSpeed(float speed);
uint8_t RCV315_GetVibLevel(void);
void    RCV315_SetVibLevel(uint8_t level);

/* Reception flag (set by ISR, cleared by application) */
uint8_t RCV315_IsNewData(void);
void    RCV315_ClearFlag(void);

/* Speed range configuration */
void    RCV315_SetSpeedRange(float min, float max, float step);

/* Application-level key handler */
void    RCV315_KeyHandler(void);

/* Pairing mode */
void    RCV315_EnterPairing(void);
void    RCV315_ExitPairing(void);
uint8_t RCV315_IsPairingDone(void);
void    RCV315_GetPairedAddr(uint8_t *addr);

#endif
