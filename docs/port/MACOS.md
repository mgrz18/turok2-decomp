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
and `Sound__Initialize`. With r3 @ 15b57137 (MenuPool + every CList__Construct offset), it
stops at row 4: `CCamera__ConstructSkySystem(s32)` `arg0 + 0x558`, which is CCamera layout (r1).
The CList offsets there are already host values (0, 8).

After those fixes the Mac reaches `CCameraPool__Construct`, the same region where Linux stops. The
remaining work is the same on both, so nothing below this point is macOS-specific.

### r1 + r3 + r5 together on the Mac (local branch `r5-test-all`, not for merge)

This test merges `names-R3_r3` @ 15b57137 and `names-R3_r1` @ ef12cb66 (which includes r2's
loader) into this branch and runs with r2's format-2 archive (`mkarchive.py`: t2/swap, sha1
`8c1a4211…` with zlib 1.2.12). On Linux, r1's tree reaches mode 0 and draws frames. On the Mac:

- **Merge**: 13 `src/` files conflict between r1 and r3. Both sides make the same fixes
  (CList offsets, MenuPool, CCamera). Three semantic conflicts break the build until resolved:
  - r1's port body of `CCache__AllocateAt` keeps `s32 addr` and `(u32)CHeap__AllocAt`, while
    r3's protos use `sptr`;
  - `CCache__AllocateMemoryForDecompression(Top)`: r1 returns `s32`, r3 `sptr`;
  - `-X theirs` keeps r1's `MenuPool_Construct(s32)`.
- **Further 64-bit truncations on the boot path**, found one crash at a time on the Mac. Each
  row is fixed on the test branch and works on Linux only because of low memory:

| # | where | cause |
|---|---|---|
| a | `CCache__DeallocateInternal` (func_002061B8) from `CCache__LoadPersistantIndex` from `CScene__Construct` | the CMemEntry is typed as `CAI` (`m_dwFlags` = data as s32, `m_VisBits` = flags at N64 0xC), and the local decls of `CHeap__Free`/`FreeMemEntry`/`DetachCacheEntry` take `s32` |
| b | `CDecompressor__Main` calls `0x2F9CC0` | `CCache__LoadPersistantSubBlock(..., s32 arg5, ...)` truncates the callback (`CScene__DecompressIntelligence`); it is also declared `void`, but every caller reads its return value (v0 of `CCache__Request`) |
| c | `CScene__GetObject` from `CScene__DecompressParticles(s32 arg0, ...)` | a decompression callback whose `notifyId` (the scene) is `s32`; the same for `CScene__DecompressIntelligence` |
| d | `CLoader__LoadSync` memmove from `CScene__LoadBitSections` (func_0042C418) | local decl with `s32 dest` |
| e | `ProPackDecompress` from `Model_DecompressCallback(s32, s32)` (func_0020B490) | callback arguments are `s32` |
| f | `CISet__GetROMAddressAndSize` from `func_00207914(s32 arg0, ...)` from `Material_Unpack` | **stopped**: `Material.texture` is an `s32` in the decompressed model block, and the texture handle is written at `texture` and `texture + 4`. This is asset/model layout with in-place pointers, not a prototype fix |

r3 @ e9b1dfcc has a-e. Rechecked on the Mac (local `r5-test-all2`: r5 + r3 @ e9b1dfcc + r1 @ ef12cb66,
with only the three semantic merge fixes): it stops at f, the same place. With fixes a-e the Mac runs `CScene__Construct` through the index loads, intelligence,
particles and bit sections, into the first model decompression. Every decompression callback
(`decompressCallback(sptr notifyId, CCacheEntry *)`) still needs the host signature. The
particle `Piece` relocation (`base + piece[i].unkNN` in s32 slots) and `Material` are the next
asset-layout items.

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

## 6. Round 4 (part s3): macOS reaches mode 0, the legal screen and the first cinema

Branch `names-R4_s3` (from 474e40af). Every `src/`/`include/` change was checked with
`tools/asm_diff.py --base 474e40af -j 8` **on the box** (a scratch copy in `/root/R4_s3/repo`:
`OUT/asmcheck.sh` sends the diff over ssh, applies it and runs asm_diff over the whole tree when
`include/` changed). Every commit: **0 differences over 3,938 files**.

### Build and run on the Mac

As in section 1, plus: the ELF is the box's `build/turok2.us.elf` at 474e40af, and the
archive is the box's format-2.0 `C18out/turok2.o2r`. Build `t2runtime` and `t2audio` with the
game library before gendata (`OUT/rebuild.sh` does this). Run headless only:

```sh
SDL_VIDEODRIVER=offscreen OUT/build/app/turok2 --archive OUT/turok2_474.o2r --rom baserom.us.z64 \
    --game --no-window --draw --frames 300            # T2_DLDUMP_AT=<frame>: dump a frame's DL
```

**No headless rendering on macOS.** SDL's offscreen driver has no Metal and no GL: LUS logs
"Error creating SDL window" and crashes in `GfxRenderingAPIMetal::Init`
(`newSamplerState` on a null device) or `glewContextInit`, because offscreen needs EGL and
macOS has none. A hidden Cocoa window would work, but the user asked for no windows. So `--draw`
on the Mac runs the game's whole draw path (`CEngineApp__Draw` and every display list), but
Fast3D does not execute it. Frames are compared as **display lists** instead of pixels:
`T2_DLDUMP_AT=<frame>` (game.cpp, new) prints the first display list at or after that frame,
before the game runs again. It also works without a window. Linux frames are compared the
same way.

### Crash and fix table (s3)

| # | where | cause | fix |
|---|---|---|---|
| s3-1 | `CISet__GetROMAddressAndSize` from `func_00207914` from `Material_Unpack` | `func_00207914(s32 arg0)` took `&Material.texture` (the texture's ROM address, then its size at +4) as an int | `sptr` arg; `CScene__GetTextureSet(..., s32 *, sptr)`. Both words are ROM values, not host pointers: no host-typed Material is needed |
| s3-2 | `CScene__GetLevelName` | levels table read as `s32` at N64 0x38 of CScene; index out-slot `s32 sp20`; sub-block handle at N64 offsets | typed on `CScene` (`m_pLevels`, `m_LevelsKey`, `m_nLevel`), `CMemEntry` (`data`, `size`), `sptr` slot |
| s3-3 | `guMtxIdent` from `MatrixHeap_Construct` | the matrix pool `{s32 items; ...}` in `CScene.m_Heap` (u8[0x10]) | `MtxPool {MtxBlock *base; s32 n; s32 remaining; MtxBlock *next}` (structs/world.h) as `CScene.m_Heap[1]`; `MatrixHeap_Construct`, `MtxPool_Reset`, `MtxPool_Alloc` typed |
| s3-4 | `CScene__FindNearestWarpDest`, `CScene__Construct(_2)`, `func_0042C194` | `CCache__LoadPersistantIndex(s32, s32 *out, ...)`: the out-slot gets a pointer | `sptr *` slots |
| s3-5 | `raw_normal` from `GameObject_GetGroundRotation` | `CScene__GetRegion`'s port body returned `(s32)(sptr)`; `FindRegionAt`/`GetRegion` declared `s32` | `sptr` returns (scene.h, abu_protos.h, the local decls) |
| s3-6 | `CGameObjectInstance__SetupAnimation` (port body) | object table `s32 a1`; request handles read as `s32 *` (`tree = *model`) | `s32 *` table, `CMemEntry *` handles (`->data`) |
| s3-7 | `memset` from `Pak_AllocDirectory` | `Shape_typemap_165.field_0` (int view of the block's data pointer) | `->data` |
| s3-8 | `SndSeq_FadeOut` from `CAudioWorld__FadeOutMusic` | `s32 arg0 + 0x1D64` | `&world->seq` (CAudioWorld typed) |
| s3-9 | `CCameraPool__NewGame` from `EngineMode00_PakCheck_Init` | all 28 `EngineModeNN_*` handlers take the app as `s32` | `sptr` (plus `func_002888BC`, `CEngineApp__StartWarp`, `CEngineApp__DrawWorld` and the callee decls); `EngineMode00_PakCheck_Draw` / `EngineMode08_MainMenu_Draw` pass the app on to `EngineMode13_Game_Draw` (the N64 left it in a0) |
| s3-10 | `Automap_IsShown` | `s32 temp_s0 = app + APP_AUTOMAP` | `sptr` |
| s3-11 | `Menu_Construct` from `MenuPool_OpenMenu` | `s32 *r; s32 s = *r` (block handle) and `s32` player/controller/context in all four `MenuPool_Open*` | `sptr`; prototypes in protos/menu.h return `sptr` |
| s3-12 | `Menu_TextWidth` from `MenuItem_Layout` | text read through `(u8 *)it->data` (s32 union view) | `it->text` |
| s3-13 | **(Linux too)** damaged-pak menu lines draw nothing | `MenuItem_DrawText(Menu *arg0, ...)` is the item's draw callback; it passed `arg0->desc`. N64: Menu.desc = MenuItem.text = 0x14. Host: Menu.desc is at 0x20 = `MenuItem.def`, the item's desc pointer. This is the "text points into the desc table" of STATUS §4.1 | typed `(MenuItem *, MenuItemLayout *, MenuItemLayout *, Menu *)`, passes `arg0->text`; the same in `MenuItem_DrawText_2`; DrawChar/DrawBinding without `s32` pointers |
| s3-14 | `CCameraViewParams__Construct(0x31)` from `CCameraPool__AllocCamera` (mode 4) | `CElementPool { pad[0xC]; CList free; CList active }`: N64 layout of CCameraPool | typed on `CCameraPool` (both copies) |
| s3-15 | `Controller_SetRumbleOption` (truncated profile) | `CPlayerContState__Init(Record14 *, s32)`: 14-word view | host body on `CPlayerContState` (`#ifdef TARGET_PORT`); `CPlayer__ResetControlState` passes `&player->m_ContState` instead of `+ 0xA40` |
| s3-16 | `MenuPool_CountOpen` from `Automap_Update` | `(u8 *)cam + 0x53C` (N64 offset of `CCamera.m_MenuPool`) | `cam->m_MenuPool`: 29 sites in 19 files. The 4 sites that also read the player at N64 offsets (`+ 0x51C`, `P_S32(player, 0x51C)`) are left to the offset sweep |
| s3-17 | `func_00267DBC` from `CScene__DecompressAnimatedInstances` | intelligence read at N64 0x14 as a word view | host body on `CInstanceHeader`/`CIntelligence` |
| s3-18 | `func_0021E5D4` from `CScene__LoadLevel(162)` | `LevelActor` view (0x14, 0xD4, 0x207), player stride 0x1328, weapon actor +0x288, AI +0x140; the caller walked the actors by 0x288 | host body on `CGameObjectInstance`/`CPlayer`; `CScene__LoadLevel(_2)` steps by `sizeof(CGameObjectInstance)` and `T2_OFFSETOF(CPlayer, m_WeaponActor/m_pNext)` |
| s3-19 | **(all hosts)** colour image truncated in every display list (`FF` w1 = low 32 bits); the scheduler's `frameOf` never matched, so it always used frame 0 | `CFrameData.m_pFrameBuffer` was `u32` | `uptr` (structs/frame.h); `frameOf` compares `uptr`; `(uptr) gDepthBuffer` in `func_0027D160` |

### How far it runs

`turok2 --game --draw --frames 300` (headless, `--no-window`): boot, level 0, **mode 0 for
300 frames**: 1,414 thread switches and 151 gfx tasks, the same counts as Linux; no crash. The
pak timer opens the pak menu at frame ~120, the same as Linux.

With a local probe that ends the pak check when the menu opens (not committed: s1/s2 own the
way out of mode 0), the Mac goes through **mode 0 → 1 (legal) → 3 (expansion pak) → 4 (first
intro cinema)**. Mode 4 warps to level 162 and constructs its actors. It then stops in the
AI:

```
CScene__SetInstanceSavedBit(arg1=0xffffffffcf4c5e70)  <- Action_IdleEnter(s32 arg0)
  <- CAI__SetMode (Source110/Dest views at N64 offsets) <- Action_Advance <- func_0021E5D4
  <- CScene__LoadLevel(162) <- CScene__UpdateWarp <- EngineMode04_Cinema_Draw
```

The AI mode/action callbacks take the actor as `s32` (`Action_*`, `GameObject_FireTriggers`,
`CScene__SetInstanceSavedBit`). `CAI__SetMode` uses N64-offset views. This is the next sweep
item, and Linux low-memory hides it.

### Display lists, Mac vs Linux

The same tree was built on the box (`/root/R4_s3/out`, low memory) and run with the same
arguments. `T2_DLDUMP_AT=101` and `=301`, headless `--no-window`, on both:

| frame | Mac | Linux | op + w0 | w1 of non-address commands |
|---|---|---|---|---|
| 101 | 83 commands, 0 texture rectangles | 83, 0 | identical | identical |
| 301 | 2,066 commands, 109 texture rectangles | 2,066, 109 | identical | identical |

The display lists differ only in host addresses (G_DL, G_MTX, G_MOVEMEM, colour/depth/texture
images). Since s3-19 the colour image carries the full 64-bit address (`FF ... 00000075C1B80000`).

**Without a window LUS reports no controller**, so `Pak_GetStatus(0)` is -2 on both hosts,
`gPakCheckHasPak` is 0, and mode 0 draws the scene and then the pak menu, not the "ACCESSING THE
CONTROLLER PAK" text. The Linux windowed run (keyboard controller on port 0) draws the text, and
since s3-13 also the menu lines:
the round-4 screenshot `R4_s3_linux_s3branch_f300.png` (not in the repo) shows "PROBLEM!", "DAMAGED
CONTROLLER PAK FOUND IN CONTROLLER #1.", "ATTEMPT REPAIR" (highlighted), "RETRY", "CONTINUE
WITHOUT SAVING". On the Mac that path needs a controller without a window: s1's PIF/controller
work, or a port flag that connects a virtual pad headless.

## 7. Round 5 (part t5): the merged tree to the title screen

Branch `names-R5_t5` (from 98574f3c). Every `src/`/`include/` commit was checked with
`tools/asm_diff.py --base 98574f3c -j 8` on the box (a scratch copy in `/root/R5_t5mac/repo`;
the local `OUT/r5/asmcheck.sh` sends `git diff 98574f3c -- src include` over ssh, applies it and
runs asm_diff over the whole tree): **0 differences over 3,938 files** at every commit.

### Build and run on the Mac

As in section 1, with the box's ELF at 98574f3c (`scp decomp:/root/turok2_decomp/build/turok2.us.elf
build/`) and a format-2.0 archive (`OUT/turok2_474.o2r` from round 4 still checks). `OUT/rebuild.sh
--fresh` builds the three libraries, runs gendata (same report as Linux: 1,543 symbols, 3,881
pointer slots) and links. Run headless only:

```sh
SDL_VIDEODRIVER=offscreen OUT/build/app/turok2 --archive OUT/turok2_474.o2r --rom baserom.us.z64 \
    --game --no-window --draw --pak-blank --frames 6000      # T2_DLDUMP_AT=<frame>: one frame's DL
```

`--no-window` now connects one virtual pad on port 1 (its pak per `--pak-*`, its buttons from
`--input`): LUS's ControlDeck has no device without a window, so before this the game saw no
controller and no pak and never left mode 0 (PIF 0 SI DMAs). With it: 812 SI DMAs, 236 pak
reads, 169 pak writes, the same as Linux windowed.

### How far it runs

```
game: frame 2:    engine mode 17 -> 0      pak check (the virtual pak passes)
game: frame 298:  engine mode 17 -> 1      legal screen
game: frame 1070: engine mode 17 -> 3      expansion pak
game: frame 1362: engine mode 17 -> 4      intro cinema (level 162)
game: frame 1852: engine mode 17 -> 5      intro cinema (level 44)
game: frame 4214: engine mode 17 -> 7      TITLE SCREEN
game: frame 5469: engine mode 17 -> 13     attract demo (level 81), runs to its end
game: frame 8802: engine mode 17 -> 1      the boot loops: legal, cinemas, title (12429)
SIGSEGV (second attract demo): CGameRegion__ContainsXZ(tri = garbage) <- CScene__FindRegionAt
    <- CScene__DecompressAnimatedInstances <- CDecompressor__Main            (Linux: the same)
```

Display lists, Mac vs Linux (the same branch built on the box, both `--no-window --draw
--pak-blank`, `T2_DLDUMP_AT`):

| frame | mode | commands Mac / Linux | op + w0 | w1 of non-address commands |
|---|---|---|---|---|
| 601 | 1 legal | 4,000 / 4,000 (dump budget; 351 texture rectangles) | identical | identical |
| 1201 | 3 expansion pak | 680 / 680 (36 rectangles, 5 G_VTX) | identical | identical |
| 1501, 2401 | 4, 5 cinemas | 246 / 246 (no G_VTX: the black cinemas, t3) | identical | identical |
| 4500 | 7 title | 1,250 / 1,250 (63 rectangles) | identical | identical |
| 6000 | 13 attract demo | 354 / 354 | identical | one `D9` (G_GEOMETRYMODE) whose w1 is a host address on both: a pointer passed as mode bits |

The Linux title at frame 4500 from this branch was checked by eye (the TUROK logo, the eye in
the O, "SEEDS OF EVIL", the red "2"); the Mac draws nothing (no headless Metal/GL) but its
display list is the same.

### Fix table (t5)

| # | where | cause | fix |
|---|---|---|---|
| t5-1 | mode 0 forever (no SI DMA) | no controller without a window | virtual pad for `--no-window` (game.cpp) |
| t5-2 | `Pak_DecodeText(src = truncated)` from `PakMenu_InitSaveGameAnyPak` | `Pak_GetNoteGameName(.., s32 *out)`: the note name pointer in an s32 | sptr out-slots in 5 files (both pak menus, `_2` copies) |
| t5-3 | `CISet__GetBlock(truncated)` from `CGameObjectInstance__PickRandomAnim`, `Actor_ReloadAnimInfo` | cache handles as `s32 *`, `*res` = the data pointer's low half | `CMemEntry *` / `sptr *` (14 files) |
| t5-4 | `CScene__SetInstanceSavedBit` from `Action_IdleEnter(s32 actor)` | AI callbacks take the actor/AI as s32 | sptr params (151 in 120 files) |
| t5-5 | `CScene__IsEventFired`, `findSorted` (event tables) | event-record pointers in s32 locals; `BinaryRange(s32 table)` | sptr |
| t5-6 | `CMtxF__TransformBox(box = truncated)` | `UpdateBounds(.., s32)`, `TransformBox(s32, s32, s32)` | sptr |
| t5-7 | `CPlayer__GetPickupCountLow(s32 player)` | player as s32 and N64 offset `-0xA74` | host bodies on `m_Data.m_SpecialPickupCount` |
| t5-8 | `AI_PlayAnim` and the rest of the class | prototypes s32 where the definition has a pointer, and definitions s32 where callers pass pointers | `tools/port/ptrdecls.py` (385 params, 27 returns), `tools/port/ptrargs.py` (331 params of 127 functions) |
| t5-9 | `--audio`: `alSaveNew(truncated)`, `alFxNew`, `alMainBusParam`, `alHeapDBAlloc`, `alFxPull` | `alHeapInit`'s `(s32)` casts, `alHeapDBAlloc` declared s32, bus/resampler constructors with s32 pointer params, `alFxParam(s32 *)` | sptr |

### Open on the Mac

- `--audio`: the mixer now runs, then `T2Asp_Run` -> `cmd_savebuff` memmoves to a truncated
  address: the Acmd words carry 32-bit DRAM addresses (`osVirtualToPhysical`), fine below 4 GB
  on Linux, not on the Mac. That is t4's item (1) (a handle table or full-width host Acmd).
- The second attract demo stops in the level load on both hosts (above).
- 1,218 port-build warnings remain (`-Wint-conversion` 895 mostly benign int-to-pointer of
  sptr values, `-Wshorten-64-to-32` 229 `sptr` -> `s32` assignments): the remaining s32
  locals/fields that hold pointers. Rerun `ptrdecls.py`/`ptrargs.py` after merges.

## 8. Round 6 (part u4): the merged tree on the Mac, `--audio`, pixels without a window

Branch `names-R6_u4` (from 6d04fabb). Every `src/` commit was checked with
`tools/asm_diff.py --base 6d04fabb -j 8` on the box (scratch copy `/root/R6_u4mac/repo`;
`OUT/r6/asmcheck.sh` sends `git diff 6d04fabb -- src include` over ssh, applies it and runs
asm_diff on the changed files): **0 differences** at every commit. No `include/` changes.

### Build and run on the Mac

As in section 7, with the box's ELF at 6d04fabb and its round-5 archive
(`scp decomp:/root/turok2_decomp/OUT/turok2.o2r OUT/turok2_6d0.o2r`). `OUT/rebuild.sh --fresh`
(about 30 s). Every run is headless, and none creates a window:

```sh
cd OUT
# the boot, the cinemas, the title, the three attract demos, the loop (no pixels)
SDL_VIDEODRIVER=offscreen build/app/turok2 --archive turok2_6d0.o2r --rom ../baserom.us.z64 \
    --game --no-window --draw --pak-blank --frames 26000
# with PIXELS: Fast3D on a CGL context with no drawable (section 8.3); T2_SHOT works
SDL_VIDEODRIVER=offscreen T2_SHOT=shot.ppm T2_SHOT_EVERY=500 build/app/turok2 --archive turok2_6d0.o2r \
    --rom ../baserom.us.z64 --game --offscreen-gl --draw --pak-blank --frames 15000 \
    --input START@4400,A@4500,A@4700,A@4800,CU@13000,CU@13004,...        # a new game
# sound
... --game --no-window --audio --wav out.wav --frames 6000
```

### 8.1 How far it runs (the same as Linux, frame for frame)

`--no-window` and `--offscreen-gl` both: no crash to frame 26,000 (the boot, both intro cinemas,
the title, **all three attract demos**, the loop back to the title at 25,786), with every mode
switch at the same frame as Linux (2, 298, 1070, 1362, 1852, 4214, 5469, 8802, 9577, 10067, 12429,
13684, 16073, 16845, 17335, 19697, 20952, 22160, 22934, 23424, 25786). The new game reaches level 51
at frame 12,903 and the player walks into the water: `T2_STATE_LOG` gives the same positions and
states as Linux ((-1111.0, -779.6, -603.6) state 9 at 13,200; stuck at (-1266.6, -779.5, 260.0) from
13,500). `T2_STATE_LOG=30` through demo 2 (frames 5,400-15,100, 324 records) is identical on both
hosts apart from the host addresses.

`--audio --wav` (6,000 frames: boot, cinemas, title, into demo 1): 100.10 s, **peak 21,304,
RMS 1,996.4**, the numbers the Linux run gives (STATUS.md section 2). The new game with `--audio`
runs to frame 15,000 (250 s of sound, peak 21,304, RMS 2,221).

### 8.2 Fix table (u4)

| # | where | cause | fix |
|---|---|---|---|
| u4-1 | `CISet__GetBlockAndSize` from `Lighting_BuildLitVertices` from `CStaticObject__BuildLitVerticesCallback` (mode 4) | the callback read the decompressed block as `*(*arg1)` (s32) and both lit-vertex callbacks declared `Lighting_BuildLitVertices(s32, s32, s32, ...)`: the model block, the regions and the box truncated; `CGameObjectInstance__ConstructLitVertices` also read the box and the draw matrices at N64 0xBC/0x98 | host bodies: `((CCacheEntry *) arg1)->memEntry->data`, sptr prototype, `&o->m_vMin`, `o->m_pmtDrawMtxs`; the definition takes `s32 *model` |
| u4-2 | `CScene__GetRegionInfo` from `GameObject_GetRegionInfo` from `CPlayer__Update` (demo 1) | the actor's region read through a `Link {pad[0x10]; s32}` view: the low half of the header's `m_pCurrentRegion` | host body on `CInstanceHeader` |
| u4-3 | `CBarrel__CompareIcons` from `Sort_Insertion` (demo 1, the weapon wheel) | `Sort_Insertion(u32 lo, u32 hi, ...)`: element addresses truncated | host body with 64-bit addresses |
| u4-4 | `--audio`: `cmd_polef` | `_filterBuffer` read `ALLowPass` at N64 offsets (`fstate` at 0x28 as s32, `first` at 0x2F) | host body on `ALLowPass` (`fstate` 0x28 pointer, `first` 0x30) |
| u4-5 | `--audio`: `cmd_savebuff` to a truncated address | the port's `am_frame` passed the host output buffer to `alAudioFrame`, whose `alSavePull` puts `dramout` in A_SAVEBUFF's 32-bit w1. The N64 `__amHandleFrameMsg` passes `osVirtualToPhysical(buffer)` | `am_frame` passes `osVirtualToPhysical(out)` (t4's handle table resolves it) |
| u4-6 | `--audio`: a call to 0x2D55D4 from `_decodeChunk` | `alLoadNew` stored the `ALDMANew` result through `typedef s32 (*FuncPtr)(void *)`: the DMA routine's address truncated | `void *(*)(void *)` |
| u4-7 | `--offscreen-gl`: Fast3D reads 0x6987E4 (G_SETTIMG) | the font texture image in an `s32` (`COnScreen__SelectNormalFont`), `(u32) v0` (`COnScreen__SelectSmallFont`), `Hud_DrawGlyph16x8/16x16(s32 texture)`, and three `(unsigned int)(GBI_ADDR(p))` casts (func_00279158, func_0027A2C4, func_0027A4DC) | `sptr` / `GBI_ADDR` (identical on the N64) |
| u4-8 | **(all hosts)** the cinema fades and flashes drew a garbage rectangle (Linux: zero height, so nothing; macOS: a black or orange block) | `Cinema_DrawFade(_2)` read the camera viewport through a `ScreenRectangle {pad[0x288]; ...}` view: N64 offset of `CCamera.m_Viewport` | the view's pad is `offsetof(CCamera, m_Viewport)` under TARGET_PORT. The new-game cinemas now fade from and to black and show the orange flash of cinema `L1handcam` (its fade keys: colour (0.9, 0.45, 0), alpha 1) |

`T2_V2P_TRAP=1` (macOS, t2ultra.c): `osVirtualToPhysical` traps on an address that cannot be a
host pointer there (below 4 GB, or sign-extended from 32 bits), so a truncated pointer on its way
into an RSP command stops at its source. It found u4-5 by elimination (no trap: the address never
went through `osVirtualToPhysical`).

`-ffp-contract=off` for the game (game.cmake): arm64 clang fuses `a * b + c` by default and the
R4300 has no fused multiply-add. It changed no observed result (the demo-2 difference in 8.4 is
not float drift), but keeps the float results the same as the x86-64 build.

### 8.3 Pixel screenshots without a window: `--offscreen-gl`

SDL's offscreen driver has no GL or Metal on macOS, and a hidden Cocoa window is still a window.
**Core OpenGL (CGL) can create a context with no drawable at all**: `CGLChoosePixelFormat`
(3.2 core profile, accelerated) + `CGLCreateContext` + `CGLSetCurrentContext`. That makes no
window, no NSApplication and no window-server surface; it gets "Apple M5, 4.1 Metal - 91.7".
Every draw then has to go to framebuffer objects, which Fast3D does on macOS anyway:
`Interpreter::ViewportMatchesRendererResolution()` is false there, so the game frame is rendered
into the interpreter's game framebuffer (`mGameFb`), and only the GUI copies it to framebuffer 0.

`port/app/offscreen_gl.cpp` builds that interpreter by hand, as `Fast3dWindow::OnInit` does, with
LUS's own `GfxRenderingAPIOGL` on the CGL context and a `GfxWindowBackend` that has no window
(a fixed 640x480, no events, `glFlush` as the swap). Per frame it runs `StartFrame`, `Run` and
`EndFrame`, without the GUI. `T2_SHOT` reads `mGameFb` back (top-down, so no row flip). Nothing in
LUS is patched. The few clears Fast3D sends to framebuffer 0 (no storage without a drawable) are
GL errors that change nothing. The GfxDebugger and the interpreter are kept alive to the exit,
because a Ship::Component's destructor terminates outside a component tree. On Linux the flag
prints a message and does nothing. There, `SDL_VIDEODRIVER=offscreen` with a window already gives
pixels through EGL.

Metal was not used. LUS's Metal backend takes its device and drawable from an SDL renderer's
`CAMetalLayer` (`SDL_GetRenderMetalLayer`), and that needs a window. A Metal device and an
offscreen texture would need a new rendering API: about 2,000 lines of gfx_metal.cpp without the
layer.

**dlcheck.** Before each `--offscreen-gl` frame (and with `T2_DLCHECK=1` in a window), game.cpp
walks the display list. A G_VTX/G_MTX/G_MOVEMEM/G_DL/G_SETTIMG/G_SETZIMG/G_SETCIMG/segment word
must be segmented (bit 0, below 0x10000000), null, or at least 4 GB. Anything else is a truncated
host pointer: it is reported once per site (`dlcheck: frame N: FD ... before: ...`) and the frame
is not drawn, instead of Fast3D crashing on it. After u4-7, **no display list fails, in all
26,000 boot frames and in the new game to frame 15,000.**

### 8.4 Mac vs Linux, pixels and display lists

The same branch was built on the box (`/root/R6_u4mac/out`, low memory, SDL offscreen window).
Both hosts ran the new game (screenshots every 500 frames, 29 frames) and the boot (every 1,500
frames, 17 frames):

- **New game: all 29 screenshots match.** At most 0.37% of the pixels differ by more than 24/255
  (GPU rounding: Apple's GL-on-Metal against Mesa). This covers the pak check, legal, the
  expansion pak, both intro cinemas, the title, the menus, the new-game cinemas (Port of Adia,
  the boat, the portal), and level 51 (sky and fog only, as on Linux, u1).
- Display lists at frames 4500, 9502, 13501: same command count, identical `w0`, identical `w1`
  apart from host addresses (one `D9` whose w1 is a host address on both hosts).
- **Boot: 15 of 17 frames match.** Frame 22,501 (the legal screen fading in) is darker on the Mac,
  which is fade timing. Frame 15,001 (attract demo 2, the weapon wheel) **differs in content**:
  the Mac draws level geometry behind the wheel (49 batched models: 63 G_VTX, 54 G_MTX in the
  display list), and Linux draws none (`sOpaqueSortFirst` is NULL at `Render_DrawBatches`, 4
  G_VTX). The player state is identical on both (above), so the difference is in the
  model-queueing path (`Render_QueueModelInstance` / `Render_QueueModelByKey` /
  `func_0020B9F4` -> `Render_FlushModelBatch`). Something there depends on the host. Linux
  low memory is the suspect, because the Mac's full-width pointers draw. This is the same symptom
  as u1's "level 51 draws no geometry", so it is a lead for u1. Found with gdb on Linux at
  `CScene__DrawGeometry` (func_002222E0.c:166): `vmask = v->m_ViewParams.m_VisBits` is **0**, so
  `vmask & a->m_ViewMask` (0x4000008) rejects every static object. The intelligence flags,
  the view box and the frustum all pass. `m_VisBits` comes from `CCamera__UpdateVisBits`
  (func_00281E4C.c): it is either `gDefaultVisBits`, or the camera region's `m_VisBits` when the
  camera is 5 to 11 units above that region's floor. So either the region record or
  `m_pCurrentRegion` is wrong on Linux. This was sent to u1.

Screenshots (not in the repo): `assets_preview/R6_u4_mac_boot_{3001,4500,7501}.png` (Iguana
cinema, title, demo 1), `R6_u4_mac_ng_{9502,13501}.png` (Port of Adia cinema, level 51),
`R6_u4_sbs*_*.png` (Mac | Linux side by side).

### 8.5 Open on the Mac

- The demo-2 difference above (draws on the Mac, not on Linux).
- The level-51 gameplay frame: sky and fog only, the same as Linux (u1's item).
- `--offscreen-gl` uses a fixed 640x480. The game frame size follows `gScreenWidth/Height`
  through `SetNativeDimensions`, as in a window.

## 9. Round 7 (part v4): the round-6 merged tree on the Mac, arm64 float conversions, performance

Branch `names-R7_v4` (from d3f05a74). Every `src/` commit was checked on the box with
`tools/asm_diff.py --base d3f05a74 -j 8` (scratch copy `/root/R7_v4mac/repo`;
`OUT/r7/asmcheck.sh` applies `git diff d3f05a74 -- src include` there): **0 differences** at
every commit. No `include/` changes. The Linux runs for comparison are this branch built in
`/root/R7_v4mac/out` (Debug, low memory, SDL offscreen window).

Build: `OUT/rebuild.sh --fresh` with the box's ELF at d3f05a74 (sha1 `61dcdf3f…`, the same as
round 6) and the box's round-6 archive (`OUT/turok2_d3f.o2r`, sha1 `47e8f73c…`).

### 9.1 What the merged tree needed on the Mac

| # | where | cause | fix |
|---|---|---|---|
| v4-1 | jump to 0x350598 from `CAI__Update` when the boot enters attract demo 1 (mode 13, frame 5467), and when the new game enters level 51 | `WeaponActor_Update` (func_00257D48) reads the CAI through an AI view at N64 offsets: `unk128 = 0` (N64 0x128, the weapon vars) lands on the high half of `m_pfStep` (host 0xF0 + 8). The step pointer truncates. Linux survives because its text is below 4 GB. The same view read `ai->state` at 0x30 = the host `m_pModeTable` (not `m_pModeCurrent`), so the weapon actor always ran the mode table's row-0 callback, **on Linux too** | host body on CAI/CGameObjectInstance: owner `m_AI.m_pParent`, `m_Weapon.m_RefireDelay/m_nLod/m_FireDelay`, `m_pModeCurrent->m_pfCallback`, `m_pModeTable->unk_10`. **The weapon switch now completes**: in the level-51 journey the wheel's weapon 2 becomes current (`weapon 2 reqw 2`), the HUD shows the bow with 20 arrows, and Z goes to the fire mode (`wact 4/27`) and back. All hosts |
| v4-2 | `EXC_BREAKPOINT` in `func_00225C08` from `GameObject_PlayLandSound` (Release build only, Turok's first landing in level 51) | the FAKEMATCH text tests an uninitialised local (`if (temp_s2)`). clang -O2 turns that into a trap. It also reads the kind-1 actor list at N64 offsets (0xDFC/0xEFC) with s32 actor pointers | host body: `AI_HearNoise` for each of `m_pKind1[0..m_nKind1)` |
| v4-3 | `CScene__ClearListCounts` | N64-offset view: on the host it cleared words inside the pointer arrays, and the counts were never reset | names-R7_v1's host view (88ec1de3), taken as is so that the branches merge cleanly |
| v4-4 | **arm64 only**: actors drawn with negative matrix elements zeroed. The Acclaim logo's letters (frame 1501: 7.4% of the pixels differed from Linux, after the fix 0.00%), Turok's hand, the boat in level 51 | C leaves a negative float -> unsigned conversion undefined. The R4300 converts through a signed word (`trunc.w.s`), and x86-64 clang does too in practice (through a 64-bit signed integer). arm64's `fcvtzu` **saturates to 0**. `packMtx` (func_0021B95C, guMtxF2L for the bone matrices) does `(u32) m[i][j]`, so every negative element became 0 on the Mac | `T2_F2U(x)` = `(u32)(s32)(x)` under TARGET_PORT (`(u32)(x)` on the N64, the same text) in packMtx, the vertex-pattern hash (func_0021285C, `worldPosition`), the scene light direction (func_0027C4E4) and the fill-rectangle corners (func_0027F64C) |

**`tools/port/f2ucheck.py --build OUT/build -j 4`** lists every float -> unsigned conversion in
the game, explicit or implicit (`clang -Xclang -ast-dump`, FloatingToIntegral to an unsigned
type). There are 99 in 36 files. The ones not changed are clamped colours, alphas and timers
that cannot be negative. A **runtime check** confirms the static triage:
`-fsanitize=float-cast-overflow` (CMAKE_C_FLAGS) reports every out-of-range conversion.
Through the whole level-51 journey (15,000 VIs) it reported only the four sites above (packMtx
and func_0021285C), and after the fix it reports nothing. Rerun both after merges: new host bodies
can bring new `(u32)` casts.

### 9.2 How far it runs on the Mac (this branch alone)

- **The new game with the scripted journey** (STATUS.md section 1), `--offscreen-gl`, 15,000
  frames: no crash. `T2_STATE_LOG` is **identical to Linux** (250 records), and now the weapon
  switch completes on both (v4-1).
- **The boot**: the boot, the cinemas and the title as before. Attract demo 1 then stops at frame
  ~5470, on both hosts. With v4-1, its weapon actor really fires (`WeaponMode_StreamFire_Update` ->
  `CPlayer__FireWeapon(p, 20)`), which reaches `CParticleSystem__Spawn` and its N64-offset views
  (`CISet__GetBlock(NULL, 1)`). That is names-R7_v1's work. With v1 merged (section 9.4), the
  boot gets into the demo, as far as v4-8.
- Debug and Release (-O2) give the same `T2_STATE_LOG` for the journey.

### 9.3 Pixels, Mac against Linux

The new game's 30 screenshots (every 500 frames): **26 agree to within 1% of the pixels**
(>24/255). Before v4-4, frame 1501 (the Acclaim logo) differed by 7.4%; now it agrees exactly. The
other four are all actor parts that Linux draws and the Mac does not: frame 7000 (a new-game
cinema: Adon matches, but Turok's hand at the right edge is missing on the Mac, 3.2%), 10502 and
11002 (cinemas, 1.7%), and 13001 (level 51, 6.9%). At 13001 **Linux draws the moored boat and
Turok's hand; the Mac draws only the boat's rigging lines**. What is known:

- the display lists are identical (`T2_DLDUMP_AT=13001 T2_DLDUMP_VTX=2`): every command word
  apart from host addresses, **every vertex of every G_VTX, all 181 matrices**, and the first 32
  bytes of every G_SETTIMG image (115) are the same on both hosts;
- Linux draws the boat in low memory and in `--high-memory` alike, so pointer width is not the
  cause;
- disabling LUS's `GL_DEPTH_CLAMP` (a local experiment, not committed) changes nothing.

So the difference lies after the display list: in Fast3D on the Mac's framebuffer-object path
(`mGameFb`, used without a window), or in data the dump does not cover (G_MOVEMEM lights and
viewports: `T2_DLDUMP_VTX=2` now prints their first 16 bytes, but that was added after the
comparison). It is open (section 9.6).

### 9.4 With names-R7_v1 (scratch merge, not committed)

For the test, names-R7_v1 @ c6ad2fa9 (the particle path, animation events on by default) was
merged into this branch in a scratch worktree (`OUT/r7/scratch`, not committed) with v1's archive
(sha1 `3ad7d240…`). On the Mac, with full 64-bit pointers, three truncations show up that Linux's low
memory hides:

| # | where | cause | status |
|---|---|---|---|
| v4-5 | `Trail_AddNode` <- `CParticleSystem__Spawn` (boot frame ~1362, the first trail) | `CScene__GetSwoosh`'s TARGET_PORT body (func_00223F50) calls `CISet__GetBlock` with no declaration: the implicit `int` return truncates the record. It was the only implicit declaration in the game build | **fixed on this branch** (a declaration line; asm_diff 0). v1 fixed it too (5e724e40, by including protos/iset.h): a trivial conflict, keep either |
| v4-6 | `ModelSweep_TestModel` <- `Collision_MoveInstance` <- `CParticle__Update` | `root = (s32 *)*data;` (func_0023DBBC.c:33) reads the block pointer as s32 | fixed by v1 (names-R7_v1 b8bca0e3, `*(s32 **)data` under TARGET_PORT) |
| v4-7 | `ImpactEvent_AreaDamage` <- `ImpactEvent_Send(kind 3)` <- `FireAnimEvents` (new-game cinema, mode 12) | `ImpactEvent_AreaDamage(s32 arg0, s32 arg1, ...)`: the actor pointers as s32 | fixed by v3 (names-R7_v3 e26d87e9: the 8 area/explosion/push handlers take sptr). `ImpactEvent_LevelSpecial` (func_00238DF8): fixed by v5 (names-R7_v5 06477da5, sptr plus the boss handler prototypes) |
| v4-8 | `RumbleCurve_Advance` <- `RumblePool_FreeFinished` <- `Controller_Service` (attract demo 1 fires; `--offscreen-gl` only) | reads `gCurrentFB[1]`, which is NULL there | open; v2 has rumble fixes (names-R7_v2 be9c4bf8) to retest with |

With v4-5 and v4-6, the boot runs into attract demo 1, which fires its weapon and spawns particles,
and the new game runs through the mode-11 cinemas into mode 12. Linux `--high-memory` should show
v4-6 and v4-7 as well.

### 9.5 Performance

[PERF.md](PERF.md): `--bench` and `--pace`. Summary: in level 51 a drawn frame costs 0.55 ms
(Release) or 1.05 ms (Debug) of the N64's 33.3 ms. Real-time pacing at 60 Hz VIs / 30 fps runs
with no late VI in level 51 (Release), and late VIs only at level loads.

### 9.6 Open on the Mac

- The actor pixels of section 9.3 (the boat, Turok's hand): identical display lists, different
  pixels. Next: compare the G_MOVEMEM data, then check the FBO path by drawing that frame's
  display list on Linux with an offscreen FBO (or on the Mac at window size).
- Attract demo 1 needs names-R7_v1 (the particle path).

## 10. Round 8 (part w4): the round-7 merged tree on the Mac, host-dependent bugs

Branch `names-R8_w4` (from 5bf03fd0). Every `src/` commit was checked on the box with
`tools/asm_diff.py --base 5bf03fd0 -j 8` (scratch copy `/root/R8_w4mac/repo`; `OUT/r8/asmcheck.sh`
applies `git diff 5bf03fd0 -- src include` there): **0 differences** at every commit. No `include/`
changes. For comparison the same branch was built on the box in `/root/R8_w4mac/out` (Debug, low
memory, `/root/R8_w4mac/build.sh`; set `FRESH=1` to drop decls.json).

Build: `OUT/rebuild.sh --fresh` with the box's ELF at 5bf03fd0 (sha1 `61dcdf3f…`, unchanged) and
the box's round-7 archive (`OUT/turok2_5bf.o2r`, sha1 `71317b96…`). The runs: `OUT/r8/battery.sh`
(boot 26,000; journey with firing; warp to 52 and back; Mother, Mantid Queen, Primagen; demo-1
enemies; `--audio`), all `--offscreen-gl` except the audio one; `OUT/r8/cmpall.sh` fetches the box
runs and compares the logs (host addresses masked) and the screenshots.

### 10.1 Fixes

| # | where | cause | fix |
|---|---|---|---|
| w4-1 | **macOS crash**, every run at frame ~2400 (the Iguana cinema): `Blast_CanHit` <- `ImpactEvent_ApplyBlast` <- `ImpactEvent_DirectDamage` | `ImpactEvent_DirectDamage(.., s32 arg1, ..)` and `func_00239058(.., s32 b, ..)`: the source actor truncated | `sptr` (asm_diff 0) |
| w4-2 | **all hosts**: the Mother never played her attack animation (mode 4 ended after 3 frames), so she never hurt the player | `CAI__UpdateModeAnim` called the mode row's animation picker as returning a pointer; the pickers return `s16` (`MotherBoss_PickPhaseAnim`) and x86-64/arm64 leave the upper bits of the return register unspecified. Linux got `0x42BF55FA` for `0x55FA`, so `PickRandomAnim` found no animation | port branch takes the result as `(s16)`: the action ids are 16-bit (the tables are BE16, func_0021EA3C). Now the Mother's slam hits the player on both hosts (health 100 -> 75 by frame 15,000) |
| w4-3 | **macOS**: actors lit differently from Linux, and the lighting changed with unrelated code (`T2_ACTOR_LOG` made the Mother and the hand dark) | `CScene__ComputeLights` read `dirs[k]` as `((B19bV3f *)(w + 128))[k]`: the ROM's frame puts `dirs` right after `w[128]`; the host frame need not (Linux happened to) | port branch reads `dirs[k]` (asm_diff 0) |
| w4-4 | **all hosts**: `sShadowQuadDL`, `sShadowQuadDL2`, `D_800ACFC8` | gendata's address heuristics took a display list's non-address w1 for a pointer: `gSPSetGeometryMode(0x210004)` became the address of `CMtxF__SetShadowProjection` (random geometry-mode bits, including G_LIGHTING_POSITIONAL) and the `gSPTexture` scale `0x80008000` a null pointer | gendata: a Gfx w1 that is not an address operand stays a number (168 words in 11 lists) |
| w4-5 | **all hosts**: the legal screen lost most of its lines (macOS: different lines per run); attract demo 1 did not follow its recorded input | `ControllerProfile` has TARGET_PORT-only members (the host rumble pool), so gendata's "N64" layout made `gMergedController` 0x298 bytes and put `gControllers` (and gPakMesgBuf, gPakWatched, gPakInserted, gFirstControllerPort) inside it as aliases. `Cont_InitAll` then wrote four host profiles past the 0x298-byte object: over the legal strings on the Mac, over other data on Linux | gendata `N64_EXTENT` (gMergedController 0x224, gControllers 0x890) and both in `RUNTIME_BSS` (they were initialised from code bytes). Now `gControllers` is its own 0xA60-byte object and `gControllers_buttons` an alias inside it |

w4-5 changes the attract demos: demo 1 now opens the weapon wheel and fires, and an enemy dies at
frame ~8200. Both hosts then crash in `EnemyAnim_PickDeath` (the N64 view of the death path),
which names-R8_w2 58b5668a fixes. So the 26,000-frame boot needs w2 merged.

### 10.2 Mac against Linux after the fixes

Same branch on both hosts, both headless (Mac `--offscreen-gl`, Linux SDL offscreen):

| run | frames | logs (state, actors, mode switches) | screenshots (>1% of the pixels off by >24/255) |
|---|---|---|---|
| journey with firing | 15,000 | 272 lines, identical | 0 of 29 |
| warp 51 -> 52 -> 51 | 16,000 | 288 lines, identical | 0 of 31 |
| Mother (`T2_WARP_TO=-4`) | 15,000 | 556 lines, identical (the player hurt the same way) | 1 of 30: frame 14,500, the Mother's spot flash is green on Linux and not on the Mac |
| Mantid Queen (7501) | 15,000 | identical | 0 of 30 (before w4-5) |
| demo-1 enemies | 8,600 | identical before w4-5 (508 lines) | the legal screen only (fixed by w4-5) |
| boot | 26,000 | identical before w4-5 | the legal screen only (fixed by w4-5) |
| `--audio --wav`, 6,000 | | | 100.10 s, peak 28,806, RMS 2,270 |

The round-7 open item (the boat and Turok's hand missing on the Mac, section 9.3) is gone: frame
13,001 of the journey matches Linux. The other round-7 differences (frame 10,502, the cinema
dinosoid lit differently; the Mother) were w4-3.

### 10.3 Open on the Mac

- The Primagen (`T2_WARP_TO=7602`) crashes in `Primagen_UpdateSpawner` (N64-offset view of
  gLevelScript): fixed on names-R8_w3 ad18ae32 / 558c0f04 (rerun gendata after the merge).
- The 14,500 Mother frame (the green spot flash): a render-side difference not chased.
- More `gendata` "N64" sizes may count TARGET_PORT members. The aliases list
  (`port/gen/data_*.c`, `/* X = Y + off */`) still has declared symbols inside bases of another
  name (`__osThreadTail` holds `gAdecBandEnds`, `__osRunQueue`...; `gCrashFont` holds
  `sCrashCursorX`): worth a check.
- `T2_DLDUMP_VTX=3`'s texture hash covers bytes past small palettes, so it differs between hosts
  where the image does not.

## 11. Round 9 (part x4): the round-8 merged tree on the Mac, host determinism, the attract demos

Branch `names-R9_x4` (from 5437cfc0). Every `src/` commit was checked on the box with
`tools/asm_diff.py --base 5437cfc0 -j 8` (scratch copy `/root/R9_x4mac/repo`; `OUT/r9/asmcheck.sh`
applies `git diff 5437cfc0 -- port src include tools` there): **0 differences** at every commit.
No `include/` changes. The Linux runs for comparison are this branch built in `/root/R9_x4mac/out`
(Debug, low memory, `/root/R9_x4mac/build.sh`, `FRESH=1` drops decls.json; `run.sh NAME FRAMES
[args]`). The box's ELF is unchanged (sha1 `61dcdf3f…`); the archive is the box's round-8 one
(`OUT/turok2_543.o2r`, sha1 `b6b9d484…`).

Build on the Mac: `OUT/rebuild.sh --fresh` (about 35 s). Runs: `OUT/r9/battery.sh` (boot, journey,
warp, fight, Mother, door, save then load, `--audio`), `OUT/r9/dets.sh` (the same scenarios with
`T2_DET_LOG=1`), all headless (`--offscreen-gl` or `--no-window`).

### 11.1 Fixes

| # | where | cause | fix |
|---|---|---|---|
| x4-1 | **macOS crash** on the first save (`T2_SAVE_PORTAL`, SAVE GAME): `t2_memset` <- `SaveGame_Build` | `buf = (SaveGame *) *handle` with `s32 *handle`: the low half of `CMemEntry.data` | `*(SaveGame **) handle` under TARGET_PORT |
| x4-2 | **macOS crash** when an enemy shot hits the player (demo 1 with `T2_TELEPORT`/`T2_WAKE_ENEMIES`, frame 6041): `CPlayer__OnParticleHit` | the hit callback `CPlayer__OnParticleHitCallback(actor, s32 aiFlags, s32 particle)`: the particle pointer truncated | `sptr` (both functions) |
| x4-3 | **macOS crash** on the player's death cinema: `CCameraPool__ReleaseAllTexts` <- `Cinema_Start` | the text lists walked with `s32` links (`temp = *node`) and the camera list by `+ 4` (N64 `m_pNext`) | pointer-wide links, `T2_OFFSETOF(CCamera, m_pNext)` |
| x4-4 | **all hosts**: every attract demo played garbage input from its first frame | gendata typed each demo (`gDemoTable` -> `DemoHeader *`) as a **table of DemoHeader** (27 of them in 0x438 bytes), so the whole demo was swapped as s16 pairs, the input bit streams too (`DemoStream_ReadBit` reads bytes MSB first) | gendata `HEADER_POINTEES`: the 0x28-byte header typed, the rest as bytes in ROM order. All 24 demos checked against the ROM |
| x4-5 | **all hosts**: the stick weapon wheel closed on its first frame while A was held (state 2 -> 3 every other frame), so the stick turned the player instead of choosing; demo 1 then walked into the wall beside its door | `CBarrel__ShouldClose` was an N64-offset view (`self + 0x380` `m_bStickWheel`, `other + 0xA50` the controller) | host body by member |

`T2_DET_LOG=N` (new, port/game/t2det.c, debug only): every N frames from `T2_DET_FROM` one line
with the RNG seeds, the player's position/facing/velocity as **float bits**, health, mode, buttons,
stick, and an FNV-1a hash over every level actor and pooled instance (position, facing, health, AI
mode/action, animation index/frame, flags); `T2_DET_ACTORS=1` adds one line per actor. `%.1f` in
`T2_STATE_LOG` hides drift; this does not.

### 11.2 Host determinism: Mac = Linux, frame for frame

`T2_DET_LOG=1` (every frame) on both hosts, host addresses masked, the same binary sources:

| run | frames traced | lines that differ |
|---|---|---|
| boot, the 3 attract demos, the loop (no input) | 1-26,000 | **0** (before and after x4-4/x4-5) |
| journey (bow, ramp, swim) | 12,800-15,000 | 0 |
| warp 51 -> 52 -> 51 | 12,800-16,000 | 0 |
| level-52 fight (kills) | 12,800-14,800 | 0 |
| the Mother | 12,800-18,000 | 0 |
| the door | 12,800-14,000 | 0 |
| demo 1 with `T2_TELEPORT`/`T2_WAKE_ENEMIES` | 5,400-8,800 | 0 (after x4-2/x4-3; before them the Mac crashed at 6041, identical up to 6040) |

Rendering does not feed back: `--offscreen-gl` and `--no-window` give the same trace on the Mac,
and Linux with and without its SDL window agree too. So **there is no float divergence between
x86-64 and arm64** in any of these runs (the game is built with `-ffp-contract=off`, and the
arm64 float -> unsigned conversions were fixed in round 7, v4-4).

**"Demo 1 kills an enemy on the Mac, not on Linux" (STATUS round 8) was not a host divergence.**
On w4's branch it was the teleported demo-1 scenario (`T2_TELEPORT=5600:... T2_WAKE_ENEMIES=1500`),
and the Mac's "kill" was a crash: on the merged tree it is x4-2 (the enemy's shot hitting the player
crashed the Mac; Linux low memory carried on). The plain boot's demo 1 kills nothing on either host
(checked on 08391f6f, names-R8_w4 and 5437cfc0 with the same trace, `OUT/r9/bis.sh`).

### 11.3 The attract demos against the N64 (x3's mupen64plus reference)

x3's reference frames (mupen64plus on the box, `/root/R9_x3/ref/s_d`, every 10 emulator frames from
the title): the player stands facing a door with the war blade, opens the wheel, picks the PLASMA
RIFLE, walks straight through the door into an amber room, meets the first enemy, takes damage
(100 -> 51) and the demo ends. Before x4-4 the port played noise (the wheel open from the first
frame, the player spinning). After x4-4 and x4-5, on both hosts:

- frames 5,467-5,520: level 81, the war blade; A opens the wheel (~15 game frames in, as on the
  N64); the plasma rifle (weapon 10, 150 cells) is picked at 5,586 with **no turn** (rotY -3.14 ->
  -3.15);
- 5,600-6,300: straight through the door ((1884, -2601) -> (1878, -1101)), the amber room; frame
  6,302 is **the same view as the reference at the first enemy** (pipe, grating, the enemy on the
  left, 100 / 150 on the HUD);
- **then the enemies diverge**: on the N64 the first enemy is still turned away there; on the port
  it already faces and attacks. The player takes more damage (100 -> 91 by 6,660, then 76 ... 0 at
  7,920) and dies; the demo ends at 8,157 (it used to end at 8,800) instead of with 51 health.

So the input path is right now; what remains is in the enemies (their wake-up/hearing or the RNG
state at the demo's start: `Demo_Start` draws `Random_Next` and nothing reseeds it). Next: compare
the RNG seeds and the enemy's AI mode at the first enemy against the emulator's RAM (mupen64plus
debugger or a RAM dump at a known frame).

New boot mode switches (both hosts): 5,467 demo 1 -> 8,157 legal; 13,035 demo 2 (level 131) ->
15,040; 19,920 demo 3 (level 109) -> 21,127; the title again at 24,749.

### 11.4 The round-8 runs on the Mac (merged tree + x4)

All headless, all exit 0, no dlcheck reports: the boot (26,000); the journey (now ends at **(352.5,
-780.7, 879.4), rotY -1.55**: with x4-5 the bow is chosen with no turn; the old (795.2, -781.5,
-37.4) came from the stick turning the player while the wheel closed); the warp 51 -> 52 -> 51;
the level-52 fight (enemies 11 and 12: 20 -> 0, death mode 52, melt); the Mother (61,440 -> 51,200,
`parts 5/6`, the same as Linux); the door (the player through it at (1650.5, -307.2, 136.9)); save
("SAVE WAS SUCCESSFUL", `mpk_saves.py`: NT2E 51 "TUROK 2" A, 90 pages, checksum OK) then a new boot
loading it (level 51 at frame 4,555); `--audio --wav` 8,000 frames: 133.48 s, peak 32,768, RMS
3,248.7 (the demo now plays its own sounds).

### 11.5 Open on the Mac

- Demo 1's enemies (11.3).
- The x5 soak and the `--high-memory` checks on Linux cover what the Mac hits with full pointers;
  x4-1..3 were all found by the Mac first (Linux low memory hides them).

## 12. Round 10 (part y4): the round-9 merged tree on the Mac, host determinism again, real time, the controller

Branch `names-R10_y4` (from cc0c322b). Every `src/` commit was checked on the box with
`tools/asm_diff.py --base cc0c322b -j 8 FILES` (scratch copy `/root/R10_y4mac/repo`;
`OUT/r10/sync.sh` applies `git diff cc0c322b -- port src include tools` there): **0 differences**
at every commit. No `include/` changes. The box's ELF and archive are unchanged (ELF sha1
`61dcdf3f…`, archive `b6b9d484…`, the Mac uses `OUT/turok2_543.o2r`). The Linux runs for comparison
are this branch built in `/root/R10_y4mac/out` (Debug, default high memory, LUS from the
coordinator's checkout: `/root/R10_y4mac/build.sh`, `FRESH=1` drops decls.json).

Build on the Mac: `OUT/rebuild.sh --fresh` (22 s; gendata: 1,532 symbols, 1,526 typed, the 3 typed
.bss tails: the same report as Linux). Scripts (`OUT/r10/`, the same files on the box in
`/root/R10_y4mac/r10/` with a `box.env`): `run.sh NAME FRAMES [args]` (headless: `--offscreen-gl`, or
`--no-window` with `NOGL=1`), `battery.sh` (boot, playthrough, Mother, warp, fight, save, load,
audio), `dets.sh [names]` (the same with `T2_DET_LOG=1`, two at a time), `soak.sh`.

### 12.1 Fixes

| # | where | cause | fix |
|---|---|---|---|
| y4-1 | **all hosts**: the Mother recipe and the totem defence **diverged between macOS and Linux** (Mother: frame 30,163; playthrough: 21,010; with the particle hash below, 14,427 and 19,114) | `CParticleSystem__Spawn` reads `gImpactEventHandlers[m_ImpactEventType[4]].flags & 1` with no 0xFFFF check (the other readers check). The N64 reads RAM 0x8012D07C (table 0x800AD080 + 0xFFFF * 8 + 4): **0x00C800FF in every reference RDRAM dump** (x3's `ig/`, `d1/`). The host read went 1 MB past the table into whatever host memory held, so bit 0, and with it the particle's hit flag 0x80000000 (`Sweep_TestBox`'s 1.5x node boxes), depended on the host: a plasma shot hit the Mother's node 41 on the Mac and missed on Linux | under TARGET_PORT the index is checked: 0xFFFF gives the N64's 0x00C800FF |
| y4-2 | **all hosts** (debug switch): after y4-1 the Mother recipe still drifted by 1 ulp at 14,543 | `T2_AIM_NODE`'s pitch used libm's `atan2f`, whose last bit differs between macOS libm and glibc (the player's fire quaternion then differs by 1 ulp) | `T2Aim_Atan2f`: + - * / only (max error ~1e-5 rad) |
| y4-3 | **macOS crash** (`__stack_chk_fail`) at 22,900 of the playthrough, right after the totem defence is won | `TotemDefense` `stunHelper`: `u8 anim[0x18]` passed to `DamageEvent_Set` as a `DamageEvent` (0x28 bytes on the host: two pointers). Linux overwrote its stack silently | `DamageEvent anim[1]` under TARGET_PORT |
| y4-4 | **macOS crash** in the soak, 13 of 26 new-game/warp runs: `Controller_MenuLeft` <- `EnterCheatMenu_UpdateTextField` | the cheat-entry and name-entry fields (func_0041AAA0, func_0041D2C4, func_00453A08) read the controller as an `s32` at N64 0x20 and called through `(s32 (*)(s32))`: the host `Menu.controller` is a pointer at another offset, and the Mac's statics are above 4 GB (Linux non-PIE ones fit, so Linux never crashed). `func_0041D2C4` also passed `Menu.player` as an s32 | `((Menu *) menu)->controller` / `->player` under TARGET_PORT |

The playthrough without y4-1 "won" the totem on Linux only by that host read: with the N64's
flags the plasma rifle's 150 cells run out at 4 of the 8 kills (on both hosts; the Mac already lost
it before the fix). `playthrough.py --fight` now also sets `T2_FILL_AMMO=600`.

New debug tools (both hosts): `T2_DET_LOG` lines now end with `ptcl HASH`, a hash of the 128
particles (position, velocity, flags), and `T2_DET_PARTICLES=1` prints one line per live particle:
a shot that flies or hits differently shows up frames before it changes the RNG or an actor.
`T2_RNG_TRACE=F1..F2` prints every RNG draw (`Random_Next`, `Random_Float`, `Random_Blend`,
`RandomInPackedRange`) with the seed and the caller's address (`atos -o turok2 -l BASE ADDR` on the
Mac, `addr2line -fie turok2 ADDR` on Linux). The chain that found y4-1: RNG step counts (Mac 28 /
Linux 26 draws in frame 21,009) -> `RandomInPackedRange` callers (two extra particle spawns) ->
`CParticle__HitInstanceBody` -> the particle hash (the shot stops earlier) -> `Sweep_TestBox`'s node
box (radius x1.5) -> the particle's `m_dwHitFlags` sign -> the handler-table read.

### 12.2 Mac = Linux, frame for frame (`T2_DET_LOG=1`, final binaries)

| run | frames traced | lines that differ |
|---|---|---|
| boot, the 3 demos, the loop | 1-26,000 | 0 |
| **the Port of Adia playthrough** (`--fight 10`) | 12,800-27,000 | **0** (before y4-1: from 21,010) |
| **the Mother recipe** | 12,800-36,000 | **0** (before y4-1/y4-2: from 14,427 / 14,543) |
| warp 51 -> 52 -> 51 | 12,800-16,000 | 0 |
| the level-52 fight | 12,800-14,800 | 0 |

### 12.3 The round-9 runs on the Mac (all headless, exit 0, no dlcheck reports)

- boot (26,000): every mode switch at Linux's frame (demo 1 5,467 -> 8,157, demo 2 13,035, demo 3
  19,920, the title 24,749);
- **the playthrough** (27,000): every level event at x1's frames (51 -> 52 -> 53 -> 52 -> 55 -> 54
  -> 56 -> 57 -> 58 -> 59 -> 109), the totem defence won (kills 8/8 at 22,700, "MISSION
  COMPLETE"), **the hub at 23,400 with "NEW CHEAT ENABLED"**, health 89 (Linux: the same);
- **the Mother** (36,000): phase 1 at 16,600, phase 2 at 26,000, health 0, **the hub at 32,400**
  (then the recipe's held stick walks Turok into the hub's portals: 157, 158 ...);
- warp 51 -> 52 -> 51, the fight (enemies 11 and 12 to 0, mode 60), save (`mpk_saves.py`: NT2E 51
  "TUROK 2" A, 90 pages, checksum OK) then load (level 51 at 5,520), `--audio --wav` 9,000 frames:
  150.17 s, peak 32,768, RMS 3,480.8 (Linux's numbers).

### 12.4 Soak on the Mac (`tools/port/soak.py`, lldb)

`soak.py` now runs under lldb when there is no gdb (`--debugger lldb`): `lldb -b -o run -k bt`,
the stop reason (`EXC_BAD_ACCESS`, `SIGABRT` ...) and the backtrace parsed from lldb's `frame #N:
... module`func at file:line`, the exit status from "Process N exited with status = RC", a hung
game killed by its pid (debugserver starts it in its own process group). `--draw` adds
`--offscreen-gl` on macOS (no window). `OUT/r10/soak.sh`: attract 2 x 100,000, new game seeds 0-11
x 36,000, warp seeds 0-11 x 30,000 (`levels.py` -> `OUT/r10/lv/warp_dests.json`), new game
`--draw` seeds 100-103 x 20,000.

Before y4-4: attract 0 crashes (199,920 frames, 11 s per 100,000 frames); new game 7 of 12 and warp
6 of 12 runs crashed in `Controller_MenuLeft` (y4-4), warp seed 2 in `T2Port_ReadAsset` (the known
open class of STABILITY.md: a host address used as a ROM key, here after `T2_WARP_TO=6010@13744`).
After y4-4 (30 runs, 1,062,660 frames): attract 0, new game 0 of 12, `--draw` new game 0 of 4,
warp 1 of 12: only the `T2Port_ReadAsset` abort (warp seed 2). y5 fixes that class on its branch
(S16, dea84b6c: the asset key masked with 0x1FFFFFFF; `CParticleSystem__Spawn` puts the draw
layer in the texture key's top bits, which the N64's PI ignores).

### 12.5 Real time on the Mac (`--pace`, Debug build, M5)

The whole playthrough paced at 60 VI/s with sound and pixels (`--offscreen-gl --audio --pace
--bench 13000`, 27,000 VIs): **450.52 s for 450.00**, the totem won and the hub reached as unpaced
(pacing does not change the game: it only sleeps between VIs). 48 late VIs, 44 by 1 ms or more,
**12 in gameplay** (from VI 13,000), the worst 16.3 ms (13,552, the first level's load); the worst of
the run 90.9 ms at VI 8,000 (the boot's demo -> legal switch) and 28-39 ms at 10,923-10,934 (the new
game's load). Per VI in gameplay: game 0.36 ms, audio 0.36 ms, draw 1.87 ms (glFinish), total 2.63
ms (p99 8.4) of the 16.7 ms budget; a drawn frame every 33.35 ms (p99 37.5). The process sleeps 73%
of the time. PERF.md section 5 has the table.

**Audio underruns**: headless runs have no sound device (LUS's AudioPlayer is a no-op), so `--pace
--audio` now drives a simulated 22,050 Hz device: 2,048 frames prefilled at the first audio frame,
drained in real time; a VI that finds it empty counts as an underrun. Over the paced playthrough the
audio manager produced 9,935,264 frames (22,052.9 Hz), **1 underrun** (VI 11,263: the new game's
load, the 28-39 ms late VIs just before it), **none in gameplay**; the buffer stayed between 48 and
8,161 frames.

**Input latency** (from the code path; no window this round): with a window, each VI starts with
`window->HandleEvents()` (SDL polls the gamepad and keyboard into LUS's ControlDeck); the game reads
the pads once per game frame (`osContGetReadData` -> `ContRead` -> LUS `osContGetReadData`), every
second VI, as on the N64; the display list built in that frame is drawn and swapped at the end of
the same VI. A press therefore reaches the game within 1 VI (16.7 ms) and the screen 1-2 game frames
later (33-67 ms), like the console; at 2.6 ms of work per VI the host adds no queueing.

### 12.6 The controller for a human player (code path checked, not run with a window)

- **Only with a window** (`turok2 --archive turok2.o2r --rom baserom.us.z64 --game --draw`, no
  `--no-window`/`--offscreen-gl`): `T2Game_Run` installs `ContInit`/`ContRead` (port/app/game.cpp),
  which call LUS's libultra replacements: `osContInit` initializes SDL gamepads and the ControlDeck
  (it loads `gamecontrollerdb.txt` if one is found in the app directories; SDL3's built-in mappings
  cover the common pads), `osContGetReadData` fills each port's buttons and stick from its mapped
  devices. Port 1 also ORs in `--input` (the scripted buttons and stick work with a pad too).
  Headless runs install a virtual pad on port 1 instead (`--input` only, the pak per `--pak-*`).
- **Default mapping** (LUS, `ControllerDefaultMappings.cpp`): gamepad A (south) -> A, B (east) ->
  B, left shoulder -> L, left trigger -> **Z (fire)**, right trigger -> **R (jump)**, Start ->
  START, D-pad -> D-pad, **left stick -> the N64 stick (turn/look)**, **right stick -> the C
  buttons (C-up/C-down walk, C-left/C-right strafe)**: Turok 2's default N64 layout maps onto a
  modern pad as a twin-stick. Keyboard: WASD the stick, the arrow keys the C buttons, X = A, C = B,
  Z = Z, E = L, R = R, Space = START, T/F/G/H the D-pad. The mapping is saved in the LUS config
  (`turok2.cfg.json`) once changed.
- The Controller Pak is per port (`--pak-dir`, `--pak-blank`); rumble is not wired (`osMotorInit`
  returns "no pack").

### 12.7 Open on the Mac

- The `T2Port_ReadAsset` abort after some debug warps (fixed on names-R10_y5, S16).
- A windowed run with a real gamepad (12.6) and real CoreAudio output: not done (headless only).
- Release build pacing (round 7 had 0 late VIs in level 51; Debug now has 12 in the full
  playthrough, all at loads).

## 13. Round 11 (part z4): the round-10 merged tree on the Mac, levels 5 and 6

Branch `names-R11_z4` (from 2df5517d). No `src/` or `include/` change (nothing to asm_diff). The
box's ELF and archive are unchanged (ELF sha1 `61dcdf3f…`, archive `b6b9d484…`; the Mac uses
`OUT/turok2_543.o2r`). Linux runs for comparison: this branch built in `/root/R11_z4mac/out`
(Debug, PIE, LUS from the coordinator's checkout; `/root/R11_z4mac/build.sh`, `FRESH=1` drops
decls.json). Scripts in `OUT/r11/` (the same files on the box in `/root/R11_z4mac/r11/` with a
`box.env`): `run.sh NAME FRAMES [args]` (`--offscreen-gl` on the Mac, nothing on Linux, `--no-window`
with `NOGL=1`), `battery.sh [names]`, `dets.sh [names]` (`T2_DET_LOG=1`), `sync.sh` (applies
`git diff 2df5517d` to the box copy and rebuilds there). Build on the Mac: `OUT/rebuild.sh --fresh`
(28 s).

### 13.1 The round-10 runs on the Mac (headless, exit 0, no dlcheck reports)

- boot + the 3 demos (26,000); **level 1** (`playthrough.py --fight 10`, 27,000): the totem
  defence won (kills 10/8), **the hub at 21,500** (Linux's frame); **level 2** (49,500): soul gate 1
  (21,313), the second sister (37,623), "all soul gates destroyed. mission complete." (42,063),
  135's two "graveyard gate open." (45,107 / 45,909), the exit -> 125: Linux's events;
- **the Mother** (36,000): death and cinema (30,817), the hub; **the Blind One** (25,000): death
  (mode 9) at 22,683, the hub (23,400);
- save (`--pak-dir`, `T2_SAVE_PORTAL`) then load (level 51); `--audio --wav` 9,000 frames: 150.17 s,
  peak 32,768, **RMS 3,402.2** (= Linux, STATUS.md section 2).

- soak (`tools/port/soak.py --kind newgame --seeds 0:8 --frames 36000`, lldb, 4 jobs): **8 runs,
  288,000 frames, 0 crashes**.

### 13.2 Mac = Linux, frame for frame (`T2_DET_LOG=1`)

| run | lines | differ |
|---|---|---|
| boot, the 3 demos, the loop | 52,000 | 0 |
| level 1 playthrough | 28,402 | 0 |
| the Mother | 46,402 | 0 |
| the Blind One | 24,402 | 0 |
| the level-52 fight | 4,002 | 0 |
| **level 2** | 73,402 | **from 42,124** (gf 633, level 132, 61 frames after the second soul gate falls) |

The level-2 divergence: in frame 42,123 the RNG sequence is the same on both hosts up to a
`CParticleSystem__Spawn` loop (`n = countBase + randomCount(countRange)`, then `allocateEffect`
per particle) where the Mac stops after 6 effects of one emitter entry and starts the next entry
(the chance draw, func_0022D2FC.c ~1868) while Linux spawns a 7th: `allocateEffect` returned 0 on
the Mac only. The slot hash (`ptcl`) was equal, so this round's det line also hashes **the pool's
lists** (free, draw, the 3 active lists, as slot sequences, port/game/t2det.c): still equal up to
42,123 on both hosts. So the state at the frame's end matches; the difference is made inside
frame 42,123 (between its start and the spawn, or a free/alloc inside the spawn itself). Next:
`T2_DET_PARTICLES` + a print of `free.m_nCount` at each `allocateEffect` in that frame on both
hosts. Not fixed this round (the game runs on; the level-2 events after it are Linux's).

### 13.3 Open on the Mac

- The level-2 divergence above.
- The crash in level 122 (section 9 of PLAYTHROUGH.md): **both hosts**.

## 14. Round 12 (part a4): the round-11 merged tree on the Mac, flush-to-zero, the level-2 divergence

Branch `names-R12_a4` (from 046a67f9). No `src/` or `include/` change (nothing to asm_diff). The
box's ELF and archive are unchanged (ELF sha1 `61dcdf3f…`, archive `b6b9d484…`; the Mac uses
`OUT/turok2_543.o2r`, the same bytes). Linux runs for comparison: this branch built in
`/root/R12_a4mac/out` (Debug, PIE, LUS from the coordinator's checkout; `/root/R12_a4mac/build.sh
[gen]`). Scripts in `OUT/r12/` (the same files on the box in `/root/R12_a4mac/r12/` with a
`box.env`): `scen.sh` (every scenario of STATUS.md section 1: `boot play1..play4 mother blind
queen prima save audio`), `run.sh NAME FRAMES [args]` (`--offscreen-gl` on the Mac, `--no-window`
with `NOGL=1`), `battery.sh [names]` (pixels, state logs), `dets.sh [names]` (`T2_DET_LOG=1`, two
at a time), `rng.sh` (the level-2 frames with `T2_DET_PARTICLES` + `T2_RNG_TRACE`), `soak.sh`,
`sync.sh [gen]` (applies `git diff 046a67f9` to the box copy and rebuilds there). Build on the
Mac: `OUT/rebuild.sh --fresh` (26 s).

### 14.1 arm64 flush-to-zero (z5's FPCR FZ, port/runtime/t2os.c)

- The game thread runs with **FPCR = 0x01000000** (FZ, bit 24; read with lldb at a breakpoint on
  `CEngineApp__Update`, thread 5: the thread `osCreateThread`'s trampoline started).
- The boot and the 3 attract demos (26,000 frames, `T2_DET_LOG=1`): **Mac = Linux, 0 lines differ**
  (52,000 det lines), so demos 1/2/3 give Linux's trace (which is the N64's RAM for demos 1 and 3,
  demo 2 to gf 607: FIDELITY.md 6).
- Control run: the same with FZ cleared by lldb (`register write fpcr 0` at the first
  `CEngineApp__Update`, then continue): demo 2 diverges at **frame 14,130 (gf 426)**: the player's
  velocity z is the denormal `0x80439b0b` where Linux (and the N64) have `0x80000000`, z5's exact
  finding. So the FZ path is live and needed on arm64.

### 14.2 The level-2 divergence: libm's `atan2f` in `T2_AIM_AT` (fixed)

The round-11 route (`playthrough.py --level 2`) now diverged at frame **42,184**, and only in the
`ptcl` hash (actors and RNG stay equal; the particles reconverge at 42,186). `rng.sh` on both
hosts: the RNG draws in frames 42,180-42,186 are the same seeds in the same order; the particle
lines differ in **one bit**: the player's plasma shot spawned in gf 663 (slots 52 and 67) has
velocity y `0x4487698f` on the Mac and `0x4487698e` on Linux. The frame is inside the route's
`T2_AIM_AT=41160-42310:...` window, and `T2Port_AimAt` (port/game/t2debug.c, round 10 y1) set the
camera pitch with **libm's `atan2f`**, whose last bit differs between macOS and glibc (the same
class as round 10's y4-2 in `T2_AIM_NODE`). Fix (8e12748c): `T2Port_AimAt` calls `T2Port_Atan2f`
(the + - * / atan2 of port/game/t2bosslog.c). It was the last libm transcendental in a game-state
path: the game's own sin/cos/acos are decompiled code, and `port/` has no other `atan2f`, `sinf`,
`powf` ... call.

After the fix both level-2 routes are identical on both hosts to their end: this round's
(64,000 frames) and **round 11's route** (`OUT/r11/p2`, 49,500 frames), which is the run where z4
saw `allocateEffect` return 0 on the Mac only at 42,124 (its frame is in the same `T2_AIM_AT`
window: a shot 1 ulp apart flies/hits a frame differently, which changes the free list inside
the frame). So the per-`allocateEffect` free-list print was not needed: the divergence is gone,
not explained away. **Linux's level-2 trace changes too** (from frame 20,416, the first
`T2_AIM_AT` window): the events, the soul gates, the sisters and the hub at 62,000 stay the same.

### 14.3 The round-11 runs on the Mac (headless `--offscreen-gl`, exit 0, 0 dlcheck reports)

| run | frames | the hub (level 60) | Mac = Linux (`T2_DET_LOG`, lines) |
|---|---|---|---|
| boot + the 3 demos | 26,000 | - | 52,000: 0 differ |
| level 1 (`--fight 10`): beacons, the children, the totem | 27,000 | 22,000 | 54,000: 0 |
| level 2: "all soul gates destroyed", "all sisters of despair banished" | 64,000 | 62,000 | 128,000: 0 (after 14.2) |
| level 3: "all prisoners free" ... 111 | 36,000 | 31,500 | 72,000: 0 |
| level 4: "blind ones trapped", the Blind One (112 -> 1) | 46,000 | 42,000 | 92,000: 0 |
| the Mother (health 0/128,000) | 36,000 | 29,000 | 72,000: 0 |
| the Blind One | 25,000 | 23,500 | 50,000: 0 |
| the Mantid Queen | 30,000 | 28,500 | 60,000: 0 |
| **the Primagen**: the ending cinema (levels 11 and 10 from 40,000), the credits, the hub | 62,000 | 59,000 (Linux: 58,828) | 124,000: 0 |
| save (`T2_SAVE_PORTAL`) / load (level 51) | 14,100 / 6,500 | - | 28,200 / 13,000: 0 |

(The hub frames are the first `T2_STATE_LOG=500` sample in level 60.) `--audio --wav` 9,000
frames: 150.17 s, peak 32,768, **RMS 3,399.5** (= Linux, STATUS.md section 2). Screenshots
(checked by eye, not committed): the Primagen's ending cinema (the lightship over the portal,
40,001), the credits ("RYAN WICKERHAM AS THE PRIMAGEN ...", 50,003), the hub after level 4
(46,000).

Soak (`OUT/r12/soak.sh`, lldb, 4 jobs, no `--draw`): new game seeds 0-7 x 36,000 and levels seeds
0-40 x 25,000 (every level id 1-164): **49 runs, 1.31 M frames, 0 crashes, 0 hangs**.

### 14.4 Open on the Mac

- A windowed run with a real gamepad (section 12.6; still headless only).
- Levels 5/6 (a1/a2) and the continuous playthrough (a3) of round 12: rerun `dets.sh` on the
  merged tree (add their scenarios to `scen.sh`).
