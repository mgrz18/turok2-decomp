# Port status after Phase 7 round 6 (merged state)

Round 6 merged five branches, in this order: u1 (gameplay draws the level: the cache hash
bucket stride, the camera visibility bits, the HUD), u5 (ABI and pointer width: indirect calls,
`--high-memory`), u4 (macOS parity and `--offscreen-gl`), u2 (animated actors) and u3 (stick
input, turning, the water, the weapon wheel). Then it fixed what only the merged tree reaches.
This page says how to build and run the result, how far it gets, and what comes next. The part
sections below keep each branch's own notes (detail pages: [RENDER.md](RENDER.md) sections 4-5,
[ABI.md](ABI.md) section 5, [64BIT.md](64BIT.md) part 4, [MACOS.md](MACOS.md) section 8,
[GAMEPLAY.md](GAMEPLAY.md) section 5, [INPUT.md](INPUT.md) section 6); round 5 and earlier
follow as history.

The matching build is unchanged: `tools/asm_diff.py --base 6d04fabb -j 8` reports 0 differences
over all 3,938 files after every merge and fix, and a clean `make clean && make setup && make
build && make verify` gives the identical ROM (CRCs good, 123 shared .rodata objects agree).

## 1. Build and run

```sh
# 0. the matching build first: gendata and asm_diff read its ELF; keep a copy as build/ref.elf
#    (the asm_diff/gendata tools expect it, and `make clean` deletes it)
make setup && make -j8 build && make verify && cp build/turok2.us.elf build/ref.elf

# 1. the archive, format 2.0. REBUILD IT this round: include/structs/region.h types the warp
#    ids (CRegionInfo 0x48/0x4C as u16), so the swap plans changed.
python tools/assets/mkarchive.py baserom.us.z64 -o OUT/turok2.o2r -j 4     # ~12 s
python tools/assets/archive_check.py OUT/turok2.o2r baserom.us.z64          # OK, format 2.0

# 2. configure; the game library AND the runtime/audio libraries (gendata reads all three)
cmake -S port -B OUT/build -G Ninja -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ \
      -DCMAKE_BUILD_TYPE=Debug
ninja -C OUT/build -j8 turok2_game t2runtime t2audio

# 3. the game data as C into port/gen/ (gitignored). Drop the cache after ANY header change
#    (this round: structs/model.h, structs/pose_host.h, structs/region.h).
rm -f OUT/decls.json
python3 tools/port/gendata.py --build OUT/build --cache OUT/decls.json -j 4      # ~1 min

# 4. re-run cmake so data.cmake sees port/gen/*.c, then the executable
cmake OUT/build && ninja -C OUT/build -j8 turok2

# 5. run headless (Linux: SDL's offscreen driver, no window). --pak-blank: a blank formatted
#    Controller Pak in memory (--pak-dir DIR: turok2_pak<N>.mpk files; --no-pak: none).
cd OUT && SDL_VIDEODRIVER=offscreen T2_SHOT=shot.ppm T2_SHOT_EVERY=300 \
    build/app/turok2 --archive turok2.o2r --rom ../baserom.us.z64 \
    --game --draw --pak-blank --frames 26000
```

Options (`turok2 --game`):

- `--input` (INPUT.md section 6): `BUTTON@F` (4 frames), `BUTTON@F1..F2` (held), `SX=V@F1..F2` /
  `SY=V@...` (stick, -128..127), `@FILE` (a script, `#` comments). Buttons: A B Z START DU DD DL
  DR L R CU CD CL CR. Controls: the stick turns/looks, C-up/C-down move, C-left/C-right strafe,
  R jumps, Z fires, A/B hold the weapon wheel (the stick picks the slot), L the map.
- **The scripted journey** (a new game to level 51 and through the harbour), as a file:

  ```
  START@4400 A@4500 A@4700 A@4800     # title -> main menu -> single player -> new game -> OK
  CU@13000..13440                     # level 51: down the ramp, into the harbour water
  SX=80@13450..13474                  # turn right about 90 degrees
  CU@13480..14000                     # swim across the basin to the far wall
  A@14050..14090 SX=80@14060..14090   # weapon wheel open, stick right: weapon 2 requested
  Z@14150..14250                      # fire
  SX=-80@14300..14324                 # turn back left
  CU@14330..14800
  ```

  `T2_STATE_LOG=60 build/app/turok2 ... --game --draw --pak-blank --input @journey.txt --frames 15000`
- `--audio` / `--wav out.wav`: sound (sfx, music, speech).
- `--high-memory` (Linux): no 32-bit arena, no low stacks: host pointers above 4 GB, the
  closest thing to macOS on the box.
- `--offscreen-gl` (**macOS**): Fast3D's OpenGL renderer on a CGL context with no drawable and
  no window; `T2_SHOT` reads the game framebuffer (MACOS.md section 8). `--no-window` (any
  host): a virtual pad on port 1, no pixels.
- Logs: `T2_STATE_LOG=60` (mode, level, the player: position, rotation, state, region, floor,
  the requested weapon, the wheel, the weapon actor's AI and animation), `T2_SCENE_LOG` (the
  camera view box, the visible box, the loaded grid sections), `T2_MENU_LOG=1`,
  `T2_DLDUMP_AT=<frame>` (+ `T2_DLDUMP_VTX=1`, `T2_DLDUMP_MAX`), `T2_V2P_TRAP=1` (trap truncated
  addresses), `T2_ANIM_EVENTS=1` (animation events on: crashes, section 4). Debug:
  `--skip-pak-check`, `--start-mode N`, `gdb -batch -ex run -ex bt --args ...`.

gendata on this tree: 1,537 data symbols (1,531 typed from a C declaration), 93 pointer targets
typed as tables, 386 zero .bss symbols, 4,041 pointer slots (2,932 relocated, 1,077 null, 32
unresolved). The archive: 26,124 units, sha1 `47e8f73c2bb7304f7f6c3e0ac34ad478f1bdf15f`;
`turok2` without `--game` reports fnv1a64 `c7909f8a7049feac` on all 3 backends.

## 2. How far it gets (Linux, headless; macOS per u4)

The boot loop, `--game --draw --pak-blank --frames 26000` (158 s, no crash):

```
frame 2:     mode 0    pak check;   frame 298: mode 1, the legal screen
frame 1070:  mode 3    "EXPANSION PAK FOUND!", the 3D pak
frame 1362:  mode 4    cinema 100: THE ACCLAIM LOGO RENDERS AND ANIMATES (an animated actor)
frame 1850:  mode 5    cinema 90: the Iguana jungle
frame 4212:  mode 7    TITLE SCREEN
frame 5467:  mode 13   attract demo 1 (level 81): the level, the Talon in Turok's hand, the HUD
frame 8800:  the boot again; 13680 attract demo 2 (level 131); 20946 attract demo 3 (level 109)
frame 25778: the title again; frame 26000
```

The new game with the scripted journey (`T2_STATE_LOG=60`, 15,000 frames, no crash):

```
frame 4455:  mode 8    main menu;  4555 mode 13, the single-player menu
frame 4855:  mode 11   new-game cinemas: Adon and Turok, the Port of Adia, soldiers, enemies
frame 8973:  mode 12   the next cinemas (levels 47, 45, 46, 48, 49), the warp portal
frame 12895: mode 13   LEVEL 51 GAMEPLAY: the Port of Adia harbour draws with the HUD
                       (health 100) and the Talon in Turok's hand
13080 (-1540.7 -735.8  -982.4) state 3, walking down the ramp (C-up)
13200 (-1530.8 -789.8  -485.4) state 9, in the harbour water
13440 (-1164.7 -780.7   -29.0) turn right (SX=80): rotY -3.14 -> -1.55
13800 (  358.3 -780.7   -60.3) swum across the basin to the far wall
14100 the weapon wheel opens (A), its 3D weapon models draw; the stick picks weapon 2
      (requested 2; the switch waits, the Talon stays: section 4); Z: no crash
14400 (  623.2 -780.7   198.3) turned back left (rotY -2.28), swimming on to (795.2, 233.0)
```

The path differs from u3's branch only where u1's loaded grid sections add collision (the ramp
now takes him into the water at x -1530 instead of -1157).

- `--audio --wav` through the boot (6,000 frames): 100.1 s, peak 21,621, RMS 1,998.6 (round 5:
  21,304 / 1,996).
- **Linux `--high-memory`**: the 26,000-frame boot loop (all three demos, the same mode switches
  at the same frames as low memory) and the new game to level 51 (15,000 frames), no crash, after
  one fix (C21-1 below).
- macOS (u4, on its branch before the merge): the boot to 26,000 frames, the new game into level
  51 and `--audio` identical to Linux; `--offscreen-gl` screenshots match Linux (29/29 new-game
  frames). The merged tree was not rerun on the Mac this round.

Screenshots (640x480, `T2_SHOT_EVERY`, checked by eye; not committed): frame 1801 the Acclaim
logo (the letters in place, the rainbow swoosh); 2403 the IGUANA letters in the jungle; 4500 the
title; 6500 demo 1 (a blue-grey corridor with a door, the Talon in hand, the weapon models,
health 100); 21200 demo 3 (green brick walls, the Talon); 8800 a new-game cinema with Adon and
Turok standing on a blue backdrop; 10302 soldiers fighting in a stone hall; level 51: 13001 the
harbour from the ramp in fog (towers, a moored boat, the Talon and the HUD), 13451 the boat's
hull and mast at the dock, 13601 the brick arches across the basin, 14101 the open weapon wheel
with the weapons in 3D.

The port tests pass: `rnc_test` (14,426 streams, 0 failed), `t2asset_test` (26,124 units, 0
mismatches; ROM backend + the archive's plans identical), `t2asset_typed_test` (1,585 checks x3,
0 errors), `gbi_test` (0 mismatches), `t2os_test` (trace hash `2b2775877e0d226b`, the same on 3
runs), `t2audio_test --direct 0` (19 frames, peak 32,433), `t2audio_test --adec 0xC29AB4 7464`
(27,136 samples), the swoosh self test (OK, 0 failed).

ABI (`tools/port/abicheck.py --build OUT/build -j 4`: 3,073 TUs, 10,824 direct calls checked,
186 function-pointer calls): harmful direct sites **11** (ptr_from_int 4, void_used 2,
missing_args 5; u5's branch: 12), indirect 4 (stores, declaration-only); struct_class,
struct_scalar, float_int 0. `ptrdecls.py`: 0 left in `src/` after this round's Throwable_Step
fix (it still lists the strlen returns and the stale, uncompiled `include/functions.h`);
`ptrargs.py` on a clean game-library build log: 0.

## 3. Merge resolutions and fixes (round 6)

| where | branches | resolution |
|---|---|---|
| func_00279158, func_0027A2C4, func_0027A4DC (`G_SETTIMG` image words) | u5, u4 | the same fix (`(GBI_ADDR(p))` without the truncating cast); u5's text kept |
| func_00213A70 (`Lighting_BuildLitVertices`) | u5, u4 | u5's `sptr model` (one signature on both targets) |
| func_00214F14, func_00220208 (lit-vertex callbacks) | u5, u4, u2 | u4's host bodies: the model from `CCacheEntry.memEntry->data`, the box and matrices by member (u5's and u2's `*(sptr *) *arg1` read the same pointer); the N64 path unchanged |
| func_00227184 (`Sort_Insertion`) | u5, u4 | u5's `t2_uptr` retype of the matched body |
| func_00252ADC, func_00254B04 (camera word 0x54C) | u2, u3 | the same fix (`m_Menus.count` by member): u2's text |
| func_0021ABC8 (`FireAnimEvents`) | u2, u3 | u2's version, a superset of u3's (the ObjectLinks view, the pointer-wide `CISet__GetBlock`, the events gate) |
| func_0021A150 (`m_nAnims`) | u2 | u2's committed fix (u3's was uncommitted) |
| port/app/main.cpp (usage) | u4, u3 | both lines |
| STATUS.md, RENDER.md | all | every part section kept |

Fixed on the merged tree (asm_diff 0):

| # | symptom | cause | fix |
|---|---|---|---|
| C21-1 | `--high-memory` crashed at frame 1362 (the Acclaim cinema) in `CISet__GetBlock` from `CAnimationState__Load` | `CAnimationState__GetData` (func_0021793C) read the cache entry's first word, a host pointer, as `s32` (reached only once u2's actors animate) | `*(sptr *)` |
| C21-2 | ptrdecls.py: `Throwable_Step` declared `(s32, sptr)` in func_00264C94 | u5 retyped the step callbacks after its last ptrdecls run | `(sptr, sptr)` |
| C21-3 | warp ids between levels would read byte-reversed | `CRegionInfo` 0x48/0x4C were `u8 unk_48[8]` (copied as bytes) but `CPlayer__Update` reads u16 there | `u16 m_WarpID` (0x48), `u16 m_PendingWarpID` (0x4C), bytes 0x4A/0x4E unchanged; the archive rebuilt; the level-51 journey is unchanged |

Demo 1's darkness (u1) is gone on the merged tree: with u2's actors, lights and shadows its
frames show the corridor, the door and the weapon models at normal brightness.

## 4. Next

1. **`CParticleSystem__Spawn`** (func_0022D2FC.c, 1,752 lines of local N64-layout views):
   firing, impacts, footsteps, blood and the animation events all end there. With
   `T2_ANIM_EVENTS=1` the boot crashes at frame 1362 in `CISet__GetBlock(sys->resource, 1)`
   (`FireAnimEvents` -> `ImpactEvent_Send` -> `Actor_SpawnParticle` -> `Spawn`): the
   `EffectSystem` view (the resource at N64 0xEC64, which must be `sptr`), then the
   Effect/Owner/Actor views and the particle table entries (`entry->params`). Port it on host
   types (hostview.py for the views), then turn the events on by default.
2. **The weapon switch**: the wheel requests weapon 2 but the switch waits for the weapon
   actor's animation (events off, in the water); retest on land once (1) is in.
3. **Bosses and the remaining actors**: `TotemDefense_InitFight` is all N64 offsets; the boss
   draw paths are untested.
4. **Warp ids**: typed this round (C21-3); test a real warp (a level exit) end to end.
5. **ABI**: abicheck's 11 harmful direct sites (ptr_from_int 4, void_used 2, missing_args 5) and
   4 indirect stores.
6. **macOS**: rerun the merged tree on the Mac (u2's actors and u3's input were not on u4's
   branch) with `--offscreen-gl` screenshots.
7. The remaining N64 views on the player path (GAMEPLAY.md section 5):
   `CPlayerPool__UpdateMatchClock`, `Action_CheckRequiredItem`, func_00271694.

## Round 7, part v1: the particle system (branch names-R7_v1)

`CParticleSystem__Spawn` and the particle path run on host types, so the **animation events
and hot points are on by default** (`T2_ANIM_EVENTS=0` turns them off). Details:
[RENDER.md](RENDER.md) section 6. All src/include changes are `TARGET_PORT`;
`tools/asm_diff.py --base d3f05a74 -j 8` reports 0 over all 3,938 files.

**Merge needs:** the archive rebuilt (`swap_spec.json`: particle-effects block 0 kept
big-endian, `"block0": "byte"`, honoured by `swapcheck.py`): sha1
`3ad7d24031060de1b4dad8df85b8c321227fe77c`; `decls.json` deleted (`structs/particle.h` changed:
`CPackedRange`, `CROMParticleDistance/Oscillation/Colors/Motion`; DLights_Load's
`gDLightBuffer[0x40]`).

What was wrong, in the order found:

| # | where | cause | fix |
|---|---|---|---|
| P1 | the templates (block 2) | nine 32-bit offsets per 0x3C record, relocated in place: no room for host pointers | `port/game/t2particles.c` builds a host copy per load (`CScene__DecompressParticles` and its twin); Spawn indexes it |
| P2 | block 0 sub-records | swapped as f32 (base and range of every `CPackedRange` exchanged); records overlap across types | kept big-endian, decoded per slot by the builder |
| P3 | func_0022D2FC.c (FAKEMATCH) | every view at N64 offsets (`EffectSystem.resource` at 0xEC64: `CISet__GetBlock(NULL/garbage, 1)`, the crash of round 6) | hostview twins of all 17 views, host aimEffect, player list head by name |
| P4 | 22 particle files | views at N64 offsets (projectile updates, HitTarget, Attach, StopLoopSound...) | hostview / host bodies |
| P5 | `CParticle__BuildCameraMatrix` | wrote the matrix at the N64 0x48, over `m_dwFlags`: freed particles left in the draw list, an endless loop | `m_mtxDraw[gEvenOdd]` |
| P6 | `CCamera__DrawGlares` | `CGlareSource` view over the particle draw list | hostview |
| P7 | `CScene__DrawParticles` | `scene + 0x18B0` | `&scene->m_ParticleSystem` |
| P8 | collision sweeps (particles move with `Collision_MoveInstance`) | static objects at the N64 stride, actor/box/face sweeps' views, the `{pointer, f32}` hit sorts | host CStaticObject stride, hostview, host-size comparators/swaps |
| P9 | `CScene__ClearListCounts` | N64 view: the scene's per-frame list counts (`m_nMovers`, `m_nKind1`, `m_nRegistered`...) were never reset on the host | hostview |
| P10 | `gDLightBuffer` | 16 bytes on the host (splat labels inside the 0x400 buffer): DLights_Load overran it | sized declaration for gendata |
| P11 | host template copies | freed on a reload while particles still used them | kept (one per scene load) |
| P12 | `CParticle__HitFloor/HitWall/HitCeiling` | the surface getters return u8, declared s32: the impact table indexed with eax garbage | u8 declarations (ABI class not caught by abicheck: see next steps) |
| P13 | `CParticleSystem__Construct` (and _2) | the particle data passed through the `s32 *m_pParticleEffects` handle: truncated with `--high-memory` | `*(sptr *)`, an `sptr` parameter |
| P14 | `CScene__GetSwoosh` | `CISet__GetBlock` without a prototype (implicit int): the trail record pointer truncated with `--high-memory` | `protos/iset.h` |
| P15 | func_0020B9F4 (particle models) | the cache entries' data pointers read as `*(s32 *)` (`--high-memory`) | pointer-wide |

How far it gets with the events on (Linux headless, low memory, `--game --draw --pak-blank`):

- **The boot loop, 26,000 frames: no crash (exit 0).** The mode switches are at the same frames as
  round 6 (1362 Acclaim, 1850 Iguana, 4212 title, 5467 demo 1, 13680 demo 2, 20946 demo 3,
  25778 the title again). The animation events fire from the Acclaim logo on (kinds 217/218
  from its hot points, 442...).
- **The scripted journey, 15,000 frames: no crash (exit 0).** Level 51 at frame 12,895, the same
  path as round 6 (13080 (-1540.7 -735.8 -982.4) state 3; 13440 (-1164.7 -780.7 -29.0);
  13800 (358.3 -780.7 -60.3); 14400 (623.2 -780.7 198.3); 14640 (795.2 -780.7 233.0)),
  health 100, the wheel requesting weapon 2. `T2_PARTICLE_LOG=50`: 350 spawn calls, all in
  the cinemas (none in level 51 on this path: no footstep hot points in first person; v2's
  firing spawns the arrow and its splash).
- **`--high-memory`: the boot to 12,000 frames, no crash** (after P13-P15).
- Particles on screen (RENDER.md 6.4): against round 6's binary, 16 of 138 frames from 1 to
  2770 differ, each where particles show (the Acclaim logo's cyan ring burst and sparks/dust,
  the Iguana jungle's mist and fireflies).
- Debug: `T2_PARTICLE_LOG=N` prints every N `CParticleSystem__Spawn` calls that reach templates.

Files others may touch (merge): func_0024032C.c, func_0023F880.c, func_0023FDF0.c,
func_0023DF70.c, func_0023D050.c, func_0023DBBC.c, the hit sorts (func_002407D8, func_0024079C,
func_0023DB98, func_0023DB5C) and func_002682CC (v3's collision area: agreed with v3);
func_00226390.c (`CScene__ClearListCounts`: changes what every list-driven system sees);
func_002284E0.c (`gDLightBuffer[0x40]`: v2 made the same fix, keep one); func_0022D2FC.c.

Next:

1. **The narrow-return ABI class** (P12) is not caught by abicheck: a regex scan finds 17
   functions defined with a u8/s8/u16/s16 return and declared s32/u32/sptr elsewhere
   (EnemyAI_GetBehaviorIndex, EnemyAI_PickAttack, WeightedTable_Pick/_PickStatic,
   CScene__GetObjectTypeID, Controller_IsPakWatched, __osContDataCrc...). Check each call site
   (some are in `#else` branches) and teach abicheck the return width.
2. The particle files not touched because no run reached them: func_00236A34, the Trail/Swoosh
   drawing (`Trail_DrawTextured`, `Trail_PlaceNodes`: typed but not checked against particles
   with trails), func_00232F38's big view (only `id` is read), `CParticle__SpawnAlongPath`.
3. Firing (v2) and enemy hits (v3) spawn projectiles and impacts through this path: their
   particles (bow arrows, bullets, blood) were not seen in this round's runs.
4. macOS with the events on was not run (v4); `--high-memory` was run to 12,000 frames only.

## Round 6, part u1: gameplay draws the level (branch names-R6_u1)

[RENDER.md](RENDER.md) section 4. Level 51 now **draws in gameplay**: the Port of Adia start
(the ramp, brick and panelled walls, the arch row, crates, sky, fog) from frame 12903 on, with the
health HUD. Attract demos 2 and 3 draw their levels; demo 1 draws its geometry, very dark (maybe its
real look). "NEW CHEAT ENABLED" no longer appears. Four causes:
the gameplay camera's visibility mask read at an N64 offset of the host `CGameRegion` (0: every
object culled), **the cache hash lookups' N64 bucket stride** (0x10 vs the host's 32-byte
`HashEntry`: no asynchronous request ever hit, so gameplay never got lit vertices; cinemas load
synchronously and got through), the preview-options field written at an N64 offset of `gScene`
(the cheat message), and the `COverlay` constructors/script start on N64 views (no HUD). Boot
26,000 frames (158 s) and the new game to frame 15,000 (82 s) run without a crash. The player
still stops at (-1266.6, -779.5, 260.0) (u3).

Merge notes: `src/us/code/func_00203CA0.c`, `func_002034F4.c`, `func_00205A2C.c`,
`func_0027D160.c`, `func_0042C3BC.c`, `func_0027ACC4.c`, `func_0027AD00.c` (all `TARGET_PORT`
blocks, asm_diff 0), `port/game/t2debug.c` (`T2_SCENE_LOG`). The archive and gendata are unchanged
(no header edits).

## Round 6, part u5: ABI and pointer width (branch names-R6_u5)

- `tools/port/abicheck.py` checks **indirect calls** (function-pointer tables/fields, gendata
  initializers included) and **asm-label aliases**; new `tools/port/abiptrfix.py` retypes the
  declarations behind `ptr_from_int`. Harmful sites **193 -> 12** (ptr_from_int 185 -> 4, a
  literal 0; void_used 2 and missing_args 6 left, ABI.md section 4); indirect 210 call + 4 store
  -> 0 call + 4 store (declaration-only). ABI.md section 5.
- Gameplay-relevant ABI fixes: the six AI step functions now pass (actor, ai) to the mode
  callbacks (doors, turrets, destructibles, throwables, actions, interactive anims: MIPS
  pass-through), `CAI__CallHurt` passes (actor, ai, damage) to the hurt handler, the
  Destructible view writes its callbacks at host CAI offsets, impact handlers PlaySound /
  func_00239350 read the sound id from the right register.
- **Linux `--high-memory` runs**: the boot, both cinemas, the title, attract demo 1, demo 2's
  start (14,000 frames) and a **new game to level 51 identical to low memory** (was: crash at
  boot). 64BIT.md part 4 lists the six crash sites fixed on the way (texture image words
  truncated by `(unsigned int)(GBI_ADDR(p))`, the menu file's `s32` asm aliases, the lit-vertex
  model pointer, `GameObject_GetRegionInfo`, `Sort_Insertion`). audit64: pointer-width warnings
  1,580 -> 1,465, disagreeing declarations 158 -> 93 functions.
- Low memory unchanged: 26,000-frame boot loop with 3 attract demos, new game to level 51
  (player stops at the water at (-1266.6, -779.5, 260.0) as before). asm_diff 0 over every
  touched file (152 src files).
- Merge notes: many one-line declaration retypes (s32 -> sptr) spread over ~150 src files; a
  conflict with another part is a declaration line: keep both changes. func_002034F4/00205A2C
  (osSendMesg declaration) also carry u1's hash-stride fix on other lines.

## Round 6, part u4: macOS runs the merged tree, with sound and pixels (branch names-R6_u4)

Details in [MACOS.md](MACOS.md) section 8. On the Mac (arm64, full 64-bit pointers, headless, no
window of any kind):
- **The boot to frame 26,000** (cinemas, title, all three attract demos, the loop) and **the new game
  into level 51** run with every mode switch at the same frame as Linux and the same player states.
- **`--audio --wav`**: peak 21,304, RMS 1,996.4 over 6,000 frames, the same as Linux.
- **Pixels without a window**: `--offscreen-gl` (port/app/offscreen_gl.cpp) runs LUS's OpenGL
  renderer on a CGL context with no drawable, and T2_SHOT reads the game framebuffer. Mac against
  Linux screenshots: all 29 new-game frames match (at most 0.37% of the pixels off), and 15 of 17
  boot frames match.
- dlcheck (game.cpp): display lists with truncated host addresses are reported and not drawn. None
  is left on the boot or new-game path.
- Fixes, all asm_diff 0 against 6d04fabb: lit-vertex callbacks, GameObject_GetRegionInfo,
  Sort_Insertion, three audio truncations (ALLowPass, the A_SAVEBUFF output handle, the ALDMAproc),
  the font/glyph G_SETTIMG images, and **Cinema_DrawFade's camera viewport at the N64 offset**.
  The last one is a bug on all hosts: the cinema fades now draw on Linux too.
- `-ffp-contract=off` for the game (game.cmake).
- **Lead for u1**: in attract demo 2 the Mac draws the level's static objects and Linux draws none.
  On Linux the camera's `m_ViewParams.m_VisBits` is 0 (CCamera__UpdateVisBits), so
  CScene__DrawGeometry rejects every CStaticObject.

Files others may touch: port/app/game.cpp (WriteShot, the draw call, dlcheck, T2_DLDUMP_MAX),
port/app/main.cpp (`--offscreen-gl`), port/app/CMakeLists.txt, port/cmake/game.cmake,
port/audio/t2audio_mgr.c, port/runtime/t2ultra.c (T2_V2P_TRAP), and src func_00213A70,
func_00214F14, func_00220208, func_00268A2C, func_00227184, func_00295304, func_002971BC,
func_00278188, func_0027836C, func_0027854C, func_0027B0FC, func_00279158, func_0027A2C4,
func_0027A4DC, func_0040F240, func_00448240.

## Round 6, part u2: animated actors (branch names-R6_u2)

[RENDER.md](RENDER.md) section 5. The actor draw and animation chain (`CGameObjectInstance__Draw`
-> `BuildBoneMatrices` -> `PoseBone` -> `CAnimationState__*`, `AdvanceAnimation`,
`CScene__AdvanceInstances`, `CAI__UpdateModeAnim`, the shadow and draw callbacks) runs on host
types. Linux, headless, this branch alone:

- mode 4 (frame 1362-1846): **the Acclaim logo renders** and animates (letters flip in, settle
  into "Acclaim entertainment, inc."); bone matrices = `tools/assets/anim.py` pose (2 units of
  s15.16).
- the Iguana cinema as before; the title; demo 1 runs to its end and the boot loops (8,800
  frames, no crash).
- new game (`--input START@4400,A@4500,A@4700,A@4800`): the mode-11/12 cinemas draw their
  characters (Adon, Turok, enemies, the caged prisoner) and the run reaches level 51 (frame
  12899) and runs to 14,500 without a crash (before: a stack overrun in `CScene__GetLightsAt`
  crashed the cinemas once actors drew).
- with u1's branch merged in a scratch branch (not committed): level 51 draws the level **and the
  Talon in Turok's hand** (frames 13001-13601).

Merge notes: `include/structs/model.h` (`CDrawArgs.m_hModel` sptr), new
`include/structs/pose_host.h` and `port/game/t2actors.c`. Files others may touch:
func_0021A150 (u3 has the same m_nAnims fix uncommitted: take one), func_00243170
(`CAI__UpdateModeAnim`: changes enemy animation timing), func_00218B10 (`GetRootMotion` flags),
func_00252ADC/func_00254B04 (player pool), func_00214F14 (t3's static lit vertices: prototype
pointer-wide). Anim events/hot points are **off** (`T2_ANIM_EVENTS=1` to enable) until
`CParticleSystem__Spawn` (func_0022D2FC) is ported.

## Round 6, part u3: player input, the water, the weapon wheel (branch names-R6_u3)

Details and the fix table u3-1..5: [GAMEPLAY.md](GAMEPLAY.md) section 5, [INPUT.md](INPUT.md)
section 6. `--input` now takes held ranges (`CU@13000..13440`), the stick (`SX=80@13450..13474`,
`SY=...`) and `@FILE` scripts. The player turns (the region-info word bug, u3-1), the water was
a corner, not a merge regression: steered, he swims across the level-51 basin to x 358. The
weapon wheel opens with A/B and the stick picks the weapon (u3-2); the weapon actor's AI runs
(u3-3); switching and firing wait for animations: `m_nAnims` is read as the wrong half of a
swapped word (fix known, not committed: it exposes a crash in `CParticleSystem__Spawn`'s N64
views at frame 1362; GAMEPLAY.md section 5). Files the merge may meet: `port/app/game.cpp`,
`port/app/main.cpp` (usage line), `port/game/t2debug.c` (state log: weapon/wheel fields; u1
also edits it: auto-merged in my scratch test), func_0024EF70, func_0024BAD4, func_00252ADC,
func_00254B04, func_00250BF0, func_0021A658, func_0021ABC8 (not touched by u1 or u2's
branches as of their tips; u2 changes func_0021A344/0021A500/0021A7EC/00218B10 nearby).
Merged with u1's branch (scratch, not committed) the gameplay frames show the level.

# Round 5, merged state (history)

Round 5 merged five branches, in this order: t2 (the ABI checker and the ABI sweep), t5 (macOS to
the title: the virtual pad on `--no-window`, pointer-wide prototypes and parameters), t3 (the 3D
scene: the cinemas draw), t4 (audio part 3: sfx, music, speech) and t1 (gameplay: the attract
demos and a new game into level 51). Then it fixed what only the merged tree reaches. This page
says how to build and run the result, how far `turok2 --game` gets, what stopped it on the way and
what comes next. The part sections further down keep each branch's own notes (t2, t5, t3, t1 here;
t4 at the end; detail pages [ABI.md](ABI.md), [MACOS.md](MACOS.md) section 7,
[RENDER.md](RENDER.md), [AUDIO.md](AUDIO.md) section 10, [GAMEPLAY.md](GAMEPLAY.md)); the round 4
and round 3 states follow as history.

The matching build is unchanged. `tools/asm_diff.py --base 98574f3c -j 8` reports 0 differences
over all 3,938 files after every merge and fix. A clean `make clean && make setup && make build &&
make verify` gives the identical ROM (CRCs good, 123 shared .rodata objects agree).

## Round 7, part v4: macOS, arm64 float conversions, performance (branch names-R7_v4)

Commits (on the Mac clone `~/Documents/Proyectos/turok2_macport`, branch `names-R7_v4` from
d3f05a74). Every `src/` change: asm_diff 0 against d3f05a74 on the box. No `include/` changes.
The archive is unchanged (`47e8f73c…`).

| commit | what |
|---|---|
| 6b74c760 | `WeaponActor_Update` (func_00257D48) host body. The N64 AI view zeroed the high half of `m_pfStep`, which crashed macOS entering mode 13. It also ran the mode table's row 0 instead of the current mode, **on Linux too**, and that is why the weapon switch never completed. Now the level-51 journey switches to weapon 2 (the bow, 20 arrows in the HUD) and Z enters the fire mode |
| de2fe1cd | `--bench [FROM]` and `--pace` (debug): see [PERF.md](PERF.md) |
| b07b8d5d, 37a8cf99 | `func_00225C08` (noise to kind-1 actors) host body: its FAKEMATCH tests an uninitialised local, which clang -O2 compiles to a trap. `CScene__ClearListCounts`: names-R7_v1's version taken byte for byte, so it merges clean |
| 18150edd | **arm64**: negative float -> u32 conversions saturate to 0 (`fcvtzu`); the R4300 and x86-64 wrap. `T2_F2U` (TARGET_PORT: through s32) in the bone-matrix packing (packMtx: the Acclaim logo and every skinned actor lost their negative matrix elements on the Mac), the vertex-pattern hash, the scene light direction, the fill-rectangle corners. `tools/port/f2ucheck.py` lists all 99 float -> unsigned conversions. `-fsanitize=float-cast-overflow` confirms that none is left on the level-51 journey |
| 321d1c9b, 37a8cf99 | `T2_DLDUMP_VTX=2/3`: every vertex, G_SETTIMG image hashes/bytes, G_MOVEMEM bytes (debug) |
| ac622745 | `CScene__GetSwoosh`: `CISet__GetBlock` declared (an implicit `int` return truncated it on macOS once v1's particles spawn trails) |

Results ([MACOS.md](MACOS.md) section 9, [PERF.md](PERF.md)):

- macOS, this branch: the new game with the scripted journey runs to 15,000 frames with
  `T2_STATE_LOG` identical to Linux. Debug and an `-O2` build agree. 26 of 30 screenshots match
  Linux within 1% of the pixels. The other 4 are actor parts that Linux draws and the Mac does
  not (the boat and Turok's hand in level 51), although the display lists are identical, every
  vertex and matrix included. That is open.
- The boot stops entering attract demo 1 (frame ~5470) **on both hosts**: v4-1 makes the demo's
  weapon fire, and firing needs v1's `CParticleSystem__Spawn`. With v1 merged (scratch), the Mac
  runs into the demo and the new game into the mode-12 cinemas. Truncations found there were
  fixed by v1 (`ModelSweep_TestModel`, b8bca0e3) and v3
  (`ImpactEvent_AreaDamage` & co., e26d87e9; `ImpactEvent_LevelSpecial` fixed by v5 in 06477da5); `RumbleCurve_Advance` (`gCurrentFB[1]` NULL,
  `--offscreen-gl` only) is open: retest with v2's rumble fixes (be9c4bf8).
- Performance (M5): level 51 costs 0.55 ms per drawn frame in Release and 1.05 ms in Debug (the
  N64 budget is 33.3 ms). Drawing (Fast3D + GL) is about 80% of that. Paced at 60 Hz VIs / 30
  fps, Release has no late VI in level 51; the late VIs are level loads only.

To merge: `src/us/code/func_00257D48.c` (v2 may write the same function: keep one; mine runs
the current mode), `func_00226390.c` (identical to v1's), `func_00223F50.c` (one line; v1 fixed it too in 5e724e40: keep either),
`func_0021B95C.c`/`func_0021285C.c`/`func_0027C4E4.c`/`func_0027F64C.c` (a `T2_F2U` macro at the
top of each), `port/app/game.cpp`/`main.cpp`/`game.h`/`offscreen_gl.*`,
`port/audio/t2audio_mgr.c`/`t2audio.h` (`T2Audio_GetTimes`). After the merge, rerun
`tools/port/f2ucheck.py` on new host bodies, and run the merged tree on the Mac and with Linux
`--high-memory`.

## Round 5: build and run (Linux x86_64, the box)

```sh
# 0. the matching build first: gendata and asm_diff read its ELF; keep a copy as build/ref.elf
#    (the asm_diff/gendata tools expect it, and `make clean` deletes it)
make setup && make -j8 build && make verify && cp build/turok2.us.elf build/ref.elf

# 1. the archive, format 2.0. REBUILD IT this round: tools/assets/swap_spec.json (collision
#    table 3, the region grid) and include/structs/region.h (CRegionInfo typed) changed the plans.
python tools/assets/mkarchive.py baserom.us.z64 -o OUT/turok2.o2r -j 4     # ~12 s
python tools/assets/archive_check.py OUT/turok2.o2r baserom.us.z64          # OK, format 2.0

# 2. configure; the game library AND the runtime/audio libraries (gendata reads all three)
cmake -S port -B OUT/build -G Ninja -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ \
      -DCMAKE_BUILD_TYPE=Debug
ninja -C OUT/build -j8 turok2_game t2runtime t2audio

# 3. the game data as C into port/gen/ (gitignored). Drop the cache after ANY header change.
rm -f OUT/decls.json
python3 tools/port/gendata.py --build OUT/build --cache OUT/decls.json -j 4      # ~1 min

# 4. re-run cmake so data.cmake sees port/gen/*.c, then the executable
cmake OUT/build && ninja -C OUT/build -j8 turok2

# 5. run headless (SDL's offscreen driver: no window). --pak-blank: a blank formatted
#    Controller Pak in memory (--pak-dir DIR: turok2_pak<N>.mpk files; --no-pak: none).
cd OUT && SDL_VIDEODRIVER=offscreen T2_SHOT=shot.ppm T2_SHOT_EVERY=300 \
    build/app/turok2 --archive turok2.o2r --rom ../baserom.us.z64 \
    --game --draw --pak-blank --frames 26000
#    a new game: --input START@4400,A@4500,A@4700,A@4800 (title -> main menu -> single player
#    -> New game -> the pak message OK), then walking: CU@13000,CU@13004,... (--input presses a
#    button for one frame; repeat it every 4 frames to hold it). Buttons: A B Z START DU DD DL
#    DR L R CU CD CL CR; no stick yet.
#    --no-window (macOS: always): a virtual pad on port 1 (its pak per --pak-*, the buttons from
#    --input); with an offscreen window LUS's ControlDeck serves the same scripted buttons.
#    --audio / --wav out.wav: sound (sfx, music, speech). T2_STATE_LOG=60: the level, the
#    player (position, state, region, floor) every 60 frames. Debug: --skip-pak-check,
#    --start-mode N, T2_MENU_LOG=1, T2_DLDUMP_AT=<frame> (+ T2_DLDUMP_VTX=1),
#    T2_TEST_SFX/T2_TEST_SONG/T2_TEST_SPEECH; gdb -batch -ex run -ex bt --args ... for a trace.
```

gendata on this tree: 1,537 data symbols the C needs (1,531 typed from a C declaration), 93
pointer targets typed as tables, 386 zero .bss symbols, 4,041 pointer slots (2,932 relocated,
1,077 null, 32 unresolved). The archive: 26,124 units, sha1
`ae9b88e69adc541a2cd9accee142da0d3ea8207c` (the plans changed, so `turok2` without `--game` now
reports fnv1a64 `ac3d498348be24c2` on all 3 backends).

## Round 5: how far `turok2 --game` gets (Linux, headless)

The boot, the cinemas, the title and **all three attract demos** run on their own, and the boot
loops as on the console:

```
--game --draw --pak-blank --frames 26000
frame 2:     mode 0    pak check, the virtual pak passes
frame 298:   mode 1    legal screen
frame 1070:  mode 3    "EXPANSION PAK FOUND!" with the 3D pak model
frame 1362:  mode 4    cinema 100 (level 162, Acclaim logo): runs, black (an animated actor)
frame 1852:  mode 5    cinema 90 (level 44, Iguana jungle): DRAWS the scene
frame 4214:  mode 7    TITLE SCREEN
frame 5469:  mode 13   attract demo 1 (level 81), plays to its end
frame 8802:  mode 1 -> 4 (9577) -> 5 (10067) -> 7 (12429): the boot again
frame 13684: mode 13   attract demo 2 (level 131)
frame 16073: the boot again, title at 19697
frame 20952: mode 13   attract demo 3 (level 109): loads and plays (round 4/5 crash fixed, C20-3)
frame 22160: the boot again, title at 25786; no crash to frame 26000 (158 s)
```

A new game:

```
--game --draw --pak-blank --input START@4400,A@4500,A@4700,A@4800,CU@13000,...  T2_STATE_LOG=60
frame 4455:  mode 8    main menu
frame 4555:  mode 13   single-player menu (New game / Load / Go back)
frame 4855:  mode 11   new-game intro cinemas (levels 50, 49): they DRAW (Port of Adia, the boat)
frame 8977:  mode 12   (levels 47, 45, 46, 48, 49), then the level-51 warp-portal cinema: draws
frame 12903: mode 13   level 51, GAMEPLAY. The player stands at (-1540.7, -566.4, -1481.6);
                       with C-up held he walks (state 3) down the slope to y -735.8, into the
                       water (state 9) at (-1266.6, -779.5, 260.0) by frame 13600, where he stays
                       (C-left, C-up + Z: no further movement; state 8 once the input stops).
                       No crash to frame 15000 (82 s).
```

Screenshots (`T2_SHOT_EVERY`, 640x480, checked by eye): frame 2401 the Iguana cinema (the IGUANA
letters on the jungle floor, tree trunks, ferns, letterbox); frame 4500 the title as in round 4;
the new-game cinemas (frame 9302: the Port of Adia, buildings, the boat, fog; frame 12902: the
level-51 warp portal between the walls); demo 1 frames are a dark grey fill, with the weapon
wheel's 3D weapon models drawn at frame 7202; demo 2 frames show a full-screen texture close-up;
**gameplay frames (level 51) draw the sky, the fog colour and the HUD text** ("NEW CHEAT
ENABLED") but no level geometry: the gameplay display list has no `G_VTX` and no `G_MTX`
(`T2_DLDUMP_AT=13001`), while `CScene__DrawGeometry` and `CStaticObject__Draw` run every frame
(section 4).

With `--audio --wav` (6,000 frames: the boot, the title and into demo 1): no crash, 100 s of
sound, peak 21,304, RMS 1,996 (silent for the pak/legal seconds, then 750-3,700 RMS per 10 s
window through the cinemas and the title).

The port tests pass on this tree: `rnc_test` (14,426 streams, 0 failed), `t2asset_test` (26,124
units, 0 mismatches; ROM backend + the archive's plans identical), `t2asset_typed_test` (1,585
checks x3, 0 errors), `gbi_test` (0 mismatches), `t2os_test` (trace hash `2b2775877e0d226b`,
deterministic), `t2audio_test --direct 0` (19 frames, peak 32,433), `t2audio_test --adec
0xC29AB4 7464` (27,136 samples), the swoosh self test (`turok2 --game`: OK, 0 failed).

ABI checker (`tools/port/abicheck.py --build OUT/build -j 4`, 3,065 TUs, 10,762 calls checked):
harmful sites 193 (102 callees): ptr_from_int 185, missing_args 6, void_used 2; struct_class,
struct_scalar, float_int 0 (t2 alone: 510 harmful, 498 of them ptr_from_int: t5's pointer-wide
prototypes took that class from 498 to 185). `ptrdecls.py` and `ptrargs.py` rerun on the merged
tree report nothing left in `src/` (the gitignored, stale `include/functions.h` still lists
names; it is not compiled).

macOS: t5 ran its own tree to the title; the merged tree was not rerun on the Mac this round.

## Round 5: merge conflicts and crash/fix table (round 5 merge)

Conflicts and how they were resolved (asm_diff 0 after each merge):

- t2 x t5 (38 files): t2's `TARGET_PORT` host copies against t5's s32 -> sptr retypes of the same
  headers: kept t2's bodies and applied t5's parameter retypes to both branches of each `#ifdef`
  (a script over the diff3 hunks), then `ptrdecls.py` (2 returns) and `ptrargs.py` (1 parameter).
- `Actors_GetLightVolumeFalloff` was fixed three times (t2: adapters `_f3`/`_v` that pass the floats
  as words; t3: a float definition over a union; t1: a typed host body). Kept t1's body and the
  float call sites, dropped t2's two adapters and their redirects.
- `gWarmBoot = 1` (t3 and t4): one assignment in `t2boot.c`, both reasons in its comment.
- The audio heap/bus files (func_002912F0, 00294E70, 00295304, 002953B4, 00295410, 0029546C):
  pointer-wide in both t5 (sptr) and t4 (`TARGET_PORT` typedefs): kept t4's, with t5's
  pointer-wide `alHeapDBAlloc` file and `alFilterNew` handler parameters.
- Grid section record sizes (t3 `SIZEOF_*`, t1 `GRID_*_SIZE`): t1's names. The CDLightSet functions
  (t2 pass-through copies, t1 typed bodies): t1's. `CGameRegion__GetNormalRaw` (t2: `_byval` rename
  + wrapper; t1: a host body taking the result pointer) and `Automap_SetPlayer` (t2: adapter `_f`
  from f32 to words; t1: a host body with the caller's types and `m_pHostRegion`): t1's, t2's
  adapters and redirects dropped. t2's host copy of `CScene__LoadLevel` now passes
  `&w->m_DLightSet` (t1's member) where it had `w->unk_18EBC`.
- `ptrdecls.py` crashed on a negative brace depth (`#ifdef`-split function heads): clamped.

Paths only the merged tree reached (all `TARGET_PORT`, asm_diff 0):

| # | where | cause | fix |
|---|---|---|---|
| C20-1 | `Cinema_Update` (func_00283CD4): **no cinema ended** (mode 4 for ever) | its `GameState` view reads a second word at N64 offset 0x1450 from `m_PlayerList.m_bCinema` (= `gEngineApp_m_Settings.match.unk44`); after t1's `CAutomap.m_pHostRegion` that host offset landed on a non-zero word | by member: `gEngineApp_m_Settings.match.unk44` |
| C20-2 | abort `T2Port_ReadAsset(0x01CB021F)` 120 frames into level 51 (`CLevelActor__LoadGeometry`) | with the scene drawing, the level actors' geometry is requested; the key was read at N64 offset 0x34 (`m_Resource`) of a host-layout `CLevelActor` | `CLevelActor.m_Resource` (LoadGeometry and TouchGeometry) |
| C20-3 | demo 3 (level 109) SIGSEGV in `CCache__RequestDecompression` <- `CStaticObject__RequestLitVertices` <- `CScene__StreamAllSections` (t1's "host pointer as cache key") | `CScene__StreamAllSections` walked the grid section records with the N64 stride and wrote the section handle at N64 offset 0x24: the static objects' `m_pOwner` stayed garbage | `ObjA`/`ObjB` as host twins of `CStaticObject`/`CLevelActor` |

The class to look for next (C20-1 is one): views that read engine globals through an alias symbol
plus an N64 offset (`extern <LocalView> gEngineApp_m_Game_...`). Without a `TARGET_PORT` twin:
`CPlayerPool__UpdateMatchClock` (func_0025492C, `Globals` off `m_MenuPool`: the multiplayer match
clock), `Action_CheckRequiredItem` (func_00265F90, the camera pool), func_00271694
(`MenuMatchGlobals`), `EngineMode03_ExpansionPak_Draw` (func_00285CF0), func_00459374.

## Round 5: next

1. (done in round 6 u1, see its section) **Gameplay frames draw no level geometry**: the sky, the fog fill and the HUD text are there, the
   gameplay display list has no `G_VTX`/`G_MTX`, `CStaticObject__Draw` runs ~3 times a frame (the
   level-51 cinema, same level, draws everything). The player camera's draw setup (view/projection
   matrices, the viewer box `CScene__LoadVisibleGridSections` culls with) is the place to start;
   "NEW CHEAT ENABLED" at level start suggests the cheat flags read garbage too.
2. **Animated actors** (the Acclaim logo, every enemy, pickup and weapon): `CGameObjectInstance__Draw`
   -> `BuildBoneMatrices` (func_0021CDAC) -> `PoseBone` (func_0021B95C) -> `CAnimationState__*`
   (func_00218B10) on N64 views; t3's draft `/root/R5_t3/actor_draw_0021D39C.patch` covers the
   first step only (RENDER.md section 3).
3. **The player stops in the water** at (-1266.6, -779.5, 260.0) in state 9 (t1's run crossed the
   water to x 772): collision against the now-loaded static objects, or a state-9 view at N64
   offsets.
4. **Rotation input**: `--input` has no stick (an `SX`/`SY` entry), so scripts cannot turn the
   player; the d-pad does not turn in Turok 2.
5. **Weapon wheel**: pressing A in gameplay crashed in `CBarrel__OpenWheel` before t1-18; untested
   on the merged tree (the wheel's models draw in demo 1).
6. **macOS**: rerun the merged tree on the Mac (t5 stopped in the second demo's
   `CGameRegion__ContainsXZ`, which t1 fixed); audio above 4 GB (`--high-memory`): t4/t5's heap
   fixes are merged, untested together; pixel screenshots there need a Metal offscreen target.
7. **ABI**: abicheck's 185 `ptr_from_int` sites (the macOS truncations), 6 missing_args, 2
   void_used; function-pointer calls (184 sites, the handler tables) are not checked yet.

## Round 5, part t2: the ABI checker and sweep

[ABI.md](ABI.md). `tools/port/abicheck.py --build OUT/build -j 4` parses the 3,099 linked C files
with libclang (about 15 s) and compares every direct call (10,724) with the callee's definition
under the x86-64 SysV and AArch64 rules: struct classes, struct vs scalars and hidden result
pointers, float vs int, f32 vs f64, pointer width, void results used, MIPS pass-through
(missing) arguments, variadic mismatches. Harmful sites 779 (294 callees) -> 510 (214), of which
498 are the 64-bit width class (`ptr_from_int`, the macOS truncations): struct_class 26 -> 0,
struct_scalar 62 -> 0, float_int 43 -> 0, void_used 85 -> 4, missing_args 67 -> 7. The fixes are
host-only (adapters in `port/game/t2abi.c` reached by a `#define` in the caller, by-value
definitions wrapped for hidden-pointer callers, `Triple` as three floats in the impact handlers,
host copies that return the last call's value or pass the pass-through registers); asm_diff 0
over all 3,938 files.

With these fixes the boot runs the same sequence to the title and then **into the attract demo
without the `EnemyAI_Update` crash**: `--game --draw --pak-blank --frames 7000` reaches mode 13
at frame 5469 and runs to frame 7000 (the gdb run exits normally); the frames are black (the 3D
scene draw, t3). The checker does not see function-pointer calls (184 sites: the handler
tables); that is the next thing to add (ABI.md section 4).

## Round 5, part t5: macOS runs the merged tree to the title screen (branch names-R5_t5)

The Mac (arm64, full 64-bit pointers) now boots the merged tree on its own, headless, with the
pak flags, through the **same mode sequence at the same frames as Linux**: 0 (2), 1 (298), 3
(1070), 4 (1362), 5 (1852), **7 title (4214)**, 13 attract demo (5469). Display lists match Linux
at frames 601, 1201, 1501, 2401, 4500 and 6000 (same command count, op and w0; w1 differs only in
host addresses). Details, the commands and the fix table: [MACOS.md](MACOS.md) section 7.

What changed (all `src/` changes are s32 -> sptr / u32 -> uptr, the same types on N64: asm_diff
0 over 3,938 files at every commit):

- `port/app/game.cpp`: `--no-window` runs get a virtual pad on port 1 (pak per `--pak-*`,
  buttons from `--input`). Without it the Mac (no headless GL/Metal, so always `--no-window`)
  saw no controller and stayed in mode 0. **Linux `--no-window` runs change the same way.**
- `tools/port/ptrdecls.py`: prototypes that declare s32/u32 where the definition takes or returns a
  pointer (385 parameters + 27 returns). `tools/port/ptrargs.py`: definitions whose s32 parameter
  receives a pointer, from the port build's `-Wint-conversion` notes (331 parameters of 127
  functions, with all their declarations). Both re-runnable; they report 0 left on this branch.
- AI mode/event callbacks (`Action_*`, `Door_*`, `Enemy*`, `PlayerDeath*`, `Turret_*`, `Weapon*`,
  the bosses...): the actor/AI/event parameters as sptr (151 parameters in 120 files).
- Hand fixes on the path: pak note names, cache handles read as `s32 *` (`CISet__GetBlock(*res)`,
  13 files), `CPlayer__GetPickupCountLow/High` host bodies, event-record tables, the audio heap
  and filters (`alHeapInit`, `alHeapDBAlloc`, bus/resampler constructors, `alFxParam`).

**Side effect on Linux:** with this branch the Linux build (rebuilt from it on the box,
`/root/R5_t5mac`) no longer stops at the first attract demo either: windowed and `--no-window`
both run demo 1 to its end (mode 13, frames 5469-8794), loop the boot (legal 8802, cinemas,
title 12429) and stop in the **second** attract demo's level load, the same place as the Mac:
`CGameRegion__ContainsXZ(tri = garbage) <- CScene__FindRegionAt <- CScene__DecompressAnimatedInstances`.
`EnemyAI_Update` still reads the enemy AI at N64 offsets (t1's item); its garbage just does not
crash any more.

Merge notes: 351+ files in `src/` change, almost all on declaration/parameter lines
(`include/rw/abu_protos.h` too). Conflicts are likely with any branch that edits the same
function headers (t1's enemy AI files, t2's ABI sweep: rerun `ptrdecls.py`/`ptrargs.py` after
merging instead of resolving by hand), t4's audio files (`func_0029*.c`).

## Round 5, part t3: cinemas and the 3D scene ([RENDER.md](RENDER.md))

The second intro cinema (mode 5, level 44) now **draws**: the Iguana logo in the jungle, the
camera moving through trees, ferns and the vine, letterboxed (frames 2401-3901). The boot still
reaches the title (frame 4214) and the attract demo (frame 5465, mode 13, level 81), where it stops
in `EnemyAI_Update` as before (t1's). 11 fixes, RENDER.md section 1: the region attributes typed
(far clip / FOV were big-endian: **rebuild the archive**), the grid sections loaded (camera view at
N64 offsets), `CStaticObject` on the host layout in 9 files, `gWarmBoot = 1` (the settings defaults:
the difficulty mask was 0), a float/word ABI mismatch in the light volumes, the main camera's frame
clear over the scene (`m_MainCamera - 0x40`), `sLitVertexOffsets` sized 0x9000 (it overran into
`gAudioWorld`/`gLoopSounds`). The first cinema (Acclaim logo, level 162) stays black: its logo is an
animated instance and the actor path (`CGameObjectInstance__Draw` -> `BuildBoneMatrices` ->
`PoseBone` -> `CAnimationState`) is still on N64 views (RENDER.md section 3; a host
`CGameObjectInstance__Draw` is ready as a patch). New debug aid: `T2_DLDUMP_VTX=1`.

## Round 5, part t1: gameplay runner (branch names-R5_t1)

Details, fix table t1-1..t1-20 and the debug aids: [GAMEPLAY.md](GAMEPLAY.md).

**Rebuild the archive**: `tools/assets/swap_spec.json` changed (collision table 3, the region
grid, is swapped now: t1-3). An archive from round 4 still loads but the grid counts are
byte-reversed (`CScene__FindRegionAt` walks garbage).

```
--game --draw --pak-blank --frames 26000                      (no input: the attract demos)
frame 5469:  mode 13, level 81   demo 1 plays to its end (8794), the player moves
frame 13684: mode 13, level 131  demo 2 plays, the player moves
frame 20948: level 109 (demo 3) loading: abort in T2Port_ReadAsset(0x40291210), a host pointer
             as a cache key <- CStaticObject__LoadGeometry <- CScene__StreamAllSections

--game --draw --pak-blank --input START@4400,A@4500,A@4700,A@4800 (+ CU@13300,CU@13304,...)
frame 4455:  mode 8 main menu; 4555 mode 13 (single-player menu D_0043916C, New game)
frame 4855:  mode 11 new-game intro cinemas (levels 50, 49); 8977 mode 12 (47, 45, 46, 48, 49)
frame 12903: mode 13, level 51: GAMEPLAY. The player stands on the floor (y -566.4) and walks
             with C-up (state 3), down to y -1228.8, into the water (states 11/10), across a
             dozen regions; C-left strafes; Z presses run. No crash to frame 15000.
```

Every gameplay frame is black (no vertices in the display list; the scene draw is t3's), so the
progress is shown by `T2_STATE_LOG` (player position, state, region, floor height). Four ABI
mismatches fixed locally (t1-6 CVector3 by value vs three u32, t1-13 s32/Triple vs f32/CVector3,
t1-16 struct return vs hidden result pointer, t1-20 three s32 vs CVector3): t2's checker should
find their siblings. Files the merge may meet: `include/structs/{particle,world,game,weapon}.h`
(CDLightSet, `CScene.m_DLightSet`, `CAutomap.m_pHostRegion`, `gWeaponInfoTable[35]`),
`port/game/t2debug.c`, `port/app/game.cpp` (the state log), `func_0024C47C.c` (CPlayer__Update
host views), `func_00220530.c` (CScene__Update), `tools/assets/swap_spec.json`.

# Round 4, merged state (history)

Round 4 merged five branches, in this order: s5 (the swoosh/effect system typed, the N64-offset
sweep), s1 (the virtual PIF and Controller Pak), s2 (the boot runner to the title screen), s3
(macOS parity to mode 0) and s4 (audio part 2). Then it fixed what only the merged tree reaches
and followed the attract demo a few steps. This page says how to build and run the result, how
far `turok2 --game` gets, what stopped it on the way and what comes next. The part sections
further down keep each branch's own notes and crash tables (s2-1..16 here, s3-1..19 in
[MACOS.md](MACOS.md) section 6, s4-1..10 below); the round 3 state follows as history. Details:
[INPUT.md](INPUT.md) (PIF and pak), [AUDIO.md](AUDIO.md), [MACOS.md](MACOS.md),
[DATA.md](DATA.md), [64BIT.md](64BIT.md), [RUNTIME.md](RUNTIME.md).

The matching build is unchanged. `tools/asm_diff.py --base 474e40af -j 8` reports 0 differences
over all 3,938 files after every merge and fix. A clean `make clean && make setup && make build &&
make verify` gives the identical ROM (CRCs good, 123 shared .rodata objects agree).

## Round 4: build and run (Linux x86_64, the box)

```sh
# 0. the matching build first: gendata and asm_diff read its ELF; keep a copy as build/ref.elf
#    (the asm_diff/gendata tools expect it, and `make clean` deletes it)
make setup && make -j8 build && make verify && cp build/turok2.us.elf build/ref.elf

# 1. the archive, format 2.0 (t2/swap byte-swap plans); the port refuses 1.x archives.
#    Round 4 changed no asset tool: an archive from round 3 still checks OK.
python tools/assets/mkarchive.py baserom.us.z64 -o OUT/turok2.o2r -j 4
python tools/assets/archive_check.py OUT/turok2.o2r baserom.us.z64     # OK, format 2.0

# 2. configure; the game library AND the runtime/audio libraries (gendata reads all three)
cmake -S port -B OUT/build -G Ninja -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ \
      -DCMAKE_BUILD_TYPE=Debug
ninja -C OUT/build -j8 turok2_game t2runtime t2audio

# 3. the game data as C into port/gen/ (gitignored). Drop the cache after ANY header change.
rm -f OUT/decls.json
python3 tools/port/gendata.py --build OUT/build --cache OUT/decls.json -j 4

# 4. re-run cmake so data.cmake sees port/gen/*.c, then the executable
cmake OUT/build && ninja -C OUT/build -j8 turok2

# 5. run headless (SDL's offscreen driver: no window). --pak-blank: a blank formatted
#    Controller Pak in memory (--pak-dir DIR: turok2_pak<N>.mpk files; --no-pak: none).
cd OUT && SDL_VIDEODRIVER=offscreen T2_SHOT=shot.ppm T2_SHOT_EVERY=300 \
    build/app/turok2 --archive turok2.o2r --rom ../baserom.us.z64 \
    --game --draw --pak-blank --frames 5400
#    --audio / --wav out.wav: the sound world (section 2); gdb -batch -ex run -ex bt --args ...
#    for a backtrace. Debug only: --skip-pak-check, --start-mode N (7: title at frame 3),
#    --input START@300,A@400, T2_MENU_LOG=1, T2_DLDUMP_AT=<frame>, T2_TEST_SFX/T2_TEST_SONG.
```

A fresh `turok2.cfg.json` is fine now: `main.cpp` writes a 640x480 windowed default into a config
that lacks the window keys, and headless runs always start windowed (SDL's offscreen driver
reports its window as fullscreen, so LUS saved `Window.Fullscreen.Enabled = true` at exit and
the next run opened a fullscreen window at the driver's size with the game in its lower-left
640x480).

gendata on this tree: 1,543 data symbols the C needs (1,535 typed from a C declaration), 279
pointer targets, 391 zero .bss symbols, 3,881 pointer slots (2,852 relocated, 997 null, 32
unresolved).

## Round 4: how far `turok2 --game` got

With the virtual pak and no debug switch, the N64 boot runs on its own to the title screen and
into the attract demo:

```
--game --draw --pak-blank --frames 9000          (Linux, headless)
game: frame 2:    engine mode 17 -> 0     pak check: "ACCESSING THE CONTROLLER PAK", the pak passes
game: frame 298:  engine mode 17 -> 1     legal screen, the full text
game: frame 1070: engine mode 17 -> 3     "EXPANSION PAK FOUND!" with the 3D pak model
game: frame 1362: engine mode 17 -> 4     intro cinema 100 (level 162): runs, draws black
game: frame 1852: engine mode 17 -> 5     intro cinema 90 (level 44): runs to its end, draws black
game: frame 4214: engine mode 17 -> 7     TITLE SCREEN
game: frame 5465: engine mode 7 -> 17     the title's timeout: attract demo, mode 13, level 81
SIGSEGV in EnemyAI_Update (func_0025C5F8.c) <- CAI__Update <- func_0021F1B4 <- CScene__LoadLevel(81)
game: PIF 812 SI DMAs, 1 status, 236 pak reads, 169 pak writes   (the first 300 frames)
```

Screenshots (`T2_SHOT_EVERY=300`, 640x480, checked by eye): frame 601 the legal text (beige game
font, centred, every line); frame 1201 the Expansion Pak screen (the rotating pak model, red top,
grille, "EXPANSION PAK FOUND!"); frames 1501 and 2401 (the cinemas) are uniformly black; frame
4500 the title (the TUROK logo with the eye in the O, "SEEDS OF EVIL", the red "2" splash on the
scale background).

With `--audio --wav`: the same sequence up to mode 5. The first cinema's music (`SndSeq_Update`
-> `__CSPVoiceHandler`) stops in `__CSPSetUsptFromTempo` (func_00290D5C) at the start of the
second cinema: s4's open music item (the compressed sequence player is still m2c views). The
mixer runs every retrace before that.

The port tests pass on this tree: `rnc_test` (14,426 streams, 0 failed), `t2asset_test`
(26,124 units, 0 mismatches), `t2asset_typed_test` (1,585 checks x3, 0 errors), `gbi_test`
(0 mismatches), `t2os_test` (trace hash `2b2775877e0d226b`), `t2audio_test --direct 0` (19
frames, peak 32,433), the swoosh self test (`turok2 --game`: OK, 0 failed), `turok2` without
`--game` (26,124 units on 3 backends, fnv1a64 `e9b9d3e7c226d5aa`).

macOS: s3 ran mode 0 headless with display lists identical to Linux (s3 alone; the merged tree
was not rerun on the Mac in this round). The merge fixes below are host-layout fixes that apply
there too.

## Round 4: crash and fix table (merge)

Paths no single branch reached. Every fix is under `TARGET_PORT` or port-only, asm_diff 0.

| # | where | cause | fix |
|---|---|---|---|
| M1 | `WeaponActor_SwitchToRequestedInline` (mode 3 -> 4, `CPlayer__SetupActors`) | the weapon actor's owner read at +0x1A8 (N64 `m_AI.m_pParent`) and the player's requested weapon at +0xB14 | `m_AI.m_pParent` / `m_RequestedWeapon` in 10 weapon files; `WeaponActor_InitAI` host body (CAI members, the actor pointer-wide) |
| M2 | `CGameRegion__GetHeight(region = a stack address)` from `CCamera__SetViewParams` | **ABI**: the definition takes a `Shared_Quad` (union of s32/f32: integer registers on x86-64), the caller a `CQuaternion` (floats: SSE registers), so every later argument arrived in the wrong register; the GameplayCamera view at N64 offsets | host body on `CCamera.m_ViewParams` (`CCameraViewParams`) |
| M3 | `CScene__GetRegionInfo(0xffffffff402bfb18)` | `ConstructFromRecord` passed the region as `s32` to a pointer parameter (upper half garbage) | s3's 0d572c47 (merged) |
| M4 | `CList__Construct(0xe58)` from `CCameraPool__ResetEmitterPools` | camera table/count at N64 offsets 4/8, stride 0xEC0 | host body on `CCameraPool`/`CCamera.m_SkySystem` |
| M5 | `CMtxF__GetTranslation(0x14c)` from `WeaponActor_BuildMatrix` | the weapon model view and the camera's 0x12C/0x14C | the actor's members and `m_mfViewOrient`/`m_qViewAlign` |
| M6 | `CGameRegion__GetNormal(0x8006ff5df)` from `CalculateOrientationMatrix` | **ABI**: `GameObject_GetFloorTilt` returns a `CQuaternion` by value (MIPS: a hidden result pointer, which the caller's prototype passed; x86-64: registers); the LevelActor view has the N64 header size | host body on `CGameObjectInstance`, the quaternion by value |
| M7 | `CCache__Unlock(NULL)` from `CScene__ReleaseSectors` | the scene's sector table at N64 offsets | host body on `CScene` (`m_nSectors`, `m_Sectors[].m_hData`, counters) |
| M8 | `CAI__SetMode` calls garbage (attract demo, enemies) | the base mode table `D_800B0700`, which every AI table chains to through its header row's integer `m_Mode`, was used by no C, so gendata copied it as N64 bytes | declared `CAIModeInfo D_800B0700[27]` (globals/ai.h) and named from the host `CAI__SetMode`: gendata lays it out (3,798 -> 3,881 pointer slots) |
| M9 | `GameObject_TraceMove(outPos NULL, outRegion 7)` from impact event 7 (`Door_CloseEnter`) | **ABI**: the Rage Wars `Triple` (three s32) by value vs the callers' `CVector3` | `func_00237FAC` takes a `CVector3`, `func_00238858` the actors pointer-wide |
| M10 | `CScene__LoadLevel` | the s5/s2 conflict resolution kept s5's whole file and lost s2's `sizeof` stride of the second actor loop | restored |
| open | `EnemyAI_Update` (func_0025C5F8.c:110) | the enemy AI read through an `AI`/`Object` view at N64 offsets (`modeIndex` at 0x133, `age` 0xCB): the behaviour index is garbage | next |

Three of these (M2, M6, M9) are one class of bug the offset sweep does not see: a struct passed
or returned **by value** whose declarations disagree between an integer-class struct (`Triple`,
`Shared_Quad`) and a float struct (`CVector3`, `CQuaternion`), or a by-value return
that a Rage Wars prototype turned into a hidden pointer. MIPS passes all of them the same way,
x86-64 and arm64 do not. 31 files use `Triple`; the impact-event handlers
(`ImpactEvent_*`, func_0023870C..func_00238FD4) take one and are called with a `CVector3`.

## Round 4: next (as it stood)

1. **The attract demo / levels run**: `EnemyAI_Update` and the rest of the enemy AI on `CAI`
   (s5's counter: func_002568CC 31 sites, func_0043EC68 19, func_00258964 12, ...).
2. **By-value struct ABI audit**: a checker (libclang over all declarations of each function:
   parameter/return classes on x86-64 and arm64) and then the `Triple` handlers; cheaper than
   finding them one crash at a time.
3. **Black cinemas**: done for static geometry in round 5 (t3, RENDER.md); animated instances
   (the Acclaim logo, every actor) need the bone/animation path on the host layout.
4. **Audio**: a sound effect without gdb crashes in `t2asp.c` `cmd_adpcm` (the ADPCM state
   address, `osVirtualToPhysical`); music stops in `__CSPSetUsptFromTempo` (the `__CSP*`/`alCSeq*`
   player is still m2c views); speech is stubbed (s4).
5. **macOS beyond mode 0**: rerun the merged tree on the Mac (the merge fixed several of the
   paths s3 listed); then the AI action callbacks that take the actor as `s32`
   (`Action_IdleEnter` -> `CScene__SetInstanceSavedBit`), and `MtxPool_Alloc` declared `s32` in
   func_0021CDAC.c.
6. **Pixel screenshots on macOS**: no headless Metal/GL there; the Mac compares display lists
   (`T2_DLDUMP_AT`). A Metal offscreen target (or a window run by a person) for pixels.
7. Input: drive the title menu into a level (`--input` scripted presses, s1) once the AI runs.

# Round 3, merged state (history)

Round 3 merged five branches, in this order: r2 (archive format 2.0, the loader through the
archive in host byte order), r1 (the boot runner to mode 0), r3 (the 64-bit typing sweep), r5
(macOS arm64) and r4 (audio behind `--audio`). Then it fixed what the merge broke and followed
the black frame to the text and the menu. This page says how to build and run the result, how
far `turok2 --game` gets, what stopped it on the way (one table for the whole round) and what
comes next. The part sections further down keep each branch's own notes. Details:
[DATA.md](DATA.md) (gendata), [64BIT.md](64BIT.md) (part 2: retype.py), [AUDIO.md](AUDIO.md),
[MACOS.md](MACOS.md), [RUNTIME.md](RUNTIME.md) (13: the typed scheduler),
[../assets/PORT.md](../assets/PORT.md) 10.4/10.6 and [../assets/ARCHIVE.md](../assets/ARCHIVE.md)
(`t2/swap`).

The matching build is unchanged. `tools/asm_diff.py --base 3798e01e -j 8` reports 0 differences
over all 3,938 files after every merge and fix. A clean `make clean && make build && make verify`
gives the identical ROM (CRCs good, 123 shared .rodata objects agree).

## Round 3: build and run (Linux x86_64, the box)

```sh
# 0. the matching build first: gendata reads its ELF (build/turok2.us.elf)
make setup && make -j8 build && make verify

# 1. the archive, format 2.0 (t2/swap byte-swap plans); the port refuses 1.x archives
#    (sha1 99cbe76733bfab1d7235e4894cec2b91be3965f7 with zlib 1.3.1, any -j)
python tools/assets/mkarchive.py baserom.us.z64 -o OUT/turok2.o2r -j 4
python tools/assets/archive_check.py OUT/turok2.o2r baserom.us.z64     # OK, format 2.0

# 2. configure; build the game library AND the runtime/audio libraries: gendata reads the
#    undefined symbols of all three (t2audio_mgr.c reads gAudioFrameSize, gAudioGlobals, ...)
cmake -S port -B OUT/build -G Ninja -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ \
      -DCMAKE_BUILD_TYPE=Debug
ninja -C OUT/build -j8 turok2_game t2runtime t2audio

# 3. the game data as C, into port/gen/ (gitignored: ROM content); libclang 19 python bindings.
#    Drop the cache whenever a header changes (it holds the types): sizes and aliases follow them.
rm -f OUT/decls.json
python3 tools/port/gendata.py --build OUT/build --cache OUT/decls.json -j 4

# 4. re-run cmake so data.cmake sees port/gen/*.c, then the executable
cmake OUT/build && ninja -C OUT/build -j8 turok2

# 5. run headless and draw (SDL's offscreen driver: no window; --no-window would skip Fast3D)
SDL_VIDEODRIVER=offscreen T2_SHOT=OUT/frame.ppm gdb -batch -ex run -ex "bt 30" \
    --args OUT/build/app/turok2 --archive OUT/turok2.o2r --rom baserom.us.z64 \
    --game --draw --frames 300            # add --audio for the sound world (section 2)
```

`T2_SHOT=<file.ppm>` writes the last frame Fast3D drew (640x480). `T2_DLDUMP=1` prints the last
display list; it is read after the run, when the game may already be filling that buffer again,
so trust a breakpoint over it for single commands.

gendata on this tree: 1,545 data symbols that module-1 C and the runtime need (1,539 typed from
a C declaration), 393 zero .bss symbols (29 byte views doubled for host pointers), 3,794
pointer slots (2,770 relocated, 994 null in the ROM, 30 unresolved).

## Round 3: how far `turok2 --game` got

```
t2: boot: arena 32 MB at 0x40200000 (low memory), ROM baserom.us.z64, mainproc started (pri 13)
game: booted, 300 frames
t2: Sound__Initialize: audio off (run with --audio)
t2: __osSiRawStartDma: PIF not modelled (no Controller Pak yet)
game: 300 frames in 2.78 s, 1414 thread switches, 151 gfx tasks (151 drawn), 0 audio tasks,
      0 PI DMAs, 0 missing-function calls
OK (0 failed checks)
```

Boot -> `CEngineApp__Main` -> the engine, camera pool, player pool, `CScene__Construct` and
`CScene__LoadLevel(0)` -> mode 17 -> **mode 0 (pak check)**, which draws every retrace through the
game's scheduler into Fast3D: 300 frames, 151 display lists drawn, no crash.

What the frames show now (they were black with a 4-pixel red bar at the end of r1):
- frames up to ~120: **"ACCESSING THE / CONTROLLER PAK"** in the game's small font, beige,
  centred at the right place on the 480x360 screen;
- from frame ~120, the timer opens the pak menu (`gPakSaveGameAnyPakMenuDesc`, then
  `gDamagedPakAtBootMenuDesc`, because no Controller Pak is modelled): the menu's **background
  image** (the reptile-scale texture, full screen, darker bands at top and bottom) and the
  **red highlight bar** of the selected line, centred. The menu lines themselves draw no
  glyphs yet (below).

With `--audio`: the sound bank is converted (2 bank files, 34 instruments, 1,115 sounds, 866
wave tables), the sound data file arrives in host order through the archive (the audio hook
sees that and leaves it alone), the audio manager starts. Mode 17's `Sound__StopAllExcept` now
works (the voice pool and request queue typed, rows I5/I6). It then stops in
`SndQueue_CancelHandle` from `CAudioWorld__StopHandle` <- `Sound__Stop` <- `LoopSounds_StopAll`
<- `CScene__LoadLevel` (mode 0's init): `CAudioWorld__StopHandle` reaches the queue through the
Rage Wars `ContextRecAt2BC0` view of `CAudioWorld` at N64 offsets.

macOS arm64 (r5, [MACOS.md](MACOS.md)): the whole port builds and the tests pass. The game runs
headless with full 64-bit pointers (a low-memory mode is impossible there). With r1+r3+r5 it gets
past the camera pool and stops in `Model_Decompress` -> `Material_Unpack` (section 4).

## Round 3: crash and fix table (all parts)

Rows r1-* and r4-* were found on the branches; I-* during the merge (C18). Every fix is under
`TARGET_PORT` or in a port-only file, asm_diff 0.

| # | where | cause | fix |
|---|---|---|---|
| r1-1 | `CScene__GetRegionInfo` via `CCamera__AdvanceAttributes` | `CEngineApp`/`CGame` held the scene, camera pool, onscreen as N64-sized bytes | `structs/game.h` under TARGET_PORT: typed one-element-array members; literal CEngineApp/CGame offsets by `T2_OFFSETOF` |
| r1-2 | same | gendata translated offsets inside a union through its last member | a union prefers the member that types the offset |
| r1-3 | `CCamera__AdvanceAttributes` | `CCameraViewParams__Construct` wrote through a 4-byte-pointer view | host body |
| r1-4 | `Math_AcosLookup(NaN)` | libultra's `du` doubles generated by their words | gendata: a union's whole-size scalar member lays out the data |
| r1-5 | `CHeap__Free(NULL)` | `gPoolBuffers` generated at its N64 0x20 bytes | gendata doubles byte-only .bss; `CCache__AllocateAt` through `CMemEntry` |
| r1-6/7 | player / loader lists | link offsets 0x131C/0x1320, 0x18/0x1C | `T2_OFFSETOF`; host `PlayerList` |
| r1-8 | `CCache__DetachCacheEntry`, `__FreeCacheEntry` | m2c offsets | typed bodies |
| r1-9 | `CCache__LoadPersistantIndex` | `s32 cart` received a pointer | `sptr cart` |
| r1-10 | `Model_Decompress` | decompression blocks written at N64 offsets | r2's typed `CCache__AllocDecompressFlags` |
| r1-11 | `SwooshSystem_UpdateSwooshes` | swoosh/effect system as N64 bytes | **stub** (left empty on the host) |
| r1-12/13 | CScene views, `CGameObjectInstance__SetupAnimation` | N64 offsets, region stride | by field / typed |
| r1-14 | `__scMain(sc=NULL)` | scheduler views overran on LP64 | `port/game/t2sched.c` (17 functions typed, originals out of the link) |
| r1-15 | `SndVoicePool_*` from `Sound__StopAllExcept` | no sound bank | `Sound__*` return early while `gAudioWorld.bank` is NULL (they stay: they only fire without `--audio`) |
| r1-16/17 | delayed level events, actor lists | bytes / m2c offsets | typed |
| r1-18 | `CCameraPool__AdvanceText` | camera text/shake/emitter lists, sky system | links by `T2_OFFSETOF`, `CEmitterPool m_SkySystem[1]` |
| r1-19/20 | Fast3D `G_MOVEMEM` | `DlCmd` 32-bit words; static display lists with N64 addresses | `DlCmd` host words; gendata relocates w1 in static lists |
| r1-21 | `Menu_Construct` & co. | menu sizes, links, desc tables, desc text addresses | `MENU_SIZE`...; pointer-target tables; `t2_n64_to_host` |
| r4-A1..A5 | `alCSPNew`, heap, `alSndpAllocate`, `alEvtqPostEvent`, `CAudioWorld__Lock` | views at N64 offsets | typed (AUDIO.md) |
| r4-A6 | `_sndpVoiceHandler` spins | `player.evtq` overwritten | open on r4; not reached now (I6 stops first) |
| I1 | link: `gAudioFrameSize`, `gAudioGlobals`, `gAudioMaxRSPCmds` undefined | r4's audio manager files left the game library, so gendata no longer saw these names | gendata also reads `libt2runtime.a`/`libt2audio.a`'s undefined data names (build them before gendata) |
| I2 | merge r1+r3 does not compile | r3's `sptr` prototypes vs r1's `s32` port bodies | r3's prototypes kept, r1's bodies adapted (`CCache__AllocateAt`, `..ForDecompression[Top]`, `Sound__Play`/`SetListener` with the guard, `MenuPool_Construct`) |
| I3 | `VM_Alloc` (r2 vs r4) | r2 reads the unit through the archive in host order; r4 copied ROM bytes and swapped | r2's `T2Port_AssetWindow` + r4's `T2Audio_FixRegion` (detects host order) |
| I4 | pak menu lines measured 0 wide | the menu strings (`D_800B50C0` "damaged controller pak found in" ...) are address-only labels (SHN_ABS) in `.code_rodata`; gendata made every SHN_ABS symbol zero .bss | gendata reads SHN_ABS labels inside `.code_rodata`/`.code_rodata_tail` as data (`ABS_DATA_SECTIONS`) |
| I5 | `--audio`: `SndVoicePool_StopAllExcept` (mode 17) | Rage Wars views of the voice pool (0xCC-byte slots, 4-byte owner) | `SndVoicePool_Construct`, `SndVoicePool_StopAllExcept` typed on `SndVoicePool`/`SndVoice` |
| I6 | `--audio`: `SndQueue_CancelHandle` | the same for the request queue | `SndQueue_Construct` and `SndQueue_Cancel{AllExcept,Handle,SoundId,Owner,Flags}` typed on `SndQueue`; now stops in `CAudioWorld__StopHandle` (open) |
| I7 | no text, no font texture | `CScene__LoadGraphicSet` read the scene through an N64-offset view (0x1C, 0x48, 0x7C): the font graphic set was never found, `sOnScreenFontTexture` stayed NULL | typed on `CScene` (`m_GraphicSetsKey`, `m_pGraphicSets`, `m_pGraphicSetTypes`) |
| I8 | texture rectangles of size 0, black text | `COnScreen__DrawString`/`DrawText` read colours, scale and spacing through `FontStyle { u8 pad0[0xA0]; ... }` | pad by `T2_OFFSETOF(COnScreen, m_TopR)` |
| I9 | wrong glyphs ("!## 33)" for "access") | `COnScreen__Construct` built the character tables through a `Hud` view at N64 offsets (0xC8/0x148) | typed on `COnScreen` (`m_CharacterSmallLookUpPos`, +0x80) |
| I10 | everything drawn 1.5x too large, off to the right | Fast3D's native size is 320x240; the game draws at 480x360 | `game.cpp` sets the interpreter's native size from `gScreenWidth`/`gScreenHeight` before each frame |
| I11 | menu at the left edge, no background | `MenuPool_GetCamera` = pool - 0x53C (N64 `offsetof(CCamera, m_MenuPool)`): the menu's camera and viewport were 44 bytes into the camera | `pool - T2_OFFSETOF(CCamera, m_MenuPool)` (`sptr`) |

## Round 3: next (as it stood)

1. **The pak menu's lines.** They are laid out and highlighted but draw no glyphs: the items of
   `gDamagedPakAtBootMenuDesc` reach `MenuItem_DrawString` with `text` pointing into the desc
   table itself (`D_0043BF18 + 0x38`, an empty string), while the first menu's items map their
   N64 text addresses correctly (`t2_n64_to_host`). Follow the item text of this desc (its update
   callbacks `DamagedPakMenu_UpdateLine1/2` and the `data` words of the .virtual desc table).
2. **Controller Pak / input** (done in round 4 s1, below): `__osSiRawStartDma` was not modelled, so the game always sees a
   damaged pak and never gets past mode 0. Model the PIF (controllers, then a pak file) to reach
   the title screen.
3. **Audio**: `CAudioWorld__StopHandle` and the rest of the `CAudioWorld` API (about 40 functions:
   `SndVoicePool_*`, `SndVoice_*`, `SndQueue_RequeueVoice`) go through Rage Wars views of
   `CAudioWorld` at N64 offsets (`ContextRecAt2BC0`, `RecordCode8025A3EC`, ...). Type them on
   `CAudioWorld`/`SndVoicePool`/`SndQueue` as I5/I6 did; then r4's open `evtq` corruption.
4. **macOS** (full 64-bit, r5 found on r1+r3+r5): `Material_Unpack` (func_00207914) writes the
   texture handle into `Material.texture` (an `s32` inside the model block) and `texture + 4`:
   needs a host-typed `Material` or a side table. Then five more truncations listed in
   [MACOS.md](MACOS.md) (decompress callbacks with `s32` arguments, the
   `LoadPersistantSubBlock` callback argument, the CAI view in `DeallocateInternal`,
   `LoadBitSections` with an `s32` destination; r3's e9b1dfcc covers part of these: re-check).
5. The swoosh/effect system (CEngineApp 0x19608-0x22BA0, r1-11) typed instead of left empty.
6. 64-bit: 1,727 pointer-width warnings, **1,019 truncating** (`retype.py --dry --rules ""` on
   this tree; 2,828 before r3), 999 disagreeing declarations. Each crash above was one of them.

## Round 4, part s5: the swoosh/effect system typed, N64-offset sweep (menus, cameras, level load)

Branch `names-R4_s5` (from 474e40af). Every commit: `tools/asm_diff.py --base 474e40af -j 8` = 0
differences (over the whole tree, 3,938 files, for the commits that touch `include/`).

**The swoosh/effect system is typed and runs on the host (r1-11's stub is gone).**
`include/structs/swoosh.h` lays out `CSwooshSystem` (`gEngineApp_m_SwooshSystem`, CEngineApp +
0x19608, N64 size 0x95B4: it runs 0x1C bytes into `unk22BA0`): the ribbon vertex pool (2 x 300
`SwooshVtx`, the count at 0x2580, the cursor at 0x2584), 96 `SwooshNode` points (0xB8) and their
free list at 0x6A88, 32 `Swoosh` trails (0x54) with their NodePool at 0x7514 (live swooshes =
`m_SwooshPool.used`, head 0x7520), 3 `SwooshTimer`s (0x1C) with their pool at 0x7580, 100
`SwooshDecal`s (0x50, ground shadows/scorches) in 10 kind `CList`s at 0x94D8 and the free list at
0x95A0. mips `_Static_assert`s keep the N64 offsets. `structs/game.h` (TARGET_PORT) embeds it as
`union { CSwooshSystem m_SwooshSystem[1]; u8 _SwooshBytes[0x22BC0 - 0x19608]; }`, so gendata's
alias `gEngineApp_m_SwooshSystem` and everything after it follow the host size.

The owner of a swoosh is a `CGameObjectInstance` when `flags & 1` (count `m_ActiveSwooshes` at
0x10F) and a `CParticle` when `flags & 2` (0x1B5); `Trail_AddNode` takes a `CParticle` (effect
flags, `m_pOwner` as the key with `flags & 8`, `header.m_nObjType` as the id).

| what | files |
|---|---|
| on the shared types (same C for N64 and host) | `func_0042D410` (init: the host stub is gone), `SwooshSystem_Update`, `_UpdateSwooshes`, `_UpdateTimers`, `RibbonVtxPool_Reset`, `Swoosh_TakeNode`, `SwooshGroup_Remove`, `Swoosh_NewNode`, `Swoosh_DetachOwner`, `func_002368B0`, `func_002360A0`, `Swoosh_AddParticleNode`, `func_002364A4` (decal), `SwooshSet_FreeOwner`, `func_00236444` (timer), `func_00235C30` (decal draw), `Trail_AddNode`, `Trail_DrawAll`, `Trail_Draw` |
| views with their pointer members (host-correct, N64 identical) | `Swoosh_Update`, `Trail_DetachOwner` (CParticle, owner counts by type; its `register asm` hack is N64-only), `Trail_PlaceNodes`, `Trail_ComputeNodeFrame`, `Trail_ComputeFrames`, `Trail_DrawTextured`, `Morph_BlendVertices(Tex)`, `func_00236D10`, `func_00236D50`; `rw/abu_types.h`: `Actor_func_802A274C_de`, `ObjRecAt28V1`, `State802A6DF8`, `Node438C`, `Morph` (its pointer/float unions are floats on the host) |
| host bodies (`#ifdef TARGET_PORT`) | the generic lists `DList_AddHead`, `DList_AddTail`, `NodePool_Alloc`, `NodePool_Free`, `func_0026D230` (they wrote 4-byte words at N64 offsets); `RibbonVtxPool_Alloc` returns the vertex pointer at host width; the decal draw reads the matrix translation as host words |

Check: `port/game/t2swoosh.c` (`T2Swoosh_SelfTest`, run by `turok2 --game` after the frames; one
line in `port/app/main.cpp`) builds a private `CSwooshSystem` with the game's code and checks the
pools (96/32/3/100, links both ways), a timer firing twice and freed, a swoosh of three points
(lengths 5 + 12, fractions, removing a point), a decal placed and freed by its owner, the ribbon
pool (host pointers, the 300 limit): `t2: swoosh self test: OK (0 failed)`. In the game the
system is built twice (mode 17 and mode 0's `CScene__LoadLevel`) and updated every frame; mode 0
spawns no swoosh, so the draw functions have only run on empty lists so far.

```
game: 300 frames in 2.78 s, 1414 thread switches, 151 gfx tasks (151 drawn), 0 audio tasks, 0 PI DMAs, 0 missing-function calls
t2: swoosh self test: OK (0 failed)
OK (0 failed checks)
```

**N64-offset sweep** (boot/menu/camera path). `tools/port/n64_offsets.py` counts the sites
(`M2C_FIELD`, `((s8 *)x) + 0xNN`, `*(T *)(x + 0xNN)`, `P_S32(p, 0xNN)` macros):
474e40af **378 sites in 135 files** -> **345 in 118** (m2c 62 -> 62, casts 161 -> 139, derefs
87 -> 80, macros 68 -> 64). Not in that count: the 34 swoosh files above, whose local pad views
were the larger part. Fixed here:
- the camera's menu pool, `(u8 *)player->m_pCamera + 0x53C` (I11's offset, used by every
  `MenuPool_OpenMenu`/`CountOpen` caller: pak menus, save portal, character/player setup,
  rumble, messages): `+ T2_OFFSETOF(CCamera, m_MenuPool)` in 21 files, the module-2 copies (func_0045xxxx)
  included;
- `CPlayer` views: `m_pCamera` (0x51C), `m_ContState.m_pContState` (0xA50),
  `m_Data.m_CheckPointWarpID` (0x92C) in `ShowPlayerMessage` (func_004233E8), func_00255C80,
  `SavePortalMenu_*` (func_00423210/278/364);
- `CScene__LoadLevel` (mode 0's init): the actor stride 0x288, `m_WeaponActor` (+0x288) and the
  player link 0x1320;
- `CCamera__GetViewOrigin` (0x24C, `m_vTCorners`), `CCameraPool__ReleaseAllTexts` (main camera
  0x40, `m_ActiveTextList` 0xE28/0xE2C, `m_FreeTextList` 0xF0C).

For the merge:
- `include/structs/game.h` (the CEngineApp swoosh member, TARGET_PORT only), new
  `include/structs/swoosh.h`, `include/rw/abu_types.h` (5 structs: pads -> the same-size
  pointer members), `port/app/main.cpp` (one CHECK line + one extern), new
  `port/game/t2swoosh.c`, new `tools/port/n64_offsets.py`.
- s2 territory touched by one-token edits only: the `+ 0x53C` lines of the pak/menu files
  (func_0041DF50, func_0041E1D8, func_0041EA84, func_0042019C, func_004206B8, func_0042085C,
  func_00423768, func_00423880, func_004183C0 and the `_2` copies); `func_00413230.c` itself is
  untouched.
- Rerun gendata with a fresh cache (CEngineApp's host layout changed after +0x19608).

Next (s5's area):
1. Exercise the trail draw path in the game (a level with swooshes: `Trail_DrawTextured` /
   `Morph_BlendVertices` write vertices and GBI through views that are now host-sized, but the
   textured path also reads `Texture_Request(trail->model)` handles (`s32 texture[2]` at 0x14,
   func_00207914's handle: s3's `Material` work on macOS changes it, keep the two in step).
2. `D_80119850` (`B21World`, func_00289F30) is a view from CEngineApp + 0x22BA0 into CGame at N64
   offsets (`sub5DC` = the menu pool): retype on `gEngineApp_m_Game`.
3. The 345 counted sites (top: func_002568CC 31 (player), func_0043EC68 19, func_00258964 12,
   func_00255F04, func_00275450 (`alCSeqNew`) 8), and the local pad views that the counter does not
   see (the swoosh files were 34 of them): the same recipe, `T2_OFFSETOF`/typed members, asm_diff
   per batch.

## Round 4, part s1: controllers and the Controller Pak (mode 0 passes)

Details: [INPUT.md](INPUT.md). Branch `names-R4_s1`, 4 commits on 474e40af.

- **Virtual PIF** (`port/runtime/t2pif.c`) behind `__osSiRawStartDma`: joybus status/reset,
  pak block read/write with address and data CRCs, over an emulator-standard 32 KB `.mpk` per
  port (`turok2_pak<N>.mpk` in the user data dir; blank formatted when missing; written block by
  block). The game's own libultra pfs C runs unchanged: the PIF converts the pack ID, inode and
  directory blocks between the file's big-endian layout and host order. Flags: `--pak-dir DIR`,
  `--pak-blank` (in memory, tests), `--no-pak`; debug: `--input START@300,A@400` (scripted
  presses), `T2_PIF_LOG=1`, `T2_MENU_LOG=1`. The run logs engine mode changes and PIF counters.
  Rumble Pak: "not present" (unchanged).
- **Game-side fixes** (TARGET_PORT only; asm_diff 0 over 3,938 files): pak address high byte
  first in `__osPackRam{Read,Write}Data`; queue as `OSMesgQueue *` in `__osContRam{Read,Write}`;
  typed `__osPfsSelectBank(_2)`; `__OSInodeUnit` `{page, bank}` on little-endian hosts
  (`include/ultra_types.h`); `MenuRecAt24` (`include/rw/abu_types.h`) and
  `PakConfirmMenu_SelectNo`'s `Item` lead with the host `Menu` fields (both crashed in the
  repair/continue confirm menus).

How far it gets now (Linux, headless, `--game --draw`):

```
--pak-blank : mode 0 -> f297 mode 1 (legal screen, full text) -> f1069 mode 3 ("EXPANSION PAK
              FOUND!", 3D model) -> f1357 mode 4 (cinema) -> SIGSEGV
--no-pak    : pak menu, any button -> mode 1
zeroed .mpk : damaged-pak menu -> attempt repair -> yes -> repaired (the file is a valid
              big-endian formatted pak) -> continue without saving -> mode 1
```

Where it stops (mode 4, not in s1's scope):

```
CCameraViewParams__Construct (o=0x31)        func_00280C28.c:12
CCamera__Construct (view=0x1)                func_0042F9B0.c:37
CCameraPool__AllocCamera (gEngineApp_m_Game_m_CameraPool) func_0042FF58.c:21
CPlayer__NewGame (character=0)               func_0042DC18.c:33
CPlayerPool__NewGame (count=1)               func_0042E5AC.c:96
CEngineApp__StartWarp (.., 100)              func_00288DD0.c:26
EngineMode04_Cinema_Init                     func_00289554.c:6
EngineMode17_Switch_Retrace                  func_00289DFC.c:42
```

For the merge: files others may touch: `include/ultra_types.h` (inode union),
`include/rw/abu_types.h` (`MenuRecAt24` only), `src/us/code/func_00420F34.c`,
`port/app/game.cpp` / `main.cpp` / `game.h` (flags, mode log, scripted input),
`port/runtime/t2ultra.c` (`__osSiRawStartDma` moved to t2pif.c), `port/runtime/t2port.h`
(`T2PortConfig.pakMode/pakDir/pakPorts`), `port/app/CMakeLists.txt` (+t2pif.c),
new `port/game/t2modelog.c`. Header changes: drop `decls.json` before gendata.

Next: the mode 4 camera crash; a swap plan for the save records so notes move between the
port and emulators (the pak filesystem already does); a Rumble Pak mode; macOS: the same code
is LP64-clean (the queue truncation is fixed) but untested there.
## Round 4, part s2: boot runner part 2 (mode 0 to the title screen)

Branch `names-R4_s2` (from 474e40af). Every commit has `tools/asm_diff.py --base 474e40af -j 8` at
0 differ or fail over the files it touches; the one include change (`structs/world.h`, under
`TARGET_PORT`) was checked over the whole tree (3,938 files, 0 differ).

### How far it gets

```
SDL_VIDEODRIVER=offscreen T2_SHOT=out.ppm T2_SHOT_EVERY=300 build/app/turok2 --archive turok2.o2r \
    --rom baserom.us.z64 --game --draw --frames 5000 --skip-pak-check
game: frame 2: engine mode 17 -> 0          pak check: "ACCESSING THE CONTROLLER PAK"
game: frame 16: engine mode 17 -> 1         legal screen, the full text
game: frame 788: engine mode 17 -> 3        "EXPANSION PAK FOUND!" with the 3D pak model
game: frame 1080: engine mode 17 -> 4       intro cinema 100 ("acclaimlogo", level 162): runs 486 frames, draws black
game: frame 1570: engine mode 17 -> 5       intro cinema 90 (level 44): runs to its end, draws black
game: frame 3932: engine mode 17 -> 7       TITLE SCREEN (logo, "SEEDS OF EVIL", the scale background)
game: 5000 frames in 32.31 s, 1911 gfx tasks (1911 drawn), 0 missing-function calls
```

The whole boot sequence of the N64 game now runs on its own, timers and cinemas included, from
the pak check to the title. `--start-mode 7` reaches the title at frame 3 for quick checks.
With s1's PIF/pak the debug switch is not needed for mode 0 (s1 reported mode 0 -> 1 -> 3 -> 4
and the crash in mode 4's `CCameraPool__AllocCamera`, fixed here: row s2-3).

After the title's timeout (frame 5183) the game starts the attract demo (mode 13, level 81) and
stops in an enemy's mode setup:
```
#0 CAI__SetMode (arg2=0)  func_00243414.c:203   entry->unk_04 (the mode's enter function) is garbage
#1 EnemyAI_InitDispatch                          func_0025BE88.c:52   the enemy's mode table, set through N64 views
#2 func_0021E5D4 <- CScene__LoadLevel(81) <- CScene__UpdateWarp <- EngineMode13_Game_Draw
```
That is the start of "levels run" (the enemy AI, s5's sweep: ~400 offset sites).

Screenshots (`T2_SHOT_EVERY`): the damaged-pak menu with its lines ("PROBLEM!", "DAMAGED
CONTROLLER PAK FOUND IN CONTROLLER #1.", "ATTEMPT REPAIR" highlighted, "RETRY", "CONTINUE WITHOUT
SAVING"), the legal text, the Expansion Pak screen with the rotating pak model, the title. The two
cinemas draw black frames: their scene draws no geometry yet (no `G_VTX` in the frame's list).

### Debug switches (DEBUG ONLY, port/game/t2debug.c)

| switch | effect |
|---|---|
| `--skip-pak-check` | once mode 0 has started, sets `gPakCheckDone` ("continue without saving"): `EngineMode00_PakCheck_Retrace` takes its own exit to mode 1 |
| `--start-mode N` | once mode 0 has started, `CEngineApp__SetMode(&gEngineApp, N)` (7: title) |
| `T2_SHOT_EVERY=n` | with `T2_SHOT=<f.ppm>`: also `<f.ppm>.<frame>.ppm`, the first drawn frame of every n |

Every run logs the engine mode changes (`game: frame F: engine mode A -> B`). The two switches go
away once s1's pak model is merged (keep them until the demo path runs, they save 1000 frames).

### Crash and fix table (round 4, s2)

| # | where | cause | fix |
|---|---|---|---|
| s2-1 | pak menu lines draw nothing | `MenuItem_DrawText` read the item as a `Menu` and passed `->desc` (N64 0x14 = `MenuItem.text`; host: `MenuItem.def`, the desc row, an empty string) | typed on `MenuItem`, passes `text` |
| s2-2 | mode 0 never ends | no PIF/pak (s1) | debug switches above |
| s2-3 | `CCameraViewParams__Construct(0x31)` from `CCameraPool__AllocCamera` (mode 4 `CPlayer__NewGame`) | pool view with the free/active lists at N64 0xC/0x20 | `CCameraPool__AllocCamera(_2)` typed on `CCameraPool` |
| s2-4 | `MenuPool_CountOpen` from `Automap_Update` | camera menu pool as `m_pCamera + 0x53C` (23 files; three through raw player offsets 0x51C/0xA50) | `T2_OFFSETOF(CCamera, m_MenuPool)`, `player->m_pCamera`, `m_ContState.m_pContState`; `MenuPool_GetCamera_2` as I11 |
| s2-5 | `func_00267DBC`, `func_0021E5D4` (actor init), `func_0021F1B4`, `CScene__LoadLevel` | N64 views: intelligence at 0x14, AI at 0x140, flags 0xD4, 0x288/0x1328 strides, player link 0x1320 | typed / host body / `sizeof`, `T2_OFFSETOF` |
| s2-6 | `InteractiveAnim_Advance` | AI vars at 0x10C | typed on `CAI.m_VarS32/F32` |
| s2-7 | `CScene__BuildActorLists`: `m_pActors` = 0x190_402B4900 | `CScene.m_Heap` is `u8[0x10]` but holds a pool of pointers (24 host bytes): `MtxPool_Reset` wrote over `m_pActors` | `MtxPool` (structs/world.h), `CScene.m_Heap` typed under TARGET_PORT; `MatrixHeap_Construct`, `MtxPool_Reset`, `MtxPool_Alloc` typed |
| s2-8 | `CCache__Deallocate(0x50)` from the cinema spline | `TempAlloc` kept the handle as an s32, `TempFree` read `((s32 **)data)[-4]` (32 bytes back on the host) | the whole pointer, 0x10 bytes back (TARGET_PORT; both cinema copies) |
| s2-9 | archive abort: grid section `0x01F748E4` not a unit | `CScene__DecompressPaths` wrote the paths through a view at 0x58/0x68 (N64 `m_pRegionData`/`unk_68`): on the host that was `m_pCollision` | typed on `CScene` |
| s2-10 | Fast3D `G_VTX` with w1 `0x00800080020000D7` | the swoosh display list `D_800ACFC8` declared `u8[]`: gendata emitted ROM bytes | declared `Gfx[]` (gendata relocates it) |
| s2-11 | Fast3D runs matrices as commands (`G_DL` to a freed model) | `CCache__LockForDisplayList`, `Begin/EndDisplayList` and the cache thread `CCache__Main` used `D_800D7BE0` (gCache - 0x5F0) through `B12Mgr`/`B12Node`/`B12Msg` N64 views: the lock bit went into `CMemEntry.lockCount`; `CCache__Main` waited on a queue never created on the host (async loads and display-list releases never completed) | the four typed on `gCache`/`CMemEntry`/`CCacheEntry` (TARGET_PORT bodies) |
| s2-12 | Fast3D runs stale commands (the frame's list 8 bytes off) | `CCamera__DrawGlares` wrote one command as two u32 and stepped 8 bytes | one host `Gfx` (TARGET_PORT) |
| s2-13 | intro cinema never ends (`clipStart` -4761, 9007 cues) | `Pack_Add` called the memcpy `func_0029E460(data)`: m2c dropped the source and size (still in a1/a2 on the N64): every cinema resource block was left uninitialised on the host | `func_0029E460(data, prepare, size)` (matches); `proutSprintf` the same; `prepare` is `sptr` |
| s2-14 | `CAI__Update` callback | Rage Wars Source/Dest views | typed on `CGameObjectInstance`/`CAI` (matches) |
| s2-15 | `CAI__SetMode` | the same, plus the mode-table chain | host body on `CAI`/`CAIModeInfo` |
| s2-16 | `LevelEvent_Fire` and 11 other files | camera pool at `&multiplayer - 0x1201/0x1221`, actor stride 0x288, AI at +0x140, weapon actor +0x288/+0x3C8 | by name / `sizeof` / `T2_OFFSETOF` |

### For the merge

- Files others may touch: `port/app/{game.cpp,main.cpp,game.h}` (s1 adds flags there too: keep
  both sets of options; the mode log and `WriteShot` are mine), `include/structs/world.h`
  (`MtxPool`, `m_Heap`), `src/us/code/func_00413230.c` (menus: `MenuItem_DrawText` only),
  `func_0044C230.c` (`MenuPool_GetCamera_2`), the cache files `func_002041E4.c`,
  `func_0020548C.c`, `func_00205574.c`, `func_0020565C.c`, cinema `func_0040F240.c`/
  `func_00448240.c`, `func_00218590.c` (`Pack_Add`), the AI files `func_00243414.c`,
  `func_00245848.c`, `func_00266794.c`, and the 23 menu-pool files of s2-4 (s5's offset sweep may
  touch the same lines: they compute the same offsets).
- Rerun gendata with a fresh cache (`rm decls.json`): `world.h` and `D_800ACFC8`'s type changed.
- A fresh `turok2.cfg.json` makes LUS open 1024x768 and the game then draws into the lower-left
  640x480 of it (seen once with a new output directory): copy a 640x480 config or fix the
  default; screenshots assume 640x480.
- macOS (s3): s2-7, s2-8 and s2-11 are host-layout fixes that matter there too; several s32
  pointer arguments remain on this path (`CEngineApp__StartWarp(s32 app)`, `Action_EventGo`,
  `CHeap__Free(s32)` in `CCache__Main`, the cinema path items' `ref`).

### Next

1. The attract demo (mode 13): `EnemyAI_InitDispatch` and the enemy mode tables (N64 views of
   `CAI`), then the level renderer for the demo and the cinemas (they draw no geometry: follow
   `CScene__Draw`/the actor draw on level 162, whose acclaim-logo actor should show).
2. Merge s1 (PIF/pak) and drop `--skip-pak-check` from the default runs.
3. The rest of the Rage Wars views on the AI path (`AI_UpdateRegionFlags`, `CAI__UpdateModeAnim`,
   `EnemyAI_*`): s5's sweep, by the crash order above.

## Round 2 (history)


### How far `turok2 --game` got after round 2

```
t2: boot: arena 32 MB at 0x40200000 (low memory), ROM baserom.us.z64, mainproc started (pri 13)
game: booted, 120 frames
t2: Sound__Initialize: audio off on the host (sound bank not converted yet)
Thread 5 "turok2" received signal SIGSEGV
#0 CISet__GetBlock (arg0=0x0, arg1=0)                         func_002017D4.c:6
#1 CScene__GetRegionInfo (arg0=&gScene, arg1=0x3e32b8c400000000) func_0022425C.c:26
#2 CCamera__AdvanceAttributes (cam=gEngineApp_m_Game_m_CameraPool_m_MainCamera+76) func_0027CAB0.c:73
#3 CCamera__Advance                                            func_0027D160.c:102
#4 CCameraPool__Construct (pool=gEngineApp_m_Game_m_CameraPool_m_ActiveList_m_pHead+4) func_0042FE4C.c:30
#5 CEngineApp__Main (e=&gEngineApp)                            func_00286674.c:81
#6 mainproc                                                    func_00288860.c:13
other threads: CLoader__Main blocked in t2_osRecvMesg (as on hardware), the cache and
decompressor threads created and waiting; platform thread in t2os_run_until_idle
```

What now runs, in order: boot -> `CEngineApp__Main` -> `InitializeSystemResources` (CHeap on
the arena, decompressor and loader threads, `CCache__Construct` with its pools, hash table and
thread) -> `GfxPool_Alloc` (frame buffers, depth buffer, display lists, RSP FIFO, audio heap)
-> `CEngineApp__Construct` (the engine object, the synthesizer up to `amCreateAudioMgr`; the
sound bank is skipped) -> `CCameraPool__Construct` -> the main camera's first `Advance`, where
it stops. Mode: none yet (`CEngineApp__SetMode(0)` comes after the pools); 0 frames drawn,
so `--draw` (q4's GBI is in) has nothing to show yet.

### Crash log (round 2)

| # | where | cause | fix |
|---|---|---|---|
| 1 | `CHashTable__Construct` (func_00202860) | CHeap hard-coded the N64 block header (0x20; host `CHeapBlock` is 48): free list wrong from the first block; `HashEntry` 16 bytes on the N64, 32 on the host | header as `sizeof(CHeapBlock)` in CHeap Construct/Alloc/AllocTop/AllocAt, addresses through `uptr` (Free too); hash table by `sizeof(HashEntry)` (Construct, Insert). asm_diff 0 |
| 2 | `CHashTable__Construct` again | heap corrupted by `CCache__Construct`: the free stack sized `n * 4` holds 8-byte pointers; pools by N64 strides 0x28/0x2C; CList link offsets 0x18/0x1C/0x20/0x24/0x28 | pools by `sizeof(CMemEntry)`/`sizeof(CCacheEntry)`, stack by `sizeof(CMemEntry *)`, offsets by `T2_OFFSETOF` (new in common.h); `CCacheEntry` gains its `last` link at 0x28 (size 0x2C). asm_diff 0, check_structs 0 wrong |
| 3 | `GfxPool_Alloc` (func_00425220) | frame/depth buffers claimed at fixed RDRAM addresses (`GetMemSize() \| 0x80000000` - 1 MB): outside the host arena, `CCache__AllocateAt` returns NULL | `#ifdef TARGET_PORT`: `T2Port_RdramTop()` (t2boot.c: arena end + 512 KB, the console's geometry) |
| 4 | `alFilterNew` from `alSynAllocFX` (func_00297420) | m2c view: `s32 records` at 0x34 + `bus * 0x4C + 0x20` | rewritten as libultra's synallocfx.c with `ALSynth`/`ALAuxBus` (matches) |
| 5 | `alMainBusParam` (func_002964A0) | source list as `s32 *` | typed as libultra's mainbus.c (matches) |
| 6 | `alAuxBusParam` (func_00294E3C) | the same | typed as auxbus.c (matches) |
| 7 | `CAudioWorld__Initialize` (func_00270240:95) | the sound bank is a `VM_Alloc` copy of ROM 0x1E0000: big-endian, N64 layout, pointers that are VM addresses | **not fixed**: `Sound__Initialize` replaced by an empty host function (link.cmake, t2boot.c). Needs the bank converted (T2Asset + swap spec) |
| 8 | `CCameraPool__Construct` (func_0042FE4C) | local m2c struct with pads at N64 offsets, `memset(0x11F0)` | typed with `CCameraPool` (structs/camera.h), list links at 0 and `sizeof(void *)` (matches) |
| 9 | `CScene__GetRegionInfo` via `CCamera__AdvanceAttributes` | `cam->m_ViewParams.m_pCurrentRegion` is garbage: `CEngineApp`/`CGame` keep the camera pool, the scene and other parts as **byte arrays of N64 size** (structs/game.h), so on the host the typed `CCameraPool` (larger) overruns its slot, and the alias symbols (`gEngineApp_m_Game_m_CameraPool`, ...) disagree with `e->m_Game.m_CameraPool` (pool at `...m_ActiveList_m_pHead+4`, not even 8-aligned) | **open**, next round (below) |

### What round 3 was given to do

1. **`gEngineApp` host layout.** `CEngineApp` and `CGame` (structs/game.h) hold `CScene`,
   `CCameraPool`, the menu pool and more as `u8[N64 size]`, and `CGameFromCameraPool` views the
   same memory from the camera pool. On the host those members must be the real types (under
   `TARGET_PORT` at least), and gendata's aliases (`gEngineApp_m_Game_m_CameraPool`,
   `gEngineApp_m_Settings*`, `gScene_*`) then follow, since it computes their host offsets through
   the declared type. This is the next crash and blocks everything after the camera pool.
2. **Audio bank**: convert the sound bank (ALBankFile at ROM 0x1E0000) for the host and restore
   `Sound__Initialize`; `alAuxBusNew` points its handler at `func_00294D54 + 0xC` (asm in the
   middle of a word): a host `alAuxBusPull` is needed before any audio frame runs.
3. The ROM data the loader DMAs is big-endian: `ProPackDecompress` tests `'RNC'` as a native
   word, so on the host it will not see RNC streams; route the loader/decompressor through
   T2Asset (assets/PORT.md §10.4) instead of raw PI DMA.
4. q2's known gaps: the 403 unresolved pointer slots in 38 runtime structs (mark them .bss),
   the `*ModeTable` integers that hold function addresses (type them, rerun gendata).
5. Remaining 64-bit work (64BIT.md part 2): 1,064 mismatched declarations, ~3,000
   pointer-width warnings; each fix above was found by running, the audit lists the rest.

## Round 3, part r2: the loader through the archive, assets in host byte order

Details: [../assets/PORT.md](../assets/PORT.md) 10.4 and 10.6, [../assets/ARCHIVE.md](../assets/ARCHIVE.md)
(`t2/swap`, format 2.0), [../assets/BYTEORDER.md](../assets/BYTEORDER.md).

- **Archive format 2.0** (`tools/assets/mkarchive.py`): new entry `t2/swap`, one byte-swap plan per
  unit, computed from `swap_spec.json` (`swapcheck.unit_plan`). Rebuild the archive: the port
  (`T2Asset_OpenLus`) refuses 1.x. Deterministic (sha1 `99cbe767...` with zlib 1.3.1, any `-j`).
  `archive_check.py` recomputes and checks the plans.
- **Reader** (`port/assets/t2asset.c`): `T2Asset_SwapUnit` applies the plan after every backend
  load (whole unit, then the prefix), so `T2Asset_Read` / `T2Asset_ReadInto` return host-order
  fields on a little-endian host; VM-window residents are swapped once; the wave table stays raw.
- **Engine** (`#ifdef TARGET_PORT`, asm_diff 0 over the 9 changed files): `CLoader__LoadSync`,
  `CLoader__LoadAsync` (one entry per request), `CLoader__Main` (no PI DMA) read through
  `T2Port_ReadAsset` (port/runtime/t2loader.c); `CCache__Request` sizes with
  `T2Port_AssetHostSize`; `VM_Alloc` returns the whole unit in host order; `ProPackDecompress`
  keeps only the plain-data branch (no 'RNC' path can be taken on the host).
- **Cache on the host** (found while checking the path in the game): `CCache__Construct` wrote
  through `D_800D7BE0`, which gendata places inside `gDecompressor`, so `gCache`'s free stack
  stayed NULL and the first `CCache__AllocMemEntry` crashed; now typed on `gCache`. Also the free
  stack by index (func_00203068, func_002044A8) and a typed `CCache__AllocDecompressBlock`
  (func_00205D7C) for `ProPackDecompress`.

Tests: `t2asset_test` (26,124 units = ROM bytes through the plans, double swap, probes, 20,000
audio reads, ROM backend + plans: 0 errors), `t2asset_typed_test` (1,585 field checks on level
headers, geometry incl. transposed vertices, texture banks against the area tools' big-endian
parsers: 0 errors), `turok2` self test (LUS = miniz = ROM+plans), `archive_check.py` OK.

How far `turok2 --game` gets: unchanged (`CCameraPool__Construct` -> ... -> `CISet__GetBlock(NULL)`,
r1's crash); no asset read happens before it. In gdb at `GfxPool_Alloc`, `CCache__Request` (direct,
RNC unit, `ProPack__Decompress` callback, 4-byte probe) returns correct host-order bytes.

Open, found on the way (not fixed here):
- `gPoolBuffers` is 0x20 bytes of .bss in port/gen (untyped), the host `GfxPool` is 64:
  `GfxPool_Alloc` writes `pool->gfx`/`vtx` over `gCFB[1]` (becomes a heap pointer), and
  `CEngineApp__Main` then memsets 150 KB there, over the cache tables. Needs a typed
  declaration so gendata sizes it.
- Other m2c views at N64 offsets on the cache path (`CCache__AllocateMemoryForDecompressionTop`,
  B12Mgr users) remain; `CHeap__Free`/`CCache__HeapAlloc` callers that pass pointers as `s32` work
  only in low-memory mode (not on macOS).
- `Texture_DecompressCallback` recolours RGBA5551 palettes with native u16 loads: needs
  big-endian accessors now that units arrive host-order but texels/TLUTs stay big-endian.
- The 0.5 % `unk` bytes of BYTEORDER.md arrive in ROM order (collision regions, particles, swooshes).

## Round 3, part r1: the boot runner (to the first frames of mode 0)

Branch `names-R3_r1` (merges r2's `names-R3_r2` at 691aad0a for the loader). Every commit:
`tools/asm_diff.py --base 3798e01e -j 8` = **3938 files, 0 differ**; `check_structs.py` on
game.h, camera.h, world.h, frame.h, sched.h: 0 wrong.

### Where it was at the end of r1

```
SDL_VIDEODRIVER=offscreen T2_SHOT=out.ppm build/app/turok2 --archive <r2's format-2 turok2.o2r> \
    --rom baserom.us.z64 --game --draw --frames 300
game: 300 frames in 2.66 s, 1404 thread switches, 151 gfx tasks (151 drawn), 0 audio tasks,
      0 PI DMAs, 0 missing-function calls
OK (0 failed checks)
```

Boot -> `CEngineApp__Construct` (scheduler thread up) -> camera pool, player pool, `CScene__Construct`
(cartridge, levels, texture sets, object types, objects indexes) -> `CScene__LoadLevel(0)` (level
0 geometry, collision, instances) -> the message loop -> mode 17 switches to **mode 0 (pak check)**:
`EngineMode00_PakCheck_Init` loads level 0 again, every retrace runs `CEngineApp__Update` ->
`CEngineApp__Draw` (`CScene__Update`, the camera pool draw, the menus) ->
`CEngineApp__SubmitGfxTask` -> the scheduler -> `osSpTaskStartGo` -> Fast3D. At frame 120 mode 0's
timer opens the pak menu (`gPakSaveGameAnyPakMenuDesc`, then `gDamagedPakAtBootMenuDesc`: no
Controller Pak is modelled) and its windows are drawn. 300 frames run to the end without a crash;
**151 display lists drawn by Fast3D** (2 retraces per frame, as on hardware).

The picture (frame 300, `T2_SHOT`): black, with the menu's frame rectangles but no text: the
window widths come out ~0 (a 4-pixel red bar at the left), so the text measuring/drawing (fonts:
`COnScreen` font loads, `Menu_TextWidth`, the HUD text path) is the next thing to follow. The game
renders at 480x360 (`gDPSetColorImage` width 480; `Video_SetMode(480, 360, ...)`). `T2_DLDUMP=1`
prints the last frame's display list (166 commands: init lists, scissor, colour/depth image, fill
rectangles, prim colours; no texture or vertex commands yet).

### What was done (crash table, round 3)

| # | where | cause | fix |
|---|---|---|---|
| 1 | `CScene__GetRegionInfo` via `CCamera__AdvanceAttributes` | `CEngineApp`/`CGame` held the scene, camera pool, onscreen as N64-sized bytes | `structs/game.h` under TARGET_PORT: `CScene m_Scene[1]`, `COnScreen unk19440[1]`, `union { CCameraPool m_CameraPool[1]; struct { pad; u8 m_MenuPool[0x18]; }; }` (one-element arrays keep `x.m_CameraPool` a pointer); mips `_Static_assert`s keep the N64 offsets (gendata parses mips too). 8 files with literal CEngineApp/CGame offsets (0x3C8, 0x22C00, 0x23DF0, 0x1268, 0x1228) by `T2_OFFSETOF` |
| 2 | same | gendata translated offsets inside a union through its *last* member (the byte pad) | `_dtranslate`: a union prefers the member that types the offset |
| 3 | `CCamera__AdvanceAttributes` | `CCameraViewParams__Construct` wrote through an ObjInit view (4-byte region pointer) | host body on `CCameraViewParams` |
| 4 | `Math_AcosLookup(NaN)` | libultra's `du` (`{ struct { u32 hi, lo; } w; f64 d; }`) generated by its words: doubles with swapped halves | gendata: a union's whole-size scalar member lays out the data |
| 5 | `CHeap__Free(NULL)` from `CCache__Flush` | `gPoolBuffers` (`extern u8 gPoolBuffers[]`, a GfxPool of pointers) generated at its N64 0x20 bytes: `GfxPool_Alloc` overwrote `gCFB`, then the frame-buffer memset hit the cache tables | gendata: a .bss symbol the C only declares as bytes gets twice its N64 size (409 symbols); `CCache__AllocateAt` through `CMemEntry` (its B12Node view put `flags` in `lockCount`); cache free stack by name (= r2's 6abc08a0) |
| 6 | `CPlayer__LevelStart` | player list links `0x131C/0x1320`, `PlayerList` view with N64 list offsets | `T2_OFFSETOF(CPlayer, m_pLast/m_pNext)`, host `PlayerList` (both pool files) |
| 7 | `CLoader__AllocLoaderEntry` | loader lists `0x18/0x1C` | merged r2 (loader through T2Asset) + links by `T2_OFFSETOF` |
| 8 | `CCache__DetachCacheEntry`, `CCache__FreeCacheEntry` | m2c offsets 0x14/8/0x10, list 0x14 before | typed bodies |
| 9 | `CCache__LoadPersistantIndex` (levels index) | `CScene__Construct`'s `s32 cart` received an 8-byte pointer, clobbering `file` | `sptr cart` |
| 10 | `Model_Decompress` | `CCache__AllocateMemoryForDecompression(Top)` wrote CMemEntry at N64 offsets | both through r2's typed `CCache__AllocDecompressBlock` (now `..Flags`) |
| 11 | `SwooshSystem_UpdateSwooshes` | the swoosh/effect system (0x9598 bytes in CEngineApp) is N64 bytes with 4-byte links | **stub**: `func_0042D410` leaves it empty on the host (no swooshes/effect slots). Needs typing |
| 12 | `CScene__DecompressCollision`, `CScene__LoadLightSettings`, `CScene__GetIntelligence`, `CScene__GetRegion` | local CScene views at N64 offsets; region stride 0x20 | views by `__builtin_offsetof(CScene, ...)`; host `CGameRegion` stride; `CGameRegion__Unpack` typed |
| 13 | `CGameObjectInstance__SetupAnimation` | Actor view at N64 offsets (called a data address) | typed on `CGameObjectInstance` |
| 14 | `__scMain(sc=NULL)` | the scheduler's views (Panel, Sched, Vid, ...) overran each other on LP64 | **`port/game/t2sched.c`**: the 17 scheduler functions typed on `T2Sched`/`T2ScTask` (`structs/sched.h`), the originals left out of the link (`link.cmake`); `CFrameData.m_Task/m_LineTask` typed; `CEngineApp__SubmitGfxTask`, `CEngineApp__AdvanceFrameData`, `Video_SetMode` typed; `gRefreshRate` (`&gCurrentFB + 1`) is a variable of its own; `__amMain`'s client is two pointers |
| 15 | `SndVoicePool_*` from `Sound__StopAllExcept` | no sound bank (audio off) | `Sound__*` return early while `T2Port_AudioReady()` is false (`port/game/t2audio.c`: `gAudioWorld.bank != NULL`), plus a guard in `CAudioWorld__StopAllSounds` |
| 16 | `LevelEvent_Fire(NULL)` | the delayed level events at CScene+0x1180 (CLists + 0x30 nodes) as bytes | `CDelayedEvent` + typed fields in `CScene` (TARGET_PORT); the 4 users typed |
| 17 | `CPlayerPool__RegisterActors`, `CScene__BuildActorLists` | m2c offsets (0x54, 0x2DC, 0x8EC...), settings-relative 0x1270/0x1210, camera 0x2D8 | by field |
| 18 | `CCameraPool__AdvanceText` | camera text/shake/camera/emitter lists `0, 4`; texts block 0x400; `m_SkySystem` was `u8[0x8D0]` but holds a `CEmitterPool` of pointers | links by `T2_OFFSETOF`, `16 * sizeof(CCameraText)`, `CEmitterPool m_SkySystem[1]` (camera.h, TARGET_PORT) |
| 19 | Fast3D `G_MOVEMEM` with data 0 | `CEngineApp__BeginDisplayList` wrote Gfx through junk_types' `DlCmd {u32 w0, w1}` | `DlCmd` is two host words under TARGET_PORT |
| 20 | Fast3D `G_MOVEMEM` at 0x800AB0D8 | static display lists (gRspInitDL...) hold N64 addresses in w1 | gendata: Gfx data (`GfxWord` pairs) get host pointers in w1 for G_VTX/MTX/MOVEMEM/DL/RDPHALF_1/SETxIMG and G_MOVEWORD segment (10 lists) |
| 21 | `Menu_Construct`, `Menu_Update`, `Menu_TextWidth` | menu windows/items/descs/model blocks by N64 sizes (0x1D8/0x28/0x24/0x330), colours by offset, pool links 0x1D0/0x1D4; desc item tables generated as one element; desc `data` (s32) holding N64 text addresses | macros `MENU_SIZE`... (func_00413230.c); gendata types a pointer target as a table (the count stored next to the pointer, e.g. MenuDesc.items/count; never over a C-declared symbol; 88 tables); `Menu_Construct` maps an N64 address in `data` with `t2_n64_to_host` |

### For the merge

- Files others touch: `src/us/code/func_00203068.c`, `func_002044A8.c` (identical to r2's; this
  branch takes r2's version), `func_00205D7C.c` (r2's typed allocation renamed
  `CCache__AllocDecompressFlags` + wrapper), CList literal offsets in `func_0042E5AC.c`,
  `func_0042E834.c`, `func_00201CC0.c`, `func_0042F9B0.c`, `func_0042FBA4.c`, `func_0042FFF0.c`,
  `func_0042B3E4.c`, `func_00413230.c` (r3's CList sweep does the same lines: take either, they
  compute the same offsets), `include/rw/junk_types.h` (DlCmd), `include/structs/{game,camera,
  world,frame,sched}.h`, `include/globals/framebuffer.h`, `tools/port/gendata.py` (unions, bss
  slack, Gfx relocations, pointer tables), `port/cmake/{game,link}.cmake` (port/game/*.c and the
  scheduler files), `port/app/game.cpp` (T2_SHOT, T2_DLDUMP), 24 `Sound__*` files (guard).
- Run with r2's archive (format 2.0, t2/swap): `/root/R3_r2/out/turok2.o2r` or rebuild it with
  r2's mkarchive. The C17 archive (format 1) no longer loads with r2's reader.
- After changing a header, rerun gendata with a fresh cache (`rebuild.sh --fresh`): the aliases
  and sizes come from the types.
- Several fixes truncate pointers through `s32` prototypes on purpose (low-memory Linux):
  `CCache__AllocateMemoryForDecompression`, `CScene__GetRegion`, `gDisplayList0/1` (s32), `CFrameData.m_pFrameBuffer`
  (u32). r5 (macOS, no low memory) will hit those; they are r3's prototype typing.

### Next

1. Fonts and text: why the menu's text measures 0 and draws nothing (COnScreen fonts, the HUD text
   path, texture loads: the first frame with textures). Then the pak menu with input (PIF/SI:
   `__osSiRawStartDma` is not modelled, so the game sees a damaged pak).
2. The swoosh/effect system (CEngineApp 0x19608-0x22BA0) typed, instead of left empty.
3. Audio (r4): the `Sound__*` guard goes away by itself once `gAudioWorld.bank` is set.
4. The remaining m2c views on the draw path (`T2_DLDUMP` shows what the frame lacks).

## Round 3, r3: 64-bit part 2 (the typing sweep)

Branch `names-R3_r3`. Every commit has `tools/asm_diff.py --base 3798e01e` at
`3938 files: 0 differ or fail`, `check_structs.py structs/game_object.h` at 0 wrong, and
`turok2_game` / `turok2_data` building with 0 errors. Details and numbers are in 64BIT.md, "Part 2".

- `tools/port/retype.py` (new) does the mechanical work at the columns clang reports.
  It turns `(s32)ptr` into `(sptr)`, retypes locals that carry pointers, drops casts in
  front of pointer parameters, and retypes the `s32` parameters and returns of functions
  that callers fill with pointers (rule P, every declaration). 880 files changed.
- Truncating pointer-width warnings went from 2,828 to 1,048 (clang, x86_64). The
  disagreeing declarations went from 1,059 to 999.
- The GfxPool path is pointer-wide: `CCache__AllocateAt`, `B12Node.key`, `MemHandle.addr`,
  and `gDepthBuffer`/`gDisplayList*`/`gLineList*`/`gAudioHeapBase` are now `sptr`. r5 saw the
  macOS run pass `GfxPool_Alloc`.
- All `CList__Construct` link offsets are now `T2_OFFSETOF`/`sizeof(void *)`.
  `MenuPool_Construct` is typed, and `CInstancePool__Construct` has a typed host body. r5
  saw the macOS run pass `MenuPool_Construct`. It next stops in
  `CCamera__ConstructSkySystem(+0x558)`, which is r1's area.
- `CAIModeInfo`: `m_Mode` (the header row's next-table link), `unk_04` (enter) and `unk_10`
  (anim id or function) are now `sptr`. gendata relocates a pointer-wide integer whose value
  is a known address. 27 runtime structs inside `.libultra` are now `RUNTIME_BSS`.
  Unresolved pointer slots went from 403 to 24, and address-holding integers from 334 to 125.
  All `*ModeTable`s are now relocated.

Merging: `include/globals/frame.h`, `include/rw/junk_types.h`,
`include/structs/game_object.h` (CAIModeInfo only, not CEngineApp/CGame), several
`include/protos/*.h` and about 880 files in `src/us/code` changed in one-token edits.
Conflicts with r1/r5 should be small and local. Rerun gendata with a fresh `--cache`,
because global and struct sizes changed.

## r5 (round 3): macOS arm64

Details: [MACOS.md](MACOS.md). On the Mac (M5, Apple clang 21, Homebrew; no new packages) the
whole port builds natively: `turok2_game` (3,917 files, 0 errors), gendata (identical output
to the box), `turok2` on LUS 2.0. Runtime and asset tests give the same hashes as Linux.
`turok2 --game` (headless) stops in `GfxPool_Alloc` (func_00425220.c:86) on a truncated `s32`
address. Linux `--high-memory` stops at the same line, and macOS cannot have a low-memory mode
(checked). A probe branch (`r5-probe`, not for merge) fixes five truncations and reaches
`CCameraPool__Construct` → `CCamera__ConstructSkySystem` (`arg0 + 0x558`), the CCamera layout
work. It also found a host-layout bug that affects Linux too: `CCache__AllocateAt` writes a
`CMemEntry` through junk_types' `B12Node` view, so `flags |= 3` lands in `lockCount`.

## Round 4, part s3: macOS parity (mode 0, then legal and the first cinema)

Details and the crash table (s3-1 to s3-19): [MACOS.md](MACOS.md) section 6. Branch
`names-R4_s3` in the Mac clone, 21 commits on 474e40af. asm_diff 0 over all 3,938 files,
checked on the box after every change.

- **macOS now runs mode 0 headless for 300 frames**: 1,414 thread switches and 151 gfx tasks,
  as on Linux. The display lists of frames 101 and 301 are command-for-command identical to
  Linux's (headless on both; only host addresses differ).
- With a local end-the-pak-check probe (not committed), the Mac goes through mode 1 (legal),
  mode 3 (expansion pak) and into mode 4 (intro cinema, level 162). It stops in the AI action
  callbacks that take the actor as `s32` (`Action_IdleEnter` -> `CScene__SetInstanceSavedBit`),
  and in `CAI__SetMode`'s N64-offset views.
- **Fixes that matter on Linux too:**
  - s3-13: the damaged-pak menu lines now draw (this is section 4 item 1).
    `MenuItem_DrawText` read `Menu.desc`, which on the host is the item's desc pointer.
  - s3-19: `CFrameData.m_pFrameBuffer` is `uptr`. Every `G_SETCIMG` used to carry a truncated
    address, and `t2sched.c`'s `frameOf` compared 32 bits.
  - s3-16: `cam + 0x53C` -> `cam->m_MenuPool` (29 sites).
  - s3-18: `CScene__LoadLevel` steps over actors by `sizeof(CGameObjectInstance)`.
- New: `T2_DLDUMP_AT=<frame>` (game.cpp) dumps a frame's display list without a window. macOS
  has no headless Metal/GL, so the Mac compares display lists, not pixels.

For the merge (files others may touch): `src/us/code/func_00413230.c` (menus: MenuPool_Open*,
MenuItem_DrawText/Layout; s2), `func_0044C230.c`, `protos/menu.h`, the 19 `+ 0x53C` files
(s5's offset sweep), `func_002718A0.c`/`func_00275A08.c` (audio, s4), `func_004263DC.c`
(pak directory, s1), `structs/world.h` (MtxPool), `structs/frame.h`, `port/game/t2sched.c`,
`port/app/game.cpp`, the 28 `EngineModeNN_*` files (s2's mode runner). `include/` changed, so
the whole tree must be rerun through asm_diff after the merge.

## Round 3, part r4: audio on the host

Details: [AUDIO.md](AUDIO.md). Behind `--audio` (off by default: runs without it are unchanged).

- Sound bank (ROM 0x1E0000) rebuilt as host libaudio structs (2 bank files, 34 instruments,
  1,115 sounds, 866 wave tables), sound data file swapped to host order (or left alone when it
  already is: r2's archive path), host audio manager (one 368-sample frame per retrace), an ABI1
  interpreter for all 16 commands, AI queue -> LUS `AudioPlayerPlayFrame` (game.cpp).
- `t2audio_test --direct K` + `tools/port/audio_compare.py`: a sound effect through
  LOADADPCM/LOADBUFF/ADPCM/RESAMPLE/ENVMIXER/INTERLEAVE/SAVEBUFF equals audio.py's VADPCM decode
  with **0** difference (sfx 0: 6,704 samples, sfx 40: 1,088).
- Game path: `Sound__Initialize` (`CAudioWorld__Initialize`) completes on the host; the first
  audio frame spins in `_sndpVoiceHandler` because `gAudioWorld.player.evtq` is corrupted
  (open, AUDIO.md §4). `turok2 --game --audio` stops at the same `CCameraPool__Construct` crash
  as without `--audio`.

| # | where | cause | fix |
|---|---|---|---|
| A1 | `alCSPNew` from `SndSeq_Construct` | `AudioStateD844`/`AudioRootD844` views: heap at N64 offset 0x1DA8 | `SndSeq_Construct` typed with `SndSeq`/`CAudioWorld` (`TARGET_PORT`), `SndSeq.csp` as `sptr`, `alCSPNew` sizes by `sizeof` |
| A2 | (heap) | `alHeapInit`/`alHeapDBAlloc` 32-bit views of `ALHeap` | host layout, `uptr` (`TARGET_PORT`) |
| A3 | `alSndpAllocate` | `PoolRecAt44`/`SlotRecAt2F` views | the 10 sound player API calls typed in port/audio/t2audio_sndp.c (files out of the link) |
| A4 | `alEvtqPostEvent` | `alEvtqNew`/`alEvtqNextEvent` 0x1C strides, s32 links | typed (`TARGET_PORT`) |
| A5 | `osRecvMesg` from `CAudioWorld__Lock` | lock at `arg0 + 0x110` | `&arg0->lock` (matches) |
| A6 | `_sndpVoiceHandler` spins | `gAudioWorld.player.evtq` overwritten with stack-like data | **open** (AUDIO.md §4) |

## Round 4, part s4: audio part 2 (the sound world typed, --audio through mode 0, --wav)

`turok2 --game --audio --frames 300` now runs mode 0 to the end (300 frames, no crash; it used
to stop in `SndQueue_CancelHandle`). The synthesizer mixes a frame every retrace; mode 0 plays
nothing itself (the volume options are still 0 until the settings are loaded), so its output is
silent. Through the game's own API a sound effect plays: with `T2_TEST_SFX=300` (debug only,
`Sound__Play2D(300)` at frame 30, volume options forced to 0xC0) the WAV has the sound
(peak 1037, rms 88 over 5 s, 197,534 non-zero samples) under gdb. Details: AUDIO.md section 8.

```sh
# the mixed output as a WAV (implies --audio) and its summary line
SDL_VIDEODRIVER=offscreen OUT/build/app/turok2 --archive OUT/turok2.o2r --rom baserom.us.z64 \
    --game --wav OUT/m0.wav --frames 300
# game: wav OUT/m0.wav: 109664 stereo frames at 22050 Hz (4.97 s), peak 0, rms 0.0, 0 non-zero samples
T2_TEST_SFX=300 ... --wav OUT/sfx.wav     # one sound effect through Sound__Play2D (debug hook)
T2_TEST_SONG=0x10 ...                     # music through Music_SetTrack: stops in the CSP (below)
```

| # | where | cause | fix |
|---|---|---|---|
| s4-1 | `SndQueue_CancelHandle` from `CAudioWorld__StopHandle` | the sound world's C is Rage Wars' (ContextRecAt2BC0, Voice, M04Emitter, NodeRecAtC8, ... views at N64 offsets, 0xCC voices) | 81 functions (CAudioWorld, SndVoice, SndVoicePool, SndQueue, SndSeq, the speech player, LoopSounds, Sound__IsPlaying) typed in `port/audio/t2audio_world.c`, originals out of the link |
| s4-2 | r4's `_sndpVoiceHandler` evtq corruption | `SpeechPlayer_Construct` allocated its sound on `CAudioWorld__GetSndPlayer(m)` = m + 0x84 (the N64 offset of the sound player: the bank pointers on the host) | typed (s4-1) |
| s4-3 | `alAudioFrame` spins | `__nextSampleTime` read the client's time at 0x10 (`ALPlayer.handler` on the host); `alSynAddPlayer`, `_timeToSamples` the same | `port/audio/t2audio_lib.c`: the synthesizer driver, voice calls (`alSynAllocVoice` helpers, `alSynSet*`, `alSynStart/StopVoice`, `alSynFreeVoice`), `alEvtqFlush*`, `_removeEvents`, `alCSPGetState`, then (s4-4..s4-7) the filter constructors, `alResampleParam`, the CSP API posts and the codebook cache (30 functions) |
| s4-4 | `alAudioFrame`: `output->setParam` = 3 | `alFilterNew` stored handler/setParam as s32 at N64 offsets; `alSaveNew`/`alSaveParam` the same | typed in t2audio_lib.c; `ALSave.dramout` is `sptr` |
| s4-5 | SAVEBUFF of 11,168 bytes over the save filter | `alFxPull`/`_doModFunc`: `&r->input[-d->input]` with a u32 delay wraps only with 32-bit pointers | `-(s32)` under `TARGET_PORT` (func_00296750.c, func_00296CD0.c) |
| s4-6 | `alAdpcmPull` -> `alResampleParam` calls a handler as setParam | `alResampleParam` (IntegerState34 view) | typed |
| s4-7 | `AudioDMA_CacheBlock`: heap NULL, entries 0x90 bytes | `gAudioHeap` set by the replaced `amCreateAudioMgr`; CacheEntry holds pointers | t2audio_mgr.c sets `gAudioHeap`; typed host cache + ageing (`T2Audio_DmaCacheAge`) |
| s4-8 | `ALParam` too small for `ALStartParamAlt` | the update casts carry a pointer at 0x20 on the host | `ALParam` gets a host pad (`TARGET_PORT`) |
| s4-9 | (silent) pitch/env halves | `FloatToHalf` took the first u16 of the float: the low half on a little-endian host | `TARGET_PORT`: the top 16 bits |
| s4-10 | (silent) `LoopSounds` paused flag past the struct | `LoopSounds` had no field at 0x28 | `m_Paused` added (structs/sound.h); `SndSource.flags` and `SndSeq.unkC` are `sptr` (pointers) |

Open (next): (1) without gdb, a playing sound effect crashes in `t2asp.c` `cmd_adpcm`
(`memcpy(ram(w1), ...)`: the ADPCM state address in the command; under gdb, no ASLR, it plays:
an address above 4 GB or a stale state pointer, look at `alAdpcmPull`/`_decodeChunk`'s
`osVirtualToPhysical` arguments first); (2) music: `Music_SetTrack` -> `SndSeq_Update` (typed)
-> `__CSPVoiceHandler` -> `__CSPSetUsptFromTempo` (func_00290D5C) crashes: the compressed
sequence player (`__CSP*`, `alCSeq*`, func_0028F1F0..func_00290D5C) is still m2c views;
(3) speech (`SpeechPlayer_Update` state 1, the adec stream) is stubbed: it ends the request and
says so once.

For the merge: new files `port/audio/t2audio_world.c`, `port/audio/t2audio_lib.c`; edits in
`port/cmake/link.cmake` (112 files added to `T2_LINK_REPLACED`), `port/app/CMakeLists.txt`
(t2audio sources), `port/app/main.cpp` / `game.h` / `game.cpp` (`--wav`, the T2_TEST_* hooks:
s2 also adds a flag in main.cpp), `port/audio/t2audio_mgr.c`, `port/audio/t2audio_sndp.c`
(`alSndpSetPriority`), `include/structs/sound.h`, `include/structs/libaudio.h`, three src files.
Drop OUT/decls.json (header changes). asm_diff 0 (see the commit).

## Round 5, part t4: audio part 3 (sfx without gdb, music, speech)

Details, crash table t4-1..6 and verification commands: [AUDIO.md](AUDIO.md) sections 9-11.

`turok2 --game --draw --pak-blank --audio --wav boot.wav --frames 5400` runs the boot to the
title screen (mode 7 at frame 4214, as without `--audio`) with sound: the first cinema from 26 s,
music song 22 over the second cinema, song 16 (the title theme) from 70 s; WAV 90 s, peak
20,820, rms 2,088, no clipping. Every note on of songs 22 and 16 matches the ROM sequence in
time (0 us error), key and channel (`tools/port/audio_notes_check.py`, 510 and 251 notes); the
ADPCM voice path is bit-exact against tools/assets/audio.py; speech streams decode with 0
difference against audio.py (`t2audio_test --adec`) and play in game (`T2_TEST_SPEECH=311`:
correlation 0.986 with the reference stream).

| # | fix |
|---|---|
| t4-1 | `osVirtualToPhysical` names any host address (region handle table, `T2Port_PhysToVirtual`); `_decodeChunk` used K0_TO_PHYS (`& 0x1FFFFFFF`) for the ADPCM state: the sfx crash without gdb |
| t4-2 | the compressed sequence player, cseq.c and the shared seqplayer.c helpers typed (port/audio/t2audio_csp.c, 31 functions, their game files out of the link) |
| t4-3 | **`gWarmBoot = 1` in `T2Port_Boot`**: `CEngineApp__Construct` runs `GameSettings_SetDefaults` only then (on the N64: power-on). Every setting was 0 before (music/sfx volume, difficulty 0 instead of 0x10, HUD alpha, multiplayer rules) |
| t4-4 | the resident wave data copy is the whole ROM (it was taken before `gAudioRomBase` was set: every wave decoded garbage) |
| t4-5 | `A_ADPCM` writes the previous frame before the new ones, as the microcode does |
| t4-6 | the audio start-up runs above 4 GB (`--high-memory`): heap/block/source pointers pointer-wide in `alHeapInit`, `alLoadNew`, `alResampleNew`, `alFxNew`, `alMainBusNew`, `alAuxBusNew` |
| t4-7 | speech: port/audio/t2audio_adec.c (the adec decoder from audio.py, `alAdecPull`), `SpeechPlayer_Update` as func_002768D0 with a host wave table |

For the merge: t4-3 changes the game state of every run, not only audio: the other parts' runs
(t1's demo and new game, t3's cinemas) start from the default settings after it. Files others
may touch: port/runtime/t2boot.c (one line and a comment), port/runtime/t2ultra.c
(`osVirtualToPhysical`), port/runtime/t2port.h, port/app/game.cpp (the `T2_TEST_SPEECH` hook),
port/app/CMakeLists.txt (two sources in t2audio), port/cmake/link.cmake (36 files added to the
exclusion list, in two blocks before the scheduler's). src/ (asm_diff 0 against 98574f3c):
func_0029FCF4.c, func_002912F0.c, func_00295304.c, func_0029546C.c, func_00294E70.c,
func_002953B4.c, func_00295410.c. `--high-memory` on Linux still stops in `CHeap__Free` from
the decompressor, with or without `--audio` (not audio).
