# Levels and world data

Part a4 of Phase 6 (assets). Tools: `tools/assets/levels.py` (levels, warp destinations, maps) and
`tools/assets/cinemas.py` (in-engine cutscenes). Both take the ROM and `--out DIR`; nothing they
write is committed.

    python tools/assets/levels.py baserom.us.z64 --out OUT [--level N]... [--map N]... [--map -1]
    python tools/assets/cinemas.py baserom.us.z64 --out OUT

`levels.py` writes `OUT/levels/NNN_<name>.json` (165 files, about 63 MB), `OUT/levels/index.json`,
`OUT/warp_dests.json` and, with `--map`, top-down PNG maps in `OUT/maps/`. The whole run takes 16 s.
`cinemas.py` writes `OUT/cinemas/index.json` and 197 track files.

The C code is the parser, and every section below names the function it was read from. Struct
names refer to `include/structs/*.h`.

## How the data is located

* **Sets.** Every container is a CISet: `u32 n`, then `n+1` big-endian u32 offsets measured from
  the start of the set (the last one is the end), then the blocks. `CISet__GetROMAddressAndSize`
  (func_0020185C.c) and `CISet__GetBlockAndSize` (func_002017E8.c) work the same way on ROM keys
  and on RAM.
* **Keys are ROM offsets.** `CCache__Request(key, size, ...)` passes `key` unchanged to
  `CLoader__LoadAsync/LoadSync`. `CLoader__Main` (func_00201FBC.c) then calls
  `osPiRawStartDma(0, key, ...)`.
* **Cartridge index at ROM 0xFCBFB4.** `CEngineApp.m_CartridgeIndex = 0xFCBFB4` (func_00286674.c),
  and `CScene__Construct(&gScene, 0xFCBFB4)` reads it. The index has 21 entries and its last block
  ends exactly at 0x1FF7AEC, the end of the asset region. The order matches Turok 3's
  `include/cartdir.h`:

| # | name (description string) | ROM | size |
|---|---|---|---|
| 0 | objects index | 0xFCC014 | 0x7958A8 |
| 1 | intelligence | 0x17618BC | 0x3D08 |
| 2 | object types | 0x17655C4 | 0x1530 |
| 3 | texture sets index | 0x1766AF4 | 0x3CACA0 |
| 4 | particle effects | 0x1B31794 | 0x6028 |
| **5** | **levels index** (165 levels) | **0x1B377BC** | **0x43F210** |
| 6 | load/save persistent data (per-level bit sections) | 0x1F769CC | 0x5310 |
| 7 | game persistent data | 0x1F7BCDC | 0x4BA0 |
| **8** | **warp dests** | **0x1F8087C** | **0x1C10** |
| 9 | binaries index (empty) | 0x1F8248C | 8 |
| 10 | object/binary types (empty) | 0x1F82494 | 8 |
| 11 | particle swooshes | 0x1F8249C | 0x1FC0 |
| 12 | dynamic lights | 0x1F8445C | 0x98 |
| 13 | curves | 0x1F844F4 | 0x80 |
| 14 | rumbles | 0x1F84574 | 0x38 |
| 15 | rumble types | 0x1F845AC | 0x30 |
| 16 | graphicset index (114 images) | 0x1F845DC | 0x59B18 |
| 17 | graphicset type: `{4, 114}` sorted s32 ids, `CScene__LoadGraphicSet` binary-searches them | 0x1FDE0F4 | 0x1D0 |
| **18** | **Cinemas index** (table + 197 tracks) | **0x1FDE2C4** | **0x19678** |
| 19 | link group types | 0x1FF793C | 0x1A8 |
| 20 | (empty) | 0x1FF7AE4 | 8 |

  The cartridge index covers 0xFCBFB4..0x1FF7AEC. Nothing in it points at the 14 MB from
  0x20F178 to 0xFCBFB4 (part a1 / audio).
* **Compression.** `ProPackDecompress` (func_00200B00.c) unpacks a block that starts with `RNC`
  and copies any other block unchanged. Method 1 data begins at header + 0x12; method 2 is given
  the whole header. The Python port follows `references/rw_junk/docs/hand_asm_c/rnc.c`, the same
  hand-written decoder. All 990 RNC blocks of the level set unpack with a matching CRC-16 (header
  +0xC). Both the decoder and the set reader are a temporary copy inside `levels.py`. Once part a1's
  `propack.py` exists, `levels.unpack()` uses it.
* **Transposed tables.** Many tables are `{s32 width, s32 count}` followed by `count*width` bytes
  stored one byte plane at a time: byte 0 of every record, then byte 1 of every record, and so on.
  This layout compresses better. `CUSet__Transpose` (func_0020192C.c) restores record order, and
  `levels.untranspose()` does the same in Python. The rest of this document marks each table as
  *transposed* or *plain*.
* **Coordinates.** Y is up, and the floor plane is X/Z. The camera stands at a player start's
  `pos.y + 204.8`.

## A level (levels index entry, `CScene__LoadLevel`, func_0042AC0C.c / _2 copy func_00463B2C.c)

A level is a set of 9 blocks:

| blk | description string | packing | contents |
|---|---|---|---|
| 0 | "level info" | raw, 0x18 bytes | `CLevelHeader`: f32 scale (1.024 / 1.025), 6 unknown bytes, light colour[3], ambient colour[3], flags (bit 2 = fog, `gFogEnabled`), s8 light dir[3], 4 more bytes |
| 1 | "collision" | RNC, `CScene__DecompressCollision` | the floor regions (below) |
| 2 | "level paths" | RNC, `CScene__DecompressPaths` | path groups (below) |
| 3 | "animated instances" | RNC, `CScene__DecompressAnimatedInstances` | actor placements (below) |
| 4 | "grid bounds" | RNC | the streaming grid (below) |
| 5 | "grid sections index" | raw nested set | one block per grid cell, each RNC, `CScene__DecompressGridSection` |
| 6 | "sky layers" | RNC | `{0x2C, 16}` plain `CEmitterDef` (structs/emitter.h): texture id (-1 = unused), colour, env colour, spin/scale/phase rates, base height, fade depth, camera scale, throttle, alpha, flags. Layer k is active in regions whose `CRegionInfo.m_EmitterMask` has bit k set (`CEmitterPool__Advance`). These are the cloud, fog and ceiling fans. |
| 7 | "links" | RNC, `CScene__DecompressLinks` | the level's event records (below) |
| 8 | (name) | raw ASCII, at most 0x18 bytes | e.g. "blind boss", "(15) Lev1 I", "cinema 5 F" |

Level names: 0 "STARTMAP(don'tDelete)", 1..4 the bosses ("blind boss", "queen", "mother",
"primagen boss"), 5..43 and 155..161 the cinema sets, 44..135 the numbered game levels
"(0)".."(91)", 136..149 the multiplayer arenas (Experimentata, Moonlit Mayhem, ... Madhouse!),
150..154 "15".."19", 162 "Acclaim logo", 163 "ingame deaths", 164 "regeneration". All 165 levels have all 9 blocks.

`CScene__LoadLevel` also maps some `m_LevelID`s (warp ids 0x123A, 0x1BBC...) to "stream all
sections" and to bigger matrix heaps. These are code tables, not assets.

### Block 1, collision → regions (`CScene__DecompressCollision`, func_0042B6B0.c)

After RNC: a set of 4 **transposed** tables.

| # | width | record |
|---|---|---|
| 0 | 0x64 | `CRegionInfo` (structs/region.h): water height @0x20, damage interval @0x28, pos @0x34, flags @0x44 (bit 0 water), damage @0x50, flags52/54, emitter mask @0x56, map colour @0x5C; the rest unnamed |
| 1 | 0x10 | vertex: s32 x, y, z, w; each × 0.029296875 (`UnpackScaledVec4`, func_0026BAE4.c) |
| 2 | 0x14 | packed region (`CGameRegion__Unpack`, func_0026BB30.c): u16 info index, u16 flags (REGION_FLAG_*: 0x40 ceiling, 0x2000 forces default vis), u16 vertex[3], u16 neighbour[3] (0xFFFF = border), s32 vis bits. At runtime it grows to a 0x20 `CGameRegion` with pointers. |
| 3 | 0x14 | a small trailing table (2..18 records), copied unchanged; meaning open |

Elsewhere a "region"/"segment" index (u16, 0xFFFF = none) always means an index into table 2.
Totals: 90,303 regions in 165 levels; the largest is (85) LEV2 F with 3,846.

### Block 4, grid bounds (`CScene__LoadVisibleGridSections`, func_002216DC.c; `CScene__StreamAllSections`)

After RNC: a set of 5 plain tables. Tables 0 and 1 are `{?, nx, f32[nx]}`: the cells' x bounds
(the code reads 0 as max and 1 as min). Tables 2 and 3 hold the z bounds in the same way. Table 4
is `{0x14, nx*nz}` cells: `CRect` (minX, minZ, maxX, maxZ) + s32 empty. Cell (x, z) is grid
section `x + z*nx` in block 5. Empty cells carry -inf bounds. Grids range from 1×1 to 19×9.

### Block 5, grid sections (`CScene__DecompressGridSection`, func_00220D00.c)

There are 6,916 sections, all RNC. Each section is a set of 4 **transposed** tables; an empty cell
is a lone 0 word.

| # | width | record (constructor) |
|---|---|---|
| 0 | 0x1C | compact static object (`CStaticObject__ConstructCompact`, func_002141F0.c): s32 vis, f32 scale, u16 object, u16 region, u16 flags, u16 intelligence, s16 y (×5.12), s8 extents[6] (×10.24), u8 bits (0..3 quarter turns, 0x10 negate x, 0x20 negate z), u8 x, u8 z (× level scale). Rare: 225 records in the whole game. |
| 1 | 0x34 | static object (`CStaticObject__Construct`, func_00214564.c): s32 vis, f32 pos[3], f32 scale[3], u16 half-float extents[6] (`HalfToFloat`), u16 object, u16 region, u16 intelligence, u8 flags (0x40 = fixed ±102.4 box), s8 quaternion[4] (/127) |
| 2 | 0x20 | level actor (`CLevelActor__Construct`, func_002150C0.c): s32 vis, f32 pos[3], f32 radius, u16 object, u16 region, u16 flags, u16 killed index (bit in the per-level persistent bit section, cart 6), u16 intelligence (0xFFFF = generic), s8 angle (×0.02473695 rad) |
| 3 | 0x48 | light volume (`LightVolume_Construct`, func_00211F64.c): s32 pos[3] (×2^-15.35 = 2.384e-5), s32 rot[3] (×2^-27 rad; mode 1: an aim vector), s32 scale[3] (×2.384e-5), s32 width, height (×2×2^-31), byte 0x2F mode, 0x37, 0x3B, u32[3] @0x3C (×2.37e-6) |

"object" is an index into cartridge entry 0 (objects index: geometry, part a3), resolved through
`CISet__GetROMAddress(m_pObjects, m_ObjectsKey, object)`. "intelligence" is an index into cart
entry 1 (`CScene__GetIntelligence`). Totals: 49,391 static objects (incl. 225 compact), 3,409 level actors, 34,047
light volumes.

### Block 3, animated instances (`CScene__DecompressAnimatedInstances`, func_0042BA40.c)

After RNC: one **transposed** table `{0x28, n}`, read by
`CGameObjectInstance__ConstructFromRecord` (func_0021E6AC.c):

| off | type | field |
|---|---|---|
| 0x00 | s32 | unused (-1) |
| 0x04 | f32[3] | position |
| 0x10 | f32[3] | scale |
| 0x1C | u16 | object (model, cart 0) |
| 0x1E | u16 | type (e.g. 100, 1000) |
| 0x20 | u16 | region (0xFFFF none) |
| 0x22 | u16 | intelligence (cart 1); an intelligence whose first word is 0xB marks a player start |
| 0x24 | s16 | rot Y × π/32767 |
| 0x26 | u8 | flags |
| 0x27 | u8 | sound (0xFF none) |

Record 0 is always the player start. The loader also uses its position for
`CScene__FindNearestWarpDest`. There are 5,939 instances in total.

### Block 2, level paths (`CScene__DecompressPaths`, func_0042CD04.c)

After RNC: a plain set of groups. Each group is a set whose block 1 is `{?, n}` + n items of 0x1C
bytes: f32[3] position, s32 region (-1 none; the loader turns it into a region pointer), f32[3]
data, whose first element is wrapped by `Angle_Wrap`. Block 0 of a group is `{?, ?}`, and its
meaning is open. Many levels have an empty paths block (8 bytes, `{0, 8}`).

### Block 7, links → level events (`CScene__DecompressLinks`, func_0042B3E4.c)

After RNC: 4 plain tables in 2 pairs: `{4, n}` s32 sorted keys + `{0x14, n}` `LevelEvent`
records (structs/event.h: actor, sound, music, level, half-float delay, flags @0x0E, key @0x0F,
message @0x10, kind @0x11, handler @0x12, link group @0x13). The first pair is
`CScene.m_pRecords`, the second `m_pAltRecords`. Cinema cues and the `gLevelEventHandlers[kind]`
callbacks fire these records. The loader sets flags 0xF0 when the record's link group bit is set
in the persistent data. Messages index `gLevelMessages`, which is a code-side table (not an asset).

## Warp destinations (cart 8, `CScene__FindWarpDests`, func_0042C194.c)

A set of 2 plain tables: `{?, n, s32 warp_id[n]}`, sorted (BinaryRange), and `{?, n, CWarpPoint[n]}`
(f32 pos[3], f32 rot Y, s16 level index, s16 spawn id). Warp ids are grouped by thousands (999, 1000..1823,
1999..2102, ...; `CScene__LoadLevel` plays a hub announcement for ids 2000/3000/4000/5000/6000).
There are 298 destinations, and their level fields fall in 1..154. `CScene__UpdateWarp` warps to a level through `FindWarpDests(-1, id)`. A negative
id means "level ~id at its start".

## Cinemas (cart 18, `Cinema_Load` etc., func_0040F240.c) → `cinemas.py`

Block 0 is the table `{0x4C, 197}` of `CinemaEntry` (structs/cinema.h): s32 id0 (for cinema
levels it equals the level index the cinema plays in, e.g. Regeneration → 164), s32 id4, s32 id
(script id, `Cinema_RequestById`), u32 flags (1 follow target, 2 follow player, 8 save start),
8 junk bytes, char name[32] (`Cinema_RequestByName`), f32 far clip (/10.24), f32 aspect (2.2),
3 s32.

Block i+1 is the track of cinema i, RNC or raw (`Cinema_PrepareResource`). It is a set of 4:

0. clips: a set of clips. Each clip is a set of 5 plain tables: eye points `{0xC, n}`, eye knots
   `{4, n}`, target points, target knots, 8 bytes. `Cinema_SplineBuild` fits natural cubic
   splines through the points.
1. fade keys `{0x14, n}`: f32 r, g, b, alpha, time.
2. cues `{0xC, n}`: f32 time, s32 list (0 records / 1 alt records), s32 event index into the
   level's links block.
3. FOV keys `{8, n}`: f32 fov, f32 time. Times are in frames (15 per second).

## Not in the asset region

* **Demos.** `Demo_PlayNext` plays `gDemoTable[]`, which lives in the first VM module's `.data`
  (docs/NAMES.md g7).
* **Level messages and text.** `gLevelMessages` and the HUD and menu text are code-side tables
  (include/globals/text.h).

Both are extracted with the code, not here.

## Coverage

This part decodes levels (0x43F210), warp dests (0x1C10) and cinemas (0x19678): 4,565,120 bytes,
or 14.6 % of the asset region 0x20F178..0x1FF7AEC (31.4 MB). Inside levels, every block type is
parsed. Fields still unnamed are listed below. The graphicset type table (cart 17) is described
above; the graphicset images themselves belong to part a2.

## Verification

* All 990 RNC blocks of the level set unpack, and their CRC-16 matches.
* Maps were drawn for (15) Lev1 I, (85) LEV2 F and blind boss. Each shows a coherent floor plan:
  corridors, rooms and water pools (blue, CRegionInfo flag 1), with the border edges outlined.
  Static objects (yellow) line the walls and props. Instances (red) sit on the floors, and the
  player start (white) sits at a corridor end.
* Cinema clip lengths are plausible (1.5–27 frame units). Warp ids are sorted, and every
  destination's level index is in range.

## Open questions

* CLevelHeader bytes 4..9 and 0x14..0x17. Collision table 3 (0x14 records). Paths group block 0
  and item data[1..2]. The cinema `id0`/`id4` and the `unk40` fields.
* Light volume semantics (width/height/mode; mode 1 "aim"), and the 0x2C..0x3B bytes.
* The meaning of a `LevelEvent`'s first word and its key byte. Mapping `kind`/`handler` to
  `gLevelEventHandlers` names.
* The time unit of the cinema spline knots (arc length versus frames).

## For other parts

* **a1 (romfs).** Cartridge index = ROM 0xFCBFB4 (a code constant). Keys are raw ROM offsets.
  The transposed `{width, count}` tables need `CUSet__Transpose`. Block types per level follow the
  table above.
* **a3 (models).** `object` fields in instances, static objects and level actors index cart 0.
  Static object transforms are quat (s8/127) + scale[3] + pos, plus `CMtxF__SwapYZRows`. Regions
  give the floor mesh (vertices × 0.029296875), and collision can be exported as a mesh from
  `collision.vertices/regions`.
* **a2 (textures).** Sky layer `texture` ids. Graphicset ids (cart 17) → images (cart 16, RNC).
* **a5 (animation).** Instances carry the `object` and `intelligence` that select the animated
  model.
