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
| v4-7 | `ImpactEvent_AreaDamage` <- `ImpactEvent_Send(kind 3)` <- `FireAnimEvents` (new-game cinema, mode 12) | `ImpactEvent_AreaDamage(s32 arg0, s32 arg1, ...)`: the actor pointers as s32 | sent to v3 (the damage pipeline) |
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
