# Loop Control Refactor and Engineering Application

## 1. What was refactored

The ACDC loop control path has been reworked into an engineering-ready dual-loop structure:

- Outer voltage loop: PI + anti-windup, decimated to 10 kHz equivalent.
- Inner current loop: PI + anti-windup, runs at ePWM ISR rate.
- Current reference generation: based on normalized real-time grid voltage (replaces triangular placeholder).
- ADC handling: grid and current channels are centered with sensor zero offsets.
- ISR timing: voltage loop -> current reference -> current loop -> PWM update in one deterministic chain.

## 2. Runtime flow (actual implementation)

In EPWM1 ISR:

1. `Adc_Cal_100KHz()`
2. `VoltLoopCtrl()` (internally decimated)
3. `Iref_Cal()`
4. `CurrLoopCtrl()`
5. `PWMDriver_Ctrl()`

This is the standard practical architecture for single-phase PFC digital control.

## 3. Engineering tuning guidance

### 3.1 Safe startup sequence

- Keep `b1LoopClose=0` during precharge and relay actions.
- Enable closed loop only after:
  - grid validity checks pass,
  - bus reaches minimum safe voltage,
  - no active faults.

### 3.2 Recommended first-shot gains

Current values in code:

- Voltage loop: `Kp=0.06`, `Ki=0.0015`, output `[0.0, 0.9]`
- Current loop: `Kp=0.25`, `Ki=0.008`, output `[0.05, 0.95]`

Bring-up method:

1. Start with current-loop-only (fixed low `iRef`).
2. Tune current loop until no high-frequency oscillation and fast tracking.
3. Enable voltage loop with low `Ki`.
4. Increase voltage-loop `Ki` until bus ripple and settling are balanced.

### 3.3 Anti-windup behavior

Back-calculation anti-windup is used in both loops:

- when output saturates, integrator is pulled back,
- reduces overshoot and long recovery after startup/load steps.

## 4. Practical deployment checklist

- Verify ADC zero offsets (`ADC_VAC_VOLT_ZERO_1V6`, `ADC_VAC_CURR_ZERO_1V5`) on real hardware.
- Calibrate scaling (`/4000`, `/3000`) against actual sensor gain and ADC reference.
- Scope verify:
  - inductor current THD,
  - bus overshoot during startup,
  - duty behavior around zero crossing.
- Recheck fault thresholds before full-power test.

## 5. Typical product scenarios

- 1 kW to 3 kW single-phase boost PFC front-end.
- Telecom rectifier AC-DC input stage.
- UPS rectifier front-end with controlled bus regulation.

This implementation now matches the common industrial software pattern: `slow voltage loop + fast current loop + bounded PWM command + anti-windup`.

## 6. Recommended Parameter Table (for first bring-up)

Assumptions:

- Current loop runs at 100 kHz ISR rate.
- Voltage loop runs with /10 decimation (10 kHz equivalent).
- Present normalization in code remains unchanged.

| Power Class | Vloop Kp | Vloop Ki | Vloop Out Max | Iloop Kp | Iloop Ki | Iloop Out Range | Iref Max | Vbus Soft Ramp Step |
|---|---:|---:|---:|---:|---:|---|---:|---:|
| 1 kW | 0.045 | 0.0010 | 0.75 | 0.18 | 0.0045 | 0.04 to 0.90 | 0.70 | 0.0004 |
| 2 kW | 0.060 | 0.0015 | 0.90 | 0.25 | 0.0080 | 0.05 to 0.95 | 0.90 | 0.0005 |
| 3 kW | 0.072 | 0.0022 | 0.95 | 0.32 | 0.0110 | 0.06 to 0.97 | 0.95 | 0.0007 |

Recommended tuning order:

1. Tune current loop first with a fixed small current reference.
2. Tune voltage loop with reduced Ki, then increase Ki gradually.
3. Increase Iref limit only after startup and line/load transitions are stable.
