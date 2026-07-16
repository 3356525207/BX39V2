# Verification 4.2 — E0 Suppressed on Timeout (Bench Build)

**Feature: e0-bypass-bt-testing, Requirement 2**
**Validates: Requirements 2.1, 2.2, 2.3, 2.4, 2.5**

## Method

There is **no host test harness** and **no ARM toolchain** available in this workspace
(verified: `arm-none-eabi-gcc` and `armcc` are both absent from PATH; no test files exist under
the tree). The firmware logic is tightly coupled to hardware (`GPIO_Input_Pin_Data_Get`,
`UART1_CheckAndClearAck`, `RCV315_*`, `g_ms_tick`), so this task is executed as a **documented
state-transition verification** by static analysis of the actual source, exactly as the design's
Testing Strategy §2 prescribes for the "E0 suppressed on timeout" and "E0 suppressed
indefinitely" example/edge cases.

No source logic was modified for this verification. No compiler or test run was fabricated.

## Bench build definition

The bench-test build defines `TEST_BYPASS_COMM_ERR`. This is wired in the MDK-ARM project
(`MDK-ARM/LedBlink.uvprojx`), whose preprocessor defines include:

```
N32G003, USE_STDPERIPH_DRIVER, TEST_BYPASS_COMM_ERR
```

With the macro defined, the preprocessor selects the `#else` branch of the guard in
`myapp/ctrl_tx.c` `CTRL_TX_Process()`.

## Stimulus (same as 4.1)

Start in a non-error state (e.g. `SYS_STATE_STANDBY`), hold the mocked/absent downstream ACK
false (`UART1_CheckAndClearAck()` returns false because no motor controller is connected), and
advance `g_ms_tick` so that `now - last_ack > 2000`, then invoke `CTRL_TX_Process()` — repeatedly,
over many 200 ms cycles.

## Source under verification (`myapp/ctrl_tx.c`, unmodified)

```c
    /* ACK 看门狗检查 */
    if (UART1_CheckAndClearAck()) {
        last_ack = now;                     /* maintained by ACK-received branch (outside guard) */
        ...
    }

    /* 通信超时 → COMM_ERR (2s 无应答) */
#ifndef TEST_BYPASS_COMM_ERR
    if (sys_state != SYS_STATE_COMM_ERR && (now - last_ack > 2000)) {
        sys_state = SYS_STATE_COMM_ERR;     /* → E0                */
        RCV315_SetSpeed(0.0f);              /* zero speed          */
        Beep_Tem = 800;                     /* 800 ms COMM_ERR beep */
    }
#else
    /* TEST_BYPASS_COMM_ERR: E0 (UART1 ACK timeout) suppressed for bench testing.
     * State is left unchanged ... `last_ack` continues to be maintained by the
     * ACK-received branch above, so no other logic changes. */
#endif
```

## Analysis and results

With `TEST_BYPASS_COMM_ERR` defined, the entire timeout `if` block is **excluded from
compilation**; only the comment remains. Therefore, on the timeout event:

| Observable | Production (4.1) | Bench (this task) | AC |
|------------|------------------|-------------------|-----|
| `sys_state` after timeout | set to `SYS_STATE_COMM_ERR` | **unchanged** (stays e.g. `SYS_STATE_STANDBY`) | 2.1 |
| `RCV315_SetSpeed(0.0f)` called by timeout path | yes | **not called** | 2.4 |
| `Beep_Tem = 800` set by timeout path | yes | **not set** | 2.5 |
| "E0" rendered by display | yes | **never** (see below) | 2.3 |

- **2.1 — state unchanged.** The only statement that writes `SYS_STATE_COMM_ERR` on the timeout
  condition is inside the excluded block. No other code in `CTRL_TX_Process()` sets `COMM_ERR`
  from a timeout (the safety-clip block requires `safety_high`, i.e. PA4 high, which is a
  separate E17 condition, not the timeout). Hence `sys_state` is left unchanged. ✔
- **2.4 — speed not forced to zero by the timeout.** `RCV315_SetSpeed(0.0f)` on the timeout path
  is inside the excluded block, so it is not called on account of the ACK timeout. (Speed may
  still be set by other legitimate paths, e.g. E17/E2 or normal command handling — those are out
  of scope for the timeout suppression.) ✔
- **2.5 — no 800 ms COMM_ERR beep.** `Beep_Tem = 800` on the timeout path is inside the excluded
  block, so the timeout does not emit the COMM_ERR beep. ✔
- **2.3 — "E0" never rendered.** In `myapp/SYS_RUN.c` `SYS_RUN_UpdateDisplay()`, "E0" is drawn
  only inside `case SYS_STATE_COMM_ERR:` when `g_safety_err == 0`:

  ```c
  case SYS_STATE_COMM_ERR:
      if (g_safety_err) { /* E17 */ }
      else {              /* 通信超时 → E0 */
          display_buffer[3] = SEG_E;
          display_buffer[4] = SEG_TABLE[0];   /* 'E' '0' */
      }
      break;
  ```

  Because the timeout never drives `sys_state` to `COMM_ERR` under the bypass, and the timeout is
  the only path that enters `COMM_ERR` with `g_safety_err == 0`, this `else` branch is
  unreachable via the timeout. "E0" is therefore never rendered. ✔

- **2.2 — suppressed indefinitely.** `last_ack` is only updated inside the ACK-received branch
  (`if (UART1_CheckAndClearAck())`), which is **outside** the guard and behaves identically to
  production. With ACK always false, `last_ack` stays fixed while `now` keeps advancing, so
  `now - last_ack > 2000` is true on every subsequent cycle. In production this would fire the
  timeout block each cycle; under the bypass the block does not exist, so **no cycle can ever set
  `COMM_ERR` via timeout**, regardless of how many times `CTRL_TX_Process()` runs. E0 is
  suppressed for all cycles, i.e. indefinitely. ✔

### `last_ack` maintenance confirmed

The ACK-received branch that maintains `last_ack` sits before the guard and is unchanged by the
bypass. If a real controller were connected and responding, `last_ack` would refresh and the
timeout condition would never arise anyway — so the bypass changes behavior only in the silent
(bench) case, exactly as intended.

### Downstream TX under bypass

Since `sys_state` never becomes `COMM_ERR` via timeout, the `switch (sys_state)` at the end of
`CTRL_TX_Process()` continues to emit normal state-appropriate frames (speed/vibration/idle)
rather than `UART1_SendEmergencyStop()`. This is consistent with 2.x (no E0 side effects).

## Conclusion

All acceptance criteria for Requirement 2 (2.1–2.5) are satisfied in the bench build by static
analysis of the actual, unmodified source. The E0 timeout transition is fully suppressed —
state unchanged, no zero-speed, no 800 ms beep, no "E0" display — and remains suppressed
indefinitely across repeated cycles with ACK always false, because the sole timeout entry point
into `COMM_ERR` is excluded from compilation when `TEST_BYPASS_COMM_ERR` is defined.

**Verified files:**
- `myapp/ctrl_tx.c` — `CTRL_TX_Process()` timeout guard (`#ifndef`/`#else`/`#endif`) and
  `last_ack` maintenance in the ACK-received branch.
- `myapp/SYS_RUN.c` — `SYS_RUN_UpdateDisplay()` `SYS_STATE_COMM_ERR` / E0 branch reachability.
- `MDK-ARM/LedBlink.uvprojx` — bench build defines `TEST_BYPASS_COMM_ERR`.
