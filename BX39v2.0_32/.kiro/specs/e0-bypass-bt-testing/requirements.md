# Requirements Document

## Introduction

This feature adds a bench-testing bypass for the BX39 treadmill firmware (Nations N32G003 MCU) that suppresses the E0 communication-timeout error so that the Bluetooth (UART2) command path can be exercised on the bench without the downstream motor controller board connected.

Under normal operation, the Control_Transmit_Module runs an ACK watchdog in `CTRL_TX_Process()` (`myapp/ctrl_tx.c`). When no ACK is received from the downstream motor controller over UART1 within 2 seconds, `last_ack` stops updating and the firmware transitions to `SYS_STATE_COMM_ERR`, which the Display_Module renders as "E0" (`myapp/SYS_RUN.c`, `SYS_RUN_UpdateDisplay()`). During bench testing there is no motor controller connected, so `UART1_CheckAndClearAck()` never returns true and the firmware becomes stuck in the E0 state permanently. That state also zeroes speed, sounds an 800 ms beep, and continuously transmits `UART1_SendEmergencyStop()` (0xAA55), which prevents normal exercise of the Bluetooth command path.

The bypass is intended for bench/test builds only. It must be gated behind a compile-time flag so that production behavior is unchanged and the change is trivially revertible. Only the E0 (UART1 timeout) path is bypassed; the E17 safety-clip (PA4) path and the E2 overheat (ESTOP) path are genuine safety features that must remain fully functional.

## Glossary

- **Firmware**: The BX39 main controller firmware running on the Nations N32G003 MCU.
- **Control_Transmit_Module**: The periodic control-send module implemented in `myapp/ctrl_tx.c`, whose `CTRL_TX_Process()` function runs the ACK watchdog and drives downstream UART1 commands.
- **Display_Module**: The 7-segment display update logic in `myapp/SYS_RUN.c`, function `SYS_RUN_UpdateDisplay()`.
- **Bluetooth_Command_Path**: The UART2 command handling path comprising `myapp/bt_transparent.c` and `SYS_RUN_HandleBTCtrl()` in `myapp/SYS_RUN.c`.
- **ACK_Watchdog**: The 2-second downstream-ACK timeout logic inside `CTRL_TX_Process()` that transitions the Firmware to `SYS_STATE_COMM_ERR` when no ACK is received.
- **COMM_ERR_State**: The system state `SYS_STATE_COMM_ERR` (enumeration value 8).
- **E0_Condition**: The COMM_ERR_State caused specifically by the UART1 ACK timeout (rendered as "E0" when `g_safety_err == 0`).
- **E17_Condition**: The COMM_ERR_State caused by the safety-clip detection on GPIO pin PA4 (rendered as "E17" when `g_safety_err == 1`).
- **E2_Condition**: The overheat emergency stop state `SYS_STATE_ESTOP` (rendered as "E2"), triggered when motor temperature exceeds 130°C.
- **Bypass_Flag**: The compile-time macro (proposed name `TEST_BYPASS_COMM_ERR`) that enables the bench-testing bypass.
- **Production_Build**: A Firmware build in which the Bypass_Flag is not defined.
- **Bench_Test_Build**: A Firmware build in which the Bypass_Flag is defined.

## Requirements

### Requirement 1: Compile-Time Gating of the Bypass

**User Story:** As a firmware developer, I want the E0 bypass to be controlled by a single compile-time macro, so that production behavior is unchanged and the modification is trivially revertible.

#### Acceptance Criteria

1. THE Firmware SHALL provide a single compile-time macro named `TEST_BYPASS_COMM_ERR` that controls activation of the E0 bypass.
2. WHERE the Bypass_Flag is defined, THE Firmware SHALL enable the E0 bypass behavior.
3. WHERE the Bypass_Flag is not defined, THE Firmware SHALL retain the existing E0 timeout behavior unchanged.
4. THE Firmware SHALL confine all bypass logic to conditionally-compiled sections guarded by the Bypass_Flag.

### Requirement 2: Suppress the E0 Communication-Timeout Path

**User Story:** As a bench tester, I want the UART1 ACK-timeout E0 error suppressed during bench testing, so that the device stays out of COMM_ERR_State when no motor controller is connected.

#### Acceptance Criteria

1. WHERE the Bypass_Flag is defined, WHEN the ACK_Watchdog detects that no downstream ACK has been received for longer than 2000 milliseconds, THE Control_Transmit_Module SHALL keep the current system state unchanged instead of transitioning to COMM_ERR_State.
2. WHERE the Bypass_Flag is defined, WHILE no motor controller is connected, THE Firmware SHALL keep the system out of the E0_Condition indefinitely.
3. WHERE the Bypass_Flag is defined, WHILE the E0 timeout path is suppressed, THE Display_Module SHALL NOT render "E0".
4. WHERE the Bypass_Flag is defined, WHILE the E0 timeout path is suppressed, THE Control_Transmit_Module SHALL NOT force the commanded speed to zero on account of the ACK timeout.
5. WHERE the Bypass_Flag is defined, WHILE the E0 timeout path is suppressed, THE Control_Transmit_Module SHALL NOT emit the 800 millisecond COMM_ERR beep on account of the ACK timeout.

### Requirement 3: Preserve the E17 Safety-Clip Path

**User Story:** As a safety engineer, I want the E17 safety-clip detection to remain fully functional even in bench-test builds, so that the safety-clip protection is never disabled.

#### Acceptance Criteria

1. WHERE the Bypass_Flag is defined, WHEN the safety-clip input on GPIO pin PA4 reads high, THE Control_Transmit_Module SHALL transition the Firmware to COMM_ERR_State with `g_safety_err` set to 1.
2. WHERE the Bypass_Flag is defined, WHILE the E17_Condition is active, THE Display_Module SHALL render "E17".
3. WHERE the Bypass_Flag is defined, WHEN the E17_Condition is active, THE Control_Transmit_Module SHALL continue transmitting the downstream emergency-stop and emergency-release commands as defined for the safety-clip path.

### Requirement 4: Preserve the E2 Overheat Emergency-Stop Path

**User Story:** As a safety engineer, I want the E2 overheat emergency-stop protection to remain fully functional even in bench-test builds, so that thermal protection is never disabled.

#### Acceptance Criteria

1. WHERE the Bypass_Flag is defined, WHEN the monitored motor temperature exceeds 130 degrees Celsius, THE Firmware SHALL transition to the E2_Condition (`SYS_STATE_ESTOP`).
2. WHERE the Bypass_Flag is defined, WHILE the E2_Condition is active, THE Display_Module SHALL render "E2".

### Requirement 5: Normal Bluetooth Command Operation Under Bypass

**User Story:** As a bench tester, I want the Bluetooth command path to work normally while the bypass is active, so that I can exercise the UART2 command path without interference from the E0 state.

#### Acceptance Criteria

1. WHERE the Bypass_Flag is defined, WHEN a valid Bluetooth command is received over UART2, THE Bluetooth_Command_Path SHALL process the command using the same handling as in a Production_Build.
2. WHERE the Bypass_Flag is defined, WHILE no motor controller is connected, THE Bluetooth_Command_Path SHALL process speed, mode, and start/stop commands without being blocked by the COMM_ERR_State.

### Requirement 6: Production Behavior Unchanged

**User Story:** As a release engineer, I want production builds to behave exactly as before, so that shipping firmware retains all existing communication-timeout protection.

#### Acceptance Criteria

1. WHERE the Bypass_Flag is not defined, WHEN the ACK_Watchdog detects that no downstream ACK has been received for longer than 2000 milliseconds, THE Control_Transmit_Module SHALL transition the Firmware to COMM_ERR_State.
2. WHERE the Bypass_Flag is not defined, WHILE the E0_Condition is active, THE Display_Module SHALL render "E0".
3. WHERE the Bypass_Flag is not defined, WHILE the E0_Condition is active, THE Control_Transmit_Module SHALL force the commanded speed to zero and continuously transmit the downstream emergency-stop command.
