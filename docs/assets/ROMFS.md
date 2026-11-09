# ROM filesystem (asset region)

Tools: `tools/assets/romfs.py` (tree walker / extractor), `tools/assets/propack.py`
(RNC ProPack methods 1 and 2), `tools/assets/cache_calls.py` (lists every CCache
request call site with its description string).

```
python tools/assets/romfs.py baserom.us.z64 --out OUT --coverage   # OUT/romfs_tree.tsv/.json, romfs_types.tsv
python tools/assets/romfs.py baserom.us.z64 --out OUT --extract    # + OUT/blobs/<path>.bin / .unp.bin
python tools/assets/propack.py FILE --offset 0x17618BC --out x.bin
python tools/assets/cache_calls.py baserom.us.z64 --markdown
```

## Summary

- The asset region really starts at **ROM 0x1E0000**, not 0x20F178: 0x1DEE00-0x1E0000 is
  still MIPS code (VM module), and 0x1E0000-0x20F194 is the audio control block.
  The "913-entry assets_index" at 0x20E334 is **not a directory**: it is the
  `soundArray` of the single instrument of the sfx sound bank (913 `ALSound *`,
  relocated pointers, see "Audio" below). `versions/turok2.us.yaml` names should
  be fixed accordingly (not done here: data-only change for the build owner).
- Four regions, all located from constants in the C, cover 0x1E0000-0x1FF7AEC
  **100 %** (every byte is inside a typed node; 241,320 bytes are ISet headers and
  8-byte alignment padding).
- Every one of the **14,426** RNC blocks in the ROM is a node of the tree, and all
  decompress with the header CRC16 matching (14,261 method 1, 165 method 2; method 2
  is used only for the 165 "level paths"). 16.5 MB packed -> 30.8 MB unpacked.

| ROM | size | region (`romfs` path) | how it is found |
|---|---|---|---|
| 0x01E0000-0x020F194 | 0x2F194 | audio control block (`audioctl`) | `VM_Alloc(0x2F194, 0, 0x1E0000)` in CAudioWorld__Initialize (func_00270240.c) |
| 0x020F194-0x0BE1814 | 0x9D2680 | audio wave table (`audiotbl`) | `gAudioRomBase = 0x20F194` (same function); max(ALWaveTable.base+len) over all sounds = 0x9D2680 exactly |
| 0x0BE1814-0x0FCBFB4 | 0x3EA7A0 | sound data file (`sound`) | `VM_Alloc(0x3EA7A0, 0, 0xBE1814)` in CAudioWorld__LoadSfxDataFile (func_00270550.c) |
| 0x0FCBFB4-0x1FF7AEC | 0x102BB38 | cartridge index (`cart`) | `m_CartridgeIndex = 0xFCBFB4` in CEngineApp__Main (func_00286674.c) -> CScene__Construct |
| 0x1FF7AEC-0x2000000 | | 0xFF padding | |

`VM_Alloc(size, 0, rom)` maps a ROM range into the TLB-paged virtual space; the
audio control block's absolute pointers are relative to virtual **0x480000**
(ROM = ptr - 0x480000 + 0x1E0000; `RomFS.audio_ptr()`).

## Containers (how the C reads them)

**ISet** (CISet__GetBlock 0x2017D4, GetBlockAndSize 0x2017E8, GetBlockSize 0x201818,
GetROMAddress 0x201848, GetROMAddressAndSize 0x20185C, GetOffsetAndSize 0x20188C):

```
+0       u32 count
+4       u32 offset[count + 1]    relative to the set start; offset[count] = set size
...      entry i = [offset[i], offset[i+1])
```
offset[0] = 8 + 4*count rounded up to 8, entries are 8-aligned (a stored block may
have up to 7 bytes of tail padding, which is why a slot can be a little larger than
`18 + packed`). `GetBlock(set, i)` is used on sets in RAM; `GetROMAddress(set, setKey, i)`
= setKey + offset[i] is used on a set whose header was loaded from ROM address setKey,
to make the key of a child **without loading the parent's data**. An empty set is
`00000000 00000008`.

**USet** (CUSet__GetTotalSize 0x2018D8, GetSetAndSize, GetArrayAndSize): `u32 elemSize;
u32 count; count*elemSize bytes` (header names in include/structs/table.h say count/size;
the data shows the first word is the element size, e.g. "object types" = `2, 0xA91`).

**Cache keys.** `CCache__Request(0, key, size, mode, owner, decompressCallback, description, direct)`
(func_0020367C.c): `key` is the ROM offset, hashed into the cache; CLoader__LoadSync/Main
DMA `size` bytes from that PI address (`osPiRawStartDma(0, key, ...)`, chunked), then
the decompress callback (run on the CDecompressor thread) replaces the data.
`CCache__LoadPersistantIndex(0, &out, key, desc)` (func_00205B94.c) reads 4 bytes to get
the count and then the whole ISet header (`count*4 + 15 & ~7` bytes);
`CCache__LoadPersistantSubBlock(0, set, i, setKey, owner, cb, desc)` (func_00205D0C.c)
requests entry i of a set by GetROMAddressAndSize. "Lit vertices" keys
(`id | gLitVerticesKey`) are not ROM blocks: they are RAM-generated from geometry.

**ProPack / RNC** (`ProPackDecompress` func_00200B00.c; `ProPack__Decompress` 0x200AE0
is the callback form): if the first word & ~0xFF == 'RNC\0' the block is unpacked
with RNC_UnpackMethod1 (`hdr+0x12, dst, packed, unpacked`) or RNC_UnpackMethod2
(`hdr, ...`) (hand asm at 0x2010C0..0x2017A8); otherwise the bytes are copied raw.
Header: `'RNC' method, u32 unpacked, u32 packed, u16 crc(unpacked), u16 crc(packed),
u8 leeway, u8 chunks`, stream at +0x12; CRC16 poly 0xA001 (the game does not check it,
propack.py does). propack.py is a port of the differential-tested C of the same
decoders in references/rw_junk/docs/hand_asm_c/rnc.c; verification here = CRC16 of
the output equals the header CRC for all 14,426 blocks. Model/Texture/Anim/Scene
callbacks (Model_DecompressCallback*, Texture_DecompressCallback, Anim_Decompress,
CScene__Decompress*) all start with this RNC step and then post-process (see the
area docs: TEXTURES.md, MODELS.md, LEVELS.md, ANIMATION.md, AUDIO.md).

## The cartridge index (ROM 0xFCBFB4, ISet of 21)

Entry numbers from CScene__Construct (func_00429FA0.c), CScene__FindWarpDests
(func_0042C194.c) and the description strings.

| # | ROM | stored | type (description) | content | callback |
|---|---|---|---|---|---|
| 0 | 0x0FCC014 | 0x7958A8 | objects index | ISet of 2705 objects (see below) | - |
| 1 | 0x17618BC | 0x3D08 | intelligence | RNC1 -> 0x204F8 | CScene__DecompressIntelligence |
| 2 | 0x17655C4 | 0x1530 | object types | USet 2705 x 2 bytes | - |
| 3 | 0x1766AF4 | 0x3CACA0 | texture sets index | ISet of 1839 "texture" blocks (all RNC1) | Texture_DecompressCallback |
| 4 | 0x1B31794 | 0x6028 | particle effects | RNC1 -> 0x1DD90 | CScene__DecompressParticles |
| 5 | 0x1B377BC | 0x43F210 | levels index | ISet of 165 levels (see below) | - |
| 6 | 0x1F769CC | 0x5310 | load/save persistent bit sections | ISet 2: [0] ISet of 165 (per level), [1] ISet 2 | (ROM address only: CScene unk_28/unk_30, gLoadSavePersistentData) |
| 7 | 0x1F7BCDC | 0x4BA0 | game persistent bit sections | ISet 1: ISet of 165 (per level) | (unk_2C/unk_34, gGamePersistentData) |
| 8 | 0x1F8087C | 0x1C10 | warp dests | ISet 2: [0] sorted keys `{?, count, ...}`, [1] placed objects | - |
| 9 | 0x1F8248C | 8 | binaries index | **empty** ISet (no "binary block" exists in this ROM) | ProPack__Decompress |
| 10 | 0x1F82494 | 8 | object types (2nd) | empty (zero) | - |
| 11 | 0x1F8249C | 0x1FC0 | particle swooshes | ISet of 91 raw records | - |
| 12 | 0x1F8445C | 0x98 | dynamic lights | USet 18 x 8 | - |
| 13 | 0x1F844F4 | 0x80 | curves | RNC1 -> 0x110 | ProPack__Decompress |
| 14 | 0x1F84574 | 0x38 | rumbles | RNC1 -> 0x2C | ProPack__Decompress |
| 15 | 0x1F845AC | 0x30 | rumble types | RNC1 -> 0x2C | ProPack__Decompress |
| 16 | 0x1F845DC | 0x59B18 | graphicset index | ISet of 114 "graphicset" blocks (all RNC1) | ProPack__Decompress (CScene__LoadGraphicSet) |
| 17 | 0x1FDE0F4 | 0x1D0 | graphicset type | USet 114 x 4 | - |
| 18 | 0x1FDE2C4 | 0x19678 | Cinemas index | ISet 198: [0] cinema table (0x3A88), [1..197] "Cinema" (raw ISet of 4) | Cinema_PrepareResource |
| 19 | 0x1FF793C | 0x1A8 | link group types | USet 104 x 4 | - |
| 20 | 0x1FF7AE4 | 8 | (end marker, zero; never read) | | |

### Object (objects index entry; 2705)

Requested as "object index" / "stat object index" / "simp object index" / "anim object
index" (0x18 bytes = the ISet header of 4), then by entry:

| entry | type | stored as | read by |
|---|---|---|---|
| 0 | anim object info | raw ISet (bounds / bone info; "anim object info", "player anim object info") | CInstancePool__SpawnAnimInstance, CGameObjectInstance__* (func_0021A150.c: `unkA4 = [0]`) |
| 1 | geometry | RNC1 (2705/2705) | "geometry"/"stat geometry"/"simp geometry"/"anim geometry" via Model_DecompressCallback(Terminated/Extra) |
| 2 | animations index | ISet of "animation" blocks (RNC1; 1857 animations; 490 objects have none) | "animations index", CAnimationState__Load -> Anim_Decompress |
| 3 | static events | raw (usually the empty 8-byte set) | CStaticObject__SpawnEvents |

Objects are addressed by index (`CScene__GetObject`, `CScene__FindObjectType` maps a
type id such as 0xDAC to an index via "object types"). Level instances store the object
index (`CStaticObject__Construct(..., GetROMAddress(m_pObjects, m_ObjectsKey, type))`).

### Level (levels index entry; 165)

CScene__LoadLevel (func_0042AC0C.c): `key = GetROMAddress(m_pLevels, m_LevelsKey, level)`.

| entry | type | stored as | callback |
|---|---|---|---|
| 0 | level info | raw 0x18 | - |
| 1 | collision | RNC1 | CScene__DecompressCollision |
| 2 | level paths | **RNC2** (the only method-2 blocks) | CScene__DecompressPaths |
| 3 | animated instances | RNC1 | CScene__DecompressAnimatedInstances |
| 4 | grid bounds | RNC1 | ProPack__Decompress |
| 5 | grid sections index | ISet of "grid section" (RNC1; 6916 in total, many tiny 8-byte empties) | CScene__DecompressGridSection (key = GetROMAddressAndSize(m_pLevelInfo, m_LevelInfoKey, col + row*width)) |
| 6 | sky layers | RNC1 | ProPack__Decompress |
| 7 | links | RNC1 | CScene__DecompressLinks |
| 8 | level name | raw C string ("STARTMAP(don'tDelete)", "blind boss", "(12) Lev1 F", ...) | CScene__GetLevelName |

The 165 "levels" include cinema stages and boss arenas (names like "cinema 3 J").

### Texture set (1839) and graphicset (114)

Each "texture" block is RNC1; its unpacked data is an ISet of 3: [0] 8-byte header,
[1] ISet of images (frames), [2] ISet of palettes (see TEXTURES.md). Each "graphicset"
is RNC1, unpacked is not an ISet (see TEXTURES.md / LEVELS.md).

## Audio

- **audio control block** 0x1E0000: `u32 2; {ALBankFile *, u32 tblOffset} x 2` =
  `{0x480014, 0}` (music bank: ALBankFile "B1" at ROM 0x1E0014, 1 bank, 33 instruments,
  44100 Hz) and `{0x48AD0C, 0x3F14F8}` (sfx bank at ROM 0x1EAD0C: 1 bank, 1 instrument,
  913 sounds; its soundArray is the table at ROM 0x20E334-0x20F178 that SEGMENTS.md
  called "assets_index"). CAudioWorld__Initialize uses `+4` as `seq.data` and `+0xC`
  as `bankFile` and sorts that instrument's soundArray.
- **wave table** 0x20F194 (`gAudioRomBase`): ALWaveTable.base is relative to it; sfx
  samples are tbl 0x3F14F8-0x9D2680, music below. All 913 sfx wavetables are type 0 (ADPCM).
- **sound data file** 0xBE1814: ISet 2. [0] (0x1A1C8) ISet 6 of sfx tables
  (USet 1319 x 4, USet 1319 x 4, ISet 1319, USet 141, USet 85, USet 6) =
  ranges/weights/variants/... (CAudioWorld__LoadSfxDataFile); [1] (0x3D05C8) ISet 2:
  [0] USet 159, [1] ISet of 318 blocks (~4 MB; speech / streamed audio?, see AUDIO.md).

## Block types and counts (ROM-level leaves)

`romfs.py --coverage` (stored bytes in ROM; `[*]` = heuristically parsed sub-entries of
a raw container, flagged `heuristic=1` in the TSV):

| type | count | stored bytes | RNC | unpacked bytes |
|---|---|---|---|---|
| audio wave table | 1 | 10299008 | 0 | 10299008 |
| animation | 1857 | 4110232 | 1857 | 5159240 |
| sound data file[*] | 2962 | 4078408 | 0 | 4078408 |
| texture | 1839 | 3968984 | 1839 | 6840256 |
| geometry | 2705 | 3607320 | 2705 | 8065976 |
| grid section | 6916 | 2896528 | 6916 | 5357304 |
| collision | 165 | 1240464 | 165 | 3162264 |
| graphicset | 114 | 366920 | 114 | 1287064 |
| audio control block | 1 | 192916 | 0 | 192916 |
| animated instances | 165 | 96944 | 165 | 238880 |
| grid bounds | 165 | 83944 | 165 | 166544 |
| Cinema[*] | 1971 | 71296 | 0 | 71296 |
| anim object info[*] | 5410 | 63200 | 0 | 63200 |
| links | 165 | 41016 | 165 | 120568 |
| level paths | 165 | 36800 | 165 | 68152 |
| particle effects | 1 | 24616 | 1 | 122256 |
| static events | 2705 | 22968 | 0 | 22968 |
| animations index | 2215 | 17720 | 0 | 17720 |
| intelligence | 1 | 15624 | 1 | 132344 |
| cinema table | 1 | 14984 | 0 | 14984 |
| game persistent bit sections[*] | 330 | 13392 | 0 | 13392 |
| sky layers | 165 | 12512 | 165 | 117480 |
| load/save persistent bit sections[*] | 992 | 8680 | 0 | 8680 |
| particle swooshes[*] | 91 | 7752 | 0 | 7752 |
| warp dests[*] | 2 | 7168 | 0 | 7168 |
| object types | 1 | 5424 | 0 | 5424 |
| level info | 165 | 3960 | 0 | 3960 |
| level name | 165 | 2888 | 0 | 2888 |
| graphicset type | 1 | 464 | 0 | 464 |
| link group types | 1 | 424 | 0 | 424 |
| dynamic lights | 1 | 152 | 0 | 152 |
| curves | 1 | 128 | 1 | 272 |
| rumbles | 1 | 56 | 1 | 44 |
| rumble types | 1 | 48 | 1 | 44 |
| binaries index | 1 | 8 | 0 | 8 |
| object types 2 | 1 | 8 | 0 | 8 |
| end marker | 1 | 8 | 0 | 8 |

ISet headers + alignment padding: 241,320 bytes. Accounted: 0x1E17AEC of 0x1E17AEC
bytes (0x1E0000-0x1FF7AEC) = **100 %**; relative to the old "assets" range
0x20F178-0x1FF7AEC also 100 %. Node kinds in the full ROM-level tree: 14,426 RNC,
9,227 ISet, 3,396 USet, 10,099 empty (8-byte empty sets / zero slots), 3,523 raw.

## Every CCache request (call sites)

From `cache_calls.py --markdown` (descriptions given as `D_800Axxxx` resolved from the
ROM; `_2` functions are the copies in the VM modules). 47 distinct descriptions.

| description | call | decompress callback | callers |
|---|---|---|---|
| "anim geometry" | Request | Model_DecompressCallbackExtra | CGameObjectInstance__LoadGeometry, CGameObjectInstance__TouchGeometry |
| "anim object index" | Request | - | CGameObjectInstance__SetupAnimation, CInstancePool__SpawnAnimInstance |
| "anim object info" | Request | - | AI_TurnHeadToTarget, CGameObjectInstance__Draw, CGameObjectInstance__LoadBoneMatrices, CGameObjectInstance__PickRandomAnim, CGameObjectInstance__SetupAnimation, CGameObjectInstance__UpdateAnimation, CInstancePool__SpawnAnimInstance, Sweep_TestBox, func_0021F2B0, func_0021F5B8 |
| "animated instances" | LoadPersistantSubBlock | CScene__DecompressAnimatedInstances, CScene__DecompressAnimatedInstances_2 | CScene__LoadLevel, CScene__LoadLevel_2 |
| "animation" | Request | Anim_Decompress | CAnimationState__Load |
| "animations index" | Request | - | CGameObjectInstance__AdvanceAnimation, CGameObjectInstance__BuildBoneMatrices |
| "binaries index" | LoadPersistantIndex | - | CScene__Construct, CScene__Construct_2 |
| "binary block" | Request | &ProPack__Decompress | CScene__LoadBinaryBlock |
| "cartridge index" | LoadPersistantIndex | - | CScene__Construct, CScene__Construct_2, CScene__FindNearestWarpDest, CScene__FindWarpDests |
| "Cinema" | Request | -, Cinema_PrepareResource, Cinema_PrepareResource_2 | Cinema_Load, Cinema_LoadTable, Cinema_LoadTable_2, Cinema_Load_2, Cinema_RequestById, Cinema_RequestById_2, Cinema_RequestByName, Cinema_RequestByName_2, Cinema_RequestForObject, Cinema_RequestForObject_2 |
| "Cinemas index" | LoadPersistantIndex | - | CScene__Construct, CScene__Construct_2 |
| "collision" | LoadPersistantSubBlock | CScene__DecompressCollision, func_004645D0 | CScene__LoadLevel, CScene__LoadLevel_2 |
| "curves" | LoadPersistantSubBlock | ProPack__Decompress | CScene__Construct, CScene__Construct_2 |
| "dynamic lights" | LoadPersistantSubBlock | - | CScene__Construct, CScene__Construct_2 |
| "geometry" | Request | Model_DecompressCallback, Model_DecompressCallbackTerminated | CScene__ConstructMap, CScene__ConstructMap_2, Render_QueueModelByKey, func_0020B9F4, loadGeomHeader |
| "graphicset index" | LoadPersistantIndex | - | CScene__Construct, CScene__Construct_2 |
| "graphicset type" | LoadPersistantSubBlock | - | CScene__Construct, CScene__Construct_2 |
| "graphicset" | Request | ProPack__Decompress | CScene__LoadGraphicSet |
| "grid bounds" | LoadPersistantSubBlock | ProPack__Decompress | CScene__LoadLevel, CScene__LoadLevel_2 |
| "grid section" | Request | CScene__DecompressGridSection | CScene__LoadVisibleGridSections, CScene__StreamAllSections |
| "grid sections index" | LoadPersistantIndex | - | CScene__LoadLevel, CScene__LoadLevel_2 |
| "intelligence" | LoadPersistantSubBlock | CScene__DecompressIntelligence, CScene__DecompressIntelligence_2 | CScene__Construct, CScene__Construct_2 |
| "level index" | LoadPersistantIndex | - | CScene__GetLevelName, CScene__LoadLevel, CScene__LoadLevel_2 |
| "level info" | LoadPersistantSubBlock | - | CScene__LoadLevel, CScene__LoadLevel_2 |
| "level name" | LoadPersistantSubBlock | - | CScene__GetLevelName |
| "level paths" | LoadPersistantSubBlock | CScene__DecompressPaths, func_00465C24 | CScene__LoadLevel, CScene__LoadLevel_2 |
| "levels index" | LoadPersistantIndex | - | CScene__Construct, CScene__Construct_2 |
| "link group types" | LoadPersistantSubBlock | - | CScene__Construct, CScene__Construct_2 |
| "links" | LoadPersistantSubBlock | CScene__DecompressLinks, CScene__DecompressLinks_2 | CScene__LoadLevel, CScene__LoadLevel_2 |
| "lit vertices" | RequestDecompression | CGameObjectInstance__ConstructLitVertices, CStaticObject__BuildLitVerticesCallback | CGameObjectInstance__DrawModelLitVertices, CStaticObject__Draw, CStaticObject__RequestLitVertices, TotemDefense_DrawTotem |
| "object index" | Request | - | CScene__ConstructMap, CScene__ConstructMap_2, loadGeomHeader |
| "object types" | LoadPersistantSubBlock | - | CScene__Construct, CScene__Construct_2 |
| "objects index" | LoadPersistantIndex | - | CScene__Construct, CScene__Construct_2 |
| "particle effects" | LoadPersistantSubBlock | CScene__DecompressParticles, CScene__DecompressParticles_2 | CScene__Construct, CScene__Construct_2 |
| "particle swooshes" | LoadPersistantSubBlock | - | CScene__Construct, CScene__Construct_2 |
| "player anim object info" | Request | - | Actor_ReloadAnimInfo, CPlayerPool__ReloadAnimInfo |
| "rumble types" | LoadPersistantSubBlock | ProPack__Decompress | CScene__Construct, CScene__Construct_2 |
| "rumbles" | LoadPersistantSubBlock | ProPack__Decompress | CScene__Construct, CScene__Construct_2 |
| "simp geometry" | Request | Model_DecompressCallbackTerminated | CLevelActor__LoadGeometry, CLevelActor__TouchGeometry |
| "simp object index" | Request | - | CLevelActor__LoadGeometry, CLevelActor__TouchGeometry, Render_QueueModelByKey, func_0020B9F4 |
| "sky layers" | LoadPersistantSubBlock | ProPack__Decompress | CScene__LoadLevel, CScene__LoadLevel_2 |
| "stat geometry" | Request | Model_DecompressCallback | CStaticObject__LoadGeometry |
| "stat object index" | Request | - | CStaticObject__LoadGeometry, CStaticObject__SpawnEvents |
| "static events" | Request | - | CStaticObject__SpawnEvents |
| "texture sets index" | LoadPersistantIndex | - | CScene__Construct, CScene__Construct_2 |
| "texture" | Request | Texture_DecompressCallback | Texture_Request, Texture_RequestSync |
| "warp dests" | LoadPersistantSubBlock | - | CScene__FindNearestWarpDest, CScene__FindWarpDests |

## Load units and their readers (for the port archive)

`fs.load_units()` yields every unit the engine reads on its own: each index set's
header (`count*4 + 15 & ~7` bytes) plus each entry of an index as a whole slot (an RNC
stream or a raw container; the sub-entries of a raw container are not units), and the
three audio regions as one unit each. The US ROM has **26,124 units**: 14,426 RNC,
6,438 raw ISet containers (e.g. anim object info, Cinema), 4,885 empty slots, 334 raw,
41 USets. `fs.readers(node)` gives `(description, decompress callback, callers)` from
the `READERS` table in romfs.py (built from the call-site table above); the TSV has a
`readers` column.

Check: **every RNC unit has a reader whose callback unpacks it**. Each callback calls
`ProPackDecompress` (func_00200B00.c): Model_Decompress (func_00209AF4.c, behind
Model_DecompressCallback/Terminated/Extra), Anim_Decompress (func_00216A9C.c),
Texture_DecompressCallback (func_00206990.c), CScene__DecompressIntelligence
(func_002261A8.c), CScene__DecompressParticles (func_0042CAB4.c),
CScene__DecompressCollision (func_0042B6B0.c), CScene__DecompressPaths (func_0042CD04.c),
CScene__DecompressAnimatedInstances (func_0042BA40.c), CScene__DecompressLinks
(func_0042B3E4.c), CScene__DecompressGridSection (func_00220D00.c),
Cinema_PrepareResource (func_0040F240.c), ProPack__Decompress. Raw units are read
without a callback, or with one whose ProPackDecompress takes the non-RNC path, which
copies the data or leaves it alone. The only unit with no reader is cartridge entry 20
(the 8-byte end marker). The persistent bit sections (entries 6/7) are kept as ROM
address+size and read by CLoader__LoadSync, not through CCache.

## romfs.py API (for the other areas)

```python
import sys; sys.path.insert(0, "tools/assets"); import romfs
fs = romfs.RomFS("baserom.us.z64")
fs.regions                      # [Region(name, start, end, desc)] audioctl, audiotbl, sound, cart
n = fs.node("cart/5/12/1")      # Node; also fs.object(i), fs.texture_set(i), fs.level(i),
                                #   fs.graphicset(i), fs.cinema(i) (= cart/18/i+1), fs.level_name(i)
n.path, n.type, n.kind          # kind: iset / uset / rnc / raw / empty
n.rom, n.size                   # ROM offset (None inside a blob: n.container, n.offset)
n.compressed, n.method, n.unpacked_size, n.count, n.heuristic
fs.children(n)                  # ISet entries; for an RNC node the entries of its unpacked ISet
fs.read(n)                      # stored bytes;  fs.blob(n) = after ProPack (raw if not RNC)
fs.walk("cart", decompress=False, maxdepth=None)
fs.count("objects" | "texture_sets" | "levels" | "graphicsets" | "cinemas")
fs.find_rom(addr)               # deepest ROM node containing a ROM address
fs.load_unit(n) -> (key, size)  # the exact CCache key/size (ISet: header only)
fs.load_units()                 # all (key, size, node) ROM load units (for a port archive)
fs.readers(n)                   # [(description, callback, callers)] that request the unit
fs.audio_ptr(vaddr)             # 0x48xxxx pointer of the audio control block -> ROM
propack.decompress(buf, off) / propack.maybe_decompress(buf, off, size) / propack.is_rnc
```

Paths are positional (`cart/0/12/1` = object 12's geometry); types come from the
`SCHEMA` table in romfs.py (patterns -> description string). Nodes under an unknown raw
block that merely *look* like ISets/USets are flagged `heuristic`.

## Open questions

- Inner layouts of the RNC blobs are the area docs' job (textures, models, levels,
  animation, audio). The texture blob header, "anim object info" ISet and the cinema
  ISet-of-4 are only described structurally here.
- Cartridge entries 6/7 (persistent bit sections) are passed by ROM address/size to
  CScene__InitPersistentData / CLoader__LoadSync, not requested through CCache.
- Entry 10 ("object types" 2nd copy) and the binaries index are empty in the US ROM.
- Sound data file [1] (318 blocks, ~4 MB): speech/streamed audio; check in AUDIO.md.
- yaml (done): `virtual_rodata_2` ends at 0x1E0000 and the asset region is four `bin`
  segments, `audio_ctl` / `audio_wavetable` / `sound_data` / `cartridge`; the old
  `assets_index` (the sfx soundArray) and `assets` segments are gone.
