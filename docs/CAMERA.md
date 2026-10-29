# The camera

How Turok 2 moves and draws its cameras, as far as the decomp has read them.
The code is one block of the engine, `0x0027B000`–`0x00283000` (93
functions). Structs: [`include/structs/camera.h`](../include/structs/camera.h)
(`CCamera`, `CCameraPool`, `CShake`, `CCameraText`, `CList`). Names in
*italics* are LibTEngine's for the Turok 3 equivalent; the rest are ours.

Status: **matched** (in `src/`, builds byte for byte), **nonmatching**
(readable C in [`nonmatching/camera/`](../nonmatching/camera/), not yet the
same bytes), **read** (understood, described here, not yet written).

## A frame

1. **The pool** (`func_0027ECF0`, matched) lays out the screen for the number
   of active cameras — one, or a split screen for 2 to 4 players — by moving
   each camera's viewport toward its place (`func_0027EA88`, below), and
   advances the pool's shake sources, returning the finished ones to its
   free list.
2. **Each camera advances** (`func_0027D160`, *Advance*, read), below.
3. **Each camera draws**: `func_0027E000` (*DisplayListSetup*) opens its
   part of the display list, `func_0027E770` (*DrawScene*) draws the world
   through it, `func_0027C1E8` (*DrawTints*) puts the tints over it and
   `func_0027FDE4` (*AdvanceText*) its texts.

## Advance (`func_0027D160`, read, 3.7 KB)

- Finds its player (the `CPlayer` whose `m_pCamera` it is) and adds the
  frame's time step to `m_Time`.
- `func_0027CAB0` (*AdvanceAttributes*, matched): the region the camera is in
  sets the fog colour, far clip and field of view; when the region set
  changes the old values are kept (`m_LastFogColor`, `m_LastFarClip`,
  `m_LastFieldOfView`, `m_LastFogStart`) and blended toward the new ones
  over `m_cRegionSetBlend`. Cinematics override them.
- Runs the camera's **mode**: a table at `D_800B6CB4`, 8 bytes an entry,
  indexed by `m_Mode`, holds each mode's update function (outside
  cinematics).
- Builds the **view**: from `m_ViewParams` (rotation, offsets, position,
  ground quaternion, zoom) it makes the orientation quaternions
  (`m_qViewAlign`, `m_qViewAlignNoZ`) through sin/cos of half angles, plus,
  outside cinematics, the **tremor** of shaking regions (`func_00281D1C`,
  matched) and the **shakes**: the camera's own (`m_Shake`) and, for any
  camera but the main one, every active shake of the pool
  (`func_0027BE90`, matched, per source). The result is `m_mfViewOrient`,
  its inverse, `m_vPos`, `m_vRotation` and `m_mfView`.
- **Under water** (player flag 0x40 at `+0xA1C`, outside cinematics) the
  view wobbles: `m_WaterWobbleXScale/YScale` follow sine waves whose speed
  rises with the player's swimming speed; otherwise the scales are 1.
  `func_00280EAC` (matched) resets the wobble.
- **Projection**: the field of view blends from `m_LastFieldOfView` by
  `m_ViewParams.m_ZoomBlend` (or a cinematic gives it), the aspect ratio
  comes from the viewport, then a perspective matrix (near 4, far
  `m_FarClip`) into `m_mfProjection`, `m_PerspNorm`, `m_Fov`,
  `m_AspectRatio`, and the fixed-point `m_mProjection[frame]`.
- **Look-at** for specular highlights: two directions from the view
  matrix, scaled to bytes, into `m_LookAt[frame]`; in game mode 4 they
  turn with time.
- **View volume**: the frustum's corners (`m_vTCorners`), its six planes
  (`m_ViewVolume`) and the bounding box (`m_ViewMin`/`m_ViewMax`, widened
  into `m_AnimMin`/`m_AnimMax`); `func_0028124C` (matched) tests a point
  against those planes.
- The on-screen (2D) projection `m_mOnScreenProjection[frame]`, when the
  pool asks for one.
- The sky (`func_00282E4C`, on `m_SkySystem`) and the menus
  (`func_004169C8`, on `m_MenuPool`) advance.

## Drawing

| Function | | Status |
|---|---|---|
| `func_0027E000` *DisplayListSetup* | pipe sync and the RDP setup lists; `gSPMatrix` (projection), `gSPPerspNormalize`; the `Vp` (scale = 2×size×warp scale, translation from it) and `gSPViewport`; `gDPSetScissor` clamped to the screen; both `LookAt`s. Then, unless it is the main camera with others active: the z buffer cleared (colour image at the z buffer, fill colour max z, fill rectangle), `G_FOG` on, `gSPFogPosition(m_FogStart, 1000)`, the fog colour (the camera's, black under game flag 0x4000, or a cinematic's) and the view cleared to it | nonmatching |
| `func_0027E770` *DrawScene* | the scene through the camera layer by layer (world, objects, effects, particles, sky, clip ratio, ...), then with game flag 0x200 a fill rectangle over the viewport | matched |
| `func_0027C1E8` *DrawTints* | the fog colour over the view while the player is under water (AI flags 0x880) or a tint is forced (`D_800AE340`, colour `D_800F1FE4`); the screen flash; the red flash of a hit fading at `m_FlashDec` a frame; one more thing in game mode 13 | matched |
| `func_0027F64C` | fill rectangles around the viewport: the borders of a split screen | matched |
| `func_00282340` | draws the sky system (`m_SkySystem`) through the camera, culled against its view | read |
| `func_002803E8` | a glare drawn from a screen point, fading with its distance to the centre (the sun's lens flare) | read |
| `func_00280AA0` | the glares of the objects this camera sees (flag 0x80000), each through `func_002803E8` | matched |
| `func_0027C4E4` | cycles a display list light (`D_800AB0D0`) and the fog colour with the frame counter | matched |
| `func_002812CC` | `gSPMatrix` of the on-screen projection | matched |

## Effects

| Function | | Status |
|---|---|---|
| `func_00281060` / `func_0027C074` / `func_00281054` | **screen flash**: start it (opacity, colour, attack/sustain/decay frames), run its state machine (rise, hold, fade into `m_FlashColor[3]`), set the red hit flash level | matched |
| `func_0028135C` / `func_002811CC` | **shake**: start one of the pool's sources (or reuse the newest) / set the camera's own — position, radius, wave type, a tenth of each amplitude | matched |
| `func_00281D1C` | **tremor** of a region flagged 0x10000: eases toward 0.5° and 0.25 high, random offset each frame | matched |
| `func_00281278` / `func_002812AC` / `func_002812BC` | **warp**: reset it, start mode 1 or 2 | matched |
| `func_0027EA88` | moves the viewport toward a size and a place, 16 pixels a frame | matched |
| `func_00280FD8` | draws the flash over the viewport | matched |
| `func_002810A0` | the fog colour to 5 bits a channel, or black under flag 0x4000 | matched |

## Texts

| Function | | Status |
|---|---|---|
| `func_002817A0` *AddMultiLineText* | a string over a camera, one text per line, centred, 80 pixels above the bottom; texts with the same id go first | matched |
| `func_0027FDE4` *AdvanceText* | stacks the texts from 81% down, draws them, fades them in and out with a quarter sine wave, starts the waiting ones | matched |
| `func_00281AB4` | takes a free text for a camera | matched |
| `func_00281FF8` / `func_002820B4` | allocate / free the pool's texts | matched |

## Other

`func_00281194` (the player whose camera this is), `func_0028130C`
(distance to a point), `func_0028205C` (free the cameras), `func_0027BA8C`
(a 128-entry lookup table from pairs), `func_0027BC34` (a value of the state
at `D_800F56B0`), `func_002810EC` (where a world point falls on the screen),
`func_00281444` (the active camera nearest a point, for sound),
`func_00281518` (average distance from the active cameras): matched.

## Globals met on the way

| Symbol | |
|---|---|
| `D_800D8E24` | the frame's display list position (`include/gfx.h`) |
| `D_80119870` | a game global; the game mode (`D_8011AC88`) is its field at 0x1418 |
| `D_801198B0` | the camera pool; `D_801198F0` its main camera |
| `D_80119890` | the players, a list through `CPlayer.m_pNext` |
| `D_8011AAD8` | game flags (0x200: clear the viewport after the scene; 0x4000: black fog) |
| `D_800B6D28` | the frame's time step |
| `D_800B6D1C` | which of the two frame buffers is being drawn |
| `D_800C2030` / `D_800C2034` | screen width / height |
