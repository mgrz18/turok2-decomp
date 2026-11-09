# macOS arm64 bring-up (Phase 7 round 3, part r5)

The native port builds and runs on macOS arm64 (Apple M5, macOS 27, Apple clang 21, Homebrew)
with **full 64-bit pointers**: the low-memory mode of the Linux build (LINK.md §5) cannot
exist here (below). The whole tree compiles and links with no game-side errors, the assets and
the runtime tests give the same results as Linux bit for bit, and `turok2 --game` stops at the
same line as Linux `--high-memory`.

## 1. Build and run

Prerequisites (Homebrew, already on the Mac): `cmake ninja sdl3 libzip glew libpng
nlohmann-json tinyxml2`. LUS fetches spdlog itself (1.16.0), so no extra `brew install` is
needed. The N64 toolchain is **not** needed: gendata only reads the matching ELF, copied from a
Linux box that built it.

```sh
# 0. the matching ELF (gendata's only N64 input) and the ROM
scp decomp:/root/turok2_decomp/build/turok2.us.elf build/
cp <rom> baserom.us.z64

# 1. python: pyelftools + libclang bindings for Apple's libclang (CLT 21)
python3.13 -m venv .venv && .venv/bin/pip install pyelftools 'clang==21.*'

# 2. the archive (or copy the box's: the container differs, the units do not, §3)
.venv/bin/python tools/assets/mkarchive.py baserom.us.z64 -o OUT/turok2.o2r -j 4

# 3. configure + game library (about 13 s for 3,917 files on 10 cores)
cmake -S port -B OUT/build -G Ninja -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ \
      -DCMAKE_BUILD_TYPE=Debug
ninja -C OUT/build turok2_game

# 4. generated data (about 10 s with -j 8; host triple and libclang default to the Mac's)
.venv/bin/python -I tools/port/gendata.py --build OUT/build --cache OUT/decls.json -j 8

# 5. executable
cmake OUT/build && ninja -C OUT/build turok2

# 6. run HEADLESS (never open a window on the user's Mac)
SDL_VIDEODRIVER=offscreen OUT/build/app/turok2 --archive OUT/turok2.o2r --rom baserom.us.z64 \
    --no-window --frames 60
SDL_VIDEODRIVER=offscreen lldb -b -o run -k "bt 25" -k quit -- OUT/build/app/turok2 \
    --archive OUT/turok2.o2r --rom baserom.us.z64 --game --no-window --frames 120
```

`--high-memory` has no effect on macOS: `T2_LOW_MEMORY` is Linux-only, and the arena already
comes from a normal allocation (seen at `0x72_00C0_0000`, statics at `0x1_006F_xxxx`).

## 2. What macOS needed (commits on `names-R3_r5`)

| issue | fix |
|---|---|
| gendata parsed the host layout with `-target x86_64-linux-gnu` and the libclang path of the box | `DEFAULT_HOST_TARGET` / `DEFAULT_LIBCLANG` per platform (`arm64-apple-macos13`, CLT `libclang.dylib`), `--host-target` option; `-arch X` dropped from the compile command (the target is explicit). The generated C is **identical** to the box's (all 7 files, apart from the alias `#if` lines): LP64 layouts agree |
| `nm` on Mach-O lists C names with the `_` prefix: gendata found no data to generate | strip `_` on Darwin |
| aliases (`gScene_m_nDrawActors` = `gScene + 0x8EC`, 226 of them) were 1-byte FIXME copies on Mach-O | `.globl X / .alt_entry X / .set X, base + off`. `.set` gives an `N_SECT` symbol at the right address. `.alt_entry` keeps it inside the base's atom, so `-dead_strip` and subsections_via_symbols cannot split it. Checked in the linked executable: **226/226 aliases at `base + host_offset`**, the same offsets as Linux |
| a Mach-O alias needs its base in a section: Apple clang still puts tentative definitions (`unsigned char gX[N];`) in common, where `.set` leaves the alias undefined | `turok2_data` is compiled with `-fno-common` (data.cmake) |
| `sprintf` declared with fixed parameters in 13 files (`void sprintf(char *, char *, s32)`, `void sprintf(void *, void *, void *, void *)` in `mainproc`). On arm64 Apple the variadic arguments go on the stack, so `t2_sprintf` read garbage. SysV x86-64 hid this | under `#ifdef TARGET_PORT` the declaration becomes variadic (`void sprintf(char *dst, const char *fmt, ...)`). The N64 text is the `#else` branch, unchanged. A scan of every declaration in `src/` + `include/` + `port/runtime` against the variadic definitions found **only `sprintf`**. `gEngineTimestamp` now reads `"Nov 10 1998 13:17:04"` |
| `port/runtime/Makefile test` reported "NOT deterministic" with the same hash 3 times | BSD `wc -l` pads its output: `tr -d " "` |

Nothing else was needed. The asm labels (`T2_LABEL_PREFIX`, BUILD.md) resolve. ld64 handles
the `t2runtime`/`turok2_game`/`turok2_data` cycle without a link group (app/CMakeLists.txt
already had the `APPLE` branch). LUS 2.0 builds with the Metal and OpenGL backends.
`data_abs.c`'s absolute symbols are not referenced from the linked closure. The link has one
warning: duplicate `libt2asset.a`, which is harmless.

## 3. Same results as Linux

| check | Mac | Linux (box) |
|---|---|---|
| `turok2_game` compile | 3,917 files, 0 errors | same |
| gendata report | 1,562 symbols (1,555 typed, 7 bytes), 277 targets, 782 bss, 226 aliases, 2,624 pointer slots (1,479 / 742 / 403) | same |
| `turok2` (no `--game`) | 26,124 units on 3 backends, fnv1a64 `6a391cadd4476aa6`, OK | same hash |
| `t2asset_test` / `rnc_test` | 0 mismatches / 14,426 RNC streams, 0 failed | same |
| `t2os_test` | trace hash `2b2775877e0d226b`, deterministic | same hash |
| `gbi_test` (game GBI vs LUS) | 0 mismatches | same |
| `mkarchive.py` | sha1 `739f60bd…`, 33,875,901 bytes | `bb77f744…`, 33,875,898 bytes |

The archive differs only in the deflate streams. Python's zlib is 1.2.12 on the Mac and 1.3.1
on the box. Every unit reads back identical (same fnv1a64, `t2asset_test` 0 mismatches against
the ROM). The archive's sha1 is therefore pinned to the zlib version, not to the platform.
To get a byte-exact archive, store the units uncompressed or bundle a fixed deflate.

## 4. How far `turok2 --game` gets (full 64-bit)

```
t2: boot: arena 32 MB at 0x7200c00000, ROM baserom.us.z64, mainproc started (pri 13)
game: booted, 120 frames
thread #5 EXC_BAD_ACCESS (address=0x0)
#0 GfxPool_Alloc (pool=&gPoolBuffers) func_00425220.c:86   memset((void *)fb->addr, 0, 0xFD200)
#1 CEngineApp__Main func_00286674.c:55
#2 mainproc func_00288860.c:13
```

Up to that point everything runs: boot, `InitializeSystemResources` (CHeap on the arena, decompressor and
loader threads, `CCache__Construct`). Linux `--high-memory` on C17out stops at **the same line**.
Linux low-memory gets further (CCameraPool__Construct) only because truncated `s32` pointers
survive below 2 GB. Pointers on macOS are above 4 GB (`__PAGEZERO`), so every `s32`-held pointer
breaks.

**No low-memory mode on macOS (checked).** A binary linked with `-pagezero_size 0x4000` is
SIGKILLed at exec. `-image_base` is ignored, since arm64 is always PIE. `mmap(0x40000000,
MAP_FIXED)` fails. The typing work (64BIT.md) is the only route.

### Probe: the next crashes (branch `r5-probe`, NOT for merge)

To map what comes next, I fixed five truncations under `TARGET_PORT` on a side branch. asm_diff
was not run, and r3 owns this code. Each row is the crash that the previous fix exposed:

| # | where | cause | probe fix |
|---|---|---|---|
| 1 | `GfxPool_Alloc` :86 | `CCache__AllocateAt(s32 heap, s32 addr, s32 size)`. `base` is truncated, and so is `MemHandle {s32 addr}`. **Also wrong in low memory**: the node is a `CMemEntry` (host: `data` void* at 0, `size` 8, `lockCount` 0xC, `flags` 0x10), but `CCache__AllocateAt` writes it through junk_types' `B12Node` (`u32 key` 0, `unk4` 4, `flags` 0xC), so `flags \|= 3` lands in `lockCount` on the host | `CCache__AllocateAt(s32, sptr, s32)` through `CMemEntry`; `MemHandle.addr` and `gDisplayList0/1`, `gLineList0/1`, `gDepthBuffer`, `gAudioHeapBase` as `sptr` |
| 2 | `GfxPool_Alloc` :122 | `CCache__AllocateInternal` keeps `CCache__HeapAlloc`'s pointer in `s32 temp_v0` | `sptr` |
| 3 | `CList__Construct` from `MenuPool_Construct(s32)` from `CCamera__Construct` | `s32` argument; link offsets 0x1D0/0x1D4 are literal | `MenuPool *`, `T2_OFFSETOF(Menu, prev/next)` |
| 4 | `CList__Construct` from `CEmitterPool__Construct` from `CCamera__ConstructSkySystem(s32 arg0)` | `arg0 + 0x558` (N64 offset into CCamera), and CList offsets `0, 4` | **stopped here**: this is CCamera/CEngineApp layout (r1) and the CList offset sweep (r3) |

**r3's branch on the Mac.** `names-R3_r3` @ ce849002 merges cleanly with this branch (local
`r5-test-r3`, gendata rerun). It fixes rows 1-2 properly and stops at row 3:
`MenuPool_Construct(s32)` from `CCamera__Construct`, past GfxPool, `CEngineApp__Construct`
and `Sound__Initialize`.

After those fixes the Mac reaches `CCameraPool__Construct`, the same region where Linux stops. The
remaining work is the same on both, so nothing below this point is macOS-specific.

## 5. Not checked / open

- **Windowed mode was not run beyond one first test.** The user asked for headless only on the
  Mac. That one run (`turok2` without `--no-window`, the p3 test display list, Metal backend)
  crashed in LUS: `Fast::GfxRenderingAPIMetal::StartDrawToFramebuffer` (gfx_metal.cpp:918)
  calls `MTL::Buffer::contents` on a garbage buffer pointer (`objc_msgSend`, `EXC_BAD_ACCESS`),
  from `Interpreter::Run` via `DrawAndRunGraphicsCommands` (main.cpp:415). This is either a LUS
  2.0 Metal issue or our test DL's framebuffer setup. To try next: the OpenGL backend on macOS,
  or the `--no-window` path with an offscreen context.
- `--draw` was not tried: no game frame is produced yet.
- No `gamecontrollerdb.txt` next to the binary. LUS logs an error and carries on.
