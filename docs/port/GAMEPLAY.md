# Gameplay on the host (Phase 7 round 5, t1)

How `turok2 --game` gets from the title screen into the game: the attract demos and a new game
started with scripted input, what had to change on the way, and the debug aids for following
gameplay headless. Build and run as in [STATUS.md](STATUS.md) section 1; everything here is Linux
x86_64, headless (SDL offscreen).

## 1. What runs

**Attract demos.** From the title (frame 4214) the timeout starts demo 1 (mode 13, level 81) at
frame 5469. It plays to its end (frame 8794: the player walks around x 1980..2320, z -2740..-1890,
y -153.6 on the floor), the boot sequence repeats, demo 2 (level 131, frame 13684) plays too
(the player moves at y 1024), and demo 3 starts loading level 109 at frame 20948, where it stops
(section 4).

**New game.** `--input START@4400,A@4500,A@4700,A@4800`:

| frame | what |
|---|---|
| 4400 START | title -> main menu (`gTitleMenuDesc`, mode 8 at 4455) |
| 4500 A | cursor 1 = `TitleMenu_Single` -> mode 13, level 0, the camera menu `D_0043916C` (New game / Load / Go back, cursor 2) |
| 4700 A | `SingleMenu_NewGame` -> the pak message `gPakMsgRumblePakBDesc` (blank pak) |
| 4800 A | OK -> mode 11 at 4855: the new-game intro cinemas (levels 50, 49), mode 12 at 8977 (levels 47, 45, 46, 48, 49) |
| 12903 | **mode 13, level 51: gameplay**. The player stands on the floor at (-1540.7, -566.4, -1481.6) |

With `CU` held (C-up, forward; `--input` repeats a press every 4 frames) the player walks:
state 3 (walk), down the slope to y -1228.8 by frame 13560, state 11/10 in the water, across a
dozen regions to x 772 by frame 13800. `CL` strafes (move x -1.0). `Z` presses run without a
crash. The d-pad does not turn the player (Turok 2 turns with the stick; `--input` has no stick).

Frames: every gameplay and demo frame is **black** (no `G_VTX`, no texture rectangle in the
display list: `T2_DLDUMP_AT=13600` gives 389 commands, fills and mode setup only; one camera is
active, `gCameraPoolHideViews` is 0). The scene draw is t3's part this round (black cinemas,
same cause or not); the HUD code (`CPlayer__DrawHud`) runs in the demos since this round.

## 2. Debug aids (port only)

- `T2_STATE_LOG=1` (every 60 frames) or `=N` (every N frames), `port/game/t2debug.c`
  `T2Port_LogState`: engine mode, level, `m_WarpState`, `paused`; the first active player:
  position, facing, health, its body's AI mode, weapon, region pointer and the floor height
  under it, lives, `m_Mode` (the player state: 2 stand, 3 walk, 10/11 water, 4 jump), the
  buttons and move/look axes of `m_ContState`, the pad (`inputButtons`/`buttons`, stick),
  the top menu of the player's camera, the active camera count and `gCameraPoolHideViews`.
- `tools/port/hostview.py --build OUT/build FILE VIEW REAL [--apply]`: an m2c / Rage Wars view
  struct (a local struct that reads a real engine struct through N64-offset pads) rebuilt at the
  host offsets of REAL. libclang parses FILE for mips (the layout the view was written for) and
  for the host; every named field of VIEW is mapped to the member path of REAL at that N64
  offset, and the host view is one union of anonymous structs, each placing one field with
  `__builtin_offsetof(REAL, path)`. `--apply` rewrites FILE: the host view under
  `#ifdef TARGET_PORT`, the original in `#else` (asm_diff 0 by construction). Warnings list
  fields whose size differs from the member they read (an `s32` over a pointer). Used for
  `CPlayer__Update` (Player/Input/Voice), `Player_UpdateAim` (AimActor/AimView/AimOptions) and
  `CBarrel__DrawIcons`. Many more local pad views remain (n64_offsets.py does not count them);
  each one that a crash names goes through it in seconds.

Box scripts used this round (not in the repo): a gdb wrapper (`gdb -batch -ex run -x cmds`) and
a new-game runner adding the four presses above to `--input`.

## 3. Fix table (t1)

Every source fix is under `TARGET_PORT` or a typed rewrite with identical N64 code; asm_diff 0
(whole tree after each header change).

| # | where it stopped | cause | fix |
|---|---|---|---|
| t1-1 | `EnemyAI_Update` (demo 1) | local `AI`/`Object` views at N64 offsets (behaviour index 0x133, mode row 0x30, timers) | typed on `CGameObjectInstance`/`CAI` (`m_Enemy.m_Behavior[]`, `m_pModeCurrent->m_pfCallback`, `m_VarF32[6]`) |
| t1-2 | `CGameRegion__ContainsXZ` (demo 2) | `CScene__FindRegionAt` walked regions at the N64 stride 0x20 | `Floor` = `CGameRegion` on the host |
| t1-3 | same, garbage `k` | **asset**: collision table 3 (the region grid: `f32[4]` rect + `u32` count) was `unk[20]` in `swap_spec.json`: counts arrived byte-reversed | `"f32[4]:rect u32:count"`; **the archive must be rebuilt** (`mkarchive.py`, 12 s) |
| t1-4 | `MenuPool_Draw` calls NULL | `TitleMenu_Single` wrote the controller at N64 offset 0x20 = host `Menu.desc` | `menu->controller` |
| t1-5 | `MenuPool_Draw` calls NULL | `CPlayer__NewGame` opened `func_0043916C`/`func_004388E0`/`func_00438808`: data declared as functions, so gendata made zero .bss placeholders | declared `MenuDesc D_0043916C, D_004388E0, gCharacterMenuDesc` under TARGET_PORT |
| t1-6 | `Actors_GetLightVolumeFalloff` (mode 11 load) | **ABI**: `CScene__ComputeLights` passed a `CVector3` by value to a callee taking three `u32` and reading them back as `*(CVector3 *)&arg1` (the o32 save area) | host signature `(obj, f32 x, f32 y, f32 z, ...)` as `func_00212680` and abu_protos.h call it |
| t1-7 | `CScene__CollectSectorObjects` | `CScene__DecompressGridSection` built static objects / level actors at strides 0x90 / 0x198 (N64 sizes) | `sizeof(CStaticObject)` / `sizeof(CLevelActor)` on the host |
| t1-8 | levels stand still | `CScene__Update` tested `*(s32 *)(gEngineApp_m_Game_m_CameraPool + 0x13F4)` (= `GameSettings.paused` on the N64; inside the camera pool on the host) | `gEngineApp_m_Settings.paused`; two `m_CameraPool + 0x40` main-camera sites by member |
| t1-9 | `m_WarpState` stuck at 1.0f | the scene's dynamic-light set (`D_8010FF34`, 0xE8 bytes of `CScene`) holds CLists of 8 lights; on the host the lists overran `m_nLevel`, `m_WarpFade`, `m_WarpState` | `CDLightSet` (particle.h) as `CScene.m_DLightSet`; the set functions, `DLight_Spawn` (passes the light index the N64 left in `$a1`, returns the light pointer-wide), `DLights_Load`, flashlight, particle users typed |
| t1-10 | `in_menu(p = garbage)` | `CPlayerPool__Update` read the pool and settings through N64 views | host `PlayerList` and `Globals` (offsets from `CGame`) |
| t1-11 | `CPlayer__Update` | the `Player`/`Input`/`Voice` views of CPlayer/CPlayerContState/CAIModeInfo | hostview.py |
| t1-12 | `Player_UpdateAim` | views of CPlayer/CCamera/CPlayerOptions | hostview.py |
| t1-13 | `Automap_RevealAroundPlayer` | **ABI**: `Automap_SetPlayer(obj, s32, Triple)` called with `(region, f32, CVector3)`; the region kept in a 4-byte word | `CAutomap.m_pHostRegion` (host only), the caller's types |
| t1-14 | `CScene__SetLevelBit` | raw `CScene` offsets 0x18FA8/0x70 | members |
| t1-15 | the player falls through every floor | `CCollisionSetup__Construct` read each behaviour as the low byte of a big-endian word (`u8` at +3): 0 on the host, so no wall/floor collision | the words read as words |
| t1-16 | `CGameRegion__GetNormalRaw(polygon = stack)` | **ABI**: defined returning `CVector3` by value; every caller passes the result address first (the MIPS hidden pointer) | host signature `(CVector3 *out, region)` |
| t1-17 | `func_00267EE8` | seven enemy-definition tests read the instance definition at N64 offset 0x14 | `header.m_pIntelligence` |
| t1-18 | `CPlayer__Update` (demo) | `gWeaponInfoTable` declared `[]`: gendata cut it at the splat label `gWeaponInfoTable_22`, entries 22..34 read past the host table | declared `[35]` (weapon.h) |
| t1-19 | `CBarrel__DrawIcons` | CPlayer/CCamera views | hostview.py |
| t1-20 | `PlayerJump_Enter` (demo) | m2c fields at N64 offsets; **ABI**: the sound position as three `s32` to `Sound__Play(s16, CVector3, ...)` | typed host body |

## 4. Where it stops, next

- **Demo 3** (level 109, frame 20948): `T2Port_ReadAsset(0x40291210)`: `CStaticObject__LoadGeometry`
  requests a cache key that is a host pointer (`CStaticObject__RequestLitVertices` <-
  `CScene__StreamAllSections` <- `CScene__LoadLevel(109)`): the static object's geometry ROM
  address field is read through a view.
- **New game, A in level 51**: `CBarrel__OpenWheel` dereferences `gWeaponInfoTable[23]` (weapon
  0x17 is in the starting kit); fixed by t1-18 for the table, not re-tested with A.
- The views: `tools/port/n64_offsets.py` counts the M2C_FIELD/cast sites; the local pad views
  are not in that count. hostview.py makes each one a few seconds of work once a crash names it.
- Turning: `--input` has no stick; a `SX`/`SY` stick entry would let scripts turn the player.
- Frames: the 3D scene and the HUD draw nothing in gameplay (section 1); t3.
