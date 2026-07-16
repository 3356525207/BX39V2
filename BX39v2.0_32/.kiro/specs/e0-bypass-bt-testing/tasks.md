# Implementation Plan: E0 Bypass for Bench Testing

## Overview

This plan implements a compile-time gated bench-testing bypass that suppresses the E0
(UART1 ACK-timeout) communication error in the BX39 v2.0 firmware (Nations N32G003 MCU). The
core change is a single `#ifndef TEST_BYPASS_COMM_ERR` guard around the existing ACK-timeout
block in `myapp/ctrl_tx.c`, plus a discoverable macro placement and build-configuration wiring.
The E17 (safety-clip / PA4) and E2 (overheat / ESTOP) paths are deliberately left outside the
guard so they remain fully active.

Per the design's Testing Strategy, property-based testing is not applicable (the change is a
finite, compile-time configuration guard). Verification uses build-configuration checks,
example/table-based state-transition tests, and on-target bench integration checks. Since the
firmware is tightly coupled to hardware with no host test harness, the state-transition and
Bluetooth verification tasks are authored as scripted on-target checks unless a host build of
the module logic is set up.

## Tasks

- [x] 1. Implement the compile-time E0 bypass guard in `myapp/ctrl_tx.c`
  - [x] 1.1 Add the discoverable macro placement at the top of `myapp/ctrl_tx.c`
    - Insert a commented-out `/* #define TEST_BYPASS_COMM_ERR */` block immediately after the
      includes, with the documentation comment describing bench-vs-production usage and the spec
      reference (`.kiro/specs/e0-bypass-bt-testing`)
    - This is the single, discoverable in-source control location for enabling the bypass
    - _Requirements: 1.1, 1.2, 1.3_

  - [x] 1.2 Wrap the ACK-timeout block in the `#ifndef TEST_BYPASS_COMM_ERR` guard
    - In `CTRL_TX_Process()`, wrap only the `if (sys_state != SYS_STATE_COMM_ERR && (now - last_ack > 2000)) { ... }`
      block (sets `SYS_STATE_COMM_ERR`, `RCV315_SetSpeed(0.0f)`, `Beep_Tem = 800`) in
      `#ifndef TEST_BYPASS_COMM_ERR` / `#else` / `#endif`
    - Add the `#else` explanatory comment documenting that E0 is suppressed and state is left
      unchanged; leave `last_ack`, `ack_inited`, and the downstream-TX `switch` outside the guard
    - Do NOT touch the earlier safety-clip (E17 / PA4) block or the downstream `switch`
    - _Requirements: 1.4, 2.1, 2.2, 2.3, 2.4, 2.5, 6.1, 6.2, 6.3_

- [x] 2. Verify build configurations compile and the macro is confined
  - [x] 2.1 Verify the production build compiles unchanged (macro undefined)
    - Build the production target with `TEST_BYPASS_COMM_ERR` undefined; confirm it succeeds and
      that `CTRL_TX_Process()` still contains the timeout block (diff preprocessed output or the
      map/listing against the pre-change baseline)
    - _Requirements: 1.3, 6.1_

  - [x] 2.2 Add and verify a bench-test build configuration (macro defined)
    - Define `TEST_BYPASS_COMM_ERR` (via a dedicated BenchTest preprocessor define in the
      MDK-ARM `.uvprojx` and/or EWARM `.ewp`, or by uncommenting the in-source `#define`) and
      confirm the bench build compiles successfully
    - Leave the production target's defines unchanged
    - _Requirements: 1.1, 1.2_

  - [x] 2.3 Verify single-symbol confinement of the bypass logic
    - Grep the source tree to confirm `TEST_BYPASS_COMM_ERR` is referenced only at the guard in
      `myapp/ctrl_tx.c` and its definition site(s)
    - _Requirements: 1.4_

- [x] 3. Checkpoint - builds verified
  - Ensure both production and bench-test builds compile and the macro is confined. Ask the user
    if questions arise.

- [x] 4. Author state-transition verification for the E0 timeout path
  - [x] 4.1 Verify E0 is raised on timeout in the production build
    - Non-error start state, ACK false, advance time past 2000 ms, run `CTRL_TX_Process()` →
      `sys_state == SYS_STATE_COMM_ERR`, `g_safety_err == 0`, speed forced to 0,
      `Beep_Tem == 800`, display renders "E0"
    - Tag: `Feature: e0-bypass-bt-testing, Requirement 6`
    - _Requirements: 6.1, 6.2, 6.3_

  - [x] 4.2 Verify E0 is suppressed on timeout in the bench build
    - Same stimulus with macro defined → `sys_state` unchanged (not `COMM_ERR`), no "E0"
      rendered, speed not forced to 0 by the timeout, no 800 ms COMM_ERR beep; repeat over many
      cycles with ACK always false to confirm E0 is suppressed indefinitely
    - Tag: `Feature: e0-bypass-bt-testing, Requirement 2`
    - _Requirements: 2.1, 2.2, 2.3, 2.4, 2.5_

- [x] 5. Author safety-path preservation verification (bench build)
  - [x] 5.1 Verify E17 safety-clip path is preserved under bypass
    - With macro defined, drive PA4 high → `sys_state == SYS_STATE_COMM_ERR`, `g_safety_err == 1`,
      display renders "E17", and the CTRL_TX `switch` issues emergency stop/release as defined
    - Tag: `Feature: e0-bypass-bt-testing, Requirement 3`
    - _Requirements: 3.1, 3.2, 3.3_

  - [x] 5.2 Verify E2 overheat ESTOP path is preserved under bypass
    - With macro defined, hold monitored temperature > 130 °C for the 10 s confirmation window in
      `SYS_RUN_Process()` → `sys_state == SYS_STATE_ESTOP`, display renders "E2"
    - Tag: `Feature: e0-bypass-bt-testing, Requirement 4`
    - _Requirements: 4.1, 4.2_

- [x] 6. Author Bluetooth command-path verification (bench build)
  - [x] 6.1 Verify commands are accepted while UART1 is silent
    - With macro defined and no motor controller connected, feed valid UART2 frames (start 0x01,
      stop 0x00, pause 0x02, resume 0x03, setSpeed reg 0x01, mode 0x04–0x07) via `BT_ParseByte()`
      and confirm `BT_Matrix()` / `SYS_RUN_HandleBTCtrl()` produce the same transitions and `ack`
      responses as a production build in an equivalent non-error state
    - _Requirements: 5.1_

  - [x] 6.2 Verify no COMM_ERR interference after the 2 s silent window
    - Confirm the command matrix is evaluated against the real operating state
      (idle/running/paused/…), never against `SYS_STATE_COMM_ERR`
    - _Requirements: 5.2_

- [x] 7. Author on-target bench integration checks
  - [x] 7.1 Bench build end-to-end verification (no motor controller connected)
    - Script/document the on-target sequence: flash bench build → confirm no "E0" and no periodic
      800 ms beep after 2 s (Req 2); issue BLE start/stop/pause/resume/speed/mode → confirm
      normal acks/telemetry (Req 5); assert PA4 high → confirm "E17" and emergency-stop frames
      (Req 3); inject > 130 °C for ≥ 10 s → confirm "E2"/ESTOP (Req 4)
    - _Requirements: 2.1, 2.2, 3.1, 3.2, 3.3, 4.1, 4.2, 5.1, 5.2_

  - [x] 7.2 Production build end-to-end verification
    - Flash the production build and confirm "E0" appears within ~2 s with no controller
      connected, proving the two configurations differ only as intended
    - _Requirements: 6.1, 6.2, 6.3_

- [x] 8. Final checkpoint - Ensure all verification passes
  - Ensure all tests/checks pass and both build configurations behave as specified. Ask the user
    if questions arise.

## Notes

- Tasks marked with `*` are optional (verification/test tasks) and can be skipped for a faster
  MVP, though the safety-path checks (5.1, 5.2, 7.1) are strongly recommended before bench use.
- Property-based tests are intentionally omitted: the design has no Correctness Properties
  section because the change is a finite compile-time configuration guard (see Testing Strategy).
- The core code change is confined to `myapp/ctrl_tx.c`; no other source file is modified.
- E17 (PA4 safety-clip) and E2 (overheat ESTOP) paths are outside the guard and never disabled.
- Each verification task references the requirement it validates for traceability.

## Task Dependency Graph

```json
{
  "waves": [
    { "id": 0, "tasks": ["1.1"] },
    { "id": 1, "tasks": ["1.2"] },
    { "id": 2, "tasks": ["2.1", "2.2"] },
    { "id": 3, "tasks": ["2.3", "4.1", "4.2", "5.1", "5.2", "6.1", "6.2"] },
    { "id": 4, "tasks": ["7.1", "7.2"] }
  ]
}
```
