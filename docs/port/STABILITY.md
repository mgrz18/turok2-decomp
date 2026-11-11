# Stability: soak runs, crash classes, the gendata size audit (Phase 7 round 9, part x5)

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
