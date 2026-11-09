# Animation and skeletons

Tool: `tools/assets/anim.py` (locates through `tools/assets/romfs.py`, unpacks through `propack.py`). `python tools/assets/anim.py baserom.us.z64 --out DIR [--objects 83,105]
[--gltf] [--png --anim N] [--no-keys]` writes per animated object `objNNNN.json` (anim object info,
skeleton, every animation with decoded per-frame keys), optionally `objNNNN.gltf/.bin` (bone node
tree + one glTF animation per clip) and a stick-figure PNG of one clip (front and side view).
Whole ROM: 490 objects, 1857 animations, 0 errors, ~15 s.

## Where the data lives

All animation data hangs off the **objects index** (cartridge index 0xFCBFB4, entry 0; 2705
objects). An object is an ISet of 4 entries (CGameObjectInstance__SetupAnimation,
`CISet__GetROMAddressAndSize(tree, id, n)`):

| entry | name in the C | content | read by |
|---|---|---|---|
| 0 | "anim object info" | raw ISet (below) | SetupAnimation, PickRandomAnim, Draw, AI_TurnHeadToTarget, sweeps |
| 1 | "anim geometry" | RNC; 6-block model, **block 3 = skeleton** | Model_DecompressCallbackExtra, PoseBone |
| 2 | "animations index" | raw ISet in ROM; entry i = one RNC-packed animation | CGameObjectInstance__AdvanceAnimation, CAnimationState__Load |
| 3 | "static events" | 8 bytes | (not this area) |

An object is animated when entry 2 is more than 8 bytes: **490 of 2705**. Static objects have a
32-byte entry 0 (`2, 0x10, 0x18, 0x20` + two 8-byte blocks) that is not an anim object info.

Bytes (ROM, asset region 0x20F178..0x1FF7AEC = 31.3 MB):

- animations (entry 2 of the 490 animated objects): 4,122,312 bytes = **13.1 %** of the region,
- anim object info: 35,600 bytes (0.11 %),
- skeletons: inside the 1,398,632 bytes (4.5 %) of anim geometry, shared with MODELS.

They are not one contiguous range: each object's 4 entries sit together inside the objects index
(ROM 0xFCEA64..0x17618BC), so animation data is interleaved with geometry object by object.
`objNNNN.json` records the ROM offset and size of every entry and every animation.

The engine keys: the actor stores `m_rpObject` / `m_rpObjectHeader` (entry 0 ROM address/size),
`m_rpGeometryHeader` (entry 1) and `animationKey` (0xA8, entry 2 address). The animations index
is requested with size `(count * 4 + 0xF) & ~7` (just the header), count = actor 0xBA = the
action-list count; animation i is then `CISet__GetROMAddressAndSize(index, animationKey, i)`.

## Containers

- ISet: `u32 count; u32 offset[count + 1]` (from the set start; last = end), 8-aligned data.
- USet: `u32 elemSize; u32 count; elements` (CUSet__*; note the order: size first).
- Everything big-endian.

## Anim object info (entry 0, uncompressed ISet of 2)

Block 0, 0x20 bytes:

| off | type | meaning | evidence |
|---|---|---|---|
| 0x00 | f32[3] | bbox min | CMtxF__TransformBox, Draw frustum test, Sweep_* |
| 0x0C | f32[3] | bbox max | |
| 0x18 | s8 | neck bone (-1 none) | AI_TurnHeadToTarget |
| 0x19 | s8 | head bone (-1 none) | " |
| 0x1A | s8 | head-turn bone count | " (divides the turn) |
| 0x1B | u8 | always 0 | |
| 0x1C | u16 | intelligence id | CScene__GetIntelligence (func_00227C10) |
| 0x1E | u16 | a float's top 16 bits (HalfToFloat): 0.25, 0.5, 1.0, 10.2... | func_0021F2B0 |

Block 1: USet of s32 (elemSize 4): the **action id of every animation**, sorted ascending
(CGameObjectInstance__PickRandomAnim binary-searches it and picks at random among equal ids);
`((u16 *)block1)[3]` = count = number of animations. Action ids are a game-wide namespace. The
enemy AI's `sEnemyAnim<Kind>` CEnemyAnimPick tables (rodata 0x800AF6F0.., include/globals/enemy.h)
give the meaning of many: 1-124 Idle, 200s Alert, 300s TakeCover, 2000-2224 Move/Patrol, 2700s
Flee, 4000s Combat, 4100s Leap, 4200s Dart, 4300s Projectile, 4400-4504 Weapon, 5000s ComboEnd,
7000-7042/7400s NormalDeath, 7100s ViolentDeath, 7200s ExplosiveDeath, 7500s MovingDeath,
8000-8124 Evade, 8200-8324 Move, 9600s/9630s left/right arm wound, 11300s joypad test. Within a
group `x0..x4` are variants. The tool labels them (`action_group`); 574 animations get a label.
Unlabeled: 21000s (655, mostly non-enemy actors: doors, pickups, cinema props), 20000s (193),
9000s (101), -1 (109, "no action"), 7000s outside the tables, 2000s (32), 11000/12000/22000/23000/32000s.
Those are probably read by the player/weapon/cinema code (open question).

## Skeleton (anim geometry, block 3)

Anim geometry is RNC-packed; unpacked it is a 6-block ISet (Model_Decompress with withExtra):
0 vertex frames, 1 data, 2 LODs, **3 bones**, 4 hot-point lists (GameObject_EmitHotPoints), 5 ?.
Block 3 is an ISet with one entry per bone (count = bone count, also the matrix-pool size in
BuildBoneMatrices); bone i is an ISet of 2:

- [0] USet of s32: child bone indices (PoseBone recurses over them from bone 0),
- [1] 0x70 bytes:

| off | type | meaning |
|---|---|---|
| 0x00 | f32[8][3] | 8 corners of the bone-space box (Sweep_TestBoxFaces via `node->corners`) |
| 0x60 | f32 | radius (`node->size`, sweep broad phase) |
| 0x64 | u8 | parent bone, 0xFF for the root (checked: agrees with every child list) |
| 0x65 | u8 | part flags (PoseBone cheats: `& 0xC7` == 0xC3/0xC7/0xC2/0xC6 -> big head/hands/feet x2, `& 3 == 1` x3) |
| 0x66 | u8 | 0..9 (mostly 4), unknown |
| 0x67 | u8 | 0 |
| 0x68 | f32 | 0.25 / 0.5 / 1.0 / 10.0 ... unknown |
| 0x6C | u32 | hit mask, ANDed with the actor's bone mask 0x14C (FireAnimEvents) |

Bones per object: 1..115 (average 6.5). The joint count of every animation equals the bone count
of its object (checked on all 1857).

**Pose** (CGameObjectInstance__PoseBone): local = quatToMtx(rotation, translation) as a row-vector
matrix (translation in row 3); `world[bone] = local * world[parent]`, root parent = the object
matrix scaled by 65536; the result is packed into an N64 Mtx (16.16) per bone, so the vertex
loads of anim geometry (`0x40` "matrix load from segment 1" in Model_Decompress) draw each part in
its bone's space. **Models are Z-up** (bbox min z ~ 0, pelvis ~140 units above the root); events
with bone -1 swap y/z when going to object space. For root bone 0 the engine zeroes x and y of the
translation when the actor is visible (root motion is applied to the actor instead,
CAnimationState__GetRootMotion).

## Animation (entry 2 of the object; each entry RNC, method 1)

Unpacked: an ISet of 6 chunks (CAnimationState__Load, FindKeys, GetRootYaw, GetEvents, Anim_Decompress):

| chunk | layout | meaning |
|---|---|---|
| 0 | USet s16 (elemSize 2, count = frames) | root yaw per frame, units pi/32768 (`* 9.587673e-05`); u16 at +6 = frame count (player->frames). Non-zero in 10 animations. |
| 1 | USet {s16 pos_track, s16 rot_track} per bone | track index into chunk 5 (-1: use chunk 2) |
| 2 | USet of 0x14 {f32 pos[3]; s16 rot[4]} per bone | default ("rest") frame: translation, quaternion x,y,z,w / 32767 |
| 3 | ISet of 2: [0] 0x10 bytes, [1] 8 bytes | [0]: u16 blend frames, u16 loop start (0xFFFF: none, then play once/hold), u8 0, u8 blend start, u8 0, u8 blend end, f32 rate (15 = normal), 4 bytes 0. [1]: zeros except in 2 clips |
| 4 | USet of 28-byte events | `{u16 frame; u16 effect (impact event id, gImpactEventHandlers); s16 bone (-1: object space); s16 0; f32 local[3]; s32 params[2]}` fired when time crosses `frame` (CGameObjectInstance__FireAnimEvents). 1029 clips have events, 8017 events, 25 effect ids. |
| 5 | ISet of 3: header, position stream, rotation stream | the compressed tracks, below |

Time advances by `gFrameIncrement * rate / 15` per update; past the last frame it wraps to the
loop start. Blending between clips uses chunk 3's blend fields (cubic ease, AdvanceAnimation).

### Compressed tracks (chunk 5; Anim_DecodeAnimation / Anim_DecodeTracks)

Header (0x20): `s32 radius; s32 frames; s32 encoding; f32 tolerance; s32 rot_tracks; s32 pos_tracks;
f32 frameCount; f32 frameStep` (the last two are filled at load time). Anim_Decompress rebuilds the
resource with chunk 5 replaced by `frames * 16`-byte tracks (one float4 per frame per track, so a
key is `track + frame * 4` floats, which FindKeys' offsets index).

Streams are bit streams read by Bits_Read: **LSB first inside big-endian 32-bit words** (a value
at bit address p is `(word[p>>5] | word[(p>>5)+1] << 32) >> (p & 31)`, masked). For every track,
for every channel (3 for positions, 4 for rotations x,y,z,w): a 31-bit word that is always
**123454321** (0x75BC371, a sync marker, skipped by the engine; the tool checks it: all 103,163
channels match), then the channel in the clip's encoding, `frames` samples:

- **range** (both lossy encodings): positions: two raw f32 (lo, hi); rotations: two 6-bit values
  `v / 63` remapped to [-1, 1] (Math_Remap).
- **encoding 0, cubic** (1810 clips): `6 bits segment count; range; 6 bits width w`; then per segment
  a record of `6 + 4w` bits: 6-bit span n, control values p0..p3 (w bits each). AnimCurve_LoadSegment:
  `s = (hi - lo) / (2^w - 1)`, `k = 1 / (n - 1)` (1 if n < 2),
  `c0 = p0 s + lo`, `c1 = (32p1 - 21p0 - 12p2 + p3)/3 * s k`, `c2 = (14p0 - 32p1 + 20p2 - 2p3) * s k^2`,
  `c3 = (64p1 - 24p0 - 48p2 + 8p3)/3 * s k^3`; value at frame f of the segment starting at f0 is
  `c0 + c1 t + c2 t^2 + c3 t^3`, t = f - f0. Segments follow one another (AnimCurve_Evaluate). If
  w = 0 the channel is constant (lo + range/2). The stream continues after `count * (6 + 4w)` bits.
- **encoding 1, quantized** (1 clip): `range; 6 bits width w`; then w bits per frame, `v / (2^w-1) * (hi-lo) + lo`.
- **encoding 2, raw** (46 clips): one f32 (32 bits) per frame.

Then, rotations only: if radius != 0 (1 clip) a wrapping box blur of +-radius frames, in place per
component; every rotation key is normalized (CQuaternion__Normalize). `tolerance` (0.005 / 0.001 ...)
is the encoder's fitting error, unused by the decoder.

Frames per clip: 1..500 (average 20). Animations per object: 1..65 (115 objects have one).

## Verification

- Sync marker present on every channel of every clip (bit positions exact across 1857 clips).
- Decoded first frames match the chunk 2 default pose closely (e.g. object 83 bone 1 translation
  (0.05, 13.33, 139.53) vs rest (0.05, 13.33, 139.54)).
- Stick-figure renders (Z up): object 83 is a humanoid holding a gun; its clip 2 (action 2010,
  Move) is a walk cycle with alternating legs, clip 18 (action 7000, NormalDeath) falls to the
  ground; object 105 is a tailed biped creature; object 75 (raw encoding) a winged flyer;
  object 77 (115 bones, raw) a large multi-limbed boss whose clip loops (first frame = last).

## For other areas

- MODELS: anim geometry vertices are in bone space; the vertex/matrix commands in the packed
  display list select bone matrices (segment 1 = the bone matrix array, 64 bytes per bone, in
  skeleton order). Pose with `anim.pose(anim, bones, frame)` (world matrices, row-vector, Z-up)
  to place them; without it the mesh collapses at the origin. `anim.parse_skeleton(geom_unpacked)`
  reads block 3.
- LEVELS / cinemas: instances reference objects by object index (CScene__FindObjectType) and pick
  their first animation by action id (`anim > 0`: random clip with that action, else `-anim` = clip index).
- Events' `effect` = impact event / particle handler id (gImpactEventHandlers), i.e. sounds and effects.
- A port can keep the packed format (decoder is small, ~200 lines of C already matched) or
  pre-decode to float4 tracks at extraction (the in-RAM layout Anim_Decompress builds).

## Open questions

- Meaning of anim object info 0x1E (scale-like half float) and bone 0x66 / 0x68.
- Action ids outside the enemy tables (player, weapons, cinema, 20000s/21000s).
- Chunk 3 block 1 (non-zero in two clips: `0x0C, 1, ...`).
- Geometry blocks 4 and 5 (hot points, per-bone attachments) are documented only by name.
- glTF export has no mesh/skin yet (needs the models exporter); nodes carry the first clip's frame 0.
