# 蓝牙协议 V2.1 — 走步机 / 律动机对接文档（定稿 r2）

硬件与 App 的对接契约。本版补齐异常路径与并发路径，**所有指令、字段、状态、错误返回均已定死，硬件按此实现**。

## 速览

| 项 | 约定 |
|----|------|
| 传输 | 蓝牙透传模块，双向字节流 |
| 下行（App→设备） | **16 进制帧**：`AA 55 [读写] [寄存器] [长度] [数据...] 55`，Write Without Response |
| 上行（设备→App） | **JSON 文本**（UTF-8），每条以 `\n`(0x0A) 结尾 |
| 单位 | 速度 **mph**、距离 **mile**（直接英里，不换算） |
| 速度 | 1.0 ~ 3.8 mph，步进 0.1，初始 1.0 |
| 上报 | telemetry 200ms/次（运行/空闲都发）、heartbeat 2s/次 |
| 并发 | **App 同一时间只允许 1 条未确认控制指令**（收到 ack 或超时后才发下一条） |
| 核心 | 设备是状态/计时的唯一权威；倒计时、状态、暂停恢复全由 telemetry 上报，App 只跟随 |

---

## 1. 蓝牙通道

### 1.1 UUID（固定）

> Service/特征 UUID **全部固定**，使用 MX-01P 透传模块的默认 16-bit UUID。

| 通道 | UUID | 方向 | 格式 |
|------|------|------|------|
| Service | `FFF0` | — | — |
| 上行 Read/Notify | `FFF1` | 设备→App | JSON |
| 下行 Write | `FFF2` | App→设备 | hex（Write Without Response） |

### 1.2 设备识别与厂商信息

- 蓝牙广播名格式为 **`TW-04<xxxx>`**，尖括号是名称的字面字符，`xxxx` 取 SN 后 4 位（例如 `TW-04<1A2B>`；SN 异常时回退 `TW-04<0000>`）。App 按 **`TW-04` 前缀**识别本款 V2 设备；连接后发现固定 `FFF0` Service。
- 型号 / 序列号 / 生产日期 / 是否首次配对，放在 **telemetry 的 `manufacturerData` 字段**里（见 §3.0，App 不需读 BLE 广播）。
- 走步机 vs 律动机由 telemetry 的 `deviceType` 区分（不靠 UUID）。
- **组合机（走步机 + 律动一体，如 BX39）**：同一物理设备按当前运行形态动态切换 `deviceType`——
  处于律动（振动）运行时上报 `deviceType:"rhythm"` + `mode`（§3.2）；其余状态
  （idle/starting/running/paused/resuming/stopping/stopped/error）上报 `deviceType:"treadmill"`（§3.1）。
  详见 §3.5。`deviceType` 可在会话内随形态变化，App 应按最新一条 telemetry 的 `deviceType` 渲染，
  并据 `mode` 恢复律动档位（重连后同样以首条 telemetry 为准）。

### 1.3 分帧与编码

- **上行 JSON**：UTF-8，每条以 `\n`(0x0A) 结尾，**不用 CRLF**（收到 CR 兼容忽略）；App 按 `\n` 切分缓冲再解析；单条 ≤ 480 字节（不含 `\n`）。
- **下行 hex**：靠帧头 `AA 55` + 长度 + 帧尾 `55` 解析，**不用 `\n`**，**无校验位**。

### 1.4 连接流程

```
扫描 `TW-04` 名称前缀 → 连接 → 发现 `FFF0`/`FFF1`/`FFF2` → 订阅上行 → 设备立即周期上报 telemetry+heartbeat → 收到即视为通信正常（无需握手）
```

---

## 2. 上行消息结构（JSON）

所有上行消息统一为这层结构，**字段全部必带**（`error` 正常为 `null`）：

```json
{ "type":"telemetry", "seq":1024, "ts":1779092555709, "status":"ok", "data":{}, "error":null }
```

| 字段 | 类型 | 必需 | 说明 |
|------|------|------|------|
| `type` | string | 是 | `telemetry`/`heartbeat`/`ack`/`warning`/`device_error` |
| `seq` | number | 是 | 设备递增序号，重启后从 0 |
| `ts` | number | 是 | 设备时间戳(ms)，无 RTC 给 0 |
| `status` | string | 是 | `ok`/`warning`/`error` |
| `data` | object | 是 | 业务数据，无内容给 `{}` |
| `error` | object/null | 是 | 正常 `null`，异常见第 7 节 |

**type × status 固定映射：**

| 场景 | type | status |
|------|------|--------|
| 遥测 | `telemetry` | `ok` |
| 心跳 | `heartbeat` | `ok` |
| 指令接受 | `ack` | `ok` |
| 指令拒绝（可继续） | `ack` | `warning` |
| 设备预警 | `warning` | `warning` |
| 严重故障 | `device_error` | `error` |

---

## 3. telemetry 数据（每 200ms）

### 3.0 manufacturerData 字段（telemetry 内）

每条 telemetry 的 `data.manufacturerData` 都带，告知厂商信息：

| 字段 | 类型 | 说明 |
|------|------|------|
| `model` | string | 设备型号，如 `TW-04` |
| `sn` | string | **设备唯一序列号**（出厂烧录、全局唯一），作为服务器保修记录主键；见 §3.4 |
| `productionDate` | string | 生产日期 `YYYYMMDD`，售后用 |
| `firstPairing` | boolean | 是否首次配对：`true`=此前从未完成 App 配对登记，`false`=已登记；**由设备持久化并上报**，翻转规则见 §3.4 |

### 3.1 走步机（`deviceType:"treadmill"`）

```json
{ "type":"telemetry","seq":1024,"ts":1779092555709,"status":"ok",
  "data":{ "deviceType":"treadmill","deviceState":"running","sessionId":12,"running":true,
           "speed":3.6,"targetSpeed":3.8,"distance":1.25,"calories":86,
           "duration":320,"countdown":0,"temperatureState":"normal",
           "manufacturerData":{ "model":"TW-04","sn":"TW-04-20260315-000123","productionDate":"20260315","firstPairing":true } },
  "error":null }
```

| 字段 | 类型 | 精度 | 说明 |
|------|------|------|------|
| `deviceType` | string | — | `treadmill` |
| `deviceState` | string | — | 见第 8 节状态机 |
| `sessionId` | number | 整数 | 本次运动会话号，每次 idle→start 自增（见 6.3） |
| `running` | boolean | — | 跑带是否在转 |
| `speed` | number | 1 位小数 | 当前实际速度 mph（0~3.8） |
| `targetSpeed` | number | 1 位小数 | 目标速度 mph（见 6.4 记忆规则） |
| `distance` | number | 2 位小数 | 本次累计 mile |
| `calories` | number | 整数 | 本次累计 kcal |
| `duration` | number | 整数秒 | 本次累计时长（暂停不增长） |
| `countdown` | number | 1 位小数 | 倒计时剩余秒数；非过渡态恒为 0 |
| `temperatureState` | string | — | `normal`/`hot`/`overheat` |
| `manufacturerData` | object | — | 厂商信息，见 §3.0，每条 telemetry 都带 |

### 3.2 律动机（`deviceType:"rhythm"`）

律动机**不带** speed/targetSpeed/distance，字段精简：

> 组合机（走步机+律动一体，如 BX39）在律动运行时也按本节格式上报 `deviceType:"rhythm"` + `mode`；形态切换规则见 §3.5。

```json
{ "type":"telemetry","seq":300,"ts":1779092555709,"status":"ok",
  "data":{ "deviceType":"rhythm","deviceState":"running","sessionId":5,"running":true,
           "mode":2,"calories":40,"duration":320,"countdown":0,"temperatureState":"normal",
           "manufacturerData":{ "model":"PF-R1","sn":"R1-20260315-000087","productionDate":"20260315","firstPairing":false } },
  "error":null }
```

| 字段 | 必需 | 说明 |
|------|------|------|
| `deviceType` | 是 | `rhythm` |
| `deviceState` | 是 | 状态机（律动机无 pause/resume，见 8.2） |
| `running` | 是 | 律动电机是否运行 |
| `mode` | 是 | 当前模式 1~4；**未运行为 0** |
| `calories` | 否 | 设备不计算则不带 |
| `duration` | 是 | 累计时长秒 |
| `countdown` | 是 | 倒计时剩余秒 |
| `temperatureState` | 是 | `normal`/`hot`/`overheat` |
| `manufacturerData` | 是 | 厂商信息（见 §3.0），每条都带 |

### 3.3 心跳

```json
{ "type":"heartbeat","seq":301,"ts":1779092555709,"status":"ok","data":{},"error":null }
```

### 3.4 首次配对与保修登记

- `firstPairing` 由**设备持久化**：出厂（或恢复出厂）为 `true`，表示从未完成过 App 配对登记。
- 设备**只**负责持久化并上报 `firstPairing`，**不存储保修日期**（设备无可靠 RTC）。保修日期由 App 用本地时钟生成、存服务器。
- App 处理流程（本协议只描述行为，不规定 App 时间来源与服务端实现）：

> **触发时机**：以**订阅上行 Notify 后收到的第一条合法 telemetry** 为准，**不是**蓝牙连接成功的瞬间。连接成功仅建立 GATT 链路，此时 App 尚无 `sn`/`firstPairing`（均在 telemetry 内）；订阅前 App 收不到任何 telemetry，订阅后每条都带完整 `data`。

```
连接成功(GATT) → 发现服务 → 订阅上行 Notify → 收到首条 telemetry → firstPairing==true ?
   ├─ 否：正常进入，不登记
   └─ 是：① App 用本地当前日期生成保修起始日期
          ② 连同设备标识(sn / model / productionDate)上传服务器登记保修
          ③ 登记成功后 App 下发「配对确认」指令(§4.3)
          ④ 设备收到后持久化 firstPairing=false，此后 telemetry 恒报 false
```

- **幂等保证**：若步骤②③任一失败（如登记前 App 掉线、确认指令丢失），设备仍报 `firstPairing:true`，下次连接 App 自动重试，不会重复登记（服务器按 `sn` 去重 / 更新）。
- `sn` 为保修记录主键，需出厂全局唯一（`model`+`productionDate` 不保证唯一）。

---

### 3.5 组合机 deviceType 动态切换（走步机 + 律动一体，如 BX39）

部分机型同一物理设备同时具备**走步机**与**律动（振动）**两种形态。此类设备**不新增 UUID / 不新增设备类型**，而是按当前运行形态在 telemetry 中动态切换 `deviceType`：

| 当前形态 | `deviceState` | 上报 `deviceType` | 字段规则 |
|----------|---------------|-------------------|----------|
| 律动运行（振动电机在转） | `running` | `rhythm` | 按 §3.2：带 `mode`(1~4)，**不带** `speed`/`targetSpeed`/`distance` |
| 其余全部状态（idle/starting/running/paused/resuming/stopping/stopped/error） | 对应 §6.1 状态 | `treadmill` | 按 §3.1：带 `speed`/`targetSpeed`/`distance` 等 |

规则要点：
- **切换时机**：进入律动模式（收到模式指令 `0x04~0x07` 并接受）即开始上报 `rhythm`；律动停止（`stop`）回到 idle 后恢复上报 `treadmill`。
- **mode 恢复**：App 以**最新一条** telemetry 的 `deviceType` 决定渲染形态；律动形态下用 `mode` 恢复当前档位。**重连后**同样以订阅后首条 telemetry 为准——因此 App 无需依赖 ack 缓存即可恢复"当前在几档律动"。
- 律动形态下 `mode` 恒为 1~4（正在振动）；本机型不上报 `deviceType:"rhythm"` 且 `mode:0` 的"律动空闲"帧（空闲统一按 `treadmill` 上报）。
- `manufacturerData.model` 始终为该物理设备型号（不因形态切换而改变）。

> 兼容性：纯走步机（永远 `treadmill`）或纯律动机（永远 `rhythm`）是本规则的两个退化特例，行为与 §3.1 / §3.2 完全一致，无需特殊处理。

---

## 4. 下行控制指令（hex）

### 4.1 帧格式（无校验）

```
AA 55   [读写]   [寄存器]   [长度]   [数据...]   55
帧头2B   1B      1B        1B       len 字节    帧尾1B
```

- `读写`：`02`=写（控制都用写）。
- `长度` = 数据字节数。
- **无校验位**（BLE 底层已有链路校验）。
- 帧头 `AA 55`、帧尾 `55` 固定。

### 4.2 寄存器表

| 寄存器 | 名称 | 数据 | 含义 |
|--------|------|------|------|
| `0x00` | 运行控制 | `00`停 `01`启 `02`暂停 `03`恢复 `04~07`律动模式1~4 | |
| `0x01` | 速度 | `0x0A~0x26` | mph×10（`0x1A`=2.6） |
| `0x02` | 坡度 | — | 不支持，保留 |
| `0x03` | 配对确认 | `01` | App 完成保修登记后下发，设备据此将 `firstPairing` 持久化为 `false`（见 §3.4） |

### 4.3 走步机指令

| 动作 | 帧 | ack.ackOf |
|------|----|-----------|
| 启动 | `AA 55 02 00 01 01 55` | `start` |
| 停止 | `AA 55 02 00 01 00 55` | `stop` |
| 暂停 | `AA 55 02 00 01 02 55` | `pause` |
| 恢复 | `AA 55 02 00 01 03 55` | `resume` |
| 调速 | `AA 55 02 01 01 [V] 55` | `setSpeed` |

调速例（V=mph×10）：1.0→`AA 55 02 01 01 0A 55`；2.6→`AA 55 02 01 01 1A 55`；3.8→`AA 55 02 01 01 26 55`。App 钳位 1.0~3.8。

**配对确认（走步机/律动机通用）**：`AA 55 02 03 01 01 55`，`ack.ackOf` = `confirmPairing`。App 仅在首次配对登记成功后下发一次（见 §3.4）。

### 4.4 律动机指令（`deviceType:"rhythm"`）

仅模式切换 + 停止；无启停/暂停/恢复/调速。

| 动作 | 帧 | ack.ackOf |
|------|----|-----------|
| 模式一 | `AA 55 02 00 01 04 55` | `mode1` |
| 模式二 | `AA 55 02 00 01 05 55` | `mode2` |
| 模式三 | `AA 55 02 00 01 06 55` | `mode3` |
| 模式四 | `AA 55 02 00 01 07 55` | `mode4` |
| 停止 | `AA 55 02 00 01 00 55` | `stop` |

### 4.5 坏帧处理（设备侧）

| 情况 | 处理 |
|------|------|
| 帧头 ≠ `AA 55` | 丢弃字节直到下一个 `AA 55` |
| 帧尾 ≠ `55` / 长度越界 | 丢弃整帧，不回 ack |
| 读写字节 ≠ `02` | 丢弃整帧 |
| 寄存器未知 | 回 `ack` warning + `COMMAND_REJECTED` |
| 速度越界（< 0x0A 或 > 0x26） | 回 `ack` warning + `COMMAND_REJECTED`（App 应已钳位） |

### 4.6 并发与写类型

- 下行特征为 **Write Without Response**，可靠性由上行 `ack` 保证。
- **App 同一时间只保留 1 条未确认控制指令**，收到 ack 或超时后才发下一条。调速滑动时 App 做节流，只发最终值。

---

## 5. ack 确认

- 设备**收到一条合法控制指令后 200ms 内回一条 `ack`**。
- **ack 表示"已收到并接受/拒绝"，不代表动作完成**；动作完成以 `telemetry.deviceState` 为准（如 start 的 ack 立即回，运行态由后续 telemetry 上报）。
- 匹配：因 App 单指令在途，按 `data.ackOf` 匹配即可。

成功：
```json
{ "type":"ack","seq":1026,"ts":1779092556100,"status":"ok","data":{"ackOf":"start"},"error":null }
```
拒绝（完整 error）：
```json
{ "type":"ack","seq":1027,"ts":1779092556200,"status":"warning","data":{"ackOf":"setSpeed"},
  "error":{ "code":"COMMAND_REJECTED","level":"warning","message":"Rejected in current state","recoverable":true } }
```

### 5.1 幂等（App 超时重发不会出错）

| 指令 | 设备已处于 | 返回 |
|------|-----------|------|
| start | running/starting | `ack ok`（不重复启动） |
| stop | stopped/idle | `ack ok` |
| pause | paused | `ack ok` |
| resume | running/resuming | `ack ok` |
| setSpeed | 同一速度 | `ack ok` |

App ack 超时（2s）重发最多 1~2 次；上述幂等保证重发安全。

---

## 6. 状态机与运行时序

### 6.1 状态机（走步机）

```
idle ─start→ starting(countdown 3→0) → running
running ─pause→ paused
paused ─resume→ resuming(countdown 3→0) → running（自动恢复到 targetSpeed）
running/paused ─stop→ stopping(countdown 3→0) → stopped(保持≥1s) → idle
任意 ─严重错误→ error
```

- 过渡态（starting/resuming/stopping）telemetry 带 `countdown` 秒数与对应 `deviceState`；App 直接显示 `countdown`、监听 `deviceState=running` 切 UI，**不本地计时**。
- `stopped` 至少保持 1 秒（或连发 ≥3 条 telemetry）再进 idle，确保 App 能捕获结束。

### 6.2 状态 × 指令矩阵（走步机，定死）

值含义：`A`=接受执行 · `OK`=幂等回 ack ok 不改变 · `BUSY`=回 ack warning `DEVICE_BUSY` · `REJ`=回 ack warning `COMMAND_REJECTED`

| 状态＼指令 | start | stop | pause | resume | setSpeed |
|-----------|-------|------|-------|--------|----------|
| idle | A(新session) | OK | REJ | REJ | REJ |
| starting | OK | A→stopping | A→paused | OK | A(改 targetSpeed) |
| running | OK | A | A | OK | A |
| paused | REJ(用 resume) | A | OK | A | A(改目标) |
| resuming | OK | A | A | OK | A |
| stopping | BUSY | OK | BUSY | BUSY | BUSY |
| stopped | A(新session) | OK | REJ | REJ | REJ |
| error | REJ | OK | REJ | REJ | REJ |

### 6.3 session 生命周期

- `idle/stopped → start` 进入 starting 时**新建 session**：`sessionId` 自增，`distance/calories/duration` 清零。
- pause/resume **不清零**。
- stop 或 critical error 后，设备**至少保留最后一次 session 数据 60 秒**；进 idle 后仍上报该 session，直到下次 start 才清零。
- App 用 `sessionId` 变化判断是否新一次运动（重连后据此决定续接还是新建）。

### 6.4 targetSpeed 记忆规则

- 从 `idle/stopped` 启动：`targetSpeed` **重置为 1.0**。
- `running/paused/resuming` 期间：`targetSpeed` 保留（暂停→恢复自动回到该速度，App 不重发调速）。
- `stop` 或 `error` 后：`targetSpeed` 重置为 1.0。

---

## 7. 错误处理

`error` 字段：`code` · `level`(`info`/`warning`/`critical`) · `message`(英文调试信息，**App 不展示**) · `recoverable`。
处理：`critical`→红色错误+结束运动；`warning`→黄色提示可继续；`recoverable:false`→提示联系售后。用户文案由 App 按 `code` 做 i18n（zh/en/es/de/it）。

### 7.1 主动事件（设备自发 warning / device_error）

| code | level | type | 含义 | 关键 data | App 处理 |
|------|-------|------|------|-----------|----------|
| `OVERHEAT_DOWNSPEED` | warning | warning | 过热降速 | `speed`/`targetSpeed`=降速后安全值，`temperatureState:"hot"` | 提示"已自动降速"，不回调 |
| `OVERHEAT_STOP` | critical | device_error | 过热停机 | `running:false`,`speed:0`,`temperatureState:"overheat"`,`deviceState:"error"` | 红色"过热已停止，请冷却"，结束运动 |
| `MOTOR_FAULT` | critical | device_error | 电机异常 | `running:false`,`speed:0` | 红色"电机异常，联系售后"（`recoverable:false`） |
| `SENSOR_FAULT` | critical | device_error | 传感器异常 | — | 红色"传感器异常"（`recoverable:false`） |
| `EMERGENCY_STOP` | critical | device_error | 急停 | `running:false`,`speed:0` | 红色"急停已触发"，结束运动 |
| `LOW_VOLTAGE` | warning | warning | 电压异常 | — | 黄色"电源电压异常" |

### 7.2 指令响应错误（ack warning）

| code | 含义 | App 处理 |
|------|------|----------|
| `DEVICE_BUSY` | 设备短暂不可处理（如 stopping 中收到 start） | **可稍后自动重试** |
| `COMMAND_REJECTED` | 当前状态无效/安全策略拒绝（如 error 态、未知寄存器、越界、冷却期） | **不自动重试**，提示用户 |
| `UNKNOWN_ERROR` | 兜底 | 提示重试，记日志；未知 `code` 一律按此处理 |

### 7.3 过热降速 / 停机 / 冷却恢复

- **降速**：设备自动降速（如 3.8→3.6），发 `OVERHEAT_DOWNSPEED`；telemetry `speed` 与 **`targetSpeed` 同步改为降速后安全值**、`temperatureState:hot`。App 以回传为准，不尝试恢复原速。
- **停机**：发 `OVERHEAT_STOP`，进 `error`，`running:false`/`speed:0`/`temperatureState:overheat`。
- **冷却恢复（定死）**：停机后处于 `error`；在 `temperatureState` 回到 `normal` 前，`start`/`resume`/`setSpeed` 一律回 `COMMAND_REJECTED`；冷却完成后设备上报 `deviceState:idle` + `temperatureState:normal`，App 才允许重新启动。

**过热停机标准回传：**
```json
{ "type":"device_error","seq":1025,"ts":1779092556000,"status":"error",
  "data":{ "deviceType":"treadmill","deviceState":"error","sessionId":12,"running":false,"speed":0,
           "targetSpeed":0,"distance":1.25,"calories":86,"duration":320,"countdown":0,
           "temperatureState":"overheat" },
  "error":{ "code":"OVERHEAT_STOP","level":"critical","message":"Overheat stop","recoverable":true } }
```

### 7.4 App 端自行判定（设备不发 JSON）

| 场景 | 判定 | App 处理 |
|------|------|----------|
| `NO_FEEDBACK` 无回传 | 6s 内无任何上行 | 连接异常→自动重连 |
| 连接断开 | BLE 断开事件 | 自动重连，按重连后 `deviceState`/`sessionId` 恢复 UI |
| ack 超时 | 发指令后 2s 无匹配 ack | 重发 1~2 次（幂等保证安全），仍失败则提示 |
| 非法 JSON / 缺字段 | 解析失败 | 丢弃该段，继续后续 |
| 未知 `type` | 不在集合 | 忽略 |

### 7.5 多事件优先级

`critical` 错误 > `warning` 错误 > ack 结果 > telemetry 常规更新。

---

## 8. 其它约定

- **MTU**：连接后 App 请求 512，设备配合。
- **重连**：App 自动重连并重新订阅上行；设备保持正常广播，重连后继续上报真实状态。
- **idle 频率**：空闲时 telemetry 仍 200ms/次（保证 App 实时拿到状态）。
- **坡度/电量**：不支持坡度、市电不报电量，协议无对应字段。

---

## 附：硬件实现清单

1. 固定 Service/上行/下行 UUID（第 1.1 节）；生产日期放广播 Manufacturer Data + 设备名。
2. 上行：200ms 发 telemetry、2s 发 heartbeat，JSON(UTF-8)+`\n` 结尾，字段按 §3 全带（含 `manufacturerData`：型号/`sn`/生产日期/firstPairing）、按精度给值。`firstPairing` 由设备持久化（出厂为 `true`），收到「配对确认」(0x03) 后持久化为 `false`；`sn` 出厂烧录、全局唯一。设备不存保修日期。
3. 下行：解析 `AA 55 02 [寄存器] [长度] [数据] 55`，**无校验位**（BLE 底层已有链路校验）；坏帧按 §4.5 处理。
4. 收到合法指令 200ms 内回 `ack`（`ackOf`，完整结构带 seq/ts/error）；ack 只表示收到，不代表完成。
5. 严格按 §6.2 状态×指令矩阵响应；幂等按 §5.1。
6. 倒计时（starting/resuming/stopping）带 `countdown` + 对应 `deviceState`；stopped 保持≥1s。
7. session 按 §6.3，带 `sessionId`；targetSpeed 记忆按 §6.4。
8. 过热：先降速（targetSpeed 同步降）后停机，冷却恢复按 §7.3。
9. 错误按 §7.1/§7.2 的 code/level/type/标准 data 发送。
10. 律动机 telemetry/指令按 §3.2/§4.4，不带速度类字段。**组合机（走步机+律动一体，如 BX39）**按 §3.5 动态切换 `deviceType`：律动运行时上报 `rhythm`+`mode`(1~4)，其余状态上报 `treadmill`；`deviceType`/`mode` 均取自当前实时状态，供 App（含重连后）恢复形态与档位。
11. 速度/距离英里制，最大 3.8、初始 1.0 mph。
