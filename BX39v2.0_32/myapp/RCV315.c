#include "RCV315.h"
#include "SYS_RUN.h"
#include <stddef.h>
#include "beep.h"
/*==============================================================================
 * 全局变量
 *============================================================================*/
uint8_t  re_run_status = 0;
uint8_t  re_newdata    = 0;
uint8_t  re_key        = 0;
uint8_t  re_addr[2]    = {0};
float    re_speed      = SPEED_DEFAULT;
uint8_t  re_vib_level  = 1;
uint8_t  re_pairing       = 0;
uint8_t  re_pairing_done  = 0;

/*==============================================================================
 * 模块内部静态变量
 *============================================================================*/
static uint8_t  re_status;
static uint8_t  re_bitcount;
static uint8_t  re_framebuf[4];
static uint8_t  re_addrEEP[2];
static uint8_t  re_lastlevel;
static uint8_t  re_last_key;
static uint16_t re_key_interval;
static uint8_t  re_candidate[3] = {0xFF, 0xFF, 0xFF};

static float re_speed_max  = SPEED_MAX_DEF;
static float re_speed_min  = SPEED_MIN_DEF;
static float re_speed_step = SPEED_STEP_DEF;

#define START_LOCK_TIME     15000
#define KEY_DEBOUNCE_TIME   1600
static uint16_t re_start_lock_count;

/*==============================================================================
 * RCV315_Init
 *============================================================================*/
void RCV315_Init(void)
{
    GPIO_InitType GPIO_InitStruct;

    /* 使能 GPIO 时钟 */
    if (RCV315_GPIO_PORT == GPIOA)
        RCC_APB_Peripheral_Clock_Enable(RCC_APB_PERIPH_IOPA);
    else if (RCV315_GPIO_PORT == GPIOB)
        RCC_APB_Peripheral_Clock_Enable(RCC_APB_PERIPH_IOPB);

    /* 配置 RF 输入引脚: 上拉输入模式 */
    GPIO_Structure_Initialize(&GPIO_InitStruct);
    GPIO_InitStruct.Pin        = RCV315_GPIO_PIN;
    GPIO_InitStruct.GPIO_Mode  = GPIO_MODE_INPUT;
    GPIO_InitStruct.GPIO_Pull  = GPIO_PULL_UP;
    GPIO_Peripheral_Initialize(RCV315_GPIO_PORT, &GPIO_InitStruct);

    /* 初始化接收状态 */
    re_status     = RCV_SYNC;
    re_bitcount   = 0;
    re_newdata    = 0;
    re_addr[0]    = 0;
    re_addr[1]    = 0;
    re_key        = 0;
    re_lastlevel  = 0;
    re_last_key   = 0;
    re_key_interval   = 0;
    re_start_lock_count = 0;
    re_run_status = 0;

    re_framebuf[0] = 0;
    re_framebuf[1] = 0;
    re_framebuf[2] = 0;
    re_framebuf[3] = 0;

    re_candidate[0] = 0xFF;
    re_candidate[1] = 0xFF;
    re_candidate[2] = 0xFF;

    re_addrEEP[0] = 0;
    re_addrEEP[1] = 0;
}

/*==============================================================================
 * RCV315_SetAddr
 *============================================================================*/
void RCV315_SetAddr(uint8_t addr_high, uint8_t addr_low)
{
    re_addrEEP[0] = addr_high;
    re_addrEEP[1] = addr_low;
}

/*==============================================================================
 * RCV315_Process — 315MHz 解码核心 (每 0.2ms 调用一次)
 *============================================================================*/
void RCV315_Process(void)
{
    uint8_t re_curlevel;
    static uint8_t re_timercount = 0;
    static uint8_t re_phase = 0;
    static uint8_t re_high_width = 0;
    uint8_t re_pulsewidth;

    re_curlevel = RCV315_READ_PIN();

    if (re_curlevel != re_lastlevel)
    {
        re_pulsewidth = re_timercount;
        re_timercount = 0;

        /* 上升沿 */
        if (re_curlevel == 1)
        {
            if (re_status == RCV_SYNC)
            {
                if (re_pulsewidth >= SYNC_LOW_MIN && re_pulsewidth <= SYNC_LOW_MAX)
                {
                    re_status   = RCV_RECV;
                    re_bitcount = 0;
                    re_phase    = 0;
                    re_framebuf[0] = 0;
                    re_framebuf[1] = 0;
                    re_framebuf[2] = 0;
                }
                else
                {
                    re_lastlevel = re_curlevel;
                    return;
                }
            }
            else if (re_status == RCV_RECV)
            {
                if (re_phase == 1)
                {
                    uint8_t re_low_width = re_pulsewidth;
                    uint8_t re_total = re_high_width + re_low_width;
                    uint8_t re_bitvalue;

                    if (re_total >= PERIOD_MIN && re_total <= PERIOD_MAX)
                    {
                        if (re_high_width >= HIGH_0_MIN && re_high_width <= HIGH_0_MAX &&
                            re_low_width  >= LOW_0_MIN  && re_low_width  <= LOW_0_MAX)
                            re_bitvalue = 0;
                        else if (re_high_width >= HIGH_1_MIN && re_high_width <= HIGH_1_MAX &&
                                 re_low_width  >= LOW_1_MIN  && re_low_width  <= LOW_1_MAX)
                            re_bitvalue = 1;
                        else
                        {
                            re_status = RCV_SYNC;
                            re_lastlevel = re_curlevel;
                            return;
                        }
                    }
                    else
                    {
                        re_status = RCV_SYNC;
                        re_lastlevel = re_curlevel;
                        return;
                    }

                    if (re_bitcount < FRAME_BITS)
                    {
                        if (re_bitvalue)
                            re_framebuf[re_bitcount / 8] |= (1 << (7 - (re_bitcount % 8)));
                        re_bitcount++;
                    }

                    if (re_bitcount == FRAME_BITS)
                    {
                        if (re_framebuf[0] == re_candidate[0] &&
                            re_framebuf[1] == re_candidate[1] &&
                            re_framebuf[2] == re_candidate[2])
                        {
                            re_addr[0]  = re_framebuf[0];
                            re_addr[1]  = re_framebuf[1];
                            re_key      = re_framebuf[2];
                            re_newdata  = 1;
                        }
                        re_candidate[0] = re_framebuf[0];
                        re_candidate[1] = re_framebuf[1];
                        re_candidate[2] = re_framebuf[2];
                        re_status   = RCV_SYNC;
                    }
                    re_phase = 0;
                }
                else
                {
                    re_status = RCV_SYNC;
                }
            }
        }
        /* 下降沿 */
        else if (re_curlevel == 0)
        {
            if (re_status == RCV_RECV && re_phase == 0)
            {
                re_high_width = re_pulsewidth;
                re_phase = 1;
            }
        }
        re_lastlevel = re_curlevel;
    }
    else
    {
        re_timercount++;
        if (re_timercount > TIMEOUT_TICKS)
        {
            re_status = RCV_SYNC;
            re_timercount = 0;
            re_phase = 0;
        }
    }
}

/*==============================================================================
 * 键值输出接口
 *============================================================================*/
uint8_t RCV315_GetKey(void)
{
    return re_key;
}

void RCV315_GetAddr(uint8_t *addr)
{
    if (addr != NULL)
    {
        addr[0] = re_addr[0];
        addr[1] = re_addr[1];
    }
}

float RCV315_GetSpeed(void)
{
    return re_speed;
}

void RCV315_SetSpeed(float speed)
{
    re_speed = speed;
}

uint8_t RCV315_GetVibLevel(void)
{
    return re_vib_level;
}

void RCV315_SetVibLevel(uint8_t level)
{
    re_vib_level = level;
}

/*==============================================================================
 * 接收标志位接口
 *============================================================================*/
uint8_t RCV315_IsNewData(void)
{
    return re_newdata;
}

void RCV315_ClearFlag(void)
{
    re_newdata = 0;
}

/*==============================================================================
 * 配对模式接口
 *============================================================================*/
void RCV315_EnterPairing(void)
{
    re_pairing = 1;
    re_pairing_done = 0;
    re_newdata = 0;
}

void RCV315_ExitPairing(void)
{
    re_pairing = 0;
}

uint8_t RCV315_IsPairingDone(void)
{
    return re_pairing_done;
}

void RCV315_GetPairedAddr(uint8_t *addr)
{
    if (addr != NULL)
    {
        addr[0] = re_addrEEP[0];
        addr[1] = re_addrEEP[1];
    }
}

/*==============================================================================
 * 速度范围配置
 *============================================================================*/
void RCV315_SetSpeedRange(float min, float max, float step)
{
    re_speed_min  = min;
    re_speed_max  = max;
    re_speed_step = step;
}

/*==============================================================================
 * RCV315_KeyHandler — 应用层按键处理 (每 0.2ms 调用)
 *============================================================================*/
#define DISPLAY_MOD_SPEED              1
#define RUN_STATUS_RUNNING1            1
#define RUN_STATUS_RUNNING2            2
#define RUN_STATUS_PAUSED              3
#define RUN_STATUS_PAUSED_START_CONT   4
#define RUN_STATUS_STOPPED             5
#define RUN_STATUS_START               6
#define RUN_STATUS_VIBRATINO           7

void RCV315_KeyHandler(void)
{
    if (re_start_lock_count > 0)
        re_start_lock_count--;

    /* 配对模式 */
    if (re_pairing && re_newdata == 1)
    {
        if (re_key == KEY_START)
        {
            re_addrEEP[0] = re_addr[0];
            re_addrEEP[1] = re_addr[1];
            re_pairing_done = 1;
        }
        re_newdata = 0;
        return;
    }

    /* 地址匹配 */
    if (re_newdata == 1 &&
        re_addrEEP[0] == re_addr[0] &&
        re_addrEEP[1] == re_addr[1])
    {
        if ((re_run_status == 0 && re_key != KEY_START && re_key != KEY_MODE) ||
            (re_run_status == 1 && re_start_lock_count > 0))
        {
            re_newdata = 0;
            return;
        }

        if (re_key != re_last_key)
        {
            switch (re_key)
            {
            case KEY_SPEED_UP:
                re_speed += re_speed_step;
                if (re_speed > re_speed_max) re_speed = re_speed_max;
                Beep_Tem = (re_speed == re_speed_max) ? BEEP_LONG : BEEP_SHORT;
                DISPLAY_MOD = DISPLAY_MOD_SPEED;
                DSPY_Tem = 0;
                break;

            case KEY_SPEED_DOWN:
                re_speed -= re_speed_step;
                if (re_speed < re_speed_min) re_speed = re_speed_min;
                Beep_Tem = (re_speed == re_speed_min) ? BEEP_LONG : BEEP_SHORT;
                DISPLAY_MOD = DISPLAY_MOD_SPEED;
                DSPY_Tem = 0;
                break;

            case KEY_MODE:
                if (re_run_status == 1)
                {
                    if (RUN_MOD == RUN_STATUS_RUNNING1 ||
                        RUN_MOD == RUN_STATUS_RUNNING2)
                    {
                        RUN_MOD = RUN_STATUS_PAUSED;
                        Beep_Tem = BEEP_LONG;
                    }
                    else if (RUN_MOD == RUN_STATUS_PAUSED)
                    {
                        RUN_MOD = RUN_STATUS_PAUSED_START_CONT;
                        Beep_Tem = BEEP_SHORT;
                    }
                }
                else
                {
                    Beep_Tem = BEEP_SHORT;
                    VIBRATINO_Flag += 1;
                    RUN_MOD = RUN_STATUS_VIBRATINO;
                    if (VIBRATINO_Flag > 4) VIBRATINO_Flag = 1;
                }
                break;

            case KEY_START:
                re_run_status = !re_run_status;
                if (re_run_status == 0 || RUN_MOD == RUN_STATUS_VIBRATINO)
                {
                    if (RUN_MOD == RUN_STATUS_VIBRATINO) re_run_status = 0;
                    RUN_MOD = RUN_STATUS_STOPPED;
                    Beep_Tem = BEEP_LONG;
                }
                else
                {
                    RUN_MOD = RUN_STATUS_START;
                    Beep_Tem = BEEP_SHORT;
                    re_start_lock_count = START_LOCK_TIME;
                }
                break;

            default:
                break;
            }
            re_last_key = re_key;
            re_key_interval = 0;
        }
        re_newdata = 0;
    }
    else
    {
        if (re_key_interval < KEY_DEBOUNCE_TIME)
            re_key_interval++;
        else
            re_last_key = 0;
    }
}
