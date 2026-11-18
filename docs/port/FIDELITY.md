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

## 7. The port against the N64 member by member (Phase 7 round 17, part f3)

### 7.1 The tool: `tools/port/n64cmp.py`

Both sides start the same warp from the attract loop and dump the same structs at the same game
frames; libclang computes each struct's layout twice (mips-linux-gnu for the N64, the port's own
compile flags for the host) down to every scalar member, and the two dumps are compared by member
path.

```sh
python3 tools/port/n64cmp.py run --warp 7602 --level 4 --gf 1-30 --work /tmp/cmp \
    --port OUT/build/app/turok2 --archive OUT/turok2.o2r --build OUT/build [--skip m_VarBuffer]
python3 tools/port/n64cmp.py cmp N64.raw PORT.raw --build OUT/build [--gf 5] [--only LVL,SCN] \
    [--all-fields] [--brief] [--no-noise] [--all-union]
```

- `--level` is `gScene.m_nLevel` (4 the lair, 60 the hub); `--warp -(L+1)` starts level L at its
  default spawn. Records: `RNG` (gRandomValue), `SCN` (gScene), `PLR` (the first CPlayer), `CAM`
  (its camera), `LVL i` (gScene.m_pActors[i]), `DYN k` (the instance pool's active list).
- Port side (DEBUG-ONLY): `T2_RAW_DUMP=FILE:LEVEL:GF[-GF2][,...]` (port/game/t2det.c, called after
  `T2_RNG_SET`), `T2_RNG_LOAD=T:HEX` (gRandomValue set at the entry of `CScene__LoadLevel(scene, T)`,
  a TARGET_PORT hook in func_0042AC0C.c).
- N64 side: `n64trace.py --raw FILE:LEVEL:GFS` (stops after the last frame), `--rng-load T:HEX`
  (a breakpoint on CScene__LoadLevel), `--rng-set LEVEL:GF:HEX` (the port's `T2_RNG_SET` twin),
  `--ticks 2` (the four retrace counts CEngineApp__Update averages held at 2: the emulator's slow
  frames no longer change gFrameIncrement).
- `run` sets the seed at the load AND at gf 1 (see 7.3), holds the N64's ticks at 2, and runs the
  port with `--audio --draw` (a player's port draws and plays sound; both draw random numbers).
- Pointers compare by what they point at (null, a dumped record and its member, or "elsewhere");
  a union compares its first member only (the other views read the same bytes in another width);
  particle slots compare only when on an active list (free slots keep stale values).
- `NOISE` (left out unless `--no-noise`): padding, memory the game never cleared before its first
  write (blend state, shadow alpha, lights: zero on the fresh N64, old heap data in the port), the
  unique ids and animation start times (counters since boot), the camera's double-buffered
  viewport/projection/LookAt and the N64 Mtx fixed-point fields, the CList offsets (layout values).
  Still reported but known layout effects: `m_AI.m_VarBuffer[k>=4]` of AI views with an 8-byte
  pointer (the host words shift by one; the values agree: use `--skip m_VarBuffer`), u8 arrays that
  hold a word (`unk_18E54`, particle `unk_100`, `m_Geom[].m_Data[5]`: the same bytes in host order).

### 7.2 The lair props (e1's lead): a host bug, fixed

N64 `lvl 10-17` (type 80, interactive anims) start on animation 4 frame 0.5 at their spawn; the port
had animation 0, frame 1/3, and drifted ~1.7 units. n64cmp named `m_AI.m_AnimType` (N64 21501 =
0x53FD, port -1) and `m_AI.m_VarBuffer[0]` (N64 0x000053FD, port 0xFD530000).
`InteractiveAnim_Advance` copies the word after the intelligence's type flags; the archive's swap
plan has no struct for the type-9 (interactive anim) body (tools/assets/swap_spec.json "no struct
yet"), so its three words (type flags, start animation id, 0; 50 records, all 32 bytes) stayed
big-endian. `CAI__UpdateModeAnim` then got animation id 0xFD530000 from `InteractiveAnim_GetAnim`
and no animation; the type-flag tests (`m_dwTypeFlags & 1/2/0x20...`) read wrong bits too.

Fix: `CScene__DecompressIntelligence` (func_002261A8.c) and its VM copy (func_00465B04.c) byte-swap
the three body words of every type-9 record once per load (TARGET_PORT; asm_diff 0). After it the
lair's level actors, pooled instances and the player are identical to the N64's at gf 1-10.
(Fixing swap_spec.json instead would change the archive; if someone does, remove this swap.)

**It changes the legacy route**: the default command without the old behaviour ends in game over in
125 (~154,400). `T2_LEGACY_INTEL9=1` keeps the old bytes; `fullrun.py` and `playthrough.py` write it
into every `teleport.env` (the `--n64-shake` route too, which f1/f2 tune on b744bd39);
`--n64-props` leaves it out. With it the default run gives all 580,000 det lines md5
`ecb9f9f2c127d91cdf889e5b926f3d33` (round 16's), exit 0. A player gets the N64's props.

### 7.3 Random numbers: the sound requests and the load time

With the seed set at the load the port drew 142 numbers by gf 1 of level 53, the N64 170.
`--calls` on the four random functions (`n64trace.py`) against the port's `T2_RNG_TRACE`:

- **headless runs have no sound**: `Sound__Initialize: audio off (run with --audio)`, and every
  sound entry point returns before `CAudioWorld__PlaySound`, which draws random numbers (pitch,
  volume). A player (`--play`, sound on) and the N64 draw them. So **every scripted run (no
  `--audio`) is on a third RNG stream**, neither the N64's nor a player's. With `--audio` the port
  drew 146.
- the other 24 are the N64's audio thread during the load: `__amHandleFrameMsg` calls
  `Sound__Update` every retrace, and `CAudioWorld__Update` requests the ambient track every 4 audio
  frames, also while the game thread is loading (12 requests of sound 0xA in the lair/53 load frame).
  The port's load takes no retraces. This is load-time dependent (real or emulated DMA speed), not a
  host bug; n64cmp sets the seed again at gf 1. Its consequence: what the game draws between the load
  and gf 1 differs (the Primagen's six tossed objects at gf 0 pick other types: pool slots 3-5).

### 7.4 Results (gf 1-10/30, seed at load and gf 1, ticks 2, port `--audio --draw`)

| level | result |
|---|---|
| 4 (lair, 7602) | RNG equal every frame; LVL/DYN/PLR/CAM identical (after 7.2); left: the actor pool's tossed objects (7.3), `m_Resource` 0 in the port's pool actors vs a ROM key on the N64 (not chased) |
| 60 (hub) | LVL all equal at gf 1 and 30; `m_SkyAngle` offset (accumulated since boot, same rate 0.0436/gf), the static-event timer `unk_18EB8` phase (load timing), `CCamera.m_MenuPool` bytes |
| 53 | before the --audio/load fixes: particle type 226 spawn times differ (the static-event timer phase, 7.3), the rest equal apart from `m_VarBuffer` view shifts; not rerun with the final settings |
| 125, 135, 61, 100, 124, 114 | not run this round (the sweep script `run` per level is ready; ~3 min each) |

### 7.5 Next

1. Run the remaining levels (`n64cmp.py run` per level) with the final settings.
2. The pooled level actors' `m_Resource` (0 in the port).
3. The route on the N64 stream should be tuned with `--audio` (a player's stream), and with
   `--n64-props`; T2_LEGACY_INTEL9 can then go from `--n64-shake`.
4. `n64trace --watch-write` catches nothing (physical or virtual address): mupen's write
   breakpoints need another look.

## 8. The round-17 merge: the sound world headless, the n64cmp sweep of 125, 61 and 100 (Phase 7 round 17, the merge C32)

**7.3's first finding is acted on**: every game run with the ROM now runs the sound world (the
samples are dropped without `--audio`/`--wav`), so a scripted run draws the sound requests' random
numbers as a player with sound and the N64 do; `T2_LEGACY_NOSOUND=1` keeps the old silent stream
for the tuned routes (PLAYTHROUGH.md 35.1). A headless boot and an `--audio` boot give the same
16,000 det lines over 8,000 frames. Against the N64's RAM the demos are as before (demos 1 and 3:
no field but `rng`; the `rng` from gf 8, 7.3's load-time draws).

**The sweep** (`n64cmp.py run --warp -(L+1) --level L --gf 1-30 --brief`, the merged Release build,
`--audio --draw` on the port side as in 7.4):

| level | differing members (30 gf) | what differs |
|---|---|---|
| 125 | 9,115 | the `m_AI.m_VarBuffer` views (as in 7.4); the level actors' blend fields (`m_BlendAnimation.m_nAnimStartFrame`, `m_BlendLength`, `m_Blend`, `m_BlendPercent`: garbage on the N64, stale heap like 7.4's `m_Resource`); the camera's `m_mProjection` (the `Mtx` integer/fraction halves: representation) and `m_MenuPool`; `SCN unk_18E54[0-5]` and `m_Geom[0,1,4].m_Data[5]`; **actor 51 (type 135)**: `m_qGround` (0.014, 0, 0.19, 0.98) on the N64, identity on the port, so its `m_mfOrient` and bounds differ every frame; actors 52-53 (type 135): `m_Lights` colours and `m_ShadowAlpha`; the weapon actor's ambient light (5 vs 0) at gf 1 only; `gRandomValue` at gf 2, 5, 7, 9, 10 but equal at gf 3, 4, 6, 8 (draws between the game frames fall on the other side of the dump: the audio thread's timing, not a different count) |
| 61 | 11,407 | the `m_VarBuffer` views; particle 0 (position, velocity, attach point); **45-65 level actors' `m_Lights` (colours, directions) and `m_ShadowAlpha`** |
| 100 | 2,470 at gf 5 | the same: **57-83 actors' `m_Lights` and `m_ShadowAlpha`**, `_pad2` |

No host bug was confirmed in the time left. Two leads: **(1) the per-actor lights** (`m_Lights`,
`m_ShadowAlpha`) differ for most actors of 61 and 100 and some of 125, while the lair matched
(7.4): either the lighting code reads a light or the level's ambient through an N64-offset view, or
the port computes them at another point of the frame (the dump follows the drawn frame); start with
`n64trace.py --calls` on the function that fills `m_Lights` for one actor of 61. **(2) actor 51 of
125** keeps an identity ground quaternion: the ground alignment for type 135 (the `m_qGround`
writers: `func_00219BC0.c`, `func_0024C47C.c`, `func_0025C018.c`, ...) does not run or reads the
floor normal differently on the host.

## 9. The two round-17 leads (Phase 7 round 18, part g3)

Run with the round-17 settings (`n64cmp.py run --warp -(L+1) --level L`, seed at the load and at
gf 1, ticks 2, port `--audio --draw`, Release build at ab0bc24c): 61 (gf 1-5) and 125 (gf 1-10).

### 9.1 The per-actor lights (`m_Lights`, `m_ShadowAlpha`): not a host bug

`CGameObjectInstance.m_Lights[2]` is a double buffer written at `[gEvenOdd]` when the object is
drawn (`CScene__GetLightsAt`, `CScene__ComputeLights`, `gDefaultLights`). Two things made 8's
"lights differ for most actors":

- **the buffer parity**: the N64 and the port dump on frames of opposite `gEvenOdd` (the frames
  counted since boot differ with the load time), so the N64's `m_Lights[0]` is the port's
  `m_Lights[1]`. 61's type-366 actors 51 and 54: the same colours and direction
  ((56, 68, 69), (255, 255, 255), (-96, 82, -5)) in opposite slots.
- **never-written buffers**: an object not drawn yet keeps the cache allocator's old bytes on both
  sides (`AmbCol != AmbColCopy`, or the pools' 0xFF fill with a direction (-1, -1, -1)).

`n64cmp.py` now pairs the two buffers the way that matches more members (`pair_buffers`) and drops
a buffer that is unwritten on either side (`written`). On 61 at gf 1-5 the remaining LVL/DYN/PLR
differences are `m_ShadowAlpha` (an uninitialised byte, NOISE) and actor 25's ground tilt (9.2);
by an independent check at gf 1, 32 of the 34 buffers written on both sides at the same index
are equal, and the other two are the parity swap (actor 54) and the 0xFF fill (actor 26). `m_ShadowAlpha` stays in NOISE.

### 9.2 125's actor 51 (and 61's actor 25): the ground tilt reads uninitialised AI flags

Only `m_qGround`, `m_mfOrient` and the bounds of LVL51 differ in 125 (every other level actor is
equal at gf 1-2; the whole run differs in 3,035 members over gf 1-10, mostly the camera's menu pool and the m_VarBuffer views). N64 (0.0144, -0, 0.1907, 0.9815), port identity. Instrumented
`GameObject_GetGroundRotation` (temporary, not committed): it runs twice per level actor, first from
`CGameObjectInstance__Construct` (`m_qGround = GameObject_GetGroundRotation(obj)`, func_00219BC0.c)
**before the AI is set up**, then later. In the first call `obj->m_AI.m_dwFlags` is whatever the
cache memory held (port: 0xFFFFFFFF, 0xC318C318, 0xC5280529...; the actors' block comes from
`CCache__AllocateMemoryForDecompression` in `CScene__DecompressAnimatedInstances`, not cleared),
and `& 0xC0000` sends it to the identity. The region's floor normal on the port is
(-0.374, 0.927, 0.028): its `floorTilt` gives exactly the N64's quaternion, so on the N64 those
bits were clear. The later call (AI flags 0x8040) does not store into `m_qGround` for this actor
(type 1, `m_dwFlags` 0x70000: no 0x300000, so `func_0021A500` does not slerp it).

So this is the original code's read of uninitialised memory, not a host view bug: both machines
read old heap bytes, and the N64's happened to have the bits clear. Matching the N64 here would
mean modelling its heap contents (or clearing the block on load, which the N64 does not do); it is
left as is. Visible effect: a few placed objects on sloped floors stand upright on the port
instead of leaning with the floor (125's actor 51 at (3328, 1229, -4147), type 135).

### 9.3 Not done this round

The sweep of 135, 124, 114, 53, the hub after walking (`--pad`) and a boss arena, and the hub pad
cut-scene's particle colours (RENDER.md 7.1): the session lost most of its time to dropped
connections. Notes for the particle colours: the template colours are bytes (`CROMParticleColors`
copied raw in `port/game/t2particles.c`, no swap needed), `EntryColor`'s view in
func_0022D2FC.c matches the struct (hue/sat/val ranges at 0x0C-0x0E), and `Color_ShiftHue`
takes the hue as `u8` and reads it as `(s8)`; next step: dump one cinema particle's
`rgb0/rgb1` on both sides with `n64cmp.py run --warp 7601` (level 3) and compare the spawn
colour against the template's before the hue shift.

## 10. The round-18 merge: the default route on the N64 stream, the demos again (Phase 7 round 18, the merge C33)

- **The default whole-game route now runs with no legacy switch** (PLAYTHROUGH.md 38): the camera
  shake (round 16 e1), the type-9 props (round 17 f3) and the headless sound world (round 17 merge)
  as on the N64, the stream a human player gets. `--legacy` keeps the old one for the routes tuned
  on it.
- **The boot and the demos** (26,000 frames, `--pak-blank`, Release): the 52,000 det lines are
  round 17's build's on both streams (no switch: `197b2dd6…`; the three legacy switches:
  `3df81cf3…`). Against fresh N64 traces of this round (`n64trace.py` + `detdiff.py`, no switch):
  **demo 1 (gf 1-1,128) and demo 3 (gf 1-344) no difference in any field but `rng`** (from gf 8 of
  demo 1: the N64's audio thread draws during the load, 7.3); demo 2 the known `player.hp` (and the
  weapon actor's mirror of it) from gf 608, the N64's uninitialised read. As in round 17.
- g2's fixes are layout fixes: the stack-depth guards never fired in the usual layout and the
  multiplayer camera head is not read in a single-player game, so no N64-facing number moved.
- Not done this round (FIDELITY.md 9.3): the rest of the n64cmp sweep (135, 124, 114, 53, the hub
  with `--pad`, a boss arena) and the 7601 particle colours.

## 11-h2. The n64cmp sweep, the 7601 colours, the sound player's voice 16 (Phase 7 round 19, part h2)

Release build at 65ec98ec plus this part's commits; `n64cmp.py run --warp -(L+1) --level L --gf
1-20 --brief --skip m_VarBuffer` (seed at the load and at gf 1, ticks 2, port `--audio --draw`), as
in sections 7-9.

### 11-h2.1 The sweep

| level | differing members (gf 1-20) | what differs | verdict |
|---|---|---|---|
| 135 | 3,858 | **LVL31 (type 101) and LVL33 (type 102)**: `m_qGround`, `m_mfOrient`, the bounds (the port's tilt 16 times the N64's); particle slot 1 (type 226) one frame early on the port; the known representation members (`unk_100`, `unk_1A4`, `unk_18E54`, `m_Data[5]`, `m_MenuPool`) | 9.2's class, not a host bug (11-h2.2) |
| 124 | 7,085 | particle type 226 spawned one game frame earlier on the port (gf 1 against gf 2), so the particle slots and lists shift from there; the representation members | load timing (7.4's static-event timer phase), not a host bug |
| 114 | 898 | **LVL5**: the same ground tilt as 135; the representation members | 9.2's class |
| 53 | 4,282 | particle 0's spawn (position, velocity, attach point) from gf 1; slot lists | load timing, as in 7.4 |
| 60, the hub after walking | 34 at gf 250 and 300 | both sides walk forward (N64 `--pad 700-900:8`, the port `--input CU@1404..1804`: C-up, the stick only looks) to the same spot (x -44.6486, z -123.6992 on both at gf 250); only LVL25's `m_AI.m_ModeTime` / `m_AnimRepeatedCount` (an idle timer started at another frame) and the representation members. A first try with the stick (which only looks) left the players at the spawn with different views: then 24 actors' drawn flags and draw matrices differed, which is the view, not a bug | equal |
| 2, the Mantid Queen's arena | gf 1 equal; 4,251 over gf 1-20 | from gf 2 the N64's player and actors advance 12.87 time units in one frame (`m_FrameIncrementScaled`), the port 0.25: the arena's intro steps by elapsed time on the N64 (a long emulated frame), as a cinema does (RENDER.md 7.1) | timing, not a host bug |
| 3 from 7601 (the Mother's arena) | 12,147 over gf 1-60 | the cut-scene runs on `osGetTime`, so cameras and animation times part from gf 2; no particle is live on either side at gf 1-175 | see 11-h2.3 for the colours |

The representation members are the same bytes in another order (u8 arrays that hold a word or an
f32: `unk_1A4` is `Render.unk14`, an f32: N64 bytes 3D CC 00 00, host 00 00 CC 3D), as 7.1 says.

### 11-h2.2 The ground tilt of 135's LVL31/33 and 114's LVL5: the uninitialised AI flags again

`n64trace.py --calls GameObject_GetGroundRotation,CGameRegion__GetNormal,CVector3__Normalize`
around the load of 135 shows what 9.2 found for 125's actor 51, the other way round: the first
call (from `CGameObjectInstance__Construct`, before the AI is set up) takes the identity branch on
the N64 (its stale `m_AI.m_dwFlags` have a bit of 0xC0000 set) and computes the floor tilt on the
port; the second call (`func_0021A500`, the same on both: same region, same three vertices,
checked with the new `--peek`, same normal y 0.99996269) slerps `m_qGround` toward the tilt by
`gFrameIncrement * 0.125` = 1/16. So the N64 ends at 1/16 of the tilt, the port at the whole tilt.
An original-game read of uninitialised cache memory, left as is (as 9.2): placed objects on a
slightly sloped floor lean by up to 0.1 degree more on the port.

### 11-h2.3 The 7601 cut-scene's colours: two host bugs, one fixed visibly

RENDER.md 7.1 put the multicoloured light shaft and the cyan mist of the hub pad's cut-scene
(warp 7601, the Mother's arena) on the particle sprites. The dumps show no live particle on either
side at those frames; the shaft and the mist are textures of **kind 3**, CI4 texels with an IA16
palette (`Texture_Load`, func_00206B30.c: `gDPSetTextureLUT(G_TT_IA16)`; 4 such textures load in
this scene). libultraship's Fast3D decodes every CI palette as RGBA5551 and ignores the TLUT type,
so an intensity/alpha pair came out as a random saturated colour.

- **Fix** (`port/game/t2tex.c`, `func_00206B30.c` under `TARGET_PORT`, asm_diff 0): kind 3 loads
  its texels expanded to an IA16 image (each CI4 index replaced by its palette entry: the same
  intensity and the full 8-bit alpha) as `G_IM_FMT_IA` 16b, `G_TT_NONE`. Fast3D caches textures
  by address, so each (texels, palette) pair keeps its own buffer, and new bytes at the same
  addresses get a new buffer (never an old address with new contents). Shots (port frame 1701 /
  N64 emulator frame 690, gf 64/65; port 1925 / N64 779): the shaft is now the pale white-yellow
  cone the N64 draws and the mist dark grey, where they were cyan, green, magenta and blue.
- **The particle colours themselves are right**: in 124 at gf 20 every live particle has the
  N64's spawn colours (type 122: prim 255,255,255, env 179,0,255; type 226: prim 255,255,0, env
  255,115,0 on both sides), so `Color_ShiftHue`/`Color_Saturate`/`Color_Darken` and the template
  copy (RENDER.md 7.1's suspects) are cleared.
- **Still different**: the arena's walls and floor are olive-green on the port and brown on the
  N64 (Rice). The scene light is the same on both (`gSceneLights` ambient 56,56,56, colour 255,
  white, direction -80,57,-68: `--peek` against a port print), the textures of the scene are kinds
  0, 1 and 3 only (no detail/mipmap kind 4), and the hub at gf 75 gives the same picture on both
  sides; so the lead is this arena's own geometry display lists (their combiner, fog or a TLUT
  type set there) rather than the game state.
- **A second host bug found on the way** (`Texture_DecompressCallback`, func_00206990.c, asm_diff
  0): the callback gets the `CCacheEntry` being completed and read the recolour mode (the top two
  bits of the ROM address, `requestId`) at N64 offset 8, on the host the high half of
  `referencedMemEntry` (0): **the port never recoloured a texture**. It now reads `requestId` at
  its host offset, and recolours the TLUT entries big-endian (they stay in RDP byte order, swap
  spec "rdp"; a host u16 access would have taken the green/blue byte for red). No texture loaded
  in the scenes checked here asks for a recolour (all 68 requests of the 7601 run have the top bits
  clear), so no shot changed; whatever does will now get the N64's colours.

### 11-h2.4 The sound player's out-of-range targets (what the N64 reads)

`alSndpGetState` (func_00293580) returns `sndState[target].state`, the word at sndState + target *
0x30 + 0x28, with the target an s32 sign-extended from the s16 sound id. `n64trace.py --sndp`
(new) breaks on `alSndpGetState`/`alSndpStop`, prints the calls whose target is outside
0..maxSounds-1 and the word a target of -1 would read; `T2_SNDP_LOG=1` prints the port's.

- **The port** (the default run, 300,000 frames): 140 calls with target -1 (`StopSoundsByFlags`
  at level loads, through `Cinema_Start`/`CScene__LoadLevel`, stopping a voice of the group whose
  handle was already freed) and 4 with voice 16's `handles[16]`, the u16 sound count after the
  handle array: 18,400, 18,441, 20,873 and 21,728 (the crash of the round-18 merge).
- **The N64**: sndState is 0x807740D0 (in the audio heap, a cache block allocated at boot). The word
  a target of -1 reads (sndState - 8, the end of the block allocated before it) is 0 on every call
  of a level 53 run with its sounds, gunfire and a load (emulator frames 600-1,500): AL_STOPPED, no
  stop posted. A count of 11,942..32,767 puts the address past the end of the 8 MB of RDRAM, where
  the read gives 0 (the emulator: no memory there), so voice 16 late in a game is stopped too; only
  a count below 11,942 or above 32,767 would read game memory on the N64.
- So the port's guard (round-18 merge: a target outside the array reads as stopped and posts
  nothing) gives the N64's result in every case the default run reaches; the comment in
  `port/audio/t2audio_sndp.c` now says so. No code change.

### 11-h2.5 Tools

- `n64trace.py --sndp` (above) and `--peek LEVEL:GF:EXPR:N[;...]`: N words of RDRAM at a game frame;
  EXPR is a hex address followed by `.OFF` steps, each following the pointer at (address + OFF),
  e.g. `801ee630.10.4` = the first vertex of the region of the actor at 0x801ee630.
- `T2_SNDP_LOG=1` (port/audio/t2audio_sndp.c): the port's out-of-range sound player targets.
