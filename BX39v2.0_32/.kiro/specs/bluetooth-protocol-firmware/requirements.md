# Requirements Document

## Introduction

本特性在 N32G003（Cortex-M0）走步机固件上补齐《蓝牙协议 V2.1 — 走步机 / 律动机对接文档（定稿 r2）》定义的尚未完成的固件行为，使设备成为协议中"状态与计时的唯一权威"。

工作范围聚焦三块：

1. **机器 SN 码**：读取芯片 96 位唯一设备 ID（N32G003 `UID_BASE = 0x1FFFF4FC`，12 字节），格式化为全局唯一、跨重启稳定的 `sn`，在每条 telemetry 的 `data.manufacturerData.sn` 中上报，替换当前硬编码常量。
2. **首次配对登记**：`firstPairing` 的持久化（出厂 `true`、收到「配对确认」后永久 `false`），以及下行 `0x03` 配对确认指令的解析与处理。
3. **补齐协议行为**：telemetry（200ms）、heartbeat（2s）、合法指令 200ms 内回 `ack`、下行 hex 帧解析与坏帧处理、状态×指令矩阵、过渡态倒计时、session 生命周期、targetSpeed 记忆、过热降速/停机/冷却恢复、主动错误事件，以及上行 JSON 的统一结构与字段值规范化。

本文档以"走步机（`deviceType:"treadmill"`）"为实现目标；律动机相关字段（§3.2/§4.4）不在本固件范围内，但上行消息结构与 SN/firstPairing 机制对两类设备通用。

现有固件已实现：状态机骨架（`SYS_RUN.c`）、下行帧字节解析（`bt_transparent.c` 的 `BT_ParseByte`）、telemetry JSON 拼装（`BT_SendTelemetry`）、配对地址 Flash 持久化（`addr_store.c`）。这些是本特性改造与扩展的基础。

## Glossary

- **Firmware（设备固件）**: 运行于 N32G003 的走步机控制固件整体。
- **SN_Provider（SN 生成模块）**: 读取芯片 96 位唯一 ID 并格式化为 `sn` 字符串的固件模块。
- **Chip_UID（芯片唯一 ID）**: N32G003 出厂烧录的 96 位（12 字节）只读唯一标识，位于 `UID_BASE = 0x1FFFF4FC`。
- **Pairing_Store（配对持久化模块）**: 负责将 `firstPairing` 标志持久化到 Flash 并跨重启读取的模块。
- **Frame_Parser（下行帧解析器）**: 解析下行 hex 帧 `AA 55 [读写] [寄存器] [长度] [数据...] 55` 的状态机（现 `BT_ParseByte`/`BT_ProcessFrame`）。
- **Telemetry_Reporter（遥测上报器）**: 周期性构建并上行 telemetry JSON 的模块（现 `BT_SendTelemetry`）。
- **Heartbeat_Reporter（心跳上报器）**: 周期性上行 heartbeat JSON 的模块。
- **Ack_Responder（指令应答模块）**: 对合法下行控制指令回 `ack` JSON 的模块。
- **State_Machine（走步机状态机）**: 管理 `idle/starting/running/pausing/paused/resuming/stopping/stopped/error` 状态及其迁移的模块（现 `SYS_RUN.c`）。
- **Thermal_Manager（过热管理模块）**: 监测电机温度、执行降速/停机/冷却恢复并触发对应错误事件的模块。
- **Uplink_Message（上行消息）**: 设备→App 的一条 JSON 文本，含 `type/seq/ts/status/data/error`，以 `\n`(0x0A) 结尾。
- **Control_Command（控制指令）**: App→设备的一条合法下行 hex 控制帧。
- **deviceState（设备状态）**: telemetry 中上报的协议状态字符串，取值 `idle/starting/running/pausing/paused/resuming/stopping/stopped/error`。
- **temperatureState（温度状态）**: telemetry 中上报的温度状态字符串，取值 `normal/hot/overheat`。
- **session（运动会话）**: 一次从启动到结束的运动周期，以 `sessionId` 标识。
- **targetSpeed（目标速度）**: 当前会话的目标速度（mph，1 位小数）。
- **mph**: 英里/小时，速度单位。
- **mile**: 英里，距离单位。

## Requirements

### Requirement 1: 从芯片 96 位唯一 ID 生成机器 SN 码

**User Story:** 作为售后系统，我希望每台设备上报全局唯一且跨重启稳定的 `sn`，以便将其作为服务器保修记录主键。

#### Acceptance Criteria

1. WHEN Firmware 上电初始化, THE SN_Provider SHALL 从地址 `0x1FFFF4FC` 读取 96 位（12 字节）Chip_UID。
2. THE SN_Provider SHALL 将 96 位 Chip_UID 编码为固定长度的十六进制字符串，每字节对应 2 个大写十六进制字符，共 24 个字符。
3. THE SN_Provider SHALL 输出长度不超过 32 个字符的 `sn` 字符串。
4. WHEN 同一台设备多次上电, THE SN_Provider SHALL 生成与此前相同的 `sn` 字符串。
5. WHERE 设备配置了型号前缀, THE SN_Provider SHALL 以 `<model>-<UID 十六进制>` 形式组合 `sn`，前缀与 UID 之间用单个连字符 `-` 分隔。
6. THE Telemetry_Reporter SHALL 在每条 telemetry 的 `data.manufacturerData.sn` 中上报 SN_Provider 生成的 `sn` 字符串。

### Requirement 2: telemetry 厂商信息字段

**User Story:** 作为 App，我希望从 telemetry 的 `manufacturerData` 中获取型号、序列号、生产日期与首次配对标志，以便完成设备识别与保修登记。

#### Acceptance Criteria

1. THE Telemetry_Reporter SHALL 在每条 telemetry 的 `data.manufacturerData` 对象中包含 `model`、`sn`、`productionDate`、`firstPairing` 四个字段。
2. THE Telemetry_Reporter SHALL 以字符串类型上报 `model`、`sn` 与 `productionDate`，并以 `YYYYMMDD` 格式上报 `productionDate`。
3. THE Telemetry_Reporter SHALL 以布尔类型上报 `firstPairing`。
4. THE Telemetry_Reporter SHALL 在 `data` 中上报 `deviceType` 字段，值为 `treadmill`。

### Requirement 3: firstPairing 持久化

**User Story:** 作为售后系统，我希望设备持久化 `firstPairing` 标志，以便保修登记仅在首次配对时触发且重启后不被重置。

#### Acceptance Criteria

1. WHEN 设备从未完成过配对登记, THE Pairing_Store SHALL 使 `firstPairing` 取值为 `true`。
2. WHEN Firmware 上电初始化, THE Pairing_Store SHALL 从 Flash 读取已持久化的 `firstPairing` 取值。
3. IF Flash 中不存在有效的 `firstPairing` 记录, THEN THE Pairing_Store SHALL 将 `firstPairing` 取值为 `true`。
4. WHEN 设备完成配对确认, THE Pairing_Store SHALL 将 `firstPairing` 持久化为 `false`。
5. WHEN 设备在 `firstPairing` 为 `false` 后重新上电, THE Pairing_Store SHALL 使 `firstPairing` 保持 `false`。
6. WHILE `firstPairing` 已持久化为 `false`, THE Telemetry_Reporter SHALL 在每条 telemetry 中上报 `firstPairing:false`。
7. THE Pairing_Store SHALL 将 `firstPairing` 的持久化与遥控器配对地址（`AddrStore`）的持久化相互独立处理。

### Requirement 4: 配对确认指令（寄存器 0x03）

**User Story:** 作为 App，我希望在完成保修登记后下发配对确认指令，以便设备将 `firstPairing` 永久翻转为 `false`。

#### Acceptance Criteria

1. WHEN Frame_Parser 收到合法控制帧 `AA 55 02 03 01 01 55`, THE Pairing_Store SHALL 将 `firstPairing` 持久化为 `false`。
2. WHEN 设备处理完配对确认指令, THE Ack_Responder SHALL 回一条 `ackOf` 值为 `confirmPairing` 的 `ack` 消息。
3. IF 设备在 `firstPairing` 已为 `false` 时再次收到配对确认指令, THEN THE Ack_Responder SHALL 回 `status:"ok"` 的 `ack`，且 THE Pairing_Store SHALL 保持 `firstPairing` 为 `false`。
4. THE Pairing_Store SHALL 在 §3.4 步骤②③任一失败导致设备未收到配对确认指令的情况下，保持 `firstPairing` 为 `true`。

### Requirement 5: 上行消息统一结构

**User Story:** 作为 App，我希望所有上行消息遵循统一 JSON 结构与编码约定，以便稳定解析。

#### Acceptance Criteria

1. THE Firmware SHALL 在每条 Uplink_Message 中包含 `type`、`seq`、`ts`、`status`、`data`、`error` 六个字段。
2. THE Firmware SHALL 以 UTF-8 编码 Uplink_Message，并以单个 `\n`(0x0A) 字符结尾。
3. THE Firmware SHALL 使单条 Uplink_Message 的长度不超过 480 字节（不含结尾 `\n`）。
4. THE Firmware SHALL 使用递增的整数 `seq`，并在每次重启后从 0 重新计数。
5. WHEN 设备无可用 RTC, THE Firmware SHALL 将 `ts` 字段上报为设备毫秒计时值。
6. THE Firmware SHALL 在正常消息中将 `error` 字段上报为 `null`。
7. THE Firmware SHALL 使 `type` 与 `status` 的组合符合下表：telemetry→ok、heartbeat→ok、指令接受 ack→ok、指令拒绝 ack→warning、warning→warning、device_error→error。

### Requirement 6: telemetry 周期上报

**User Story:** 作为 App，我希望设备无论运行或空闲都以固定周期上报 telemetry，以便实时跟随设备状态。

#### Acceptance Criteria

1. WHILE 设备处于任意状态, THE Telemetry_Reporter SHALL 每 200 毫秒上报一条 `type:"telemetry"`、`status:"ok"` 的消息。
2. THE Telemetry_Reporter SHALL 在 telemetry 的 `data` 中上报 `deviceType`、`deviceState`、`sessionId`、`running`、`speed`、`targetSpeed`、`distance`、`calories`、`duration`、`countdown`、`temperatureState`、`manufacturerData` 字段。
3. THE Telemetry_Reporter SHALL 以 1 位小数上报 `speed` 与 `targetSpeed`、以 2 位小数上报 `distance`、以整数上报 `calories` 与 `duration`、以 1 位小数上报 `countdown`。
4. THE Telemetry_Reporter SHALL 以布尔类型上报 `running`，其值为跑带是否在转动。

### Requirement 7: heartbeat 周期上报

**User Story:** 作为 App，我希望设备周期上报心跳，以便确认通信正常。

#### Acceptance Criteria

1. WHILE 设备处于任意状态, THE Heartbeat_Reporter SHALL 每 2 秒上报一条 `type:"heartbeat"`、`status:"ok"` 的消息。
2. THE Heartbeat_Reporter SHALL 在 heartbeat 消息的 `data` 字段上报空对象 `{}`，并将 `error` 上报为 `null`。

### Requirement 8: 下行帧解析

**User Story:** 作为 App，我希望设备按固定 hex 帧格式解析下行控制指令，以便可靠下发控制。

#### Acceptance Criteria

1. THE Frame_Parser SHALL 按 `AA 55 [读写1B] [寄存器1B] [长度1B] [数据 len 字节] [帧尾 0x55]` 的结构解析下行帧。
2. WHEN 读写字节为 `0x02` 且帧合法, THE Frame_Parser SHALL 将数据写入对应寄存器并触发相应动作。
3. WHEN 收到寄存器 `0x00` 的合法写帧, THE Frame_Parser SHALL 按数据值映射控制动作（`00`停、`01`启、`02`暂停、`03`恢复）并交由 State_Machine 处理。
4. WHEN 收到寄存器 `0x01` 的合法写帧, THE Frame_Parser SHALL 将数据值除以 10 作为目标速度（mph）交由 State_Machine 处理。
5. THE Frame_Parser SHALL 不要求帧中包含校验位。

### Requirement 9: 坏帧处理

**User Story:** 作为设备，我希望对非法下行帧做确定性处理，以便不被脏数据影响且对可恢复错误给出反馈。

#### Acceptance Criteria

1. IF 接收到的字节序列帧头不等于 `AA 55`, THEN THE Frame_Parser SHALL 丢弃字节直到出现下一个 `AA 55`。
2. IF 帧尾不等于 `0x55` 或长度字段越界, THEN THE Frame_Parser SHALL 丢弃整帧, 且 THE Ack_Responder SHALL 不回 `ack`。
3. IF 读写字节不等于 `0x02`, THEN THE Frame_Parser SHALL 丢弃整帧。
4. IF 收到的帧寄存器为未知寄存器, THEN THE Ack_Responder SHALL 回 `status:"warning"`、`error.code` 为 `COMMAND_REJECTED` 的 `ack`。
5. IF 速度寄存器的数据值小于 `0x0A` 或大于 `0x26`, THEN THE Ack_Responder SHALL 回 `status:"warning"`、`error.code` 为 `COMMAND_REJECTED` 的 `ack`。

### Requirement 10: ack 应答

**User Story:** 作为 App，我希望在下发控制指令后及时收到 ack，以便确认指令已被接受或拒绝。

#### Acceptance Criteria

1. WHEN Frame_Parser 接收到一条合法 Control_Command, THE Ack_Responder SHALL 在 200 毫秒内上报一条 `type:"ack"` 的消息。
2. THE Ack_Responder SHALL 在 `ack` 的 `data.ackOf` 中上报对应动作名（`start`/`stop`/`pause`/`resume`/`setSpeed`/`confirmPairing`）。
3. WHEN 指令被接受执行, THE Ack_Responder SHALL 上报 `status:"ok"`、`error:null` 的 `ack`。
4. WHEN 指令被拒绝, THE Ack_Responder SHALL 上报 `status:"warning"` 且 `error` 含 `code`、`level`、`message`、`recoverable` 四个字段的 `ack`。
5. THE Ack_Responder SHALL 使 `ack` 表示"已收到并接受或拒绝"，且动作完成状态由后续 telemetry 的 `deviceState` 反映。

### Requirement 11: 状态×指令矩阵

**User Story:** 作为 App，我希望设备在每个状态下对每条指令给出协议定死的响应，以便交互行为可预测。

#### Acceptance Criteria

1. WHILE 设备处于 `idle` 或 `stopped`, WHEN 收到 `start` 指令, THE State_Machine SHALL 新建 session 并进入 `starting`。
2. WHILE 设备处于 `idle` 或 `stopped`, WHEN 收到 `pause`、`resume` 或 `setSpeed` 指令, THE Ack_Responder SHALL 回 `COMMAND_REJECTED` 的 `ack` warning。
3. WHILE 设备处于 `running`, WHEN 收到 `stop`、`pause` 或 `setSpeed` 指令, THE State_Machine SHALL 接受并执行该指令。
4. WHILE 设备处于 `pausing`, WHEN 收到 `start` 指令, THE Ack_Responder SHALL 回 `DEVICE_BUSY` 的 `ack` warning。
5. WHILE 设备处于 `paused`, WHEN 收到 `start` 指令, THE Ack_Responder SHALL 回 `COMMAND_REJECTED` 的 `ack` warning。
6. WHILE 设备处于 `stopping`, WHEN 收到 `start`、`pause`、`resume` 或 `setSpeed` 指令, THE Ack_Responder SHALL 回 `DEVICE_BUSY` 的 `ack` warning。
7. WHILE 设备处于 `error`, WHEN 收到 `start`、`pause`、`resume` 或 `setSpeed` 指令, THE Ack_Responder SHALL 回 `COMMAND_REJECTED` 的 `ack` warning。

### Requirement 12: 指令幂等

**User Story:** 作为 App，我希望在 ack 超时重发时设备幂等处理，以便重发安全。

#### Acceptance Criteria

1. WHILE 设备处于 `running` 或 `starting`, WHEN 收到 `start` 指令, THE Ack_Responder SHALL 回 `status:"ok"` 的 `ack`, 且 THE State_Machine SHALL 不重复启动。
2. WHILE 设备处于 `stopped` 或 `idle`, WHEN 收到 `stop` 指令, THE Ack_Responder SHALL 回 `status:"ok"` 的 `ack`。
3. WHILE 设备处于 `paused` 或 `pausing`, WHEN 收到 `pause` 指令, THE Ack_Responder SHALL 回 `status:"ok"` 的 `ack`。
4. WHILE 设备处于 `running` 或 `resuming`, WHEN 收到 `resume` 指令, THE Ack_Responder SHALL 回 `status:"ok"` 的 `ack`。
5. WHEN 收到与当前 targetSpeed 相同值的 `setSpeed` 指令, THE Ack_Responder SHALL 回 `status:"ok"` 的 `ack`。

### Requirement 13: 过渡态与倒计时

**User Story:** 作为 App，我希望设备在过渡态上报倒计时与对应状态，以便直接显示且无需本地计时。

#### Acceptance Criteria

1. WHILE 设备处于 `starting`、`resuming` 或 `stopping`, THE Telemetry_Reporter SHALL 上报对应的 `deviceState` 与递减至 0 的 `countdown` 秒数。
2. WHILE 设备处于非过渡态, THE Telemetry_Reporter SHALL 上报 `countdown` 为 0。
3. WHEN 倒计时从 3 递减到 0 且处于 `starting` 或 `resuming`, THE State_Machine SHALL 迁移到 `running`。
4. WHEN 设备进入 `stopped`, THE State_Machine SHALL 保持 `stopped` 至少 1 秒后再迁移到 `idle`。
5. THE State_Machine SHALL 作为状态与计时的唯一权威，由 telemetry 上报状态与倒计时。

### Requirement 14: session 生命周期

**User Story:** 作为 App，我希望通过 sessionId 判断是否新一次运动，以便重连后正确续接或新建。

#### Acceptance Criteria

1. WHEN 设备从 `idle` 或 `stopped` 启动进入 `starting`, THE State_Machine SHALL 自增 `sessionId` 并将 `distance`、`calories`、`duration` 清零。
2. WHEN 设备执行 pause 或 resume, THE State_Machine SHALL 保留 `distance`、`calories`、`duration` 不清零。
3. WHEN 设备因 stop 或严重错误结束运动, THE State_Machine SHALL 在进入 `idle` 后继续上报该 session 数据至少 60 秒。
4. WHEN 设备在保留期后再次 `start`, THE State_Machine SHALL 在该次启动时清零上一 session 数据。
5. WHILE 设备处于 `running`, THE State_Machine SHALL 在暂停期间停止 `duration` 增长。

### Requirement 15: targetSpeed 记忆

**User Story:** 作为 App，我希望设备按规则记忆目标速度，以便暂停恢复后自动回到原速度而无需重发调速。

#### Acceptance Criteria

1. WHEN 设备从 `idle` 或 `stopped` 启动, THE State_Machine SHALL 将 `targetSpeed` 重置为 1.0 mph。
2. WHILE 设备处于 `running`、`paused` 或 `resuming`, THE State_Machine SHALL 保留 `targetSpeed` 的值。
3. WHEN 设备从 `paused` 恢复到 `running`, THE State_Machine SHALL 将实际速度恢复到记忆的 `targetSpeed`。
4. WHEN 设备执行 stop 或进入 `error`, THE State_Machine SHALL 将 `targetSpeed` 重置为 1.0 mph。
5. THE State_Machine SHALL 将 `setSpeed` 的目标速度钳位在 1.0 至 3.8 mph，步进 0.1。

### Requirement 16: 过热降速、停机与冷却恢复

**User Story:** 作为用户，我希望设备过热时自动降速或停机并在冷却后恢复，以便安全运动。

#### Acceptance Criteria

1. WHEN Thermal_Manager 判定需要降速, THE Thermal_Manager SHALL 自动降低 `speed` 与 `targetSpeed` 至同一安全值，并将 `temperatureState` 上报为 `hot`。
2. WHEN 设备执行过热降速, THE Firmware SHALL 上报一条 `type:"warning"`、`status:"warning"`、`error.code` 为 `OVERHEAT_DOWNSPEED` 的消息。
3. WHEN Thermal_Manager 判定需要停机, THE State_Machine SHALL 进入 `error`，且 THE Telemetry_Reporter SHALL 上报 `running:false`、`speed:0`、`temperatureState:"overheat"`。
4. WHEN 设备过热停机, THE Firmware SHALL 上报一条 `type:"device_error"`、`status:"error"`、`error.code` 为 `OVERHEAT_STOP`、`error.level` 为 `critical` 的消息。
5. WHILE 设备处于过热停机后的 `error` 且 `temperatureState` 未回到 `normal`, WHEN 收到 `start`、`resume` 或 `setSpeed` 指令, THE Ack_Responder SHALL 回 `COMMAND_REJECTED` 的 `ack` warning。
6. WHEN 设备冷却完成, THE Telemetry_Reporter SHALL 上报 `deviceState:"idle"` 与 `temperatureState:"normal"`。

### Requirement 17: 主动错误与预警事件

**User Story:** 作为 App，我希望设备自发上报预警与严重故障，以便正确提示用户。

#### Acceptance Criteria

1. WHEN 设备检测到电机异常, THE Firmware SHALL 上报 `type:"device_error"`、`error.code` 为 `MOTOR_FAULT`、`error.recoverable` 为 `false` 的消息，并上报 `running:false`、`speed:0`。
2. WHEN 设备检测到传感器异常, THE Firmware SHALL 上报 `type:"device_error"`、`error.code` 为 `SENSOR_FAULT`、`error.recoverable` 为 `false` 的消息。
3. WHEN 设备触发急停, THE Firmware SHALL 上报 `type:"device_error"`、`error.code` 为 `EMERGENCY_STOP` 的消息，并上报 `running:false`、`speed:0`。
4. WHEN 设备检测到电源电压异常, THE Firmware SHALL 上报 `type:"warning"`、`error.code` 为 `LOW_VOLTAGE`、`error.level` 为 `warning` 的消息。
5. THE Firmware SHALL 在每条主动错误或预警消息的 `error` 对象中包含 `code`、`level`、`message`、`recoverable` 四个字段。

### Requirement 18: 状态字段值规范化

**User Story:** 作为 App，我希望 deviceState 与 temperatureState 仅取协议规定的枚举值，以便按固定集合解析。

#### Acceptance Criteria

1. THE Telemetry_Reporter SHALL 将 `deviceState` 取值限定为 `idle`、`starting`、`running`、`pausing`、`paused`、`resuming`、`stopping`、`stopped`、`error` 之一。
2. THE Telemetry_Reporter SHALL 将 `temperatureState` 取值限定为 `normal`、`hot`、`overheat` 之一。
3. THE Telemetry_Reporter SHALL 将内部状态 `SYS_STATE_STANDBY` 映射为 `idle`、`SYS_STATE_COUNTDOWN_START` 映射为 `starting`、`SYS_STATE_COUNTDOWN_RESUME` 映射为 `resuming`、`SYS_STATE_STOPPING` 映射为 `stopping`、`SYS_STATE_ESTOP` 与 `SYS_STATE_COMM_ERR` 映射为 `error`。
4. THE Telemetry_Reporter SHALL 上报速度范围在 0 至 3.8 mph 之间的 `speed` 值。
