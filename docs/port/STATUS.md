# Port status after Phase 7 round 2 (merged state)

Round 2 merged five parts and then integrated them: q1 (clean-room RNC), q4 (host-sized GBI),
q3 (`sptr`/`uptr`, canonical prototypes), q2 (the game data generated as C) and q5 (the game
linked into `turok2`, t2_ names, runtime, placeholders). This page says how to build and run
the result, how far `turok2 --game` gets and what stops it. Details per part:
[DATA.md](DATA.md) (q2), [LINK.md](LINK.md) (q5), [64BIT.md](64BIT.md) (q3), [GBI.md](GBI.md)
(q4), [../assets/ARCHIVE.md](../assets/ARCHIVE.md) (q1's RNC).

The matching build is unchanged: `tools/asm_diff.py --base 55932a88` reports 0 differences over
all 3,938 files, and a clean `make build` + `make verify` gives the identical ROM.

## 1. Build and run (Linux x86_64, the box)

```sh
# 0. the matching build first: gendata reads its ELF (build/turok2.us.elf)
make setup && make -j8 build && make verify

# 1. the archive (sha1 bb77f744594dd036be4d6738eecbb6f1ef25d8fc with Python 3.13 / zlib 1.3.1)
python tools/assets/mkarchive.py baserom.us.z64 -o OUT/turok2.o2r -j 4

# 2. configure; build the game library (gendata needs its compile_commands.json and objects)
cmake -S port -B OUT/build -G Ninja -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ \
      -DCMAKE_BUILD_TYPE=Debug
ninja -C OUT/build -j8 turok2_game

# 3. the game data as C, into port/gen/ (gitignored: ROM content); libclang 19 python bindings
pip install clang==19.1.7                 # in .venv; libclang from /usr/lib/llvm-19
python3 tools/port/gendata.py --build OUT/build --cache OUT/decls.json -j 4
#    (about 40 s with 4 workers; drop the cache file whenever a header changes: it holds types)

# 4. re-run cmake so data.cmake sees port/gen/*.c, then the executable
cmake OUT/build && ninja -C OUT/build -j8 turok2

# 5. run headless (T2_LOW_MEMORY is the Linux default: non-PIE + game memory below 2 GB)
SDL_VIDEODRIVER=offscreen gdb -batch -ex run -ex "bt 30" -ex "thread apply all bt 8" \
    --args OUT/build/app/turok2 --archive OUT/turok2.o2r --rom baserom.us.z64 \
    --game --no-window --frames 120
```

`turok2` links `t2runtime` + `turok2_game` + `turok2_data` as one rescanned group, then
`t2placeholders`. With the generated data the placeholders shrink from 597 to **1**
(`osResetType`, which only `T2Port_Boot` writes); regenerate them with the procedure in
`tools/port/genplaceholders.py` after the data changes. The 32 asm-only functions q2 listed are
all covered: `RNC_UnpackMethod1/2` call `Rnc_Unpack` (port/assets/rnc.c, header-driven: the
size arguments are big-endian ROM words read natively), the rest are t2os / t2ultra under the
t2_ names, `sqrtf` and `sprintf` are in t2boot.c, `memcpy` is libc.

Generated data on this tree (`gendata.py` report): 1,562 data symbols that module-1 C needs
(1,555 typed from a C declaration, 7 as bytes), 277 extra pointer targets, 782 .bss symbols,
226 aliases, 2,624 pointer slots (1,479 relocated, 742 null in the ROM, 403 unresolved and set
to NULL), 388 functions referenced from data (all defined by the game C), 1.22 MB.

Two integration fixes were needed for that:
- `CMAKE_EXPORT_COMPILE_COMMANDS` moved to `port/CMakeLists.txt`, before `turok2_game` exists.
  In `data.cmake` (included after `game.cmake`) it missed every game target, so gendata found
  no compile command and typed nothing (0 of 1,562, all as bytes).
- `__osCurrentTime` is defined by libultraship's `os.cpp` too (multiple definition): the game
  names it `t2___osCurrentTime` (`port/include/t2_rename.h`, new data block) and gendata
  generates `t2_X` data from the N64 symbol `X` (`#define X t2_X` at the top of each file).

## 2. How far `turok2 --game` gets

```
t2: boot: arena 32 MB at 0x40200000 (low memory), ROM baserom.us.z64, mainproc started (pri 13)
game: booted, 120 frames
t2: Sound__Initialize: audio off on the host (sound bank not converted yet)
Thread 5 "turok2" received signal SIGSEGV
#0 CISet__GetBlock (arg0=0x0, arg1=0)                         func_002017D4.c:6
#1 CScene__GetRegionInfo (arg0=&gScene, arg1=0x3e32b8c400000000) func_0022425C.c:26
#2 CCamera__AdvanceAttributes (cam=gEngineApp_m_Game_m_CameraPool_m_MainCamera+76) func_0027CAB0.c:73
#3 CCamera__Advance                                            func_0027D160.c:102
#4 CCameraPool__Construct (pool=gEngineApp_m_Game_m_CameraPool_m_ActiveList_m_pHead+4) func_0042FE4C.c:30
#5 CEngineApp__Main (e=&gEngineApp)                            func_00286674.c:81
#6 mainproc                                                    func_00288860.c:13
other threads: CLoader__Main blocked in t2_osRecvMesg (as on hardware), the cache and
decompressor threads created and waiting; platform thread in t2os_run_until_idle
```

What now runs, in order: boot -> `CEngineApp__Main` -> `InitializeSystemResources` (CHeap on
the arena, decompressor and loader threads, `CCache__Construct` with its pools, hash table and
thread) -> `GfxPool_Alloc` (frame buffers, depth buffer, display lists, RSP FIFO, audio heap)
-> `CEngineApp__Construct` (the engine object, the synthesizer up to `amCreateAudioMgr`; the
sound bank is skipped) -> `CCameraPool__Construct` -> the main camera's first `Advance`, where
it stops. Mode: none yet (`CEngineApp__SetMode(0)` comes after the pools); 0 frames drawn,
so `--draw` (q4's GBI is in) has nothing to show yet.

## 3. Crash log (this round)

| # | where | cause | fix |
|---|---|---|---|
| 1 | `CHashTable__Construct` (func_00202860) | CHeap hard-coded the N64 block header (0x20; host `CHeapBlock` is 48): free list wrong from the first block; `HashEntry` 16 bytes on the N64, 32 on the host | header as `sizeof(CHeapBlock)` in CHeap Construct/Alloc/AllocTop/AllocAt, addresses through `uptr` (Free too); hash table by `sizeof(HashEntry)` (Construct, Insert). asm_diff 0 |
| 2 | `CHashTable__Construct` again | heap corrupted by `CCache__Construct`: the free stack sized `n * 4` holds 8-byte pointers; pools by N64 strides 0x28/0x2C; CList link offsets 0x18/0x1C/0x20/0x24/0x28 | pools by `sizeof(CMemEntry)`/`sizeof(CCacheEntry)`, stack by `sizeof(CMemEntry *)`, offsets by `T2_OFFSETOF` (new in common.h); `CCacheEntry` gains its `last` link at 0x28 (size 0x2C). asm_diff 0, check_structs 0 wrong |
| 3 | `GfxPool_Alloc` (func_00425220) | frame/depth buffers claimed at fixed RDRAM addresses (`GetMemSize() \| 0x80000000` - 1 MB): outside the host arena, `CCache__AllocateAt` returns NULL | `#ifdef TARGET_PORT`: `T2Port_RdramTop()` (t2boot.c: arena end + 512 KB, the console's geometry) |
| 4 | `alFilterNew` from `alSynAllocFX` (func_00297420) | m2c view: `s32 records` at 0x34 + `bus * 0x4C + 0x20` | rewritten as libultra's synallocfx.c with `ALSynth`/`ALAuxBus` (matches) |
| 5 | `alMainBusParam` (func_002964A0) | source list as `s32 *` | typed as libultra's mainbus.c (matches) |
| 6 | `alAuxBusParam` (func_00294E3C) | the same | typed as auxbus.c (matches) |
| 7 | `CAudioWorld__Initialize` (func_00270240:95) | the sound bank is a `VM_Alloc` copy of ROM 0x1E0000: big-endian, N64 layout, pointers that are VM addresses | **not fixed**: `Sound__Initialize` replaced by an empty host function (link.cmake, t2boot.c). Needs the bank converted (T2Asset + swap spec) |
| 8 | `CCameraPool__Construct` (func_0042FE4C) | local m2c struct with pads at N64 offsets, `memset(0x11F0)` | typed with `CCameraPool` (structs/camera.h), list links at 0 and `sizeof(void *)` (matches) |
| 9 | `CScene__GetRegionInfo` via `CCamera__AdvanceAttributes` | `cam->m_ViewParams.m_pCurrentRegion` is garbage: `CEngineApp`/`CGame` keep the camera pool, the scene and other parts as **byte arrays of N64 size** (structs/game.h), so on the host the typed `CCameraPool` (larger) overruns its slot, and the alias symbols (`gEngineApp_m_Game_m_CameraPool`, ...) disagree with `e->m_Game.m_CameraPool` (pool at `...m_ActiveList_m_pHead+4`, not even 8-aligned) | **open**, next round (below) |

## 4. What the next round needs

1. **`gEngineApp` host layout.** `CEngineApp` and `CGame` (structs/game.h) hold `CScene`,
   `CCameraPool`, the menu pool and more as `u8[N64 size]`, and `CGameFromCameraPool` views the
   same memory from the camera pool. On the host those members must be the real types (under
   `TARGET_PORT` at least), and gendata's aliases (`gEngineApp_m_Game_m_CameraPool`,
   `gEngineApp_m_Settings*`, `gScene_*`) then follow, since it computes their host offsets through
   the declared type. This is the next crash and blocks everything after the camera pool.
2. **Audio bank**: convert the sound bank (ALBankFile at ROM 0x1E0000) for the host and restore
   `Sound__Initialize`; `alAuxBusNew` points its handler at `func_00294D54 + 0xC` (asm in the
   middle of a word): a host `alAuxBusPull` is needed before any audio frame runs.
3. The ROM data the loader DMAs is big-endian: `ProPackDecompress` tests `'RNC'` as a native
   word, so on the host it will not see RNC streams; route the loader/decompressor through
   T2Asset (assets/PORT.md §10.4) instead of raw PI DMA.
4. q2's known gaps: the 403 unresolved pointer slots in 38 runtime structs (mark them .bss),
   the `*ModeTable` integers that hold function addresses (type them, rerun gendata).
5. Remaining 64-bit work (64BIT.md part 2): 1,064 mismatched declarations, ~3,000
   pointer-width warnings; each fix above was found by running, the audit lists the rest.

## Round 3, part r2: the loader through the archive, assets in host byte order

Details: [../assets/PORT.md](../assets/PORT.md) 10.4 and 10.6, [../assets/ARCHIVE.md](../assets/ARCHIVE.md)
(`t2/swap`, format 2.0), [../assets/BYTEORDER.md](../assets/BYTEORDER.md).

- **Archive format 2.0** (`tools/assets/mkarchive.py`): new entry `t2/swap`, one byte-swap plan per
  unit, computed from `swap_spec.json` (`swapcheck.unit_plan`). Rebuild the archive: the port
  (`T2Asset_OpenLus`) refuses 1.x. Deterministic (sha1 `99cbe767...` with zlib 1.3.1, any `-j`).
  `archive_check.py` recomputes and checks the plans.
- **Reader** (`port/assets/t2asset.c`): `T2Asset_SwapUnit` applies the plan after every backend
  load (whole unit, then the prefix), so `T2Asset_Read` / `T2Asset_ReadInto` return host-order
  fields on a little-endian host; VM-window residents are swapped once; the wave table stays raw.
- **Engine** (`#ifdef TARGET_PORT`, asm_diff 0 over the 9 changed files): `CLoader__LoadSync`,
  `CLoader__LoadAsync` (one entry per request), `CLoader__Main` (no PI DMA) read through
  `T2Port_ReadAsset` (port/runtime/t2loader.c); `CCache__Request` sizes with
  `T2Port_AssetHostSize`; `VM_Alloc` returns the whole unit in host order; `ProPackDecompress`
  keeps only the plain-data branch (no 'RNC' path can be taken on the host).
- **Cache on the host** (found while checking the path in the game): `CCache__Construct` wrote
  through `D_800D7BE0`, which gendata places inside `gDecompressor`, so `gCache`'s free stack
  stayed NULL and the first `CCache__AllocMemEntry` crashed; now typed on `gCache`. Also the free
  stack by index (func_00203068, func_002044A8) and a typed `CCache__AllocDecompressBlock`
  (func_00205D7C) for `ProPackDecompress`.

Tests: `t2asset_test` (26,124 units = ROM bytes through the plans, double swap, probes, 20,000
audio reads, ROM backend + plans: 0 errors), `t2asset_typed_test` (1,585 field checks on level
headers, geometry incl. transposed vertices, texture banks against the area tools' big-endian
parsers: 0 errors), `turok2` self test (LUS = miniz = ROM+plans), `archive_check.py` OK.

How far `turok2 --game` gets: unchanged (`CCameraPool__Construct` -> ... -> `CISet__GetBlock(NULL)`,
r1's crash); no asset read happens before it. In gdb at `GfxPool_Alloc`, `CCache__Request` (direct,
RNC unit, `ProPack__Decompress` callback, 4-byte probe) returns correct host-order bytes.

Open, found on the way (not fixed here):
- `gPoolBuffers` is 0x20 bytes of .bss in port/gen (untyped), the host `GfxPool` is 64:
  `GfxPool_Alloc` writes `pool->gfx`/`vtx` over `gCFB[1]` (becomes a heap pointer), and
  `CEngineApp__Main` then memsets 150 KB there, over the cache tables. Needs a typed
  declaration so gendata sizes it.
- Other m2c views at N64 offsets on the cache path (`CCache__AllocateMemoryForDecompressionTop`,
  B12Mgr users) remain; `CHeap__Free`/`CCache__HeapAlloc` callers that pass pointers as `s32` work
  only in low-memory mode (not on macOS).
- `Texture_DecompressCallback` recolours RGBA5551 palettes with native u16 loads: needs
  big-endian accessors now that units arrive host-order but texels/TLUTs stay big-endian.
- The 0.5 % `unk` bytes of BYTEORDER.md arrive in ROM order (collision regions, particles, swooshes).

## Round 3, part r1: the boot runner (to the first frames of mode 0)

Branch `names-R3_r1` (merges r2's `names-R3_r2` at 691aad0a for the loader). Every commit:
`tools/asm_diff.py --base 3798e01e -j 8` = **3938 files, 0 differ**; `check_structs.py` on
game.h, camera.h, world.h, frame.h, sched.h: 0 wrong.

### Where it is now

```
SDL_VIDEODRIVER=offscreen T2_SHOT=out.ppm build/app/turok2 --archive <r2's format-2 turok2.o2r> \
    --rom baserom.us.z64 --game --draw --frames 300
game: 300 frames in 2.66 s, 1404 thread switches, 151 gfx tasks (151 drawn), 0 audio tasks,
      0 PI DMAs, 0 missing-function calls
OK (0 failed checks)
```

Boot -> `CEngineApp__Construct` (scheduler thread up) -> camera pool, player pool, `CScene__Construct`
(cartridge, levels, texture sets, object types, objects indexes) -> `CScene__LoadLevel(0)` (level
0 geometry, collision, instances) -> the message loop -> mode 17 switches to **mode 0 (pak check)**:
`EngineMode00_PakCheck_Init` loads level 0 again, every retrace runs `CEngineApp__Update` ->
`CEngineApp__Draw` (`CScene__Update`, the camera pool draw, the menus) ->
`CEngineApp__SubmitGfxTask` -> the scheduler -> `osSpTaskStartGo` -> Fast3D. At frame 120 mode 0's
timer opens the pak menu (`gPakSaveGameAnyPakMenuDesc`, then `gDamagedPakAtBootMenuDesc`: no
Controller Pak is modelled) and its windows are drawn. 300 frames run to the end without a crash;
**151 display lists drawn by Fast3D** (2 retraces per frame, as on hardware).

The picture (frame 300, `T2_SHOT`): black, with the menu's frame rectangles but no text: the
window widths come out ~0 (a 4-pixel red bar at the left), so the text measuring/drawing (fonts:
`COnScreen` font loads, `Menu_TextWidth`, the HUD text path) is the next thing to follow. The game
renders at 480x360 (`gDPSetColorImage` width 480; `Video_SetMode(480, 360, ...)`). `T2_DLDUMP=1`
prints the last frame's display list (166 commands: init lists, scissor, colour/depth image, fill
rectangles, prim colours; no texture or vertex commands yet).

### What was done (crash table, round 3)

| # | where | cause | fix |
|---|---|---|---|
| 1 | `CScene__GetRegionInfo` via `CCamera__AdvanceAttributes` | `CEngineApp`/`CGame` held the scene, camera pool, onscreen as N64-sized bytes | `structs/game.h` under TARGET_PORT: `CScene m_Scene[1]`, `COnScreen unk19440[1]`, `union { CCameraPool m_CameraPool[1]; struct { pad; u8 m_MenuPool[0x18]; }; }` (one-element arrays keep `x.m_CameraPool` a pointer); mips `_Static_assert`s keep the N64 offsets (gendata parses mips too). 8 files with literal CEngineApp/CGame offsets (0x3C8, 0x22C00, 0x23DF0, 0x1268, 0x1228) by `T2_OFFSETOF` |
| 2 | same | gendata translated offsets inside a union through its *last* member (the byte pad) | `_dtranslate`: a union prefers the member that types the offset |
| 3 | `CCamera__AdvanceAttributes` | `CCameraViewParams__Construct` wrote through an ObjInit view (4-byte region pointer) | host body on `CCameraViewParams` |
| 4 | `Math_AcosLookup(NaN)` | libultra's `du` (`{ struct { u32 hi, lo; } w; f64 d; }`) generated by its words: doubles with swapped halves | gendata: a union's whole-size scalar member lays out the data |
| 5 | `CHeap__Free(NULL)` from `CCache__Flush` | `gPoolBuffers` (`extern u8 gPoolBuffers[]`, a GfxPool of pointers) generated at its N64 0x20 bytes: `GfxPool_Alloc` overwrote `gCFB`, then the frame-buffer memset hit the cache tables | gendata: a .bss symbol the C only declares as bytes gets twice its N64 size (409 symbols); `CCache__AllocateAt` through `CMemEntry` (its B12Node view put `flags` in `lockCount`); cache free stack by name (= r2's 6abc08a0) |
| 6 | `CPlayer__LevelStart` | player list links `0x131C/0x1320`, `PlayerList` view with N64 list offsets | `T2_OFFSETOF(CPlayer, m_pLast/m_pNext)`, host `PlayerList` (both pool files) |
| 7 | `CLoader__AllocLoaderEntry` | loader lists `0x18/0x1C` | merged r2 (loader through T2Asset) + links by `T2_OFFSETOF` |
| 8 | `CCache__DetachCacheEntry`, `CCache__FreeCacheEntry` | m2c offsets 0x14/8/0x10, list 0x14 before | typed bodies |
| 9 | `CCache__LoadPersistantIndex` (levels index) | `CScene__Construct`'s `s32 cart` received an 8-byte pointer, clobbering `file` | `sptr cart` |
| 10 | `Model_Decompress` | `CCache__AllocateMemoryForDecompression(Top)` wrote CMemEntry at N64 offsets | both through r2's typed `CCache__AllocDecompressBlock` (now `..Flags`) |
| 11 | `SwooshSystem_UpdateSwooshes` | the swoosh/effect system (0x9598 bytes in CEngineApp) is N64 bytes with 4-byte links | **stub**: `func_0042D410` leaves it empty on the host (no swooshes/effect slots). Needs typing |
| 12 | `CScene__DecompressCollision`, `CScene__LoadLightSettings`, `CScene__GetIntelligence`, `CScene__GetRegion` | local CScene views at N64 offsets; region stride 0x20 | views by `__builtin_offsetof(CScene, ...)`; host `CGameRegion` stride; `CGameRegion__Unpack` typed |
| 13 | `CGameObjectInstance__SetupAnimation` | Actor view at N64 offsets (called a data address) | typed on `CGameObjectInstance` |
| 14 | `__scMain(sc=NULL)` | the scheduler's views (Panel, Sched, Vid, ...) overran each other on LP64 | **`port/game/t2sched.c`**: the 17 scheduler functions typed on `T2Sched`/`T2ScTask` (`structs/sched.h`), the originals left out of the link (`link.cmake`); `CFrameData.m_Task/m_LineTask` typed; `CEngineApp__SubmitGfxTask`, `CEngineApp__AdvanceFrameData`, `Video_SetMode` typed; `gRefreshRate` (`&gCurrentFB + 1`) is a variable of its own; `__amMain`'s client is two pointers |
| 15 | `SndVoicePool_*` from `Sound__StopAllExcept` | no sound bank (audio off) | `Sound__*` return early while `T2Port_AudioReady()` is false (`port/game/t2audio.c`: `gAudioWorld.bank != NULL`), plus a guard in `CAudioWorld__StopAllSounds` |
| 16 | `LevelEvent_Fire(NULL)` | the delayed level events at CScene+0x1180 (CLists + 0x30 nodes) as bytes | `CDelayedEvent` + typed fields in `CScene` (TARGET_PORT); the 4 users typed |
| 17 | `CPlayerPool__RegisterActors`, `CScene__BuildActorLists` | m2c offsets (0x54, 0x2DC, 0x8EC...), settings-relative 0x1270/0x1210, camera 0x2D8 | by field |
| 18 | `CCameraPool__AdvanceText` | camera text/shake/camera/emitter lists `0, 4`; texts block 0x400; `m_SkySystem` was `u8[0x8D0]` but holds a `CEmitterPool` of pointers | links by `T2_OFFSETOF`, `16 * sizeof(CCameraText)`, `CEmitterPool m_SkySystem[1]` (camera.h, TARGET_PORT) |
| 19 | Fast3D `G_MOVEMEM` with data 0 | `CEngineApp__BeginDisplayList` wrote Gfx through junk_types' `DlCmd {u32 w0, w1}` | `DlCmd` is two host words under TARGET_PORT |
| 20 | Fast3D `G_MOVEMEM` at 0x800AB0D8 | static display lists (gRspInitDL...) hold N64 addresses in w1 | gendata: Gfx data (`GfxWord` pairs) get host pointers in w1 for G_VTX/MTX/MOVEMEM/DL/RDPHALF_1/SETxIMG and G_MOVEWORD segment (10 lists) |
| 21 | `Menu_Construct`, `Menu_Update`, `Menu_TextWidth` | menu windows/items/descs/model blocks by N64 sizes (0x1D8/0x28/0x24/0x330), colours by offset, pool links 0x1D0/0x1D4; desc item tables generated as one element; desc `data` (s32) holding N64 text addresses | macros `MENU_SIZE`... (func_00413230.c); gendata types a pointer target as a table (the count stored next to the pointer, e.g. MenuDesc.items/count; never over a C-declared symbol; 88 tables); `Menu_Construct` maps an N64 address in `data` with `t2_n64_to_host` |

### For the merge

- Files others touch: `src/us/code/func_00203068.c`, `func_002044A8.c` (identical to r2's; this
  branch takes r2's version), `func_00205D7C.c` (r2's typed allocation renamed
  `CCache__AllocDecompressFlags` + wrapper), CList literal offsets in `func_0042E5AC.c`,
  `func_0042E834.c`, `func_00201CC0.c`, `func_0042F9B0.c`, `func_0042FBA4.c`, `func_0042FFF0.c`,
  `func_0042B3E4.c`, `func_00413230.c` (r3's CList sweep does the same lines: take either, they
  compute the same offsets), `include/rw/junk_types.h` (DlCmd), `include/structs/{game,camera,
  world,frame,sched}.h`, `include/globals/framebuffer.h`, `tools/port/gendata.py` (unions, bss
  slack, Gfx relocations, pointer tables), `port/cmake/{game,link}.cmake` (port/game/*.c and the
  scheduler files), `port/app/game.cpp` (T2_SHOT, T2_DLDUMP), 24 `Sound__*` files (guard).
- Run with r2's archive (format 2.0, t2/swap): `/root/R3_r2/out/turok2.o2r` or rebuild it with
  r2's mkarchive. The C17 archive (format 1) no longer loads with r2's reader.
- After changing a header, rerun gendata with a fresh cache (`rebuild.sh --fresh`): the aliases
  and sizes come from the types.
- Several fixes truncate pointers through `s32` prototypes on purpose (low-memory Linux):
  `CCache__AllocateMemoryForDecompression`, `CScene__GetRegion`, `gDisplayList0/1` (s32), `CFrameData.m_pFrameBuffer`
  (u32). r5 (macOS, no low memory) will hit those; they are r3's prototype typing.

### Next

1. Fonts and text: why the menu's text measures 0 and draws nothing (COnScreen fonts, the HUD text
   path, texture loads: the first frame with textures). Then the pak menu with input (PIF/SI:
   `__osSiRawStartDma` is not modelled, so the game sees a damaged pak).
2. The swoosh/effect system (CEngineApp 0x19608-0x22BA0) typed, instead of left empty.
3. Audio (r4): the `Sound__*` guard goes away by itself once `gAudioWorld.bank` is set.
4. The remaining m2c views on the draw path (`T2_DLDUMP` shows what the frame lacks).
