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
`scratchpad/assets_preview/R4_s3_linux_s3branch_f300.png` shows "PROBLEM!", "DAMAGED
CONTROLLER PAK FOUND IN CONTROLLER #1.", "ATTEMPT REPAIR" (highlighted), "RETRY", "CONTINUE
WITHOUT SAVING". On the Mac that path needs a controller without a window: s1's PIF/controller
work, or a port flag that connects a virtual pad headless.
