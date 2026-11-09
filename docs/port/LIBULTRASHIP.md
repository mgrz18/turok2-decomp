# libultraship integration (Phase 7, part p3)

Status: libultraship builds on the box, `turok2` (the host executable skeleton, `port/app/`)
links it, opens a window through LUS's context, mounts `turok2.o2r` through LUS's
`ArchiveManager`, reads **all 26,124 load units** through it (byte-identical to the miniz
archive reader and to the ROM reader of `port/assets/t2asset.c`), initializes the
controllers through LUS's `osContInit`, runs a test display list through Fast3D (F3DEX2)
and reads the rendered pixels back. Then it exits with 0. No game code is linked yet.

## 1. Build

```
# Debian 13 packages (LUS's .github/workflows/apt-deps.txt, minus tcc which we do not use)
apt-get install -y libsdl3-dev libpng-dev libglew-dev libzip-dev zipcmp zipmerge ziptool \
    nlohmann-json3-dev libtinyxml2-dev libspdlog-dev libgl-dev \
    libx11-dev libxext-dev libxrandr-dev libxcursor-dev libxfixes-dev libxi-dev \
    libxss-dev libxtst-dev libxkbcommon-dev
# (gdb was also installed, for debugging only)

CC=clang CXX=clang++ cmake -S port -B OUT -G Ninja -DCMAKE_BUILD_TYPE=RelWithDebInfo \
    [-DT2_LUS_SOURCE_DIR=/path/to/libultraship]       # default: FetchContent of the pinned commit
ninja -C OUT turok2                                  # also builds OUT/t2port.o2r
python tools/assets/mkarchive.py baserom.us.z64 -o OUT/turok2.o2r -j 8
SDL_VIDEODRIVER=offscreen OUT/app/turok2 --archive OUT/turok2.o2r [--rom baserom.us.z64] [--frames N] [--no-window]
```

- **Pin**: libultraship tag `2.0.0.0` = `c6ecfdb4` (2026-08-15, the first "epoch 2" release;
  `main` was `759b134`, 2.0.0.0+28, on 2026-11-09). `port/cmake/libultraship.cmake` takes a
  local checkout (`T2_LUS_SOURCE_DIR`) or FetchContent's the pinned commit; both were built
  on the box (the FetchContent build pulled the same `c6ecfdb4`). LUS itself FetchContent's
  imgui (v1.91.9b-docking, patched), thread-pool v4.1.0, prism, monocypher and stb at
  configure time, so a first configure needs the network.
- **Options we set**: `GBI_UCODE=F3DEX_GBI_2`, `ENABLE_SCRIPTING=OFF` (no TCC),
  `INCLUDE_MPQ_SUPPORT=OFF` (no `.otr`). LUS needs CMake >= 3.24.
- **Cost**: ~185 objects, 65 s wall on 12 jobs (12 CPU-min) from clean, LUS dominates.
  Warnings in LUS headers seen from our TU (harmless): `-Wdefaulted-function-deleted`
  (Context.h), `GIMMCMD` redefined (lus_gbi.h vs gbi.h).
- **Headless**: SDL3's `offscreen` video driver gives a real OpenGL context (EGL, Mesa
  llvmpipe) with no X server: `SDL_VIDEODRIVER=offscreen` is all that is needed; no Xvfb.
- **Layout** (agreed with p1, whose game library is `port/cmake/game.cmake`, target
  `turok2_game`): `port/CMakeLists.txt` has `option(T2_BUILD_GAME_LIB)` and
  `option(T2_BUILD_EXE)`; the exe part is `include(cmake/libultraship.cmake)` +
  `add_subdirectory(app)`. `port/app/CMakeLists.txt` builds `t2asset` (the Phase 6 reader,
  miniz with `MINIZ_NO_ZLIB_COMPATIBLE_NAMES` because LUS links libzip -> system zlib) and
  `turok2`.

## 2. What the skeleton proves (box run, 2026-11-09)

```
archive: /root/P_p3/out/run/../turok2.o2r
  mounted archives: 2, game versions: 49088A11
    /root/P_p3/out/build/t2port.o2r: no game version (port archive)
    /root/P_p3/out/run/../turok2.o2r: Turok 2 US (49088A11)
  version: byteOrder 0, gameVersion 49088A11, ext 'T2AV' 1.0
  ListFiles(t2/u/*): 26124 entries
T2Asset over ArchiveManager: 26124 units in t2/units
  t2/u/00FCEA9C rom    2E8 host    520 flags 001 first bytes 00000003  blob: same bytes (size 1328)
  t2/u/00FCBFB4 rom     60 host     60 flags 002 first bytes 00000015  blob: same bytes (size 112)
  t2/u/001E0000 rom  2F194 host  2F194 flags 010 first bytes 00000002  blob: same bytes (size 192932)
  ...
reading every unit:
  libultraship  26124 units, 45890836 bytes, fnv1a64 6a391cadd4476aa6, 0 failed, 0.78 s
  miniz         26124 units, 45890836 bytes, fnv1a64 6a391cadd4476aa6, 0 failed, 0.86 s
  rom           26124 units, 45890836 bytes, fnv1a64 6a391cadd4476aa6, 0 failed, 0.77 s
window: backend OpenGL, video driver offscreen, 640x480
  osContInit: controller bits 01
  frames: 5 requested, 5 drawn; pad 0 buttons 0000 stick 0,0
  framebuffer #0 640x480 (native 320x240): centre 1F1600 corner 040C04 (5-bit channels)
  wrote ./turok2_testdl.ppm
OK (0 failed checks)
```

- The archive (`mkarchive.py -j 8`, 7.4 s) is the documented one: sha1 `bb77f744...`.
- **ARCHIVE.md's b1 recommendation ("Blob + skip header") holds with real LUS**, with one
  correction. `ArchiveManager::LoadFile("t2/u/XXXXXXXX")` + skip 0x44 gives the unit
  bytes; the `ResourceManager` Blob route (`ResourceLoad` / `ResourceGetDataByName`) gives
  the same bytes + 16 zero bytes, and `ResourceGetSizeByName` = host size + 16, exactly as
  predicted. **Correction**: in LUS 2.0 the Blob factory is *not* registered globally (only
  `Json` and `Shader` are, `ResourceLoader::RegisterGlobalResourceFactories`); a port must
  register `Ship::ResourceFactoryBinaryBlobV0` itself (Ghostship, SpaghettiKart and Starship
  all do, in `Engine.cpp`). The raw `LoadFile` route, which `T2Asset` uses, needs no factory.
- `port/app/t2asset_lus.cpp` is the `T2AssetBackend` on the `ArchiveManager` that PORT.md
  §10 anticipated: it reads `t2/units` once through LUS, then serves `lookup`/`load`
  through `LoadFile`, checks each entry's header (byte order 0, `OBLB`, `T2LU`, key, Blob
  size) and installs itself with `T2Asset_SetBackend`. The engine-facing API
  (`T2Asset_Read`, `T2Asset_ReadInto`, `T2Asset_HostSize`) is unchanged; the inner-address
  read (`T2Asset_ReadInto(0x20F194 + 0x1000, ...)`, the `__amDMA` case) works through it.
  Reading all 45.9 MB takes 0.78 s (deflate included). Mods: a later archive shadowing
  `t2/u/X` works if its size matches `t2/units`; a resized unit is refused today (TODO:
  take the size from the loaded entry, ARCHIVE.md "Mods").
- **Game version**: the archive's `version` (byte order 0, `49088A11`) is read by LUS
  (`Archive::HasGameVersion/GetGameVersion`). We pass **no** `validHashes` to
  `ResourceManager::Init`: with a non-empty list LUS refuses every archive whose `version`
  is not listed, including the ROM-independent port archive, which has none. The skeleton
  checks per mounted archive instead (exactly one Turok 2 US archive, no other ROM), as
  Ghostship does (`IsArchiveGameVersionUnknown`).
- **Fast3D needs the port archive**: LUS loads its GLSL/HLSL/Metal shaders as resources
  (`shaders/opengl/default.shader.glsl`) and aborts without them ("missing f3d.o2r?"). The
  build packs `libultraship/src/fast/shaders` into `t2port.o2r` (a plain zip, `cmake -E tar
  --format=zip`; the HM ports use `torch pack`). Mount order: `t2port.o2r`, `turok2.o2r`,
  then mods.
- **Controllers**: LUS's `osContInit` (SDL gamepad init + `ControlDeck::Init`),
  `osContStartReadData` (no-op) and `osContGetReadData` (`ControlDeck::WriteToPad`) work
  as the game will call them; keyboard is port 1 by default (bits `01`). The
  `ControlDeck` must be initialized through `osContInit` before the first
  `HandleEvents` that reaches it, or it throws ("requires WheelHandler"). Missing
  `gamecontrollerdb.txt` only logs an error (ship one next to the exe later).
- **Fast3D**: `Fast3dWindow::DrawAndRunGraphicsCommands(dl, {})` ran a display list made
  with LUS's `gbi.h` macros (`port/app/testdl.c`: fill rectangles) and the pixels were in
  the window framebuffer (fb 0; `mGameFb` is only the target when the game is shown in an
  ImGui viewport). Fast3D treats a fill whose colour image equals the depth image as a
  depth clear and skips it, so the DL must set both (as the game's frame setup does).

### Problems hit (and fixed in the skeleton)

| problem | cause | fix |
|---|---|---|
| `typedef redefinition ... 'uint64_t' vs 'unsigned long long'` | `t2asset.h` had `u64 = uint64_t` (unsigned long on LP64); LUS/ultra64 use `unsigned long long` | `t2asset.h` now uses `unsigned long long` (the game's `include/common.h` already does) |
| crash at exit in `~CrashHandler` (spdlog null) | bridge setters keep `shared_ptr`s in statics; with no `Ship::Bridge` component the Context does not clear them, so they die during static destruction | clear every bridge (`XxxSet...(nullptr)`) before destroying the Context |
| crash in `~Fast3dWindow` | the window's destructor calls the backend that only `Init()` creates | only construct the window when it will be initialized |
| `for (auto& a : *am->GetArchives())` crashes | `GetArchives()` returns a fresh `shared_ptr<vector>`; the temporary dies before the loop body | keep it in a local |
| black frame | see the fill/depth-image note above, and the read target (fb 0) | set colour + depth images; read fb 0 |

## 3. How the HarbourMasters ports structure the port layer

Studied: Ghostship `aefb89b` (SM64; LUS fork commit `37daaa6`, epoch 2), SpaghettiKart
`ce0f935` (MK64; LUS `60c9bb7`, port-maintenance line), Starship `3322643` (SF64; LUS
`eaaf9d0`, March 2026). None pins a LUS release tag; each pins a commit (Ghostship one not
even on Kenix3's branches). We pin a tag.

**Directory layout.** Game decomp tree kept as is (`src/`, `include/`), plus:
`libultraship/` (git submodule), `Torch/` (submodule, the extractor),
`src/port/` (C++: `Engine.cpp/.h` = the `GameEngine` singleton that builds the LUS
context, `Game.cpp` = `main()` and the frame function, `GBIMiddleware.cpp` = display-list
macro hooks for resource paths, `resource/` = their factories/importers, `ui/` ImGui
menus, `audio/`, `interpolation/` (frame interpolation for >30 fps), `pak.cpp`
(SpaghettiKart: Controller Pak on files), mods, extractor glue). Game C is edited in place
with `#ifdef`s or outright (they are not matching-build-preserving forks of the decomp;
we are, so our edits must stay `TARGET_PORT`-guarded or live in `port/`).

**libultra calls -> LUS.** LUS provides a libultra replacement surface
(`src/libultraship/libultra/*.cpp`, headers `include/libultraship/libultra/*.h`, one
umbrella `#include <libultraship/libultraship.h>`): `osContInit/StartReadData/GetReadData`,
`osMotorInit` + rumble (`__osMotorAccess`), `osEeprom*` (to a file), `osCreateMesgQueue`,
`osSendMesg`, `osRecvMesg`, `osJamMesg`, `osSetEventMesg`, `osGetTime/osGetCount/osSetTime/
osSetTimer`, `osPiStartDma/osEPiStartDma/osPiReadIo/osPiWriteIo/osCartRomInit/
osCreatePiManager` (DMA served from registered resources, `AudioDmaRegistry`), `osVi*`
(`SwapBuffer`, `SetMode`, `Black`, `GetCurrent/NextFramebuffer`, `SetEvent`, ...),
`osAiSetNextBuffer/osAiGetLength`, cache ops as no-ops, `osVirtualToPhysical`,
`osMapTLB` (no-op), plus `gu*` math and GBI. **Not provided**: threads
(`osCreateThread/osStartThread/osSetThreadPri/osYieldThread`), `osSpTask*` (the RSP),
`osPfs*` (Controller Pak; SpaghettiKart writes its own on files), libaudio (`al*`).
**LUS message queues never block**: `osRecvMesg(..., OS_MESG_BLOCK)` on an empty queue
returns -1 at once. That is the key fact for the thread model.

**Thread model.** All three flatten the game's threads into one host thread driven by the
LUS window loop. Starship: `main()` = `GameEngine::Create(); Main_SetVIMode();
Main_Initialize(); Main_ThreadEntry(NULL);` (with the `osRecvMesg` loop commented out),
then `while (WindowIsRunning()) push_frame();` where `push_frame` calls
`Graphics_ThreadUpdate()` (the body of the graphics thread's loop, one iteration),
`StartAudioFrame`/`EndAudioFrame` and timers. Ghostship/SpaghettiKart: `thread5_game_loop`
does the init, `thread5_iteration()` is one frame. `osCreateThread` calls are deleted or
`#if`'d out; scheduler/VI/SP/DP message loops are commented out.

**Display list submission.** Where the N64 code would `osSpTaskStart` the gfx task, the
port calls `GameEngine::ProcessGfxCommands(masterDL)` (Starship `Graphics_PushFrame`),
which computes interpolation replacements and calls
`Fast3dWindow::DrawAndRunGraphicsCommands(dl, mtxReplacements)` (once per interpolated
sub-frame). `HandleEvents()` is called first (input/resize). `osViSwapBuffer` is LUS's
no-op-ish replacement. `SetRendererUCode(ucode_f3dex2 / ucode_f3d ...)` selects the GBI
decoder; `GBI_UCODE` selects the macro encoding at compile time. Fast3D resolves segments
**only for addresses with bit 0 set** (`Interpreter::SegAddr`: `w1 & 1` -> segment table;
otherwise `w1` is a raw pointer). `Gfx` is two `uintptr_t` (16 bytes on 64-bit).

**Audio.** LUS's `Ship::Audio` component owns a PC backend (SDL, CoreAudio, WASAPI, null);
the game synthesizes. Starship runs the game's synthesis on a `std::thread` woken once per
frame (`StartAudioFrame`/`EndAudioFrame` with a condvar): it asks
`AudioPlayerBuffered()` vs `AudioPlayerGetDesiredBuffered()` to pick 560 or 528 samples (32 kHz),
calls the game's `AudioThread_CreateNextAudioBuffer(buf, n)` once per VI, and hands the PCM
to `AudioPlayerPlayFrame(buf, bytes)`. Ghostship creates `Ship::Audio(AudioSettings{
.SampleRate = 32000, .SampleLength = 512, .DesiredBuffered = 1100 })`.

**Assets.** Torch extracts the ROM into `<game>.o2r` with typed resources (textures, DLs,
vertices, audio) and the C references them by path strings (`"__OTR__..."`) through
`GBIMiddleware` and `LOAD_ASSET`. Our design keeps the engine's ROM-address keys
(`T2Asset`, PORT.md option C) instead; only the port archive (shaders, later fonts and UI)
is "typed".

## 4. Plan for Turok 2

Ordered; each step ends in something that runs.

1. **Link the game library** (p1's `turok2_game`) into `turok2` with LUS's headers in
   front of p1's shim where LUS provides the same thing (`libultra` types/prototypes,
   `gbi.h` with `F3DEX_GBI_2`). Exclude from the native link every game-side
   implementation of a function LUS replaces: the libultra io/os code decompiled in the n6
   segment (0x290000-0x2AFFFF: `osContInit`, `osCreateMesgQueue`, `osSendMesg/osRecvMesg`,
   `osJamMesg`, `osSetEventMesg`, `osPiStartDma`, `osCreatePiManager`,
   `osCreateViManager`, `osViSetMode/Event/SpecialFeatures/YScale`, `osVirtualToPhysical`,
   `osSetTimer`, `osGetTime`, `osGetCount`, `osAiSetNextBuffer`, `osMotorInit`,
   `osCartRomInit` ... all referenced by the game), else duplicate symbols. Keep the
   game's `osPfs*` only if we emulate the pak at the SI level; better: replace `osPfs*`
   with a file-backed implementation (SpaghettiKart `pak.cpp` is the model; Turok 2 calls
   `osPfsSelectBank` 43x, `osPfsRWInode` 26x, `osPfsReadWriteFile` 12x, ...).
2. **One host thread, frame function.** Turok 2 has eight `osCreateThread` sites:
   `CEngineApp__Idle` -> `mainproc` (main), `__scMain` (scheduler, `func_0028ADF0`),
   `__amMain` (audio manager), `CLoader__Main`, `CDecompressor__Main`, `CCache__Main`,
   `VM_ManagerThread` + `VM_ReaderThread` (pager). Following Starship: a port `main()`
   runs the boot/init part of `mainproc` up to its frame loop, then
   `while (WindowIsRunning()) T2_Frame();` where `T2_Frame` is one iteration of the
   engine's main loop. Because LUS queues never block, every `osRecvMesg(BLOCK)` in a
   frame path must either find its message already queued or be rewritten. The worker
   threads become synchronous calls: `CLoader`/`CDecompressor`/`CCache` requests are
   served immediately from `T2Asset` (no RNC on the host: ARCHIVE.md); the VM pager
   disappears (VM_WINDOW units stay resident). Details and the dependency order are p4's
   RUNTIME.md; LUS gives us nothing for threads, so a `port/src/os_thread.c` with
   `osCreateThread` recording the entry and `osStartThread` as a no-op (or a real
   `SDL_Thread` for audio only) is the minimal shim.
3. **Graphics.** Where the scheduler would start the gfx task (`osSpTaskStart` on the
   `OSTask` with `gspF3DEX_NoN_fifo`), call `GfxSubmit(task->t.data_ptr)` ->
   `DrawAndRunGraphicsCommands`. The L3DEX task (lines) is a gap: LUS defines
   `F3DEX2_G_LINE3D` (opcode 0x08) but the interpreter has no handler for it, so the
   game's line display lists need a port-side handler (draw each line as a thin quad) or
   a LUS patch; find which screens use L3DEX first. `osViSwapBuffer(gCFB[i])` stays LUS's. (Both changes below are done: docs/port/GBI.md.) Two code changes the asset side already
   flagged (PORT.md 1.6): `Model_Decompress` sizes output as 8 bytes per command (must be
   `sizeof(Gfx)` = 16), and it emits segmented addresses (segment 1 matrices, segment 2
   vertices) which **Fast3D resolves only with bit 0 set**: emit `seg<<24 | off | 1`
   under `TARGET_PORT` (vertex/matrix offsets are even), or resolve to real pointers at
   build time. The game's own `gSPSegment` calls then just work.
4. **Input.** Nothing to write: the game's `osContInit/StartReadData/GetReadData` calls
   reach LUS once the game's libultra copies are excluded. Rumble: LUS `osMotorInit` +
   `__osMotorAccess`.
5. **Audio.** libaudio runs on the CPU already (synthesis in C, the RSP only executes the
   command list): port `aspMain`'s audio microcode as a C interpreter of the `Acmd` list
   (ABI v1; the HM ports and Rage Wars' `audio_hle.c` did this), run the audio manager's
   frame on a thread woken per frame (Starship pattern) and feed `AudioPlayerPlayFrame`.
   `osAiSetNextBuffer` (LUS) is the alternative sink.
6. **Saves.** Controller Pak -> files (step 1). EEPROM is not used by Turok 2.
7. **Port archive.** `t2port.o2r` grows: fonts for the ImGui menus, `gamecontrollerdb.txt`
   (or ship it beside the exe), port UI. Torch `pack` can replace the CMake zip once Torch
   is in the tree.

## 5. Recommendations

- Keep the **tag pin** (`2.0.0.0`) and move to a git submodule `port/libultraship` when
  the port tree stabilizes (CI then needs no network for LUS itself; LUS's own
  FetchContent deps still do). Re-pin deliberately; epoch 2 changed the Context API
  completely (components, bridge caches), so 1.x-era port code (Starship) does not apply.
- Use the **manual component graph** (EPOCH-2.md option B, what `main.cpp` does); call the
  bridge setters and clear them at shutdown (or add a `Ship::Bridge` component so the
  Context does it).
- `T2Asset` over `ArchiveManager::LoadFile` is the asset path; do not route units through
  the ResourceManager (it would cache a second copy of every unit: the engine's `CCache`
  is the cache). Register the Blob factory anyway (cheap; mods/tools).
- Do not pass `validHashes`; check the game archive's version ourselves.
- Headless CI: `SDL_VIDEODRIVER=offscreen` + Mesa; `turok2 --frames N` exits 0/1.

## 6. Files

- `port/CMakeLists.txt`, `port/cmake/libultraship.cmake`, `port/app/CMakeLists.txt`
- `port/app/main.cpp` (context, archive checks, window/controller/Fast3D smoke test)
- `port/app/t2asset_lus.cpp/.h` (T2Asset backend on `ArchiveManager`)
- `port/app/testdl.c` (test display list)
- `port/assets/t2asset.h` (`u64` typedef fix)
