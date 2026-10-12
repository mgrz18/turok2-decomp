# The HUD

How Turok 2 draws its heads-up display, as far as the decomp has followed
it. Structs are in [`include/structs/player.h`](../include/structs/player.h)
(`CPlayer`, `COverlay`, `COnScreen`); addresses are US 1.0 vram, ROM offsets
are into `baserom.us.z64`.

## The pieces

| What | Where | Notes |
|---|---|---|
| Player HUD, drawn every frame | `func_0024C648(CPlayer *, CCamera *)` | runs each overlay's script, restarts it when its value changes, draws icons and numbers |
| Overlay script interpreter | `func_00277328(COverlay *)` | one step of a `COverlay`'s script per frame (below) |
| Start a script on an overlay | `func_0027AD00(COverlay *, int *script)` | `m_pScript = script; m_Mode = 2;` then one interpreter step |
| Draw a HUD sprite | `func_0027B320(id, frame, x, y, scaleX, scaleY, ?)` | 300 is the box behind ammo and health; 0x1F7 is the health icon |
| Draw a number | `func_00277CF0(value, x, y, scaleX, scaleY, ...)` | digit by digit through `func_0027A7A0` |
| Text | `COnScreen__DrawText`, `__SetFontColor`, `__SetFontScale` | the `COnScreen` global is `g_OnScreen`, 0x801100F0 |

Overlay positions are in a 320-wide virtual screen: `func_0024C648` maps
them through the camera's viewport (`CCamera.m_Viewport`, 0x288..0x294), so
they follow split screen. A negative X counts from the right edge.

## Overlays in `CPlayer`

| Overlay | Offset | Value shown | Script |
|---|---|---|---|
| `m_AmmoOverlay` | 0x1064 | ammo of the current weapon | `D_800ADB84`, restarted when the value changes |
| `m_HealthOverlay` | 0x10A0 | `(m_Data.m_Health + 0xFF) >> 8` (health is 24.8 fixed point) | none: always on, at its position |
| `m_LivesOverlay` | 0x10DC | `m_Data.m_Lives` | `D_800ADBC0`, restarted on change |
| `m_TokensOverlay` | 0x1118 | `m_Data.m_LifeForce` | `D_800ADC00`, restarted on change |
| `m_TotalKillsOverlay` | 0x1208 | multiplayer kills | `D_800ADC40`, restarted on change |
| `m_TotalTagsOverlay` | 0x1244 | multiplayer tags | `D_800ADC80`, restarted on change |

## Script language

A script is a list of 32-bit big-endian words: an opcode, then its
arguments. The interpreter (`func_00277328`) runs one step per frame; the
`m_Mode` it leaves says what the overlay is doing.

| Op | Name | Args | Effect |
|---|---|---|---|
| 0 | stop | — | `m_Mode = 1`, script cleared: the overlay stays where it is |
| 1 | set position | x, y | `m_PositionX/Y` at once, then the next op |
| 2 | wait | frames | `m_Mode = 3` counts `m_Time` down |
| 3 | slide to | x, y, speed | on screen; `m_Mode = 4` eases to (x, y) by 1/speed of the gap each frame |
| 4 | fall in Y | accel | `m_Mode = 5` accelerates down/up until off screen |
| 5 | fall in X | accel | `m_Mode = 6` the same sideways |
| 6 | end | — | `m_Mode = 0`, script cleared |

## The scripts in the ROM

| Script | vram | ROM | Steps |
|---|---|---|---|
| ammo | 0x800ADB84 | 0x0AE784 | slide to (120, −28) speed 2; stop |
| lives | 0x800ADBC0 | 0x0AE7C0 | slide to (16, 16) speed 2; wait 96; slide to (−64, 16) speed 4; end |
| tokens | 0x800ADC00 | 0x0AE800 | slide to (−40, 16) speed 2; wait 96; slide to (40, 16) speed 4; end |
| total kills | 0x800ADC40 | 0x0AE840 | slide to (−40, 42) speed 2; wait 96; slide to (40, 42) speed 4; end |
| total tags | 0x800ADC80 | 0x0AE880 | slide to (−40, 68) speed 2; wait 96; slide to (40, 68) speed 4; end |
| boss bar | 0x800B2C50 | 0x0B3850 | slide to (0, 0) speed 2; stop (`func_0027AD24`) |
| boss bar 2 | 0x800B2C64 | 0x0B3864 | slide to (0, 0) speed 2; stop (`func_0027AD54`, `COnScreen.m_BossOverlay2`) |
| VM modules | 0x800B2C30 / 0x800B2C40 | 0x0B3830 / 0x0B3840 | set position (0, −40) / (0, −20); stop |

So lives, tokens and the kill counters slide in for 96 frames whenever they
change and slide out again; ammo slides in and stays; health never moves.
Editing these words moves an overlay or changes how long it shows. The
health overlay's fixed position is set elsewhere, not found yet.
