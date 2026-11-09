# Native build, step 1: the game C on the host (Phase 7, p1)

The goal of this step: compile every C-origin file of the game with clang for
a 64-bit little-endian host, as a static library, **without editing `src/`**,
and measure what stands between that library and a linked, running port.

Result: **3917 of 3917 compiled files build, 0 errors**, on three targets:

| target | compiled | errors | build time (32 cores) |
|---|---:|---:|---:|
| x86_64-linux-gnu (Debian 13, clang 19.1.7) | 3917/3917 | 0 | ~9 s, `libturok2_game.a` 7.4 MB |
| arm64-apple-macos13 (clang 19 cross, `-target`) | 3917/3917 | 0 | |
| x86_64-pc-windows-msvc (clang 19 cross, LLP64) | 3917/3917 | 0 | |

`src/us/code/` holds 3926 `.c` files: 3917 build, 9 are left out on purpose
because their code is the N64 itself (libgcc, CP0, SN64 varargs; see "Host
replacements" below). The macOS and Windows targets are cross-compiled on the
Linux box (the game C includes no system header, so no SDK is needed); a
build on a real Mac is still to do, but the object files are real Mach-O
arm64 and showed one real Mach-O problem (asm labels, fixed below).

The matching build is untouched: `python tools/asm_diff.py --base 8fb13231`
reports `3938 files against 8fb13231: 0 differ or fail` after the two
`include/` edits (both inside `#ifdef TARGET_PORT`).

## How to build

```sh
cmake -S port -B <out> -G Ninja -DCMAKE_C_COMPILER=clang
ninja -C <out> -k 0 turok2_game
# cross check another host ABI (no SDK needed):
cmake -S port -B <out>-mac -G Ninja -DCMAKE_C_COMPILER=clang \
      -DCMAKE_C_FLAGS="-target arm64-apple-macos13" \
      -DCMAKE_TRY_COMPILE_TARGET_TYPE=STATIC_LIBRARY -DCMAKE_AR=$(which llvm-ar-19)
```

Options (port/cmake/game.cmake): `T2_WARN_64BIT` (default ON: only the
64-bit hazard warnings), `T2_WARN_ALL` (every default clang warning), both
OFF: `-w`. `T2_BUILD_GAME_LIB` (port/CMakeLists.txt) turns the library off.

Files:

| file | role |
|---|---|
| `port/CMakeLists.txt` | top level; includes the pieces under `port/cmake/` (p3's LUS executable plugs in next to it) |
| `port/cmake/game.cmake` | `turok2_game`: sources, exclusions, flags, the hostify step |
| `port/include/port_shim.h` | force-included (`-include`) first in every game file |
| `port/include/globals/ai.h` | shadow header (port/include is first on the path) |
| `port/tools/hostify.py` | writes a filtered host copy of the 34 files that need it |

Compiler requirement: **clang** (or clang-cl on Windows). The flags below are
clang's; GCC 14 would need `-fpermissive` and drops `-Wshorten-64-to-32`;
MSVC `cl.exe` cannot take GNU C89 with statement expressions and asm labels.

## Flags and why

| flag | why |
|---|---|
| `-std=gnu89` | the code is GCC 2.8 C: K&R-style declarations, implicit int |
| `-fsigned-char` | MIPS GCC's `char` is signed; arm64 (macOS, Linux) defaults to unsigned. Without it every `char` comparison against a negative value changes meaning on the Mac |
| `-fno-strict-aliasing` | `M2C_FIELD`, s32/f32 views, `*(s32 *)((s8 *)p + 4)` everywhere |
| `-fwrapv` | SN64 arithmetic wraps; the decomp relies on it in hashes/RNG |
| `-fcommon` | tentative definitions repeated across files (SN64 linked them as COMMON) |
| `-Wno-error=int-conversion,incompatible-pointer-types,implicit-function-declaration,implicit-int,return-type` | clang 16+ makes these errors in C; they stay warnings so they can be counted (they are the 64-bit hazards) |
| `-DTARGET_PORT=1 -D_LANGUAGE_C -DF3DEX_GBI_2 -DGAME_VERSION="us"` | the N64 build's defines plus the port switch |

Include order: `port/include`, then `include/`. The game files include only
project headers (`common.h`, `structs/*`, `globals/*`, `rw/*`, `gfx.h`); no
`<PR/...>` and no system header. So the "shim for the N64 headers" this step
expected is mostly not needed: the decomp already carries its own minimal
ultra types, and libultra/GBI come from the project's headers or from macros
local to each file (see GBI below).

## What had to be fixed (error categories, first run → now)

First run (plain clang, nothing shimmed): 678 of 3926 files failed.

| category | files | fix | where |
|---|---:|---|---|
| struct pads `char padN[0x4 - 0x0 - sizeof(char *)]` go negative with 8-byte pointers (`array is too large`) | 509 (via `rw/abu_types.h`) | the 10 pads use `RW_PTR_SIZE`: `4` under `TARGET_PORT`, `sizeof(void *)` otherwise | `include/rw/abu_types.h` (guarded, asm_diff 0) |
| `-Wint-conversion` etc. are errors in clang 16+ | ~150 | `-Wno-error=...`, counted as hazards below | game.cmake |
| MIPS register pins `register f32 s asm("$f4")` (unknown register) | 27 | host copy drops the pin | hostify.py |
| empty asm barriers `__asm__("" : "+f"(tmp))`: `"f"` is an x87 stack register on x86-64 and **crashes clang 19** ("Cannot pop empty stack") | 7 | host copy drops the statement | hostify.py |
| `extern struct CAIModeInfo tbl[33]` with the struct only forward-declared (GCC 2.8 accepts it) | 24 | shadow `port/include/globals/ai.h` includes `structs/game_object.h` first, then `#include_next` | port/include |
| `typedef unsigned size_t` clashes with the host's (Windows predeclares it as 64-bit) | 4 | `typedef __SIZE_TYPE__ size_t` under `TARGET_PORT` | `include/rw/ultralib_types.h` (guarded, asm_diff 0) |
| `__attribute__((section(".text_vm")))`: Mach-O requires `"segment,section"` | 2 | host copy drops it (one copy of everything on the host) | hostify.py |
| asm labels `extern void f(void) __asm__("Name")`: taken verbatim, Mach-O wants `_Name` → 112 labels in ~30 files would be unresolved at link on macOS (they compiled; `llvm-nm` showed `U MenuPool_GetCamera` next to `T _MenuPool_GetCamera`) | ~30 | host copy writes `__asm__(T2_LABEL_PREFIX "Name")`; `T2_LABEL_PREFIX` = stringified `__USER_LABEL_PREFIX__` | hostify.py + port_shim.h |
| MIPS-only code: `multu` (libgcc longlong.h), `mtc0`/`mfc0`, `sqrt.s`, `break 1`, `__builtin_next_arg` | 9 | excluded; host replacement (below) | game.cmake `T2_GAME_EXCLUDE` |

hostify is a build step (`add_custom_command` into `<out>/hostified/`, with a
`#line` back to the original so diagnostics and debuggers point at `src/`).
CMake picks the files by regex at configure time: 34 files today. The
alternative, a `REG_PIN("$f4")`-style macro edited into `src/`, is cleaner
in the long run and byte-identical on N64, but touches 34 matched files;
recommended only if the src tree is going to be edited for the port anyway
(the pointer-type work below will).

## 64-bit hazard warnings (what compiles but will not run)

Counted on the Linux build with the default `T2_WARN_64BIT` set, unique
diagnostic lines (each is a source site; headers produced none):

| warning | sites | typical code |
|---|---:|---|
| `-Wpointer-to-int-cast` | 1029 | `return (s32)t + *(s32 *)((s8 *)t + (i << 2) + 4);` / `lo = (u32)f + f->used;` |
| `-Wint-conversion` | 335 | `arg0->m_pTail = value;` (s32 into a pointer field) / `int node = arg0->m_pHead;` |
| `-Wint-to-pointer-cast` | 256 | `temp_v1 = *(s32 *)(arg1 + arg0->m_LastOffset);` (an s32 used as an address) |
| `-Wvoid-pointer-to-int-cast` | 186 | `temp_v1_2 = (s32) arg0->m_pHead;` |
| `-Wint-to-void-pointer-cast` | 92 | `arg0->m_pTail = (void *) arg2;` |
| `-Wincompatible-library-redeclaration` | 90 | `rw/abu_protos.h` declares `memset/memmove/memcmp/strlen/strchr/sprintf` with the N64 types (size_t = u32) |
| `-Wshorten-64-to-32` | 40 | `len = (u8 *)gfx - (u8 *)start;` (ptrdiff into s32: harmless) / `Crash_PrintRegPair(..., context->v0, ...)` (u64 regs) |
| `-Wimplicit-function-declaration` | 4 | `memcpy` (2), `alLink`, `alCSPNew`: an implicit int return truncates a pointer |

**~1,900 sites in 785 files** hold or move a pointer through a 32-bit integer.
Most are m2c-era signatures (`void CList__AddTail(CList *arg0, s32 arg1)`
where arg1 is a node pointer) and `s32` locals; the fix is typing them as
pointers in `src/` (byte-identical on N64, provable with asm_diff), which is
p2's audit. Where they concentrate (site count, files, by the name prefix of
the file's function):

| subsystem | sites | files |
|---|---:|---:|
| EnemyAnim | 129 | 61 |
| PakMenu | 127 | 20 |
| unnamed `func_*` | 101 | 46 |
| CList (the intrusive list, used everywhere) | 100 | 10 |
| CScene | 88 | 46 |
| libultra (replaced anyway) | 78 | 29 |
| CCache | 75 | 22 |
| MenuItemDesc | 62 | 2 |
| CPlayer | 61 | 28 |
| Render / GfxPool | 68 | 10 |
| BlindOne, Primagen, MotherBoss (boss AI, both module copies) | 92 | 58 |

Not caught by any warning, and worse: **offset arithmetic over structs that
change layout on 64-bit**. Every struct with a pointer field grows; code that
reaches fields by literal offset still reads the N64 offset. 378 sites of
`(s8 *)p + 0x..` in 165 files and 62 `M2C_FIELD` in 9 files. Those must be
rewritten as member accesses (or the structs kept 32-bit, see decisions).

## Link readiness (what the library does not define)

`nm` over `libturok2_game.a`: 4137 defined global functions, **1789 symbols
referenced but not defined**. Classified against `build/turok2.us.elf`:

| kind | count | what the native build needs |
|---|---:|---|
| data in `.code_rodata` / `.code_rodata_tail` | 531 | engine .rodata (literal pools, tables, strings) still lives in `us/asm/data/*.s`: C definitions |
| data in `.libultra` (+ tail) | 142 | libultra .data/.bss (replaced with libultra) |
| data in `.virtual` / `.virtual_1` | 126 | VM module data (menu descriptors `g*MenuDesc`, ...) |
| absolute, KSEG0 (0x800xxxxx) | 814 | .bss/.data the linker defines by address (`D_800A51E0`, `CartRomHandle`, ...): need real storage |
| absolute, VM addresses (0x0040xxxx) | 109 | module-2 references to module-1 labels by address (docs/VMASM.md): disappear if one module copy is linked |
| absolute, ROM offsets/constants | 6 | `D_14A000`, `D_29E60`, ...: ROM offsets → archive keys (docs/assets/PORT.md) |
| functions in `.virtual_1` still asm | 26 | module-2 copies (`BlindOne_Draw_2`, `Primagen_*_2`, ...): not needed if only module 1 links |
| functions, asm by origin / excluded | 32 | the list below |
| `memcpy` | 1 | host libc |

So the next blocker after typing is **data**: ~1,600 data symbols are defined
today by assembly or by absolute addresses in the linker script, not by C.
Their contents hold N64 pointers (jump tables, descriptor records, function
pointer tables), so they cannot be pasted as bytes: they need a generator
that emits them as C with symbolic relocations (splat/the ELF relocations
know which words are pointers).

Libc name clashes: the library defines `memcmp memmove memset strchr strlen
lldiv __assert` (plus excluded `sprintf`). Linked with a host libc they
collide or, worse, silently replace the host's with the N64 size_t
versions. The link step should rename or drop them (the game's `memset` etc.
are SDK libc copies).

## Host replacements: asm-only code and excluded files

None of these can be compiled for the host; each needs a host function or a
no-op. From docs/ASM_ORIGIN.md (47 entries) and the 9 excluded C files:

| group | functions | host replacement |
|---|---|---|
| SN64 boot / TLB | `boot_main`, `STVM_MapTLB` (boot.s), func_0028EE40/func_0028EE44 (engine STVM_MapTLB), func_0028EEC0 (unmap), func_0028EEFC (TLB dump), `__osUnmapTLBAll`, `__osProbeTLB` | no TLB on the host: `main()` replaces romMain; map/unmap become no-ops once one VM module copy is linked at its real address (p4) |
| RNC unpacker | func_00200C40 (`RNC_UnpackMethod1`), func_00200CB0, func_00200E1C, func_00200EC0, func_00200EE8, func_00200F20, func_002010C0 (`RNC_UnpackMethod2`) | `port/third_party/rnc/rnc.c` (already used by t2asset) |
| libultra thread/exception core (exceptasm.s) | `__osException`, `send_mesg`, `__osEnqueueAndYield`, `__osEnqueueThread`, `__osPopThread`, func_0029C8E0 (`__osDispatchThread`), func_0029CADC (`__osCleanupThread`) | the whole OS thread/message layer is replaced by host threads or a cooperative scheduler (p4); these go with it |
| CP0 / FPU / interrupts | func_0029DFD0 `__osGetFpcCsr`, func_0029E220 `__osSetFpcCsr`, func_0029DFE0 `__osGetSR`, func_0029E2E0 `__osSetSR`, func_0029DFF0 `__osDisableInt`, `__osRestoreInt`, `osSetIntMask`, func_002A31E4/func_002A31F0 (SR get/set) | no-ops (or a host mutex for DisableInt/RestoreInt if host threads are used) |
| caches | func_0029E030 `osInvalDCache`, func_0029E0D0 `osInvalICache`, func_0029E340 `osWritebackDCache`, func_0029E3B0 `osWritebackDCacheAll`, func_002A31AC, func_002A31C8 | no-ops |
| timer | `osGetCount` (func_0029DFC0.c, excluded), `__osSetCompare` (func_0029E210.c, excluded) | host monotonic clock scaled to 46.875 MHz; Compare unused |
| watch registers | func_00201074, func_00425710 (copy) | no-op |
| crash handler | func_002A3088 (vector stubs), func_002A3100, func_002A31FC, func_002A3708, func_002A3774, func_002A37CC | host crash handler (signal handler) or nothing |
| libgcc (excluded) | func_002A3AE0 `__divdi3`, func_002A40D0 `__moddi3`, func_002A4650 `__udivdi3`, func_002A4BE0 `__umoddi3` | the host compiler's runtime |
| libc (excluded) | func_0029E3E0 `sprintf`, func_00298470 `sqrtf` | host libc/libm. `_Printf` (func_0029E4F0), `_Putfld`, `_Litob`, `_Ldtob` compile but walk N64 varargs by hand (`(u32)args + 3 & ~3`): replace with `vsnprintf` |
| SN debug host I/O (excluded) | func_002A259C `HostIo_Call` (+ `HostIo_ReadWord/WriteWord`, compile but poke 0xA4xxxxxx) | stub returning failure |
| dead fragments | func_002C22D8, func_002C232C, func_002C23E0 | nothing (dead copies) |

Beyond the asm: **124 libultra functions compile but drive N64 hardware**
(25 files poke 0xA4xxxxxx registers: VI, AI, SP, DP, SI, PI). They are in the
library today so the count is honest; at link time they must be replaced by
the host layer (LUS for VI/controller/AI where it can, our own for the
rest). Names: `osCreateThread osStartThread osRecvMesg osSendMesg osJamMesg
osSetEventMesg osCreateMesgQueue osSetTimer osGetTime osCreateScheduler
osScAddClient osScGetCmdQ osViSetMode osViSwapBuffer osViGetCurrentFramebuffer
osViBlack osViSetSpecialFeatures osViSetYScale osAiSetFrequency
osAiSetNextBuffer osAiGetStatus osSpTaskLoad osSpTaskStartGo osSpTaskYield
osDpSetNextBuffer osPiStartDma osPiRawStartDma osEPiRawStartDma osCartRomInit
osContInit osContStartReadData osContGetReadData osContStartQuery
osContGetQuery osContReset osMotorInit osMotorStart osMotorStop osPfs*
(11) osLeoDiskInit osVirtualToPhysical osMapTLBRdb osInitialize` and 66
`__os*` internals (full list: `nm` the library, `grep -E '^_*os'`). 99
libaudio functions (`al*`, `n_al*`) also compile; those are pure CPU code and
can stay (p4: run the synthesizer on the CPU).

## GBI: what the game actually uses

The game does not include `PR/gbi.h`. `include/gfx.h` defines `Gfx` as two
`u32` words and the engine writes commands as raw words: 474 `w0`/`w1`
writes in 89 files, and 10 files define their own `gDP*/gSP*` macros
(`_GBI_CMD(pkt, 0xE200001C, mode)`, `gDPSetTextureImage(..., (unsigned int)(img))`).
34 files use those macros. Consequences:

- A host `gbi.h` in `port/include` cannot intercept anything: the macros
  are defined inside the `.c` files, after the includes.
- Every address that goes into a display list is cast to a 32-bit integer
  (`(unsigned int)(img)`, `w1 = (u32)ptr`): ~180 such `w1 = (unsigned int ...)`
  sites. LUS's Fast3D takes `Gfx` with `uintptr_t` words on 64-bit hosts
  (p3: `include/fast/lus_gbi.h`), so a pointer only fits if the game's
  `Gfx` becomes LUS's.

Recommendation: (1) move the file-local GBI macros into one project header
(say `include/gbi_t2.h`) that defines them with `GBI_ADDR(x)` for the
address operand, as an asm_diff-proven refactor; (2) under `TARGET_PORT`,
`include/gfx.h` and that header become LUS's `Gfx`/`gbi.h` (words are
`uintptr_t`, `GBI_ADDR(x)` is `(uintptr_t)(x)`); (3) the raw `w0`/`w1`
writes then need `(uintptr_t)` where they carry addresses (the
`-Wpointer-to-int-cast` list finds them). Segment addresses (`0x0Xnnnnnn`)
stay integers and Fast3D resolves them through `gSPSegment` as on N64.

## Decisions I recommend

1. **Type the pointers in `src/`, don't hide them.** ~1,900 warning sites +
   ~440 literal-offset accesses. Each change is byte-identical on N64 and
   provable with `tools/asm_diff.py`; the warnings list is the worklist
   (`T2_WARN_64BIT`). The alternative, a 32-bit arena (all game memory below
   4 GB so `s32` holds a pointer), works on Linux/Windows with
   `MAP_32BIT`/`VirtualAlloc` hints but not on macOS arm64, whose
   `__PAGEZERO` reserves the whole low 4 GB: it would block the primary
   target.
2. **Keep the hostify step** for pins/barriers/labels/sections until `src/`
   is edited anyway; then fold them into macros (`REG_PIN`, `ASM_LABEL`) and
   delete hostify.
3. **Link one VM module copy.** 629 files are module-2 (`.virtual_1`)
   copies of module-1 code (793 files are `.virtual`, 2494 `.code`). The
   native lib should take `.code` + `.virtual` and drop `.virtual_1` (its
   26 asm-only `_2` functions and 109 absolute cross-module labels go with
   it), unless p4 finds module-2 code that differs and is the one that runs.
   A configure-time list (from `versions/` symbol sections) can do that.
4. **Data is the next big item**: generate C for `.code_rodata`, `.virtual`
   data and the absolute-address .bss/.data (~1,600 symbols) with symbolic
   relocations.
5. **Build with clang everywhere** (clang-cl on Windows), `-fsigned-char`
   mandatory.

## What the next round needs

(Round 2: the data generator exists: docs/port/DATA.md. Module 1 + generated data + a stub
main link with 33 unresolved symbols, all function side.)

- Exclude the libultra/hardware files and the module-2 copies from
  `turok2_game` (lists derivable from the ELF sections), and add a
  `port/src/host_ultra.c` with the replacements above, then attempt a link
  with a stub `main` to get the real unresolved list.
- Data generator for the ~1,600 asm/absolute data symbols.
- Start the pointer typing on the core types first: `CList` (100 sites,
  used by everything), `CCache`, `CScene`, the GBI unification.
- Build once on a real Mac (arm64) to confirm the cross-compile result.
