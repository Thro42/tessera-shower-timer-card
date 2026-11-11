# Verification — 0.1.0

Verified locally on 2026-11-11. No physical display was flashed and no live heating action was performed.

## Executed checks

- Official `tools/check.py`: exit **0**, `tessera-shower-timer-card: ok`, including the generated store image.
- C++ model tests: compiled with WSL `g++ -std=c++17 -Wall -Wextra -Werror`; **47 checks passed**.
  Covers parsing, 60:00/59:59 boundaries, pause/resume, expiry, missing clock/data and invalid temperatures.
- Official `tools/preview.py --strict`: exit **0**, **4 of 4 screens rendered, 0 warnings**.
  Screens: CYD, Guition square, Waveshare 4.3 portrait and Guition JC8012P4A1 10.1-inch.
  Both themes, active/off/paused/unavailable/unconfigured fixtures, supported sizes on each default grid.
  Twelve informational findings only: sizes that do not fit a board's grid are skipped as expected.
- The bench recorded the actual tile tap as `input_boolean.toggle` with entity
  `input_boolean.duschwassert`. Unknown/unconfigured controls are deliberately non-operable.
- 151 PNG files including the overview are in the ignored local `.preview/` directory;
  `.preview/index.html` and `.preview/summary.json` contain the detailed results.
- Inspected CYD and 10.1-inch renders in light and dark and the English `store.png`.
- Official `tools/store_image.py --from ...`: generated **1280×720**, about **137 KB**.
- `git diff --check`: exit **0**.

## Build environment and limits

The host-firmware bench ran in Ubuntu 22.04 under WSL with its cached Python 3.13/ESPHome environment and
user-local SDL2 libraries. It used the existing isolated Tessera host-test tree
`~/.cache/tessera-plugins/tessera-build-test-minpower`: its documented host stand-in adjustment is `min_power: 0.0`
for template float outputs. This avoids a core host-harness `set_min_power` build issue and changes no plugin
logic or physical board configuration. SDL2's include, architecture-library and PulseAudio paths were exported.
The first attempt lacked the SDL2 include directory, failed honestly, and was corrected before the passing run.

This is a **real Tessera host firmware build and render**, not a physical ESP32 target build, flash or live HA
integration test. Preview states are explicitly synthetic realistic HA fixtures. The timer attribute mapping uses
Tessera's real entity-option shaping (`finishes_at` is converted to epoch by its `_at` suffix).

English rendering was benched. All four languages have complete matching app/screen translation keys, but German,
Portuguese and Dutch still need visual confirmation on the configured physical display. Store text is English.
Memory/flash figures in the manifest are estimates, not physical-board measurements. The plugin remains **beta**.

## Real-device acceptance checklist

1. Install/build this plugin on the desired Tessera screen; enable HA action permission.
2. Select Robert's three entities from README.de.md (or other equivalent helpers).
3. Tap once: confirm HA toggles the boolean, sets the boiler target and starts the existing 60-minute timer.
4. Confirm the displayed countdown advances every second and is red while the boolean is on.
5. Tap again: confirm the existing automation lowers the setpoint and cancels the timer.
6. Let a test timer finish: independently verify HA's shutdown automation, including after an HA restart.
7. Check pause/resume, unavailable entities, reconnect, both themes and the desired screen language.

The local files have not been committed, pushed, released or installed in Home Assistant by this task.
