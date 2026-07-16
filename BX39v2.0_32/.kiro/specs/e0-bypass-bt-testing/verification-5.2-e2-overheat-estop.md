# Verification 5.2 — E2 Overheat ESTOP Path Preserved Under Bypass

Feature: e0-bypass-bt-testing, Requirement 4
Validates: Requirements 4.1, 4.2

## Purpose

Prove that with the bench-testing bypass macro `TEST_BYPASS_COMM_ERR` defined, the
E2 overheat emergency-stop path is fully preserved:

- **Req 4.1** — When monitored motor temperature exceeds 130 °C (sustained for the
  confirmation window), the firmware transitions to `SYS_STATE_ESTOP`.
- **Req 4.2** — While in the E2 condition, the display renders "E2".

## Method

This is a documented **static / state-transition** verification, not a runnable test.
Rationale: the firmware targets the Nations N32G003 MCU (bare-metal, MDK-ARM /
EWARM). There is **no host test harness** in the repository and **no ARM toolchain
present in this environment**, so the E2 path cannot be exercised by a host unit test
or a fabricated build run. Instead the preservation is established by code inspection
of the exact symbols, thresholds, and the macro's compilation scope. No source logic
was modified.

## Key finding: the macro never touches the E2 path

A tree-wide search shows `TEST_BYPASS_COMM_ERR` is referenced only in:

| Location | Role |
|----------|------|
| `myapp/ctrl_tx.c` (line ~10) | Commented in-source default placement `/* #define TEST_BYPASS_COMM_ERR */` |
| `myapp/ctrl_tx.c` (lines ~74–86) | `#ifndef` / `#else` / `#endif` guard around the **UART1 ACK-timeout (E0)** block in `CTRL_TX_Process()` |
| `MDK-ARM/LedBlink.uvprojx` | BenchTest preprocessor `<Define>` that turns the bypass on |

The macro does **not** appear anywhere in `myapp/SYS_RUN.c`. The entire overheat/ESTOP
(E2) implementation lives in `SYS_RUN.c` (`SYS_RUN_Process()`, `SYS_RUN_EnterState()`,
`SYS_RUN_UpdateDisplay()`) and is outside every `#ifndef TEST_BYPASS_COMM_ERR` region.
Therefore the E2 path compiles **byte-for-byte identically** whether or not the macro is
defined — the bypass and the E2 path are structurally independent.

## Code evidence (as found in source, unmodified)

### 1. Temperature threshold — `SYS_RUN.c`, `SYS_RUN_Process()`, `case SYS_STATE_RUNNING`

```c
uint8_t temp = UART1_GetTemp();          /* monitored motor temp, °C, via UART1 */
uint8_t inst_level;
...
if      (temp > 130) inst_level = 4;     /* >130 °C → level 4 (ESTOP)          */
else if (temp > 125) inst_level = 3;
else if (temp > 120) inst_level = 2;
else if (temp > 110) inst_level = 1;
else                 inst_level = 0;
```

- **Actual threshold found:** strictly greater than `130` (`temp > 130`). `temp` is the
  value returned by `UART1_GetTemp()`.

### 2. Confirmation window — 10 s hysteresis

```c
if (inst_level != s_temp_pending_lvl)
{
    s_temp_pending_lvl  = inst_level;
    s_temp_stable_timer = 0;             /* interval changed → restart timer   */
}
else
{
    s_temp_stable_timer += elapsed;
    if (s_temp_stable_timer >= 10000)    /* 10 s continuous confirmation       */
    {
        s_temp_confirmed_lvl = s_temp_pending_lvl;
    }
}
```

- **Actual window found:** `s_temp_stable_timer >= 10000` ms = **10 seconds** of the
  level being held continuously before it is confirmed. `elapsed` accumulates the
  per-call delta of `g_ms_tick` (`SYS_RUN_Process()` runs ~every 50 ms).

### 3. Transition to ESTOP once level 4 is confirmed

```c
switch (s_temp_confirmed_lvl)
{
case 4:
    /* >130°C 持续 10s —— 触发急停保护 */
    SYS_RUN_EnterState(SYS_STATE_ESTOP);
    break;
...
}
...
/* 急停触发后跳过剩余 RUNNING 处理，下一周期进入 ESTOP */
if (s_temp_confirmed_lvl == 4)
    break;
```

- Confirmed level 4 (temp > 130 °C held for 10 s) → `SYS_RUN_EnterState(SYS_STATE_ESTOP)`,
  satisfying **Req 4.1**. `SYS_RUN_EnterState(SYS_STATE_ESTOP)` also zeroes speed
  (`RCV315_SetSpeed(0.0f)`) and sounds the long alarm, consistent with the design's
  state-transition table.

### 4. "E2" render — `SYS_RUN.c`, `SYS_RUN_UpdateDisplay()`

```c
case SYS_STATE_ESTOP:
    /* 电机过热急停 → 数码管显示 "___E2" */
    display_buffer[0] = SEG_BLANK;
    display_buffer[1] = SEG_BLANK;
    display_buffer[2] = SEG_BLANK;
    display_buffer[3] = SEG_E;         /* 'E'                                   */
    display_buffer[4] = SEG_TABLE[2];  /* '2' → renders "E2"                    */
    break;
```

- When `sys_state == SYS_STATE_ESTOP`, the two rightmost 7-seg digits are `E` and `2`,
  i.e. "E2", satisfying **Req 4.2**. This render is unconditional on the ESTOP state and
  independent of `g_safety_err` (which only distinguishes E0 vs E17 in the COMM_ERR case).

## State-transition verification table (macro DEFINED — bench build)

| Step | Stimulus | Confirmed level | `sys_state` | Display |
|------|----------|-----------------|-------------|---------|
| Start | Running normally, `UART1_GetTemp() ≤ 110` | 0 | `SYS_STATE_RUNNING` | speed |
| Inject overheat | `UART1_GetTemp()` reports > 130 °C | pending 4, timer accumulating | `SYS_STATE_RUNNING` (not yet confirmed) | speed |
| Hold < 10 s | temp still > 130 °C, `s_temp_stable_timer < 10000` | 0 (unconfirmed) | `SYS_STATE_RUNNING` | speed |
| Hold ≥ 10 s | temp still > 130 °C, `s_temp_stable_timer >= 10000` | **4** | `SYS_STATE_ESTOP` (via `SYS_RUN_EnterState`) | **"E2"** |

Because none of the symbols above (`UART1_GetTemp`, `s_temp_*`, `SYS_STATE_ESTOP`,
`SYS_RUN_EnterState`, `SYS_RUN_UpdateDisplay`, the display buffer) sit inside any
`TEST_BYPASS_COMM_ERR` conditional, this table is **identical** for the production build
(macro undefined) and the bench build (macro defined). The bypass provably cannot
disable or alter the E2 path.

## Result

PASS (by inspection).

- Req 4.1 satisfied: `temp > 130` °C confirmed over a 10 s (`>= 10000` ms) window →
  `SYS_RUN_EnterState(SYS_STATE_ESTOP)`.
- Req 4.2 satisfied: `case SYS_STATE_ESTOP` in `SYS_RUN_UpdateDisplay()` renders "E2".
- Bypass independence confirmed: `TEST_BYPASS_COMM_ERR` is confined to `myapp/ctrl_tx.c`
  and the MDK project define; it is absent from `myapp/SYS_RUN.c`, so the E2 path is
  unaffected by the macro.

## On-target confirmation (optional, when hardware + toolchain available)

Per design Testing Strategy §4, on a BX39 v2.0 board flashed with the bench build:
inject a > 130 °C motor temperature over UART1 for ≥ 10 s and confirm the display shows
"E2" and the motor is stopped. This documented verification stands in for that step in
the absence of a host harness / ARM toolchain in this environment.
