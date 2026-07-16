# Requirements Document

## Introduction

本特性面向左侧 N32G003 走步机/律动机裸机固件工程（工程根 `BX39v2.0_32`），按蓝牙协议 V2.1（定稿 r2）补齐当前未实现的关键部分。

本期范围（经与用户确认）聚焦两件事：

1. **读取芯片的 96 位唯一设备 ID（Unique Device ID）作为机器序列号 SN**，替换 `myapp/bt_transparent.c` 中当前硬编码的 `TELEM_SN "T2-20260315-000123"`，使每台设备的 SN 出厂即全局唯一、稳定可重现，满足协议 §3.0 / §3.4 中「`sn` 出厂烧录、全局唯一、作为保修记录主键」的语义。
2. **完整实现 `firstPairing` 的持久化与配对确认寄存器 `0x03` 的处理**：将 `firstPairing` 由当前错误绑定的 RF315 遥控器配对状态（`re_pairing_done`）解耦，改为基于独立的 Flash 持久化标志；收到协议 §4.2 的配对确认帧后持久化为 `false`，此后 telemetry 恒报 `false`，满足协议 §3.4 的首次配对与保修登记语义及幂等保证。

本期**不包含**：ack 确认回复（§5）、坏帧 ack-warning（§4.5）、完整状态×指令矩阵（§6.2）、控制指令幂等（§5.1）、主动错误事件 JSON（§7）。这些保留为后续迭代。

技术背景：芯片为 N32G003（Cortex-M0，国民技术）。`CMSIS/device/n32g003.h` 中已定义 `UID_BASE = 0x1FFFF4FC`、`UID_LENGTH = 0x0C`（12 字节 = 96 位），即芯片唯一 ID 的只读地址。本工程为 Keil MDK-ARM / IAR EWARM 裸机 C 工程，无标准单元测试框架，验收以「编译通过 + 串口实测上行 JSON」为准。Flash 持久化可复用 `myapp/addr_store.c`（页地址 `0x08007400`、magic `0xA5A5xxxx`）的模式。

## Glossary

- **固件 (Firmware)**：运行于 N32G003 的整体走步机/律动机控制程序。
- **UID**：芯片只读的 96 位（12 字节）全局唯一设备标识，存于地址 `0x1FFFF4FC`，出厂烧录、不可更改。
- **SN生成模块 (SN_Builder)**：负责读取 UID 并按规则生成 SN 字符串的固件逻辑单元。
- **SN**：设备唯一序列号字符串，本期由 `model` 前缀 + 连字符 + UID 的 24 位大写十六进制字符组成（例：`PF-T2-A1B2C3D4E5F6A7B8C9D0E1F2`）。
- **model**：设备型号字符串，走步机固定为 `PF-T2`（对应当前 `TELEM_MODEL` 宏）。
- **遥测模块 (Telemetry_Module)**：周期性构建并发送上行 telemetry JSON 的固件逻辑单元（当前 `BT_SendTelemetry`）。
- **manufacturerData**：telemetry `data` 内的厂商信息对象，含 `model`/`sn`/`productionDate`/`firstPairing` 四字段。
- **firstPairing**：布尔值，`true` 表示此前从未完成 App 配对保修登记，`false` 表示已登记；由设备持久化并在每条 telemetry 上报。
- **配对持久化模块 (Pairing_Store)**：负责 `firstPairing` 标志在 Flash 中读写与判定的固件逻辑单元。
- **配对确认帧**：App 完成保修登记后下发的下行指令 `AA 55 02 03 01 01 55`（写寄存器 `0x03`，数据 `01`）。
- **下行解析模块 (Frame_Parser)**：解析下行 hex 帧的固件逻辑单元（当前 `BT_ParseByte` / `BT_ProcessFrame`）。
- **Flash配对标志 (Pairing_Flag)**：固件在 Flash 中持久化的「已完成 App 配对登记」标记，带 magic 校验。
- **RF315配对状态**：遥控器无线配对状态（`re_pairing_done`），与 App 首次配对登记无关，本期需与 `firstPairing` 解耦。

## Requirements

### 需求 1：读取芯片 96 位唯一 ID 生成 SN

**用户故事：** 作为售后/生产系统，我希望每台设备的 SN 来自芯片固化的 96 位唯一 ID，以便 SN 出厂即全局唯一、无需逐台烧录配置即可作为保修记录主键。

#### 验收标准

1. WHEN 固件完成上电初始化，THE SN_Builder SHALL 从地址 `0x1FFFF4FC` 起读取连续 12 字节（96 位）的 UID。
2. THE SN_Builder SHALL 将读取的 12 字节按地址从低到高顺序、每字节高半字节在前，转换为 24 个大写十六进制字符（`0`–`9`、`A`–`F`）。
3. THE SN_Builder SHALL 生成格式为 `model + "-" + 24位大写hex` 的 SN 字符串（例：`model` 为 `PF-T2` 时生成 `PF-T2-` 加 24 位 hex，总长 30 字符）。
4. THE SN_Builder SHALL 在生成的 SN 字符串末尾追加 C 字符串结束符，且生成的 SN 字符串长度 SHALL 不超过 31 字节（含结束符）。

### 需求 2：SN 的稳定性与全局唯一性

**用户故事：** 作为 App 与服务器，我希望同一台设备每次上电上报的 SN 始终相同、不同设备的 SN 互不相同，以便用 SN 作为去重和保修登记的稳定主键。

#### 验收标准

1. WHEN 同一台设备多次上电，THE SN_Builder SHALL 生成与之前完全相同的 SN 字符串。
2. THE SN_Builder SHALL 仅依据芯片 UID 生成 SN，不引入随机数、运行时计数或时间戳。
3. IF 读取的 12 字节 UID 全为 `0xFF` 或全为 `0x00`（视为 UID 不可用），THEN THE SN_Builder SHALL 仍按需求 1 的规则输出确定性 hex 字符串，使 telemetry 的 `sn` 字段始终为合法非空字符串。

### 需求 3：telemetry 使用动态 SN 替换硬编码值

**用户故事：** 作为 App，我希望 telemetry 的 `manufacturerData.sn` 反映设备真实唯一 ID，以便正确识别和登记设备。

#### 验收标准

1. THE Telemetry_Module SHALL 在每条 telemetry 的 `data.manufacturerData.sn` 字段填入 SN_Builder 生成的 SN 字符串。
2. THE Telemetry_Module SHALL 不再使用硬编码常量 `T2-20260315-000123` 作为 `sn` 值。
3. THE Telemetry_Module SHALL 保持 `data.manufacturerData` 中 `model`、`productionDate`、`firstPairing` 其余三字段的现有取值与格式不变。
4. WHEN 构建 telemetry JSON，THE Telemetry_Module SHALL 确保整条上行 JSON（不含结尾换行）长度不超过 480 字节。

### 需求 4：firstPairing 的 Flash 持久化与上报

**用户故事：** 作为 App 保修登记流程，我希望设备能持久化首次配对状态，以便重启或断电后仍能正确反映是否已完成保修登记。

#### 验收标准

1. WHERE Flash 中不存在有效的 Pairing_Flag（出厂或恢复出厂状态），THE Pairing_Store SHALL 将 `firstPairing` 判定为 `true`。
2. WHERE Flash 中存在有效的 Pairing_Flag，THE Pairing_Store SHALL 将 `firstPairing` 判定为 `false`。
3. THE Pairing_Store SHALL 使用 magic 标识校验 Pairing_Flag 的有效性，区分「已写入」与「未编程（`0xFFFFFFFF`）」。
4. THE Telemetry_Module SHALL 在每条 telemetry 的 `data.manufacturerData.firstPairing` 字段填入 Pairing_Store 当前判定的布尔值。
5. WHEN 设备重新上电，THE Pairing_Store SHALL 依据 Flash 中的 Pairing_Flag 恢复并上报与断电前一致的 `firstPairing` 值。

### 需求 5：配对确认寄存器 0x03 的处理

**用户故事：** 作为 App，我希望在完成保修登记后下发配对确认指令，设备据此将 firstPairing 永久置为 false，以便后续不再重复触发登记流程。

#### 验收标准

1. WHEN Frame_Parser 收到合法的配对确认帧 `AA 55 02 03 01 01 55`，THE Pairing_Store SHALL 将 Pairing_Flag 持久化写入 Flash。
2. WHEN Pairing_Flag 持久化成功，THE Telemetry_Module SHALL 在后续所有 telemetry 中将 `firstPairing` 报告为 `false`。
3. IF 收到配对确认帧时 Pairing_Flag 已为有效（`firstPairing` 已为 `false`），THEN THE Pairing_Store SHALL 保持 `firstPairing` 为 `false` 且不重复执行 Flash 擦除写入（幂等，避免无谓擦写）。
4. WHERE 配对确认帧的寄存器为 `0x03` 但数据字节不为 `0x01`，THE Pairing_Store SHALL 不改变 Pairing_Flag。

### 需求 6：firstPairing 与 RF315 遥控器配对解耦

**用户故事：** 作为固件维护者，我希望 firstPairing 仅反映 App 保修登记状态，不受遥控器无线配对影响，以便符合协议语义、避免误翻转。

#### 验收标准

1. THE Pairing_Store SHALL 仅依据 Flash 中的 Pairing_Flag 判定 `firstPairing`，不依据 RF315配对状态（`re_pairing_done`）。
2. WHEN RF315 遥控器完成无线配对（`re_pairing_done` 置位），THE Telemetry_Module SHALL 保持 `firstPairing` 上报值不受影响。

### 需求 7：下行配对确认帧不影响既有控制路径

**用户故事：** 作为固件维护者，我希望新增的 0x03 处理不破坏现有的运行控制（0x00）与调速（0x01）解析，以便保证回归安全。

#### 验收标准

1. WHEN Frame_Parser 收到寄存器 `0x00` 或 `0x01` 的合法写帧，THE Frame_Parser SHALL 维持现有运行控制与调速行为不变。
2. WHEN Frame_Parser 处理配对确认帧（寄存器 `0x03`），THE Frame_Parser SHALL 不触发任何运行状态机切换或调速动作。
3. IF 下行帧不符合帧头 `AA 55`、读写字节 `02`、帧尾 `55` 的格式，THEN THE Frame_Parser SHALL 维持现有坏帧丢弃行为，不写入 Pairing_Flag。
