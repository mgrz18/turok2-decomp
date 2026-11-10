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

## 3. Next: animated instances (actors)

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
