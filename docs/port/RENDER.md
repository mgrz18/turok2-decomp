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
