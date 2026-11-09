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

## Round 3, r3: 64-bit part 2 (the typing sweep)

Branch `names-R3_r3`. Every commit has `tools/asm_diff.py --base 3798e01e` at
`3938 files: 0 differ or fail`, `check_structs.py structs/game_object.h` at 0 wrong, and
`turok2_game` / `turok2_data` building with 0 errors. Details and numbers are in 64BIT.md, "Part 2".

- `tools/port/retype.py` (new) does the mechanical work at the columns clang reports.
  It turns `(s32)ptr` into `(sptr)`, retypes locals that carry pointers, drops casts in
  front of pointer parameters, and retypes the `s32` parameters and returns of functions
  that callers fill with pointers (rule P, every declaration). 880 files changed.
- Truncating pointer-width warnings went from 2,828 to 1,048 (clang, x86_64). The
  disagreeing declarations went from 1,059 to 999.
- The GfxPool path is pointer-wide: `CCache__AllocateAt`, `B12Node.key`, `MemHandle.addr`,
  and `gDepthBuffer`/`gDisplayList*`/`gLineList*`/`gAudioHeapBase` are now `sptr`. r5 saw the
  macOS run pass `GfxPool_Alloc`.
- All `CList__Construct` link offsets are now `T2_OFFSETOF`/`sizeof(void *)`.
  `MenuPool_Construct` is typed, and `CInstancePool__Construct` has a typed host body. r5
  saw the macOS run pass `MenuPool_Construct`. It next stops in
  `CCamera__ConstructSkySystem(+0x558)`, which is r1's area.
- `CAIModeInfo`: `m_Mode` (the header row's next-table link), `unk_04` (enter) and `unk_10`
  (anim id or function) are now `sptr`. gendata relocates a pointer-wide integer whose value
  is a known address. 27 runtime structs inside `.libultra` are now `RUNTIME_BSS`.
  Unresolved pointer slots went from 403 to 24, and address-holding integers from 334 to 125.
  All `*ModeTable`s are now relocated.

Merging: `include/globals/frame.h`, `include/rw/junk_types.h`,
`include/structs/game_object.h` (CAIModeInfo only, not CEngineApp/CGame), several
`include/protos/*.h` and about 880 files in `src/us/code` changed in one-token edits.
Conflicts with r1/r5 should be small and local. Rerun gendata with a fresh `--cache`,
because global and struct sizes changed.
