# Implementation Plan: 蓝牙协议 V2.1 固件（SN 生成 + firstPairing 持久化）

## Overview

本计划将设计文档拆解为增量式编码任务，按模块依赖顺序推进：先实现两个内聚的独立模块（`SN_Builder`、`Pairing_Store`），再改造下行解析（`BT_ProcessFrame`）与遥测构建（`BT_SendTelemetry`），最后在上电流程接线并纳入工程构建。每个任务都建立在前序任务之上，结尾完成集成与验证，确保没有孤立、未接入的代码。

实现语言：C（N32G003 Cortex-M0 裸机，Keil MDK-ARM）。本工程无主机侧单元测试框架，验证以「编译通过 + 串口实测上行 JSON + 下行帧实测」为准（对应 design.md 的 VP-1 ~ VP-17）。

## Tasks

- [x] 1. 实现 SN_Builder 模块（读 UID 生成 SN）
  - [x] 1.1 创建 `myapp/sn_builder.h` 接口头文件
    - 定义头文件保护宏，`#include "main.h"`
    - 定义 `SN_BUF_SIZE 32`（实际占用 31 字节，满足 ≤31 约束）
    - 声明 `void SN_Builder_Init(void);` 与 `const char *SN_Builder_Get(void);`
    - 以注释标注接口语义（Init 必须在首次 telemetry 前调用一次；Get 在 Init 前返回空字符串）
    - _Requirements: 1.3, 1.4_

  - [x] 1.2 实现 `myapp/sn_builder.c`
    - 定义模块静态缓冲区 `static char g_sn[SN_BUF_SIZE];` 与内部前缀 `#define SN_MODEL_PREFIX "PF-T2"`（与 `TELEM_MODEL` 同值，避免跨文件耦合）
    - `SN_Builder_Init()`：以 `UID_BASE`(0x1FFFF4FC) 为起点，按字节地址低→高逐字节读取 `UID_LENGTH`(12) 字节，避免端序歧义
    - 每字节先输出高半字节再输出低半字节，映射到 `"0123456789ABCDEF"`，共 24 个大写 hex 字符
    - 拼装 `SN_MODEL_PREFIX + '-' + 24 hex + '\0'`（总长 30 字符 + 终止符 = 31 字节）写入 `g_sn`
    - 兜底：UID 全 `0xFF`/全 `0x00` 不做特殊跳变，hex 转换天然产出确定性合法非空字符串（注释标注此为已知可接受输出，不引入随机/时间因子）
    - `SN_Builder_Get()`：返回 `g_sn` 指针（纯读、可重入）
    - _Requirements: 1.1, 1.2, 1.3, 1.4, 2.1, 2.2, 2.3_

- [x] 2. 实现 Pairing_Store 模块（firstPairing 的 Flash 持久化）
  - [x] 2.1 创建 `myapp/pairing_store.h` 接口头文件
    - 定义头文件保护宏，`#include "main.h"`
    - 定义 `PAIR_FLASH_PAGE 0x08007200`（与 `addr_store` 的 0x08007400 物理隔离，512 字节对齐）与 `PAIR_DONE_MAGIC 0xA5A50003`
    - 声明 `void Pairing_Store_Init(void);`、`uint8_t Pairing_IsFirst(void);`、`void Pairing_MarkDone(void);`
    - 以注释标注判定语义（未编程 0xFFFFFFFF → first=1；== magic → first=0；MarkDone 幂等）
    - _Requirements: 4.1, 4.2, 4.3_

  - [x] 2.2 实现 `myapp/pairing_store.c`
    - `Pairing_Store_Init()`：读取 `*(__IO uint32_t*)PAIR_FLASH_PAGE`，与 `PAIR_DONE_MAGIC` 比较并缓存有效性
    - `Pairing_IsFirst()`：返回 `(*(__IO uint32_t*)PAIR_FLASH_PAGE == PAIR_DONE_MAGIC) ? 0 : 1`（每次实时读 Flash，确保遥测反映最新值）
    - `Pairing_MarkDone()` 幂等判定置于屏蔽中断之前：先读 Flash 比对 magic，若已 `== PAIR_DONE_MAGIC` 立即返回不擦写（幂等），以缩短临界区，绝大多数情况下根本不触发临界区
    - `Pairing_MarkDone()` 擦写临界区用保存/恢复 `PRIMASK` 屏蔽中断：`uint32_t primask = __get_PRIMASK(); __disable_irq();` … `FLASH_Unlock()` → `FLASH_One_Page_Erase(PAIR_FLASH_PAGE)` →（仅当返回 `FLASH_EOP` 才）`FLASH_Word_Program(PAIR_FLASH_PAGE, PAIR_DONE_MAGIC)` → `FLASH_Lock();` … `__set_PRIMASK(primask);`
    - 理由注释：芯片 Flash 驱动擦/写不屏蔽中断、无 RWW 保障；屏蔽中断杜绝与 TIM1 中断中 `AddrStore_Save` 的嵌套 Flash 操作及擦写期间 ISR 从同一片 Flash 取指冲突
    - 标注可接受副作用：擦写期间（约数毫秒）TIM1 1ms tick 被挂起、`g_ms_tick` 少计若干拍轻微滞后，属业界标准代价
    - 需 `#include` 对应 CMSIS 头以使用 `__get_PRIMASK/__disable_irq/__set_PRIMASK`（经 main.h/n32g003.h 间接包含即可，无需额外显式 include）
    - 擦/写失败（非 `FLASH_EOP`）时不写入，标志保持无效，不产生半写脏数据
    - _Requirements: 4.1, 4.2, 4.3, 4.5, 5.1, 5.3_

- [~] 3. Checkpoint - 模块编译验证
  - 单独编译 `sn_builder.c`、`pairing_store.c`，确保无未定义符号、无语法错误。确保编译通过，如有疑问询问用户。

- [ ] 4. 改造 Frame_Parser 与配对确认提交标志
  - [x] 4.1 在 `myapp/bt_transparent.c` 的 `BT_ProcessFrame` 新增 reg=0x03 分支
    - 在 `bt_cmd == 0x02` 分支内、既有 0x00/0x01 逻辑之前插入命中判断 `if (bt_reg <= 3 && (bt_reg + bt_len) > 3)`
    - 取数据字节 `pair_val = bt_buf[3 - bt_reg]`；当 `pair_val == 0x01` 时置 `volatile uint8_t g_pair_commit_req = 1`（方案 A：中断仅置标志，不在中断中擦写）
    - 命中 0x03 后直接 `return`，不进入 `g_bt_regs` 写入与运行控制路径，不触发状态机切换/调速
    - `pair_val != 0x01` 时不置标志（标志不变）
    - 定义 `volatile uint8_t g_pair_commit_req`（在 bt_transparent.c），并在 `bt_transparent.h` 中 `extern` 声明供主循环引用；`#include "pairing_store.h"`
    - _Requirements: 5.1, 5.4, 7.1, 7.2, 7.3_

  - [-] 4.2 在主循环检测 `g_pair_commit_req` 并提交持久化
    - 在 `src/main.c` 的 `while(1)` 主循环中检测 `g_pair_commit_req`，为真时调用 `Pairing_MarkDone()` 并清零标志（在主上下文完成擦写，避免中断长时间阻塞）
    - 在 main.c 顶部 `#include "pairing_store.h"`
    - _Requirements: 5.1, 5.2, 5.3_

- [ ] 5. 改造 Telemetry_Module（动态 SN 与 Flash firstPairing）
  - [-] 5.1 改造 `myapp/bt_transparent.c` 的 `BT_SendTelemetry`
    - 移除 `#define TELEM_SN "T2-20260315-000123"`
    - 移除函数内 `static uint8_t s_first_pair` 及 `extern uint8_t re_pairing_done;` 相关代码块（解耦 RF315）
    - `sprintf` 的 `sn` 实参由 `TELEM_SN` 改为 `SN_Builder_Get()`
    - `firstPairing` 实参改为 `Pairing_IsFirst() ? "true" : "false"`
    - 保持 `model`(`TELEM_MODEL`)、`productionDate`(`TELEM_PROD_DATE`) 与 JSON 其余结构/字段顺序不变
    - `#include "sn_builder.h"` 与 `"pairing_store.h"`
    - _Requirements: 3.1, 3.2, 3.3, 3.4, 4.4, 6.1, 6.2_

- [x] 6. 上电初始化接线（main）
  - [x] 6.1 在 `src/main.c` 上电流程加入模块初始化调用
    - 在 `BT_Init()` 之前（首次 `BT_SendTelemetry()` 前）调用 `SN_Builder_Init()` 与 `Pairing_Store_Init()`
    - 在 main.c 顶部 `#include "sn_builder.h"`
    - _Requirements: 1.1, 4.1, 4.5, 5.5_

- [x] 7. 工程构建集成
  - [x] 7.1 将新增源文件加入 Keil MDK-ARM 工程
    - 在 `MDK-ARM/LedBlink.uvprojx` 的 myapp 分组中加入 `..\myapp\sn_builder.c` 与 `..\myapp\pairing_store.c` 文件条目
    - 确认头文件包含路径已覆盖 `myapp` 目录（现有 myapp 文件已在路径内，无需额外配置）
    - 注：本主工程仅含 Keil MDK-ARM 工程，未发现 IAR EWARM 工程文件（无 .ewp），故 IAR 工程更新不适用
    - _Requirements: 1.1, 4.1_

- [ ] 8. Checkpoint - 整体编译/链接验证
  - [x] 8.1 整体编译链接与 Flash 布局核对（VP-1、VP-2）
    - VP-1：Keil MDK-ARM 完整编译链接通过，新增文件已加入工程，无未定义符号
    - VP-2：检查链接 map 文件，确认代码段末尾 `_etext`/ROM 使用上界远低于 `0x08007200`，证明 Pairing_Store 专用页未与程序映像重叠
    - 确保编译通过，如有疑问询问用户。
    - _Requirements: 1.1, 4.1_

- [ ] 9. 实测核对（串口/调试器，对应 design.md VP-3 ~ VP-17）
  - [~] 9.1 telemetry SN 字段实测核对（VP-3 ~ VP-7）
    - VP-3：`manufacturerData.sn` 形如 `PF-T2-` + 24 位大写 hex（长度 30）；同一设备多次上电一致；不同设备不同
    - VP-4：模拟 UID 全 0xFF/0x00 时 `sn` 仍为合法非空字符串
    - VP-5：报文中不再出现 `T2-20260315-000123`
    - VP-6：`model`/`productionDate`/JSON 结构与字段顺序与改造前一致
    - VP-7：最坏情况整条 JSON（不含 `\r\n`）字节数 ≤ 480
    - _Requirements: 1.1, 1.2, 1.3, 1.4, 2.1, 2.2, 2.3, 3.1, 3.2, 3.3, 3.4_

  - [~] 9.2 firstPairing 持久化与配对确认帧实测核对（VP-8 ~ VP-13）
    - VP-8：擦除 0x08007200 页后上电，`firstPairing` 报 `true`
    - VP-9：下发 `AA 55 02 03 01 01 55` 后，所有 telemetry `firstPairing` 报 `false`
    - VP-10：断电重上电，`firstPairing` 仍为 `false`
    - VP-11：重复下发配对确认帧，0x08007200 仅被擦写一次（幂等）
    - VP-12：下发 reg=0x03 但 data≠0x01（如 `AA 55 02 03 01 00 55`），`firstPairing` 不变
    - VP-13：触发 RF315 遥控配对后，`firstPairing` 不受影响
    - _Requirements: 4.1, 4.2, 4.3, 4.4, 4.5, 5.1, 5.2, 5.3, 5.4, 5.5, 6.1, 6.2_

  - [~] 9.3 回归与 Flash 隔离实测核对（VP-14 ~ VP-17）
    - VP-14：下发 reg=0x00 运行控制帧与 reg=0x01 调速帧，行为与改造前一致
    - VP-15：下发配对确认帧（reg=0x03）时不发生状态机切换/调速
    - VP-16：下发非法帧（破坏帧头/读写字节/帧尾），坏帧丢弃，0x08007200 页不被写入
    - VP-17：先写 addr_store(0x08007400) 再写配对确认(0x08007200)，调试器确认两页数据并存互不破坏；反向顺序同样验证
    - _Requirements: 7.1, 7.2, 7.3, 4.5_

  - [~] 9.4 并发与中断屏蔽实测核对（VP-18）
    - VP-18：用调试器/逻辑分析仪确认 `Pairing_MarkDone()` 擦写期间中断被屏蔽（PRIMASK 置位），不与 TIM1 中断中的 `AddrStore_Save` 并发，亦无 ISR 介入；1ms tick 在擦写期间短暂挂起、`g_ms_tick` 轻微滞后但功能恢复正常
    - _Requirements: 4.5, 5.2_

## Notes

- 本工程为 N32G003 裸机 C 工程，无主机侧测试框架，故不规划单元测试/属性测试（PBT）任务（design.md 已省略 Correctness Properties 章节）。
- 任务 9（实测核对）依赖硬件、串口助手与调试器，由人工配合执行；其余任务为可由编码代理完成的代码/工程改动。
- 每个任务引用具体需求条款，便于追溯；Checkpoint 用于增量验证。
- 并发采用方案 A：中断仅置 `g_pair_commit_req` 易失标志，Flash 擦写在主循环上下文完成；擦写临界区用保存/恢复 `PRIMASK` 屏蔽中断（`__get_PRIMASK/__disable_irq/__set_PRIMASK`），杜绝与 TIM1 中断中 `AddrStore_Save` 的嵌套擦写及取指冲突，保证遥测读取一致、无中间态（可接受副作用：擦写期间 1ms tick 短暂挂起、`g_ms_tick` 轻微滞后）。

## Task Dependency Graph

```json
{
  "waves": [
    { "id": 0, "tasks": ["1.1", "2.1"] },
    { "id": 1, "tasks": ["1.2", "2.2"] },
    { "id": 2, "tasks": ["4.1", "6.1", "7.1"] },
    { "id": 3, "tasks": ["5.1", "4.2"] },
    { "id": 4, "tasks": ["8.1"] },
    { "id": 5, "tasks": ["9.1", "9.2", "9.3", "9.4"] }
  ]
}
```
