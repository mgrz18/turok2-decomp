# Byte order (what T2Asset_Read swaps on a little-endian host)

Tools: `tools/assets/swap_spec.json` (the table) and `tools/assets/swapcheck.py` (applies it to the ROM).

```
python tools/assets/swapcheck.py baserom.us.z64 [-v] [--apply] [--type "grid section"] [--plan 0x17618BC] [--json OUT]
```

The archive keeps every load unit in ROM byte order (big-endian, RNC unpacked; PORT.md 5.1 to 5.3).
The port swaps each unit once when the host is little-endian. `swap_spec.json` says which bytes of
each unit type are fields of which width. `swapcheck.py` interprets the spec on all 26,124 units of the
US ROM (through `romfs.load_units()`), gives every byte one class and checks that no byte is claimed
twice and that every container parses. `--apply` swaps every unit, checks that swapping twice gives
the ROM bytes back and that the swap never touches a byte outside a swap field. `--plan KEY` prints one
unit's swap plan. A full run takes 23 s, or 31 s with `--apply`.

## Classes

| class | meaning | on a little-endian host |
|---|---|---|
| swap | 2-, 4- or 8-byte field the CPU reads with a native load | reverse the bytes |
| byte | u8 / s8 / char fields, byte streams read with byte loads | keep |
| rdp | 16-bit texels and TLUTs that Fast3D reads as big-endian **bytes** | keep (see Textures) |
| opaque | VADPCM frames, CI4/CI8/I4 texels: byte-wise decoders | keep |
| pad | alignment, set padding, struct filler the code never reads (`pad*` fields in include/structs) | keep (no reader) |
| guess | a width taken from the data, not from the C (listed below) | as the guessed width |
| unk | the spec reaches it but the width is not known (`unk*` fields, unmapped blocks) | **not covered** |
| none | no rule reaches it | **not covered** (none left) |

## Result (US ROM)

All 47 unit types have a rule. **99.49 %** of the 45,890,836 unit bytes are covered
(swap 51.0 %, opaque 35.9 %, byte 7.0 %, rdp 3.9 %, pad 1.5 %, guess 0.03 %); 0.5 % (about 230 KB) is `unk`.
The swap plan of the whole ROM is 228,911 runs of `(offset, width, count)` plus 11,920 transposed tables.

| type | units | bytes | covered | split |
|---|---|---|---|---|
| audio wave table | 1 | 10,299,008 | 100 % | opaque (VADPCM) |
| geometry | 2705 | 8,065,976 | 99.96 % | swap 67.6, byte 31.7 (command streams, vertex colours) |
| texture | 1839 | 6,840,256 | 100 % | opaque 85.5 (texels), rdp 12.7 (TLUTs), swap 1.3 |
| grid section | 6916 | 5,357,304 | 100 % | swap 84.0, pad 9.4, byte 6.6 |
| animation | 1857 | 5,159,240 | 99.71 % | swap 98.9 (incl. the bit streams as u32) |
| sound data file | 1 | 4,106,144 | 100 % | swap 95.0 (adec streams as u16), byte 4.6 (alCSeq tracks) |
| collision | 165 | 3,162,264 | 94.40 % | `unk` = the unnamed CRegionInfo bytes and table 3 |
| graphicset | 114 | 1,287,064 | 100 % | rdp 71.9 (RGBA16), opaque 26.6 (I4) |
| animated instances | 165 | 238,880 | 100 % | |
| audio control block | 1 | 192,916 | 100 % | bank graph: swap 86.8, pad 8.5 |
| grid bounds, links, sky layers, anim object info, static events, warp dests, persistent bit sections, cinema table, all indexes, small tables | | | 100 % | |
| intelligence | 1 | 132,344 | 96.72 % | `unk`: bytes after some bodies, type 9 body |
| particle effects | 1 | 122,256 | 82.17 % | `unk`: CROMParticleDistance/Motion `unk_00`, the oscillation records |
| Cinema | 197 | 88,272 | 97.50 % | `unk`: clip block 4 (8 bytes) |
| level paths | 165 | 68,152 | 95.66 % | `unk`: group block 0 (8 bytes) |
| particle swooshes | 1 | 8,128 | 13.58 % | only the TrailInfo fields the C names |
| level info | 165 | 3,960 | 58.33 % | `unk`: CLevelHeader `unk_04[6]`, bytes 0x14..0x17 |
| curves | 1 | 272 | 96.32 % | `unk`: type-1 curve bytes 0x0A..0x13 |

## The rules, by family

**Containers.** ISet headers (`count`, `count+1` offsets) and USet headers (`elemSize, count`) are u32.
Index units (the `count*4+15 & ~7` header loads) are all u32.

**Transposed tables** (`CUSet__Transpose`: collision, animated instances, grid sections, model vertices).
The record bytes are stored as planes: byte 0 of every record, then byte 1, and so on. A w-byte field at
record offset o is planes o..o+w-1, so swapping it **reverses those w planes** (a block move, no per-record
work). After `CUSet__Transpose` the records come out in host order. Checked: 4,032 records (grid-section
static objects, actors and light volumes of 3 levels, and the vertices of 3 models) read little-endian
after swap + untranspose equal the big-endian reads of `levels.untranspose` on the ROM bytes.

**Model vertices** are F3DEX2 `Vtx` (s16 ob[3], u16 flag, s16 tc[2], u8 cn[4]), transposed: planes 0..11
are swapped in pairs, planes 12..15 (colour or normal) are kept. Fast3D reads `Vtx` as a host struct.
The packed command stream (`Model_Decompress`) is read byte by byte: no swap.

**Textures: keep the texels and TLUTs big-endian.** libultraship's Fast3D decodes texture memory
from bytes, as the RDP does: `ImportTextureRgba16` and the CI4/CI8 paths read
`(addr[2*i] << 8) | addr[2*i+1]`. Texel and palette data must therefore stay in N64 byte order on every host,
so the `rdp` class is not swapped. Two consequences for Phase 7:
- `Texture_DecompressCallback` recolours the RGBA5551 palettes (request flags, TEXTURES.md) with native u16
  loads and stores. On a little-endian host it must use big-endian accessors (or swap, recolour, swap back).
- Graphic sets: the headers and block sizes are swapped (`u16` grid, `u32` w/h), the RGBA16 texels and
  I4 masks are not. Hud code that reads texels as words would need the same care (none found).

**Animation bit streams: Bits_Read fetches 32-bit words, so the streams are swapped as u32.**
`Bits_Read` (func_00217C58.c) loads `word[0]` and `word[1]` with native 32-bit loads at
`bitAddress >> 3 & ~3` and takes the bits LSB first inside each word (`value >>= shift`,
`next << (32 - shift)`). A u32 swap of the whole stream on a little-endian host gives the same words
and the same bit order, so the decoder runs unchanged. This answers PORT.md's open question: the
position and rotation streams (chunk 5, blocks 1 and 2) are u32 arrays. Note that `Bits_Read` reads one word past
the bit it needs (`word[1]`): a stream that ends exactly at the end of its block reads 4 bytes beyond it.
That is harmless in the cache, but a bounds-checked port must allow for it.

**adec streams** (sound data file, songs 310..614): `adecReadBits` (func_0029FE70.c) fetches **u16 words**
(`*(st->rd++)` on a `u16 *`) and takes bits MSB first, and `adecHuffDecode` reads only through it. The
bitstream is swapped as u16. The 12-byte stream header (`adecStreamOpen`: rate, frames, bytes) is u32.

**alCSeq songs**: the 0x44-byte header (`s32 trackOffset[16], s32 division`) is swapped; the track data
is read with byte loads (`__getTrackByte`): kept.

**VADPCM samples** (audio wave table): 9-byte frames decoded byte-wise: kept. The ADPCM codebooks
(`ALADPCMBook.book`, s16) and loop states (`ALADPCMloop.state`, s16) live in the audio control block
and are swapped with it.

**Audio control block**: a pointer graph (`ALBankFile -> ALBank -> ALInstrument -> ALSound ->
ALEnvelope / ALKeyMap / ALWaveTable -> ALADPCMloop / ALADPCMBook`), pointers pre-relocated to 0x480000.
The spec walks it from the header and swaps every field it reaches (all of them u32/s32/s16 except the
u8 runs). Unreached bytes are alignment (8.5 %). The 64-bit port rebuilds these structures in host
types anyway (PORT.md 9).

**Persistent bit sections** (cart 6/7): ISet headers and the `{u32 bitCount, pad}` section headers are
u32, the bits are read as bytes (`CScene__IsEventFired`): kept.

**Intelligence**: an ISet of records `CIntelligence` (header 0x14) + a body chosen by `m_Type`
(include/structs/intelligence.h), read through `swapcheck.read_cstructs`, which parses the
offset-commented headers, so the C struct definitions are the source of truth. The turret body is the
enemy's first 0x34 bytes + `CTurretIntelligence` from 0x34.

**Particle effects**: block 2 is the `CROMParticleEffect` templates; their 9 offsets point into block 0,
a pool of sub-records (relocated by `CScene__DecompressParticles`). The spec types each pointed record
by the slot it comes from (impact 0xC4, offset 0xC, unk20 0x18, distance 0x24, direction 0xC,
oscillation 0x1C, motion 0x14, colours 0x10, params 0x10).

**C structs used directly**: CRegionInfo, CEmitterDef (sky layers), LevelEvent (links), CinemaEntry,
CLevelHeader, SndEnv, CIntelligence and its bodies, CROMParticleEffect / Impact / Distance / Motion / Params.
The other layouts come from the area docs and the struct.unpack formats of the area tools.

## Guesses (width from the data, not from the C)

- `dynamic lights` (USet 18 x 8): `u16 mask, u16 step, u8 colour[4]` (mask+1 times step is always 0x10000).
- `sound data file` effect cue (8 bytes): taken as u8[8] (the engine keeps it as `u8 *cue`).
- particle sub-records: offset/direction (f32[3]), unk20 (f32[6]), colours (u8[16]).

## Not known yet (`unk`, 0.5 %)

| where | bytes | why it matters |
|---|---|---|
| CRegionInfo `unk_00[0x20]`, `unk_24`, `unk_30`, `unk_40`, `unk_48[8]`, `unk_58..5F` (collision table 0) | ~150 KB | read by region code still unnamed; widths needed before levels work on LE |
| collision table 3 (0x14 records) | small | copied unchanged by CScene__DecompressCollision; reader unknown |
| particle: `CROMParticleDistance.unk_00[0x1C]`, `CROMParticleMotion.unk_00[0x10]`, oscillation records | ~22 KB | some words look like **pairs of half floats** (`3ecc3fcc`), others like f32: must be read in the C (HalfToFloat users) |
| particle swooshes (TrailInfo past 0x21) | 7 KB | only `unk0`, `sound`, `tex0/1` are named |
| intelligence: bytes after some bodies, type 9 (interactive anim) body | 4 KB | |
| level info `unk_04[6]` and bytes 0x14..0x17; cinema clip block 4; path group block 0; geometry block 1 and 5; animation chunk 3 block 1; type-1 curve bytes | < 4 KB | |

The 3 particle records shared by templates of different slot types (offsets 0x0, 0x843C, 0x9DAC of
block 0) hold non-zero data; `swapcheck` reports them as warnings. They need the field widths
above to decide.

## Hazards for Phase 7 (not visible in a width table)

- **Mixed-width access.** A field written as u16 pairs and read as one u32 (or the reverse) swaps
  differently. `REGION_WORD` (structs/region.h) does this, but on the runtime `CGameRegion` that
  `CGameRegion__Unpack` builds field by field, not on asset bytes. Every `(u32 *)` cast over asset data
  found in a loader should be checked against this table.
- **Half floats.** `HalfToFloat` reads u16 halves of f32s (level events' delay, static objects' extents,
  anim object info +0x1E, particle motion). These are u16 fields and are swapped as u16.
- **Prefix reads.** The engine reads prefixes of units (`LoadPersistantIndex` reads 4 bytes, then the
  header; object index probes are 0x18 bytes). Swapping each unit **once, whole, when it is first read**
  (and serving prefixes from that host-order copy) avoids partial-field cases. Swapping per read would
  have to clip runs at the read boundary.
- **Texture recolour** and any CPU access to texels: big-endian accessors (above).

## For the port (next step)

Two ways to run this table in C:

1. **Generated per-type C** (PORT.md 6.3: `swapgen.py` -> `port/t2swap.c`): one function per type,
   written from `swap_spec.json`. Needs a C interpreter of the same rule tree (iset/uset/switch/graph).
2. **Precomputed plans in the archive (recommended).** The builder (mkarchive.py, part b1) already
   walks every unit. It can run `swapcheck` and store each unit's plan (`t2/swap/<key>`: runs of
   `(u32 offset, u8 width, u32 count)` + transposed-table plane reversals; about 1.4 MB for the whole ROM)
   or a per-unit plan offset in the manifest. The port then needs a 30-line generic applier and no
   per-type code. Format knowledge stays in one place (this spec), the spec can be fixed and the archive
   rebuilt without touching the port, and `HOST_ORDER` archives (pre-swapped) remain possible.

Either way, `unk` bytes must be resolved (or proven unread) before the types that hold them are trusted
on a little-endian host: collision (regions), particles, swooshes.
