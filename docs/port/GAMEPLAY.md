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

## 6. Round 7, part v2: weapons and firing

The weapon change completes and the weapons fire in level 51: the weapon actor's state machine,
the fire animations, the projectiles (through v1's `CParticleSystem__Spawn` port), the ammo
counter in the HUD, the muzzle flashes, shell casings and smoke, the firing sounds, the Talon's
melee swings. Firing needs the animation events (the fire event, `ImpactEvent_SetWeaponFirePos`,
is an animation event) and the particle spawn, so it was tested on a **scratch** merge of
names-R7_v1 (`T2_ANIM_EVENTS` on, v1's archive sha1 `3ad7d240`); this branch alone (events off)
switches weapons and runs the fire modes but launches nothing. All fixes are `TARGET_PORT`
host bodies or views; `asm_diff` 0 (the whole tree after the `controller.h` change).

**The path.** Z sets `m_dwButtons` 0x2000; `WeaponActor_CheckFire` puts the weapon AI in mode 4
(or 5); the mode's animation (`CAIModeInfo.unk_10`: an id, a picker function or an (id, weight)
table, `CAI__UpdateModeAnim`) plays; its fire event calls `ImpactEvent_SetWeaponFirePos` ->
`WeaponActor_FireCallback` -> `CPlayer__FireWeapon` (ammo spent, the projectile type from the
weapon's ammo class) -> `CParticleSystem__Spawn`. The spin/stream weapons call
`CPlayer__FireWeapon` from their mode updates instead (`WeaponMode_SpinFire_Update`,
`WeaponMode_StreamFire_Update`, `WeaponMode_AE634_Enter`).

| # | where | cause | fix |
|---|---|---|---|
| v2-1 | the switch never completed (v4 found it on the Mac too; its commit is cherry-picked here, keep one at merge) | `WeaponActor_Update` (func_00257D48): AI view at N64 offsets: ran the mode table's row 0 instead of `m_pModeCurrent`, and its `unk128 = 0` cut `m_pfStep` | host body on `CAI` |
| v2-2 | the weapon modes | `WeaponActor_EquipRequested`, `WeaponMode_AE714_Enter`, `WeaponMode_AE794_Update`, `WeaponBarrel_Spin_AF180`/`AF324`: State/Spin views of the CAI (mode table 0x2C, effects 0x108, the weapon vars 0x10C..0x130) and of the player's controller (0xA50 -> `rumbleBase`) | host bodies on `CAI.m_Weapon` (`CWeaponAIVars`) |
| v2-3 | the barrels' bone callbacks | `WeaponBarrel_NodeMatrix_AEE38`, `WeaponNode_Rotate_AF324/AF084/AF1D4/AF180` read the pose context at N64 0x00/0x90 | `T2PoseContext` (`obj`, `bone`) |
| v2-4 | `WeaponMode_Switch_Update`, `WeaponMode_Ready_Update`, `CPlayer__CanSelectWeapon` | CPlayer views (0xB2C/0xB30/0xB38 crosshair, 0xA64 buttons, 0x996/0xA08/0xB14, 0x3C8/0x972); `Ready_Update` wrote the next weapon into host `m_WeaponList[26]` | members |
| v2-5 | `CBarrel__MarkUsable` | the wheel items at the N64 stride 0x18 | `CBarrel.m_Items[]` |
| v2-6 | spin weapons' sounds | the camera position at N64 0x114 | `m_pCamera->m_vPos` |
| v2-7 | `ImpactEvent_Send` (broadcast kinds) | `gScene + 0xDF8 / 0xBF8` | `m_nRegistered` / `m_pRegistered[]` |
| v2-8 | `CPlayer__StopWeaponFiringSound` / `StopWeaponBackgroundSound` | the LoopSounds node's id at N64 0xC (after a pointer) | the host node layout |
| v2-9 | pickups | `Pickup_FindWeaponEntry` walked rows at 0x18 with the id at +4; `CPlayer__HasRoomForAmmo` read the ammo type at 0xC; `CPlayer__CollectPickup`'s picker view cut the camera pointer | `CPickupRow18`, `CPickupRow14`, a host view of CPlayer |
| v2-10 | first shot: crash in `RumbleCurve_Advance` | `ControllerProfile.rumblePool[0x28]` / `rumbleNodes[3][0x3C]` were N64-sized byte arrays (the host `RumblePool`/`RumbleEffect` hold pointers); `Rumble_Start` views; `RumbleCurve_Init` stored the curve pointer as s32 | host sizes in `structs/controller.h` (static asserts in func_0026EB3C.c), host bodies |
| v2-11 | pistol, mag 60, shredder, grenade launcher, PFM layer: mode 4 but the idle animation, no shot | `IsCodePointer`/`IsDataPointer` test the N64 segments (`CList__Construct + D_A4694`: size symbols, meaningless in the host link); `CGameObjectInstance__PickAvailableAction` read its (id, weight) table, left as ROM-order bytes by gendata (reached only through an sptr), little-endian, and returned s16 to a caller expecting a pointer | host text range (Linux `__executable_start..etext`, macOS `getsectiondata(__TEXT,__text)`); the table read big-endian; the result pointer-wide |
| v2-12 | the shotgun's flash: the screen an opaque cyan for 8 frames | `ImpactEvent_FlashScreens` took its 7 bytes from the event value in memory order; the camera shakes their 4 halves | the value is two host-order words (below) |
| v2-13 | `func_0023801C` (event sound) | the instance's intelligence at N64 0x14 | `header.m_pIntelligence` |
| (v1) | crash in the new-game cinemas, `CParticle__HitFloor` | the region surface getters return u8, declared s32 | v1's 146bc42e (mine reverted, keep v1's) |

**Event values.** `ImpactEvent_Send`'s last 8 bytes reach the handler as **two host-order
words**, word 0 = the N64's first big-endian word: the archive swaps the animation events'
values as `s32[2]` (swap_spec `AnimEvent`), and v1 declares `CEventValue` `{lower, upper, extra}`
on the host (a9c15e5e) so the particle impacts give the same words. The integer handlers
(damage `IntPair`, sound id, message index, `SetWeaponFirePos`) read the words; the byte/half
handlers extract from them.

**Results (scratch merge with v1, Linux, low memory).** With `T2_GIVE_WEAPONS=1` and the wheel
(A and B groups, 8 directions each, Z held 60 frames each; 17,200 frames, no crash):

| weapon | ammo | | weapon | ammo |
|---|---|---|---|---|
| 1 war blade | melee (mode 4) | | 12 sunfire pod | 6 -> 5 |
| 3 tek bow | 10 -> 9 | | 13 cerebral bore | no lock, no shot (needs a target: crosshair mode 2) |
| 5 mag 60 | 50 -> 41 | | 14 p.f.m. layer | 10 -> 9 |
| 7 charge dart rifle | 30 -> 29 | | 15 grenade launcher | 10 -> 9 |
| 9 shredder | 10 -> 8 | | 16 scorpion launcher | 12 -> 11 |
| 10 plasma rifle | 150 -> 145 | | 20 flame thrower | 50 -> 41 |
| 11 firestorm cannon | 145 -> 140 (shares plasma's ammo) | | 21 razor wind, 22 nuke, 23 flare | fire modes run, ammo unchanged (the flare costs 0; razor wind and nuke to check) |

`T2_WEAPON=4` (pistol), Z held 40 frames: ammo 50 -> 44, `--audio --wav`: RMS 200 before the
first shot, 2,500-4,200 while firing (peak 12,782). The Talon (no `T2_GIVE_WEAPONS`) swings
through its combo animations (4, 5, 6). The journey with the events on (the bow picked at
14050 in the water) runs to 15,000 frames; the bow does not fire under water (its flags allow
land only). `--high-memory` with the events on stops in the new-game cinemas in the damage
pipeline (`Blast_CanHit`, func_0023726C: N64 views; v3).

Frames (`T2_SHOT_FROM`): the arrow of the tek bow flying from the dock (13121) and its splash on
the water (13161); the pistol's muzzle flash, ejected shell casings and smoke (13111-13121), the
counter 50 -> 49; the shotgun's flash star and smoke, then a full-screen purple flash fading over
8 frames (the event's colour: opacity 0xF0, RGB C8 40 FF, as the data says; not checked against
the N64); the bow drawn with an arrow nocked and released (counter 20 -> 19 -> 18).

**Firing in the scripted journey** (INPUT.md section 6): the bow from the wheel and two arrows on
the dock before the harbour:

```
START@4400 A@4500 A@4700 A@4800    # title -> main menu -> single player -> new game -> OK
A@12950..12990 SX=80@12960..12990  # weapon wheel, stick right: the bow (weapon 2)
Z@13060..13120                     # draw (hold Z), release: the arrow flies (20 -> 19)
Z@13200..13204                     # a quick second shot (19 -> 18)
CU@13300..13740                    # down the ramp, into the harbour water
SX=80@13750..13774                 # turn right about 90 degrees
CU@13780..14300                    # swim across the basin
```

Debug switches (port/game/t2debug.c, debug-only): `T2_GIVE_WEAPONS=1` every weapon usable on land
or in water with full ammo once per level in gameplay; `T2_WEAPON=<n>` also requests weapon n;
`T2_WEAPON_LIST=1` prints the weapon table (names, flags, model, ammo class); `T2_STATE_LOG`
prints the current weapon's ammo; `T2_SHOT_FROM=<frame>` starts the periodic screenshots there.

**The journey on this branch alone** (events off) runs to 15,000 frames and the boot to 26,000
(no crash), but from the turn at 13450 the swim differs from round 6 (13800: x 691.8, rotY -1.67
instead of x 358.3, -1.55; both end at (795.2, 233.0)): round 6's `WeaponMode_Ready_Update` read
the player's pose from a wrong host offset and wrote into `m_WeaponList[26]`; with v2-1/v2-4 the
weapon AI follows the player's pose. Treat the new numbers as the reference.

**Next.** The cerebral bore (lock-on: crosshair mode 2, `Player_UpdateAim`), the nuke's charge,
the flare and the razor wind ammo; the underwater weapons (harpoon, torpedo) in the water; the
weapon pickups lying in level 51 (the pickup handlers are typed; walking over one is untested);
the projectiles hitting enemies (v3's damage path, which also fails `--high-memory`).
(`CPlayer__CollectKeyPiece`, func_00248644, the key and nuke pieces: raw CPlayer offsets, now
by member; untested.)
