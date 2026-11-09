# 64-bit and endian audit of the engine (Phase 7, p2)

What the game C assumes about pointer width, the N64 memory map and byte order, and what the
native build (64-bit, little-endian: macOS arm64, Linux/Windows x86_64) has to change. Every
number below comes from `tools/port/audit64.py` (12 s on the box) or a grep given next to it,
over the tree at 8fb13231. Nothing in `src/` or `include/` was changed.

```
python3 tools/port/audit64.py [--out DIR]   # default port/build/audit64 (gitignored)
```

It compiles every `src/us/code/*.c` with `clang -fsyntax-only -target x86_64-linux-gnu` and only
the pointer-width warnings on, maps each warning to its function and subsystem and a category,
and compares every function definition with every other declaration of it. Output:
`warnings.json` (one row per warning: file, line, function, subsystem, category, source line) and
`prototype_mismatches.json`.

## TL;DR

| | count |
|---|---|
| C files that pass clang x86_64 syntax with the stock `include/` | 3,892 / 3,926 (the 34 left: MIPS register asm, 2 implicit decls) once the 10 `sizeof(T *)` pads of `include/rw/abu_types.h` are pinned (they go negative on 64-bit and stop 1,018 files) |
| pointer-width warnings | **1,927** in 786 files (1,684 in first-module code, 243 in the `_2` copies) |
| functions whose declarations disagree pointer vs integer | **1,100** of 4,156 definitions, 2,681 declarations (75 on the return type) |
| asset structures relocated in place (pointer written into asset bytes) | **3** families: particle templates (10 fields), path items (1 field), audio control block (absolute pointers pre-relocated to 0x480000) |
| intrusive lists with hard-coded link offsets | 39 `CList__Construct` sites (first module; 33 more in the `_2` copies) |
| raw display-list / audio-command word writes | 680 Gfx words in 87 files (129 of them expanded `gbi.h` macros), 107 Acmd writes in 17 files |
| sub-word views that read the wrong half on little-endian | ~20 sites + 2 bitfield/word unions + the libaudio `Acmd` bitfields |
| pointer-valued words in the data asm (`us/asm/data`) | 4,710 symbolic `.word sym` + ~3,950 hex words that look like unsymbolized addresses |

The good news: the engine's own asset idiom is the **offset table resolved on access**
(`CISet__GetBlock(base, i)` = `base + offsets[i]`, 729 calls; `CUSet`, the model/animation/cinema
`Pack` builders all write 32-bit offsets, never pointers). So almost no asset bytes hold host
pointers. The problem is the code: m2c-era types that carry pointers in `s32`.

**Recommended model: real 64-bit pointers, fixed by a typedef that is `s32` on N64.** See
"Decisions" at the end. A "keep everything below 4 GB" shortcut works on Linux and Windows
x86_64 for bring-up but not on macOS arm64.

---

## (a) Structures with pointers that live in asset memory

The loaders that write into a unit after `ProPackDecompress` / `CCache__AllocateMemoryForDecompression`
were read one by one (every `CCache__Request` callback and every `CScene__Decompress*`):

| unit / struct | code | what it does | native port |
|---|---|---|---|
| **Particle effect templates** `CROMParticleEffect` (include/structs/particle.h; `Piece` in rw/abu_types.h, `Entry` in func_00214188.c / func_0022D2FC.c), stride 0x3C, 10 pointer slots 0x14..0x38 | `CScene__DecompressParticles` (func_0042CAB4.c, copy func_004659D4.c) | `tag` (0x14): object index -> `CScene__GetObject()` model pointer (`-1` -> 0); 0x18..0x38: block-0 offset -> `base + off` (impact, offset, unk20, distance, direction, oscillation, motion, colours, params) | the record has 4-byte slots and is read in place by `CParticleSystem__Spawn` and friends (`entry->motion->unk8[1]`...). Two ways: **(1, recommended)** a host-side `CROMParticleEffectHost` built by the port's version of this function (same fields, real pointers, an array parallel to the template block; the readers take it instead of the ROM record); **(2)** keep the 4-byte offsets and resolve on access (`T2_PTR(base, entry->m_pMotion)`), which touches every reader of the template (CParticleSystem__*, func_0022D2FC.c and the other `Entry` users). 10 fields, 1 function + its copy. |
| **Path items** `ListItem.ref` (func_0042CD04.c, `CScene__DecompressPaths`) | same | `ref` index into level table 2 -> `(s32)&entries[ref]` written back into the 0x1C-byte item | ref is one field: keep the index and resolve in the readers (`CScene__GetPathEntry(level, item)`), or a host side table. Small. |
| **Audio control block** (ROM 0x1E0000, VM window, `ALBankFile -> ALBank -> ALInstrument -> ALSound -> ALEnvelope / ALKeyMap / ALWaveTable -> loop / book`) | none at run time: the pointers in ROM are **already absolute**, relative to the VM address 0x00480000 (AUDIO.md: `rom = ptr - 0x480000 + 0x1E0000`, `ALWaveTable.flags = 1`). `CAudioWorld__Initialize` (func_00270240.c) reads `r->bankFile`, `bk->bankArray[0]`, `instArray[0]`, and **sorts `soundArray` by pointer value** (u32 compare) | rebuild the graph in host structs once at load (`T2Audio_LoadBanks`: walk it by `ptr - 0x480000`, allocate host `ALBank/ALInstrument/ALSound/ALWaveTable` with 64-bit pointers, `ALWaveTable.base` = offset into the tbl unit as libaudio expects for the DMA callback). The sort must keep **the ROM-address order** (sort by the 32-bit ROM pointer before converting), or sound indices change (AUDIO.md "Sound index order"). The header's `{file, tblOffset}` pairs and `r->f4` (the sequence bank pointer `m->seq.data`) also come from it. |
| Sequences (`alCSeqNew`, func_0028F518.c) | `state->objects[i] = base + entry` | pointers go into the **runtime** `ALCSeq`, the song stays an offset table | only the type: `u32 base` -> `u8 *`. |
| Sound data file (`CAudioWorld__LoadSfxDataFile`, func_00270550.c) | `m->soundData = VM_Alloc(...)` then `CISet__GetBlock` | offsets resolved on access | `VM_Alloc` returns a pointer (it is `s32` today): see (c). |
| Animated instances (`CScene__DecompressAnimatedInstances`, func_0042BA40.c) | `CUSet__Transpose(node, block->data, 1)` transposes the records **in place**; then builds `CGameObjectInstance[n]` field by field (`ConstructFromRecord`), `m_pIntelligence` = `CScene__GetIntelligence()` | the records hold no pointers; the objects are runtime structs (35 pointer fields) built by code, so they grow cleanly. Allocation `n * sizeof(CGameObjectInstance) + 8` already uses `sizeof`. |
| Grid sections (`CScene__DecompressGridSection`, func_00220D00.c) | builds `CStaticObject` / `CLevelActor` / light volumes into a CISet-shaped block | runtime objects with pointers, but **with hard-coded sizes**: `szA = n * 0x90`, `szC = nC * 0x198`, `szD = nD * 0xC0` and `StrideTable.stride = 0x90 / 0x198 / 0xC0` (read back by the walkers) | replace with `sizeof(CStaticObject)` etc. under `TARGET_PORT`; the readers use the stored stride, so they follow. |
| Links / collision / models / textures / animation / cinema / intelligence | `CScene__DecompressLinks`, `__DecompressCollision`, `Model_Decompress`, `Texture_DecompressCallback`, `Anim_Decompress`, `Cinema_PrepareResource`, `CScene__DecompressIntelligence` | all build **offset tables** (`TableSet`, `packAppend`) or copy; pointers into the unit go only into runtime structs (`world->faces = ...`) | nothing to relocate. Two side issues: `Model_Decompress` sizes the display list at **8 bytes per command** (`len += 8`) and writes `Gfx` words (see Gfx below); `CGameRegion__Unpack` builds runtime regions (pointer fields `m_pCorners[3]`, `m_pNeighbours[3]`) from asset bytes, field by field: fine. |
| ISet / USet containers, persistent index, level header, events, warp dests, sky layers | `CISet__*`, `CUSet__*`, `CCache__LoadPersistantIndex` | offsets only | the accessors must take/return `void *` (they are `s32 CISet__GetBlock(s32, s32)` today, with 121 disagreeing declarations). |

Asset structs checked for pointer fields (include/structs, the ones BYTEORDER.md says are read
directly): `CRegionInfo, CEmitterDef, LevelEvent, CinemaEntry, CLevelHeader, SndEnv, CIntelligence`
and its bodies, `CROMParticleImpact/Distance/Motion/Params`, `CGeomHeader`, `ALKeyMap`, `ALEnvelope`,
`ALSeqFile`: **no pointer fields**. Pointer fields: `CROMParticleEffect` (10) and the libaudio bank
types (`ALBankFile.bankArray`, `ALBank.percussion/instArray`, `ALInstrument.soundArray`,
`ALSound.envelope/keyMap/wavetable`, `ALWaveTable.base`, `waveInfo.adpcmWave.loop/book`).

Rule for the port: **an asset struct keeps its N64 layout** (4-byte slots), it is byte-swapped by
the spec, and any field that becomes a pointer at run time is either resolved on access or copied
into a host struct. Never widen a struct that `swap_spec.json` describes.

## (b) Pointers carried in integers

### By warning flag (all code, `_2` copies included)

| flag | count | meaning |
|---|---|---|
| `-Wpointer-to-int-cast` | 1,029 | `(s32)ptr`, `(u32)ptr`: truncates on 64-bit |
| `-Wint-conversion` | 336 | pointer passed/assigned to an `s32` (or the reverse) with no cast; a hard error in clang ≥ 16 unless `-Wno-error=int-conversion` |
| `-Wint-to-pointer-cast` | 256 | `(T *)s32value` |
| `-Wvoid-pointer-to-int-cast` | 186 | `(s32)voidptr` |
| `-Wint-to-void-pointer-cast` | 92 | `(void *)s32value` |
| `-Wshorten-64-to-32` | 28 | `size_t`/`long` results stored in s32 (mostly harmless: `sizeof`, `strlen`) |

### By kind (first-module code, 1,684)

| category | count | example |
|---|---|---|
| pointer passed as an `s32` argument | 585 | `CCache__AllocateMemoryForDecompressionTop(0, (s32) arg0, ...)`, `CAI__SetMode(arg0, (s32) arg1, 2)` |
| other (s32 locals, returns, misc) | 275 | `CISet__GetBlock: return arg0 + ((s32 *)(arg0 + (arg1 << 2)))[1];` |
| pointer arithmetic through an integer | 218 | `lo = (u32)f + f->used;` (CHeap), `*(s32 *)(*(s32 *)gLoadBuffer + (s32)gLoadBuffer - 4)` (PakMenu) |
| implicit call/return type mismatch | 170 | `CHeap__Free(&gHeap, node->data)` with `s32` parameter |
| CList intrusive links | 152 | `*(s32 *)(arg1 + arg0->m_NextOffset) = temp_v1;` |
| Gfx/Acmd word holds an address | 108 | `GFX_W(gDList++, 0xDA380003, (u32)matrix)` |
| pointer stored in / loaded from an int field | 100 | `found->notifyId = (s32)owner;`, `entry->m_pFrameBuffer` (`u32`) |
| CISet/CUSet offset tables | 47 | `CISet__GetBlock(*(s32 *) handle, 2)` |
| KSEG / physical address math | 19 | `isRestorable((u32)dst | 0x80000000)` |
| pointer compare / range check as integer | 10 | `if ((u32)msg < 2)` (CCache__Main: message = small int or pointer) |

### By subsystem (first-module code): warnings / functions with disagreeing declarations

| subsystem | warn | protos | | subsystem | warn | protos |
|---|---|---|---|---|---|---|
| EnemyAnim_ | 129 | 21 | | CParticle | 28 | 5 |
| CList | 100 | 6 | | CCameraPool | 24 | 9 |
| unnamed `func_` | 89 | 44 | | EnemyAI_ | 25 | 8 |
| libultra/libaudio internals (`_`) | 84 | 7 | | CAudioWorld | 16 | 14 |
| CScene | 80 | 43 | | AI_ | 10 | 15 |
| libultra | 77 | 31 | | EnemyMode_ | 19 | 7 |
| CCache | 75 | 19 | | MenuPool_ / Menu* | 24 | 16 |
| libaudio | 64 | 35 | | Automap_ | 16 | 7 |
| PakMenu_ | 61 | 16 | | CEngineApp | 15 | 8 |
| CPlayer | 50 | 32 | | Cinema_ | 10 | 9 |
| Render_ | 46 | 6 | | Model_ | 13 | 5 |
| BlindOne_ / MotherBoss_ / Primagen_ / MantidQueen_ (bosses) | 68 | 123 | | Hud_ / Trail_ / WeaponMode_ | 52 | 6 |
| CGameObjectInstance | 28 | 15 | | others (≈150 subsystems) | ≈480 | ≈330 |

The full table is printed by the tool.

### The prototype problem (the biggest single item)

Every C file carries its own prototypes, written when it was matched (often `s32` for every
argument). 1,100 functions have at least one declaration that puts an `s32` where the definition
has a pointer, or the reverse. On a 64-bit ABI this is not a warning but a **silent truncation at
the call** (the caller converts the pointer to 32 bits, or the callee reads the low half of the
register). Worst offenders (declarations that disagree):

| function | decls | definition vs a caller's view |
|---|---|---|
| `CISet__GetBlock` | 121 | `s32 (s32, s32)` vs `void *(void *, s32)` |
| `CAI__SetMode` | 93 | `(CGameObjectInstance *, CAI *, s32)` vs `(s32, s32, s32)` |
| `AI_PickAnimFromTable` | 75 | table pointer as `s32` |
| `CList__Remove / AddTail / AddHead` | 56 / 45 / 42 | node as `s32` vs `void *` |
| `AI_TurnClamped` | 49 | |
| `CCache__Deallocate` | 41 | handle `s32` vs `void **` |
| `__osContRamRead/Write` | 36 / 30 | (libultra: replaced anyway) |
| `CCache__Request` | 35 | 9 different signatures (owner, callback, name as `s32`) |
| `MenuPool_OpenMenu` (+ `_2`) | 33 + 26 | |
| `CCache__Allocate` | 31 | |
| `CPlayerPool__GetIndex` | 29 | |

`include/functions.h` (995 lines, generated from register use) is not included by any C file and
is all `s32`: it cannot serve as the canonical header.

### Struct fields

`include/structs` is well typed: 40 headers, 502 pointer fields, and only **one** integer field
named as a pointer (`CFrameData.m_pFrameBuffer`, `u32`, gCFB). Fields that carry pointers as
integers are in the local structs of the C files (`RecordRecAt4V2`, `DemoStream.player`,
`MenuItem.data`, `CDrawArgs.m_hModel` "a handle, void **", `CCacheEntry.notifyId`, ...) and in
`include/rw/abu_types.h` (imported Rage Wars types: `Piece.unk18..unk38` are the particle
pointers as `s32`). Runtime structs **grow** on the host (every pointer +4); that is harmless
except where code assumes the N64 size or offset:

- **CList link offsets** are literals: `CList__Construct(list, prevOffset, nextOffset)` with
  `0, 4` (20 sites), `0x18, 0x1C` (loader), `0x20, 0x24` / `0x28, 0x24` (cache), `0x1C4, 0x1C8` /
  `0x1CC, 0x1D0` (particles), `0x1D0, 0x1D4` (menus), `0x288, 0x28C`, `0x131C, 0x1320`
  (players), `4, 8`: 39 sites in first-module code. And `CList` itself reads the links as `*(s32 *)(node + off)`.
  Native: `offsetof(T, m_pLast)` / `offsetof(T, m_pNext)` at each site (under `TARGET_PORT`),
  `void *` loads in the 10 CList functions (func_00200500..00200A9C). `CPlayerCList` has the same
  fields.
- **Hard-coded strides/sizes**: grid sections (0x90 / 0x198 / 0xC0, above), 23 `ptr + i * 0xNN`
  raw strides in 19 files, 119 typed dereferences at constant byte offsets in 71 files
  (`*(s32 *)((s8 *)arg0 + 0x98)`), 22 container tricks from a field address
  (`(u8 *)&gEngineApp_m_Settings_difficulty - 0x1270`, `(s8 *)&gCache_cacheUsedList - 0x14`,
  `(u8 *)&gPakQueueState + 8`). Each is a fixed N64 offset across a struct that has pointers.
- **`include/rw/abu_types.h`**: 10 pads of the form `char pad[0x4 - 0x0 - sizeof(char *)]`
  (negative on 64-bit). Port build: the shim defines them away or the header gets `T2_PTRSZ`
  (= 4 on N64).
- **Gfx is 16 bytes in libultraship** (`Gwords { uintptr_t w0, w1; }`). The game writes display
  lists word by word (`GFX_W`, `GFX`, `GFX_CMD`, expanded `gbi.h` macros: 680 writes in 87
  files) through its own `Gfx { u32 w0, w1; }` (include/gfx.h). That works on the host if the
  port's `gfx.h` makes `Gfx` LUS's type and the address-carrying commands put a full pointer in
  `w1` (`(u32)matrix` -> `(uintptr_t)matrix`: 108 sites flagged). What does not follow
  automatically: buffers sized in N64 bytes (`Model_Decompress`: `len += 8` per command, `+ 0x30`
  part header; GfxPool / gDList sizes; `(end - start) >> 3`), and segment math on raw words
  (`(u32)&vtxBase[i] - (u32)vtxTbl + 0x02000000` is a segmented offset: fine, it is < 4 GB).
- **Acmd** (audio command lists): 107 expanded `aSetBuffer`/`aLoadADPCM`... writes in 17 libaudio
  files put `osVirtualToPhysical(ptr)` in `w1` (52 calls). The synthesis HLE that runs these
  lists on the CPU must get 64-bit addresses: either `Acmd` with `uintptr_t` words + an HLE
  interpreter that reads them as host pointers (the sm64/SoH `mixer.c` approach) or replace the
  writes with direct calls. Same for `alAudioFrame`'s `(s32)` buffer arguments.

## (c) Hard-coded N64 addresses, KSEG math, the TLB pager

| what | where (count) | native replacement |
|---|---|---|
| RCP/PI/SI/VI/AI registers `0xA4xxxxxx`, `IO_READ/IO_WRITE`, `| 0xA0000000` | libultra C (38 files, 132 lines: `__osPiRawStartDma`, `osViSwapContext`, `__osSpRawStartDma`, Leo...) | not compiled: the port provides libultra (LUS's / port shim: `osPiStartDma` = asset read, `osViSwapBuffer` = present, `osAiSetNextBuffer` = SDL audio). |
| `osVirtualToPhysical` (52 in 18 files), `& 0x1FFFFFFF`, `__osProbeTLB` | libultra, libaudio Acmd, `osSpTaskLoad` | identity (`(uintptr_t)p`); `osWritebackDCache*` / `osInvalDCache` / `osInvalICache` (51 calls) = no-ops. |
| RDRAM size / heap top: `top = 0x80400000`, `(GetMemSize() | 0x80000000) - 0x80000` | `InitializeSystemResources` (func_00202130.c), `CCache__Construct` (func_00202BB0.c: expansion pak test), `func_00425220` / `func_0045E140` (`base = 0x80400000`, gfx pool), `func_0028932C` | heap = one host allocation of 4 or 8 MB (keep the N64 size so the cache behaves the same; `D_8014A000` "end of image" becomes the arena start). `GetMemSize()` returns 0x800000 (expansion pak). |
| Boot / soft-reset: copy to `0x80000400`, jump to `0x80000400`, `isRestorable((u32)dst | 0x80000000)`, `Boot_CanOverwrite` address ranges (`&gEngineApp_m_Settings_match ... + 0x4C`), `osMemSize` at `0x80000318` | func_0028D380.c (Engine_Boot), func_0028D714.c, func_0028E068.c, func_002A2104.c | the reset path re-runs `romMain` with "keep these ranges"; on PC: a restart of the game loop that keeps the saved-settings struct. Rewrite, ~5 functions. |
| Crash screen: `IsCodePointer` / `IsDataPointer` (`&CList__Construct + &D_A4694` range), stack walk | func_00243170.c, func_00267024.c, func_0028BDFC.c | drop (host crash handler) or compare against the host image range. |
| **TLB / VM pager**: `boot.s` maps 0x200000 (2 MB page) for the engine; `VMASM.S` + `VMEMORY.C` map the `virtual` modules (0x400000+) and the audio/sound windows (0x480000...) in 4 KB pages: `VM_Initialize, VM_ManagerThread, VM_ReaderThread, VM_HandleTLBMiss, VM_MapTLBEntry, VM_UnmapTLBEntry, VM_AcquireNextTLB, VM_ReclaimLRUPage, VM_Age, VM_Alloc, VM_AllocRegion, VM_FindRegion, VM_Free, VM_FreeRegion, VM_FinishPageJob, VM_GetStats, VM_List*`, `osMapTLBRdb`, `__osUnmapTLBAll`, asm `STVM_MapTLB` (func_0028EE44), `__osProbeTLB` | func_0028D820.c ... func_0028EC54.c, docs/VMASM.md | **not built.** Code: in a native build every function is just a function. Both VM modules' C compile and link side by side (the second module's copies are already separate symbols, `_2`), so "which copy is mapped" stops being a memory question; the engine's calls through module descriptors pick the function. Data windows: `VM_Alloc(size, 0, romAddr)` -> `T2Asset_Read(romAddr)` resident (PORT.md 10.4); its return type becomes `void *` (callers: `CAudioWorld__Initialize`, `CAudioWorld__LoadSfxDataFile`, which declares it `void *` already while the definition says `s32`). The fault/page threads are not created. |
| Code addresses in data | the VM `.rodata` descriptor records (`0x0000FF01` + function pointers, 0x43C05C...), menu tables (`MenuItem_DrawText` ×682, `Menu_Draw` ×146 in `.word`), weapon/AI/pickup tables | when the data asm becomes C (needed for any native link) these must be typed initializers (`{ 0xFF01, func }`), pointer-sized. 4,710 symbolic `.word` in `us/asm/data`; **~3,950 hex `.word`s in 0x80000400-0x80400000, 0x00200500-0x00300000 and 0x00400000-0x00490000 look like unsymbolized pointers** (as floats they would be denormals): they must be symbolized first, or the native data will hold dead N64 addresses. |
| Segmented addresses | `Model_Decompress` (segments 1, 2), `gSPSegment` words (`0xDB06....`, 7 files) | kept: Fast3D resolves segments; the segment table must hold host pointers (`uintptr_t`). |

## (d) Endian beyond `swap_spec.json`

`swap_spec.json` makes asset fields host-order. What it cannot cover is code that looks at a
wider object through a narrower type, which picks the other half/byte on little-endian:

| site | code | fix |
|---|---|---|
| func_00232168.c | `((u8 *)&gCollisionResults.m_nInstanceNode)[3]` "its low byte" | `(u8)x` |
| func_0025E428/0025E434/0025F730.c | `((s8 *)&ai->m_VarBuffer[9])[3]` (AI var low byte, get/set) | `T2_LO8(x)` accessor, or index `[0]` on LE |
| func_00272B54.c | `((u8 *)&vp->pan)[1]`, `*(s16 *)&o->rec.w4` | value casts |
| func_0028EF90.c (`alCSeq` track) | `((u8 *)&result->extra2)[0..1] = __getTrackByte()` builds a BE u16 | `extra2 = hi << 8 | lo` |
| func_0026D200.c | `return *(u16 *)&arg0;` (high half of an s32 argument) | `(u16)(arg0 >> 16)` |
| func_00413230.c:1244 (+ copy) | `buf[0] = ((u8 *)&a->data)[3]` | value cast |
| func_002998B0 / func_00299C60 | `*(u16 *)&packet.bytes[4]` (Controller Pak packet) | libultra, replaced |
| func_0025739C / func_002573FC / func_004214C4 / func_004402C8 | `*(u8 *)&gEngineApp_m_Settings.multiplayer`, `*(u8 *)&gPakProfile.character`, `*(s8 *)&gLevelScript.unk3CA` | byte read of a wider field: low byte on BE = byte 3, byte 0 on LE: check each field's width |
| bitfield/word unions | func_002600E8.c / func_00260554.c: `union { u32 word; struct { u32 hi:12; b80000:1; ... } bits; } flags` used both ways (`flags.word & 0x80000` and `bits.*`) | GCC allocates bitfields from the MSB on BE and the LSB on LE: the two views disagree. Use the word with masks only. |
| `include/structs/libaudio.h` | 63 bitfield members (`Acmd` `cmd:8 flags:8 gain:16` ...): the RSP's view of the command word | only matters if the HLE reads them through the bitfields; use shifts (LUS gbi does the same: bitfield views only `IS_BIG_ENDIAN && !IS_64_BIT`). |
| Mixed-width asset access | `REGION_WORD` (runtime `CGameRegion` only, BYTEORDER.md), `HalfToFloat` (89 calls in 22 files on u16 fields) | covered by the spec as long as every reader uses the field's width; `(u32 *)` over asset u16 pairs still to be checked when the `unk` bytes of the spec are named. |
| Texels / TLUTs | `Texture_DecompressCallback` recolours RGBA5551 palettes with native u16 loads | big-endian accessors (BYTEORDER.md). |
| Saves | Controller Pak images are BE on N64; the save code does `*(s32 *)((s32)gLoadBuffer + 4)` and a CRC over the bytes | decide the file format: keep N64 byte order (mempak files interoperate with emulators) and swap at the pak boundary, or host order (simpler, not portable between hosts). |

Size: about 20 code sites + 2 unions; none in the hot render/asset paths except the texture recolour.

## Effort ranking (smallest first)

| # | item | size | notes |
|---|---|---|---|
| 1 | libultra hardware access, KSEG/physical math, cache ops | 0 game files | replaced by the port's libultra; nothing to edit |
| 2 | TLB/VM pager | ~20 functions not built, `VM_Alloc` rewritten (PORT.md 10.4) | both modules link side by side |
| 3 | endian sub-word views | ~20 sites, 2 unions | `TARGET_PORT` guards or value casts (identical N64 code is likely; prove with asm_diff) |
| 4 | path ref + particle template relocation | 2 functions (+ 2 copies) and the template readers | host side array for templates |
| 5 | CList offsets + CList implementation | 39 sites + 10 small functions | `offsetof` |
| 6 | hard-coded sizes/offsets (grid strides, constant byte offsets, container tricks, abu pads) | ~180 sites | each needs a `sizeof`/`offsetof` |
| 7 | heap/boot/reset/crash memory map | ~10 functions | rewrite in port/ |
| 8 | audio bank rebuild + Acmd 64-bit (HLE synthesis) | 1 loader + 17 libaudio files | do with the audio HLE (p4) |
| 9 | Gfx words: `(u32)ptr` in w1 + DL buffer sizing | 108 sites + the size computations | after `Gfx` = LUS `Gfx` |
| 10 | data asm -> C with typed pointers; symbolize ~3,950 hex addresses | large, mostly mechanical (splat + tools) | blocks any native link |
| 11 | **prototypes + s32 pointers**: 1,100 functions / 2,681 declarations, 1,684 warning lines | largest | mechanical with a tool, see below |

## Decisions recommended

1. **Pointer model: real 64-bit pointers.** Three options were weighed:
   - *Low 4 GB arena* (all game memory, statics and asset buffers below 4 GB so `s32` holds a
     pointer): works on Linux x86_64 (`-no-pie`, `mmap(MAP_32BIT)`) and Windows x64
     (`/LARGEADDRESSAWARE:NO` or a fixed low `/BASE` + `VirtualAlloc` hints), with **zero** source
     changes for item 11. It does **not** work on macOS arm64: the 64-bit address space starts
     above `__PAGEZERO` (4 GB), and arm64 macOS does not accept a smaller page zero (the reason
     LuaJIT is GC64-only there; to re-verify on a Mac). Pointers held in `s32` also sign-extend
     above 2 GB. Use it only as a **bring-up mode on Linux** (`T2_LOW32=1`), to get frames before
     item 11 is done, with a runtime assert that every arena pointer is < 2 GB.
   - *32-bit guest address space* (the Rage Wars port: i686 build, guest ranges reserved at the
     N64 addresses, a big-endian IR rewrite): no 32-bit target on macOS arm64; not an option here.
   - *Retype* (recommended for the shipping port): see 2.
2. **One typedef family, identical on N64.** `include/ptrint.h` (or in `common.h`):
   `#ifdef TARGET_PORT typedef intptr_t sptr; typedef uintptr_t uptr; #else typedef s32 sptr; typedef u32 uptr; #endif`.
   A matched C file that carries a pointer in `s32` changes `s32` -> `sptr` (and `(s32)p` ->
   `(sptr)p`) at exactly the sites `audit64.py` lists. On N64 `sptr` *is* `s32`, so cc1 emits the
   same code: `tools/asm_diff.py --base 8fb13231 <files>` must show 0 differences (no `TARGET_PORT`
   noise in the C). Never use `sptr` in an asset struct.
3. **Canonical prototypes.** Generate `include/protos/*.h` from the definitions (typed by the
   above), make each file include it and drop its local prototype, file by file, with asm_diff
   proving the N64 output unchanged. Where a file's local prototype type is load-bearing for the
   match ("the field's real type changes the ROM's code here"), keep it under `#ifndef
   TARGET_PORT`. Start with the top 25 functions of the mismatch list (≈950 of the 2,681
   declarations): `CISet__*`, `CList__*`, `CCache__*`, `CAI__SetMode`, `AI_*`, `MenuPool_*`,
   `CPlayerPool__*`, `osRecvMesg/osCreateMesgQueue` (the last two disappear with the port libultra).
4. **Asset structs stay N64-shaped** (4-byte slots, swapped by the spec); runtime structs widen.
   Relocated asset pointers (particles, paths) get host side tables; the audio bank is rebuilt.
5. **Build the port with** `-Werror=pointer-to-int-cast -Werror=int-to-pointer-cast
   -Werror=int-conversion` once item 11 is done, so new code cannot regress; until then the
   p1 build should keep them as warnings and track the count (1,927 today).
6. **Gfx**: the port's `include/gfx.h` replacement aliases LUS's `Gfx`; the game's raw-word
   writers stay, and every display-list buffer size moves to `sizeof(Gfx)`.

## What the next round needs

- p1's port build to adopt `sptr/uptr` + `-Wno-error=int-conversion` and report its own count
  next to this one (they should agree: same flags).
- A `tools/port/retype.py` that applies decision 2 at the listed sites and runs asm_diff per
  file (the list is in `warnings.json`; ~45 % of the sites are call arguments that go
  away once the callee's prototype is right, so do 3 before 2).
- Symbolize the ~3,950 address-like hex words in the data asm before converting data to C.
- The audio bank rebuild belongs with the audio HLE work (p4).
- Name the `unk` bytes of `swap_spec.json` that sit in code-read structs (collision regions,
  particle distance/motion) before trusting them on little-endian; check for `(u32 *)` reads of
  u16 pairs there.
