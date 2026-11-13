# Stability: soak runs, crash classes, the gendata size audit (Phase 7 rounds 9 (x5) and 10 (y5): section 7)

Branch names-R9_x5 (from 5437cfc0). Every `src/` change is `TARGET_PORT` or `s32` -> `sptr`
(`sptr` is `s32` on the N64): `tools/asm_diff.py --base 5437cfc0 -j 8` = 0 on every touched file.
No `include/` change, no archive change. gendata changed: **drop decls.json and rerun gendata**.

## 1. The soak harness: tools/port/soak.py

```sh
python3 tools/port/soak.py --exe OUT/build/app/turok2 --archive OUT/turok2.o2r \
    --rom baserom.us.z64 --out OUT/soak/n --kind newgame --seeds 0:40 --both-memory --frames 36000
#   --kind attract   no input: the boot loop and the three demos (use --frames 100000)
#   --kind newgame   a new game (START@4400 A@4500 A@4700 A@4800), then random but valid play from
#                    frame 13000: walk/strafe/turn/look (C buttons, stick), fire (Z taps and holds),
#                    jump (R), the weapon wheel (A/B held + a stick slot), START + D-pad menu moves
#                    (+ A/B), the map (L); T2_GIVE_WEAPONS=1 on even seeds
#   --kind warp      newgame + T2_WARP_TO with 1-3 random warp ids of warp_dests.json (levels.py),
#                    T2_GIVE_WEAPONS=1, T2_WAKE_ENEMIES=1500 (--warps OUT/warp_dests.json)
#   --both-memory    each seed twice: the default (high memory) and --low-memory ("-lm" runs)
#   --draw           --draw with SDL_VIDEODRIVER=offscreen vblank_mode=0 (about 140 VI/s)
```

Each run goes under `gdb -batch` (the LUS crash handler would otherwise eat the signal), headless
(`--no-window` unless `--draw`), with `T2_STATE_LOG=600`. A run directory keeps `cmd.sh` (the
exact command and env to repeat it outside gdb), `input.txt` and `run.log` (the log without LUS's
info lines, the crashing thread's backtrace). `OUT/summary.json` has every run; `OUT/crashes.txt`
one entry per crash class: the key is the signal and the top 4 named frames (raise/abort and
signal frames skipped), the runs that hit it, one backtrace. A hang (no exit in `--timeout`, 1800
s) kills the run's own session (gdb and the game) and reports `HANG`. Pools are capped at 4 jobs.
No-window runs are fast: 100,000 frames of the attract loop in 10 s, 36,000 frames of play in
5-8 s.

## 2. Totals (final binary, Linux headless)

| set | runs | frames | crashed | classes |
|---|---|---|---|---|
| attract, 100,000 frames, seeds 0-1, default + `--low-memory` | 4 | 399,840 | **0** | 0 |
| newgame, 36,000 frames, seeds 0-39, high + low memory (`soak5`, before S13) | 80 | 2,880,000 | **0** | 0 |
| newgame, 36,000 frames, seeds 40-79, default + `--low-memory` | 80 | 2,866,920 | 4 | 2 (open, both hosts' memory modes: section 4) |
| warp, 30,000 frames, seeds 0-39, high + low (`soak5`) | 80 | 2,170,552 | 22 | 7 (open: section 4) |

Plus: the boot loop with `--draw` (26,000 frames, default memory) and w2's level-52 kill run
(`--no-window`, 14,800, the corpses melt) exit 0. At the start of the round the same harness found
6 of 8 new games and both 100,000-frame attract loops crashing.

## 3. Crash classes found and fixed

| id | where | crash | cause | fix |
|---|---|---|---|---|
| S1 | `alCSPStop` via `SndSeq_Stop` (port/audio/t2audio_lib.c) | quit to title from the pause menu, any memory mode, no `--audio` | audio off: no sequence players; `csp_post(NULL)` | `csp_post` and `alCSPGetState` treat a NULL player as stopped |
| S2 | `CCameraPool__Advance` (func_0027ECF0.c) | attract loop at 27,029 (the second camera of a cinema) | `NEXT_CAMERA` read `m_pNext` at the N64 +4 (inside the 8-byte `m_pLast`) | `(c)->m_pNext` |
| S3 | `Cinema_Update`/`Cinema_RunEnd` call 0x100000000; `MenuPool_Draw` NULL | every cinema end (death cinema, level intros), random | **gendata**: `gCinemaPlayerP = gBlindOne + 0x560`, the cinema player is the 0x100 unlabelled N64 bytes after the BlindOne struct; the host gave gBlindOne only its struct size, so the cinema player (larger on the host) lay over sMenuBindingText/sMenuBindingValue/... and the menu code wrote over its callbacks | `grow_bss_tails` (section 5) |
| S4 | `Menu_Draw` via `PauseMenu_Update` | `--high-memory`, the pause menu | the menu and its pool as `s32` | `sptr` |
| S5 | `MenuPool_Draw` calls NULL | the cheats menu (`CheatsMenu_WarpSelect` -> `Menu_ClaimPressingController`) | `Menu::controller` written at the N64 0x20: on the host that is `Menu::desc`, the window's descriptor became `&gControllers` | the view's padding is `offsetof(Menu, controller)` (func_00417ACC.c and its twin func_00450ACC.c) |
| S6 | `CBarrel__ShouldClose` (func_00254F24.c) | the weapon wheel closing | N64 offsets: `m_bStickWheel` 0x380, `m_nCursor` 0x37C, the player's controller 0xA50 and its buttons 0xB0 (`CBarrelItem` holds a pointer) | host body on the members |
| S7 | `EnemyMode_InteractiveIdle_Update` with obj NULL / garbage ai; `Actor_TurnBy(NULL)` | a woken enemy in idle-interactive mode | `AI_UpdateTargets` fills a host `CAITargets` (pointers: larger than 0x88) into 0x88-byte locals: it overwrote the caller's saved obj/ai | the locals at `sizeof(CAITargets)` / views by `offsetof` (func_0025F068.c, func_00403144.c, func_004032E4.c, func_00446C38.c, func_0040DC38.c (+ its CAI timers at N64 0x40/0x64), func_00442A64.c (the CAI at N64 0x140)) |
| S8 | `try_jump` (func_00250EC0.c) | climbing after a debug warp (1432 then 2200) | the player body's `m_pIntelligence` NULL | NULL: no ladder jump (port guard) |
| S9 | `CBarrel__DrawRing` (stack smashed) | the strip wheel with 19+ weapons (T2_GIVE_WEAPONS, control styles without the stick wheel) | `CBarrel__Open` picks 2 rows for up to 26 weapons: 2 x count slots past the 36 of `m_Items`; on the host the overrun hit the player and `DrawRing`'s `items[36]` on the stack | `CBarrel__Fill` stops at 36 slots (port guard; the ROM overruns too) |
| S10 | `Menu_StepValue` via `*Menu_Step*` | `--high-memory`, any slider/option in the sound/options/multiplayer menus | the item callbacks `(s32, s32)` | `sptr` (12 files) |
| S11 | `CAI__SetMode` via `Turret_Hurt` | `--high-memory`, a turret hit | the actor as `s32` | `sptr` |
| S12 | `LevelEvent_Fire` via `func_0042C96C` | `--high-memory`, a level load with delayed events | `(s32)(sptr)` cast on the event pointer | full width |
| S13 | `CISet__GetBlock` via `Player_DrawNodeCallback` | `--high-memory`, attract loop at 27,033 | the draw args' model handle read through an N64 view, its data pointer as `s32`; the "amount" read `m_nVariant` (N64 0x18 is `m_nModel`) | view on `CDrawArgs` by `offsetof`, `sptr` |

Also the menu callbacks of the title/pause/deathmatch menus that open child windows
(`PauseMenu_*`, `TitleMenu_*`, `DeathmatchMenu_*`, `MissionMenu_SelectOk`,
`InventoryMenu_SelectOk`, `SoundMenu_GoBack`, `PlayerSetupMenu_EditName`, `MenuItem_SelectSubmenu`
and its prototypes): `(s32, s32, s32)` -> `sptr` (19 files; `--high-memory` crashed in
`MenuPool_OpenChildMenu` from each).

## 4. Open (repro: the run's cmd.sh; the seeds are deterministic)

| class | runs | notes |
|---|---|---|
| `CGameObjectInstance__SetupAnimation` calls a data address (`m_AI.m_pfCalculateOrientationMatrix`) | warp 13, 22, 24, 26, 34 (both memories) | an object of type 1150 (model 2669) built by `CGameObjectInstance__ConstructFromRecord` after a debug warp (e.g. `T2_WARP_TO=3503@13510,6789@22553`): its CAI callback is not set before SetupAnimation calls it |
| `T2Port_ReadAsset` abort: a host address as a ROM key | warp 19, 33; newgame 59 | `Texture_Request(gScene_m_nDrawActors + 30056)`: a texture key read where a pointer now lives (level 46, mode 12); `CCache__Request` |
| `COverlay__Update` (`*ov->m_pScript++`) | newgame 72 | the death cinema (level 164) HUD overlay script |
| `func_00267EA8` | warp 5 | the player body's intelligence NULL after warps 1432 -> 2200 (as S8): check that warp destination |
| `t2_memset` in `SaveGame_Build` (`PakMenu_InitSaveGame`) | warp 25, 28 (high memory) | the save menu after a debug warp |
| `GameObject_IsSolid` in `CPlayerPool__RegisterActors` | warp 38 | |
| `raw_normal` via `floorTilt` (obj = `D_8010AB98 + 16`, a list tail treated as an actor) | warp 17 (high memory) | `GameObject_GetGroundRotation`'s caller walks a host CList as actors |
| `CISet__GetBlock` via `Model_AdvanceMorph` (`CLevelActor__Draw`) | warp 36 (high memory) | an s32 handle |

The warp class is the debug `T2_WARP_TO` to any of the 298 warp ids: some put the player where
the game never does; the newgame and attract classes are the ones a player can reach.

## 5. The gendata size audit (w4's suggestion)

`tools/port/gendata.py` now writes two lists into `port/gen/data_report.txt`:

- **typed bss structs with an N64 tail**: a typed `.bss` struct that the data points *past*
  (a relocated pointer at an offset >= its N64 size). Those pointers translate to host offset
  `h + (off - n)`, i.e. just after the host struct, where the next host symbol was. The struct now
  gets that tail, twice its N64 size (pointers). Three today: `gBlindOne` (the cinema player, S3),
  `__osTimerCounter` (0x2C), `sMdctFft` (0x204). A first attempt that gave *every* typed struct its
  N64 extent broke the boot (the scheduler watchdog at frame 6,000): kept to the pointed-at tails.
- **typed symbols larger than their N64 extent, with aliases past it**: a C type bigger than the
  ELF's distance to the next label swallows the following symbols as aliases at translated
  offsets. 42 today. Most are fields named as symbols (gEngineApp_*, gScene_*, gCollisionResults_*,
  gMotherBoss_*, gPager_*, gCache_*), which is right. The foreign ones (unrelated objects laid at a
  translated offset inside the base): `__osThreadTail` (extent 8, typed 0x1B0: `__osRunQueue`,
  `__osFaultedThread`, `__osTimerList`, the printf tables `spaces`/`zeroes`/`ldigs`/`udigs`;
  libultra's thread code is replaced by t2os, the printf tables are only read by the crash
  printer), `gCrashFont` (0x5F0 vs 0x600: `sCrashCursorX/Y`, `sCrashBgColor`, `gCrashCauseDesc`;
  the crash screen only), `gThreadStackMain` (0xCCC vs 0x8000: `D_800C894C`), `gSwingAmplitudes`
  (`sWobbleSineTable` at 0x60), `D_004388E0` (`sPlayerSpeedTable` at 0x20), `gCharacterHealthRegen`
  (`D_800ADDB8` at 0x30), `gCheatEntry` (`gCheatEntryText` at 0xC), `gSceneLights`,
  `__osViContexts`, `sBoneMatrixProfile` (`sBoneMatrixCount`). None of these is a host-size
  problem as long as base and alias are laid out the same way on both sides (byte arrays, f32
  tables); the ones whose base has pointers (`__osThreadTail`, `gThreadStackMain`) are not used by
  the port's runtime. Not changed this round: the list is the work list for the next.

## 6. `--high-memory` is the default

`turok2` puts the game memory where the host allocator puts it (above 4 GB on Linux) unless
`--low-memory` (Linux non-PIE builds, debug only: the arena and the game thread stacks below
2 GB). `--high-memory` is still accepted. The executable stays non-PIE on Linux (`T2_LOW_MEMORY`
CMake option, ON): the statics are still below 2 GB; a PIE build is the next step (it moves the
game's data too). macOS has no low-memory mode (unchanged).

## 7. Round 10 (part y5): the open classes closed, a level-warp soak, the raw CAI offsets, PIE

Branch names-R10_y5 (from cc0c322b). Every `src/` change is under `TARGET_PORT` or `s32` ->
`sptr`: `tools/asm_diff.py --base cc0c322b -j 8` = 0 on every touched file. No `include/` change,
no archive change, no gendata change (the archive and `port/gen` of round 9 are reused as they
are).

### 7.1 soak.py

- `--kind levels`: a new game, then `T2_WARP_TO=~level` (a level's start) through **every level
  id 1-164**, four per seed in order (seed s: levels 4s+1..4s+4, so seeds 0-40 cover them all),
  random play in each (`T2_GIVE_WEAPONS=1`, `T2_WAKE_ENEMIES=1500`); `--frames 25000` gives each
  level about 2,800 frames.
- A hang (no exit within `--timeout`, now 600 s: a run takes 3-12 s) is interrupted with SIGINT:
  gdb stops the game and prints every thread (`thread apply all bt 10`, after the crashing
  thread's `bt`); the class is `HANG:` + the busy thread's top frames (the first thread with no
  wait in its stack). Round 9 killed the session and kept no stack.
- `/root/R10_y5/gdbrun.sh RUNDIR [-ex ...]` (box helper, not committed): a run's `cmd.sh` under
  gdb with a backtrace, for the watchpoint work below (gdb disables ASLR: the heap and the static
  addresses repeat, so `watch -l *(long *)ADDR if ...` on a corrupted field finds the writer).

### 7.2 Classes found and fixed (S14-S23)

| id | where | crash | cause | fix |
|---|---|---|---|---|
| S14 | `CGameObjectInstance__SetupAnimation` calls a data address | warp 13/22/24/26/34 (type 1150, a throwable) | `Throwable_Setup` stores `func_00264994 + 0xC` (a splat mis-cut: the real function is `Throwable_CalcOrientation` at 0x2649A0); on the host that address is inside data | the function itself; the whole throwable family on the host: the owner is pointer-wide (`m_VarPtr[0]`, so the tilt moves to `m_VarF32[2]/[3]`; `Throwable_Advance` zeroed the owner's high half), the `OwnedState`/`SpinObject`/`SpinState` views by `offsetof`, `EnemyAI_SetGrabbed`/`EnemyAI_Mount` write `m_Enemy.m_pGrabbed` (13 files) |
| S15 | HANG in `Menu_MoveCursor` | warp 28 (both memories; round 9: `t2_memset` in `SaveGame_Build`) | the save portal menu with the cursor on "health" (no longer selectable) and a link of 0 to the left: the loop never moves | port guard: stop after a full turn (the N64 would spin too; reached only after a debug warp) |
| S9b | `COverlay__Update` (`*ov->m_pScript++`) | warp 19, newgame 72 | `CBarrel__Fill`'s slot layout loop (2 rows of 24 = 48 slots) past the 36 `m_Items`: angles and scales over the player's HUD overlays (S9 had capped only the fill loop) | the layout loop capped at 36 too (found with a watchpoint on the overlay's script pointer) |
| S16 | `T2Port_ReadAsset` abort 0x81AE935C | warp 25/19/33, newgame 59 | `CParticleSystem__Spawn` ORs the draw layer into the texture key (`*bits \|= layer << 30`); the N64 PI decodes 29 bits and ignores it | `T2Port_ReadAsset`/`T2Port_AssetHostSize` mask the key with 0x1FFFFFFF (the PI's physical address) |
| S17 | `GameObject_IsSolid` NULL intelligence | warp 38 (`T2_WARP_TO=3999`: the ending, engine mode 16) | the player body has no intelligence there | port guard: not solid |
| S18 | `raw_normal` via `floorTilt`; then `CGameObjectInstance__Construct` | warp 17 (high memory) | `CInstancePool__SpawnAnimInstance` / `spawn_effect` / `CInstancePool__SpawnAnimInstance_i` took the region (`owner`) and the intelligence (`count`) as `s32` | `sptr` |
| S19 | `Render_DrawModel(resource=0x7fff)` in `BlindOne_Draw` | levels 0 (level 1, the Blind One) | the draw args read at N64 offsets (the model handle's high half) | the view on `CDrawArgs` by `offsetof` |
| S20 | (no crash of its own) `CPlayer__UpdateSpeedScale`, every frame | | all N64 offsets: it read the mode at CPlayer 0xA08 and wrote the speed scale at 0xB28, which on the host are inside `m_Data` (0xB28 is `m_Data.m_AdonAmmoFlagHelped[0]`, set to 1.0f every frame), and the actor's CAI flags at 0x140; `m_CameraMovementScaler` (the head bob) was never set | a host body on the members. The intelligence's speed (0x1C) is typed `u8 pad08[]` in `CPlayerIntelligence`, so the archive's swap plan leaves it big-endian: read big-endian (a little-endian read gave 4.6e-41 and froze the player; this briefly hid S22). The attract demos' `T2_DET_LOG` stays identical to round 9 (52,000 lines) |
| S21 | (no crash seen) the raw `+ 0x140` CAI addresses still compiled on the host | | `BlindOne_PhaseVulnerableInit` (+ `_2`), `EnemyMode_PickupThrowObject_Enter` (also `m_VarBuffer[2]` as the held pointer), `EnemyMode_PickupThrowObject_Update`, `CPlayer__Dismount`, `GameObject_GetFloorKind`, the riding view (`Mount`, func_00251A2C), `ImpactEvent_Rumble` (the hit player's controller rumble pool, N64 0x1A8/0xA50/0x140) | `&actor->m_AI`, the members, host views |
| S22 | `Player_OnMultiplayerKill` (`m_KilledBy[killerIndex]++`), `Cinema_RequestForObject` | levels 2/3/4/5/6/8/15/16/38 (high memory and PIE) | `CPlayerPool__GetIndex` read the player array as a u32 and divided by the N64 CPlayer size (0x1328; host 5,800); `Event_PlayCinema`/`Event_PlayCinemaRemote` took the actor/event as `s32` | host body; `sptr` |
| S23 | `Controller_MenuLeft` via `EnterCheatMenu_UpdateTextField` | PIE only: 22/40 newgame, 16/40 warp, 18/41 levels | the cheat/name entry fields (Rage Wars views) read `Menu::controller` as an s32 at N64 0x20 (on the host `Menu::desc`) and called through `(s32 (*)(s32))`: in a PIE `&gControllers` is above 4 GB | the window's members (`cursor`, `controller`, `player`). y4 fixed the same on the Mac (also the `_2` copy func_00453A08.c): keep one body |

### 7.3 Totals (final binaries, Linux headless, `/root/R10_y5/out/soak/s2`)

| set | binary | runs | frames | crashed |
|---|---|---|---|---|
| attract 100,000, seeds 0-1, high + low | non-PIE | 4 | 399,840 | 0 |
| newgame 36,000, seeds 0-39, high + low | non-PIE | 80 | 2,880,000 | 0 |
| warp 30,000, seeds 0-39, high + low | non-PIE | 80 | 2,400,000 | 0 |
| levels 25,000, seeds 0-40 (levels 1-164), high + low | non-PIE | 82 | 2,046,720 | 0 |
| levels 25,000, seeds 0-40 | PIE (final) | 41 | 1,023,360 | 0 |
| warp 30,000, seeds 0-39 | PIE (final) | 40 | 1,200,000 | 0 |
| newgame 36,000, seeds 0-39 | PIE (final) | 40 | 1,440,000 | 0 |
| attract 100,000, seeds 0-1 | PIE (final) | 2 | 199,920 | 0 |

The non-PIE rows ran before S22/S23 and before the big-endian read of S20 (the little-endian
read froze the player, so those runs missed S22: the final rows are the PIE ones). The baseline
at cc0c322b (warp 0-39, both memories) had 21 crashes in 6 classes; the first levels soak 10
crashes in 5 classes. Boot + 3 demos: `T2_DET_LOG` identical to round 9's binary.

### 7.4 The 42 typed symbols larger than their N64 extent

None needs a change: gendata places an alias at the host address of the member path that
holds its N64 offset (DATA.md point 7), so an alias is wrong only where the base's host layout
differs AND the alias is read through the base's type by host code. By kind:

- **fields named as symbols** (28: `gEngineApp_*`/`gScene_*`, `gCollisionResults_*`,
  `gMotherBoss_*`, `gPager_*`, `gCache_*`, `gControllers_buttons`, `gMergedController_*`,
  `gDemoPlayer_*`, `gLevelScript*` in `D_8012F604`, `gThrowableCollisionInfo_*` in
  `D_800F1F90`, `gEnemyAlertPoint_y/z`, `sRegionNormal_y/z`, `sRegionUnitNormal_y/z`,
  `gPakProfileRead_character`, `gHeap_usedHead`, `gMantidQueen_blink`, `gCheatEntryText`
  (`TextEntry.text`), `gVideoModesPAL`, `D_801306F8` (`gBlindOne.script`), `D_00430F90`,
  `D_800F55F0`, the `sWeaponActorModeTable` rows): right by construction.
- **byte/scalar neighbours** (10: `sPlayerSpeedTable` after `D_004388E0`, `D_800ADDB8` after
  `gCharacterHealthRegen`, `sWobbleSineTable` after `gSwingAmplitudes`, `sBoneMatrixCount`,
  `D_800F1CC6`, `D_800F5C66`, `D_800F677A/B`, `D_800F5A34`/`D_800F5A18` (the camera boxes),
  `D_800ADDE4`, `D_800AE9D2`, `D_800C203B`, `D_800D8E13`, `D_800F5EA3`, `D_800AB0D8`): the base
  is f32/u8/s16 data, the same bytes on both sides; the C type is a few bytes too long (m2c's
  guess). Harmless; shortening the declarations is an `include/` change for no behaviour.
- **foreign objects in a base with pointers** (4): `__osThreadTail` (`__osRunQueue`,
  `__osTimerList`, the printf tables: libultra's thread code is t2os on the host; the tables
  are read by the crash printer only), `gCrashFont` (the crash screen), `__osViContexts`
  (`D_800B7A90`: libultra VI, replaced), `gThreadStackMain` (`D_800C894C`, "the bottom of the
  stack"): `Collision_SweepRegionWalls` and its twin compare `&local < &D_800C894C` to stop a
  deep recursion. On the host the game threads' stacks are never below a static, so the guard
  never fires (harmless: host stacks are 8 MB; it would matter only with a tiny stack).

### 7.5 The raw `+ 0x140` CAI accesses

After this round no `+ 0x140` actor->CAI address is compiled on the host (checked on the
preprocessed `TARGET_PORT` code of every file in compile_commands.json: `clang -E` with each
file's flags, grep `+ 0x140`). What is left: the N64 `#else` branches (they stay for the
matching build), the module-2 `_2` copies (compile-only), the prototype-only view
`Member00401FCC`, and func_0043D57C (`Actor0040457C`, a `_2`-module copy). Not reached and left:
`TotemDefense_KillsLeft` / `TotemDefense_TryHitEnemyAfterDelay` (N64 views over
`TotemDefenseState`, no C caller in the tree).

### 7.6 PIE

`-DT2_LOW_MEMORY=OFF` links a PIE (code and statics above 4 GB). The first PIE soak found one
class (S23); fixed, the final PIE binary ran the 123 runs above with 0 crashes and the demos'
`T2_DET_LOG` identical to round 9. **`T2_LOW_MEMORY` now defaults to OFF**: the Linux build is
a PIE; `-DT2_LOW_MEMORY=ON` brings back `-no-pie` and `--low-memory` (debug). With a PIE build
use soak.py without `--both-memory`. An existing build dir keeps its cached `T2_LOW_MEMORY=ON`:
reconfigure with `-DT2_LOW_MEMORY=OFF`.

## 8. Round 12 (part a5): the N64-offset sweep (`tools/port/n64viewcheck.py`)

Round 11 fixed twelve functions that viewed an instance at its N64 offsets, one at a time, as
crashes and wrong events found them. This round's checker finds the class statically:

```sh
python3 tools/port/n64viewcheck.py --build OUT/build -j 4 [--json out.json] [--all] [--unlinked] [files]
```

It parses every linked `src/` file the way the host compiles it (libclang, `x86_64-linux-gnu`;
only the `TARGET_PORT` side of each `#ifdef` exists) and, for files with a candidate, again for
`mips-linux-gnu` (the N64 layout). Three kinds of site: **raw** (a pointer cast of `base +/- C`
or `((u8 *) base)[C]`: `M2C_FIELD`, `*(s32 *)((u8 *) p + 0x1C)`, the `P_xx` macros), **stride**
(the same with an `i * K` term) and **view** (a member read through a struct defined in the .c
file whose fields are placed by pads). Classes: `BAD` (the base's static type is an `include/`
struct and its member at that N64 offset is elsewhere on the host), `suspect` (a view whose
fields fit a key struct: CGameObjectInstance, CIntelligence, CPlayer, CAI, CGameRegion, CCamera,
CRegionInfo, CScene, ...; and some of those members move), `moves` (the view's own host layout
differs from its N64 one), `global` (`&symbol +/- C`), `untyped` (void * / sptr base; the
`guess` lists key-struct members that move), `local`, `same`. ~90 s for 3,020 linked files with
`-j 4`. A view's field counts as moved only when the member's host offset differs from the
view field's own host offset (a pointer field realigns a view, so `u8 pad[0x14]; void *def;`
lands on the host `m_pIntelligence` at 0x18).

First run (046a67f9 + the PIE build): raw BAD 15, stride BAD 3, view suspect 60, moves 83,
untyped raw 115. Reviewed by hand; **fixed (asm_diff 0, all under `TARGET_PORT`, the views
rebuilt with `tools/port/hostview.py --apply` where they are whole-struct views):**

| function (file) | what it read on the host |
|---|---|
| `Blast_GetPush` (func_002374AC) | the source's flags (0xD4), owner (0x1A8), the target's velocity, yaw (0x50) and mode flags (0x17C); the owner's 0xBB0 is `CPlayer.m_pOnEnemy` (a pointer): every blast pushed in the wrong frame |
| `Blast_GetReach` (func_00238334) | the foot height `m_CollisionYOffset` (0x54) |
| `func_0021B038` | the target's flags (0xD4), health (0x144), foot height |
| `CPlayer__TryMount` (func_0024EAF8) | the mount's yaw and type flag (0xB8), its CAI (0x140) |
| `CPlayer__Dismount` (func_002553BC) | the thrown body's velocity and yaw |
| `Projectile_PlaceAtMuzzle` (func_0025FEB0) | wrote the muzzle velocity, rotation and yaw over the wrong members, and the region pointer through an `s32` (cut to 32 bits) |
| `Turret_Aim` (func_002623EC) | the turret's yaw (0x50) |
| `CLevelActor__BuildMatrix` (func_00215884) | the actor's yaw (0x144), scale (0x164), height (0x168), its matrices |
| `CPooledLevelActor__Update` (func_00227680) | the velocity, bounds and the pooled timer (0x19C) |
| `func_00238EE4` (impact sounds) | the hit object's word at 0x80 and `m_AI.m_VisBits` (0x14C) |
| `CGameRegion__FloodSetWater`, `CGameRegion__FloodClearFlag4Same` (func_0026C5C4, func_0026C934) | the neighbours at N64 0x10-0x18: on the host the vertex pointers, so the flood walked into vertex data and set flags there |
| `CCamera__DrawHitFlash` (func_00280EE8), `CPlayer__DrawScope` (`HudView`, func_00247438), `func_00254834`, `func_00254768` | the camera viewport (0x288-0x294) and the flash (0x530); func_00254768 also `CPlayer.m_SightIconFrame` (0xB60) |
| `CCameraText__Reset` (func_00281B58) | `m_nId` is pointer-wide since round 11 (z3): the reset cleared the padding and kept the old owner id |
| `CPlayer__CheckJumpOn` (func_00255340) | `m_SniperMode` (0xB8C), `m_TimeOffGround` (0xA9C), `m_ContState.m_dwButtonsSingle` (0xA68) |
| `CPlayerPool__ReloadAnimInfo` (func_00252C70) | the camera's `m_ViewParams.m_nType` (0x24) |
| `CPlayer__Update` (func_0024C47C) | the menu pool's first word as `gEngineApp_m_Settings - 0xCAC`: the host's blocks are not N64-sized (0xDB8 apart) |
| `SndVoicePool_FindSlot`, `SndVoicePool_ReleaseVoice`, `SndVoice_Update` (func_00273370, func_00274A98, func_0027314C) | the sound handles through `CAudioWorld + 0x7C` (host `handles` is at 0x12C, not 0xE8) and the voices at the N64 stride 0xCC + 4 (host 0xD8 + 8) |
| `CAudioWorld__PlaySound` (func_00270C80) | the low half of `lastVariant` at +0x132 (big-endian position) |

**Left, with the reason** (`/root/R12_a5/vc/final.txt` after the fixes: raw BAD 10, stride
BAD 3, suspect 44):

- raw BAD that are fine: `GameMatch` minus 0x1B0/0x1B5/0x1D8 (`CStaticObject__Draw`,
  `EngineMode01_Legal_Init`): inside `GameSettings`, which has no pointer (match at 0x1D8 on both);
  `Quad_ComputeNormal` / `Quad_Translate`: arrays of `CVector3`; `Primagen_EnterMode18`
  (`gLevelScript_bossMode - 0xA`, a run of scalars, its own comment).
- strides: 20-byte animation keys (`func_002179F0`, `Anim_GetBoneRotationSlerp`: ROM data),
  `Video_SetMode` (a u16 table); the 18 untyped ones are ISet offsets, matrices, ROM tables.
- suspects that are look-alikes: ROM records (the platform/destructible intelligences
  `StateInfo`, `M1Info`, `Rec54E0`, the movement `Tuning`, `SoundEvent`, `TurretParams`), the
  views of the game's own allocations used consistently (`PoseContext`, `Profile`, the swoosh and
  trail `SSlot`/`SEnt`/`Emitter`/`EmitNode`, `FontStyle`, the menus, `PlayerList` (the same
  `PlayerCList` members as `CPlayerPool`)), the scope `HudModel` (it views `CPlayerOverlay`: its
  only pointer, `m_rpObject`, holds an s32 key, and `m_mOrient` is at 0x10 on both),
  `animReached` (the N64-only helper; the host `Door_OpenUpdate` uses `animReachedHost`).
- **not resolved** (the struct behind the view is not known; worth a trace):
  `Turret_Effects` (`Effect00262B58`: a node-callback context, `value` at 0x90),
  `Generator_SpawnObject` (`T2MotionState` 0x10C/0x110: no CAI member there),
  `CStaticObject__Draw` / `CStaticObject__RequestLitVertices` (`PlacedProp`, `SpawnSource`: no
  clean fit with `CStaticObject`), `RumbleMenu_Play` (`+0x1C` -> `+0xBF8`, maybe
  `CPlayer.m_bEnteringDeathmatch`), and the 110 `untyped` raw sites (mostly menus and
  m2c-only code on `void *` arguments).

**Checks after the fixes** (Linux headless PIE, the rebuilt archive below):

- boot + the 3 demos, `T2_DET_LOG=1 T2_DET_GF=1 T2_DET_ACTORS=1`, 26,000 frames: **523,287
  det lines identical** to the round-11 tree (046a67f9) built here (both commits).
- `playthrough.py --level 1..4` (`--fight 10` for level 1): every level ends at the hub (60)
  with every "mission complete" and the totem saved, as on 046a67f9. Levels 1 and 3:
  every `state`/`event`/`totem`/`boss` line identical (pointers masked); level 2 from 30,000
  (the sisters' level 133) and level 4 from 27,000 (the Cave Totem 112): fights with
  explosions, where the blast push/reach fixes change where the player is thrown (level 4's
  defence: 56 kills instead of 59; the Blind One still dies, mode 9).
- soak (PIE, the new archive): levels seeds 0-39 x 25,000 (40 runs, 998,400 frames, every level id warped into) and new game seeds 0-9 x 36,000 (10 runs, 360,000 frames): **0 crashes, 0 hangs**.

## 9. Round 13 (part b5): a5's unresolved views

The four views section 8 left open, each read against its caller:

| function | what it is | result |
|---|---|---|
| `Turret_Effects` (func_00262B58) | an `m_AI.m_pfEffects` callback: `CGameObjectInstance__PoseBone` passes the bone-posing context (host layout `T2PoseContext`, structs/pose_host.h). Its view `Effect00262B58` read the bone index at 0x94 (N64 0x90 plus the realigned owner pointer); the host member is at 0xE0 | **fixed** (asm_diff 0): the host view places `value` at `offsetof(T2PoseContext, bone)`, the owner asserted on `obj`. Before it, a turret's gun and turn nodes were never rotated |
| `RumbleMenu_Play`, `RumbleMenu_Play_2` (func_00417EBC, func_00450E94) | the select callback of the rumble notice's last item (`MenuItemDesc.select(item, menu)`): `menu + 0x1C` is `Menu.player`, `+ 0x20` `Menu.controller` (its `port` at +4), `0xBF8` `CPlayer.m_bEnteringDeathmatch` | **fixed** (asm_diff 0): a `TARGET_PORT` body through the members. The m2c body took the window's descriptor for the controller and wrote 0 through a pointer made of padding (host `Menu.player` is at 0x30) |
| `Generator_SpawnObject` (func_002638A0) | a Rage Wars import | **unreachable**: no `jal` to 0x2638A0 in the ROM's code and no word holding its address in any data section (`build/ref.elf`), and no caller in `src/` or the generated data. Left as it is |
| `CStaticObject__Draw` / `CStaticObject__RequestLitVertices` (func_00214844, func_00214D04) | `PlacedProp` / `SpawnSource` are host twins of `CStaticObject` | **already right**: `T2_STATIC_OBJECT_CHECK` and the `_Static_assert`s pin the descriptor, the cache handle (`m_pOwner`), the matrix, `m_LitKey`, `m_ViewMask` and the flags to the members; the checker flags them because they are local views |

The `untyped` raw sites (110, `void *` arguments of menus and m2c-only code) were not swept this
round; `RumbleMenu_Play` was one of them. Also found (64BIT.md part 7): the health pickup table, a
union laid out by its first member.

## 10. Round 14 (part c5): locals read before they are written, a Release build on x86-64

Round 13 (b4-2, [MACOS.md](MACOS.md) 15) added `-ftrivial-auto-var-init=zero` to the game code
after Linux Debug and the Mac split in level 135. The variable was not found. This round looked
for it with three tools.

**Switches (port-only, debug):**

- `cmake -DT2_AUTO_VAR_INIT=zero|pattern|none` (port/cmake/game.cmake). `zero` is the default.
  `none` leaves the compiler's default (no init), which is what you need to look for such reads.
- `T2_SCRIBBLE=HEX` (port/app/game.cpp, port/game/t2debug.c): before each game frame, fill 256 KB
  of the stack below the frame with the 32-bit word HEX. A local read before it is written then
  holds HEX, not what the previous frame left there.

**1. The compiler's warnings.** `-Wuninitialized -Wsometimes-uninitialized
-Wconditional-uninitialized` (syntax only) over the 4,044 game C files give 19 warnings:

- 5 "is uninitialized": `CCache__HeapAlloc`, `func_0023640C`, `func_00268080`,
  `GameObject_GetLeashRadius` and `osPiGetCmdQueue`. Each is a matching idiom,
  `if (unset) { A } else { A }`, with the same code in both branches. The result does not depend
  on the value.
- 14 "may be uninitialized": libultra (pfs `fpage`/`start_page`, `alCSeqNextEvent`'s `selected`,
  `alCSeqNewMarker`'s `firstTrack`, `Pak_SaveNoteVerified`'s `result`), `func_00209AF4`'s extra
  block sizes, a HUD message switch, the Mother's `p`. In each one, the path that skips the write
  is not taken.

**2. clang's static analyzer** (`clang --analyze`, the default checkers) over the 3,056 linked
files finds the same ones, plus two more:

- `Door_OpenUpdate`, `Door_OpenNow` and two siblings pass an 8-byte event argument with its
  second word unset to `ImpactEvent_Call`. The N64 code passes the same stack leftover.
- `Path_ExpandNode`'s `next[]` is unset only when there are more than 3 candidates, and there
  are never more than 3.

None of these changes a run.

**3. Runs, Linux Debug, PIE** (the `--all` run of section 16 of PLAYTHROUGH.md, `T2_DET_LOG=1`):

| build | compared with zero-init | det lines | differ |
|---|---|---|---|
| `none`, the same link | the whole game | 160,000 (80,000 frames) | **0** |
| `none`, the game archive's members linked in **reverse order** (another code layout: round 13 changed the trace this way) | the whole game, 290,000 frames | 580,000 | **0** |
| `none` + `T2_SCRIBBLE=0x4b4b4b4b` | through level 135 (80,000 frames) | 160,000 | **0** |
| zero + `T2_SCRIBBLE=0x4b4b4b4b` | the same | 160,000 | **0** |

On c8414758 no unset local changes the run, in either layout, even with a stack full of
garbage. The round-13 split does not reproduce on the merged tree. One of round 13's later
merges (b5, b1, b3) most likely removed it. **The zero flag stays as a safety net.** It costs
nothing, and it makes Mac Debug = Mac -O2 = Linux by construction. The scripts are
`/root/R14_c5/scan/{uninit,an,mix,rev}.py`: the warning pass, the analyzer, the object mixer for
a file bisection, and the relink in reverse or shuffled order. To use them again after a new
split: build `none` and `zero` side by side, then bisect the objects with `mix.py`.

**4. A Release build on Linux x86-64 crashed at frame 2** (`-O2` and CMake's `-O3`). The fault
is in `Controller_ReadAll`: an SSE `orps 0xb0(%rbx),%xmm1` on `gControllers[i]`. The x86-64 ABI
gives a global array of 16 bytes or more 16-byte alignment, and clang relies on that for the
game's typed `extern ControllerProfile gControllers[]`. gendata defined the object as
`T2_ALIGN(8) unsigned char gControllers[0xA60]` (at ...ea8). The Mac (arm64, no aligned SIMD
operands) never showed it.

- Giving every gendata object of 16 bytes or more 16-byte alignment broke the game: level 50's
  trace changed at frame 4,855, then the gfx watchdog fired (`*(u8 *)0 = 0` at frame 6,053, in
  Debug too). Some code still walks from one N64 object into the next, so moving the objects
  apart breaks it. That change was reverted.
- The fix is a list in gendata, `ALIGN16`: only the objects in it get 16-byte alignment.
  `gControllers` moved the crash to frame 1,069: `movaps (%rbx,%rsi)` on `sBatchInstances` in
  `Render_FlushModelBatch`. With `{"gControllers", "sBatchInstances", "sBatchResources"}` the -O2
  build plays the whole `--all` run.

| Linux x86-64, PIE, the `--all` run, 290,000 frames | wall | det lines vs Debug zero-init |
|---|---|---|
| Debug, zero-init | 255 s | - |
| **Release `-O2`, zero-init** (`-DCMAKE_BUILD_TYPE=Release -DCMAKE_C_FLAGS_RELEASE="-O2 -DNDEBUG"`, same for CXX) | **138 s** | 580,000, **0 differ**, exit 0 |
| Release `-O2`, `none` (register leftovers instead of zero) | see STATUS (c5) | |

When a new object faults the same way, add it to `ALIGN16`. To find it: take the faulting
`movaps`/`movdqa`/`orps mem` address from the crash handler's registers and look it up with
`nm -n` (`sBatchInstances` was rbx = base + 0x56ec58). Rerun gendata and rebuild.
