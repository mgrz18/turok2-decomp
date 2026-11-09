# Port asset architecture (Phase 6 design)

This document chooses how the native port (Phase 7) gets Turok 2's assets
without shipping any game data, and lists the steps for the two Phase 6 items
that are still open:

- Extractor that builds a resource archive from the user's ROM (the port ships no game data)
- Code loads assets through the archive instead of fixed ROM offsets

It builds on what the decomp already shows about the engine's own loading
code (section 1), on how the existing source ports handle assets (section 2),
and ends with a recommendation (section 4) and a step list (section 6). Format
details of each asset type are in the sibling documents (ROMFS, TEXTURES,
MODELS, LEVELS, ANIMATION, AUDIO); this one is about the plumbing.

Tool: `python tools/assets/romid.py baserom.us.z64 [--out DIR]` normalizes
.z64/.v64/.n64 images, checks the SHA1 and prints the four load roots and the
cartridge index below. The extractor and the port start with the same check.

## TL;DR

- **Turok 2 has no linked-in asset symbols.** The engine is a runtime cache
  over ROM addresses: `CCache__Request(key = ROM address, size, ...)`, with every
  address computed at run time from offset tables (`CISet__GetROMAddress`).
  That puts it in the same family as Banjo-Kazooie's asset table and the
  Rage Wars port, not SM64/SF64/MK64, where Torch replaces C symbols with archive paths.
- **All asset I/O goes through four functions:** `CLoader__LoadSync`,
  `CLoader__LoadAsync` / `CLoader__Main` (the only `osPiRawStartDma` caller),
  and `VM_Alloc` (TLB demand-paged ROM windows). Changing those four, plus one
  line in `CCache__Request`, moves the whole game onto an archive.
- **Recommendation:** use libultraship (Fast3D, O2R archives, controllers,
  audio output, ImGui). The game archive `turok2.o2r` is generated on first
  run from the user's ROM. It holds one entry per **load unit**: the exact
  byte range one engine call site loads, keyed by its ROM address. RNC streams
  are stored already unpacked, while byte order and layout stay as in the ROM.
  The engine's own C keeps doing all the parsing. Byte-order conversion to the
  host happens in one place in the port, `T2Asset_Read`, through per-type swap
  functions written from the format documents.
- **Extractor:** the shipped extractor is a Torch module (`T2:` factories plus an RNC
  decompressor in Torch's `Decompressor`), embedded in the port the way
  Ghostship and Starship embed Torch (ROM picker, SHA1 check, regenerate
  option). The Python in `tools/assets/` stays the specification, the
  development path (`mkarchive.py`, stdlib `zipfile`) and the oracle that
  checks Torch's output byte for byte.
- **Converting to typed libultraship resources** (textures as `OTEX`, display lists as
  `ODLT`) is *not* needed to run the game. It is a later step for texture
  packs and other mods.

## 1. How the engine reads assets (from the C)

### 1.1 One choke point for ROM reads

| function | file | what it does |
|---|---|---|
| `CLoader__LoadAsync(loader, address, length, dest, replyQ, replyMsg, direct)` | `func_00201E94.c` | splits a read into `LOADER_CHUNK_SIZE` (0x2000) entries and queues them; `direct` jams one entry in front |
| `CLoader__LoadSync(loader, address, length, dest)` | `func_00201DBC.c` | the blocking form |
| `CLoader__Main` | `func_00201FBC.c` | the loader thread: `osPiRawStartDma(OS_READ, address, dest, length)` for each entry; **the only PI DMA of asset data** in the game |
| `VM_Alloc(size, unused, romAddr)` | `func_0028E60C.c` | maps a ROM window into TLB-paged virtual memory; `VM_HandleTLBMiss` (`func_0028E068.c`) queues 4 KB page reads, done by `CLoader__LoadSync` in the pager thread (`func_0028E8A4.c`) |

Callers of the loader: `CCache__Request` (every cached asset), the VM pager,
the audio DMA callback (`func_002700D0.c`, 0x300-byte windows into the sample
data), and the persistent-data defaults (`func_0042C418.c`, `func_0042C46C.c`
and their VM-module twins). The other `osPiStartDma` calls
(`func_002A1690.c`, `func_002A1B24.c`) are host-IO debug channels.

### 1.2 The cache is keyed by ROM address

`CCache__Request(unused, key, size, mode, owner, decompressCallback, description, direct)`
(`func_0020367C.c`) hashes `key`, the **ROM address**, and returns the cached
entry if one exists that holds at least `size` bytes. Otherwise it allocates
`size` bytes, loads `size` bytes from ROM `key`, and on completion runs
`decompressCallback(owner, entry)`. The description string (7th argument) is
the asset's type name ("texture", "simp geometry", "grid section", ...). The
callback reshapes the raw block into its runtime form, in place or by
allocating a replacement (`CCache__AllocateMemoryForDecompression` +
`CCache__DeallocAndReplaceCompressedData`).

`CCache__LoadPersistantIndex(…, &out, key, desc)` reads 4 bytes (the count),
then `(count*4 + 0xF) & ~7` bytes: an offset table.
`CCache__LoadPersistantSubBlock(…, table, i, base, …)` requests
`CISet__GetROMAddressAndSize(table, base, i)`.

### 1.3 Sets: addresses are base + offset tables

A *CISet* is `u32 count; u32 offset[count + 1]`, with offsets relative to the
set's start and `offset[count]` its end (`func_002017D4.c` .. `func_002018BC.c`;
Turok 3's `SetN64.c`). The same accessors work on ROM (`GetROMAddress[AndSize]`:
`base + offset[i]`, size `offset[i+1] - offset[i]`) and on loaded data
(`GetBlock[AndSize]`: pointer + offset). A *CUSet* is `u32 count; u32 stride;
data[count*stride]`. **No asset contains an absolute address.** Every reference is
an offset within a set, an index into one of the top-level sets (object type
-> `CISet__GetROMAddress(world->m_pObjects, world->m_ObjectsKey, type)`), or
a segmented GBI address (below).

### 1.4 Where the engine starts

Four ROM addresses are constants in the C. Everything else is reached from them:

| root | ROM range | bytes | reached by |
|---|---|---|---|
| audio window | 0x1E0000-0x20F194 | 192,916 | `CAudioWorld__Initialize`: `VM_Alloc(0x2F194, 0, 0x1E0000)` (bank file, sequence data; holds the 913-entry table at 0x20E334) |
| audio data | 0x20F194-0xBE1814 | 10,299,008 | `gAudioRomBase = 0x20F194`; sample reads by the audio DMA callback |
| sound data | 0xBE1814-0xFCBFB4 | 4,106,144 | `CAudioWorld__LoadSfxDataFile`: `VM_Alloc(0x3EA7A0, 0, 0xBE1814)` |
| cartridge | 0xFCBFB4-0x1FF7AEC | 16,956,216 | `m_CartridgeIndex = 0xFCBFB4` (`func_00286674.c`) -> `CScene__Construct` (`func_00429FA0.c`) |

The cartridge index is a 21-entry CISet that ends exactly at the end of the
asset region (0x1FF7AEC). `CScene__Construct` names its children: 0 objects
index, 1 intelligence, 2 object types, 3 texture sets index, 4 particle
effects, 5 levels index, 6/7 persistent-data defaults (read with
`CLoader__LoadSync` straight into `gLoadSavePersistentData` /
`gGamePersistentData`), 8 warp dests, 9 binaries index, 10 object types 2,
11 swooshes, 12 dynamic lights, 13 curves, 14 rumbles, 15 rumble types,
16 graphicset index, 17 graphicset type, 18 cinemas index, 19 link group
types, 20 empty. The level file (`CScene__LoadLevel`, `func_0042AC0C.c`) is
itself a set: info, collision, paths, animated instances, grid bounds, links,
sky layers, plus a grid-sections index.

So the four roots cover the whole asset region (the 28 bytes at
0x20F178-0x20F194 precede `gAudioRomBase` and are read through the audio
window).

### 1.5 Compression

RNC ProPack (`"RNC"` + method byte, u32 unpacked size, u32 packed size, 18-byte
header). The cartridge holds **14,426 RNC streams** (ROMFS.md; all unpack
with the header CRC16 checked by `tools/assets/propack.py`): 14,261 of method 1,
16.4 MB packed -> 30.8 MB unpacked, so 96.8% of the cartridge's bytes are inside
RNC streams; and 165 of method 2, one "level paths" block per level. The audio
and sound roots have none (audio has its own codec; see AUDIO.md).

`ProPackDecompress(handle, inPlace)` (`func_00200B00.c`) unpacks when the
block starts with `RNC` and **passes any other block through** (a copy or
nothing). This matters: a block that is stored unpacked runs through every
decompress callback unchanged. The two unpackers (`RNC_UnpackMethod1/2`) are
hand-written assembly. Unpacking at extraction time means the host never needs
them.

### 1.6 What the loaders build at run time

The callbacks turn packed data into runtime structures. Two consequences for
the port, both code-side (Phase 7) rather than archive-side:

- **Display lists are generated, not stored.** `Model_Decompress`
  (`func_00209AF4.c`) expands a packed command stream (0x80 triangles,
  0x20 vertex load, 0x40 matrix, 0x60 moveword) into F3DEX2 commands whose
  addresses are *segmented* (segment 1 for matrices, segment 2 for vertices).
  libultraship's Fast3D interprets F3DEX2 (`GBI_UCODE=F3DEX_GBI_2`) and
  resolves segments, so nothing has to be converted for the renderer. But
  libultraship's `Gfx` is two `uintptr_t` (16 bytes on 64-bit hosts), and the
  loaders size their output as 8 bytes per command (`len += 8`), so those sizes
  must become `sizeof(Gfx)`. Texture commands built at draw time point at cache
  memory (raw pointers), which Fast3D also accepts.
- **Runtime records live in cache blocks.** For example, `CScene__DecompressGridSection`
  (`func_00220D00.c`) builds 0x90-, 0xC0- and `CLevelActor`-sized records
  with embedded pointers inside a cache block sized from N64 struct sizes. On
  a 64-bit host those sizes change. This is a Phase 7 "64-bit clean" item,
  independent of how the bytes arrive.

## 2. How the source ports do it

### 2.1 libultraship (Kenix3/libultraship)

- **Archives:** `O2R` (a ZIP, the modern default), `OTR` (MPQ, legacy,
  `INCLUDE_MPQ_SUPPORT`), and `FolderArchive` (a plain directory, for
  development). `ArchiveManager` mounts several archives at once. Files are
  addressed by virtual path or by its 64-bit CRC, so a later archive (a mod)
  can shadow an entry by path.
- **Resources:** every file starts with a 64-byte header (byte order, "is
  custom" flag, u32 resource type magic, u32 version, u64 id), followed by
  type-specific data. `ResourceManager` keeps a cache and dispatches to
  `ResourceFactoryBinary` subclasses registered per type magic (built-in:
  Blob, Json, Shader; Fast3D adds Texture, DisplayList, Vertex, Matrix,
  Light, ...). Game C reaches them through the bridge
  (`src/libultraship/bridge/resourcebridge.cpp`): `ResourceLoad(name)`,
  `ResourceGetDataByName/ByCrc`, `ResourceGetSizeByName`,
  `ResourceLoadDirectoryAsync`, `ResourceUnloadByName`, ...
- **Fast3D** interprets display lists at run time. It adds port opcodes for
  archive references: `G_SETTIMG_OTR_FILEPATH/HASH`, `G_VTX_OTR_*`,
  `G_DL_OTR_*`, `G_MARKER`. A texture pointer that is a string starting with
  `"__OTR__"` is resolved as an archive path. `gSPInvalidateTexCache` drops a
  cached texture by address.
- `docs/PORTING.md` names two asset routes: "Option A" O2R through Torch
  (recommended), and "Option B" raw assets with endianness handled by the port.
  It also says O2R is not required to start a port.

### 2.2 Torch (HarbourMasters/Torch)

- A C++ asset processor. `config.yml` is keyed by the **ROM SHA1**. Each
  version names its YAML directory, GBI variant, output files (`binary: x.o2r`,
  `headers`, `code`, `modding`) and segment table. A `preprocess` step can
  turn one hash into another (SF64: `decompress_mio0`, then the decompressed
  ROM's hash selects the real config).
- YAML entries are `name: {type, offset, size, symbol, ...}` and dispatch to
  factories: generic (`BLOB`, `TEXTURE`, `VTX`, `GFX`, `MTX`, `LIGHTS`,
  `ARRAY`, `ASSET_ARRAY`, ...) and per game (`SM64:*`, `SF64:*`, `MK64:*`,
  `PM64:*`, `FZX:*`, `BK64:*`, `NAUDIO:*`). Every factory has exporters for the
  modes `code` (C/headers for the decomp), `binary` (the O2R entry) and
  `modding`.
- `Decompressor` knows MIO0, Yay0/Yay1, Yaz0 and BK's zip, but not RNC.
- The nearest analogue to Turok 2 is `BK64:ASSET_TABLE`: one factory walks a
  table of (offset, compression flag, type) entries instead of listing every
  asset in YAML.
- The ports link Torch as a library (`Companion`) and run it at first start.

### 2.3 Ghostship (SM64), Starship (SF64), SpaghettiKart (MK64)

- **Two archives.** A port archive (`ghostship.o2r`: fonts, shaders, UI
  textures, all made by the project) is shipped. A game archive (`sm64.o2r` /
  `sf64.o2r`) is generated from the user's ROM.
- **`GameExtractor`** (`src/port/GameExtractor.cpp`) looks for a ROM or opens a
  file dialog (portable-file-dialogs; `baserom.us.z64` next to the app on
  mobile), SHA1-checks it against a game list, and runs Torch's `Companion` to
  write the O2R. A "Regenerate Game Assets" menu item drops a marker so
  extraction runs again on the next start. The archives carry `version` (ROM
  identity) and `portVersion` entries. A mismatch sends the user back through
  extraction.
- **Code side.** Those decomps had their assets as C data. Torch's `code` mode
  regenerates the headers so every asset symbol becomes
  `static const char sFoo[] = "__OTR__assets/...";`. Display lists that
  reference such a "pointer" resolve through Fast3D, and C that needs bytes calls
  `LOAD_ASSET(x)` -> `ResourceGetDataByName`. The game code barely changes,
  because the assets were never loaded by the game itself.
- SpaghettiKart has the same layout, with MK64-specific factories
  (`MK64:PACKED_GFX`, course data).

### 2.4 2 Ship 2 Harkinian (MM)

ZAPDTR + OTRExporter, the XML-driven extractor inherited from Ship of
Harkinian, which predates Torch. Same runtime model (O2R, `__OTR__` paths). It
is the legacy route. New ports use Torch.

### 2.5 Turok: Rage Wars Windows port (references/rw_junk/port)

The same engine family (`docs/PORT_PLAN.md`, `docs/PORT_STATUS.md`):

- **ROM at run time:** `port/rom_find.c` finds the user's ROM (env var,
  working directory, next to the exe, data folder, any 8 MiB
  `.z64/.n64/.v64`). It normalizes byte order, checks the SHA1 against a list
  that names the unsupported releases, asks with a file dialog when nothing
  fits, and copies the accepted image into the data folder so it asks only once.
- **Data stays big-endian:** `port/be_ir.py` rewrites every load and store in
  the compiled LLVM IR into a byte-swapped access. The C needs no changes and
  the ROM bytes are used as is.
- **32-bit x86, guest address == host address** (globals at their N64
  addresses). PI DMA becomes a copy from the ROM image. The TLB demand paging
  is kept through an access-violation handler that loads the faulting 4 KB
  page.
- Its own D3D11 F3DEX2 renderer; libaudio runs with an ABI HLE
  (`port/audio_hle.c`).

This is the fastest route to a running game, and it proves the engine works
off unmodified ROM data. But it is tied to 32-bit x86, a custom IR rewriter
and Windows exception handling, and it leaves no room for mods
(`PORT_PLAN.md`'s own step 4 wants "loaders reading from extracted files ...
so assets can be replaced by modders").

## 3. What differs for Turok 2

1. **No asset symbols to replace.** Torch's main trick (symbol -> `__OTR__`
   path) has nothing to act on. Turok 2's code asks for *ROM addresses* that
   it reads out of other assets.
2. **Identity is the ROM address.** Every reference chain starts at the four
   roots and goes through offset tables inside the data. An archive keyed by
   anything else needs a translation table. One keyed by ROM address needs
   none.
3. **The data is offset-based and has no absolute pointers**, so it survives
   64-bit hosts. The 64-bit problems are in the runtime structures the loaders
   build (1.6), not in the stored bytes.
4. **Display lists are generated at load time** with segmented addresses, so
   there are no stored GBI streams to convert. Fast3D runs them as they are.
5. **Big-endian.** The engine reads fields with native loads, so a
   little-endian host needs the bytes swapped per field. Only the format
   knowledge in the area documents can do that. Bit streams and byte arrays
   (animation bits, 8-bit textures, ADPCM) must *not* be swapped.

## 4. Options and recommendation

| option | engine change | extractor | mods | verdict |
|---|---|---|---|---|
| A. ROM at run time (Rage Wars) | CLoader reads the ROM file; RNC unpacked by the engine; BE via an IR rewriter or a 32-bit guest | none | none | bring-up only |
| B. Full Torch conversion (every texture/model/level an LUS resource with its own path) | rewrite every loader to request paths; replace the cache | large: a factory per format | best | too invasive: throws away the matched loaders |
| **C. O2R of load units keyed by ROM address** | 4 functions + 1 line (5.4) | small: tree walk, RNC, zip | replace any unit; typed overlays later | **recommended** |
| D. Own archive format | same as C | same as C | own tools | no gain over O2R; loses libultraship's ArchiveManager, mod stacking, tools |

**Recommendation: C.** It keeps the matched engine code (cache, sets,
decompress callbacks) as the parser. It is exactly the "smallest code change"
(swap the PI DMA for archive reads). It works with libultraship's archive
stack (mod O2Rs shadow game entries) and needs no per-asset YAML. Typed
resources (option B) can be layered on later for single asset kinds without
changing the scheme (section 7).

## 5. Design

### 5.1 Archives

| archive | content | shipped? |
|---|---|---|
| `t2port.o2r` | port-made data: fonts, shaders, UI art, default configs | yes (no game data) |
| `turok2.o2r` | generated from the user's ROM at first run | **never** |
| `mods/*.o2r` | user mods; mounted after `turok2.o2r`, so entries with the same path win | user's |

`turok2.o2r` entries:

| path | content |
|---|---|
| `version` | ROM SHA1 (20 bytes) + extractor format version (u16 major, u16 minor) |
| `portVersion` | port build that wrote it (Ghostship convention) |
| `t2/units` | the manifest: sorted array of `{u32 romAddr, u32 romSize, u32 hostSize, u16 type, u16 flags}` |
| `t2/types` | type id -> description string ("texture", "grid section", ... from the `CCache__Request` strings) |
| `t2/u/XXXXXXX` | one load unit, `XXXXXXX` = ROM address in hex (7 digits) |

Unit entry = the standard 64-byte libultraship header (resource type
`'T2LU'`, version 0) + `romAddr, romSize, hostSize, type, flags` + the bytes.
Flags: `RNC_UNPACKED` (stored unpacked; `romSize` is the packed size),
`RAW_RANGE` (a raw byte range that may be read at any inner address: the
audio data and sound data roots), `HOST_ORDER` (already swapped; for mod tools
that prefer little-endian), `KEEP_PACKED` (stored as in the ROM; the engine
unpacks it).

A FolderArchive with the same tree (`t2/u/0FCC014`, ...) works for
development and for mods that are just a dropped-in file.

### 5.2 What a load unit is

A load unit is a (ROM address, length) pair that some engine call site
passes to the loader:

- every distinct `key` of `CCache__Request`, with the largest `size` any call
  site uses for it (smaller requests, such as the 0x18-byte "object index"
  probes and the 4-byte count probe of `CCache__LoadPersistantIndex`, read a
  prefix);
- the persistent-data defaults (cart 6, 7) read by `CLoader__LoadSync`;
- the two `VM_Alloc` windows and the audio sample range (`RAW_RANGE`).

The tree walker (`tools/assets/romfs.py`, part a1) enumerates units by
following the same sets the engine follows, starting at the roots in 1.4.
The call-site table (which description string loads which set level) tells
it where a set is a unit and where it is data *inside* a unit. A unit is stored
unpacked only when (a) it is exactly one RNC stream and (b) every call site
that reads it goes through a ProPack-family callback (`ProPack__Decompress`,
`Model_Decompress*`, `Anim_Decompress`, `CScene__Decompress*`,
`Texture_DecompressCallback`, ...). Then the bytes the engine sees after the
callback are identical, because `ProPackDecompress` passes unpacked data
through. A raw container whose children are RNC streams that the engine unpacks
from memory stays `KEEP_PACKED`, unless the walker also rewrites the container's
offset table, which is legitimate because the engine reads those offsets from
the loaded block. Either way the port must then carry a C version of the RNC
unpacker (`tools/assets/propack.py` is the reference; Rage Wars has `rncUnpack` / `tools/rnc.py`).

### 5.3 Byte order: one swap point

`T2Asset_Read` returns host-order bytes. Conceptually:

```
T2Asset_Read(key, offset, dst, len):
    unit = find(key)                    # exact key; RAW_RANGE units by containment
    copy unit.bytes[offset : offset+len] to dst (zero-fill past hostSize)
    if not (unit.flags & HOST_ORDER): t2_swap[unit.type](dst, offset, len)
```

`t2_swap[type]` is a C function per format, generated or written from the
area documents' field tables (u8 / s16 / u32 / f32 runs, CISet tables are all
u32). It must handle reads of a prefix (the probes) by swapping only whole
fields inside `[offset, offset+len)`. Rules learned so far:

- CISet / CUSet headers and offset tables: u32.
- 8-bit pixels and CI indices, bit streams (`Bits_Read`, animation channels),
  ADPCM and Acclaim audio frames: no swap. Check whether `Bits_Read` fetches
  bytes or words: if it reads words, the swap of those blocks is u32
  (ANIMATION.md).
- Model vertex frames are stored byte-transposed (`CUSet__Transpose` in
  `Model_Decompress`), so swapping them is "un-transpose, swap the `Vtx` s16
  fields, re-transpose". That is still a pure byte permutation, so it can
  happen at read time or at extraction time.
- RGBA16 textures: u16 per texel. `Texture_DecompressCallback` recolours them
  as u16.

Why swap at read time rather than extraction time: the archive stays a
faithful (unpacked) copy of the ROM, the extractor needs no format knowledge,
the same functions serve the bring-up ROM backend (5.6), and a missing or
wrong swap table is a port fix rather than a re-extraction. The cost is small:
one pass over a block when it enters the cache, at most 30 MB in a whole
playthrough. Units tagged `HOST_ORDER` skip it, so pre-swapping at extraction
stays possible later.

Unknown types: the walker tags them `unknown:<description>`, and their swap
function is a no-op that logs once, so missing coverage shows in the log.

### 5.4 Engine changes (Phase 7, all in a few files)

| function | change |
|---|---|
| `CLoader__LoadSync(l, address, length, dest)` | `T2Asset_Read(address, 0, dest, length)` |
| `CLoader__LoadAsync(...)` | queue **one** entry per request (no 0x2000 chunking: chunk addresses `key + k*0x2000` would leave an unpacked unit's ROM range); keep the queue and the reply message so the cache's async protocol is untouched |
| `CLoader__Main` | `T2Asset_Read(entry->address, 0, entry->dest, entry->length)` instead of `osPiRawStartDma`; drop the cache writeback/invalidate |
| `VM_Alloc(size, _, romAddr)` | return the unit's bytes (load whole, swap once, keep resident); the pager (`VM_HandleTLBMiss`, `VM_Age`, `VM_MapTLBEntry`) is not built |
| `CCache__Request` | before allocating: `size = T2Asset_HostSize(key, size)`. If a unit starts at `key` and `size >= romSize`, the result is `hostSize`; otherwise `size` (a prefix probe) |
| `CCache__FreeMemEntry` / heap compaction (`CCache__Compact`) | `gSPInvalidateTexCache(block)` (or the Fast3D call behind it) for freed or moved blocks, since Fast3D caches textures by address and the cache reuses memory |
| audio DMA callback (`func_002700D0.c`) | unchanged: it calls `CLoader__LoadAsync(direct = 1)` with addresses inside the `RAW_RANGE` audio unit |

`T2Asset_*` lives in `port/t2asset.cpp` on top of
`Ship::Context::GetInstance()->GetResourceManager()->GetArchiveManager()`.
At start it loads `t2/units` into a sorted array (binary search by address)
and reads unit files from the ArchiveManager on demand. The engine's cache
stays the real cache: `T2Asset_Read` does not keep `ResourceManager` copies
of `T2LU` entries, except the resident VM windows.

The rest of the C keeps asking for ROM addresses. The phrase "Code loads
assets through the archive instead of fixed ROM offsets" is therefore met at
the I/O layer. The four root constants (1.4) can stay or become named
constants (`T2_ROOT_CARTRIDGE`, ...) checked against the manifest at start.

### 5.5 Extractor

Shipping extractor: **Torch with a `T2` module**, embedded in the port.

- `config.yml`: one entry keyed by `fb0400f21e3f043939ab56500c7b12a3231006f1`,
  `output.binary: turok2.o2r`, path `assets/yaml/us`.
- YAML: a handful of entries rather than thousands. `T2:CARTRIDGE` at
  0xFCBFB4 walks the cartridge tree. `T2:RAW_RANGE` covers 0x1E0000+0x2F194,
  0x20F194-0xBE1814 and 0xBE1814+0x3EA7A0. The factory, like
  `BK64:ASSET_TABLE`, emits all units itself.
- `Decompressor`: add `CompressionType::RNC` (ProPack methods 1 and 2), ported from
  `tools/assets/propack.py`, which is itself verified against the game's unpacker.
- Exporters: `binary` only (the `T2LU` entries + `t2/units`, `t2/types`,
  `version`). No `code` mode: the decomp keeps no asset C.
- In the port: a `GameExtractor` like Ghostship's. Search order: `TUROK2_ROM`
  env var, `baserom.us.z64` in the working directory / app directory, any
  32 MiB `.z64/.v64/.n64` there (found by content); then a file dialog.
  Normalize byte order, check the SHA1, explain a wrong file ("Turok 2 (Europe)
  / (Japan) / not an N64 ROM / another game"), write `turok2.o2r` into the app
  data directory, and keep a "Regenerate game assets" menu item. The ROM
  itself is not copied (Rage Wars copies it, but with an archive the ROM is
  needed only once).

Development path: `tools/assets/mkarchive.py baserom.us.z64 --out DIR`
(Python stdlib `zipfile`) writes the same `turok2.o2r`, or with `--folder`
the FolderArchive tree. A CI check (`tools/assets/archive_check.py`) runs both
and requires identical manifests and unit bytes.

### 5.6 Bring-up backend (optional)

Before the extractor exists, `T2Asset_Read` can sit on the ROM image itself.
The manifest comes from `romfs.py --manifest` (offsets only, no game data),
RNC is unpacked on read with the C unpacker, and the same swap table is used.
This lets Phase 7 start (title screen, first level) while the Torch module is
written. It is the Rage Wars approach behind the same interface, and is
deleted when the archive path works.

## 6. Step list (Phase 6 remainder)

Dependencies: a1 = ROMFS, a2 TEXTURES, a3 MODELS, a4 LEVELS, a5 ANIMATION, a6 AUDIO.

1. **Load-unit census** (a1, done): `RomFS(rom).load_units()` in
   `tools/assets/romfs.py` yields `(key, size, node)` for every unit the engine
   reads on its own (index headers, `(count*4+15)&~7` or 0x18 for object-index
   headers, plus whole index entries; RNC children inside raw containers are not
   separate units). US ROM: 26,124 units = 14,426 RNC, 6,438 raw ISet containers,
   4,885 empty, 334 raw, 41 USet, plus the 3 audio regions (audio window and sound
   data as `VM_Alloc` units, the sample data read piecemeal). `fs.readers(node)`
   gives `[(description, callback, callers)]` (column "readers" in romfs_tree.tsv;
   ROMFS.md "Load units and their readers"). Still to add for the archive: a
   `--manifest` writer with (address, ROM size, unpacked size, type, flags).
2. **Unpack rule check** (a1, done): every RNC unit has a reader with a callback,
   and every such callback calls `ProPackDecompress` (`Model_Decompress*`,
   `Anim_Decompress`, `Texture_DecompressCallback`, `CScene__Decompress*`,
   `CScene__DecompressGridSection`, `Cinema_PrepareResource`, `ProPack__Decompress`),
   so all 14,426 RNC units can be stored unpacked. The only unit without a reader
   is cart/20 (8-byte end marker). Re-run the check whenever the reader table grows.
3. **Swap tables** (a2-a6): each area document gets a field-width table per
   type, e.g. `grid section: CISet; child 0: CUSet stride 0x1C {f32 x3, u16, u16, ...}`.
   `tools/assets/swapgen.py` turns them into `port/t2swap.c`. Until a type has
   a table it is `unknown` and logged.
4. **`mkarchive.py`** (Python, stdlib): writes `turok2.o2r` / the folder
   tree, `version`, `t2/units`, `t2/types`. Unit tests: unit bytes equal the ROM
   range or its RNC unpack; manifest sizes; no unit outside the asset region
   except the audio window.
5. **Torch `T2` module:** `T2:CARTRIDGE`, `T2:RAW_RANGE`, RNC in
   `Decompressor`, binary exporter, `config.yml`. Validate with
   `archive_check.py` against step 4.
6. **Port side** (opens Phase 7): `port/t2asset.cpp` (manifest, read,
   host size, swap dispatch), the five function changes of 5.4,
   `GameExtractor`, `t2port.o2r` packaging.
7. **Verification:** a debug mode that logs every `CCache__Request` (key,
   size, description) and checks the description against the manifest's type
   for that key. Boot -> attract demo -> a level must produce no mismatches.

## 7. Later: typed resources and mods

- **Unit replacement** works from day one: a mod O2R containing
  `t2/u/<addr>` (ROM byte order, unpacked) shadows the original, and
  `T2Asset_HostSize` serves its new size. Offsets *inside* the unit are the
  mod's business. Sizes of siblings in parent tables do not matter, because
  the size comes from the archive.
- **Texture packs:** Turok's textures reach Fast3D as pointers into cache
  memory, so libultraship's path-based replacement does not apply directly.
  Two routes: tag texture uploads with the source unit (the port knows the
  unit when `Texture_DecompressCallback` runs and can register `block ->
  "t2/tex/<addr>/<n>"`, then emit `G_SETTIMG_OTR_FILEPATH` for replaced
  ones), or hash-based replacement in Fast3D (RT64/GLideN64 style). That is a
  Phase 8 decision.
- **Typed exports for tools** (`OTEX` textures, glTF models) come from the
  area decoders (`tools/assets/*.py`). They are for viewing and editing, not
  needed by the game.

## 8. Licensing and the no-data rule

- The repository never contains game data: no extracted bytes, images,
  sounds or dumps. `.gitignore` already excludes ROM images. Archives
  (`*.o2r`, `*.otr`) and extraction output folders must be added to it when
  the tools land.
- Tools and docs only: the manifest format, swap tables and walkers describe
  *where* and *how*, never *what*.
- The port refuses any image whose SHA1 is not
  `fb0400f21e3f043939ab56500c7b12a3231006f1` (US). The archive records the
  SHA1 it was built from, and the port re-extracts when it differs or when the
  extractor format version changes.
- Third-party code: libultraship and Torch are MIT. The Rage Wars port
  (references/rw_junk) is CC0, so its `rom_find.c` and `audio_hle.c` may be
  reused. Turok 3's decomp is referenced, not copied.

## 9. Open questions

- Storing RNC units unpacked: a1's reader table shows every RNC unit goes
  through a callback that calls `ProPackDecompress` (step 2). Still worth a
  look: whether any callback touches the block *before* that call, and whether
  the raw containers whose RNC children are unpacked from memory need
  `KEEP_PACKED` or an offset-table rewrite (5.2).
- Units read at more than one size where the larger read crosses into what
  another call site treats as a separate unit: does the tree walker see any?
- `Bits_Read`: byte or word fetches? (Decides the swap rule for animation
  streams.)
- The audio window 0x1E0000-0x20F194 (now its own splat segment, `audio_ctl`)
  holds the 913-entry sfx soundArray at 0x20E334 (AUDIO.md / ROMFS.md).
  The libaudio bank structures it contains are pointer-fixed in place by
  `alBnkfNew`-style code with 32-bit fields. That is a 64-bit problem the
  audio port (Phase 7) must solve by rebuilding the bank in host structures,
  not an archive problem.
- Other releases (EU, JP) are not supported by the decomp. Adding one means
  another SHA1, another set of root constants, and checking whether the
  layout differs.
