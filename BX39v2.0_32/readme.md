# BX39 健身器材主控制器

## 1. 项目概述

BX39 是一款基于 **Nations N32G003F5Q7** (Cortex-M0, 48MHz) 微控制器的健身器材主控板固件，用于**走步机 / 律动机**设备。系统通过 315MHz 无线遥控、蓝牙透传串口（App 控制）和下行电机控制器串口通信三条通道，实现完整的运动控制、状态管理和安全保护功能。

| 项目 | 说明 |
|------|------|
| **MCU** | Nations N32G003F5Q7 (ARM Cortex-M0, 48MHz) |
| **Flash** | 29.5 KB (`0x08000000` - `0x08007600`) |
| **RAM** | 3 KB (`0x20000000` - `0x20000C00`) |
| **开发环境** | Keil MDK-ARM uVision 5 |
| **SDK** | N32G003 Standard Peripheral Library v1.0.0 |
| **蓝牙协议** | BLE 透传串口，上行 JSON / 下行 Hex 帧，详见 [蓝牙协议文档](bluetooth-v2.1-hardware-spec(3).md) |
| **遥控协议** | 315MHz OOK 无线，24-bit 帧 (2字节地址 + 1字节按键) |
| **下行通信** | UART1 9600bps，Hex 帧协议，与电机控制器通信 |

---

## 2. 系统功能

### 2.1 核心功能

- **多模式运动控制**：支持走步模式（速度 1.0~3.8 mph，步进 0.1）和律动/震动模式
- **315MHz 无线遥控**：支持 START / SPEED+ / SPEED- / MODE 四键遥控，含配对学习功能
- **蓝牙 App 控制**：支持手机 App 通过 BLE 透传串口进行速度调节、模式切换、启停控制
- **下行电机控制**：通过 UART1 与电机驱动器通信，发送速度/震动/急停指令，接收温度/步数反馈
- **7 段数码管显示**：通过 AIP1640 驱动芯片显示速度、时间、步数、状态码等信息
- **多重安全保护**：
  - 安全卡扣检测（PA4 引脚）
  - 电机温度监控与自动降功率（>110°C 限速 3.6 → >120°C 限速 3.4 → >125°C 限速 3.2 → >130°C 急停）
  - 下行通信超时看门狗（2 秒无 ACK 进入通信错误状态）
  - 独立看门狗（IWDG）

### 2.2 系统状态机（9 状态）

| 状态 | 枚举值 | 说明 |
|------|--------|------|
| `STANDBY` | 0 | 待机，等待启动指令 |
| `COUNTDOWN_START` | 1 | 启动倒计时 3 秒 |
| `RUNNING` | 2 | 正常运行中 |
| `PAUSED` | 3 | 暂停，速度缓降 |
| `COUNTDOWN_RESUME` | 4 | 恢复倒计时 3 秒 |
| `STOPPING` | 5 | 停止中，速度缓降 |
| `VIBRATION` | 6 | 律动/震动模式 |
| `ESTOP` | 7 | 急停（电机过热保护） |
| `COMM_ERR` | 8 | 通信错误（安全卡扣脱落 / UART 超时） |

### 2.3 E 错误代码

系统通过 7 段数码管显示 `E` 前缀错误代码以指示故障状态。错误码定义见 [SYS_RUN.c](myapp/SYS_RUN.c) `SYS_RUN_UpdateDisplay()` 函数。

| 错误码 | 数码管显示 | 触发条件 | 系统状态 | 处理动作 |
|--------|-----------|----------|----------|----------|
| **E0** | `___E0` | 下行 UART 通信超时（2 秒内未收到 ACK 应答） | `COMM_ERR` | 速度置零，蜂鸣 800ms 报警，持续发送急停指令 `0xAA55` |
| **E2** | `____E2` | 电机温度超过 **130°C** | `ESTOP` | 速度立即置零，蜂鸣长鸣报警，持续发送急停指令 |
| **E17** | `__E17` | 安全卡扣脱落（PA4 引脚检测为高电平） | `COMM_ERR` | 速度置零，蜂鸣 800ms 报警，发送急停指令；安全扣复位且收到 ACK 后自动恢复至 `STANDBY` |

**温度分级保护**（`RUNNING` 状态下持续监控）：

| 温度阈值 | 限制动作 | 恢复条件 |
|----------|----------|----------|
| > 110°C | 最大速度限制为 3.6 mph | 温度降至阈值以下 |
| > 120°C | 最大速度限制为 3.4 mph | 温度降至阈值以下 |
| > 125°C | 最大速度限制为 3.2 mph | 温度降至阈值以下 |
| > 130°C | 触发 **E2** 急停，进入 `ESTOP` | 需重新上电或手动复位 |

> **注意**：温度传感器数据通过 UART1 从下行电机控制器寄存器 `0x01` 读取（uint8，单位 °C），监控逻辑位于 [SYS_RUN.c:192-228](myapp/SYS_RUN.c#L192)。

---

## 3. 子系统模块

### 3.1 目录结构

```
BX39v2.0_32/
├── CMSIS/                          # ARM Cortex-M0 CMSIS 层
│   ├── core/                       #   Cortex-M0 核心访问头文件
│   └── device/                     #   N32G003 设备定义、启动文件、链接脚本
├── n32g003_std_periph_driver/      # N32G003 标准外设库 (HAL 层)
│   ├── inc/                        #   外设驱动头文件 (GPIO/UART/TIM/ADC/Flash...)
│   └── src/                        #   外设驱动源文件
├── src/                            # BSP 板级支持包
│   ├── main.c                      #   主程序入口
│   ├── n32g003_it.c                #   中断服务函数
│   ├── bsp_delay.c / .h            #   SysTick 微秒/毫秒延时
│   ├── bsp_led.c / .h              #   LED 控制
│   └── n32g003_std_periph_driver/  #   (构建使用的驱动副本)
├── inc/                            # BSP 头文件
├── myapp/                          # 应用层 — 核心业务逻辑
│   ├── SYS_RUN.c / .h              #   主状态机
│   ├── RCV315.c / .h               #   315MHz 遥控解码
│   ├── bt_transparent.c / .h       #   蓝牙透传串口
│   ├── uart_comm.c / .h            #   下行电机通信
│   ├── ctrl_tx.c / .h              #   周期控制指令发送
│   ├── tim1.c / .h                 #   TIM1 5KHz 定时器
│   ├── aip1640.c / .h              #   AIP1640 数码管驱动
│   ├── beep.c / .h                 #   蜂鸣器控制
│   └── addr_store.c / .h           #   Flash 地址存储
├── MDK-ARM/                        # Keil 工程文件与构建输出
└── examples/                       # N32G003 SDK 外设示例
```

### 3.2 模块详解

#### CMSIS 层 (`CMSIS/`)

ARM Cortex-M0 核心支持与 N32G003 器件定义层。

| 文件 | 路径 | 说明 |
|------|------|------|
| `n32g003.h` | [CMSIS/device/n32g003.h](CMSIS/device/n32g003.h) | 主 MCU 头文件：全部外设寄存器结构体、中断向量枚举、时钟定义 |
| `n32g003_conf.h` | [CMSIS/device/n32g003_conf.h](CMSIS/device/n32g003_conf.h) | 外设驱动包含配置 |
| `system_n32g003.c` | [CMSIS/device/system_n32g003.c](CMSIS/device/system_n32g003.c) | 系统初始化：时钟树配置（HSI 48MHz）、Flash 等待周期、向量表重映射 |
| `startup_n32g003.s` | [CMSIS/device/startup/startup_n32g003.s](CMSIS/device/startup/startup_n32g003.s) | Keil 启动文件：中断向量表、Reset_Handler、堆栈分配 |

#### 标准外设库 (`n32g003_std_periph_driver/`)

N32G003 官方 HAL 层，提供 GPIO / UART / TIM / ADC / Flash / Beeper / IWDG / SPI / I2C / CRC / COMP / PWR / RCC / EXTI 等外设的寄存器级操作 API。

> **注意**：`src/n32g003_std_periph_driver/` 为构建使用的副本，与顶层目录内容相同。

#### BSP 板级支持包 (`src/` + `inc/`)

| 模块 | 文件 | 说明 |
|------|------|------|
| **主程序** | [src/main.c](src/main.c) | `main()` 入口，系统初始化与主循环 |
| **中断服务** | [src/n32g003_it.c](src/n32g003_it.c) | SysTick / TIM1 / 异常处理中断；全局 `g_ms_tick` 毫秒计数器 |
| **延时** | [src/bsp_delay.c](src/bsp_delay.c) | 基于 SysTick 的微秒/毫秒延时函数 |
| **LED** | [src/bsp_led.c](src/bsp_led.c) | PA6/PA7/PA10 三路 LED 控制 |

#### 应用层 (`myapp/`) — 核心业务逻辑

##### a) SYS_RUN — 系统主状态机

- **文件**：[myapp/SYS_RUN.c](myapp/SYS_RUN.c) (639 行) / [myapp/SYS_RUN.h](myapp/SYS_RUN.h) (57 行)
- **功能**：9 状态系统状态机，处理按键事件、蓝牙指令、温度监控、显示更新、运动计时
- **调用周期**：~50ms（TIM1 ISR 触发）

##### b) RCV315 — 315MHz 无线遥控解码

- **文件**：[myapp/RCV315.c](myapp/RCV315.c) (421 行) / [myapp/RCV315.h](myapp/RCV315.h) (109 行)
- **功能**：解码 315MHz OOK 无线遥控信号（PA12 引脚边沿检测）
- **协议**：24-bit 帧，位周期 ~1.6ms，脉冲宽度调制
- **按键**：START=`0x01`, SPEED_UP=`0x02`, MODE=`0x04`, SPEED_DOWN=`0x06`
- **支持配对**：开机 3 秒内按 START 键进入配对学习模式
- **调用周期**：0.2ms（TIM1 ISR 中调用 `RCV315_Process()`）

##### c) BT_TRANSPARENT — 蓝牙透传串口

- **文件**：[myapp/bt_transparent.c](myapp/bt_transparent.c) (262 行) / [myapp/bt_transparent.h](myapp/bt_transparent.h) (55 行)
- **功能**：BLE 透传模块 UART2 通信（PA1=RX, PA2=TX, 115200bps）
- **初始化**：发送 `AT+NAME=TW-04<0733>` 和 `AT+REBOOT=1` 配置蓝牙模块（名称前缀为机型 `TW-04`，`<XXXX>` 取 `sn` 后 4 位）
- **协议**：`AA 55 [CMD] [REG] [LEN] [DATA...] 55` Hex 帧，16 字节寄存器映射
- **方向**：下行（App → 设备）Hex 帧；上行（设备 → App）JSON 文本（协议文档已定义，待实现）

##### d) UART_COMM — 下行电机控制器通信

- **文件**：[myapp/uart_comm.c](myapp/uart_comm.c) (364 行) / [myapp/uart_comm.h](myapp/uart_comm.h) (76 行)
- **功能**：UART1 与下行电机驱动器通信（PB0=TX, PB1=RX, 9600bps, 设备地址 0x01）
- **协议**：`AA 55 [DevAddr] [Cmd] [RegAddr] [Len] [Data...] [Reserved]` 帧
- **寄存器映射**：温度(0x01)、步数(0x02-0x03)、速度(0x0A)、震动(0x0E)、急停(0x10)、ACK(0x00)

##### e) CTRL_TX — 周期控制指令发送

- **文件**：[myapp/ctrl_tx.c](myapp/ctrl_tx.c) (108 行) / [myapp/ctrl_tx.h](myapp/ctrl_tx.h) (13 行)
- **功能**：每 200ms 发送速度/震动/急停指令到下行控制器
- **安全监控**：PA4 安全卡扣检测、2 秒 ACK 超时看门狗

##### f) TIM1 — 5KHz 系统心跳定时器

- **文件**：[myapp/tim1.c](myapp/tim1.c) (44 行) / [myapp/tim1.h](myapp/tim1.h) (9 行)
- **配置**：SYSCLK=48MHz, Prescaler=48 (→1MHz), Period=200 (→5KHz)
- **ISR 任务**：每 5 次中断产生 1ms 滴答 → 驱动 `g_ms_tick`、`RCV315_Process()`、`Beep_Tem`、`SYS_RUN_Process()`

##### g) AIP1640 — 7 段数码管驱动

- **文件**：[myapp/aip1640.c](myapp/aip1640.c) (119 行) / [myapp/aip1640.h](myapp/aip1640.h) (34 行)
- **功能**：GPIO 模拟时序驱动 AIP1640 LED 驱动芯片（PA3=CLK, PA5=DIN）
- **支持**：16 位显存、5 位数字显示、7 段码字母显示

##### h) BEEPER — 蜂鸣器控制

- **文件**：[myapp/beep.c](myapp/beep.c) (61 行) / [myapp/beep.h](myapp/beep.h) (22 行)
- **功能**：使用 N32G003 BEEPER 外设输出 PWM 蜂鸣音（PA14, AF7）
- **支持**：时长控制与频率自定义

##### i) ADDR_STORE — Flash 地址存储

- **文件**：[myapp/addr_store.c](myapp/addr_store.c) (51 行) / [myapp/addr_store.h](myapp/addr_store.h) (13 行)
- **功能**：将配对的 315MHz 遥控地址持久化到 Flash 末页 (`0x08007400`)
- **格式**：`0xA5A5HHLL`（4 字节：Magic + 地址高字节 + 地址低字节）

---

## 4. 硬件引脚分配

| 引脚 | 功能 | 方向 | 所属模块 |
|------|------|------|----------|
| PA1 | UART2 RX（蓝牙模块） | 输入 | bt_transparent |
| PA2 | UART2 TX（蓝牙模块） | 输出 | bt_transparent |
| PA3 | AIP1640 CLK（数码管时钟） | 输出 | aip1640 |
| PA4 | 安全卡扣检测 | 输入（下拉） | ctrl_tx |
| PA5 | AIP1640 DIN（数码管数据） | 输出 | aip1640 |
| PA6 | LED1 | 输出 | bsp_led |
| PA7 | LED2 | 输出 | bsp_led |
| PA10 | LED3 | 输出 | bsp_led |
| PA12 | 315MHz RF 接收模块 | 输入（上拉） | RCV315 |
| PA14 | 蜂鸣器 | AF7 (BEEPER) | beep |
| PB0 | UART1 TX（下行电机控制） | AF2 (UART1) | uart_comm |
| PB1 | UART1 RX（下行电机控制） | AF2 (UART1) | uart_comm |

---

## 5. 中断向量表

| 中断 | 处理函数 | 位置 | 说明 |
|------|----------|------|------|
| NMI | `NMI_Handler` | [src/n32g003_it.c:72](src/n32g003_it.c#L72) | 不可屏蔽中断（存根） |
| HardFault | `HardFault_Handler` | [src/n32g003_it.c:82](src/n32g003_it.c#L82) | 硬件错误（死循环） |
| SysTick | `SysTick_Handler` | [src/n32g003_it.c:158](src/n32g003_it.c#L158) | 系统滴答（存根） |
| **TIM1_BRK** | `TIM1_BRK_UP_TRG_COM_IRQHandler` | [src/n32g003_it.c:170](src/n32g003_it.c#L170) | **5KHz 主心跳 ISR** |
| **UART1** | `UART1_IRQHandler` | [myapp/uart_comm.c:276](myapp/uart_comm.c#L276) | **下行通信接收中断** |
| **UART2** | `UART2_IRQHandler` | [myapp/bt_transparent.c:233](myapp/bt_transparent.c#L233) | **蓝牙通信接收中断** |

---

## 6. 重要函数速查表

### 6.1 系统初始化

| 函数 | 文件:行 | 说明 |
|------|---------|------|
| `Reset_Handler` | [CMSIS/device/startup/startup_n32g003.s:120](CMSIS/device/startup/startup_n32g003.s#L120) | 复位入口，调用 System_Initializes 和 __main |
| `System_Initializes` | [CMSIS/device/system_n32g003.c:158](CMSIS/device/system_n32g003.c#L158) | 系统时钟初始化（HSI 48MHz） |
| `System_Clock_Set` | [CMSIS/device/system_n32g003.c:288](CMSIS/device/system_n32g003.c#L288) | 系统时钟切换 |
| `main` | [src/main.c:80](src/main.c#L80) | 应用程序入口，外设初始化与主循环 |
| `GPIO_Init` | [src/main.c:195](src/main.c#L195) | 安全卡扣 GPIO 初始化 |

### 6.2 系统状态机

| 函数 | 文件:行 | 说明 |
|------|---------|------|
| `SYS_RUN_Init` | [myapp/SYS_RUN.c:82](myapp/SYS_RUN.c#L82) | 状态机初始化，进入 STANDBY |
| `SYS_RUN_Process` | [myapp/SYS_RUN.c:107](myapp/SYS_RUN.c#L107) | 状态机主处理（~50ms 周期），处理事件/温度/显示 |
| `SYS_RUN_HandleKey` | [myapp/SYS_RUN.c:281](myapp/SYS_RUN.c#L281) | 处理 315MHz 遥控按键事件 |
| `SYS_RUN_HandleBTCtrl` | [myapp/SYS_RUN.c:377](myapp/SYS_RUN.c#L377) | 处理蓝牙 App 控制指令 |
| `SYS_RUN_EnterState` | [myapp/SYS_RUN.c:445](myapp/SYS_RUN.c#L445) | 状态切换处理（进入/退出动作） |
| `SYS_RUN_UpdateDisplay` | [myapp/SYS_RUN.c:510](myapp/SYS_RUN.c#L510) | 更新数码管显示内容 |

### 6.3 315MHz 遥控

| 函数 | 文件:行 | 说明 |
|------|---------|------|
| `RCV315_Init` | [myapp/RCV315.c:40](myapp/RCV315.c#L40) | 初始化 PA12 GPIO 与解码状态机 |
| `RCV315_Process` | [myapp/RCV315.c:95](myapp/RCV315.c#L95) | 边沿检测与位解码状态机（0.2ms 周期） |
| `RCV315_KeyHandler` | [myapp/RCV315.c:315](myapp/RCV315.c#L315) | 按键事件分发 |
| `RCV315_EnterPairing` | [myapp/RCV315.c:267](myapp/RCV315.c#L267) | 进入配对模式 |
| `RCV315_GetSpeed` / `RCV315_SetSpeed` | [myapp/RCV315.c:231-239](myapp/RCV315.c#L231) | 速度值读写 |

### 6.4 蓝牙通信

| 函数 | 文件:行 | 说明 |
|------|---------|------|
| `BT_UART_Init` | [myapp/bt_transparent.c:139](myapp/bt_transparent.c#L139) | UART2 硬件初始化（115200bps） |
| `BT_Init` | [myapp/bt_transparent.c:208](myapp/bt_transparent.c#L208) | 蓝牙模块 AT 指令配置 |
| `BT_ParseByte` | [myapp/bt_transparent.c:87](myapp/bt_transparent.c#L87) | Hex 帧解析状态机（7 状态） |
| `BT_ProcessFrame` | [myapp/bt_transparent.c:56](myapp/bt_transparent.c#L56) | 处理接收到的完整帧 |
| `UART2_IRQHandler` | [myapp/bt_transparent.c:233](myapp/bt_transparent.c#L233) | UART2 接收中断服务 |
| `UART2_SendData` | [myapp/bt_transparent.c:215](myapp/bt_transparent.c#L215) | 发送数据到蓝牙模块 |

### 6.5 下行电机通信

| 函数 | 文件:行 | 说明 |
|------|---------|------|
| `UART1_Comm_Init` | [myapp/uart_comm.c:33](myapp/uart_comm.c#L33) | UART1 初始化（9600bps, 地址 0x01） |
| `UART1_SendSpeed` | [myapp/uart_comm.c:127](myapp/uart_comm.c#L127) | 发送速度指令（speed × 1300） |
| `UART1_SendVibration` | [myapp/uart_comm.c:151](myapp/uart_comm.c#L151) | 发送震动等级指令 |
| `UART1_SendEmergencyStop` | [myapp/uart_comm.c:171](myapp/uart_comm.c#L171) | 发送急停指令（0xAA55） |
| `UART1_SendEmergencyRelease` | [myapp/uart_comm.c:193](myapp/uart_comm.c#L193) | 发送急停释放指令（0x5555） |
| `UART1_GetTemp` | [myapp/uart_comm.c:244](myapp/uart_comm.c#L244) | 读取电机温度 |
| `UART1_GetSteps` | [myapp/uart_comm.c:236](myapp/uart_comm.c#L236) | 读取步数（uint16 大端） |
| `UART1_CheckAndClearAck` | [myapp/uart_comm.c:253](myapp/uart_comm.c#L253) | 检查并清除 ACK 标志 |
| `UART1_IRQHandler` | [myapp/uart_comm.c:276](myapp/uart_comm.c#L276) | UART1 接收中断，帧解析状态机 |

### 6.6 周期控制发送

| 函数 | 文件:行 | 说明 |
|------|---------|------|
| `CTRL_TX_Init` | [myapp/ctrl_tx.c:11](myapp/ctrl_tx.c#L11) | 初始化控制发送模块 |
| `CTRL_TX_Process` | [myapp/ctrl_tx.c:15](myapp/ctrl_tx.c#L15) | 200ms 周期发送，含安全卡扣检测与 ACK 看门狗 |

### 6.7 定时器与中断

| 函数 | 文件:行 | 说明 |
|------|---------|------|
| `TIM1_Config` | [myapp/tim1.c:9](myapp/tim1.c#L9) | TIM1 5KHz 定时器配置 |
| `TIM1_BRK_UP_TRG_COM_IRQHandler` | [src/n32g003_it.c:170](src/n32g003_it.c#L170) | 5KHz ISR：g_ms_tick / RCV315_Process / Beep_Tem / SYS_RUN_Process |

### 6.8 数码管显示

| 函数 | 文件:行 | 说明 |
|------|---------|------|
| `aip1640_init` | [myapp/aip1640.c:17](myapp/aip1640.c#L17) | AIP1640 初始化（GPIO 模拟时序） |
| `aip1640_display` | [myapp/aip1640.c:84](myapp/aip1640.c#L84) | 刷新 16 字节显存到芯片 |
| `aip1640_Display_Number5` | [myapp/aip1640.c:103](myapp/aip1640.c#L103) | 显示 5 位数字 |

### 6.9 蜂鸣器

| 函数 | 文件:行 | 说明 |
|------|---------|------|
| `Beep_Init` | [myapp/beep.c:15](myapp/beep.c#L15) | 蜂鸣器初始化（PA14, BEEPER 外设） |
| `Beep` | [myapp/beep.c:45](myapp/beep.c#L45) | 蜂鸣指定时长（ms） |
| `Beep_Freq` | [myapp/beep.c:54](myapp/beep.c#L54) | 自定义频率蜂鸣 |

### 6.10 Flash 存储

| 函数 | 文件:行 | 说明 |
|------|---------|------|
| `AddrStore_Load` | [myapp/addr_store.c:8](myapp/addr_store.c#L8) | 从 Flash 末页加载配对遥控地址 |
| `AddrStore_Save` | [myapp/addr_store.c:28](myapp/addr_store.c#L28) | 保存配对地址到 Flash |

### 6.11 BSP 延时与 LED

| 函数 | 文件:行 | 说明 |
|------|---------|------|
| `SysTick_Delay_Us` | [src/bsp_delay.c:83](src/bsp_delay.c#L83) | 微秒延时 |
| `SysTick_Delay_Ms` | [src/bsp_delay.c:107](src/bsp_delay.c#L107) | 毫秒延时 |
| `LED_Initialize` | [src/bsp_led.c:83](src/bsp_led.c#L83) | LED GPIO 初始化 |

---

## 7. 关键全局变量

| 变量 | 定义位置 | 说明 |
|------|----------|------|
| `g_ms_tick` | [src/n32g003_it.c:61](src/n32g003_it.c#L61) | 毫秒计数器（TIM1 ISR 驱动） |
| `sys_state` | [myapp/SYS_RUN.c](myapp/SYS_RUN.c) | 当前系统状态（0-8） |
| `sys_run_time` | [myapp/SYS_RUN.c](myapp/SYS_RUN.c) | 运动运行时间（秒） |
| `sys_steps` | [myapp/SYS_RUN.c](myapp/SYS_RUN.c) | 步数计数 |
| `g_uart_regs[]` | [myapp/uart_comm.c](myapp/uart_comm.c) | 下行通信 16 字节寄存器映射 |

---

## 8. 蓝牙协议

蓝牙通信使用 BLE 透传模块，详细协议见 [蓝牙协议 V2.1 文档](bluetooth-v2.1-hardware-spec(3).md)。

**当前实现状态**：
- ✅ 下行 Hex 帧解析（App → 设备）：`AA 55 [CMD] [REG] [LEN] [DATA...] 55` 协议已实现
- ✅ 速度写入（寄存器 0x01）和运行控制（寄存器 0x00）已实现
- ⬜ 上行 JSON 遥测上报（设备 → App）：协议已定义，待实现

---

## 9. 构建说明

1. 使用 **Keil MDK-ARM uVision 5** 打开 `MDK-ARM/LedBlink.uvprojx`
2. 目标芯片：**N32G003F5Q7**
3. 编译生成文件位于 `MDK-ARM/Objects/`（AXF / HEX / BIN）
4. 下载调试工具：J-Link（配置见 `MDK-ARM/JLinkSettings.ini`）

> **注意**：工程名为历史遗留的 "LedBlink"，实际项目为 BX39 健身器材控制器。

---

## 10. 相关文档

- [蓝牙协议 V2.1 — 走步机 / 律动机对接文档](bluetooth-v2.1-hardware-spec(3).md)
- N32G003 数据手册与标准外设库文档（Nations Technologies 官方提供）
