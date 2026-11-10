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

## 5. Round 6, part u3: turning, the water, the weapon wheel

With the stick in `--input` ([INPUT.md](INPUT.md) section 6) the player turns and can be steered
through level 51. Fixes (all `TARGET_PORT`, asm_diff 0 against 6d04fabb):

| # | where | cause | fix |
|---|---|---|---|
| u3-1 | the player never turns (rotY fixed at -3.14 whatever the stick) | `CPlayer__TurnAndLook` (func_0024EF70) reads the region info's u16 pair at 0x50 (`m_DamageAmount`, `m_wFlags52`) as one big-endian word and tests the low half for 0x2000 (no turning on that floor); the archive swaps the pair as two u16, so the host low half is `m_DamageAmount` (0x2710 in level 51: 0x2710 & 0x6000 = 0x2000) | the view reads the u16 at 0x52 |
| u3-2 | the weapon wheel's stick picks nothing (requested weapon stays 0) | `CBarrel__RunStick` (func_0024BAD4): the `MenuRow` view puts the items at N64 0x1C with stride 0x18 and reads the weapon id as `half.lo` of a big-endian word (the high half on the host) | host body on `CBarrelItem` (`m_Items[slot]`, `(s16) m_Weapon`) |
| u3-3 | the weapon actor never runs its AI (mode 0 for ever) | `CPlayerPool__AnimateWeapons` (func_00252ADC) and `CPlayerPool__GetViewPlayer` (func_00254B04) test camera word N64 0x54C (`m_Menus.count`) at the raw offset: non-zero on the host, so "a menu is open" | `CCamera.m_Menus.count` by member |
| u3-4 | (gameplay path, found reading) | `CGameObjectInstance__MoveWithCollision` (func_0021A658): ObjectLinks23C/CallbackState288 views at N64 offsets (flags 0xD4, the mode's collision info, path-avoid region, anim type, collision callback); `CGameObjectInstance__FireAnimEvents` (func_0021ABC8) ObjectLinks | host body on `CGameObjectInstance`; hostview.py for ObjectLinks plus `anim` = `m_CurrentAnimation` by hand |
| u3-5 | (surfacing) | `PlayerSwim_Update` (func_00250BF0) plays the gasp at `((Ui *) m_pCamera)->pos` = N64 0x114 | `m_pCamera->m_vPos` |

**The water is not a bug.** The round-5 "stuck in state 9 at (-1266.6, -779.5, 260.0)" is a
corner: walking straight on (rotY -3.14, no turning possible before u3-1) the player goes down
the ramp, swims (state 11) under the surface, surfaces into state 9 (treading water: wade
tuning, the surface at y -779.5 over a floor at -1228.8) and slides along the walls into a
corner. Turned right (`SX=80` for 25 frames, rotY -1.55) he crosses the basin to x 358
(frame 13800), where the next wall stops him. t1's run went to x 772 because its tree had no
static-object collision yet (t3's grid sections were not merged), not because the swim path
differs. Journey (this branch, `T2_STATE_LOG=60`, the script in INPUT.md section 6):

```
12903 mode 13 level 51, (-1540.7 -566.4 -1481.6) state 2
13080 (-1540.7 -735.8  -982.4) state 3, down the ramp
13140 (-1157.1 -879.0  -911.3) state 11, swimming under water (floor -1228.8)
13200 (-1111.0 -779.6  -603.6) state 9, at the surface
13440 (-1266.6 -779.5   260.0) the corner; turn right
13560 ( -872.6 -779.5    58.6) rotY -1.55
13800 (  358.4 -779.5   -52.5) the far wall (slides 0.12/frame)
14100 wheel open (A + stick right): requested weapon 2; Z fires: no crash
```

Frames with u1's branch merged on top (scratch only): frame 13051 the stone ramp down to the
harbour, the brick walls, the hanging lamp and boxes; 13501 a wooden pier against the wall at
the corner; 13801 a close-up of the rock at the far wall (that scratch run stopped at frame 14400, no crash). This branch alone runs the same script to frame 15000 without a crash.

**Weapons.** The wheel opens and closes (state 1, 2, 3) with A or B, the stick picks a slot
(u3-2), `m_RequestedWeapon` follows; `CBarrel__OpenWheel` no longer crashes. The switch and
firing need the weapon actor's animation: after u3-3 the weapon AI runs (mode 1, raising) but
waits for `m_AI.m_AnimRepeatedCount`, and the actor's animation never loads because
**`m_nAnims` is 0 for every actor**: `CGameObjectInstance__SetupAnimation` (func_0021A150,
host body) reads it as `((u16 *) block1)[3]`, the low half of the second big-endian word;
the archive swaps that block as words, so on the host `[3]` is the high half, 0 (the block
reads 4, 6, 20000, 20010 as words: the weapon has 6 animations). Reading
`(u16) ((u32 *) block1)[1]` fixes it (tested), **but it is not committed**: with animations
loading, the animation events fire, and `CGameObjectInstance__FireAnimEvents` ->
`ImpactEvent_Send` -> `Actor_SpawnParticle` -> `CParticleSystem__Spawn` (func_0022D2FC) crashes
at frame 1362 (the Acclaim cinema): `EffectSystem` (sys->resource at N64 0xEC64; hostview.py
maps it, the resource must be `sptr`) and then the `Effect`/`Owner`/`Actor` views and the
particle table entries (`entry->params`) are all N64 layouts. Port the particle spawn first,
then apply the m_nAnims fix (with u2's animation chain: AdvanceAnimation, UpdateAnimation,
func_0021A500, RootActor, the bit readers, which u2 committed in its branch).

Not done (the C20-1 sweep): only the sites the player path hit were fixed (u3-1..5). Remaining
known views on this path: `CPlayerPool__UpdateMatchClock` (func_0025492C), `Action_CheckRequiredItem`
(func_00265F90), func_00271694, `GameObject_GetRegionInfo` (func_00268A2C: the region read as
s32 at 0x10, a pointer truncation on macOS), `CParticleSystem__Spawn` (above), the region info's
`unk_48` bytes (0x48 warp ID and 0x4C pending warp are u16 read by `CPlayer__Update`'s `Surface`
view, but `swap_spec.json` keeps them as bytes: warps between levels will read byte-reversed IDs;
type them in `structs/region.h` and rebuild the archive).

## 6. Round 7, part v5: level warps and the boss levels

**A real warp, end to end.** Level 51's exit is region info 21 (`CRegionInfo.m_WarpID` 1098,
flags 0x40340060: 0x40000 "changes the player's state", 0x200000 "entered from a region without
it"), a strip at x 3778..3893, z 529..683, y 307.2, on the high ledge at the east end of the
harbour. `tools/assets/levels.py --level 51` decodes it (the region infos' `rest` bytes 12..13 are
the warp id); `warp_dests.json` gives the destinations: 1098 -> level 52 (1280, 0, -3225.6),
1813 -> level 51 (3819.5, 307.2, 614.4), the arrival point inside that same strip.

The test (`T2_STATE_LOG=30 T2_WARP_TO=1813 ... --input @exit52.txt`, 16,000 frames):

```
START@4400 A@4500 A@4700 A@4800     # new game
CU@13100..13120                     # step out of the exit strip (west)
CD@13140..13300                     # back into it: CPlayer__Update -> enter_surface -> CPlayer__Warp(1098)
CU@15000..15300                     # level 52: forward into its exit back to level 51 (warp 1813)
```

```
12955 T2_WARP_TO: CPlayer__Warp(player, 1813, 2, 0): fade, level 51 reloaded at the exit
13140 (3712.6 307.2 614.4) out of the strip
13200 warp state 1: the player walked back in (the game's own trigger)
13260 level 52, (1280.0 0.0 -3225.6) rotY -3.14: the arrival point of 1098
15060 level 52's exit, warp state 1;  15120 level 51 at (3819.5 307.2 614.4)
```

Level 52 draws (frame 13351: a stone corridor with ladders and wooden walkways, the HUD). The
same run passes with `--high-memory`, and on the scratch merge with v1's particles.

`T2_WARP_TO=<id>[@frame][,<id>[@frame]...]` (port/game/t2debug.c, **debug only**): at the frame
(default 60 frames into gameplay: mode 13 in a level > 0, a player, no warp running) it calls
`CPlayer__Warp(player, id, 2, 0)`, the path a warp region takes (the hub/key redirects of
`CPlayer__Warp`, `CScene__FindWarpDests`, `CScene__RequestWarp`, the fade in
`CScene__UpdateWarp`, `CScene__LoadLevel`). A negative id is `~level`: that level index at its
default spawn (`CScene__UpdateWarp`'s ~id path). Ids redirect like the game does: 7100..7601
send the player to the hub (1000..6000) while the hub's objectives are not done (7100 -> 1000
with the "level not complete" message, 7601 -> hub 6, level 114, after its first-visit
cinemas 155..161).

**The boss levels** (warp ids from warp_dests.json; Linux, headless):

| warp | level | boss | this branch alone | with v1's particles (scratch merge) |
|---|---|---|---|---|
| 7501 | 2 | Mantid Queen | loads; the boss intro cinema (Turok, the Queen's legs, letterboxed); gameplay at the arena entrance to frame 18,500 | the same |
| `-4` (~3) | 3 | Mother | needs the particles | loads, the arena draws (the slime pit, the grating, the pipe) to frame 17,000 |
| 7602 | 4 | Primagen | `CParticleSystem__Spawn` (Primagen_SpawnTossedObject) | loads, the arena draws (frame 13201: the green-lit floor, the blue panels); stops at 13260 in `Path_ExpandNode` (AI pathfinding: a region read through garbage) |
| `-110` (~109) | 109 | totem 1 | `CParticleSystem__Spawn` (Generator_SpawnEnter) | loads and runs to frame 16,000 |

Fixes (all `TARGET_PORT`, asm_diff 0 against d3f05a74; include/ edits: whole tree):

| # | where it stopped / what | cause | fix |
|---|---|---|---|
| v5-1 | `CPlayer__Warp` | `WarpPlayer` view of CPlayer at N64 offsets; region, camera and controller words as s32 | hostview.py, pointer-wide fields |
| v5-2 | `Turret_Advance` (level 3 load) | raw N64 offsets on the actor and its CAI | host body |
| v5-3 | `Action_Advance`, `Generator_Setup` | type id/flags at 0xB8/0xD4; the mode table/step at CAI 0x2C/0xF0 (generators never got their step: no spawns) | members; host view |
| v5-4 | `Platform_Step`, `_AtStartUpdate`, `_AtEndUpdate`, `_ForwardUpdate`, `_BackEnter`, `_GetModeAnim`, `_UpdatePosition` | raw offsets / views (the region read as s32) | host bodies, hostview |
| v5-5 | `Primagen_InitAI`, `Primagen_Step`, `MotherBoss_Init` | CAI/actor views: the mode table, callbacks and flags written at N64 offsets | hostview |
| v5-6 | `MantidQueen_Draw`, `Primagen_Draw`, `MotherBoss_Draw` | the boss views (mask 0x14C, mode 0x174); `Render_DrawModel` given the draw matrices as `*(s32 *)(actor + 0x98)`; the weak-spot stride 0x68 | hostview; members; `sizeof(BossWeakSpot)` |
| v5-7 | `TotemDefense_InitFight` | all N64 offsets (and a pointer stored in an s32) | host body on `TotemDefenseState`, after module 2's typed twin |
| v5-8 | `func_00268460` (collision radius), `func_0026841C` (collision y offset) | raw offsets (0x14, 0xD4, 0x1A8 -> CPlayer 0xBB0, 0x54) | host bodies |
| v5-9 | `GameObject_GetFrameDelta`/`GetFrameTurn` (stack smash in `AI_RideMovingPlatform`) | `AI_ComputeFrameMotion` fills a 0x40-byte `CAIFrameMotion` into a 12-byte buffer (safe only in the MIPS frame), the AI at N64 0x140 | the whole `CAIFrameMotion`, `&obj->m_AI` |
| v5-10 | `CScene__FindInstances` (the bosses found no parts: `MotherBoss_Init` NULL) | the scene's actor table at 0xE0 and the N64 instance stride 0x288 | host body |
| v5-11 | `Math_AcosLookup` (Mother level) | NaN -> int: MIPS `trunc.w.s` gives 0x7FFFFFFF (the end branch), x86 INT_MIN (a negative index) | MIPS result on the host |
| v5-12 | `Primagen_Step` divided by zero | `D_8012F604` (the block gLevelScript lies in) was bytes of its N64 label extent 0x448; the host `LevelScriptState` is 0x608: `partsDone` overlapped `gLevelScript_path`, which `Primagen_InitAI` clears | `LevelScriptBlock` (structs/boss.h, TARGET_PORT) declared for `D_8012F604`: gendata sizes it and places `gLevelScript_*`, `gTotemDefense*` at host offsets; `TotemDefenseState` as `LevelScriptState.totemDefense` (N64 layout unchanged). **gendata reruns** |
| v5-13 | the player path (STATUS round 6 next 7) | `Action_CheckRequiredItem` (Pickup/State/Toucher views), `CPlayerPool__UpdateMatchClock` (settings through a menu-pool view at N64 0xCC5/0xE78/0xE84), `CScene__CheckAutoActivation` (`e + 0x140`), `GameObject_IsOnGround` (0xD4/0x140), the path track at `obj + 0x1D4` | members (`func_00271694` is replaced by port/audio; `GameObject_GetRegionInfo` was already host) |
| v5-14 | `spawnPickup` (generators), `ImpactEvent_LevelSpecial` | the region / the actors as s32 | sptr |

Not done: the Primagen fight past its start (`Path_ExpandNode`; the 41 files that read the level
script through local views anchored at `gLevelScript_*` symbols with N64 deltas, e.g.
`Primagen_UpdateMode19`'s `LevelScriptF604`/`BossCtl`: the anchors are now right, the deltas
past a pointer member are not); a boss fight with the boss on screen (the Queen and the Mother
appear on triggers the scripts do not reach yet); the Blind One (level 1, 7401).
