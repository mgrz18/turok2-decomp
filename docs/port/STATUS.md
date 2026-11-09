# Port status after Phase 7 round 3 (merged state)

Round 3 merged five branches, in this order: r2 (archive format 2.0, the loader through the
archive in host byte order), r1 (the boot runner to mode 0), r3 (the 64-bit typing sweep), r5
(macOS arm64) and r4 (audio behind `--audio`). Then it fixed what the merge broke and followed
the black frame to the text and the menu. This page says how to build and run the result, how
far `turok2 --game` gets, what stopped it on the way (one table for the whole round) and what
comes next. The part sections further down keep each branch's own notes. Details:
[DATA.md](DATA.md) (gendata), [64BIT.md](64BIT.md) (part 2: retype.py), [AUDIO.md](AUDIO.md),
[MACOS.md](MACOS.md), [RUNTIME.md](RUNTIME.md) (13: the typed scheduler),
[../assets/PORT.md](../assets/PORT.md) 10.4/10.6 and [../assets/ARCHIVE.md](../assets/ARCHIVE.md)
(`t2/swap`).

The matching build is unchanged. `tools/asm_diff.py --base 3798e01e -j 8` reports 0 differences
over all 3,938 files after every merge and fix. A clean `make clean && make build && make verify`
gives the identical ROM (CRCs good, 123 shared .rodata objects agree).

## 1. Build and run (Linux x86_64, the box)

```sh
# 0. the matching build first: gendata reads its ELF (build/turok2.us.elf)
make setup && make -j8 build && make verify

# 1. the archive, format 2.0 (t2/swap byte-swap plans); the port refuses 1.x archives
#    (sha1 99cbe76733bfab1d7235e4894cec2b91be3965f7 with zlib 1.3.1, any -j)
python tools/assets/mkarchive.py baserom.us.z64 -o OUT/turok2.o2r -j 4
python tools/assets/archive_check.py OUT/turok2.o2r baserom.us.z64     # OK, format 2.0

# 2. configure; build the game library AND the runtime/audio libraries: gendata reads the
#    undefined symbols of all three (t2audio_mgr.c reads gAudioFrameSize, gAudioGlobals, ...)
cmake -S port -B OUT/build -G Ninja -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ \
      -DCMAKE_BUILD_TYPE=Debug
ninja -C OUT/build -j8 turok2_game t2runtime t2audio

# 3. the game data as C, into port/gen/ (gitignored: ROM content); libclang 19 python bindings.
#    Drop the cache whenever a header changes (it holds the types): sizes and aliases follow them.
rm -f OUT/decls.json
python3 tools/port/gendata.py --build OUT/build --cache OUT/decls.json -j 4

# 4. re-run cmake so data.cmake sees port/gen/*.c, then the executable
cmake OUT/build && ninja -C OUT/build -j8 turok2

# 5. run headless and draw (SDL's offscreen driver: no window; --no-window would skip Fast3D)
SDL_VIDEODRIVER=offscreen T2_SHOT=OUT/frame.ppm gdb -batch -ex run -ex "bt 30" \
    --args OUT/build/app/turok2 --archive OUT/turok2.o2r --rom baserom.us.z64 \
    --game --draw --frames 300            # add --audio for the sound world (section 2)
```

`T2_SHOT=<file.ppm>` writes the last frame Fast3D drew (640x480). `T2_DLDUMP=1` prints the last
display list; it is read after the run, when the game may already be filling that buffer again,
so trust a breakpoint over it for single commands.

gendata on this tree: 1,545 data symbols that module-1 C and the runtime need (1,539 typed from
a C declaration), 393 zero .bss symbols (29 byte views doubled for host pointers), 3,794
pointer slots (2,770 relocated, 994 null in the ROM, 30 unresolved).

## 2. How far `turok2 --game` gets

```
t2: boot: arena 32 MB at 0x40200000 (low memory), ROM baserom.us.z64, mainproc started (pri 13)
game: booted, 300 frames
t2: Sound__Initialize: audio off (run with --audio)
t2: __osSiRawStartDma: PIF not modelled (no Controller Pak yet)
game: 300 frames in 2.78 s, 1414 thread switches, 151 gfx tasks (151 drawn), 0 audio tasks,
      0 PI DMAs, 0 missing-function calls
OK (0 failed checks)
```

Boot -> `CEngineApp__Main` -> the engine, camera pool, player pool, `CScene__Construct` and
`CScene__LoadLevel(0)` -> mode 17 -> **mode 0 (pak check)**, which draws every retrace through the
game's scheduler into Fast3D: 300 frames, 151 display lists drawn, no crash.

What the frames show now (they were black with a 4-pixel red bar at the end of r1):
- frames up to ~120: **"ACCESSING THE / CONTROLLER PAK"** in the game's small font, beige,
  centred at the right place on the 480x360 screen;
- from frame ~120, the timer opens the pak menu (`gPakSaveGameAnyPakMenuDesc`, then
  `gDamagedPakAtBootMenuDesc`, because no Controller Pak is modelled): the menu's **background
  image** (the reptile-scale texture, full screen, darker bands at top and bottom) and the
  **red highlight bar** of the selected line, centred. The menu lines themselves draw no
  glyphs yet (below).

With `--audio`: the sound bank is converted (2 bank files, 34 instruments, 1,115 sounds, 866
wave tables), the sound data file arrives in host order through the archive (the audio hook
sees that and leaves it alone), the audio manager starts. Mode 17's `Sound__StopAllExcept` now
works (the voice pool and request queue typed, rows I5/I6). It then stops in
`SndQueue_CancelHandle` from `CAudioWorld__StopHandle` <- `Sound__Stop` <- `LoopSounds_StopAll`
<- `CScene__LoadLevel` (mode 0's init): `CAudioWorld__StopHandle` reaches the queue through the
Rage Wars `ContextRecAt2BC0` view of `CAudioWorld` at N64 offsets.

macOS arm64 (r5, [MACOS.md](MACOS.md)): the whole port builds and the tests pass. The game runs
headless with full 64-bit pointers (a low-memory mode is impossible there). With r1+r3+r5 it gets
past the camera pool and stops in `Model_Decompress` -> `Material_Unpack` (section 4).

## 3. Crash and fix table (round 3, all parts)

Rows r1-* and r4-* were found on the branches; I-* during the merge (C18). Every fix is under
`TARGET_PORT` or in a port-only file, asm_diff 0.

| # | where | cause | fix |
|---|---|---|---|
| r1-1 | `CScene__GetRegionInfo` via `CCamera__AdvanceAttributes` | `CEngineApp`/`CGame` held the scene, camera pool, onscreen as N64-sized bytes | `structs/game.h` under TARGET_PORT: typed one-element-array members; literal CEngineApp/CGame offsets by `T2_OFFSETOF` |
| r1-2 | same | gendata translated offsets inside a union through its last member | a union prefers the member that types the offset |
| r1-3 | `CCamera__AdvanceAttributes` | `CCameraViewParams__Construct` wrote through a 4-byte-pointer view | host body |
| r1-4 | `Math_AcosLookup(NaN)` | libultra's `du` doubles generated by their words | gendata: a union's whole-size scalar member lays out the data |
| r1-5 | `CHeap__Free(NULL)` | `gPoolBuffers` generated at its N64 0x20 bytes | gendata doubles byte-only .bss; `CCache__AllocateAt` through `CMemEntry` |
| r1-6/7 | player / loader lists | link offsets 0x131C/0x1320, 0x18/0x1C | `T2_OFFSETOF`; host `PlayerList` |
| r1-8 | `CCache__DetachCacheEntry`, `__FreeCacheEntry` | m2c offsets | typed bodies |
| r1-9 | `CCache__LoadPersistantIndex` | `s32 cart` received a pointer | `sptr cart` |
| r1-10 | `Model_Decompress` | decompression blocks written at N64 offsets | r2's typed `CCache__AllocDecompressFlags` |
| r1-11 | `SwooshSystem_UpdateSwooshes` | swoosh/effect system as N64 bytes | **stub** (left empty on the host) |
| r1-12/13 | CScene views, `CGameObjectInstance__SetupAnimation` | N64 offsets, region stride | by field / typed |
| r1-14 | `__scMain(sc=NULL)` | scheduler views overran on LP64 | `port/game/t2sched.c` (17 functions typed, originals out of the link) |
| r1-15 | `SndVoicePool_*` from `Sound__StopAllExcept` | no sound bank | `Sound__*` return early while `gAudioWorld.bank` is NULL (they stay: they only fire without `--audio`) |
| r1-16/17 | delayed level events, actor lists | bytes / m2c offsets | typed |
| r1-18 | `CCameraPool__AdvanceText` | camera text/shake/emitter lists, sky system | links by `T2_OFFSETOF`, `CEmitterPool m_SkySystem[1]` |
| r1-19/20 | Fast3D `G_MOVEMEM` | `DlCmd` 32-bit words; static display lists with N64 addresses | `DlCmd` host words; gendata relocates w1 in static lists |
| r1-21 | `Menu_Construct` & co. | menu sizes, links, desc tables, desc text addresses | `MENU_SIZE`...; pointer-target tables; `t2_n64_to_host` |
| r4-A1..A5 | `alCSPNew`, heap, `alSndpAllocate`, `alEvtqPostEvent`, `CAudioWorld__Lock` | views at N64 offsets | typed (AUDIO.md) |
| r4-A6 | `_sndpVoiceHandler` spins | `player.evtq` overwritten | open on r4; not reached now (I6 stops first) |
| I1 | link: `gAudioFrameSize`, `gAudioGlobals`, `gAudioMaxRSPCmds` undefined | r4's audio manager files left the game library, so gendata no longer saw these names | gendata also reads `libt2runtime.a`/`libt2audio.a`'s undefined data names (build them before gendata) |
| I2 | merge r1+r3 does not compile | r3's `sptr` prototypes vs r1's `s32` port bodies | r3's prototypes kept, r1's bodies adapted (`CCache__AllocateAt`, `..ForDecompression[Top]`, `Sound__Play`/`SetListener` with the guard, `MenuPool_Construct`) |
| I3 | `VM_Alloc` (r2 vs r4) | r2 reads the unit through the archive in host order; r4 copied ROM bytes and swapped | r2's `T2Port_AssetWindow` + r4's `T2Audio_FixRegion` (detects host order) |
| I4 | pak menu lines measured 0 wide | the menu strings (`D_800B50C0` "damaged controller pak found in" ...) are address-only labels (SHN_ABS) in `.code_rodata`; gendata made every SHN_ABS symbol zero .bss | gendata reads SHN_ABS labels inside `.code_rodata`/`.code_rodata_tail` as data (`ABS_DATA_SECTIONS`) |
| I5 | `--audio`: `SndVoicePool_StopAllExcept` (mode 17) | Rage Wars views of the voice pool (0xCC-byte slots, 4-byte owner) | `SndVoicePool_Construct`, `SndVoicePool_StopAllExcept` typed on `SndVoicePool`/`SndVoice` |
| I6 | `--audio`: `SndQueue_CancelHandle` | the same for the request queue | `SndQueue_Construct` and `SndQueue_Cancel{AllExcept,Handle,SoundId,Owner,Flags}` typed on `SndQueue`; now stops in `CAudioWorld__StopHandle` (open) |
| I7 | no text, no font texture | `CScene__LoadGraphicSet` read the scene through an N64-offset view (0x1C, 0x48, 0x7C): the font graphic set was never found, `sOnScreenFontTexture` stayed NULL | typed on `CScene` (`m_GraphicSetsKey`, `m_pGraphicSets`, `m_pGraphicSetTypes`) |
| I8 | texture rectangles of size 0, black text | `COnScreen__DrawString`/`DrawText` read colours, scale and spacing through `FontStyle { u8 pad0[0xA0]; ... }` | pad by `T2_OFFSETOF(COnScreen, m_TopR)` |
| I9 | wrong glyphs ("!## 33)" for "access") | `COnScreen__Construct` built the character tables through a `Hud` view at N64 offsets (0xC8/0x148) | typed on `COnScreen` (`m_CharacterSmallLookUpPos`, +0x80) |
| I10 | everything drawn 1.5x too large, off to the right | Fast3D's native size is 320x240; the game draws at 480x360 | `game.cpp` sets the interpreter's native size from `gScreenWidth`/`gScreenHeight` before each frame |
| I11 | menu at the left edge, no background | `MenuPool_GetCamera` = pool - 0x53C (N64 `offsetof(CCamera, m_MenuPool)`): the menu's camera and viewport were 44 bytes into the camera | `pool - T2_OFFSETOF(CCamera, m_MenuPool)` (`sptr`) |

## 4. Next

1. **The pak menu's lines.** They are laid out and highlighted but draw no glyphs: the items of
   `gDamagedPakAtBootMenuDesc` reach `MenuItem_DrawString` with `text` pointing into the desc
   table itself (`D_0043BF18 + 0x38`, an empty string), while the first menu's items map their
   N64 text addresses correctly (`t2_n64_to_host`). Follow the item text of this desc (its update
   callbacks `DamagedPakMenu_UpdateLine1/2` and the `data` words of the .virtual desc table).
2. **Controller Pak / input**: `__osSiRawStartDma` is not modelled, so the game always sees a
   damaged pak and never gets past mode 0. Model the PIF (controllers, then a pak file) to reach
   the title screen.
3. **Audio**: `CAudioWorld__StopHandle` and the rest of the `CAudioWorld` API (about 40 functions:
   `SndVoicePool_*`, `SndVoice_*`, `SndQueue_RequeueVoice`) go through Rage Wars views of
   `CAudioWorld` at N64 offsets (`ContextRecAt2BC0`, `RecordCode8025A3EC`, ...). Type them on
   `CAudioWorld`/`SndVoicePool`/`SndQueue` as I5/I6 did; then r4's open `evtq` corruption.
4. **macOS** (full 64-bit, r5 found on r1+r3+r5): `Material_Unpack` (func_00207914) writes the
   texture handle into `Material.texture` (an `s32` inside the model block) and `texture + 4`:
   needs a host-typed `Material` or a side table. Then five more truncations listed in
   [MACOS.md](MACOS.md) (decompress callbacks with `s32` arguments, the
   `LoadPersistantSubBlock` callback argument, the CAI view in `DeallocateInternal`,
   `LoadBitSections` with an `s32` destination; r3's e9b1dfcc covers part of these: re-check).
5. The swoosh/effect system (CEngineApp 0x19608-0x22BA0, r1-11) typed instead of left empty.
6. 64-bit: 1,727 pointer-width warnings, **1,019 truncating** (`retype.py --dry --rules ""` on
   this tree; 2,828 before r3), 999 disagreeing declarations. Each crash above was one of them.

## Round 4, part s2: boot runner part 2 (mode 0 to the title screen)

Branch `names-R4_s2` (from 474e40af). Every commit has `tools/asm_diff.py --base 474e40af -j 8` at
0 differ or fail over the files it touches; the one include change (`structs/world.h`, under
`TARGET_PORT`) was checked over the whole tree (3,938 files, 0 differ).

### How far it gets

```
SDL_VIDEODRIVER=offscreen T2_SHOT=out.ppm T2_SHOT_EVERY=300 build/app/turok2 --archive turok2.o2r \
    --rom baserom.us.z64 --game --draw --frames 5000 --skip-pak-check
game: frame 2: engine mode 17 -> 0          pak check: "ACCESSING THE CONTROLLER PAK"
game: frame 16: engine mode 17 -> 1         legal screen, the full text
game: frame 788: engine mode 17 -> 3        "EXPANSION PAK FOUND!" with the 3D pak model
game: frame 1080: engine mode 17 -> 4       intro cinema 100 ("acclaimlogo", level 162): runs 486 frames, draws black
game: frame 1570: engine mode 17 -> 5       intro cinema 90 (level 44): runs to its end, draws black
game: frame 3932: engine mode 17 -> 7       TITLE SCREEN (logo, "SEEDS OF EVIL", the scale background)
game: 5000 frames in 32.31 s, 1911 gfx tasks (1911 drawn), 0 missing-function calls
```

The whole boot sequence of the N64 game now runs on its own, timers and cinemas included, from
the pak check to the title. `--start-mode 7` reaches the title at frame 3 for quick checks.
With s1's PIF/pak the debug switch is not needed for mode 0 (s1 reported mode 0 -> 1 -> 3 -> 4
and the crash in mode 4's `CCameraPool__AllocCamera`, fixed here: row s2-3).

After the title's timeout (frame 5183) the game starts the attract demo (mode 13, level 81) and
stops in an enemy's mode setup:
```
#0 CAI__SetMode (arg2=0)  func_00243414.c:203   entry->unk_04 (the mode's enter function) is garbage
#1 EnemyAI_InitDispatch                          func_0025BE88.c:52   the enemy's mode table, set through N64 views
#2 func_0021E5D4 <- CScene__LoadLevel(81) <- CScene__UpdateWarp <- EngineMode13_Game_Draw
```
That is the start of "levels run" (the enemy AI, s5's sweep: ~400 offset sites).

Screenshots (`T2_SHOT_EVERY`): the damaged-pak menu with its lines ("PROBLEM!", "DAMAGED
CONTROLLER PAK FOUND IN CONTROLLER #1.", "ATTEMPT REPAIR" highlighted, "RETRY", "CONTINUE WITHOUT
SAVING"), the legal text, the Expansion Pak screen with the rotating pak model, the title. The two
cinemas draw black frames: their scene draws no geometry yet (no `G_VTX` in the frame's list).

### Debug switches (DEBUG ONLY, port/game/t2debug.c)

| switch | effect |
|---|---|
| `--skip-pak-check` | once mode 0 has started, sets `gPakCheckDone` ("continue without saving"): `EngineMode00_PakCheck_Retrace` takes its own exit to mode 1 |
| `--start-mode N` | once mode 0 has started, `CEngineApp__SetMode(&gEngineApp, N)` (7: title) |
| `T2_SHOT_EVERY=n` | with `T2_SHOT=<f.ppm>`: also `<f.ppm>.<frame>.ppm`, the first drawn frame of every n |

Every run logs the engine mode changes (`game: frame F: engine mode A -> B`). The two switches go
away once s1's pak model is merged (keep them until the demo path runs, they save 1000 frames).

### Crash and fix table (round 4, s2)

| # | where | cause | fix |
|---|---|---|---|
| s2-1 | pak menu lines draw nothing | `MenuItem_DrawText` read the item as a `Menu` and passed `->desc` (N64 0x14 = `MenuItem.text`; host: `MenuItem.def`, the desc row, an empty string) | typed on `MenuItem`, passes `text` |
| s2-2 | mode 0 never ends | no PIF/pak (s1) | debug switches above |
| s2-3 | `CCameraViewParams__Construct(0x31)` from `CCameraPool__AllocCamera` (mode 4 `CPlayer__NewGame`) | pool view with the free/active lists at N64 0xC/0x20 | `CCameraPool__AllocCamera(_2)` typed on `CCameraPool` |
| s2-4 | `MenuPool_CountOpen` from `Automap_Update` | camera menu pool as `m_pCamera + 0x53C` (23 files; three through raw player offsets 0x51C/0xA50) | `T2_OFFSETOF(CCamera, m_MenuPool)`, `player->m_pCamera`, `m_ContState.m_pContState`; `MenuPool_GetCamera_2` as I11 |
| s2-5 | `func_00267DBC`, `func_0021E5D4` (actor init), `func_0021F1B4`, `CScene__LoadLevel` | N64 views: intelligence at 0x14, AI at 0x140, flags 0xD4, 0x288/0x1328 strides, player link 0x1320 | typed / host body / `sizeof`, `T2_OFFSETOF` |
| s2-6 | `InteractiveAnim_Advance` | AI vars at 0x10C | typed on `CAI.m_VarS32/F32` |
| s2-7 | `CScene__BuildActorLists`: `m_pActors` = 0x190_402B4900 | `CScene.m_Heap` is `u8[0x10]` but holds a pool of pointers (24 host bytes): `MtxPool_Reset` wrote over `m_pActors` | `MtxPool` (structs/world.h), `CScene.m_Heap` typed under TARGET_PORT; `MatrixHeap_Construct`, `MtxPool_Reset`, `MtxPool_Alloc` typed |
| s2-8 | `CCache__Deallocate(0x50)` from the cinema spline | `TempAlloc` kept the handle as an s32, `TempFree` read `((s32 **)data)[-4]` (32 bytes back on the host) | the whole pointer, 0x10 bytes back (TARGET_PORT; both cinema copies) |
| s2-9 | archive abort: grid section `0x01F748E4` not a unit | `CScene__DecompressPaths` wrote the paths through a view at 0x58/0x68 (N64 `m_pRegionData`/`unk_68`): on the host that was `m_pCollision` | typed on `CScene` |
| s2-10 | Fast3D `G_VTX` with w1 `0x00800080020000D7` | the swoosh display list `D_800ACFC8` declared `u8[]`: gendata emitted ROM bytes | declared `Gfx[]` (gendata relocates it) |
| s2-11 | Fast3D runs matrices as commands (`G_DL` to a freed model) | `CCache__LockForDisplayList`, `Begin/EndDisplayList` and the cache thread `CCache__Main` used `D_800D7BE0` (gCache - 0x5F0) through `B12Mgr`/`B12Node`/`B12Msg` N64 views: the lock bit went into `CMemEntry.lockCount`; `CCache__Main` waited on a queue never created on the host (async loads and display-list releases never completed) | the four typed on `gCache`/`CMemEntry`/`CCacheEntry` (TARGET_PORT bodies) |
| s2-12 | Fast3D runs stale commands (the frame's list 8 bytes off) | `CCamera__DrawGlares` wrote one command as two u32 and stepped 8 bytes | one host `Gfx` (TARGET_PORT) |
| s2-13 | intro cinema never ends (`clipStart` -4761, 9007 cues) | `Pack_Add` called the memcpy `func_0029E460(data)`: m2c dropped the source and size (still in a1/a2 on the N64): every cinema resource block was left uninitialised on the host | `func_0029E460(data, prepare, size)` (matches); `proutSprintf` the same; `prepare` is `sptr` |
| s2-14 | `CAI__Update` callback | Rage Wars Source/Dest views | typed on `CGameObjectInstance`/`CAI` (matches) |
| s2-15 | `CAI__SetMode` | the same, plus the mode-table chain | host body on `CAI`/`CAIModeInfo` |
| s2-16 | `LevelEvent_Fire` and 11 other files | camera pool at `&multiplayer - 0x1201/0x1221`, actor stride 0x288, AI at +0x140, weapon actor +0x288/+0x3C8 | by name / `sizeof` / `T2_OFFSETOF` |

### For the merge

- Files others may touch: `port/app/{game.cpp,main.cpp,game.h}` (s1 adds flags there too: keep
  both sets of options; the mode log and `WriteShot` are mine), `include/structs/world.h`
  (`MtxPool`, `m_Heap`), `src/us/code/func_00413230.c` (menus: `MenuItem_DrawText` only),
  `func_0044C230.c` (`MenuPool_GetCamera_2`), the cache files `func_002041E4.c`,
  `func_0020548C.c`, `func_00205574.c`, `func_0020565C.c`, cinema `func_0040F240.c`/
  `func_00448240.c`, `func_00218590.c` (`Pack_Add`), the AI files `func_00243414.c`,
  `func_00245848.c`, `func_00266794.c`, and the 23 menu-pool files of s2-4 (s5's offset sweep may
  touch the same lines: they compute the same offsets).
- Rerun gendata with a fresh cache (`rm decls.json`): `world.h` and `D_800ACFC8`'s type changed.
- A fresh `turok2.cfg.json` makes LUS open 1024x768 and the game then draws into the lower-left
  640x480 of it (seen once with a new output directory): copy a 640x480 config or fix the
  default; screenshots assume 640x480.
- macOS (s3): s2-7, s2-8 and s2-11 are host-layout fixes that matter there too; several s32
  pointer arguments remain on this path (`CEngineApp__StartWarp(s32 app)`, `Action_EventGo`,
  `CHeap__Free(s32)` in `CCache__Main`, the cinema path items' `ref`).

### Next

1. The attract demo (mode 13): `EnemyAI_InitDispatch` and the enemy mode tables (N64 views of
   `CAI`), then the level renderer for the demo and the cinemas (they draw no geometry: follow
   `CScene__Draw`/the actor draw on level 162, whose acclaim-logo actor should show).
2. Merge s1 (PIF/pak) and drop `--skip-pak-check` from the default runs.
3. The rest of the Rage Wars views on the AI path (`AI_UpdateRegionFlags`, `CAI__UpdateModeAnim`,
   `EnemyAI_*`): s5's sweep, by the crash order above.

## Round 2 (history)


### How far `turok2 --game` got after round 2

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

### Crash log (round 2)

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

### What round 3 was given to do

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

### Where it was at the end of r1

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

## r5 (round 3): macOS arm64

Details: [MACOS.md](MACOS.md). On the Mac (M5, Apple clang 21, Homebrew; no new packages) the
whole port builds natively: `turok2_game` (3,917 files, 0 errors), gendata (identical output
to the box), `turok2` on LUS 2.0. Runtime and asset tests give the same hashes as Linux.
`turok2 --game` (headless) stops in `GfxPool_Alloc` (func_00425220.c:86) on a truncated `s32`
address. Linux `--high-memory` stops at the same line, and macOS cannot have a low-memory mode
(checked). A probe branch (`r5-probe`, not for merge) fixes five truncations and reaches
`CCameraPool__Construct` → `CCamera__ConstructSkySystem` (`arg0 + 0x558`), the CCamera layout
work. It also found a host-layout bug that affects Linux too: `CCache__AllocateAt` writes a
`CMemEntry` through junk_types' `B12Node` view, so `flags |= 3` lands in `lockCount`.

## Round 3, part r4: audio on the host

Details: [AUDIO.md](AUDIO.md). Behind `--audio` (off by default: runs without it are unchanged).

- Sound bank (ROM 0x1E0000) rebuilt as host libaudio structs (2 bank files, 34 instruments,
  1,115 sounds, 866 wave tables), sound data file swapped to host order (or left alone when it
  already is: r2's archive path), host audio manager (one 368-sample frame per retrace), an ABI1
  interpreter for all 16 commands, AI queue -> LUS `AudioPlayerPlayFrame` (game.cpp).
- `t2audio_test --direct K` + `tools/port/audio_compare.py`: a sound effect through
  LOADADPCM/LOADBUFF/ADPCM/RESAMPLE/ENVMIXER/INTERLEAVE/SAVEBUFF equals audio.py's VADPCM decode
  with **0** difference (sfx 0: 6,704 samples, sfx 40: 1,088).
- Game path: `Sound__Initialize` (`CAudioWorld__Initialize`) completes on the host; the first
  audio frame spins in `_sndpVoiceHandler` because `gAudioWorld.player.evtq` is corrupted
  (open, AUDIO.md §4). `turok2 --game --audio` stops at the same `CCameraPool__Construct` crash
  as without `--audio`.

| # | where | cause | fix |
|---|---|---|---|
| A1 | `alCSPNew` from `SndSeq_Construct` | `AudioStateD844`/`AudioRootD844` views: heap at N64 offset 0x1DA8 | `SndSeq_Construct` typed with `SndSeq`/`CAudioWorld` (`TARGET_PORT`), `SndSeq.csp` as `sptr`, `alCSPNew` sizes by `sizeof` |
| A2 | (heap) | `alHeapInit`/`alHeapDBAlloc` 32-bit views of `ALHeap` | host layout, `uptr` (`TARGET_PORT`) |
| A3 | `alSndpAllocate` | `PoolRecAt44`/`SlotRecAt2F` views | the 10 sound player API calls typed in port/audio/t2audio_sndp.c (files out of the link) |
| A4 | `alEvtqPostEvent` | `alEvtqNew`/`alEvtqNextEvent` 0x1C strides, s32 links | typed (`TARGET_PORT`) |
| A5 | `osRecvMesg` from `CAudioWorld__Lock` | lock at `arg0 + 0x110` | `&arg0->lock` (matches) |
| A6 | `_sndpVoiceHandler` spins | `gAudioWorld.player.evtq` overwritten with stack-like data | **open** (AUDIO.md §4) |
