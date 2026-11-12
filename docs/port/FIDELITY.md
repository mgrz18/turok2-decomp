# Fidelity: the port against an N64 reference (Phase 7 round 9, x3)

This page covers the reference ("oracle") built from the user's ROM on a headless N64 emulator:
how to run it, what it showed, and the per-scene differences. **Never commit a captured frame or
RDRAM dump.** They are derived from the ROM, so keep them in a scratch directory.

## 1. The oracle

Debian packages, installed on the box (headless; Xvfb is a virtual display, no window):

```sh
apt install mupen64plus-ui-console mupen64plus-video-rice mupen64plus-video-glide64mk2 \
            mupen64plus-video-z64 mupen64plus-rsp-hle mupen64plus-rsp-z64 \
            mupen64plus-input-sdl libmupen64plus-dev xvfb xauth
```

Plugin results under Xvfb (Mesa llvmpipe):

| video plugin | result |
|---|---|
| **Rice** (HLE) | **works**: correct frames, F3DEX2 lighting, fog, HUD. ~1,100 emulator frames/min with screenshots |
| Glide64mk2 (HLE) | every screenshot is black (972-byte PNGs), with or without its FBO wrapper |
| z64 + rsp-z64 (LLE) | too slow (no frame in 3 minutes); not usable |

There is no angrylion package, so this is an HLE reference: Rice's combiner and texture filtering
are not bit-exact to the RDP. Use it to compare **what** is drawn (objects, lights, colours,
layout), not pixel values. The CPU, the game logic and RDRAM are exact.

**`tools/port/refshots.py`** drives `libmupen64plus` through ctypes, the same way
`mupen64plus-ui-console --testshots` does, but it can also dump RDRAM and press buttons:

```sh
xvfb-run -a -s "-screen 0 1024x768x24" python3 tools/port/refshots.py \
    --rom baserom.us.z64 --out /tmp/ref --shots 1100,2000:2400:10 --rdram 1150,2700 \
    [--input "START@1500 A@1600 Z@2000..2050"] [--gfx rice] [--frames N]
# -> /tmp/ref/shot_<frame>.png (640x480), /tmp/ref/rdram_<frame>.bin (8 MB, N64 byte order)
```

- Controller 1 is a keyboard-mapped standard controller with a **Controller Pak**, a blank pak per
  run (`--keep-pak` keeps it), the same as the port's `--pak-blank`. With `--input dummy` the game
  stops at "CONTROLLER PAK NOT FOUND".
- Buttons: `A B Z START R L CU CD CL CR DU DD DL DR` and the stick `SL SR SU SD`;
  `NAME@F` is a short press, `NAME@F1..F2` holds the button (as in the port's `--input`).
- **Frame numbers are the emulator's**: one per new frame buffer, which is one game frame. The port's
  `--frames` and `T2_SHOT` count VIs. The ratio is not constant, so align scenes by event. Boot,
  emulator vs port: legal 50/298, expansion pak 450/1070, Acclaim 600/1362, Iguana cinema
  850/1850, title 1400/4212, demo 1 1990/5467, the legal screen again 3150/8900.
- RDRAM lookups: symbol addresses come from `build/ref.elf` (`nm`). An offset is
  `addr & 0x7FFFFF` into the dump. Structures are at their **N64** offsets, e.g. `gScene`
  0x800F7078 (`m_pActors` +0xE0, `m_nFirstActor` +0xE4, `m_nActors` +0xE8), instances 0x288
  bytes apart (`m_dwFlags` +0xD4, `m_Lights[2]` +0x110), `gSceneLights` 0x800AB0D0,
  `gEngineApp_m_Game_m_CameraPool_m_MainCamera` 0x801198F0 (`m_ViewMin/Max` +0x338).
  `gScene.m_nSectors` reads 0 between frames on both machines (the per-frame list is cleared),
  so lights that depend on it can only be compared from inside a frame.

## 2. Findings

### 2.1 Translucent instances and static objects were never drawn (fixed)

On the N64, the Iguana cinema shows the iguana on the logo, the iguana with the binoculars and the
iguana with the gun (emulator 850-1350). The port showed the jungle and the logo, but **no
iguana**. The RDRAM dump at 1150 shows actor #50 at (-2895, 358, -870), flags 0x13072305, lit
050505/ffffb5. The port has the same 52 actors, and #50 animates there too. Its flags have bit 4
(translucent), so `CScene__DrawRegisteredInstances` defers it to `CScene.m_pDeferred`, which
`CScene__DrawSortedList` (func_0022077C) draws later. That function read the list through an N64
view of the world (`count` at 0x1064, `items` at 0x1068). On the host those offsets land elsewhere
in `CScene`, so **no flag-4 instance and no deferred (translucent) static object was ever drawn**.
The fix is a `TARGET_PORT` view by host offset (`m_nDeferred`, `m_pDeferred`,
`CInstanceHeader.m_vPos`, `CStaticObject.m_Bounds`). After it, the three iguana shots match the
N64. asm_diff is 0.

### 2.2 The enemies' black silhouettes: the lighting matches the N64

The data was checked step by step against RDRAM:

- **The light volumes** (`LightVolume_Construct`, the 0xC0 records): in the Iguana level every
  record found in RDRAM by its colour has the same falloff, value, colour, inverse matrix and box
  as on the port.
- **`CScene__ComputeLights`** at the same point gives the same lights. Demo 1, level 81, enemy 6
  at (1974.4, 0, -904.4), emulator frame 2600: the N64 instance holds ambient `2d1a05`, colour
  `fff80b`, direction (c8 ee 70). The port, probed at (1974.4, 30..60, -904.4), gives ambient
  `2d1a05`, colour `fff80b`, direction (cc f8 73)/(cd f3 73). The Iguana level's actor lights
  (`22230f`/`e6ef6a`) are identical too.
- `gSceneLights` / `gDefaultLights` (383838, ffffff, (b0 39 bc) / (00 00 7f)) are identical.

So the actor lighting is faithful. The silhouettes in the level-52/81 test scenes are places
where the volumes give almost nothing: level 52 at the enemy, ambient `0a0c10` and colour
`0e1014`, because the bright volume there (93, 113, 125) is a sphere that does not reach the
enemy's centre. The N64 draws its enemies the same way in dark corridors: demo 1 at emulator
frame 2550 shows a dark figure with only its red eyes visible, and the enemies turn amber once
they reach the lit room (2600-2950). `G_MOVEMEM` lights, the normals and Fast3D's lighting were
not changed. No fix needed. Seeing it in one frame on both machines needs demo 1 to follow the
same path (2.4).

### 2.3 The legal screen's text is dimmer and more orange (fixed in round 11: 6.4)

At the same moment the N64's legal text peaks at (200, 200, 138), mean (110, 107, 73). The port's
peaks at (80, 64, 40), mean (62, 51, 32), the same on every frame from 401 to 901. Open: check the
text's primitive/environment colour and the IA font texture's intensity on the host (the fonts
are TARGET_PORT-swapped data).

### 2.4 Demo 1 does not follow the N64 from its first frame (both hosts)

On the N64 the demo starts in level 81 facing a door with the war blade. At ~2020 it opens the
wheel and picks the plasma rifle (on screen 2050). It walks through the door into the amber room
(2120-2200), meets the first enemy at ~2390 and takes damage 100 -> 92 -> 84 -> 67 -> 59 -> 57
-> 56 -> 51. The demo ends at ~3150. The port's first demo frame already has the wheel open
(`reqw 14`), move (1, 0) and buttons 0x101. The player spins in place (rotY +0.93 every 4 VIs),
picks the P.F.M. layer and never takes damage. The recorded input is read or applied wrongly
(the Demo_* bit streams, `gControllers_buttons`' 0x224 `Slot` view, `fastForward`). This is
x4's determinism subject, reported to the coordinator. Neither host matches the N64.

## 3. Per-scene table (Linux port at 5437cfc0 + 2.1, vs Rice)

| scene | emulator / port frame | geometry | actors | lighting / colour | HUD / text | verdict |
|---|---|---|---|---|---|---|
| legal screen | 300 / 601 | n/a | n/a | n/a | same layout, **text ~2x dimmer, more orange** (round 11: the same colour, 6.4) | differs (2.3) -> fixed |
| expansion pak | 500 / 1201 | same | same (the pak model) | same | same | match |
| Acclaim logo | 700 / 1701 | same | logo animation same | same | n/a | match (phase only) |
| Iguana cinema | 850-1350 / 2043-4143 | same jungle, fog, sky | **iguanas missing** -> present after 2.1 | same actor lights (22230f/e6ef6a) | logo letters same | fixed |
| title | 1500 / 4500 | n/a | n/a | same | same | match |
| demo 1 | 2000-3150 / 5467-8900 | level 81 same | enemies drawn | volumes and ComputeLights identical (2.2) | HUD same place | **path differs** (2.4) |
| level 52 combat (w2 script) | (no N64 run) | n/a | dark enemies | lights computed from the same data | n/a | consistent with 2.2 |

## 4. Next

1. Demo 1's input (2.4): with the demo following the N64 path, the emulator frames 2000-3150 can
   be compared shot by shot (enemies, amber lighting, damage).
2. The legal text's colour (2.3); then the HUD and menu text, which may share the cause.
3. A new-game route on the N64 with `--input` (keyboard presses through `M64CMD_SEND_SDL_KEYDOWN`
   are wired but not yet checked in a menu) so levels 51/52 can be compared by position.

## 5. Demo 1 against the N64's RAM, game frame by game frame (round 10, y3)

### 5.1 The tools

- **`tools/port/n64trace.py`**: runs the ROM on mupen64plus (the refshots.py setup, under
  `xvfb-run`) with the core's **debugger**: a breakpoint at `CEngineApp__Update` (the start of each
  game frame, so every read sees a finished frame; reading from the frame callback instead,
  `--at vi`, lands at a varying point of the frame and is noisy). There it reads RDRAM in place
  at the N64 offsets (computed from `include/` with `clang --target=mips-linux-gnu`, no
  TARGET_PORT; `--offsets` prints them) and writes the port's `T2_DET_LOG` lines: gf, ticks, the
  RNG seeds, the player, the weapon actor, every level actor and pooled instance and the live
  particles. `--calls SYM,...` prints every entry into those functions with `ra` and `a0-a3`;
  `--args REG:TYPE:MEMBER[:f|d|x]` reads members through an argument register or a global
  (`gCollisionResults:CCollisionResults:m_Frame.action:x`). ~1,000 emulator frames a minute
  (the cached interpreter: the debugger needs it).

```sh
xvfb-run -a -s "-screen 0 1024x768x24" python3 tools/port/n64trace.py --rom baserom.us.z64 \
    --from 1980 --to 3300 --actors > n64.det           # demo 1: emulator 1986 = gf 1
SDL_VIDEODRIVER=offscreen T2_DET_LOG=1 T2_DET_GF=1 T2_DET_ACTORS=1 T2_DET_FROM=5400 \
    build/app/turok2 --archive turok2.o2r --rom ../baserom.us.z64 --game --pak-blank \
    --frames 8300 > port.log
python3 tools/port/detdiff.py n64.det port.log --shift 0 --skip rng,flags
```

- `T2_DET_GF=1` (port): one line set per game frame (a new `gFrameCountGameplay`), with `gf`
  and `ticks`; `T2_DET_ACTORS=1` adds the weapon actor (`wpn`) and the particles (`prt`).
- **`tools/port/detdiff.py`**: aligns both by gf (`--shift 0` for n64trace's breakpoint
  reads; the port logs between frames: the same state) and prints the first frames whose fields
  differ (`--skip rng,flags,prt,dyn`).
- The RNG field is a phase marker only: in a demo `Random_Next` reseeds from
  `gFrameCountGameplay` on every call, so every draw of a frame returns the same number; the
  N64 draws once more between frames on even gf (a non-game thread), so `rng` differs on even
  frames with no effect.

### 5.2 The divergences found and fixed (asm_diff 0 each)

| first divergent field (gf) | cause | fix |
|---|---|---|
| player hp at gf 414: N64 100 -> 92, port 100 -> 99 | an impact event's 8-byte value reaches its handler as two host-order words (word 0 = the N64's (upper << 16) \| lower: the `CEventValue` convention). `Blast` read it by position: radius and damage swapped (damage 256 = 1 hp instead of 2048 = 8) | `Blast` (func_00237AFC, func_002374AC), `B15Blk8` (func_00239254: the floor sound) and `ParamsRecAt4` (func_00238EE4: the team sound) declared in host order under TARGET_PORT. Every particle hit (shots, explosions) did the wrong damage with the wrong radius |
| enemy 7 mode at gf 439: N64 18 (projectile attack), port 4 | `AI_InitHealth` (func_002427E0) wrote `m_Aggression` through an N64-offset view of CAI (0xC8): every enemy had aggression 0, so its attack delay was 60 instead of 15 (`EnemyMode_Attack_Enter`), and the aggression-scaled dodges and stuns were off | host view (hostview.py) |
| particle type 103 at gf 202, N64 only (**open, fix reverted**) | `CScene__AdvanceListedInstances` (func_002263C0) reads the timer, count and list at N64 offsets of CScene: on the host no listed static object ever fires its events (demo 1's ambient particle, every second) | a host body by member (3c790a0b) made demo 1 match here, but in demo 2 (level 131, gf 1) a static object's kind-8 event (func_00238134 -> Actor_SpawnParticle -> GameObject_TraceMove) crashed in `Collision_SweepRegions` (the static object's `m_pCurrentRegion` is its grid segment, not a host CGameRegion?). Reverted (b12d96c7); next step |
| particle hit flags (y4's finding) | `CParticleSystem__Spawn` read `gImpactEventHandlers[0xFFFF]` past the table | the N64 word 0x00C800FF (same fix as y4 627b899b: keep y4's) |

### 5.3 Where demo 1 stands

With the fixes (and 3c790a0b, see above), every traced field (the player, the weapon actor, the 24 level actors, the
pooled instances and every particle) is bit-identical to the N64 **from gf 1 to gf 416**, except
two transients with no lasting effect: enemy 21 (a generator) toggles mode 0 -> 1 -> 0 one frame
later than on the N64 (gf 236-238), and actor flag 0x200 (drawn this frame). At gf 417 the
player's first plasma shots leave from a point 0.18 units off sideways: the weapon actor's
position, rotation and animation are identical, but its `m_mfOrient` translation (and so every
bone matrix and the fire position) differs, because the **camera's `m_mfViewOrient` translation
already differs by (0.18, 0.03) in x/z at gf 412** (the rotation part is identical).
`CCamera__Update` (func_0027D160) adds the tremor and the camera shake (`CShake__ApplyOffset`)
to the view position: that is the next lead. From there the shot hits enemy 6 instead of 7
(gf 589) and the demo drifts. It now ends at 50 health on the port (N64 51; before these fixes 31).

## 6. Round 11 (z5): demo 1 to its end, static object events, the font colour

### 6.1 The camera line

`T2_DET_ACTORS=1` (port) and `n64trace.py --actors` (N64) now also write one `cam` line per game
frame for the first player's camera: the view parameters (`m_ViewParams.m_vPos`, `m_RotY`,
`m_vRotOffset`, `m_YPosOffset`), the tremor (`m_TremorAngle`, `m_TremorHeight`), the shake
(`m_Shake` per axis: value and amplitude) and the view translation (`m_vPos`, the previous
frame's `m_mfViewOrient` translation at the N64's read point). `detdiff.py` compares it as actor
`cam 0` (fields `vpx`..`posz`).

### 6.2 The divergences found and fixed (asm_diff 0 each)

| first divergent field (gf) | cause | fix |
|---|---|---|
| `cam.vpx` at **gf 74** (N64 player x - 0.14, port + 0.14; the rotation, the tremor and the shake identical) | `CPlayer__BobViewStride` / `CPlayer__BobView` (func_0024FE10 / func_0024FC08) read the body's yaw through an N64-offset view (`BobObject`, yaw at 0x50): 0 on the host, so the stride's side bob (`m_vEyeOffset.x/z`) went along the world x axis whatever the facing. `n64trace.py --calls CPlayer__BobViewStride --args a1:CGameObjectInstance:m_RotY:f` showed -3.1496 on the N64 | the view's pad at the host offset of `m_RotY`. This was round 10's "0.18 at gf 412" (it only reached the shots there) |
| particle type 103 at gf 202, N64 only (y3) | `CScene__AdvanceListedInstances` (func_002263C0) read CScene at N64 offsets: no listed static object fired its events | y3's host body by member again (3c790a0b, reverted in round 10) |
| demo 2 crash with that body (level 131, gf 1: `Collision_SweepRegions`) | `CStaticObject__Construct` / `__ConstructCompact` (func_00214564 / func_002141F0) set a static object's region (`header.m_pCurrentRegion`) to `regions + index * 32`: the N64 `CGameRegion` size; on the host a region is 0x40 bytes (pointer-wide vertex and neighbour slots), so the address landed inside another region and the kind-8 event's particle trace (`Actor_SpawnParticle` -> `GameObject_TraceMove`) swept garbage vertices | the host `CGameRegion` stride (as `CScene__GetRegion` already does) |
| demo 2 `player.vz` at **gf 426** (N64 -0 / 0, port -6.2e-39 decaying) | the game runs with the R4300's FS bit (denormal results flush to zero); the host kept denormals | every game thread sets the host's flush-to-zero in `t2os.c` (x86-64 MXCSR FTZ and DAZ, arm64 FPCR FZ); port code only |
| demo 3 `player.vy` at **gf 71** (N64 jumps: +249.12, port falls: -140) | `PlayerJump_Enter` / `PlayerJump_Update` (func_002555CC / func_00255670) read the jump scale, a float at 0x20 of the player's intelligence: it lies in `CPlayerIntelligence.pad08`, which the archive's swap plan leaves big-endian (1.0 read as 4.6e-41, flushed to 0), and `PlayerJump_Update` wrote the velocity through an N64 view (0x1C); every jump of the port left with no upward speed | read the float big-endian as `CPlayer__UpdateSpeedScale` already does for 0x1C, the velocity by member; `PlayerDodge_Enter` (func_00255840) had the same 0x1C view (fixed, not reached by the demos) |

### 6.3 Where the demos stand

- **Demo 1** (level 81): every traced field (the player, the camera, the weapon actor, the 24
  level actors, the pooled instances, every particle, the ambient particle included) is
  **bit-identical to the N64 from gf 1 to the end of the demo (gf 1,128)**; it ends at health
  12,814 (50.05, the HUD's 51) on both. `detdiff.py n64.det port.log --shift 0 --skip rng`:
  "no difference over the common game frames" (y3's two transients, the generator mode and the drawn flag, are gone too).
- **Demo 3** (level 109, 344 game frames): **bit-identical to the N64 from gf 1 to its end**
  (with the flush-to-zero and the jump fixes; before them it left at gf 71, where the N64
  player jumps).
- **Demo 2** (level 131, 789 game frames): bit-identical from gf 1 to gf 607 (with the
  flush-to-zero; before it, gf 426). At gf 607 a small enemy's (type 0x12C) animation event
  (an area blast, `CGameObjectInstance__FireAnimEvents` -> `ImpactEvent_AreaDamage`) leaves from
  (3.42, 270.51, -1920.27) on the port and (-3.47, 275.11, -1920.29) on the N64: the same
  offset turned by the enemy's ground tilt (`m_qGround`: z 0.029 on the port, 0.060 on the
  N64), so the player 30 units away takes 540 damage instead of 518 (health 24,695 vs 24,717
  at gf 608); every other traced field stays identical to the end of the demo. **Cause: the
  N64 reads uninitialised memory.** `CGameObjectInstance__Construct` (func_00219BC0) calls
  `GameObject_GetGroundRotation` before `CAI__Setup`, and that tests `m_AI.m_dwFlags & 0xC0000`:
  stale heap contents at level load. `n64trace.py --calls CGameObjectInstance__Construct
  --args a0:CGameObjectInstance:m_AI.m_dwFlags:x` shows the N64's leftovers (0xF7B7061F,
  0x20000000, 0xFE90FC45 for the three 0x12C enemies), the port's differ (0x0048FE61, 0x2000,
  0xFEC8FEC3): the third starts flat on the port (bit 0x80000) and tilted on the N64, and
  `func_0021A500` eases the tilt in afterwards (`T2_QDBG`-style prints were used, not kept). Not
  fixable by a host fix without copying the N64's heap contents; left as it is.

The commands (the N64 side once per demo; `--from 4915 --to 7870` covers demos 2 and 3):

```sh
xvfb-run -a -s "-screen 0 1024x768x24" python3 tools/port/n64trace.py --rom baserom.us.z64 \
    --from 4915 --to 7870 --actors > n64_d23.det
cd OUT && SDL_VIDEODRIVER=offscreen T2_DET_LOG=1 T2_DET_GF=1 T2_DET_ACTORS=1 T2_DET_FROM=12900 \
    build/app/turok2 --archive turok2.o2r --rom ../baserom.us.z64 --game --pak-blank \
    --frames 21200 > port_d23.log
python3 ../tools/port/detdiff.py ../n64_d23.det port_d23.log --level 131 --shift 0 --skip rng
python3 ../tools/port/detdiff.py ../n64_d23.det port_d23.log --level 109 --shift 0 --skip rng
```

On the N64 demo 2 runs from emulator frame 4922 to 5710 (level 131) and demo 3 from 7520 to
7864 (level 109); on the port from 13,037 and 19,922. `n64trace.py --calls` now also takes an
address (`func_0021A500`) for a function with no symbol name.

### 6.4 The legal screen's text colour (2.3, fixed)

`COnScreen__BeginText` (func_00277F6C) sets combine mode 0x1A, two cycles: colour
`(ENV - PRIM) * TEXEL1 + PRIM`, alpha `TEXEL0 * ENV`, with ENV (200, 200, 138) and PRIM
(86, 71, 47) (`COnScreen__SetFontColor`), and tile 1 over the font block with **line 0**. The
RDP's texel address is `tmem + t * line + s`, so with line 0 every row of tile 1 is row 0 of the
block, sixteen 0xF texels: TEXEL1 is 1 and the text is the environment colour. Fast3D takes the
line-0 tile's width from that line (0): an empty texture, TEXEL1 reads 0 and every text came
out in the primitive colour: the legal screen's (80, 64, 40). The host body emits, right after
mode 0x1A, the same combiner with ENV in place of the TEXEL1 mix (alpha unchanged). The legal
text now peaks at (200, 200, 136), the reference's (200, 200, 138); its mean is (129, 129, 88)
against Rice's (110, 106, 73): Rice probably reads tile 1 with a pitch from its width (rows 1, 3, ... of
the block are half 0xF, half 0), which gives its glyphs dark vertical stripes on their right
half; the hardware does not (line 0). Every text drawn after `COnScreen__BeginText` (HUD, menus,
messages) gets the same colour fix.

### 6.5 Next

1. Demo 2 after gf 607 depends on the N64's stale heap (above): a later comparison can skip that
   enemy's blast (or start a second alignment after it).
2. `CPlayerIntelligence.pad08` (0x1C speed, 0x20 jump scale, 0x24 the next float): name the
   fields in `include/structs/intelligence.h` and `swap_spec` so the archive swaps them, and drop
   the three big-endian reads (`CPlayer__UpdateSpeedScale`, `PlayerJump_Enter`, `PlayerJump_Update`).
   Needs an archive rebuild and a whole-tree asm_diff.
3. A new-game route on the N64 (levels 51/52) with `--input`, compared like the demos.
4. The rest of section 3's table (HUD, menus) against the reference with the colour fix.
