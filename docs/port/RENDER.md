# The 3D scene on the host (Phase 7 round 5, t3)

Round 4 left both intro cinemas black: they ran to their end, but their display lists had no
`G_VTX`. This round follows the scene's draw path (`CEngineApp__DrawWorld` -> `CScene__Update` ->
`CCameraPool__Draw` -> `CCamera__DrawScene` -> `CScene__DrawGeometry` / `CScene__DrawRegisteredInstances`
...) down to the pixels. Result: the **second intro cinema (mode 5, level 44, the Iguana logo in the
jungle) draws its static level geometry** (trees, ferns, the vine, the sky, the IGUANA letters,
the letterbox), then the title screen draws as before. The first cinema (mode 4, level 162, the
Acclaim logo) still draws black apart from a backdrop quad: its logo is an animated instance, and
the actor draw path (bones, animation) is still on N64 views (section 3).

## 1. What stopped the scene, in the order it was found

| # | where | cause | fix |
|---|---|---|---|
| R1 | `CCamera__AdvanceAttributes` (func_0027CAB0) | the region set attributes (`CRegionInfo` 0x00-0x24: fog colours, fog start, far clip, FOV, held values, blend time) were `u8 unk_00[0x20]` in `structs/region.h`, so the archive's swap plan (swap_spec.json reads that header) left them big-endian: far clip 0, a NaN projection, the camera's view box a point | fields typed in region.h (**the archive must be rebuilt**: `mkarchive.py`, a few seconds) |
| R2 | same | the fog colours written at N64 offsets `cam + 0x508/0x50C` | `cam->m_FogColor[i]` / `m_LastFogColor[i]` |
| R3 | `CScene__Update` (func_00220530) | `paused` read at camera pool + 0x13F4: instances never advanced | `gEngineApp_m_Settings.paused` |
| R4 | `CCamera__DrawScene` (func_0027E770) | `world + 0x10518` for the actor pool | `&world->m_ActorPool` |
| R5 | `CScene__LoadVisibleGridSections` (func_002216DC) | its `Viewer` view (next at 0x4, box at 0x350) is a camera at N64 offsets: no grid section was ever loaded (`m_nSectors` 0) | `m_pNext`, `m_AnimMin/m_AnimMax` through macros |
| R6 | `CScene__DecompressGridSection` (func_00220D00) | records sized 0x90 / 0x198 (host `CStaticObject` 0xA0, `CLevelActor` 0x1A0) | `sizeof` |
| R7 | the static object files (func_002141F0, 00214564, 00214844, 00214B58, 00214D04, 00214DBC, 00214E88, 00214F14) | each reads `CStaticObject` through its own view (`PlacedProp`, `SpawnSource`, `Drawable`, M2C offsets) with N64 offsets; `CStaticObject__RequestLitVertices` called `LoadGeometry()` with the object passed through `$a0` | `CStaticObject` fully named in `structs/level_actor.h` (N64 layout unchanged); host twins of the views checked against it by `_Static_assert` (`T2_STATIC_OBJECT_CHECK`); LoadGeometry/BuildLitVerticesCallback on the members |
| R8 | `CEngineApp__Construct` | the port never ran the boot's cold-start path (func_0028D380 sets `gWarmBoot = 1` when no boot signature is in RDRAM), so `GameSettings_SetDefaults` did not run: the difficulty mask stayed 0 and `CScene__DrawGeometry` rejected every object | `gWarmBoot = 1` in `T2Port_Boot` (port/runtime/t2boot.c) |
| R9 | `Actors_GetLightVolumeFalloff` (func_002123D4) | **ABI**: defined with three `u32` (the ROM reads float words from a1..a3 and takes `&arg1` as a vector), called with three floats and, from `CScene__ComputeLights`, with a 12-byte float struct by value (xmm0 {x,y} + xmm1 {z}) | the definition takes floats into a local union; the struct caller passes `.x, .y, .z` |
| R10 | `CCamera__DisplayListSetup` (func_0027D160) | the file's `gEngineApp_m_Game_m_CameraPool` macro is `&m_MainCamera - 0x40`; on the host `m_MainCamera` is at 0x68, so the main camera was never recognised and its setup **cleared the z buffer and the frame over the drawn scene** (this alone kept the cinemas black once geometry was emitted) | `__builtin_offsetof(CCameraPool, m_MainCamera)` |
| R11 | `Actors_AccumulateVertexLighting` / `Lighting_BuildLitVertices` | `sLitVertexOffsets` has no size in the ELF, gendata made it one `CVector3`; the lit-vertex builder wrote up to `total` vertices over `gAudioWorld` (bank pointer: SIGSEGV in `SndVoicePool_StopAllExcept` at the end of cinema 2) and `gLoopSounds` | declared `[0x9000 / 12]` (0x800DD790-0x800E6790, 3072 vertices) in globals/render.h |

Every fix is under `TARGET_PORT` or port-only; `tools/asm_diff.py --base 98574f3c -j 8` = 0 over
all 3,938 files after each commit.

## 2. Frames (Linux, headless, `--game --draw --pak-blank`, T2_SHOT_EVERY)

- 2401: the IGUANA letters on the jungle floor, tree trunks and ferns behind, letterbox bars.
- 3001 / 3301 / 3901: the camera moving through the jungle: tree trunks, fern canopies against the
  sky, the hanging vine with moss; textures, vertex colours (lit vertices) and depth look right.
- 4500: the title as in round 4.
- 1501-1801 (mode 4, Acclaim logo): black but for a dark grey backdrop quad (the logo is an
  animated instance, section 3).

`T2_DLDUMP_VTX=1` (with `T2_DLDUMP_AT=<frame>`) now also prints the first vertices of every
`G_VTX` (segments followed through `G_MW_SEGMENT`) and every `G_MTX` as s15.16 values: that is how
the camera clear (R10) was found (sane vertices and matrices, a fill over them at the end).

## 3. Animated instances (actors): done in round 6, see section 5

`CGameObjectInstance__Draw` (func_0021D39C) reads the instance through an `Actor` view at N64
offsets. A host body on `CGameObjectInstance` members is ready (saved on the box as
`/root/R5_t3/actor_draw_0021D39C.patch`: m_dwFlags, m_AI.m_pfDraw,
m_AI.m_pfCalculateOrientationMatrix, m_rpObject/m_rpObjectHeader, m_vMin/m_vMax, m_mfOrient,
m_Lights) but not committed: with it the Acclaim logo reaches
`CGameObjectInstance__BuildBoneMatrices` (func_0021CDAC: `AnimObject` view, the animation file at
N64 0xA8 read as 0 -> `T2Port_ReadAsset(0)` abort), and behind that
`CGameObjectInstance__PoseBone` (func_0021B95C, `PoseObject`/`PoseContext` views, 392 lines) and
`CAnimationState__*` (func_00218B10). Mapping: def 0x14 = header.m_pIntelligence, mtx 0x58 =
m_mfOrient, animIndex 0x98 = m_pmtDrawMtxs (MtxPool_Alloc's pointer, declared s32), animFile 0xA8
= m_rpAnims, animCount 0xBA = m_nAnims, flags 0xD4, channel 0xD8 = m_CurrentAnimation,
blendChannel 0xEC = m_BlendAnimation, hasEffects 0x10F = m_ActiveSwooshes, attachMask 0x14C =
m_AI+0xC, owner 0x1A8 = m_AI.m_pParent, hasAttachments 0x27C = m_dwHotPointFlags. Every enemy,
pickup, the player's weapon and the cinema logos go through this path.

Also seen, not fixed: `CScene__BuildGeometryDisplayList` (multiplayer) reads the camera list at
`&gEngineApp_m_Settings_difficulty - 0x1210`.

## 4. Gameplay draws the level (Phase 7 round 6, u1)

Level 51 (the first gameplay level after a new game) drew only the sky, the fog fill and
"NEW CHEAT ENABLED". Its display list had no `G_VTX`/`G_MTX` although `CScene__DrawGeometry` ran
every frame. Five causes, found in this order with gdb on the gameplay frame (breakpoints on
`CScene__DrawGeometry if w->m_nLevel == 51`, then a watchpoint or stepping):

| # | where | cause | fix |
|---|---|---|---|
| G1 | `CCamera__Advance` (func_0027D160) | its `CFloor` view read the camera region's `m_VisBits` at N64 offset 0x1C; the region is a host `CGameRegion` (pointer-wide vertex/neighbour slots), so it read the high half of `m_pNeighbours[1]`, 0. With view mask 0, `vmask & m_ViewMask` rejected every static object. (`CCamera__SetViewParams` sets the right mask during the update; `CCamera__Advance` overwrote it before the draw.) Cinemas use `gDefaultVisBits`, so they were not affected. | `CFloor` = `CGameRegion` under `TARGET_PORT` |
| G2 | `CCache__RequestDecompression` (func_00203CA0), `CCache__LookupAndLock` (func_002034F4), `CCache__Touch` (func_00205A2C) | **the cache hash lookup** used the N64 bucket stride `* 0x10`; the host `HashEntry` is 32 bytes. These lookups probed the wrong bucket and never found a resident entry, so each frame made a new request. In the asynchronous mode (`gCache.frameDelay` 0: gameplay) the request returns NULL, so `CStaticObject__Draw` dropped every object (no lit vertices). The cinemas load synchronously, and that path returns `CCache__CompleteRequest`'s existing entry, which is why they drew. | `HASH_STRIDE` = `sizeof(HashEntry/HashNode)` under `TARGET_PORT` |
| G3 | `CScene__InitPersistentData` (func_0042C3BC) | a `MenuState` view wrote the preview fields at N64 offsets 0x18FE0-0x18FE8 of `gScene`. On the host this hit other `gScene` bytes, and `m_PreviewOptions` stayed 0, so the first warp of every new game announced `unlockedCheats` (0x4000000, the default) as "NEW CHEAT ENABLED". | `CScene` members (`m_PreviewState/Timer/Options`) |
| G4 | `COverlay__Construct` (func_0027ACC4), `COverlay__StartScript` (func_0027AD00) | N64 views of `COverlay`: on the host `m_pScript` is pointer-wide. The script pointer was stored in 4 bytes and the start position landed in `m_Mode`/`m_Time`, so no HUD element (health, ammo, lives...) was positioned or scripted. | host bodies on `COverlay` |

All under `TARGET_PORT`; `tools/asm_diff.py --base 6d04fabb` = 0 for each file.

G2 matters beyond the level geometry: every asynchronous cache lookup (`CCache__RequestDecompression`
and `CCache__LookupAndLock`/`CCache__Touch` callers: lit vertices, models, textures, sounds loaded
during play) missed, so anything requested after the level load was re-requested every frame.

Frames (Linux, headless, `--input START@4400,A@4500,A@4700,A@4800`, T2_SHOT):

- 13001 (the first gameplay frames): the Port of Adia start. A ramp runs down between a brick
  wall with an arch row (left) and a panelled wall (right); crates on the ledge, a rope and
  pole, the sky above, fog in the distance. Textures, vertex colours and depth look right.
  Health HUD "+ 100" bottom left (after G4). No first-person weapon yet (u2's actor path).
- 13201-14401 with C-up held (the player walks to (-1266.6, -779.5, 260.0), u3's water stop):
  the arch row with red lights in each arch and the poles, seen from the water level.
- Attract demos: demo 2 (level 131, frame 14502) and demo 3 (level 109, frame 21501) draw their
  levels (corridors, a door, walls) with the weapon wheel's 3D models over them. They were a
  grey fill or a texture close-up before. Demo 1 (level 81, frames 5501-8001) draws its geometry,
  but very dark: the lit vertex colours are mostly 0x050505 with blue lamp highlights, and the fog
  colour is 0x101010. That may be the level's real look (a dark interior); not verified against a console.
- The boot still runs 26,000 frames (158 s) through 3 demos and the boot loop, the Iguana
  cinema and title as before.

Debug aid: `T2_SCENE_LOG=1` (with `T2_STATE_LOG=N`) prints the first player's camera (position,
view box, far clip, FOV, visibility mask, region), `gCameraVisibleBox`/`gCameraViewBox`, the
camera's animation box, and the level, the grid sections (`m_nSectors` is 0 between frames: it is
filled by `CScene__LoadVisibleGridSections` in the draw), the difficulty mask and the cheat flags.

Next on this path: the animated actors (section 3; the player's weapon, enemies, pickups), the water
surface (a translucent level actor, not checked), demo 1's brightness, and
`CScene__BuildGeometryDisplayList` (multiplayer: reads the camera list at `&difficulty - 0x1210`).

## 5. Animated actors (Phase 7 round 6, u2)

The actor path now runs on host types: the **Acclaim logo cinema (mode 4) renders** (the
letters flip in and settle into "Acclaim entertainment, inc."), the new-game cinemas draw their
characters (Adon, Turok, the enemies, the prisoner in the cage, posed and animated), and with
u1's level fixes merged the **player's weapon (the Talon in Turok's hand) draws in level 51**.
Pose check: the logo's 11 bone matrices at its first frame equal `tools/assets/anim.py`'s
`pose()` times the instance matrix (x65536) within 2 units of s15.16 (script used:
`/root/R6_u2/oracle.py`, ROM + object-info ROM address + a gdb dump of `ctx.animIndex`).

What stopped it, in the order found (all `TARGET_PORT`, asm_diff 0 over 3,938 files):

| # | where | cause | fix |
|---|---|---|---|
| A1 | `CGameObjectInstance__Draw` (func_0021D39C), `BuildBoneMatrices` (func_0021CDAC), `PoseBone` (func_0021B95C), `AdvanceAnimation` (func_0021A7EC), `UpdateAnimation` (func_0021A344), func_0021A500, `CAnimationState__GetRootMotion` (`RootActor`, func_00218B10) | instance views at N64 offsets | `tools/port/hostview.py --apply` + hand fixes: ROM addresses/matrix block `sptr`, `m_mfOrient`/`m_vMin` added where the tool found no member, `m_AI.m_vDesiredPos` (the tool's byte delta inside `m_AI` was wrong) |
| A2 | BuildBoneMatrices / PoseBone | the two `PoseContext` views differ on the host (`s32 animIndex` vs `u32 *out`, `arg2`) and the animation handle `*handle` read as s32 | one layout (`structs/pose_host.h` `T2PoseContext`, `_Static_assert`ed against PoseBone's); handle `sptr *` |
| A3 | `CScene__AdvanceInstances` (func_00226490) | `m_pRegistered`/`m_nRegistered` at N64 0xBF8/0xDF8: no instance ever advanced | host views of `CScene`/`CGameObjectInstance` |
| A4 | `CAnimationState__Load` | frame count read as the u16 at +6 of a CUSet header (the low half of the count on BE; the archive swaps words): 0 frames | the s32 count at +4 |
| A5 | `CGameObjectInstance__SetupAnimation` (func_0021A150) | same for `m_nAnims` (u3's find) | same |
| A6 | `Bits_Read`, `Bits_Peek`, `Bits_ReadFloat`, `Bits_PeekFloat`, `BitAddr_*`, func_00218248/002182EC/00218398, `Bits_Poke` | KSEG bit addresses (`seg | offset*8`) overflow above 32 MB of arena (and drop 64-bit pointers): SIGSEGV in `Anim_DecodeTracks` | a bit offset from `gT2BitStreamBase`, set by `BitAddr_FromPointer` (one stream at a time: the decompressor thread). func_00217EFC (an encoder) is not converted |
| A7 | `CAI__UpdateModeAnim` (func_00243170) | `Source110`/`Dest`/`StateEntry` views: `gFrameIncrement *= garbage` for every actor (animations frozen) | host views of `CGameObjectInstance`/`CAI`/`CAIModeInfo` |
| A8 | `CGameObjectInstance__DrawShadow` (func_0021DC84), `SubmitShadowModel` (func_0021ED04), func_00268160 | `CDrawArgs` at N64 offsets; the shadow loaded and "restored" the projection from N64 camera offsets (every actor after a shadow drew with a garbage projection); func_00268160 got the instance through MIPS `$a0` pass-through | `CCamera::m_mfProjection/m_mProjection`, `CDrawArgs` members; the instance passed explicitly. `CDrawArgs.m_hModel` is `sptr` (include change) |
| A9 | `Action_Draw`, `InteractiveAnim_DrawAllModels`, `WeaponActor_Draw` | raw N64 offsets | members |
| A10 | `CGameObjectInstance__ConstructLitVertices`, `Lighting_CollectVolumes`, `Lighting_BuildLitVertices` | model pointer as s32; sectors walked with the 8-byte N64 stride (demo 1 SIGSEGV) | pointer-wide; `m_Sectors[i]` |
| A11 | `CScene__GetLightsAt` | `u8 buf[0x28]` for 10 pointers: overran `CGameObjectInstance__Draw`'s frame (new-game cinema SIGSEGV) | `10 * sizeof(void *)` |
| A12 | `EnemyAI_HeadNodeCallback` (m_pfEffects) | pose context and `m_HeadTurn/m_HeadTilt` at N64 offsets, matrix through an s32 adapter | `T2PoseContext`, members, float `CMtxF__RotateX/Z` |
| A13 | `CGameObjectInstance__FireAnimEvents` (func_0021ABC8) | `ObjectLinks` view | host view |
| A14 | `CPlayerPool__AnimateWeapons` (func_00252ADC), `CPlayerPool__GetViewPlayer` (func_00254B04) | camera word 0x54C (`m_Menus.count`): the weapon actor never animated | by member |

**Off for now:** the animation events and hot points (footstep/splash sounds and particles) end
in `CParticleSystem__Spawn` (func_0022D2FC.c, 1,752 lines, FAKEMATCH, every view at N64
offsets), which corrupts memory on the host. `T2Port_AnimEventsEnabled()` (port/game/t2actors.c)
returns 0 unless `T2_ANIM_EVENTS=1`; FireAnimEvents and GameObject_EmitHotPoints skip their
events. Porting CParticleSystem__Spawn (its `Effect`/`EffectSystem`/`Actor`/`Owner` views, the
`CParticle` host layout) is the next step; weapon fire and impacts reach it too.

Not done: the bosses' draw callbacks (`MantidQueen_Draw` func_00403660, `Primagen_Draw`
func_00407DC0, `MotherBoss_Draw` func_0040BEE4) still use raw offsets; the other m_pfEffects
callbacks (`gWeaponInfoTable[]->effects`, `Actor_PlayRangedSound`, `MotherBoss_PartEffects`)
were not checked against `T2PoseContext`.

## 6. Particles (Phase 7 round 7, v1)

The particle system runs on host types: `CParticleSystem__Spawn` and the rest of the particle
path, so the animation events and hot points are **on by default** (`T2_ANIM_EVENTS=0` turns
them off). Footsteps, splashes, muzzle flashes, sparks and the cinemas' ambient particles
(the Acclaim logo's, the Iguana jungle's) now spawn, move, collide, light and draw (6.4).

### 6.1 The templates: a host copy

The particle-effects unit (cart 4) is an ISet of three blocks: block 0 a pool of sub-records,
block 1 the sorted kinds (u32), block 2 the `CROMParticleEffect` templates (USet, 0x3C each)
whose nine pointer fields are offsets into block 0. The N64 relocates them in place
(`CScene__DecompressParticles`); a 0x3C record cannot hold 8-byte pointers. On the host,
`CScene__DecompressParticles` (and its second-module twin) calls `T2Port_ParticlesBuild`
(port/game/t2particles.c), which builds a host array of `CROMParticleEffect`
(`sizeof` 0x60 on x86_64/arm64) whose pointers point at host copies of the sub-records, and
`CParticleSystem__Spawn` indexes that copy (`T2Port_ParticleEffects`) instead of block 2.
`CParticle.m_pEffect` then points into it, so every typed reader (`m_pEffect->m_pImpact`,
`m_pMotion`, `m_pModel`...) works unchanged. The model field holds the object's ROM address
(`CScene__GetObject`) as a pointer-sized value.

Block 0 stays **big-endian** in the archive (`swap_spec.json` `"block0": "byte"`, honoured by
`swapcheck.py op_particles`): the packer shares byte strings between records of different types
(an impact record's tail is a colour record's head; 3 records hold data of several types), so
no in-place swap is right for every reader. The builder decodes each record by the slot that
points at it. The sub-records are typed in `structs/particle.h` from their readers:
`CPackedRange` (two half floats, base and range: every random value of a template),
`CROMParticleDistance` (size/growth ranges, the shadow scale, the near/far scales),
`CROMParticleOscillation` (the wobble ranges and the opacity ramp), `CROMParticleMotion`
(gravity and speed ranges, four render half floats, acceleration, top speed),
`CROMParticleColors` (two colours and the hue/saturation/value ranges). The old spec swapped
the offset/direction/rotation records as f32, which swapped each range's base with its range.

### 6.2 The views

`func_0022D2FC.c` (BuildMatrix, DrawShadow, DrawSprite, Free, Spawn: a FAKEMATCH file) keeps
its N64 text; every local view has a `TARGET_PORT` twin from `tools/port/hostview.py` (fields
at `__builtin_offsetof` of the real member, in one union): `MtxParticle`, `ShadowActor`,
`Particle`, `EffectActor`, `Effect`, `Actor` -> `CParticle`; `MtxParticleDesc`, `ShadowDef`,
`ParticleDesc`, `Entry`, `SourceModel` -> `CROMParticleEffect`; `MtxCamera`, `ParticleView`,
`Camera` -> `CCamera`; `ParticleHead` -> `CObjectHead`; `EffectList` -> `CList`;
`EffectSystem` -> `CParticleSystem`. By hand: the views the tool cannot place (the Effect's
`unk12C`/`unk15C`/`rot`/`render` spans over runs of CParticle scalars, `_Static_assert`ed
contiguous on the host; `shake` in the anonymous union), the sizes of views used as arrays or
copied (`Entry`, `ParticleHead`, `EffectList`), pointer-wide `resource`/`target`/`unk14`, a
host `aimEffect` (the hit target is a `CGameObjectInstance`: bones, intelligence, AI by
member), the player list head by name (the N64 reads it at `&settings.paused - 0x1414`), the
type-0 owner id (`CStaticObject.m_LitKey`), Free's lists and the firing-ammo flag by member.

The other particle files (hostview or a host body, asm_diff 0): the projectile updates
(`CParticle__Wobble`, `__UpdateHomingMissile`, `__UpdateReturning`, `__UpdateHoming`,
`__Steer`), `__HitTarget`, `__Attach`, `__GetParentTransform` (the camera's face-screen
matrices), `__CheckProximity`, `CParticleSystem__ResolveInteractions`, `__StartEffects`,
`__StopLoopSound` (the loop sound pointer-wide), `__AttachToHitNode` (the bone is the low
byte of the node: byte 0 on a little-endian host, not byte 3), `__GetDistanceScale`,
`__BuildCameraMatrix` (it wrote the matrix at the N64 0x48, over `m_dwFlags`: the draw list
then held freed particles and looped forever), `FreeAll`, `FreeOldest`, `AllocNode`,
func_00232018, func_00232ECC, `CCamera__DrawGlares` (walks the particle draw list).

### 6.3 What the particles reached

A particle moves with `Collision_MoveInstance`, so the sweeps ran on host data for the first
time in the cinemas: `Collision_TestModels` (static objects at the host `CStaticObject` stride,
0xA0), `ModelSweep_TestModel`, `Collision_SweepActors`, `Sweep_TestInstance`, `Sweep_TestBox`,
`Sweep_TestBoxFaces`, func_002682CC, and the two hit sorts (`{pointer, f32}` records: the
comparators read the key at +4 and the swaps moved 8 bytes). `CScene__ClearListCounts` cleared
the scene's per-frame list counts through an N64 view, so on the host `m_nMovers`, `m_nKind1`,
`m_nRegistered`, `m_nDrawActors`... were never reset (the movers list was full of stale and
NULL entries). The dynamic lights: `gDLightBuffer` was 16 bytes on the host (splat labels inside
it); DLights_Load declares it with its size (0x40 lights) on TARGET_PORT for gendata.

### 6.4 Frames and checks

Compared with round 6's binary (no particles at all; `T2_SHOT_EVERY=20`, frames 1..2770): 16 of
138 frames differ, all where particles show. Frame 1421, a cyan ring burst as the Acclaim letters
flip in; 1561-1701, yellow sparks and dark dust puffs around the letters; 2443-2763, the Iguana
jungle's mist sprite and fireflies (yellow points). The rest are pixel-identical. With
`T2_ANIM_EVENTS=0` the same frames are identical to the default: the particles seen in these
cinemas come from their scripted events, not from the animation events (whose particles, kinds
217/218/442 from the logo's hot points, carry a light and a trail).

A display list with sprites (`T2_DLDUMP_AT=2763 T2_DLDUMP_VTX=1`): 18 sprites, each a G_MTX load
of `m_mtxDraw[gEvenOdd]` (world scale/rotation and position, e.g. (-2924.8, 403.5, -925.1),
which the frame's projection puts at NDC (-0.02, 0.17), w 333), the texture, the combiner and
prim colour, one G_VTX of `gParticleSpriteVtx` and one G_TRI2.

Level 51 on the journey spawns no particles (no `CParticleSystem__Spawn` call after frame 11,340):
the first-person player has no footstep hot points, and swimming does not spawn splashes through
this path. Firing (round 7 v2) and the enemies (v3) are the next users.

## 7. Round 17 (part f4): the port against Linux and the N64, the boss bars

Tools and the Mac/Linux comparison: [MACOS.md](MACOS.md) 19 (`shotcmp.py`, 8-bit `T2_SHOT`,
`refshots.py --start-warp`). Mac = Linux up to GPU rounding on edges, apart from the scissor's
last row at the 4/3 scale (MACOS.md 19.2).

### 7.1 Against the N64 (Rice), 8-bit shots

| scene | N64 (emulator frame) / port (VI) | result |
|---|---|---|
| legal screen | 300 / 601 | text peak (200, 200, 138) on the N64, (200, 200, 136) on the port; the port's glyphs are a little wider (three-point filter at 4/3 against Rice's): no bug |
| title | 1500 / 4500 | same layout; mean (43.1, 37.8, 23.3) against (42.8, 37.6, 23.1) (before the 8-bit readback the port's shot read 2-3 levels darker); the differences are edges (filtering, sub-pixel position) |
| demo 1, the first enemy | 2370 (`s_d` 041) / 6253 (the best match of 50 port frames) | same view, HUD and lighting: mean (12.6, 19.0, 18.2) against (12.5, 18.7, 17.9); the enemy's pose differs (demo 1's known divergence, FIDELITY 11.3) |
| the Primagen's lair from `--start-warp 7602@600` / `T2_START_WARP=7602@1400`, gf 75 | 700 / 1601 | **the boss health bar (green, top centre) was missing on the port: fixed (7.2)**; the blue items on the floor near the spawn are drawn on the N64 and not on the port (f3's lead: the 8 static props near the lair spawn, wrong animation); the centre screen's lighting differs a little |
| hub portal 5000 (level 5's cinema) | 700 / 1745 | not comparable by gf: a cinema camera path advances by `osGetTime` (`gFrameIncrement = elapsed`), not by game frames |
| Adon's save room (warp 999, level 97), gf 175 | 800 / 2145 | **match**: Adon, the SAVE / HEALTH / AMMO / GO BACK menu with its highlight, the floor; only the drifting stars differ |
| the hub pad's cut-scene (warp 7601, level 3), gf 75 and 175 | 700 / 1745, 800 / 2145 | **round 19 h2 (FIDELITY.md 11.3): the shaft and the mist are CI4 textures with an IA16 palette (kind 3), which Fast3D decoded as RGBA5551; fixed (port/game/t2tex.c): the shaft is white-yellow and the mist dark grey as on the N64. Not particles (none is live then). The olive walls (brown on the N64) are still open.** Round 17's note: the cameras differ (a cinema), but the colours do not follow: the N64's mist around the creature is brown-grey and its light beam white-yellow, the port's mist is cyan and the beam multicoloured (blue, green, magenta), and the whole scene greener (mean (35.8, 33.1, 23.7) against (34.2, 35.2, 9.6)). The display list (`T2_DLDUMP_AT=2145`) has prim colours 0096FF, 00FFCB, FF00D6, 5100FF from the particle sprites (`CParticle__DrawSprite`); with R and B exchanged they would be the oranges and pinks the N64 shows. Suspects: the spawn colours (`Color_ShiftHue`/`Color_Saturate`/`Color_Darken` on `CROMParticleColors` with the template's hue range), `Particle_ColorMode`'s channel swizzle, or a palette (35 `G_LOADTLUT`s) |

### 7.2 The boss bars (fixed)

`COnScreen` holds two `COverlay`s (the boss bars) and their bosses. On the host `COverlay.m_pScript`
is pointer-wide, so the struct is 0x48 bytes, not 0x3C, and every member after the first overlay
moves. Five places used the N64 offsets:

| function | file | what it did on the host |
|---|---|---|
| `COnScreen__DrawBossBars` | func_0027ADA8.c | tested `m_pBoss` at 0x40 (= `m_BossOverlay.m_Value`, 0 until the bar is drawn) and `m_pBoss2` at 0x88: **no boss bar was ever drawn** |
| `COnScreen__SetBoss2` | func_0027AD54.c | the second bar's boss/name/percent and overlay at N64 offsets |
| `COnScreen__ClearBoss` | func_0027AD98.c | 0 into `m_BossOverlay.m_Value` instead of `m_pBoss`: the bar stayed after the fight |
| `COnScreen__SetBossPercent` | func_0027AD88.c | the percent into padding: the bar never went down |
| the boss-bar reset `func_0042EBBC`, called from func_0042AC0C.c (2 sites) | func_0042EBBC.c | the callers pass `gEngineApp_m_SwooshSystem - 0x1C8` (COnScreen at its N64 size: on the host that is COnScreen + 0x40), and the body used N64 offsets: the second overlay was built over `m_pBoss`, which stayed non-NULL from level 1 on |

Each is a `TARGET_PORT` body by member (the callers pass `&gEngineApp_m_OnScreen`);
`tools/asm_diff.py --base b744bd39` = 0 for the six files. The bars are render-only: the default
run's det log is unchanged (580,000 lines, md5 `ecb9f9f2…`). With the fix the default run shows
the totem defence's two bars (green "Enemies", red "Time"; their percents went 78, 74, 57, 55,
4 over the run), the Blind One's, the Queen's and the Primagen's bars (his seen down to about
two thirds at 240,991), and no bar between fights
(checked: `TMPBOSS` instrumentation, not committed, every change of `m_pBoss`/`m_pBoss2` over the
whole run: set by the totems, Blind, Queen, Primagen; cleared after each).

### 7.3 Next

- The hub pad cut-scene's particle colours (7.1, warp 7601): compare one particle's
  `m_PrimColor`/`m_EnvColor` and its template's colour record with the N64 RAM (`n64trace.py
  --start-warp 7601@600`, f3's `n64cmp.py`) from the same spawn.
- More warps with `refshots.py --start-warp` (every level start the route uses) against
  `T2_START_WARP`: gameplay views are deterministic from the warp for the first frames.
- The Mac window's Metal renderer and the retina scale (MACOS.md 19.3) need a window.

## 8-i1. Round 20 (part i1): the sweep of the hub, levels 1-3 and the menus

Method: N64 `refshots.py --start-warp=W@600 --shots 680:900:20` (shots.txt gives each shot's gf),
port `T2_START_WARP=W@1400 T2_SHOT_EVERY=25 T2_DET_LOG=1 T2_DET_GF=1 --draw` (8-bit shots), paired
by gf and compared with `shotcmp.py --mean --diff`; the attract demo 1 by gf (N64 emulator frame
E = gf E - 1985; the port frames from a run without `--draw`, then drawn only there with
`T2_DRAW_FROM`); the menus and the pause screen with `--input` on the port and `n64trace.py --pad
... --shots` on the N64. Classes: **host bug** (fixed), **timing** (the scene advances by
elapsed time or a shot lands on another frame of a flashing effect), **Rice** (the emulator).

| scene | N64 / port | result | class |
|---|---|---|---|
| title | 1450-1500 / 4362-4500 | same | - |
| main menu | 1600 / 4800 | **SINGLE and MULTI missing on the port** (FIDELITY 13-i1.2); fixed | host bug |
| single-player menu, Enter Cheat keyboard, options | port only (refshots `--input` does not reach the N64 game) | NEW GAME / LOAD missing, the keyboard's A-Z missing; fixed; options as expected | host bug |
| pause menu (START in the hub) | n64trace `--pad` 820 / port 2025 | same layout and highlight | - |
| automap (L in the hub) | n64trace `--pad` 760 / port 1800 | **never drawn on the port** (no G_LINE3D, the index array overran the ortho matrix, black flicker frames); fixed; Rice draws no lines | host bug / Rice |
| the Mother's arena (-4), gf 75 | 700 / 1753 | **olive walls fixed** (packed vertex colour offsets big-endian, FIDELITY 13-i1.1): mean (35.8, 33.1, 23.7) / (35.4, 32.6, 23.2); Turok's pose differs (a cinema on `osGetTime`) | host bug / timing |
| hub (-61), gf 55-275 | 680-900 / 1575-2025 | PSNR 28-31, means within 0.5; the HUD (lives, keys, health) the same | - |
| level 53 (-54) | same | the sky, the water ring, the arches equal; a fire sprite on the right is up on the port at gf 135-175 where the N64 shows a spark (particle 0 spawns a game frame apart: FIDELITY 11.1); the cyan warp ring is a flashing effect: at a gf one side can show it and the other not | timing |
| level 55 (-56), 58 (-59), 59 (-60) | same | the same rooms, light and HUD; the means differ only by the cyan ring's flashes | timing |
| level 125 (-126), 128 (-129), 131 (-132), 133 (-134), 135 (-136) | same | PSNR 26-36, means within 1 | - |
| level 61 (-62), 64 (-65), 67 (-68), 71 (-72) | same | PSNR 25-32 outside the ring's flashes. The ring checked frame by frame in 67 (N64 every 2 emulator frames gf 135-175, port every 10 VIs): it is lit about 3 game frames in every ~22 on both | timing |
| attract demo 1, gf 135-1095 (14 pairs) | 2120-3080 / 5736-8061 | the HUD (health, ammo and its icon), the weapon wheel, the enemies, the corridors and lights match (PSNR 20-31); gf 935: the hurt flash is redder on the port (fades by elapsed time); gf 1015: the enemy's pose (demo 1's known divergence) | timing |

Next: refshots `--input` reaching the N64 game (the menus are port-only shots); the hurt flash's
fade against the N64 frame by frame; the ring's phase (both flash, at different gfs).
## 8-i3. Round 20 (part i3): levels 4-6, the bosses, the endings against the N64; the trails

Method (FIDELITY.md 13-i3 has the commands): for each warp, `refshots.py --start-warp=W@600
--shots 650:1100:25` on the N64 (Rice, 8-bit PNG, `shots.txt` gives each shot's gf) and
`T2_START_WARP=W@1400 T2_DET_LOG=1 T2_DET_GF=1 T2_SHOT_EVERY=2` on the port (Release, `--draw`),
the port's shot kept for each gf that is a multiple of 25; pairs compared by eye and by their
mean RGB. The player stands at the warp's spawn in every scene (no input), so the views are the
same on both sides; enemies move by the random stream, which parts from the N64's within the
first game frames of a level (the particle spawn frame, FIDELITY.md 11.1).

### 8-i3.1 The scenes

| warp (level) | what | result |
|---|---|---|
| -101 (100) | level 4's start, the cave tunnel | **match**; mean (20.7, 30.4, 23.5) N64, (20.4, 30.0, 23.1) port at every gf |
| -104 (103) | the cave with the mushrooms and the totem rock | match; the enemies stand elsewhere (the random stream) |
| -125 (124) | level 5's start (HIVE TOP), the rifle pickup | match (the pickup's spin phase differs) |
| -88 (87), -92 (91), -93 (92) | the hive's tunnels and its hub | **the hive's light shafts (tall pale blue beams that come and go) were missing on the port: a host bug, fixed (8-i3.2)**; the liquid surface that rises and falls matches |
| -115 (114) | level 6's start (the Lightship's hub) | match |
| -118 (117), -121 (120), -124 (123) | the Lightship's green corridors | match but for the side doorway: the N64 shows thin bright green lines there (the room behind), the port a dark doorway with a ring; the port's green is ~4 % brighter (fog). classified Rice, not proven: the lines look like the edges of the geometry behind the door, as Rice draws coverage/fog modes it does not emulate; open lead (8-i3.3) |
| -77 (76), -78 (77) | the talisman chambers of levels 5 and 6 | match |
| -83 (82), -84 (83) | the flesh-eater portals of levels 5 and 6 | match |
| -2 (1) | the Blind One's arena (her spawn) | match |
| -3 (2), -4 (3) | the Mantid Queen's and the Mother's arenas | their intros are cut-scenes on elapsed time (FIDELITY.md 11.1): the cameras part from gf 2, not comparable by gf; the Mother's olive walls are i1's lead |
| -5 (4), 7602 (4) | the Primagen's lair from the level start and from the hub pad | **match**, the boss bar and the blue screens included (round 17's blue items near the spawn are there since round 17 f3's intelligence fix) |
| -11 (10), -12 (11) | the ending levels (END 1/END 2's ship deck, the nebula sky) | match (the sky, the stars, the deck lights) |

The port reads 1-2 % darker than Rice on every non-fogged scene (the round-17 rounding, 7.1).

The two arenas after their intros (N64 `--shots 1200:2600:100`, gf 575-1675; the port to frame
9,500; the player stands still and takes hits):

| warp (level) | result |
|---|---|
| -3 (2), the Mantid Queen | **match**: the arena, her legs, the boss bar, the red damage flash (at other gfs: the hits follow the random stream), the HUD |
| -4 (3), the Mother | the Mother herself, her pose aside (the random stream), matches; **the whole arena is yellow-olive on the port** (floor cream on the N64, yellow on the port; mean RGB at gf 575 N64 (66.9, 58.7, 48.9), port (61.8, 54.5, 30.2): blue 40 % low). **the same cause as i1's walls: fixed on names-R20_i1 (cb236902: `Model_ApplyPackedOffsets` read the big-endian vertex colour offsets with a host load); i1 measured that build against this N64 shot: (59.6, 52.6, 43.5), B/R 0.73 on both** |

### 8-i3.2 The trails (swooshes) were never drawn: three host bugs

The hive's light shafts are the trails of its drip particles (types 226/227, falling at up to 1,024
units/s): `CParticleSystem__Draw` hands each particle with swooshes to `Swoosh_AddParticleNode`,
and `Trail_DrawAll`/`Trail_DrawTextured` draw a ribbon through the points. On the port the
ribbons came out empty (`T2_DLDUMP_AT`: the trail's combiner, colours and texture, then no
vertex):

1. **The swoosh records were left big-endian.** The "particle swooshes" unit (91 records, ROM
   0x1F8249C) had only `unk0`, `sound`, `tex0/1` in `tools/assets/swap_spec.json`; the node life
   (0x08) and the fade time (0x04) stayed N64 bytes, so a new point's life read as 4e-41 and every
   point died the frame it was born (lives 0, -1, -2, -3 in a print), and the blends' u16 point
   indices read as 256 times theirs. The whole record is now typed (f32 unk0/fadeTime/nodeLife,
   s16 layer, the colour and count bytes, then count + 1 blends of u8[4] u8[4] u16 u16):
   coverage 13.6 % to 92.2 %, **a new archive** (sha1 `8eb5b23b…`, `archive_check` OK).
2. **The trail points were untyped data.** `sTrailTextures` points at four tables of points (x, y,
   z and two more f32; `D_800ACE70`, `D_800ACE98`, `D_800ACEC0`, `D_800ACF38`); no C used them by
   name, so gendata emitted their N64 bytes and every point was about 0: a ribbon with no width.
   A `TARGET_PORT` reference in func_002350EC.c types them (gendata's rule, as for
   `gAIBaseModeTable_port`).
3. **Four views of a Swoosh at N64 offsets** (the code reaches the rest of the struct by name):
   `Morph_BlendVertices` and `Morph_BlendVerticesTex` read the fade's `remaining` (Swoosh.time) at
   host 0x30, the owner pointer, so a fading trail took a random alpha; `Trail_GetAnimFrame` read
   the texture frame from `texture[1]`/`id` and **wrote the end frame over `texture[1]`** (the
   trail's second texture handle); `Trail_DrawTextured` and `Trail_PlaceNodes`' emitter kept the
   length in the padding after the host `SwooshLinks`. Each is now a `TARGET_PORT` layout with
   `_Static_assert`s against `structs/swoosh.h`.

With the three, level 91's shafts show as on the N64 (positions follow the random stream, so they
are not at the same place on both sides, but their look, size, colour and frequency match). Every
other trail in the game is affected the same way: weapon and projectile trails, the particle
ribbons of explosions and energy effects all draw now.

**Gameplay**: none. The trails' lifetimes change what a free particle slot keeps (a "matrix built"
flag, 0x100000, left on one slot of level 1 at frame 21,132), so the det lines' particle-pool hash
moves; the RNG, the player and every actor are identical over the whole default run (600,000
lines with the hash taken out) and the `--legacy` run (580,000). New md5s in FIDELITY.md 13-i3.

### 8-i3.3 Next

- The Lightship doorway (117/120/123): dump the N64 display list at that frame (or try
  glide64mk2/angrylion under Xvfb) to tell Rice from a missing pass; the ~4 % greener fog there.
- Weapon fire against the N64: the warps start with the talon only; a scripted weapon change on
  both sides (`--input` / `refshots.py --input`) would compare muzzle flashes and projectile trails.
- The cut-scenes (hub portals, totems, the Queen's and Mother's intros, the ending): both sides
  advance them by elapsed time, so pair them by the cinema's own clock, not by gf.
