/**
*     Copyright (c) 2022, Nations Technologies Inc.
*
*     All rights reserved.
*
*     This software is the exclusive property of Nations Technologies Inc. (Hereinafter
* referred to as NATIONS). This software, and the product of NATIONS described herein
* (Hereinafter referred to as the Product) are owned by NATIONS under the laws and treaties
* of the People's Republic of China and other applicable jurisdictions worldwide.
*
*     NATIONS does not grant any license under its patents, copyrights, trademarks, or other
* intellectual property rights. Names and brands of third party may be mentioned or referred
* thereto (if any) for identification purposes only.
*
*     NATIONS reserves the right to make changes, corrections, enhancements, modifications, and
* improvements to this software at any time without notice. Please contact NATIONS and obtain
* the latest version of this software before placing orders.

*     Although NATIONS has attempted to provide accurate and reliable information, NATIONS assumes
* no responsibility for the accuracy and reliability of this software.
*
*     It is the responsibility of the user of this software to properly design, program, and test
* the functionality and safety of any application made of this information and any resulting product.
* In no event shall NATIONS be liable for any direct, indirect, incidental, special,exemplary, or
* consequential damages arising in any way out of the use of this software or the Product.
*
*     NATIONS Products are neither intended nor warranted for usage in systems or equipment, any
* malfunction or failure of which may cause loss of human life, bodily injury or severe property
* damage. Such applications are deemed, "Insecure Usage".
*
*     All Insecure Usage shall be made at user's risk. User shall indemnify NATIONS and hold NATIONS
* harmless from and against all claims, costs, damages, and other liabilities, arising from or related
* to any customer's Insecure Usage.

*     Any express or implied warranty with regard to this software or the Product, including,but not
* limited to, the warranties of merchantability, fitness for a particular purpose and non-infringement
* are disclaimed to the fullest extent permitted by law.

*     Unless otherwise explicitly permitted by NATIONS, anyone may not duplicate, modify, transcribe
* or otherwise distribute this software for any purposes, in whole or in part.
*
*     NATIONS products and technologies shall not be used for or incorporated into any products or systems
* whose manufacture, use, or sale is prohibited under any applicable domestic or foreign laws or regulations.
* User shall comply with any applicable export control laws and regulations promulgated and administered by
* the governments of any countries asserting jurisdiction over the parties or transactions.
**/

/**
*\*\file main.c
*\*\author Nations
*\*\version v1.0.0
*\*\copyright Copyright (c) 2022, Nations Technologies Inc. All rights reserved.
**/

#include "main.h"
#include "bsp_led.h"
#include "bsp_delay.h"
#include "tim1.h"
#include "beep.h"
#include "aip1640.h"
#include "RCV315.h"
#include "SYS_RUN.h"
#include "bt_transparent.h"
#include "addr_store.h"
#include "sn_builder.h"
#include "pairing_store.h"
#include <string.h>
#include "uart_comm.h"
#include "ctrl_tx.h"
#include "n32g003_iwdg.h"
extern volatile uint32_t g_ms_tick;

#define APP_IWDG_RELOAD    500U    /* LSI ~32kHz / 256: approximately 4 seconds */

#define PAIRING_TIMEOUT   2000    /* 配对超时 2 秒 */
#define PAIRING_FLASH_MS  250     /* 闪烁间隔 250ms */

void GPIO_Init(void);

static void APP_IWDG_Init(void)
{
    IWDG_Write_Protection_Disable();
    IWDG_Prescaler_Division_Set(IWDG_CONFIG_PRESCALER_DIV256);
    IWDG_Counter_Reload(APP_IWDG_RELOAD);
    IWDG_Key_Reload();
    IWDG_Enable();
}
/**
 *\*\name   main.
 *\*\fun    main function.
 *\*\param  none.
 *\*\return none.
**/
int main(void)
{
    /* LED 初始化 */
    LED_Initialize(LED1_GPIO_PORT, LED1_GPIO_PIN | LED2_GPIO_PIN | LED3_GPIO_PIN);
    LED_Off(LED1_GPIO_PORT, LED1_GPIO_PIN | LED2_GPIO_PIN);
    LED3_OFF;
    LED_On(LED2_GPIO_PORT, LED1_GPIO_PIN | LED2_GPIO_PIN);

    /* 蜂鸣器初始化 */
    Beep_Init();

    /* 数码管显示初始化 */
    aip1640_init();

    /* 315MHz 遥控接收初始化 */
    RCV315_Init();

    /* 从 Flash 加载上次配对的遥控器地址 */
    {
        uint8_t flash_hi, flash_lo;
        AddrStore_Load(&flash_hi, &flash_lo);
        if (flash_hi != 0 || flash_lo != 0)
            RCV315_SetAddr(flash_hi, flash_lo);
    }

    /* TIM1 5KHz 定时器初始化 (0.2ms / RCV315_Process / Beep_Tem / g_ms_tick) */
    TIM1_Config();

    /* 系统状态机初始化 */
    SYS_RUN_Init();

    SysTick_Delay_Ms(1000);		
		
    /* 蓝牙透传 UART2 初始化 */
    BT_UART_Init();

 GPIO_Init();
    UART1_Comm_Init(9600, 0x01); /* UART1 9600波特率，设备地址0x01 */
    CTRL_TX_Init();             /* 下位机控制状态机初始化 */


    /*---- 开机配对: 阻塞 3 秒, 等待遥控器开始键 ----*/
    {
        uint32_t pairing_start;
        uint8_t  flash_toggle = 0;

        RCV315_EnterPairing();

        pairing_start = g_ms_tick;
        while (g_ms_tick - pairing_start < PAIRING_TIMEOUT)
        {
            SYS_RUN_Process();

					

                aip1640_display();
					
					
            if (RCV315_IsPairingDone())
            {
                /* 配对成功: 数码管显示 "----" 500ms 提示用户 */
                aip1640_Display_Number5(11111);
                aip1640_display();
                SysTick_Delay_Ms(500);
                Beep(100);
                break;
            }

            /* 等待配对期间闪烁 (每 250ms 翻转) */
            if (g_ms_tick - pairing_start >= (uint32_t)(flash_toggle + 1) * PAIRING_FLASH_MS)
            {
                flash_toggle++;
                if (flash_toggle & 1){
                    aip1640_Display_Number5(88888);
									aip1640_display();
								}
                else
                {
                    memset(display_buffer, 0, 5);
                    aip1640_display();
                }
            }
        }

        RCV315_ExitPairing();
    }
Beep(600);
    /* 模块初始化：必须在首次 BT_SendTelemetry 之前完成 */
    SN_Builder_Init();      /* 读取 UID 生成并缓存 SN 字符串 */
    Pairing_Store_Init();   /* 读取 firstPairing 持久化标志 */
    BT_Init();
    APP_IWDG_Init();
    while (1)
    {
        static uint32_t last_disp = 0, last_UART = 0, last_telem = 0, last_hb = 0;

        IWDG_Key_Reload();

        /* 配对确认提交：UART2 接收中断置位 g_pair_commit_req，
         * 在主上下文完成 Flash 擦写（屏蔽中断的临界区在 Pairing_MarkDone 内），
         * 避免中断长时间阻塞（需求 5.1/5.2/5.3）。 */
        if (g_pair_commit_req)
        {
            Pairing_MarkDone();      /* 幂等：已登记则直接返回不擦写 */
            g_pair_commit_req = 0;
        }

        /* FW-8：设备主动事件优先于 ack/telemetry 发送 (§7.5 优先级)。
         * 检测在中断/CTRL_TX 中置标志，发送在此主循环上下文。 */
        if (g_evt_overheat_stop)
        {
            g_evt_overheat_stop = 0;
            BT_SendDeviceError("OVERHEAT_STOP", "Overheat stop", "overheat");
        }
        if (g_evt_emergency_stop)
        {
            g_evt_emergency_stop = 0;
            BT_SendSafetyKeyError();
        }
        if (g_evt_system_fault)
        {
            uint8_t fault_code = g_sys_err_code;
            g_evt_system_fault = 0;
            if (fault_code == 6)
                BT_SendDeviceError("SENSOR_FAULT", "Sensor fault", "normal");
            else if (fault_code == 7)
                BT_SendDeviceError("OVERHEAT_STOP", "Overheat stop", "overheat");
            else
                BT_SendDeviceError("MOTOR_FAULT", "Motor fault", "normal");
        }
        if (g_evt_overheat_downspeed)
        {
            g_evt_overheat_downspeed = 0;
            BT_SendOverheatDownspeed();
        }

        /* FW-1：排空非阻塞 ack 请求 (中断只置标志，主循环发送，SAFE-4)。
         * 先快照再清 pending，避免读到半更新结构体；覆盖对单指令在途安全 (SAFE-3)。 */
        if (g_pending_ack.pending)
        {
            BT_PendingAck_t snap = g_pending_ack;
            g_pending_ack.pending = 0;
            BT_SendAck(snap.ackOf, snap.status, snap.err);
        }

        if (g_ms_tick - last_disp >= 50)
        {
            SYS_RUN_UpdateDisplay();
            aip1640_display();
            last_disp = g_ms_tick;
        }

        CTRL_TX_Process();

        if (g_ms_tick - last_UART >= 500)
        {
//         UART2_SendData(g_bt_regs, 16); /* 定期发送蓝牙接收数据 (调试用) */
            last_UART = g_ms_tick;
        }

        /* FW-7：蓝牙遥测上行每 200ms，心跳每 2s (原 1000ms telemetry 已替换)。
         * ack/事件在上方先发，确保不饿死 telemetry。
         * 状态切换即时补发 (g_telem_now)：EnterState 置位后立即发一帧，让 App 在
         * starting→running 等切换瞬间就拿到最新 deviceState，不必等下一个 200ms 周期，
         * 也能抵御蓝牙模组偶发丢帧造成的「设备已启动 App 仍显示启动中」延迟。 */
        if (g_telem_now || g_ms_tick - last_telem >= 200)
        {
            g_telem_now = 0;
            BT_SendTelemetry();
            last_telem = g_ms_tick;
        }
        if (g_ms_tick - last_hb >= 2000)
        {
            BT_SendHeartbeat();
            last_hb = g_ms_tick;
        }
    }
}



void GPIO_Init(void)
{
    GPIO_InitType GPIO_InitStructure;

    RCC_APB_Peripheral_Clock_Enable(RCC_APB_PERIPH_IOPA);

    /* PA4: 安全扣输入, 高电平=脱落, 内部下拉 */
    GPIO_Structure_Initialize(&GPIO_InitStructure);
    GPIO_InitStructure.Pin        = GPIO_PIN_4;
    GPIO_InitStructure.GPIO_Mode  = GPIO_MODE_INPUT;
    GPIO_InitStructure.GPIO_Pull  = GPIO_PULL_DOWN;
    GPIO_Peripheral_Initialize(GPIOA, &GPIO_InitStructure);
}
