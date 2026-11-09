# Linking and booting the game on the host (Phase 7, round 2, part q5)

`turok2` now links the game: the engine and VM module 1 (`turok2_game`), the port runtime
(`t2runtime`: t2os threads, the libultra device shims, the boot glue) and, last, a generated
library of WEAK placeholders for the game data that is still defined only in assembly
(`t2placeholders`, until q2's generated data lands). `turok2 --game` boots it and pumps frames.

**Merged state (end of round 2): [STATUS.md](STATUS.md).** q2's data is linked (`turok2_data`,
placeholders 597 -> 1), RNC goes to `port/assets/rnc.c`, and the run gets past the CHeap /
CCache / GfxPool / synthesizer crashes below to `CCameraPool__Construct`. §6 and §7 describe
q5 alone.

## 1. Build and run

```sh
cmake -S port -B OUT -G Ninja -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++
ninja -C OUT -j8 turok2
SDL_VIDEODRIVER=offscreen OUT/app/turok2 --archive OUT/turok2.o2r --rom baserom.us.z64 \
    --game --frames 120 [--no-window] [--draw] [--verbose] [--high-memory]
# first crash with a backtrace of every thread:
gdb -batch -ex run -ex "thread apply all bt 25" --args OUT/app/turok2 ... --game --no-window
```

Options (port/app/CMakeLists.txt): `T2_LINK_GAME` (ON: link the game into `turok2`; OFF gives
p3's skeleton), `T2_LOW_MEMORY` (ON on Linux: bring-up aid, §5), `T2_BUILD_UNLINKED`
(game.cmake, ON: also compile the files that are not linked, so "every C file builds" stays
checked).

## 2. What is linked

| piece | files | where |
|---|---:|---|
| engine (`.code`) + module 1 (`.virtual`) game C | 3,226 | `turok2_game` |
| module 2 (`.virtual_1`, dead: RUNTIME.md §1.4) | 629 | `turok2_game_unlinked` (compile only) |
| game libultra/boot C replaced by port/runtime | 62 | `turok2_game_unlinked` (compile only) |
| N64-only files (libgcc, CP0, SN varargs, host I/O) | 9 | not compiled (game.cmake `T2_GAME_EXCLUDE`) |
| t2os, device shims, boot glue | 3 | `t2runtime` (port/runtime) |
| WEAK placeholders (generated) | 1 | `t2placeholders` (port/runtime/t2_placeholders.c) |

The split is `port/cmake/link.cmake`: `T2_LINK_REPLACED` lists the replaced files with the
function each one holds, `t2_is_module2()` recognises module 2 (`func_0043CC14` ..
`func_00469374` and the four `*_2.c`). The static library only contributes what `mainproc`
reaches, so dead engine code (crash screen, SN host I/O, the VM pager threads, ...) drops out
by itself.

### Names: t2_rename.h

`port/include/t2_rename.h` (force-included into every game file through `port_shim.h`) renames
every libultra function the port implements and the game's libc copies:
`#define osSendMesg t2_osSendMesg`, ..., `#define memset t2_memset`, `#define sprintf
t2_sprintf`. Three reasons:

- libultraship's static library defines `osSendMesg`, `osRecvMesg`, `osCreateMesgQueue`,
  `osContInit`, `osViSwapBuffer`, `osSetTimer`, `osGetTime`, ... (non-blocking or no-op; 41
  `os*` symbols). Under the game's names, whichever object the linker met first would win on
  one platform and collide on another.
- the game defines `memset memmove memcmp strlen strchr lldiv __assert` with the N64 types
  (`size_t` = u32). Linked under those names, the executable's copy would interpose the host
  libc for every caller, LUS and SDL included. Renamed, the game uses its own SN64 copies and
  everything else the host's.
- libultraship's own `osContInit`/`osContGetReadData` stay reachable from port code
  (`port/app/game.cpp` does not include the header) for the controller hooks.

Data names (`osMemSize`, `osTvType`, `__osContPifRam`, ...) are not renamed: they are game
data (q2's generator, or the placeholders), and the LUS objects that define LUS's versions
are only pulled in through LUS functions the game no longer names.

## 3. The runtime (port/runtime)

| file | what |
|---|---|
| `t2os.c/.h` | p4's threads/queues/events (unchanged model), compiled under the t2_ names; new: `t2os_set_stack_allocator` |
| `t2ultra.c` | device layer: VI (retrace per frame, swaps recorded), SP/DP (tasks recorded, gfx task to the app, SP+DP done posted at the next frame), AI (FIFO reported full: no audio yet), PI (raw DMA from the ROM image + OS_EVENT_PI), SI (controllers via the app's hooks on LUS, OS_EVENT_SI after every StartReadData/StartQuery, no pak, no rumble), virtual clock (46.875 MHz per retrace, deterministic), `osSetTimer` list, caches/TLB/FPU/interrupt no-ops, `t2_missing` |
| `t2boot.c` | `T2Port_Boot`: arena, ROM, IPL data (`osMemSize` = 8 MB so `GetMemSize` takes the expansion-pak paths, `osTvType` NTSC), thread priorities, `mainproc` on `gThreadMain` (id 1, pri 13) without the idle thread; replaced game functions `InitializeSystemResources` (heap = host arena, 32 MB), `VM_Initialize` (empty), `VM_Alloc`/`VM_Free` (resident copy from the ROM), `RNC_UnpackMethod1/2` (`port/assets/rnc.c` since the merge), `t2_sprintf` (host vsprintf) |
| `t2port.h` | the platform API: `T2Port_Boot`, `T2Port_Frame`, `T2Port_TakeGfxTask`, `T2Port_GetStats`, controller hooks |
| `t2_placeholders.c` | GENERATED (tools/port/genplaceholders.py) |

Frame pump (`port/app/game.cpp`, platform thread): `HandleEvents` -> `T2Port_Frame` (post the
SP/DP completions owed by last frame's tasks, due timers, one VI retrace, `t2os_run_until_idle`)
-> `T2Port_TakeGfxTask` -> with `--draw`, `DrawAndRunGraphicsCommands(task.data_ptr)`.
`--draw` is off by default: the display lists are not host-correct until q4's GBI work.

## 4. Placeholders

`tools/port/genplaceholders.py` (procedure in its docstring) reads the undefined references of
a link without placeholders, sizes data from the N64 ELF and writes
`port/runtime/t2_placeholders.c`. Today (box, 2026-11-09): **597 undefined symbols in the
linked closure**, down from p1's 1,789 undefined over the whole library (that count included
module 2, the hardware libultra and everything unreachable from `mainproc`):

| category (N64 ELF) | symbols |
|---|---:|
| engine .rodata (`.code_rodata*`: tables, literal pools, thread priorities, `gEngineModeTable`) | 270 |
| absolute KSEG0 (.data/.bss the linker script places by address: `gEngineApp`, `gScene`, `gHeap`, `gLoader`, `gCache*`, `gPager*`, ...) | 220 |
| libultra .data (`__osContPifRam`, `__osViContexts`, `osViModeTable`, ...) | 97 |
| absolute ROM offsets / constants (`D_11059`, `D_29E60`, `D_94F`, `D_A4694`) | 4 |
| module-1 labels by VM address (`func_00438808` = `gCharacterMenuDesc`, `func_004388E0`, `func_0043916C`: menu descriptors named like functions) | 3 |
| module-1 data (`gCollisionSetupTable`) | 1 |
| `func_00294D54` (a stray asm word used as data by `alFxNew`) | 1 |
| functions | 1 (`func_00298470` = `sqrtf`, now defined in t2boot.c) |

So the function side is closed: every function `mainproc` reaches is game C or port/runtime;
**the remainder is data only** (596 symbols, 623 KB of placeholders), which is q2's generator.
Classification is by relocation: a symbol some object calls (`R_X86_64_PLT32`) is a function.
Sizes: ELF size or distance to the next symbol, x2 + 64, at least 128 bytes: the first run with
x2 + 16 put `gHeap`'s 40-byte host `CHeap` over the next placeholder.

Values the boot needs before q2's data exists are written by `T2Port_Boot` only while they are
zero (thread priorities 10-18 from the ROM's `.code_rodata`, `osTvType`), so real definitions
win after the merge. `osMemSize` (= 8 MB) and `osResetType` (= 0) are always written: the IPL
sets them on the console.

## 5. Low-memory mode (bring-up aid, Linux only)

The game C still moves pointers through `s32`/`u32` (BUILD.md: ~1,900 warning sites; e.g.
`CHeap__Construct` aligns its start with `((u32)start + 15) & ~15`, `VM_Alloc` passes
`(s32)&stackVar` to `osCreateMesgQueue`). Until the typing work (64BIT.md, q3) is done, the
Linux build can make those truncations harmless: `-no-pie` keeps the executable's statics at
0x400000+, and `T2Port_Boot` allocates the 32 MB arena and every game thread's 8 MB stack with
`mmap(MAP_32BIT)` (below 2 GB, so an s32 round trip, even sign-extended, gives the pointer
back). `T2_LOW_MEMORY` (CMake, default ON on Linux) + `--high-memory` (run time) switch it.
It is not a design (macOS reserves the low 4 GB; struct layouts still widen) but it separates
"pointer truncated" crashes from "layout wrong" crashes during bring-up:

| mode | first crash |
|---|---|
| `--high-memory` | `CHeap__Construct` (func_00202474.c:13): `block` = `(u32)start` aligned, truncated host pointer |
| low memory (default) | `CHashTable__Construct` (func_00202860.c:35), one step later (below) |

## 6. How far it ran (q5 alone; merged: STATUS.md §2)

`SDL_VIDEODRIVER=offscreen turok2 --archive turok2.o2r --rom baserom.us.z64 --game --no-window
--frames 120` under gdb (box, -j8 build, 2026-11-09):

```
T2Asset over ArchiveManager: 26124 units in t2/units
t2: boot: arena 32 MB at 0x40200000 (low memory), ROM baserom.us.z64, mainproc started (pri 13)
game: booted, 120 frames
Thread 5 "turok2" received signal SIGSEGV, Segmentation fault.
#0 CHashTable__Construct (capacity=...) at src/us/code/func_00202860.c:35   gHashTable[n].value = 0;
#1 CCache__Construct (unused=..., id=5) at src/us/code/func_00202BB0.c:130
#2 CEngineApp__Main (...) at src/us/code/func_00286674.c:52
#3 trampoline (...) at port/runtime/t2os.c:157
other game threads: CLoader__Main blocked in t2_osRecvMesg(&gLoader.queue) (func_00201FBC.c:19),
CDecompressor thread (pri 11) created, not run yet; platform thread in t2os_run_until_idle
```

What works, in order: LUS context + archive + T2Asset backend; `T2Port_Boot` (arena, ROM,
priorities, `gThreadMain`); t2os runs `mainproc` on its host thread (`t2_sprintf` builds the
timestamp, FPCSR no-ops) -> `CEngineApp__Main` -> the port's `InitializeSystemResources` ->
`CHeap__Construct` on the arena -> `CDecompressor__Construct` and `CLoader__Construct` create
their threads (t2os ids 3 and 4, the loader blocks on its queue as on hardware: the baton
model works with the real game code) -> `VM_Initialize` (empty) -> `CCache__Construct` ->
`CHashTable__Construct` crashes. Mode: none yet (before `CEngineApp__SetMode(0)`); 0 frames
rendered, 0 placeholder functions called.

Why: `CHashTable__Construct` asks `CHeap__Alloc(&gHeap, half << 5)` and gets NULL. The heap's
block header `CHeapBlock` is 0x20 bytes on the N64 and 48 on the host (four pointers), but
`CHeap__Construct/Alloc/AllocTop/AllocAt/Free/GetFreeStats` (func_002021F0 .. func_0020280C)
hard-code 0x20 (`size = (request + 0x2F) & ~0xF`, `used = 0x20`), so the free list is
inconsistent from the first block. Then `HashEntry` (16 bytes on N64, 32 on the host)
overflows the `half << 5` allocation. Both are the 64-bit layout problem (BUILD.md "offset
arithmetic over structs that change layout"), not link or runtime problems.

## 7. What the next round needed (done or carried over: STATUS.md §3-4)

1. **CHeap host-correct** (6 functions): header size as `sizeof(CHeapBlock)` (the N64
   constant folds to the same 0x20/0x2F, asm_diff-provable) and pointer alignment through
   `uintptr_t`; then `HashEntry`'s allocation as `n * sizeof(HashEntry)`. Expect the next crashes
   in `CCache__Construct` (entry pools sized with N64 strides) and `GfxPool_Alloc`.
2. **q2's data** replaces the placeholders: rerun genplaceholders after the merge; the
   mode table (`gEngineModeTable`), the video-mode tables and the menu descriptors are the
   ones the boot reaches first (all zero now: the first call through `gEngineModeTable` would
   jump to NULL).
3. RNC: point `RNC_UnpackMethod1/2` (t2boot.c) at q1's `port/assets/rnc.c`, or better route
   the loader/decompressor to `T2Asset` (PORT.md §10.4) so the host never unpacks.
4. Audio stays off (`osAiGetStatus` = full) until the ABI1 HLE; Controller Pak not modelled
   (`__osSiRawStartDma` completes without data).
5. `t2_sprintf` relies on SysV x86-64 passing variadic arguments like fixed ones; arm64 macOS
   needs real prototypes at the `sprintf` call sites. Done in round 3 (r5, MACOS.md §2): the 13
   fixed-parameter declarations are variadic under `TARGET_PORT`.
6. `--draw` once q4's GBI lands; `T2Port_TakeGfxTask` already hands over `data_ptr`.

## 8. Files

`port/cmake/link.cmake` (new), `port/cmake/game.cmake` (split + `t2_game_flags`),
`port/include/t2_rename.h` (new), `port/include/port_shim.h` (includes it),
`port/runtime/t2ultra.c`, `t2boot.c`, `t2port.h`, `t2_placeholders.c` (new, last one
generated), `port/runtime/t2os.c/.h` (stack allocator hook, `u8`/`s8` in stand-alone mode),
`port/app/game.cpp/.h` (new), `port/app/main.cpp` (`--game` etc.), `port/app/CMakeLists.txt`
(t2runtime, t2placeholders, link group, `T2_LOW_MEMORY`), `tools/port/genplaceholders.py`.
`make -C port/runtime test` still passes (deterministic hash); `turok2` without `--game`
still passes p3's checks (26,124 units on three backends, test DL pixels).
