# Verification 5.1 — E17 Safety-Clip Path Preserved Under Bypass

**Feature: e0-bypass-bt-testing, Requirement 3**
**Validates: Requirements 3.1, 3.2, 3.3**

## Purpose

Prove that when the bench-test bypass macro `TEST_BYPASS_COMM_ERR` is defined and the
safety-clip input on GPIO **PA4** reads high, the firmware still:

- transitions to `SYS_STATE_COMM_ERR` with `g_safety_err == 1` (Req 3.1),
- renders **"E17"** on the 7-segment display (Req 3.2), and
- continues transmitting the downstream emergency-stop / emergency-release frames (Req 3.3).

Because the firmware is tightly coupled to hardware (GPIO PA4, UART1 ACK, `g_ms_tick`) and the
repository has **no host test harness and no ARM toolchain configured in this workspace**, this
is a documented **static state-transition verification** derived directly from the source. No
build/test was executed or fabricated. The equivalent on-target check is captured in Task 7.1
(step 3).

## Environment check (performed)

- `myapp/` contains no test files or host build harness (searched: no matches).
- No ARM cross-compiler invocation is available/expected in this workspace.
- Conclusion: verification is by source inspection of the exact code paths, per the design's
  Testing Strategy §2 (state-transition example/table checks executed as scripted/documented
  checks when no host build exists).

## Structural fact: E17 block is OUTSIDE the bypass guard

In `myapp/ctrl_tx.c`, `CTRL_TX_Process()` runs three watchdog checks in order. The safety-clip
(E17) block is **early** in the function and is **not** wrapped by any `#ifndef`:

```c
/* 安全扣脱落 → COMM_ERR */
if (safety_high) {                       /* safety_high = (PA4 == SET) */
    g_safety_err     = 1;                /* Req 3.1: g_safety_err = 1  */
    safety_releasing = 0;
    if (sys_state != SYS_STATE_COMM_ERR) {
        sys_state = SYS_STATE_COMM_ERR;  /* Req 3.1: → COMM_ERR        */
        RCV315_SetSpeed(0.0f);
        Beep_Tem = 800;
        g_evt_emergency_stop = 1;        /* FW-8 emergency-stop event  */
    }
}
```

The `#ifndef TEST_BYPASS_COMM_ERR / #else / #endif` guard appears **later** and wraps **only**
the UART1 ACK-timeout (E0) block:

```c
/* 通信超时 → COMM_ERR (2s 无应答) */
#ifndef TEST_BYPASS_COMM_ERR
    if (sys_state != SYS_STATE_COMM_ERR && (now - last_ack > 2000)) {
        sys_state = SYS_STATE_COMM_ERR;
        RCV315_SetSpeed(0.0f);
        Beep_Tem = 800;
    }
#else
    /* E0 suppressed for bench testing; state left unchanged. */
#endif
```

**Therefore, defining `TEST_BYPASS_COMM_ERR` has zero effect on the E17 block** — it lies
entirely above the guard and compiles identically in both build configurations. Confirmed by
grep: the macro is referenced only at this one guard (plus its documentation comment) in
`ctrl_tx.c` and its definition site in `MDK-ARM/LedBlink.uvprojx`.

## Display: "E17" render is untouched

In `myapp/SYS_RUN.c`, `SYS_RUN_UpdateDisplay()`, `case SYS_STATE_COMM_ERR` selects the render by
`g_safety_err` — no macro is involved:

```c
case SYS_STATE_COMM_ERR:
    if (g_safety_err) {              /* == 1 → E17 */
        display_buffer[2] = SEG_E;         /* 'E' */
        display_buffer[3] = SEG_TABLE[1];  /* '1' */
        display_buffer[4] = SEG_TABLE[7];  /* '7' */
    } else {                         /* == 0 → E0  */
        display_buffer[3] = SEG_E;
        display_buffer[4] = SEG_TABLE[0];
    }
    break;
```

With `g_safety_err == 1` the buffer renders `_ _ E 1 7` → **"E17"** (Req 3.2). This branch is
reached because the E17 block set `sys_state = SYS_STATE_COMM_ERR`.

## Downstream TX: emergency stop / release still issued

The CTRL_TX `switch` is **outside** the guard. For `SYS_STATE_COMM_ERR` it drives the safety
handshake purely off `g_safety_err` and the live `safety_high` reading (Req 3.3):

```c
case SYS_STATE_COMM_ERR:
    if (g_safety_err && !safety_high) {
        UART1_SendEmergencyRelease();   /* clip re-seated → release */
        safety_releasing = 1;
    } else {
        UART1_SendEmergencyStop();      /* clip still out → stop    */
    }
    break;
```

## State-transition table (bench build, macro defined)

| Step | Precondition | Event (PA4) | `sys_state` | `g_safety_err` | Display | UART1 TX |
|------|--------------|-------------|-------------|----------------|---------|----------|
| 1 | any non-error state | PA4 high (clip out) | → `COMM_ERR` | → 1 | **E17** | `UART1_SendEmergencyStop()` |
| 2 | `COMM_ERR`, `g_safety_err==1` | PA4 still high | `COMM_ERR` | 1 | E17 | `UART1_SendEmergencyStop()` |
| 3 | `COMM_ERR`, `g_safety_err==1` | PA4 low (clip re-seated) | `COMM_ERR` | 1 | E17 | `UART1_SendEmergencyRelease()`, `safety_releasing=1` |
| 4 | `COMM_ERR`, releasing | PA4 low + UART1 ACK received | → `STANDBY` | → 0 | (normal) | normal frames |

Steps 1–4 are identical with the macro defined or undefined, because none of this code is under
the guard. This satisfies Requirement 3 in the Bench_Test_Build.

## Result

**PASS (by source verification).** Under `TEST_BYPASS_COMM_ERR`:

- Req 3.1 — PA4 high → `sys_state = SYS_STATE_COMM_ERR`, `g_safety_err = 1`. ✔ (E17 block is
  outside the guard, unchanged)
- Req 3.2 — Display renders "E17" (`SEG_E`, `SEG_TABLE[1]`, `SEG_TABLE[7]`). ✔ (display branch
  keyed on `g_safety_err`, no macro)
- Req 3.3 — CTRL_TX `switch` issues `UART1_SendEmergencyStop()` / `UART1_SendEmergencyRelease()`
  per the safety handshake. ✔ (switch is outside the guard)

No source logic was modified. On-target confirmation of this path is tracked by Task 7.1 step 3.
