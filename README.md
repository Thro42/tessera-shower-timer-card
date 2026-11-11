# Shower Timer Card

[Deutsch](README.de.md) · English · Português and Nederlands are available in the editor and on the screen.

A tappable Tessera tile for your existing Home Assistant shower-water heating automation. Based on the supplied
button design: shower icon and title at the top, a large countdown in the centre, on/off at bottom left and the
current boiler temperature at bottom right. The whole tile is the button. It adapts to Tessera's light/dark theme.

## Set up

1. Keep your existing Home Assistant automation: an **input boolean** enables/disables heating and starts/cancels
   a **timer**. Timer completion must lower the boiler setpoint in Home Assistant. The plugin does not implement this.
2. Install the plugin in Tessera using **Plugins → Add with a link** and
   `https://github.com/Thro42/tessera-shower-timer-card` **after these local files have been pushed**. For local testing,
   copy this repository's plugin files into `/config/tessera-plugins/shower_timer/` in Home Assistant instead.
3. Add the plugin to your screen and build its firmware. Minimum plugin API: **0.15** (per-tile entity options).
4. In **Layout**, add the plugin tile **Shower water**, choose the **input_boolean** as its entity and select the
   **Shower-water timer** and **Boiler temperature** in its options. Entity selections can be changed without rebuilding.
5. Enable permission for the ESPHome device to perform Home Assistant actions. Use the tile's normal/default tap
   behaviour, not **None** or an overriding tap action. Save the layout.
6. Tap the tile and check your existing automation. A **2×1** tile most closely matches the reference design; sizes
   from **1×1** to **3×2** are supported. On narrow tiles the icon is omitted and a shorter title may be used.

### Robert's entities

| Selection | Entity |
|---|---|
| Main entity / heating switch | `input_boolean.duschwassert` |
| Timer | `timer.duchwasser_warm` |
| Temperature | `sensor.boiler_dhw_current_intern_temperature` |

These are examples, not hardcoded values. No extra HA template helper is needed.

## What it shows and controls

- One tap calls **`input_boolean.toggle`** on the selected main entity. Status changes only when HA reports it.
- The countdown is **red whenever the boolean is on**, also if its timer is paused or unavailable. Otherwise it uses
  the theme's normal text colour. Red uses Tessera's accessible `ALERT` tone, in both themes.
- An active timer uses HA's **`finishes_at`** and the screen clock. It updates once per second, even if HA does not
  resend the timer state. A one-hour timer starts at **60:00**, not 00:00. Other timer lengths work too.
- A paused timer shows its frozen **`remaining`** value; an idle/cancelled timer shows **00:00**. An expired active
  timer is clamped at zero until HA reports its new state. The tile never switches heating off by itself.
- Missing/unavailable timer data or an unset screen clock show **--:--**, not an invented countdown. Unknown switch
  states show **--** and block tapping. Invalid/unavailable temperature values show **--°**.
- Temperature has one decimal place and the sensor's own unit (no conversion). °C and °F use the compact **°** sign
  shown in the reference; Kelvin uses **K**. Select a real temperature sensor, not an arbitrary numeric sensor.
- English, German, Portuguese and Dutch translations cover the editor and the screen. The screen language follows
  Tessera's build language; a language change may require rebuilding. Decimal punctuation follows that language.

The countdown is informational, **not a safety controller or a replacement for the HA automation**. A lost connection
can leave the last switch/temperature state visible while a known countdown continues. The plugin sends no boiler
setpoint, timer-start, timer-cancel or timer-finished action. Verify your HA shutdown automation independently,
including behaviour across Home Assistant restarts.

## Privacy and AI involvement

The plugin uses your local Home Assistant entities and the existing Tessera connection only. It has no cloud fetch,
API key or separate network connection. **AI assisted with the implementation, translations, documentation and tests.**
The manifest declares `ai-developed`. The store image is generated from firmware-rendered preview fixtures, not an
AI-generated illustration. Preview data is synthetic and is not a claim that a live boiler was exercised.

## Development and verification

See [AGENTS.md](AGENTS.md) for the upstream rules. With the official plugins repository next to this one:

```sh
python3 ../tessera-plugins/tools/check.py .
python3 ../tessera-plugins/tools/preview.py . --strict
python3 ../tessera-plugins/tools/store_image.py . --from .preview
c++ -std=c++17 -Wall -Wextra -Werror tests/test_model.cpp -o /path/to/scratch/shower-timer-test
/path/to/scratch/shower-timer-test
```

See [TESTING.md](TESTING.md) for the actual verification results and outstanding real-device checks.

MIT License. Maintainer: [Thro42](https://github.com/Thro42).
