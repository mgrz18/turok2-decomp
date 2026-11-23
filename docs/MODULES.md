# Modules: the original object files

The game was built from a few hundred source files (Acclaim's `.CPP`/`.C`,
libultra's, libgcc's), linked in one order. This page recovers those
boundaries from the ROM, gives each run of functions a module name, and the
source tree follows it (round 23): every C file sits in its module's
directory, still one function per file (`func_<vram>.c`) except in the few
modules merged into one file (below).

| Directory | What |
|---|---|
| `src/us/code/<module>/` | `.code`: the engine, the libultra/libaudio/libc modules ultralib does not build (`code/libultra/io/pfsisplug/` ...), libgcc (`code/libgcc/_divdi3/`) |
| `src/us/vm1/<module>/` | the paged VM module |
| `src/us/vm2/<module>/` | its stale second copy, under the first copy's module names (`vm2/Primagen/` holds `Primagen_2` .. ) |
| `src/port_libultra/<file>/` | the 88 game files of libultra modules the N64 build takes from `lib/ultralib` (docs/LIBULTRA.md): only the port compiles them (`audio/csplayer/`, `vm1/io/pfsinitpak/` ...) |
| `src/us/libultra/`, `src/us/boot.c`, `src/us/asm/` | unchanged: 11 libultra files kept in their own directory (`__osDisableInt.c` ...), the boot code, the hand-written asm |

`versions/modules.us.txt` gives each module's directory (`// dir`), and
`tools/srcfiles.py` finds a file by name (`python3 tools/srcfiles.py
func_0027ECF0`; file names are unique across the tree, so tools that take
a file name or a stem still work). The yaml names each file by its path
(`[0x1100, c, code/CList/func_00200500]`), and the objects follow
(`build/src/us/code/CList/func_00200500.c.o`). `tools/asm_diff.py --base`
compares a moved file with the base file of the same name.

```
tools/modules.py            # check the map, print a summary (and the .rodata order check)
tools/modules.py --write    # write versions/modules.us.txt (module -> its functions, with the evidence)
tools/modules.py --md       # print the table below
```

It reads `build/ref.elf` (functions, sizes, ROM offsets), the ROM,
`versions/ultralib.us.txt` (tools/ultralib_version.py --map,
[LIBULTRA.md](LIBULTRA.md)) and, when there is one, `build/turok2.us.map`.

## Merged modules (round 23, a proof of concept)

The matching build compiles each file as its own object, so a module can be
one file only where the merged translation unit compiles to the same bytes.
`tools/merge_module.py src/us/code/<module>` writes `<module>.c` from the
files in `.text` order (each file's declarations before its functions, a
declaration an earlier file gave dropped, a prototype of a function the
module defines replaced by the definition's signature, a `static` helper
repeated identically kept once) and points the yaml's first `c` and `.rodata`
subsegments at it; `make setup`, a clean build and `make verify` decide.
It refuses a module whose `.rodata` subsegments are not one block in `.text`
order (a literal another object owns, or the files' pools in another order).

**Merged, byte-exact (14 modules, 72 files -> 14):** `CList` (13 files),
`SoundEffectInfo` (14), `CHeap` (7), `CSun` (7), `CLoader` (6),
`CDecompressor` (5), `CHashTable` (5), `CPathTrack` (3), `MtxPool`,
`HalfToFloat`, `Level`, `RandomInPackedRange`, `ProPackDecompress`,
`Collision` (2 each). Two of them own `.rodata` (`RandomInPackedRange`,
`Collision`: one slice each). `make verify` OK, shared
.rodata 123 objects OK, `tools/asm_diff.py --base bab902cd`: 0 differ (the 14
merged files are new to it: it compares files, and a merged file has no
base file).

**Tried, not merged without hand work:**

| Module | Why it does not compile merged |
|---|---|
| `Sort`, `Color` | a file brings its own `typedef s32/u32/f32` (a file without `common.h`): a redefinition next to `common.h`'s; `Sort_Insertion`'s hoisted prototype also names `t2_uptr`, which its own file defines later |
| `DLights` | two different `ListNode` views |
| `GameObjectMove` | two files declare `Collision_MoveInstance` with different types |
| `CCollisionSetup` | the prototype the tool hoists for `CCollisionSetup__Construct` names `EntryRecAt24`, a view its own file declares later |
| `Boot` | two declarations of `gSavedPlayers` with different types |
| `Demo` | `Demo_GetPlayerCount` is a real function *and* a `static __inline__` copy in two callers: the original was one inline definition the compiler both inlined and emitted; the merged file needs it once, before its first caller |
| `CLevelActor` | its `.rodata` subsegments are not in `.text` order (refused before compiling) |

Round 24's `merge_module.py` merges `Sort`, `Color`, `Demo` and
`CCollisionSetup` mechanically (static renames, typedef dedupe, deferred
prototypes: next section); `DLights`, `GameObjectMove`, `Boot` still need
header work and `CLevelActor` its `.rodata` order. Round 23 did not try the
other modules (budget). What a merge needs is what the
table shows: one declaration per name across the module's files (the
`include/structs`/`include/globals` work of Phase 5 removes most of these),
and `.rodata` owned in `.text` order. Every function of a merged module is
still found by name: `tools/srcfiles.py` falls back to the module's file.

## Merging the other modules: the dry runs (round 24, part m3)

Round 24 edited no source file for this: it ran the merge of every module
directory that still holds one function per file (101 game directories and
18 of the game's libultra modules) in dry-run, compiled each merged file, and
classified what stops it. The table below is the plan for round 25, to be
re-run after the round-24 header work (m1: local structs, m2: BSS members)
lands, since that work removes many of the declaration conflicts it lists.

```
python3 tools/merge_check.py                 # every directory with 2+ C files: OK / COMPILE / DIFF / REFUSED
python3 tools/merge_check.py code/Sort       # one directory; --json for the compiler's messages
python3 tools/merge_module.py src/us/code/Sort   # the real merge (yaml, git rm/add), then:
make setup && make -j6 build && make verify
```

`tools/merge_check.py` runs `merge_module.py --dry-run`, then cpp and the
game's cc1 on the merged file; when it compiles, it compares every function's
assembly (labels renumbered) and the `.rodata` data with the
one-file-per-function compile. It needs no build tree. OK there is the
compiler's verdict; `make verify` after the real merge is the arbiter.

**`tools/merge_module.py` now does, mechanically** (`--no-fix` gives round 23's
behaviour):

- a `static` defined in more than one file is a separate object in each
  original object file: the later files' copies are renamed `<name>_<vram>`
  (the name the readable files already use for such helpers); identical
  `static __inline__` helpers stay once; a static copy of a function the module
  also defines (`Demo_GetPlayerCount`) is renamed in every caller file;
- a file that `typedef`s `s32` etc. itself (no `common.h`): the module's
  includes go first and a local typedef that a header gives the same way is
  dropped (only when there is one: moving includes up can turn a file's
  block-scope `extern` into a conflict, `reverb`);
- the definition's signature that replaces a prototype waits until the types it
  names are declared (a later file's typedef, or a later file's header), or
  pulls those declarations up to its first caller (`Sort`, `CCollisionSetup`);
- `vm2/<Module>` merges into `<Module>_2.c` (file names are unique across
  `src/`; vm2's functions carry the same suffix);
- a module with one file in `versions/shared_rodata.us.txt` hands the entry to
  the merged file; two or more refuse (one object has one `.rodata` section,
  placed at one address);
- it sees every yaml subsegment, also splat's unnamed ones (`- [0xAB194,
  rodata]`): a zero pad shorter than 16 bytes between two of the module's
  `.rodata` slices goes with the merge, anything else refuses. Before, such a
  line stayed and splat stretched it over the dropped slices.

**Verified:** the 28 directories `merge_check.py` passes, merged together in a
scratch copy (17 game, 11 libultra; 104 files -> 28): `make setup`, build,
`make verify` OK, shared `.rodata` 123 objects OK. They are the first batch for
round 25 (`merges` in the table). Not committed here: round 24 does not edit
`src/`.

**What stops the rest** (119 directories; the names are what cc1 reports on
the first pass, so fixing them can show more):

| Blocker | Game | libultra | What it takes |
|---|---|---|---|
| merges (cc1 identical, `make verify` OK together) | 17 | 11 | run `merge_module.py` |
| declarations differ, local types differ, calls typed differently | 42 | 4 | header work: one declaration per name across the module (m1/m2's kind of work), then re-check |
| `.rodata` interrupted by data no C file owns (an unnamed `rodata` subsegment that is not a zero pad) | 15 | 0 | give that data to its C file (a literal splat could not attribute), then re-check |
| `.rodata` interleaved with another directory's: `code/CPlayer` with `vm1/CPlayer`, `code/CScene` with `vm1/SceneLoad`, `code/CCamera` with `vm1/Demo` (both ways) | 6 | 0 | one original object spans the engine segment and the VM: a merged file would need its VM functions in their own section in one object; build work, not header work |
| `.rodata` slices not in `.text` order (adjacent, some pairs inverted) | 5 | 1 | the order the original compiler emitted them in is not understood yet; refused until it is |
| vm2: several files place their `.rodata` over vm1's pool one by one (`versions/shared_rodata.us.txt`) | 5 | 0 | `shared_rodata.py` would need one placement per merged object; possible only where the vm1 data is one block in the same order |
| vm2: functions of the module still in asm between its C files (`us/asm/18B9E0.s` = `TotemDefense_DrawTotem_2` ...) | 9 | 0 | those functions in C first |
| the files' `cc1-flags` differ (`HostIo`, `adecHuffDecode`, `gu/perspective`, `gu/lookat`) | 2 | 2 | cannot be one translation unit with these flags; two files per module, or the flags proven equal |

None of these needs a ROM change as far as checked. The `.rodata` order and
the unowned-data cases may instead mean that the module boundary is wrong
there (two original objects, not one).

## Merged in round 24

On the tree with m1's and m2's header work, `tools/merge_check.py` passed the same 28
directories the dry run listed; they are merged (two commits: 17 game, 11 libultra). Then
the header work for the closest of the 46 "declarations differ" directories: one declaration
per name across the module (the definition's signature, `protos/list.h`, `globals/save.h`,
`structs/rumble.h`, casts at the call sites; clashing local type names renamed; the two Rage
Wars `Door_OpenNow`/`Door_CloseNow` files on the module's types; `Turret_EventShow` given the
event handler's three arguments), `asm_diff` 0, and 16 more directories merged:
`code/{Action,Boot,DLights,Door,EnemyAnim,GameObjectMove,InteractiveAnim,LoopSounds,Platform,
SavedPlayers,Sweep,Turret,libultra/io/sptask}`, `vm1/{Cont,CheatMenu}`, `vm2/Cont`.

**Round 24: 44 module directories merged (370 files -> 44)**; with round 23's 14, 58 modules
are one C file. `make verify` OK, shared `.rodata` 123 objects OK, `asm_diff --base d2d07205`
0 differ (merged files are new to it), the port builds and plays the same (demos `6ab97584`).

**The port and a merged module.** `port/cmake/link.cmake` keeps game files out of the link by
file name where `port/` has a host version. A merged module is one object: where only some of
its functions are replaced, they are under `#ifndef TARGET_PORT` in the merged file
(`alAdecPull` in `alAdpcmPull.c`, `_removeEvents`/`alSaveParam`/`alResampleParam` in
`sndplayer.c`/`save.c`/`resample.c`, `LoopSounds_Construct` .. `LoopSounds_StopAll` in
`LoopSounds.c`); where all are, the merged file is listed (`vimgr.c`, `sptask.c`). The linker
reports a missed one (a duplicate definition), because the merged object is pulled in for
its other functions.

**Left: 75 directories** (`python3 tools/merge_check.py`, `COMPILE 30 REFUSED 45`):

| Blocker | Directories | What it takes |
|---|---:|---|
| declarations differ, local types differ | 30 | more header work as above. What remains is harder: field symbols read as arrays (`gEngineApp_m_Settings_match_tag[]`, `gEngineApp_m_Settings_unk1C8[]`, `gThrowableCollisionInfo_m_InstanceBehavior[]`, `__osContNumControllers[]`: the array view is what keeps the ROM's code, `(&x)[0]` does not, tried on `vm1/StatsMenu`), return types the callers' code depends on (`s32` vs `void *` on the host), Rage Wars `Triple` vs `CVector3` by value, the CList/CISet/EnemyAI prototype families spread over many files |
| `.rodata` not one block in `.text` order (data no C file owns, slices interleaved with another directory's or out of order) | 25 | as in the table: attribute the unowned data, or a module boundary that is not one object |
| vm2: functions still in asm between its C files (`.text` not adjacent) | 9 | those functions in C first |
| vm2: files placing `.rodata` over vm1's pool one by one | 5 | one placement per merged object in `shared_rodata.py` |
| the files' `cc1-flags` differ | 4 | two files per module, or the flags proven equal |
| a merged file of the same name exists (`code/CEngineApp` / `vm1/CEngineApp`, `vm1/Demo` / `code/Demo`) | 2 | `--name`; both are refused for `.rodata` order anyway |

### The dry-run table (round 24; merged rows updated after the merges)

| Directory | Files | Result of the dry run (round 24), or merged |
|---|---|---|
| `code/libultra/audio/cseq` | 12 | declarations differ (1: `__readVarLen`); local types differ (4: `DecodeResult`, `RuntimeState`, `sptr`, `uptr`) |
| `code/libultra/audio/drvrnew` | 8 | declarations differ (3: `FuncPtr`, `alFilterNew`, `alHeapDBAlloc`) |
| `code/libultra/audio/env` | 7 | **merged** (round 24, first batch) |
| `code/libultra/audio/resample` | 2 | **merged** (round 24, first batch) |
| `code/libultra/audio/reverb` | 8 | **merged** (round 24, first batch) |
| `code/libultra/audio/save` | 2 | **merged** (round 24, first batch) |
| `code/libultra/audio/seq` | 12 | **merged** (round 24, first batch) |
| `code/libultra/audio/sndplayer` | 5 | **merged** (round 24, first batch) |
| `code/libultra/audio/synthesizer` | 9 | .rodata not in .text order (1 inverted pairs of 3 subsegments) |
| `code/libultra/gu/lookat` | 2 | the files do not share their cc1 flags |
| `code/libultra/gu/perspective` | 2 | the files do not share their cc1 flags |
| `code/libultra/io/controller` | 3 | declarations differ (2: `__osContBuffer`, `__osContNumControllers`) |
| `code/libultra/io/piacs` | 3 | **merged** (round 24, first batch) |
| `code/libultra/io/siacs` | 3 | **merged** (round 24, first batch) |
| `code/libultra/io/sptask` | 3 | **merged** (round 24, after unifying its declarations; was: declarations differ (2: `__osSpSetStatus`, `osSpTaskStartGo`)) |
| `code/libultra/io/vimgr` | 2 | **merged** (round 24, first batch) |
| `code/libultra/libc/xprintf` | 2 | **merged** (round 24, first batch) |
| `code/libultra/os/timerintr` | 4 | **merged** (round 24, first batch) |
| `code/AI` | 48 | .rodata interrupted by data no C file owns (1: `unnamed@0xA7478`) |
| `code/Action` | 17 | **merged** (round 24, after unifying its declarations; was: declarations differ (1: `Action_Draw`)) |
| `code/Anim` | 49 | .rodata interrupted by data no C file owns (1: `unnamed@0xA64F4`) |
| `code/BinaryRange` | 13 | **merged** (round 24, first batch) |
| `code/Boot` | 4 | **merged** (round 24, after unifying its declarations; was: declarations differ (1: `gSavedPlayers`)) |
| `code/CAudioWorld` | 101 | .rodata interrupted by data no C file owns (3: `unnamed@0xA8C94`) |
| `code/CCache` | 56 | calls typed differently (2: `CCache__AllocateInternal`, `CCache__WaitForDisplayLists`); declarations differ (18: `CCache__DeallocateInternal`, `CCache__DetachCacheEntry`, `CCache__FreeCacheEntry`, `CCache__FreeMemEntry`, ...); local types differ (3: `B12Bucket`, `B12Mgr`, `B12Node`); other (1: `request for member `m_pHead' in something not a structure or`) |
| `code/CCamera` | 66 | .rodata interleaved with another directory's (2 subsegments between, e.g. `func_0042F9B0`) |
| `code/CCollisionSetup` | 3 | **merged** (round 24, first batch) |
| `code/CEmitter` | 10 | declarations differ (4: `CList__AddHead`, `CList__Remove`, `Emitter_CompareHeightAsc`, `Sortable`) |
| `code/CEngineApp` | 90 | .rodata interrupted by data no C file owns (1: `unnamed@0xAA9FC`) |
| `code/CGameObjectInstance` | 64 | .rodata interrupted by data no C file owns (2: `unnamed@0xA66C4`) |
| `code/CGameRegion` | 53 | calls typed differently (1: `incompatible types in initialization`); declarations differ (24: `CGameRegion__FloodClearFlag4`, `CGameRegion__FloodSetFlag4`, `CGameRegion__FloodSetHeight`, `CGameRegion__GetCeilingHeight`, ...); local types differ (2: `CollisionBuildVertex`, `Node75`); parse errors (6: `&`, `.`, `if`, `return`, ...); other (4: `initializer element is not constant`, `request for member `x' in something not a structure or union`, `request for member `y' in something not a structure or union`, `request for member `z' in something not a structure or union`) |
| `code/CISet` | 14 | declarations differ (3: `CUSet__GetTotalSize`, `CUSet__Transpose`, `osCreateMesgQueue`); local types differ (2: `CMutex`, `OffsetTable`) |
| `code/CInstancePool` | 18 | .rodata interrupted by data no C file owns (1: `unnamed@0xA6C6C`) |
| `code/CLevelActor` | 9 | .rodata not in .text order (2 inverted pairs of 5 subsegments) |
| `code/CMtxF` | 124 | .rodata interrupted by data no C file owns (3: `unnamed@0xA6004`) |
| `code/CParticle` | 212 | .rodata interrupted by data no C file owns (8: `unnamed@0xA7004`) |
| `code/CPlayer` | 245 | .rodata interleaved with another directory's (7 subsegments between, e.g. `func_0042DA60`) |
| `code/CScene` | 96 | .rodata interleaved with another directory's (7 subsegments between, e.g. `func_0042AA80`) |
| `code/Cinema` | 34 | declarations differ (5: `Sound__StopAllExcept`, `gScene`, `gScene_m_LevelID`, `gScene_m_WarpSpawn`, ...) |
| `code/Color` | 3 | **merged** (round 24, first batch) |
| `code/Controller` | 37 | declarations differ (3: `Controller_IsPakWatched`, `RumblePool_Init`, `osMotorStop`) |
| `code/Crash` | 16 | .rodata interrupted by data no C file owns (1: `unnamed@0xAACEC`) |
| `code/DLights` | 6 | **merged** (round 24, after unifying its declarations; was: local types differ (1: `ListNode`)) |
| `code/Demo` | 3 | **merged** (round 24, first batch) |
| `code/Destructible` | 15 | declarations differ (3: `AI_DropPickupsOnce`, `Actor_SpawnParticle`, `Sound__Play`) |
| `code/Door` | 16 | **merged** (round 24, after unifying its declarations; was: declarations differ (1: `ImpactEvent_Call`); local types differ (1: `ResourceManagerState`)) |
| `code/EnemyAI` | 119 | own basic typedefs (3: `s32`, `s8`, `u16`); calls typed differently (1: `EnemyAI_CheckOffscreen`); declarations differ (50: `AI_AngleToPoint`, `AI_IsAimedAt`, `AI_StopSound`, `AI_TurnClamped`, ...); local types differ (1: `Motion`); parse errors (1: `*`) |
| `code/EnemyAnim` | 76 | **merged** (round 24, after unifying its declarations; was: declarations differ (1: `WeightedTable_Clear`)) |
| `code/EnemyHover` | 9 | declarations differ (6: `AI_IsAimedAt`, `AI_LerpByAggression`, `AI_UpdateTargets`, `CMtxF__SetRotateY`, ...) |
| `code/EnemySwim` | 19 | declarations differ (5: `AI_ComputeFrameMotion`, `AI_UpdateTargets`, `CAI__CallHurt`, `CAI__SetMode`, ...) |
| `code/GameObject` | 62 | own basic typedefs (3: `s32`, `u32`, `u8`); calls typed differently (2: `GameObject_GetGroundHeight`, `GameObject_TraceMove`); declarations differ (5: `CGameRegion__GetHeight`, `CScene__GetObjectTypeID`, `CVector3__Normalize`, `GameObject_GetFrameDelta`, ...); local types differ (2: `CGameRegion`, `Triple`); parse errors (1: `}`); other (1: `structure has no member named `m_pVertices'`) |
| `code/GameObjectMove` | 6 | **merged** (round 24, after unifying its declarations; was: declarations differ (1: `Collision_MoveInstance`)) |
| `code/Generator` | 16 | declarations differ (5: `Actor_SpawnParticle`, `CInstancePool__Spawn`, `CLevelActorPool__Spawn`, `CScene__GetIntelligence`, ...); local types differ (1: `B01Info`) |
| `code/HostIo` | 34 | the files do not share their cc1 flags |
| `code/Hud` | 48 | declarations differ (5: `Draw`, `ModeState`, `gEngineApp_m_OnScreen_m_FontXScale`, `gEngineApp_m_OnScreen_m_ShadowXOff`, ...); local types differ (2: `Chunk`, `FontStyle`); parse errors (12: `(Each undeclared identifier is reported `, `*`, `FrameEntry`, `byte`, ...); other (1: `for each function it appears in.)`) |
| `code/InteractiveAnim` | 15 | **merged** (round 24, after unifying its declarations; was: declarations differ (3: `AI_PlayAnim`, `CGameObjectInstance__AdvanceAnimation`, `CGameObjectInstance__SetAnimByAction`)) |
| `code/LevelEvent` | 31 | .rodata interrupted by data no C file owns (1: `unnamed@0xA8C3C`) |
| `code/LoopSounds` | 10 | **merged** (round 24, after unifying its declarations; was: declarations differ (3: `CList__AddTail`, `CList__Remove`, `Sound__Stop`)) |
| `code/Platform` | 27 | **merged** (round 24, after unifying its declarations; was: declarations differ (2: `Platform_Step`, `StateInfo`)) |
| `code/Render` | 28 | calls typed differently (1: `Model_BlendMorphFrames`); declarations differ (2: `CCache__LockForDisplayList`, `CCache__Unlock`); local types differ (2: `Material`, `Vtx`) |
| `code/Rumble` | 10 | declarations differ (3: `RumbleCurve_Advance`, `__osDisableInt`, `__osRestoreInt`); local types differ (1: `Clip`) |
| `code/RumbleCurve` | 3 | **merged** (round 24, first batch) |
| `code/SavedPlayers` | 8 | **merged** (round 24, after unifying its declarations; was: declarations differ (1: `gSavedPlayers`)) |
| `code/SndQueue` | 12 | own basic typedefs (2: `s32`, `u8`); declarations differ (1: `SndVoice_Construct`); local types differ (5: `ChannelSet`, `Header`, `ListHead`, `ListLink`, ...) |
| `code/SndSeq` | 12 | declarations differ (8: `BinarySearch`, `HalfToFloat`, `alCSPSetBank`, `alCSPSetChlFXMix`, ...); local types differ (2: `ContextRecAt2BB8`, `IntPair`) |
| `code/Sort` | 2 | **merged** (round 24, first batch) |
| `code/Sound` | 31 | declarations differ (2: `CAudioWorld__SetForce2D`, `gEngineApp_m_Settings_unk1C8`) |
| `code/SpeechPlayer` | 7 | declarations differ (5: `CAudioWorld__GetSndPlayer`, `CAudioWorld__Lock`, `CAudioWorld__Unlock`, `alSndpSetSound`, ...) |
| `code/Sweep` | 6 | **merged** (round 24, after unifying its declarations; was: local types differ (1: `HitInst`)) |
| `code/Texture` | 8 | declarations differ (3: `CISet__GetBlock`, `CISet__GetBlockAndSize`, `Pair`) |
| `code/Throwable` | 13 | declarations differ (3: `CAI__CallHurt`, `OwnedState`, `gThrowableCollisionInfo_m_InstanceBehavior`) |
| `code/Turret` | 15 | **merged** (round 24, after unifying its declarations; was: calls typed differently (1: `Turret_EventShow`); declarations differ (1: `Turret_Effects`)) |
| `code/VM` | 20 | .rodata not in .text order (1 inverted pairs of 2 subsegments) |
| `code/WeaponMode` | 49 | declarations differ (13: `Anim`, `Bone`, `CAI__SetMode`, `CCameraPool__AddText`, ...) |
| `code/__scHandleRSP` | 17 | declarations differ (2: `Sched`, `__scHandleRetrace`); local types differ (1: `Stream`); parse errors (7: `(Each undeclared identifier is reported `, `*`, `Qn`, `Stream`, ...); other (1: `for each function it appears in.)`) |
| `code/adecHuffDecode` | 18 | the files do not share their cc1 flags |
| `code/alAdpcmPull` | 5 | **merged** (round 24, first batch) |
| `vm1/BlindOne` | 70 | declarations differ (26: `AI_UpdateTargets`, `Animator_Update`, `CCache__Allocate`, `CCache__AllocateMemoryForDecompression`, ...); parse errors (1: `DrawInfo`) |
| `vm1/CEngineApp` | 4 | **merged** (round 24, first batch) |
| `vm1/COnScreen` | 3 | **merged** (round 24, first batch) |
| `vm1/CPlayer` | 13 | .rodata interleaved with another directory's (39 subsegments between, e.g. `func_00248340`) |
| `vm1/CheatMenu` | 48 | **merged** (round 24, after unifying its declarations; was: declarations differ (2: `Controller_MenuLeft`, `Controller_MenuRight`)) |
| `vm1/Cont` | 2 | **merged** (round 24, after unifying its declarations; was: declarations differ (1: `RumblePool_Init`)) |
| `vm1/ContinueMenu` | 2 | **merged** (round 24, first batch) |
| `vm1/Demo` | 28 | .rodata interleaved with another directory's (6 subsegments between, e.g. `func_0027C074`) |
| `vm1/GameOverMenu` | 3 | **merged** (round 24, first batch) |
| `vm1/MissionMenu` | 14 | .rodata interrupted by data no C file owns (1: `unnamed@0xC2368`) |
| `vm1/MotherBoss` | 65 | declarations differ (6: `AI_UpdateTargets`, `Actor_SpawnParticle`, `CAI__CallHurt`, `CLevelActorPool__Spawn`, ...) |
| `vm1/MpMenu` | 110 | .rodata interrupted by data no C file owns (3: `unnamed@0xC1A78`) |
| `vm1/NameEntryMenu` | 8 | .rodata interrupted by data no C file owns (1: `unnamed@0xC1F4C`) |
| `vm1/OptionsMenu` | 31 | .rodata not in .text order (2 inverted pairs of 4 subsegments) |
| `vm1/Pak` | 42 | .rodata not in .text order (1 inverted pairs of 3 subsegments) |
| `vm1/PakMenu` | 140 | .rodata interrupted by data no C file owns (2: `unnamed@0xC2008`) |
| `vm1/Primagen` | 115 | .rodata interrupted by data no C file owns (2: `unnamed@0xC0F14`) |
| `vm1/SceneActorPool` | 2 | **merged** (round 24, first batch) |
| `vm1/SceneLoad` | 24 | .rodata interleaved with another directory's (15 subsegments between, e.g. `func_00220530`) |
| `vm1/StatsMenu` | 15 | declarations differ (1: `gEngineApp_m_Settings_match_tag`) |
| `vm1/TotemDefense` | 18 | .rodata not in .text order (8 inverted pairs of 9 subsegments) |
| `vm2/BlindOne` | 60 | its .text subsegments are not adjacent |
| `vm2/CEngineApp` | 4 | **merged** (round 24, first batch) |
| `vm2/COnScreen` | 2 | **merged** (round 24, first batch) |
| `vm2/CPlayer` | 13 | vm2 files placing .rodata over vm1's pool one by one (6 files, versions/shared_rodata.us.txt) |
| `vm2/CheatMenu` | 48 | vm2 files placing .rodata over vm1's pool one by one (7 files, versions/shared_rodata.us.txt) |
| `vm2/Cont` | 2 | **merged** (round 24, after unifying its declarations; was: declarations differ (1: `RumblePool_Init`)) |
| `vm2/ContinueMenu` | 2 | **merged** (round 24, first batch) |
| `vm2/Demo` | 21 | its .text subsegments are not adjacent |
| `vm2/GameOverMenu` | 3 | **merged** (round 24, first batch) |
| `vm2/MissionMenu` | 14 | vm2 files placing .rodata over vm1's pool one by one (7 files, versions/shared_rodata.us.txt) |
| `vm2/MotherBoss` | 30 | its .text subsegments are not adjacent |
| `vm2/MpMenu` | 106 | its .text subsegments are not adjacent |
| `vm2/NameEntryMenu` | 8 | vm2 files placing .rodata over vm1's pool one by one (2 files, versions/shared_rodata.us.txt) |
| `vm2/OptionsMenu` | 31 | vm2 files placing .rodata over vm1's pool one by one (6 files, versions/shared_rodata.us.txt) |
| `vm2/Pak` | 41 | its .text subsegments are not adjacent |
| `vm2/PakMenu` | 133 | its .text subsegments are not adjacent |
| `vm2/Primagen` | 35 | its .text subsegments are not adjacent |
| `vm2/SceneActorPool` | 2 | **merged** (round 24, first batch) |
| `vm2/SceneLoad` | 19 | its .text subsegments are not adjacent |
| `vm2/StatsMenu` | 15 | declarations differ (2: `CPlayerPool__GetIndex`, `gEngineApp_m_Settings_match_tag`); local types differ (1: `FieldRecAt14`) |
| `vm2/TotemDefense` | 7 | its .text subsegments are not adjacent |

## Evidence

**Linker padding (hard).** SN64's linker starts every object at a 16-byte
boundary (`SECTION ... align` in the LNK records, [LNK-FORMAT.md](LNK-FORMAT.md))
and fills the gap with whatever was in its buffer, not with nops: after
`CList__Contains` the gap holds `"Color"`, after `CHashTable__GetLongestChain`
the bytes of another function. These gaps are the `stray_*.s` files and the
unsized space between symbols; a function that starts on 16 after one is
always the first of an object. 274 such boundaries, plus 2 gaps that do not
end on 16 (before the asm exception-context code at 0x002A3708 and before the
dead fragments at the end of `.code`).

Inside one object GCC packs functions at 4 bytes, so a run without a gap can
still be several objects: one that ends exactly on 16 leaves no trace (about
one object in four).

**ultralib files (hard).** In libultra/libaudio/libc each function is matched
to its ultralib file (LIBULTRA.md); a change of file is a boundary (176, most
of them also padded). Every ultralib file the game has comes out as one
contiguous run (no file is split in two).

**Known blocks (hard).** The asm objects of [ASM_ORIGIN.md](ASM_ORIGIN.md)
(RNC unpacker, STVM TLB helpers, `exceptasm.s`, the exception stubs and
context code, the dead `.code`-tail fragments) and the six libgcc2 objects
(`_divdi3`, `_moddi3`, `_udivdi3`, `_umoddi3`, `_cmpdi2`, `_floatdisf`):
12 starts: 8 also padded, 2 after a gap, and 2 known only (`exceptasm.s`, which the ultralib map also marks, and the exception stubs at 0x002A3088, which follow `osPiGetCmdQueue` with no gap: a C object ending on 16).

**The two VM copies (hard).** The paged module is in the ROM twice, from two
links of the same objects ([VMASM.md](VMASM.md)); the stale copy's objects
end at other offsets, so a boundary padded in one copy and not in the other
holds in both (2 found this way: `OptionsMenu`, `NameEntryMenu`). The module-2
copies are named after their module-1 twins (`_2`).

**.rodata order (check).** Each object's `.rodata` is linked in the same order
as its `.text`. `tools/modules.py` checks that no module's `.rodata` (from
`build/turok2.us.map`) starts below the previous module's: **0 exceptions**.
Inside a module the per-function files put a literal shared by two functions
with either one, so the order there is not checked. One curiosity: the VM
module's level-load module (`SceneLoad`, `MatrixHeap_Construct`..) keeps its
`.rodata` in the engine's `.code_rodata` (0x800A5D70), not with the rest of
the VM module's (0x800C17xx).

**Names (soft).** Inside a hard run, runs of one class or module prefix
(`CHeap__*`, `Pak_*`) are listed as candidate objects ("name runs" in
`versions/modules.us.txt`); 100 modules hold more than one. They are not
boundaries: one `.CPP` often holds several classes (`CPlayer` with
`CPlayerPool`, `CBarrel`, the `Player*` state handlers). Module names are the
dominant prefix (by bytes), not original file names, except the libultra
ones and the few the ROM names, and four renamed where the prefix was taken
or said less than the functions (`tools/modules.py` RENAMES): `CollisionRegions`
(`Collision_SweepRegions`, was `Collision#2`), `GameObjectMove` (`ApplyCurrent`,
`SlideTo`, `TestMove`, `CanSee`, `CanMoveTo`, `PlayLandSound`; was `GameObject#2`),
`LevelEvent` (the DList/NodePool/WeightedTable helpers, then the level events
and triggers, 17 of its 31 functions; was `GameObject#3`) and `SceneLoad` (the
VM module's level loader, `CScene__Construct`/`LoadLevel`/`Decompress*`; was
`CScene#2`, its copy `SceneLoad_2`). A VM module's directory drops the `#2`
(`vm1/Cinema/`, `vm1/CPlayer/`, `vm1/Demo/`, `vm1/CEngineApp/`: the VM's own
tree), and the second copy's `unk_0042D0E0_2` (an unnamed function) goes to
`vm2/Light/`, its first-copy twin, by position.

**Original file names in the ROM.** Only three survive: the LNK records of
`engine/PTIMER.CPP` (profiling timers, compiled out of this build: none of its
exports is in the game), `engine/Virtual/VMEMORY.C` (the `VM` module,
`VM_Initialize`..`VM_FinishPageJob`, and `VM_PhysicalPool`) and
`engine/Virtual/VMASM.S` (the STVM TLB asm); the make dependency lines
`ptimer.obj: ptimer.cpp stdafx.h cppu64.h defs.h turoktime.h ptimer.h` and
`vmemory.obj: vmemory.c ie_vm.h` are stale bytes in the ROM; and libultra's
assert strings (`audio/csplayer.c`, `io/sirawdma.c` ...). Turok 1's leaked
tree ([SOURCE-LEAK.md](SOURCE-LEAK.md)) and the Rage Wars decomps' folders are
hints for naming, not evidence of Turok 2's layout.

## Numbers

| Region | Modules | Functions | Of which libraries |
|---|---:|---:|---|
| `.code` (engine) | 241 | 2,576 | 155 modules, 289 functions: libultra/libaudio/libc (149 ultralib files), libgcc (6) |
| VM module | 40 | 893 | 12 modules, 29 functions: the Controller Pak (`pfs*.c`, `contpfs.c`) |
| VM module, stale copy | 39 | 835 | the same 12 |
| all | 320 | 4,304 | |

The game's own code is 86 modules in `.code` and 28 in the VM module. With
about one object in four leaving no padding, the real count is nearer 115
and 37: the multi-prefix modules are where to look.

## The modules

`tools/modules.py --md`, the stale VM copy left out (it mirrors the VM
module). "Start evidence" is why the module starts there.

| Module | Region | First function | Last function | vram | Functions | Bytes | Start evidence |
|---|---|---|---|---|---:|---:|---|
| CList | engine | CList__Construct | CList__Contains | 0x00200500 | 13 | 1492 | region |
| ProPackDecompress | engine | ProPack__Decompress | ProPackDecompress | 0x00200AE0 | 2 | 344 | pad |
| rnc | engine | RNC_UnpackMethod1 | func_00201074 | 0x00200C40 | 7 | 1144 | known, pad, known: RNC ProPack unpacker, hand-written asm (docs/ASM_ORIGIN.md) |
| RNC | engine | RNC_UnpackMethod2 | RNC_UnpackMethod2 | 0x002010C0 | 1 | 1768 | pad |
| CISet | engine | CISet__GetDataSize | CMutex__Construct | 0x002017B0 | 14 | 792 | pad |
| CDecompressor | engine | CDecompressor__Construct | CDecompressor__Main | 0x00201AD0 | 5 | 488 | pad |
| CLoader | engine | CLoader__Construct | CLoader__DeallocLoaderEntry | 0x00201CC0 | 6 | 1128 | pad |
| InitializeSystemResources | engine | InitializeSystemResources | InitializeSystemResources | 0x00202130 | 1 | 184 | pad |
| CHeap | engine | CHeap__AllocAt | CHeap__GetFreeStats | 0x002021F0 | 7 | 1644 | pad |
| CHashTable | engine | CHashTable__Construct | CHashTable__GetLongestChain | 0x00202860 | 5 | 840 | pad |
| CCache | engine | CCache__Construct | CCache__FindOldestMemEntry | 0x00202BB0 | 56 | 15720 | pad |
| MtxPool | engine | MtxPool_Reset | MtxPool_Alloc | 0x00206920 | 2 | 100 | pad |
| Texture | engine | Texture_DecompressCallback | Texture_InvalidateCache | 0x00206990 | 8 | 4308 | pad |
| Render | engine | Render_SetCombineMode | Render_SortTreeInsert | 0x00207A70 | 30 | 18012 | pad |
| CMtxF | engine | CMtxF__TransformBox | Model_PulseVertexColors | 0x0020C0D0 | 124 | 36840 | pad |
| CLevelActor | engine | CLevelActor__Construct | CLevelActor__TouchGeometry | 0x002150C0 | 10 | 2588 | pad |
| Anim | engine | CAnimationState__GetBonePosition | Bits_CeilLog2 | 0x00215AE0 | 49 | 12328 | pad |
| CAnimationState | engine | CAnimationState__SwitchToQueued | CAnimationState__Load | 0x00218B10 | 11 | 4268 | pad |
| CGameObjectInstance | engine | CGameObjectInstance__Construct | CGameObjectInstance__ConstructLitVertices | 0x00219BC0 | 65 | 26268 | pad |
| CScene | engine | CScene__SetInstanceSavedBit | Blast_GetStrength | 0x00220260 | 96 | 27948 | pad |
| Sort | engine | Sort_Quick | Sort_Insertion | 0x00226F90 | 2 | 684 | pad |
| CInstancePool | engine | CLevelActorPool__Spawn | DLights_SwapBuffers | 0x00227240 | 18 | 5832 | pad |
| DLights | engine | DLights_GetAtPoint | CDLightSet__GetNext | 0x00228910 | 6 | 660 | pad |
| CSun | engine | CSunFrameData__Construct | func_00228C30 | 0x00228BB0 | 7 | 136 | pad |
| Color | engine | Color_ShiftHue | Color_Darken | 0x00228C40 | 3 | 1780 | pad |
| CParticle | engine | CParticle__SpawnChild | Pickup_Feedback | 0x00229340 | 216 | 91452 | pad |
| Sweep | engine | Sweep_TestBoxFaces | Collision_CompareInstanceHits | 0x0023F880 | 6 | 3964 | pad |
| Collision | engine | Collision_SweepRegionWalls | Collision_SweepMapWalls | 0x00240800 | 2 | 2052 | pad |
| CollisionRegions | engine | Collision_SweepRegions | Collision_SweepRegions | 0x00241010 | 1 | 2116 | pad |
| CCollisionSetup | engine | CCollisionInfo__Reset | CCollisionSetup__Construct | 0x00241860 | 3 | 252 | pad |
| AI | engine | Platform_GetCollisionBox | AI_DropPickupsByMask | 0x00241960 | 48 | 21084 | pad |
| CPlayer | engine | CPlayer__ReadInput | String_CountTrailingSpaces | 0x00246BC0 | 250 | 69244 | pad |
| WeaponMode | engine | WeaponActor_BuildMatrix | WeaponMode_AE934_Enter | 0x00257A40 | 49 | 9240 | pad |
| EnemyAnim | engine | EnemyAnim_Idle_AF6F0 | EnemyAnim_Move_B0600 | 0x00259E60 | 76 | 3880 | pad |
| EnemyAI | engine | AI_TurnHeadToTarget | EnemyAI_DrawFadeOut | 0x0025AD90 | 119 | 22652 | pad |
| EnemyHover | engine | EnemyStationary_Setup | EnemyAI_Sidestep | 0x00260610 | 9 | 1860 | pad |
| EnemySwim | engine | EnemyFly_SteerToPathScaled | EnemySwim_SteerToPath | 0x00260D60 | 19 | 5100 | pad |
| Turret | engine | Turret_UpdateHinge | Turret_Hurt | 0x00262150 | 15 | 2760 | pad |
| Generator | engine | Generator_IsPickupUnlocked | Generator_SpawnEffect | 0x00262C20 | 16 | 3572 | pad |
| Platform | engine | Platform_UpdateSpecial | Platform_EventHide | 0x00263A20 | 27 | 3956 | pad |
| Throwable | engine | Throwable_CalcOrientation | Throwable_FlyingUpdate | 0x002649A0 | 13 | 2140 | pad |
| Door | engine | Door_OpenUpdate | AI_AnimReached | 0x00265200 | 16 | 2084 | pad |
| Destructible | engine | Destructible_Setup | Destructible_Hurt | 0x00265A30 | 15 | 1364 | pad |
| Action | engine | Action_CheckRequiredItem | Action_DrawFade | 0x00265F90 | 17 | 1812 | pad |
| InteractiveAnim | engine | AI_PlayAnim | InteractiveAnim_DrawAllModels | 0x002666B0 | 15 | 1228 | pad |
| BinaryRange | engine | GetMemSize | IsCodePointer | 0x00266B80 | 13 | 1288 | pad |
| GameObject | engine | GameObject_IsSolid | GameObject_GetFloorTilt | 0x00267090 | 62 | 8120 | pad |
| GameObjectMove | engine | GameObject_ApplyCurrent | GameObject_PlayLandSound | 0x00269050 | 6 | 2620 | pad |
| CGameRegion | engine | CGameRegion__GetCeilingNormal | CGameRegion__GetMaxHeight | 0x00269A90 | 53 | 13784 | pad |
| RandomInPackedRange | engine | Material_Unpack | RandomInPackedRange | 0x0026D070 | 2 | 392 | pad |
| HalfToFloat | engine | FloatToHalf | HalfToFloat | 0x0026D200 | 2 | 40 | pad |
| LevelEvent | engine | DList_Init | LevelEvent_CheckRequiredItem | 0x0026D230 | 31 | 5416 | pad |
| Rumble | engine | Rumble_GetLevelAt | RumblePool_FreeAll | 0x0026E760 | 10 | 1732 | pad |
| RumbleCurve | engine | RumbleCurve_Init | RumbleCurve_GetValue | 0x0026EE30 | 3 | 380 | pad |
| Level | engine | func_0026EFB0 | Level_GetHub | 0x0026EFB0 | 2 | 200 | pad |
| CAudioWorld | engine | Frustum_FromCorners | SndVoicePool_ReleaseVoice | 0x0026F080 | 101 | 23220 | pad |
| SndSeq | engine | SndSeq_Update | SndSeq_Load | 0x00274B40 | 12 | 2524 | pad |
| Sound | engine | Sound__Initialize | Sound__Unlock | 0x00275520 | 31 | 1700 | pad |
| LoopSounds | engine | func_00275BE0 | func_00275F8C | 0x00275BE0 | 10 | 948 | pad |
| SndQueue | engine | SndQueue_Add | SndQueue_LinkHead | 0x00275FA0 | 12 | 2164 | pad |
| SoundEffectInfo | engine | SoundEffectInfo_GetGroup | SoundEffectInfo_GetPanEnvelope | 0x00276820 | 14 | 168 | pad |
| SpeechPlayer | engine | SpeechPlayer_Update | SpeechPlayer_StopUnlocked | 0x002768D0 | 7 | 1976 | pad |
| Hud | engine | COnScreen__DrawBossBar | func_0027BB74 | 0x00277090 | 50 | 19180 | pad |
| Demo | engine | Demo_UpdateInput | Demo_AdvanceMatchStream | 0x0027BB80 | 3 | 396 | pad |
| CCamera | engine | CIntelligence__Init | CCameraPool__AddTextEx | 0x0027BD10 | 67 | 26156 | pad |
| CEmitter | engine | CEmitter__Draw | Emitter_CompareHeightAsc | 0x00282340 | 10 | 5276 | pad |
| Cinema | engine | Cinema__Initialize | Cinema_GetActive | 0x002837E0 | 34 | 3700 | pad |
| CPathTrack | engine | CPathTrack__Advance | CPathTrack__GetCurrentPoint | 0x00284660 | 3 | 360 | pad |
| unk_002847D0 | engine | func_002847D0 | func_002847D0 | 0x002847D0 | 1 | 8 | pad |
| Controller | engine | Controller_ReadAll | Controller_EnableMenuSounds | 0x002847E0 | 37 | 5384 | pad |
| CEngineApp | engine | EngineMode03_ExpansionPak_Draw | CEngineApp__StopRumble | 0x00285CF0 | 90 | 17192 | pad |
| __scHandleRSP | engine | __scMain | Sched_SubmitTask | 0x0028A020 | 17 | 5124 | pad |
| Crash | engine | Crash_PutChar | Crash_PrintString | 0x0028B430 | 16 | 6988 | pad |
| SavedPlayers | engine | SavedPlayers_Arm | SavedPlayers_Save | 0x0028CF80 | 8 | 1012 | pad |
| Boot | engine | func_0028D380 | func_0028D814 | 0x0028D380 | 4 | 1180 | pad |
| VM | engine | VM_Initialize | VM_FinishPageJob | 0x0028D820 | 20 | 5660 | pad |
| stvm_tlb | engine | func_0028EE40 | func_0028EEFC | 0x0028EE40 | 4 | 324 | known, pad, known: STVM TLB helpers, asm (docs/ASM_ORIGIN.md) |
| libultra/audio/cseq.c | engine | __alCSeqGetTrackEvent | __readVarLen | 0x0028EF90 | 12 | 2672 | pad, ultralib |
| libultra/audio/cspgetstate.c | engine | alCSPGetState | alCSPGetState | 0x0028FA00 | 1 | 12 | ultralib |
| libultra/audio/csplayer.c | engine | alCSPNew | __CSPSetUsptFromTempo | 0x0028FA10 | 8 | 4988 | pad, ultralib |
| libultra/audio/cents2ratio.c | engine | alCents2Ratio | alCents2Ratio | 0x00290D90 | 1 | 80 | pad, ultralib |
| libultra/audio/cspplay.c | engine | alCSPPlay | alCSPPlay | 0x00290DE0 | 1 | 44 | ultralib |
| libultra/audio/cspsetbank.c | engine | alCSPSetBank | alCSPSetBank | 0x00290E10 | 1 | 48 | pad, ultralib |
| libultra/audio/cspsetfxmix.c | engine | alCSPSetChlFXMix | alCSPSetChlFXMix | 0x00290E40 | 1 | 68 | ultralib |
| libultra/audio/cspsetseq.c | engine | alCSPSetSeq | alCSPSetSeq | 0x00290E90 | 1 | 48 | pad, ultralib |
| libultra/audio/cspsetvol.c | engine | alCSPSetVol | alCSPSetVol | 0x00290EC0 | 1 | 48 | ultralib |
| libultra/audio/cspstop.c | engine | alCSPStop | alCSPStop | 0x00290EF0 | 1 | 44 | ultralib |
| libultra/audio/event.c | engine | alEvtqNew | alEvtqFlushType | 0x00290F20 | 5 | 840 | pad, ultralib |
| libultra/audio/copy.c | engine | alCopy | alCopy | 0x00291270 | 1 | 44 | pad, ultralib |
| libultra/audio/heapalloc.c | engine | alHeapDBAlloc | alHeapDBAlloc | 0x002912A0 | 1 | 72 | pad, ultralib |
| libultra/audio/heapinit.c | engine | alHeapInit | alHeapInit | 0x002912F0 | 1 | 52 | pad, ultralib |
| libultra/audio/seqplayer.c | engine | alSeqpNew | __setUsptFromTempo | 0x00291330 | 21 | 6832 | pad, ultralib |
| libultra/audio/seq.c | engine | alSeqNextEvent | __alSeqReadWord | 0x00292DE0 | 13 | 1476 | ultralib |
| libultra/audio/sl.c | engine | alUnlink | alClose | 0x002933B0 | 4 | 188 | pad, ultralib |
| libultra/audio/sndpallocate.c | engine | alSndpAllocate | alSndpAllocate | 0x00293470 | 1 | 184 | pad, ultralib |
| libultra/audio/sndpdeallocate.c | engine | alSndpDeallocate | alSndpDeallocate | 0x00293530 | 1 | 68 | pad, ultralib |
| libultra/audio/sndpgetstate.c | engine | alSndpGetState | alSndpGetState | 0x00293580 | 1 | 36 | pad, ultralib |
| libultra/audio/sndplayer.c | engine | alSndpNew | _DivS32ByF32 | 0x002935B0 | 5 | 1832 | pad, ultralib |
| libultra/audio/sndpplay.c | engine | alSndpPlay | alSndpPlay | 0x00293CE0 | 1 | 96 | pad, ultralib |
| libultra/audio/sndpsetfxmix.c | engine | alSndpSetFXMix | alSndpSetFXMix | 0x00293D40 | 1 | 76 | ultralib |
| libultra/audio/sndpsetpan.c | engine | alSndpSetPan | alSndpSetPan | 0x00293D90 | 1 | 76 | pad, ultralib |
| libultra/audio/sndpsetpitch.c | engine | alSndpSetPitch | alSndpSetPitch | 0x00293DE0 | 1 | 76 | pad, ultralib |
| libultra/audio/sndpsetpriority.c | engine | alSndpSetPriority | alSndpSetPriority | 0x00293E30 | 1 | 40 | pad, ultralib |
| libultra/audio/sndpsetsound.c | engine | alSndpSetSound | alSndpSetSound | 0x00293E60 | 1 | 16 | pad, ultralib |
| libultra/audio/sndpsetvol.c | engine | alSndpSetVol | alSndpSetVol | 0x00293E70 | 1 | 76 | ultralib |
| libultra/audio/sndpstop.c | engine | alSndpStop | alSndpStop | 0x00293EC0 | 1 | 72 | pad, ultralib |
| libultra/audio/synaddplayer.c | engine | alSynAddPlayer | alSynAddPlayer | 0x00293F10 | 1 | 80 | pad, ultralib |
| libultra/audio/synallocvoice.c | engine | alSynAllocVoice | _allocatePVoice | 0x00293F60 | 2 | 488 | ultralib |
| libultra/audio/syndelete.c | engine | alSynDelete | alSynDelete | 0x00294150 | 1 | 8 | pad, ultralib |
| libultra/audio/synfreevoice.c | engine | alSynFreeVoice | alSynFreeVoice | 0x00294160 | 1 | 156 | pad, ultralib |
| libultra/audio/synsetfxmix.c | engine | alSynSetFXMix | alSynSetFXMix | 0x00294200 | 1 | 176 | pad, ultralib |
| libultra/audio/synsetpan.c | engine | alSynSetPan | alSynSetPan | 0x002942B0 | 1 | 144 | ultralib |
| libultra/audio/synsetpitch.c | engine | alSynSetPitch | alSynSetPitch | 0x00294340 | 1 | 140 | ultralib |
| libultra/audio/synsetpriority.c | engine | alSynSetPriority | alSynSetPriority | 0x002943D0 | 1 | 8 | pad, ultralib |
| libultra/audio/synsetvol.c | engine | alSynSetVol | alSynSetVol | 0x002943E0 | 1 | 184 | pad, ultralib |
| libultra/audio/synstartvoice.c | engine | alSynStartVoice | alSynStartVoice | 0x002944A0 | 1 | 148 | pad, ultralib |
| libultra/audio/synstartvoiceparam.c | engine | alSynStartVoiceParams | alSynStartVoiceParams | 0x00294540 | 1 | 260 | pad, ultralib |
| libultra/audio/synstopvoice.c | engine | alSynStopVoice | alSynStopVoice | 0x00294650 | 1 | 124 | pad, ultralib |
| libultra/audio/synthesizer.c | engine | alSynNew | _timeToSamplesNoRound | 0x002946D0 | 9 | 1668 | pad, ultralib |
| libultra/audio/auxbus.c | engine | alAuxBusPull | alAuxBusParam | 0x00294D60 | 2 | 268 | pad, ultralib |
| libultra/audio/drvrnew.c | engine | alFxNew | init_lpfilter | 0x00294E70 | 8 | 2072 | pad, ultralib |
| libultra/audio/env.c | engine | alEnvmixerPull | _ldexpf | 0x00295690 | 7 | 3236 | pad, ultralib |
| libultra/audio/filter.c | engine | alFilterNew | alFilterNew | 0x00296340 | 1 | 28 | pad, ultralib |
| libultra/audio/mainbus.c | engine | alMainBusPull | alMainBusParam | 0x00296360 | 2 | 368 | pad, ultralib |
| libultra/audio/resample.c | engine | alResamplePull | alResampleParam | 0x002964D0 | 2 | 640 | ultralib |
| libultra/audio/reverb.c | engine | alFxPull | _doModFunc | 0x00296750 | 8 | 2952 | ultralib |
| libultra/audio/save.c | engine | alSavePull | alSaveParam | 0x002972E0 | 2 | 308 | pad, ultralib |
| libultra/audio/synallocfx.c | engine | alSynAllocFX | alSynAllocFX | 0x00297420 | 1 | 148 | pad, ultralib |
| libultra/gu/cosf.c | engine | func_002974C0 | func_002974C0 | 0x002974C0 | 1 | 260 | pad, ultralib |
| libultra/gu/lookat.c | engine | guLookAt | guLookAtF | 0x002975D0 | 2 | 1288 | pad, ultralib |
| libultra/gu/mtxutil.c | engine | guMtxIdent | guMtxL2F | 0x00297AE0 | 4 | 620 | pad, ultralib |
| libultra/gu/ortho.c | engine | guOrtho | guOrthoF | 0x00297D50 | 2 | 604 | pad, ultralib |
| libultra/gu/perspective.c | engine | guPerspective | guPerspectiveF | 0x00297FB0 | 2 | 824 | pad, ultralib |
| libultra/gu/sinf.c | engine | func_002982F0 | func_002982F0 | 0x002982F0 | 1 | 380 | pad, ultralib |
| libultra/gu/sqrtf.s | engine | func_00298470 | func_00298470 | 0x00298470 | 1 | 12 | pad, ultralib |
| libultra/io/aigetstat.c | engine | osAiGetStatus | osAiGetStatus | 0x00298480 | 1 | 20 | pad, ultralib |
| libultra/io/aisetfreq.c | engine | osAiSetFrequency | osAiSetFrequency | 0x002984A0 | 1 | 280 | pad, ultralib |
| libultra/io/aisetnextbuf.c | engine | osAiSetNextBuffer | osAiSetNextBuffer | 0x002985C0 | 1 | 140 | pad, ultralib |
| libultra/io/ai.c | engine | __osAiDeviceBusy | __osAiDeviceBusy | 0x00298650 | 1 | 20 | pad, ultralib |
| libultra/io/contquery.c | engine | osContStartQuery | osContGetQuery | 0x00298670 | 2 | 168 | pad, ultralib |
| libultra/io/contreaddata.c | engine | osContStartReadData | __osPackReadData | 0x00298720 | 3 | 480 | pad, ultralib |
| libultra/io/contreset.c | engine | osContReset | __osPackResetData | 0x00298900 | 2 | 320 | ultralib |
| libultra/io/controller.c | engine | osContInit | __osContGetInitData | 0x00298A40 | 3 | 780 | ultralib |
| libultra/io/dpsetnextbuf.c | engine | osDpSetNextBuffer | osDpSetNextBuffer | 0x00298D50 | 1 | 156 | pad, ultralib |
| libultra/io/dp.c | engine | __osDpDeviceBusy | __osDpDeviceBusy | 0x00298DF0 | 1 | 24 | pad, ultralib |
| libultra/io/motor.c | engine | osMotorStop | osMotorInit | 0x00298E10 | 3 | 2148 | pad, ultralib |
| libultra/io/contramread.c | engine | __osContRamRead | __osPackRamReadData | 0x00299680 | 2 | 952 | pad, ultralib |
| libultra/io/contramwrite.c | engine | __osContRamWrite | __osPackRamWriteData | 0x00299A40 | 2 | 952 | pad, ultralib |
| libultra/io/crc.c | engine | __osContAddressCrc | __osContDataCrc | 0x00299E00 | 2 | 168 | pad, ultralib |
| libultra/io/pirawdma.c | engine | osPiRawStartDma | osPiRawStartDma | 0x00299EB0 | 1 | 212 | pad, ultralib |
| libultra/io/siacs.c | engine | __osSiCreateAccessQueue | __osSiRelAccess | 0x00299F90 | 3 | 232 | pad, ultralib |
| libultra/io/sirawdma.c | engine | __osSiRawStartDma | __osSiRawStartDma | 0x0029A080 | 1 | 184 | pad, ultralib |
| libultra/io/si.c | engine | __osSiDeviceBusy | __osSiDeviceBusy | 0x0029A140 | 1 | 24 | pad, ultralib |
| libultra/io/sptask.c | engine | _VirtualToPhysicalTask | osSpTaskStartGo | 0x0029A160 | 3 | 536 | pad, ultralib |
| libultra/io/sp.c | engine | __osSpDeviceBusy | __osSpDeviceBusy | 0x0029A380 | 1 | 24 | pad, ultralib |
| libultra/io/sprawdma.c | engine | __osSpRawStartDma | __osSpRawStartDma | 0x0029A3A0 | 1 | 252 | pad, ultralib |
| libultra/io/spsetpc.c | engine | __osSpSetPc | __osSpSetPc | 0x0029A4A0 | 1 | 44 | pad, ultralib |
| libultra/io/spsetstat.c | engine | __osSpSetStatus | __osSpSetStatus | 0x0029A4D0 | 1 | 20 | pad, ultralib |
| libultra/io/sptaskyield.c | engine | osSpTaskYield | osSpTaskYield | 0x0029A4F0 | 1 | 28 | pad, ultralib |
| libultra/io/sptaskyielded.c | engine | osSpTaskYielded | osSpTaskYielded | 0x0029A510 | 1 | 76 | pad, ultralib |
| libultra/io/spgetstat.c | engine | __osSpGetStatus | __osSpGetStatus | 0x0029A560 | 1 | 20 | pad, ultralib |
| libultra/io/viblack.c | engine | osViBlack | osViBlack | 0x0029A580 | 1 | 92 | pad, ultralib |
| libultra/io/vi.c | engine | __osViInit | __osViInit | 0x0029A5E0 | 1 | 252 | pad, ultralib |
| libultra/io/vigetcurrframebuf.c | engine | osViGetCurrentFramebuffer | osViGetCurrentFramebuffer | 0x0029A6E0 | 1 | 56 | pad, ultralib |
| libultra/io/vimgr.c | engine | osCreateViManager | viMgrMain | 0x0029A720 | 2 | 752 | pad, ultralib |
| libultra/io/vigetcurrcontext.c | engine | __osViGetCurrentContext | __osViGetCurrentContext | 0x0029AA10 | 1 | 16 | ultralib |
| libultra/io/visetevent.c | engine | osViSetEvent | osViSetEvent | 0x0029AA20 | 1 | 88 | ultralib |
| libultra/io/visetmode.c | engine | osViSetMode | osViSetMode | 0x0029AA80 | 1 | 76 | pad, ultralib |
| libultra/io/visetspecial.c | engine | osViSetSpecialFeatures | osViSetSpecialFeatures | 0x0029AAD0 | 1 | 356 | pad, ultralib |
| libultra/io/visetyscale.c | engine | osViSetYScale | osViSetYScale | 0x0029AC40 | 1 | 68 | pad, ultralib |
| libultra/io/viswapbuf.c | engine | osViSwapBuffer | osViSwapBuffer | 0x0029AC90 | 1 | 68 | pad, ultralib |
| libultra/io/viswapcontext.c | engine | __osViSwapContext | __osViSwapContext | 0x0029ACE0 | 1 | 828 | pad, ultralib |
| libultra/debug/assert.c | engine | __assert | __assert | 0x0029B020 | 1 | 8 | pad, ultralib |
| libultra/os/createmesgqueue.c | engine | osCreateMesgQueue | osCreateMesgQueue | 0x0029B030 | 1 | 36 | pad, ultralib |
| libultra/os/createthread.c | engine | osCreateThread | osCreateThread | 0x0029B060 | 1 | 216 | pad, ultralib |
| libultra/os/getcurrfaultthread.c | engine | __osGetCurrFaultedThread | __osGetCurrFaultedThread | 0x0029B140 | 1 | 16 | pad, ultralib |
| libultra/os/getthreadpri.c | engine | osGetThreadPri | osGetThreadPri | 0x0029B150 | 1 | 28 | ultralib |
| libultra/os/gettime.c | engine | osGetTime | osGetTime | 0x0029B170 | 1 | 124 | pad, ultralib |
| libultra/os/initialize.c | engine | osInitialize | osInitialize | 0x0029B1F0 | 1 | 544 | pad, ultralib |
| libultra/io/pirawread.c | engine | osPiRawReadIo | osPiRawReadIo | 0x0029B410 | 1 | 148 | ultralib |
| libultra/io/sirawread.c | engine | __osSiRawReadIo | __osSiRawReadIo | 0x0029B4B0 | 1 | 144 | pad, ultralib |
| libultra/io/sirawwrite.c | engine | __osSiRawWriteIo | __osSiRawWriteIo | 0x0029B540 | 1 | 108 | ultralib |
| libultra/os/jammesg.c | engine | osJamMesg | osJamMesg | 0x0029B5B0 | 1 | 308 | pad, ultralib |
| libultra/os/recvmesg.c | engine | osRecvMesg | osRecvMesg | 0x0029B6F0 | 1 | 292 | pad, ultralib |
| libultra/os/sendmesg.c | engine | osSendMesg | osSendMesg | 0x0029B820 | 1 | 304 | pad, ultralib |
| libultra/os/seteventmesg.c | engine | osSetEventMesg | osSetEventMesg | 0x0029B950 | 1 | 92 | ultralib |
| libultra/os/setthreadpri.c | engine | osSetThreadPri | osSetThreadPri | 0x0029B9B0 | 1 | 196 | pad, ultralib |
| libultra/os/settimer.c | engine | osSetTimer | osSetTimer | 0x0029BA80 | 1 | 140 | pad, ultralib |
| libultra/os/startthread.c | engine | osStartThread | osStartThread | 0x0029BB10 | 1 | 276 | pad, ultralib |
| libultra/os/thread.c | engine | __osDequeueThread | __osDequeueThread | 0x0029BC30 | 1 | 52 | pad, ultralib |
| libultra/os/timerintr.c | engine | __osTimerInterrupt | __osInsertTimer | 0x0029BC70 | 4 | 772 | pad, ultralib |
| libultra/os/virtualtophysical.c | engine | osVirtualToPhysical | osVirtualToPhysical | 0x0029BF80 | 1 | 96 | pad, ultralib |
| libultra/os/exceptasm.s | engine | __osException | __osCleanupThread | 0x0029BFE0 | 7 | 2820 | known, ultralib, known: ultralib exceptasm.s (docs/ASM_ORIGIN.md) |
| libultra/io/leointerrupt.c | engine | __osLeoInterrupt | __osLeoResume | 0x0029CAF0 | 3 | 1692 | pad, ultralib |
| libultra/io/epirawdma.c | engine | osEPiRawStartDma | osEPiRawStartDma | 0x0029D190 | 1 | 468 | pad, ultralib |
| libultra/io/leodiskinit.c | engine | osLeoDiskInit | osLeoDiskInit | 0x0029D370 | 1 | 212 | pad, ultralib |
| libultra/io/pimgr.c | engine | osCreatePiManager | osCreatePiManager | 0x0029D450 | 1 | 376 | pad, ultralib |
| libultra/io/cartrominit.c | engine | osCartRomInit | osCartRomInit | 0x0029D5D0 | 1 | 192 | pad, ultralib |
| libultra/io/devmgr.c | engine | __osDevMgrMain | __osDevMgrMain | 0x0029D690 | 1 | 920 | ultralib |
| libultra/io/epirawread.c | engine | osEPiRawReadIo | osEPiRawReadIo | 0x0029DA30 | 1 | 404 | pad, ultralib |
| libultra/io/epirawwrite.c | engine | osEPiRawWriteIo | osEPiRawWriteIo | 0x0029DBD0 | 1 | 320 | pad, ultralib |
| libultra/io/piacs.c | engine | __osPiCreateAccessQueue | __osPiRelAccess | 0x0029DD10 | 3 | 232 | ultralib |
| libultra/os/destroythread.c | engine | osDestroyThread | osDestroyThread | 0x0029DE00 | 1 | 216 | pad, ultralib |
| libultra/os/resetglobalintmask.c | engine | __osResetGlobalIntMask | __osResetGlobalIntMask | 0x0029DEE0 | 1 | 72 | pad, ultralib |
| libultra/os/setglobalintmask.c | engine | __osSetGlobalIntMask | __osSetGlobalIntMask | 0x0029DF30 | 1 | 64 | pad, ultralib |
| libultra/os/yieldthread.c | engine | osYieldThread | osYieldThread | 0x0029DF70 | 1 | 72 | ultralib |
| libultra/os/getcount.s | engine | osGetCount | osGetCount | 0x0029DFC0 | 1 | 12 | pad, ultralib |
| libultra/os/getfpccsr.s | engine | __osGetFpcCsr | __osGetFpcCsr | 0x0029DFD0 | 1 | 12 | pad, ultralib |
| libultra/os/getsr.s | engine | __osGetSR | __osGetSR | 0x0029DFE0 | 1 | 12 | pad, ultralib |
| libultra/os/interrupt.s | engine | __osDisableInt | __osRestoreInt | 0x0029DFF0 | 2 | 60 | pad, ultralib |
| libultra/os/invaldcache.s | engine | osInvalDCache | osInvalDCache | 0x0029E030 | 1 | 152 | pad, ultralib |
| libultra/os/invalicache.s | engine | osInvalICache | osInvalICache | 0x0029E0D0 | 1 | 104 | pad, ultralib |
| libultra/os/probetlb.s | engine | __osProbeTLB | __osProbeTLB | 0x0029E140 | 1 | 200 | pad, ultralib |
| libultra/os/setcompare.s | engine | __osSetCompare | __osSetCompare | 0x0029E210 | 1 | 12 | pad, ultralib |
| libultra/os/setfpccsr.s | engine | __osSetFpcCsr | __osSetFpcCsr | 0x0029E220 | 1 | 16 | pad, ultralib |
| libultra/os/setintmask.s | engine | osSetIntMask | osSetIntMask | 0x0029E230 | 1 | 168 | ultralib |
| libultra/os/setsr.s | engine | __osSetSR | __osSetSR | 0x0029E2E0 | 1 | 16 | pad, ultralib |
| libultra/os/unmaptlball.s | engine | __osUnmapTLBAll | __osUnmapTLBAll | 0x0029E2F0 | 1 | 68 | ultralib |
| libultra/os/writebackdcache.s | engine | osWritebackDCache | osWritebackDCache | 0x0029E340 | 1 | 104 | pad, ultralib |
| libultra/os/writebackdcacheall.s | engine | osWritebackDCacheAll | osWritebackDCacheAll | 0x0029E3B0 | 1 | 40 | pad, ultralib |
| libultra/libc/sprintf.c | engine | sprintf | proutSprintf | 0x0029E3E0 | 2 | 128 | pad, ultralib |
| libultra/libc/string.c | engine | memcpy | strlen | 0x0029E460 | 3 | 132 | ultralib |
| libultra/libc/xprintf.c | engine | _Printf | _Putfld | 0x0029E4F0 | 2 | 2480 | pad, ultralib |
| libultra/libc/xlitob.c | engine | _Litob | _Litob | 0x0029EEA0 | 1 | 592 | ultralib |
| libultra/libc/ldiv.c | engine | lldiv | lldiv | 0x0029F0F0 | 1 | 136 | ultralib |
| libultra/libc/xldtob.c | engine | _Ldtob | _Ldtob | 0x0029F180 | 1 | 8 | pad, ultralib |
| alAdpcmPull | engine | alAdpcmPull | _decodeChunk | 0x0029F190 | 5 | 3292 | pad, ultralib |
| adecHuffDecode | engine | adecReadBits | adecClearStopFlag | 0x0029FE70 | 18 | 5244 | pad |
| HostIo | engine | HostIo_DisableInt | PClseek | 0x002A12F0 | 34 | 7352 | pad |
| osPiStartDma | engine | osPiStartDma | osPiStartDma | 0x002A2FB0 | 1 | 172 | pad |
| osPiGetCmdQueue | engine | osPiGetCmdQueue | osPiGetCmdQueue | 0x002A3060 | 1 | 40 | pad |
| exception_stubs | engine | HostIo_VectorStubs | HostIo_ReadTlb | 0x002A3088 | 7 | 504 | known, known: exception-vector stubs, asm (docs/ASM_ORIGIN.md) |
| exception_context | engine | HostIo_RestoreVectors | HostIo_SaveContext | 0x002A3708 | 3 | 972 | gap, known, known: exception context save/return, asm (docs/ASM_ORIGIN.md) |
| libgcc/_divdi3 | engine | __divdi3 | __divdi3 | 0x002A3AE0 | 1 | 1516 | known, pad, known: GCC 2.8.1 libgcc2.c L_divdi3 (file header) |
| libgcc/_moddi3 | engine | __moddi3 | __moddi3 | 0x002A40D0 | 1 | 1400 | known, pad, known: GCC 2.8.1 libgcc2.c L_moddi3 |
| libgcc/_udivdi3 | engine | __udivdi3 | __udivdi3 | 0x002A4650 | 1 | 1412 | known, pad, known: GCC 2.8.1 libgcc2.c L_udivdi3 |
| libgcc/_umoddi3 | engine | __umoddi3 | __umoddi3 | 0x002A4BE0 | 1 | 1268 | known, pad, known: GCC 2.8.1 libgcc2.c L_umoddi3 |
| libgcc/_cmpdi2 | engine | __cmpdi2 | __cmpdi2 | 0x002A50E0 | 1 | 72 | known, pad, known: GCC 2.8.1 libgcc2.c L_cmpdi2 |
| libgcc/_floatdisf | engine | __floatdisf | __floatdisf | 0x002A5130 | 1 | 176 | known, pad, known: GCC 2.8.1 libgcc2.c L_floatdisf |
| code_tail_fragments | engine | func_002C22D8 | func_002C232C | 0x002C22D8 | 2 | 256 | gap, known, known: dead copies of older code (docs/ASM_ORIGIN.md) |
| unk_002C23E0 | engine | func_002C23E0 | func_002C23E0 | 0x002C23E0 | 1 | 148 | pad |
| Primagen | vm1 | Automap_RevealRegion | func_00407F0C | 0x00400000 | 115 | 32532 | region |
| TotemDefense | vm1 | TotemDefense_Complete | TotemDefense_OnEvent | 0x00407F20 | 18 | 4312 | pad |
| MotherBoss | vm1 | MotherBoss_Hurt | MotherBoss_OnDefeatCinemaDone | 0x00409000 | 66 | 14412 | pad |
| BlindOne | vm1 | BlindOne_UpdateMover | BlindOne_SumGroupHealth | 0x0040C850 | 73 | 10728 | pad |
| Cinema#2 | vm1 | Cinema_SplineSecondDerivs | Cinema_IsWaitCue | 0x0040F240 | 44 | 16360 | pad |
| MenuItem | vm1 | MenuItem_FormatBinding | MenuPool_CloseMenu | 0x00413230 | 52 | 16980 | pad |
| MpMenu | vm1 | MpMenu_StartFragTag | PauseMenu_Update | 0x00417490 | 110 | 12056 | pad |
| ContinueMenu | vm1 | ContinueMenu_Yes | ContinueMenu_No | 0x0041A3B0 | 2 | 108 | pad |
| GameOverMenu | vm1 | GameOverMenu_Init | GameOverMenu_TickCountdown | 0x0041A420 | 3 | 172 | pad |
| CheatMenu | vm1 | CheatsMenu_Init | CheatMenu_FrootyStripesText | 0x0041A4D0 | 48 | 5496 | pad |
| OptionsMenu | vm1 | GameSettings_SetDefaults | OptionsMenu_BrightnessText | 0x0041BA50 | 31 | 3844 | pad |
| StatsMenu | vm1 | StatsMenu_SaveProfile | StatsMenu_InitAfterPak | 0x0041C960 | 15 | 1772 | pad |
| NameEntryMenu | vm1 | NameEntryMenu_Init | NameEntryMenu_IsMatchOver | 0x0041D050 | 8 | 2100 | pad |
| PakMenu | vm1 | SaveGame_Build | InventoryMenu_SelectOk | 0x0041D890 | 140 | 29132 | pad |
| MissionMenu | vm1 | Mission_LevelObjectivesDone | func_00425710 | 0x00424A60 | 15 | 3316 | pad |
| Pak | vm1 | Pak_RefreshNotes | Pak_IsDirectoryAllocated | 0x00425760 | 42 | 6676 | pad |
| libultra/io/pfsallocatefile.c | vm1 | osPfsAllocateFile | __osClearPage | 0x00427180 | 3 | 1536 | pad, ultralib |
| libultra/io/pfsdeletefile.c | vm1 | osPfsDeleteFile | __osBlockSum | 0x00427780 | 3 | 1180 | ultralib |
| libultra/io/pfsfilestate.c | vm1 | osPfsFileState | osPfsFileState | 0x00427C20 | 1 | 572 | pad, ultralib |
| libultra/io/pfsfreeblocks.c | vm1 | osPfsFreeBlocks | osPfsFreeBlocks | 0x00427E60 | 1 | 280 | pad, ultralib |
| libultra/io/pfsgetstatus.c | vm1 | __osPfsGetStatus | __osPfsGetOneChannelData | 0x00427F80 | 3 | 480 | pad, ultralib |
| libultra/io/pfsinitpak.c | vm1 | osPfsInitPak | osPfsInitPak | 0x00428160 | 1 | 464 | ultralib |
| libultra/io/pfschecker.c | vm1 | osPfsChecker | corrupted | 0x00428330 | 3 | 1768 | ultralib |
| libultra/io/pfsisplug.c | vm1 | osPfsIsPlug | __osPfsGetInitData | 0x00428A20 | 3 | 716 | pad, ultralib |
| libultra/io/pfsreadwritefile.c | vm1 | osPfsReadWriteFile | osPfsReadWriteFile | 0x00428CF0 | 1 | 1076 | pad, ultralib |
| libultra/io/pfsrepairid.c | vm1 | osPfsRepairId | osPfsRepairId | 0x00429130 | 1 | 432 | pad, ultralib |
| libultra/io/pfssearchfile.c | vm1 | osPfsFindFile | osPfsFindFile | 0x004292E0 | 1 | 372 | ultralib |
| libultra/io/contpfs.c | vm1 | __osRepairPackId | __osPfsSelectBank | 0x00429460 | 8 | 2744 | pad, ultralib |
| SceneLoad | vm1 | MatrixHeap_Construct | CScene__ConstructMap | 0x00429F20 | 25 | 12388 | pad, ultralib |
| SceneActorPool | vm1 | SceneActorPool_Construct | CInstancePool__Construct | 0x0042CF90 | 2 | 388 | pad |
| CDLightSet | vm1 | CDLightSet__Construct | CDLightSet__Construct | 0x0042D120 | 1 | 152 | pad |
| Light | vm1 | Light_SetDefaultDirection | Light_SetDefaultDirection | 0x0042D1C0 | 1 | 84 | pad |
| CParticleSystem | vm1 | CParticleSystem__Construct | CParticleSystem__Construct | 0x0042D220 | 1 | 488 | pad |
| SwooshSystem | vm1 | SwooshSystem_Construct | SwooshSystem_Construct | 0x0042D410 | 1 | 252 | pad |
| CPlayer#2 | vm1 | CPlayer__SetupActors | CPlayer__StopLoopingSounds | 0x0042DA60 | 13 | 4332 | pad |
| COnScreen | vm1 | COnScreen__Construct | COnScreen__ResetBossBars | 0x0042EB50 | 3 | 220 | pad |
| Demo#2 | vm1 | Demo_ReadRunLength | CEmitterPool__Construct | 0x0042EC30 | 28 | 5176 | pad |
| Cont | vm1 | Cont_InitAll | Cont_ResetSlot | 0x00430070 | 2 | 552 | pad |
| CEngineApp#2 | vm1 | Render_InitUnitQuad | CEngineApp__StartGame | 0x004302A0 | 4 | 604 | pad |
| Automap | vm1 | Automap_ClearBuffers | Automap_ClearBuffers | 0x00430500 | 1 | 680 | pad |
