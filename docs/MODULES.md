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

**Left: 75 directories** (`python3 tools/merge_check.py`, `COMPILE 30 REFUSED 45`) (round 25: part n2 merged 5 of them and found the real cause of the `.rodata`, flags and name-clash rows: next section):

| Blocker | Directories | What it takes |
|---|---:|---|
| declarations differ, local types differ | 30 | more header work as above. What remains is harder: field symbols read as arrays (`gEngineApp_m_Settings_match_tag[]`, `gEngineApp_m_Settings_unk1C8[]`, `gThrowableCollisionInfo_m_InstanceBehavior[]`, `__osContNumControllers[]`: the array view is what keeps the ROM's code, `(&x)[0]` does not, tried on `vm1/StatsMenu`), return types the callers' code depends on (`s32` vs `void *` on the host), Rage Wars `Triple` vs `CVector3` by value, the CList/CISet/EnemyAI prototype families spread over many files |
| `.rodata` not one block in `.text` order (data no C file owns, slices interleaved with another directory's or out of order) | 25 | as in the table: attribute the unowned data, or a module boundary that is not one object |
| vm2: functions still in asm between its C files (`.text` not adjacent) | 9 | those functions in C first |
| vm2: files placing `.rodata` over vm1's pool one by one | 5 | one placement per merged object in `shared_rodata.py` |
| the files' `cc1-flags` differ | 4 | two files per module, or the flags proven equal |
| a merged file of the same name exists (`code/CEngineApp` / `vm1/CEngineApp`, `vm1/Demo` / `code/Demo`) | 2 | `--name`; both are refused for `.rodata` order anyway |

## Round 25, part n3: the second VM module (vm2)

The 14 vm2 directories the round-24 table stopped at asm or at the shared pool.

**No vm2 function is asm any more.** The 57 asm subsegments between vm2's C files held
100 functions (`TotemDefense_DrawTotem_2`, `CScene__DecompressCollision_2`, `Primagen_*_2`,
`MotherBoss_*_2`, `BlindOne_*_2`, the `func_` copies in `Demo` and `PakMenu` ...). Every one is
byte for byte its first-module twin (`progress.py` counts such copies once, which is why
nobody wrote them): at the same link address, or 0xE0 below it in `Demo` and `PakMenu`, whose
`j`-free bodies do not care. Each is now its twin's C file with the second module's names
(functions by position in the module, data with a `_2` symbol such as `gMotherSpawnPoints_2`)
and a header line saying whose copy it is; 44 read the first module's pool and have their
twin's `.rodata` address in `versions/shared_rodata.us.txt`. `MotherBoss_AnimEvent_2` went into
`func_00442A64.c` with `MotherBoss_TeleportTarget_2`, as vm1's `func_00409A64.c` holds both. The
generator was a one-off (twin pairs from `versions/modules.us.txt`, `.rodata` addresses from the
link map); `make verify` is what checked it.

**A merged vm2 file places its `.rodata` per original file.** A vm2 module's files read the
first module's pool at different, not adjacent addresses (`CPlayer`: 0x800A6A98, 0x800A6AC4,
0x800A6AC8, 0x800A70D8 ...), so one placement per merged object cannot work. The merged file
keeps one placement per part instead: `versions/shared_rodata.us.txt` names a part by the
merged file and the first function its original file emitted,

    CPlayer_2:CPlayer__LoseLife_2 0x800A6AC8

and the assembler input does the rest (`tools/sn_as.sh`, only for a file with such lines):
`tools/sn_li.py` writes its `li.s` literal pool out before each part's first function (as it did
at the end of each file: without it the pools of all parts landed in the last one), and
`tools/shared_rodata.py --split` moves each `.rodata` chunk into `.rodata.<function>` of its
part: a string written before a function goes with that function, a jump table with the
function it is in, an `sn_li` pool with the function it was flushed after. `make setup` gives
each part its output section (NOLOAD) and `make verify` checks each part against the ROM as it
did each file. `tools/merge_module.py` writes the part lines itself (one file still hands its
line over whole). Strings a TU shares between parts are emitted once, in the first part that
uses them; that holds wherever the parts read the same pool address (the menus' "  %02d",
"none" at 0x800C1318 for three OptionsMenu parts), and `make verify` would catch any other case.
`ASSEMBLER=asn64` does not split (it has no such filter): parts need the default gas route.

**Merged (8 directories, 221 files -> 8):** `vm2/CPlayer`, `vm2/NameEntryMenu`, `vm2/TotemDefense`,
`vm2/OptionsMenu`, `vm2/MissionMenu`, `vm2/CheatMenu`, `vm2/MotherBoss`, `vm2/Pak`. The last six first got one
declaration per name, without touching shared headers: a field symbol the ROM's code reads as
an array is reached through a pointer (`s32 *kills = &gTotemDefense.kills; *kills += 1;`,
`u8 *value = &gEngineApp_m_Settings_unk17;` keep the address-building code that `x[0]` gave,
where the plain member does not), a local struct of a name another file or `rw/abu_types.h` also
defines is that header's or renamed, the CheatMenu files take vm1 `CheatMenu.c`'s declarations,
MotherBoss's local prototypes take `void *` arguments, and the two callers whose arguments
disagree with the module's prototype (`CAI__CallHurt` with three, `ImpactEvent_Send` with
another 8-byte params type) call through a function type of their own (still a direct `jal`;
`asm_diff` 0); in Pak, `Pak_RefreshNotes_2` returns its last `osPfsFreeBlocks_2` as vm1's
`Pak_RefreshNotes` does (its caller tests it) and `rw/abu_protos.h` left the two files that needed
nothing from it.

**Left (6 directories), all header work now, no asm and no pool question:**

| Directory | Files | First conflicts (`tools/merge_check.py`) |
|---|---:|---|
| `vm2/BlindOne` | 70 | `rw/abu_protos.h` against `protos/*.h` (`CCache__*`, `CList__*`), `AI_UpdateTargets`, local struct names |
| `vm2/Demo` | 28 | **merged** (round 26, o1: `CList__Construct` from `protos/list.h` instead of `rw/abu_protos.h`; was: declarations differ) |
| `vm2/MpMenu` | 109 | field symbols read as arrays (`gEngineApp_m_Settings_match[]`, `_unk16[]`, `_unk18[]`, `_match_teamScore`), `Controller_ConfirmPressed` in five forms, `MenuPool_OpenChildMenu_2`, `Menu_StepValue_2`, `sprintf`, local struct names; round 26 (o2): split off `vm2/TitleMenu` |
| `vm2/PakMenu` | 139 | `osCreateMesgQueue` and the libultra prototypes, `CCache__*`, local struct names (87 messages on the first pass); round 26 (o2): split off `vm2/Video` |
| `vm2/Primagen` | 53 | the `gLevelScript` field-symbol views (`ModeState0040457C`, `LevelScriptF950` against `LevelScriptState`), a local type used before a file defines it |
| `vm2/SceneLoad` | 23 | the `CList`/`CCache`/`CScene` prototype families (`rw/abu_protos.h` vs `protos/*.h`), `ProPackDecompress` in four forms, local struct names (72 messages) |

These are the families part n1 works on in vm1 (`CList`/`CISet`/`CCache`, the field-symbol views,
`gLevelScript`); the vm2 copies can follow once vm1's declarations are one. None needs a ROM change.

## Round 25, part n1: the header-work directories

Of the 30 directories `merge_check.py` stopped at the compiler ("declarations differ, local types
differ"), 17 are merged (one C file each; `make verify` OK, `asm_diff` 0): `code/{CEmitter,
CISet,Cinema,Controller,Destructible,EnemyHover,EnemySwim,Render,Rumble,Sound,SpeechPlayer,
Texture,Throwable}`, `code/libultra/{io/controller,audio/drvrnew}`, `vm1/StatsMenu`,
`vm2/StatsMenu`. **75 modules are one C file; 58 directories left.** What it took, by kind:

- *Field symbols read as arrays.* Where every user's code is the same with the array view, the
  header now declares the array and the scalar users read `[0]`: `__osContNumControllers[]`
  (globals/libultra.h), `gEngineApp_m_Settings_match_tag[]` / `_unk1C8[]` (globals/game.h),
  `gThrowableCollisionInfo_m_InstanceBehavior[]` (globals/collision.h). Where it is not (the
  array view changes `SavedPlayers_Arm`: GCC 2 then keeps the address in a register for two
  reads), the header keeps both views behind a switch: `gScene_m_LevelID` / `gScene_m_nLevel`
  are arrays under `SCENE_LEVEL_ARRAYS` (globals/scene.h), which the Cinema module's own header
  `cinema_scene.h` defines. A struct member read (`gScene.m_LevelID`) or `(&x)[0]` does not give
  Cinema_Start's code (tried: GCC bases the loads on gScene, or reloads after the stores).
- *Host-dependent return types.* `Controller_IsPakWatched` now returns s32 (the u8 flag,
  zero-extended by its `lbu`): one prototype for both targets, N64 code unchanged (a u8
  prototype adds an `andi` at each N64 caller; before this, the merged Controller.c had it
  implicitly declared). `EnemyAI_GetBehaviorIndex` cannot: its definition's s8 makes GCC
  sign-extend at the callers, so `include/protos/enemy.h` keeps `#ifdef TARGET_PORT` s8 / s32
  in the one header.
- *Triple vs CVector3 by value*: the callers pass the CVector3 member of their unions
  (`pos.v`), the same 12 bytes (Destructible).
- *Prototype families.* New headers: `protos/enemy.h` (AI_UpdateTargets,
  AI_ComputeFrameMotion, AI_LerpByAggression, AI_IsAimedAt, EnemyAI_GetBehaviorIndex: void *
  for the actor/AI/output so every caller's view fits), `protos/mtxf.h` (CMtxF__SetRotateY,
  CMtxF__TransformPoint); fixed: `protos/ai.h` CAI__CallHurt takes its three arguments,
  `protos/iset.h` CUSet__GetTotalSize / CUSet__Transpose on struct tags, `rw/abu_protos.h`
  Controller_IsPakWatched. Pointer-only differences are free on the N64; an s16 parameter, a
  narrower return or a float argument are not, and were kept as the definition has them.
- *An extra argument*: `Model_BlendMorphFrames` takes the third argument its one caller passes
  (unused).
- *The port*: a merged file goes through clang too, which rejects what GCC 2.8 lets pass
  (a later declaration that disagrees with the definition in pointer types: Throwable.c,
  Rumble's block-scope externs); `merge_check.py` does not see those. The functions the port
  replaces are under `#ifndef TARGET_PORT` (osContInit, Sound__Initialize, Sound__IsPlaying,
  six SpeechPlayer functions, alSaveNew) and left `port/cmake/link.cmake`.

Left of the 30: 13, each in the table with what stops it. The pattern is the same everywhere
(one prototype per function, in a header where the family is shared), but the remaining ones
are the big imported Rage Wars files with their own basic typedefs and struct views
(`code/{EnemyAI,GameObject,SndQueue,libultra/audio/cseq}`), the CCache / CScene prototype
families (`code/{CCache,CGameRegion}`, `vm1/BlindOne`), and two that need a structural change
first (`Generator`'s host rename macro, `__scHandleRSP`'s sched.h views). None needs a ROM
change; they need more time.

### The dry-run table (round 24; merged rows updated after the merges)

| Directory | Files | Result of the dry run (round 24), or merged |
|---|---|---|
| `code/libultra/audio/cseq` | 12 | **merged** (round 26, o1: `__getTrackByte`/`__readVarLen`/`__alCSeqGetTrackEvent` on `ALCSeq`/`ALEvent`, the Rage Wars view of `__alCSeqGetTrackEvent` written with libaudio's field names; the port's two under `#ifndef TARGET_PORT`; was: declarations differ) |
| `code/libultra/audio/drvrnew` | 8 | **merged** (round 25 n1, after unifying its declarations; was: declarations differ (3: `FuncPtr`, `alFilterNew`, `alHeapDBAlloc`)) |
| `code/libultra/audio/env` | 7 | **merged** (round 24, first batch) |
| `code/libultra/audio/resample` | 2 | **merged** (round 24, first batch) |
| `code/libultra/audio/reverb` | 8 | **merged** (round 24, first batch) |
| `code/libultra/audio/save` | 2 | **merged** (round 24, first batch) |
| `code/libultra/audio/seq` | 12 | **merged** (round 24, first batch) |
| `code/libultra/audio/sndplayer` | 5 | **merged** (round 24, first batch) |
| `code/libultra/audio/synthesizer` | 9 | **merged** (round 25, n2: flags proven, three functions `DEFERRED`; was: .rodata not in .text order) |
| `code/libultra/gu/lookat` | 2 | **merged** (round 25, n2: `-fforce-addr` proven; was: cc1 flags differ) |
| `code/libultra/gu/perspective` | 2 | **merged** (round 25, n2: `-fforce-addr` proven; was: cc1 flags differ) |
| `code/libultra/io/controller` | 3 | **merged** (round 25 n1, after unifying its declarations; was: declarations differ (2: `__osContBuffer`, `__osContNumControllers`)) |
| `code/libultra/io/piacs` | 3 | **merged** (round 24, first batch) |
| `code/libultra/io/siacs` | 3 | **merged** (round 24, first batch) |
| `code/libultra/io/sptask` | 3 | **merged** (round 24, after unifying its declarations; was: declarations differ (2: `__osSpSetStatus`, `osSpTaskStartGo`)) |
| `code/libultra/io/vimgr` | 2 | **merged** (round 24, first batch) |
| `code/libultra/libc/xprintf` | 2 | **merged** (round 24, first batch) |
| `code/libultra/os/timerintr` | 4 | **merged** (round 24, first batch) |
| `code/AI` | 48 | .rodata interrupted by data no C file owns (1: `unnamed@0xA7478`); round 25 (n2): linker padding before `func_00241E30` or `func_002427E0`: two objects; also declarations differ; round 26 (o2): split at `AI_InitHealth` (`PlatformCollide` merged, `AI` header work) |
| `code/Action` | 17 | **merged** (round 24, after unifying its declarations; was: declarations differ (1: `Action_Draw`)) |
| `code/Anim` | 49 | .rodata interrupted by data no C file owns (1: `unnamed@0xA64F4`); round 25 (n2): `D_800A58F4` (a float the C reads as an extern); also declarations differ |
| `code/BinaryRange` | 13 | **merged** (round 24, first batch) |
| `code/Boot` | 4 | **merged** (round 24, after unifying its declarations; was: declarations differ (1: `gSavedPlayers`)) |
| `code/CAudioWorld` | 101 | .rodata interrupted by data no C file owns (3: `unnamed@0xA8C94`); round 25 (n2): linker padding three times: four objects; also declarations differ; round 26 (o2): split into `Frustum` (merged), `AudioMgr`, `CAudioWorld`, `SndVoice` |
| `code/CCache` | 56 | calls typed differently (2: `CCache__AllocateInternal`, `CCache__WaitForDisplayLists`); declarations differ (18: `CCache__DeallocateInternal`, `CCache__DetachCacheEntry`, `CCache__FreeCacheEntry`, `CCache__FreeMemEntry`, ...); local types differ (3: `B12Bucket`, `B12Mgr`, `B12Node`); other (1: `request for member `m_pHead' in something not a structure or`). Round 25 n1: 35+ conflicts: its own B12Node/B12Bucket/B12Mgr views against structs/cache.h, osSendMesg, CHeap__Alloc/Free; the cache manager's real struct first (part n4's D_800D7BE0 work) |
| `code/CCamera` | 66 | .rodata interleaved with another directory's (2 subsegments between, e.g. `func_0042F9B0`); round 25 (n2): interleaved with `vm1/Demo`: one object over the engine and the VM; also declarations differ |
| `code/CCollisionSetup` | 3 | **merged** (round 24, first batch) |
| `code/CEmitter` | 10 | **merged** (round 25 n1, after unifying its declarations; was: declarations differ (4: `CList__AddHead`, `CList__Remove`, `Emitter_CompareHeightAsc`, `Sortable`)) |
| `code/CEngineApp` | 90 | .rodata interrupted by data no C file owns (1: `unnamed@0xAA9FC`); round 25 (n2): `60.0f` no file owns (not padding); also `--name` |
| `code/CGameObjectInstance` | 64 | .rodata interrupted by data no C file owns (2: `unnamed@0xA66C4`); round 25 (n2): `sCGameObjectInstanceCacheDesc_AnimObjectInfo` and a `1.0f` read as externs; also declarations differ |
| `code/CGameRegion` | 53 | calls typed differently (1: `incompatible types in initialization`); declarations differ (24: `CGameRegion__FloodClearFlag4`, `CGameRegion__FloodSetFlag4`, `CGameRegion__FloodSetHeight`, `CGameRegion__GetCeilingHeight`, ...); local types differ (2: `CollisionBuildVertex`, `Node75`); parse errors (6: `&`, `.`, `if`, `return`, ...); other (4: `initializer element is not constant`, `request for member `x' in something not a structure or union`, `request for member `y' in something not a structure or union`, `request for member `z' in something not a structure or union`). Round 25 n1: 47+ conflicts: CISet__GetBlock and the CScene__ prototype family against protos/scene.h (pointer types and host returns) |
| `code/CISet` | 14 | **merged** (round 25 n1, after unifying its declarations; was: declarations differ (3: `CUSet__GetTotalSize`, `CUSet__Transpose`, `osCreateMesgQueue`); local types differ (2: `CMutex`, `OffsetTable`)) |
| `code/CInstancePool` | 7 | **merged** (round 27, p3: split into `CLevelActorPool`, `CInstancePool`, `DLightsLoad` at two `.rodata` pads; its strings as literals; the stale bytes off 4 by `tools/ld_fill.py`; was: `sCInstancePoolCacheDesc_*` strings read as externs) |
| `code/CLevelActorPool` | 9 | **merged** (round 27, p3: split off `CInstancePool`) |
| `code/DLightsLoad` | 2 | **merged** (round 27, p3: split off `CInstancePool`) |
| `code/CLevelActor` | 9 | **merged** (round 27, p3: header work, `--inline func_00215944`, the two `DEFERRED` functions moved to where the ROM has their strings; was: .rodata not in .text order) |
| `code/CMtxF` | 124 | .rodata interrupted by data no C file owns (3: `unnamed@0xA6004`); round 25 (n2): `sCStaticObjectCacheDesc_*` strings and `0, 1.0, 0` read as externs; also declarations differ |
| `code/CParticle` | 212 | .rodata interrupted by data no C file owns (8: `unnamed@0xA7004`); round 25 (n2): linker padding six times; `D_800A652C` and a float table read as externs; also declarations differ; round 26 (o2): split into `CParticle`, `Trail`, `ImpactEvent`, `Path` (merged), `SweepShapes`, `ModelSweep`, `Pickup` |
| `code/CPlayer` | 245 | .rodata interleaved with another directory's (7 subsegments between, e.g. `func_0042DA60`); round 25 (n2): interleaved with `vm1/CPlayer`: one object over the engine and the VM; also declarations differ |
| `code/CScene` | 96 | .rodata interleaved with another directory's (7 subsegments between, e.g. `func_0042AA80`); round 25 (n2): interleaved with `vm1/SceneLoad`: one object over the engine and the VM; also declarations differ |
| `code/Cinema` | 34 | **merged** (round 25 n1, after unifying its declarations; was: declarations differ (5: `Sound__StopAllExcept`, `gScene`, `gScene_m_LevelID`, `gScene_m_WarpSpawn`, ...)) |
| `code/Color` | 3 | **merged** (round 24, first batch) |
| `code/Controller` | 37 | **merged** (round 25 n1, after unifying its declarations; was: declarations differ (3: `Controller_IsPakWatched`, `RumblePool_Init`, `osMotorStop`)) |
| `code/Crash` | 16 | **merged** (round 27, p3: its `.data` in C, inside `code_rodata` by `tools/ld_fill.py`; `gCrashCauseDesc`'s names its first `.rodata`; was: .rodata starts with the strings of its `.data` tables) |
| `code/DLights` | 6 | **merged** (round 24, after unifying its declarations; was: local types differ (1: `ListNode`)) |
| `code/Demo` | 3 | **merged** (round 24, first batch) |
| `code/Destructible` | 15 | **merged** (round 25 n1, after unifying its declarations; was: declarations differ (3: `AI_DropPickupsOnce`, `Actor_SpawnParticle`, `Sound__Play`)) |
| `code/Door` | 16 | **merged** (round 24, after unifying its declarations; was: declarations differ (1: `ImpactEvent_Call`); local types differ (1: `ResourceManagerState`)) |
| `code/EnemyAI` | 119 | own basic typedefs (3: `s32`, `s8`, `u16`); calls typed differently (1: `EnemyAI_CheckOffscreen`); declarations differ (50: `AI_AngleToPoint`, `AI_IsAimedAt`, `AI_StopSound`, `AI_TurnClamped`, ...); local types differ (1: `Motion`); parse errors (1: `*`). Round 25 n1: 69+ conflicts: own basic typedefs, the AI helper family (now protos/enemy.h: the callers here still declare their own), CGameObjectInstance__PickRandomAnim, gEngineApp_m_Settings_multiplayer, EnemyAI_GetBehaviorIndex's definition (s8) against its N64 callers (s32) |
| `code/EnemyAnim` | 76 | **merged** (round 24, after unifying its declarations; was: declarations differ (1: `WeightedTable_Clear`)) |
| `code/EnemyHover` | 9 | **merged** (round 25 n1, after unifying its declarations; was: declarations differ (6: `AI_IsAimedAt`, `AI_LerpByAggression`, `AI_UpdateTargets`, `CMtxF__SetRotateY`, ...)) |
| `code/EnemySwim` | 19 | **merged** (round 25 n1, after unifying its declarations; was: declarations differ (5: `AI_ComputeFrameMotion`, `AI_UpdateTargets`, `CAI__CallHurt`, `CAI__SetMode`, ...)) |
| `code/GameObject` | 62 | own basic typedefs (3: `s32`, `u32`, `u8`); calls typed differently (2: `GameObject_GetGroundHeight`, `GameObject_TraceMove`); declarations differ (5: `CGameRegion__GetHeight`, `CScene__GetObjectTypeID`, `CVector3__Normalize`, `GameObject_GetFrameDelta`, ...); local types differ (2: `CGameRegion`, `Triple`); parse errors (1: `}`); other (1: `structure has no member named `m_pVertices'`). Round 25 n1: 16+ conflicts: own basic typedefs (imported Rage Wars files), CVector3__Normalize, GameObject_TraceMove / GetGroundHeight called with different argument lists |
| `code/GameObjectMove` | 6 | **merged** (round 24, after unifying its declarations; was: declarations differ (1: `Collision_MoveInstance`)) |
| `code/Generator` | 16 | **merged** (round 26, o1: `B01Info` in `structs/generator.h`; the spawn prototypes in the MotherBoss form; `Generator_SpawnObject`/`SpawnPickup` typed as the twins of `spawnObject`/`spawnPickup`; `CScene__GetObjectTypeID` through an `s32` type of its own on N64; was: declarations differ) |
| `code/HostIo` | 34 | **merged** (round 25, n2: `-msplit-addresses` proven for all 34; was: cc1 flags differ) |
| `code/Hud` | 48 | declarations differ (5: `Draw`, `ModeState`, `gEngineApp_m_OnScreen_m_FontXScale`, `gEngineApp_m_OnScreen_m_ShadowXOff`, ...); local types differ (2: `Chunk`, `FontStyle`); parse errors (12: `(Each undeclared identifier is reported `, `*`, `FrameEntry`, `byte`, ...); other (1: `for each function it appears in.)`). Round 25 n1: 21+ conflicts: gEngineApp_m_OnScreen_m_* field symbols read as arrays, `Draw`/`ModeState` names used as both type and function |
| `code/InteractiveAnim` | 15 | **merged** (round 24, after unifying its declarations; was: declarations differ (3: `AI_PlayAnim`, `CGameObjectInstance__AdvanceAnimation`, `CGameObjectInstance__SetAnimByAction`)) |
| `code/LevelEvent` | 31 | .rodata interrupted by data no C file owns (1: `unnamed@0xA8C3C`); round 25 (n2): linker padding before `func_0026DFB0`: two objects; also declarations differ; round 26 (o2): split into `DList` and `LevelEvent` at `LevelEvent_Fire` |
| `code/LoopSounds` | 10 | **merged** (round 24, after unifying its declarations; was: declarations differ (3: `CList__AddTail`, `CList__Remove`, `Sound__Stop`)) |
| `code/Platform` | 27 | **merged** (round 24, after unifying its declarations; was: declarations differ (2: `Platform_Step`, `StateInfo`)) |
| `code/Render` | 28 | **merged** (round 25 n1, after unifying its declarations; was: calls typed differently (1: `Model_BlendMorphFrames`); declarations differ (2: `CCache__LockForDisplayList`, `CCache__Unlock`); local types differ (2: `Material`, `Vtx`)) |
| `code/Rumble` | 10 | **merged** (round 25 n1, after unifying its declarations; was: declarations differ (3: `RumbleCurve_Advance`, `__osDisableInt`, `__osRestoreInt`); local types differ (1: `Clip`)) |
| `code/RumbleCurve` | 3 | **merged** (round 24, first batch) |
| `code/SavedPlayers` | 8 | **merged** (round 24, after unifying its declarations; was: declarations differ (1: `gSavedPlayers`)) |
| `code/SndQueue` | 12 | **merged** (round 26, o1: `SndQueue_InitNode` typed on `SndNode`, the N64 `SndQueue_Construct` view from `rw/abu_types.h`, `SndVoice_Construct` as defined; the port's four under `#ifndef TARGET_PORT`; was: declarations differ) |
| `code/SndSeq` | 12 | **merged** (round 26, o1: the libaudio calls through `protos/libaudio.h` with ultralib's prototypes, which compile the ROM's callers; the port's ten under `#ifndef TARGET_PORT`; was: declarations differ) |
| `code/Sort` | 2 | **merged** (round 24, first batch) |
| `code/Sound` | 31 | **merged** (round 25 n1, after unifying its declarations; was: declarations differ (2: `CAudioWorld__SetForce2D`, `gEngineApp_m_Settings_unk1C8`)) |
| `code/SpeechPlayer` | 7 | **merged** (round 25 n1, after unifying its declarations; was: declarations differ (5: `CAudioWorld__GetSndPlayer`, `CAudioWorld__Lock`, `CAudioWorld__Unlock`, `alSndpSetSound`, ...)) |
| `code/Sweep` | 6 | **merged** (round 24, after unifying its declarations; was: local types differ (1: `HitInst`)) |
| `code/Texture` | 8 | **merged** (round 25 n1, after unifying its declarations; was: declarations differ (3: `CISet__GetBlock`, `CISet__GetBlockAndSize`, `Pair`)) |
| `code/Throwable` | 13 | **merged** (round 25 n1, after unifying its declarations; was: declarations differ (3: `CAI__CallHurt`, `OwnedState`, `gThrowableCollisionInfo_m_InstanceBehavior`)) |
| `code/Turret` | 15 | **merged** (round 24, after unifying its declarations; was: calls typed differently (1: `Turret_EventShow`); declarations differ (1: `Turret_Effects`)) |
| `code/VM` | 20 | .rodata not in .text order (1 inverted pairs of 2 subsegments); round 25 (n2): deferred from `func_0028E068` on (`--inline`); also declarations differ |
| `code/WeaponMode` | 49 | declarations differ (13: `Anim`, `Bone`, `CAI__SetMode`, `CCameraPool__AddText`, ...). Round 25 n1: 21+ conflicts: Anim/Bone views, Sound__Play, CAI__SetMode, the CPlayer__ weapon family |
| `code/__scHandleRSP` | 17 | declarations differ (2: `Sched`, `__scHandleRetrace`); local types differ (1: `Stream`); parse errors (7: `(Each undeclared identifier is reported `, `*`, `Qn`, `Stream`, ...); other (1: `for each function it appears in.)`). Round 25 n1: 19+ conflicts: Sched/ScTask/Client views of sched.h's OSSched/OSScTask/OSScClient (parse errors once they meet), __scHandleRetrace |
| `code/adecHuffDecode` | 18 | **merged** (round 26, o1: `func_002A0044` re-matched under `-fforce-addr`, merged with `--cc1-flags auto`; `AdecStream` in `structs/adec.h`; was: cc1 flags differ) |
| `code/alAdpcmPull` | 5 | **merged** (round 24, first batch) |
| `vm1/BlindOne` | 70 | declarations differ (26: `AI_UpdateTargets`, `Animator_Update`, `CCache__Allocate`, `CCache__AllocateMemoryForDecompression`, ...); parse errors (1: `DrawInfo`). Round 25 n1: 28+ conflicts: the CCache__ family (protos/cache.h), gScene_m_LevelID, DrawInfo |
| `vm1/CEngineApp` | 4 | **merged** (round 24, first batch) |
| `vm1/COnScreen` | 3 | **merged** (round 24, first batch) |
| `vm1/CPlayer` | 13 | .rodata interleaved with another directory's (39 subsegments between, e.g. `func_00248340`); round 25 (n2): interleaved with `code/CPlayer` (compiles merged otherwise) |
| `vm1/CheatMenu` | 48 | **merged** (round 24, after unifying its declarations; was: declarations differ (2: `Controller_MenuLeft`, `Controller_MenuRight`)) |
| `vm1/Cont` | 2 | **merged** (round 24, after unifying its declarations; was: declarations differ (1: `RumblePool_Init`)) |
| `vm1/ContinueMenu` | 2 | **merged** (round 24, first batch) |
| `vm1/Demo` | 28 | .rodata interleaved with another directory's (6 subsegments between, e.g. `func_0027C074`); round 25 (n2): interleaved with `code/CCamera`; also `--name` |
| `vm1/GameOverMenu` | 3 | **merged** (round 24, first batch) |
| `vm1/MissionMenu` | 14 | .rodata interrupted by data no C file owns (1: `unnamed@0xC2368`); round 25 (n2): linker padding before `func_00424B90`; also declarations differ; round 26 (o2): split off `Mission` (`Mission_LevelObjectivesDone`) |
| `vm1/MotherBoss` | 65 | **merged** (round 25 merge, after unifying its declarations as its vm2 twin's; was: declarations differ (6: `AI_UpdateTargets`, `Actor_SpawnParticle`, `CAI__CallHurt`, `CLevelActorPool__Spawn`, ...)) |
| `vm1/MpMenu` | 110 | .rodata interrupted by data no C file owns (3: `unnamed@0xC1A78`); round 25 (n2): linker padding before one of ten starts; `sMpMenuFmt_3d` read as an extern; also declarations differ; round 26 (o2): split off `TitleMenu` at `SingleMenu_Init` |
| `vm1/NameEntryMenu` | 8 | .rodata interrupted by data no C file owns (1: `unnamed@0xC1F4C`); round 25 (n2): the strings of its key table in `.data` (asm): the table in C first (compiles merged otherwise; round 25 merge: merged with `--rodata-free`, `make verify` fails, the strings are not in the C); **merged** (round 26 o2, with its key table as C `.data`) |
| `vm1/OptionsMenu` | 31 | **merged** (round 25, n2: every function `DEFERRED`; was: .rodata not in .text order) |
| `vm1/Pak` | 42 | .rodata not in .text order (1 inverted pairs of 3 subsegments); round 25 (n2): deferred from index 2..6 on, index 7 defined before 6; also declarations differ |
| `vm1/PakMenu` | 140 | .rodata interrupted by data no C file owns (2: `unnamed@0xC2008`); round 25 (n2): linker padding (two places); `sPakMenuText_Spaces3` read as an extern; also declarations differ; round 26 (o2): split off `Video` at `Video_ApplyResolution`; round 26 merge: the split-off `Video` merged |
| `vm1/Primagen` | 115 | .rodata interrupted by data no C file owns (2: `unnamed@0xC0F14`); round 25 (n2): linker padding twice; also declarations differ; round 26 (o2): split into `AutomapDraw`, `MantidQueen`, `Primagen`; round 26 merge: the split-off `AutomapDraw` merged |
| `vm1/SceneActorPool` | 2 | **merged** (round 24, first batch) |
| `vm1/SceneLoad` | 24 | .rodata interleaved with another directory's (15 subsegments between, e.g. `func_00220530`); round 25 (n2): interleaved with `code/CScene`; also declarations differ |
| `vm1/StatsMenu` | 15 | **merged** (round 25 n1, after unifying its declarations; was: declarations differ (1: `gEngineApp_m_Settings_match_tag`)) |
| `vm1/TotemDefense` | 18 | .rodata not in .text order (8 inverted pairs of 9 subsegments); round 25 (n2): every function deferred (`--inline all`); also declarations differ |
| `vm2/BlindOne` | 60 | its .text subsegments are not adjacent; round 25: no asm left (70 files), declarations differ (n3 table above) |
| `vm2/CEngineApp` | 4 | **merged** (round 24, first batch) |
| `vm2/COnScreen` | 2 | **merged** (round 24, first batch) |
| `vm2/CPlayer` | 13 | **merged** (round 25, n3: its 6 pool placements are parts of `CPlayer_2.c`) |
| `vm2/CheatMenu` | 48 | **merged** (round 25, n3: declarations as vm1's `CheatMenu.c`, 7 parts) |
| `vm2/Cont` | 2 | **merged** (round 24, after unifying its declarations; was: declarations differ (1: `RumblePool_Init`)) |
| `vm2/ContinueMenu` | 2 | **merged** (round 24, first batch) |
| `vm2/Demo` | 21 | its .text subsegments are not adjacent; round 25: no asm left (28 files), declarations differ (n3 table above) |
| `vm2/GameOverMenu` | 3 | **merged** (round 24, first batch) |
| `vm2/MissionMenu` | 14 | **merged** (round 25, n3: one declaration per name, 7 parts) |
| `vm2/MotherBoss` | 30 | **merged** (round 25, n3: its asm in C, one declaration per name, 19 parts) |
| `vm2/MpMenu` | 106 | its .text subsegments are not adjacent; round 25: no asm left (109 files), declarations differ (n3 table above); round 26 (o2): split off `vm2/TitleMenu` |
| `vm2/NameEntryMenu` | 8 | **merged** (round 25, n3: 2 parts) |
| `vm2/OptionsMenu` | 31 | **merged** (round 25, n3: one declaration per name, 6 parts) |
| `vm2/Pak` | 41 | **merged** (round 25, n3: its asm in C, one declaration per name, 3 parts) |
| `vm2/PakMenu` | 133 | its .text subsegments are not adjacent; round 25: no asm left (139 files), declarations differ (n3 table above); round 26 (o2): split off `vm2/Video`; round 26 merge: the split-off `Video` merged |
| `vm2/Primagen` | 35 | its .text subsegments are not adjacent; round 25: no asm left (53 files), declarations differ (n3 table above) |
| `vm2/SceneActorPool` | 2 | **merged** (round 24, first batch) |
| `vm2/SceneLoad` | 19 | its .text subsegments are not adjacent; round 25: no asm left (23 files), declarations differ (n3 table above) |
| `vm2/StatsMenu` | 15 | **merged** (round 25 n1, after unifying its declarations; was: declarations differ (2: `CPlayerPool__GetIndex`, `gEngineApp_m_Settings_match_tag`); local types differ (1: `FieldRecAt14`)) |
| `vm2/TotemDefense` | 7 | **merged** (round 25, n3: its asm in C, 4 parts) |

## Round 25, part n2: `.rodata` order, unowned data, cc1 flags

**How SN's toolchain fills an object's `.rodata`** (small test files through `tools/sn64/cc1`, the
original `cc1n64.exe` and `asn64.exe` under wine; same results from both compilers):

- cc1 writes a string literal into `.rodata` when it expands the function that first uses it in the
  file (once per file: a second use shares it), i.e. at the function's definition, before its code. A
  file-scope initializer's strings go out where the variable is output.
- asn64 puts each `li.s`/`li.d` constant into `.rodata` at the instruction, in code order, never
  pooled (two `0.5f` in two functions are two words); cc1 writes a jump table at its `switch`.
  `tools/sn_li.py` flushes its pool at cc1's next section directive, which gives the same order.
- A function is output (code, literals, jump tables) at its definition, **except an `inline` one**:
  GCC 2.8.1 at `-O2` keeps every `inline` function, public ones too, until the end of the file and
  outputs them after all the others, in the order of their first declaration; their strings are
  already out, at the definition. At `-O3` (`-finline-functions`) the same happens to `static`
  functions defined after their callers. The original sources are C++ (`MATCHING.md`): a member
  function defined in its class body is `inline`.

So a module whose `.rodata` has a later function's strings before an earlier function's float
literals is one object with deferred functions, not a wrong boundary. ultralib's `synthesizer.c`
proves it: in its own order (`_timeToSamplesNoRound` static, `_timeToSamples`, `__nextSampleTime`
static) at `-O3` cc1 gives exactly the ROM's `.text` order (`_timeToSamples`, `__nextSampleTime`,
`_timeToSamplesNoRound`) and `.rodata` order (`"EX"`, `"audio/synthesizer.c"`, then the two
functions' literals).

**`merge_module.py` does it:** `--inline all|stem,..` writes those files' functions `DEFERRED`
(`include/common.h`: `inline` on the N64 build, nothing on the port's, where C99 `inline` would emit
no definition) and declares the deferred functions before them in `.text` order wherever one is
declared early (output order is first-declaration order); `--rodata-free` takes the slices and the
unnamed data between them as the merged object's one `.rodata` (another file's slice in between still
refuses); `make verify` decides. `--cc1-flags auto` (or a given set) picks one flag set under which
every file of the module compiles to the same assembly as under its own, else refuses.

**Merged here (5 directories, 74 files -> 5), `make verify` OK:**

| Directory | What it took |
|---|---|
| `vm1/OptionsMenu` (31) | every function `DEFERRED` (`--inline all`): the ROM has the strings of `OptionsMenu_ControlSideText`/`OptionsMenu_StickHorzText` before `GameSettings_SetDefaults`' `1.0f` |
| `code/libultra/audio/synthesizer` (9) | `--cc1-flags auto` (the game's flags compile all nine the same, also the one with `-msplit-addresses`); `_timeToSamples`, `__nextSampleTime`, `_timeToSamplesNoRound` `DEFERRED`; one `RateSource`; the port's five functions under `#ifndef TARGET_PORT` |
| `code/HostIo` (34) | `-msplit-addresses` compiles all 34 the same; one `HostIo_GetSR`/`SetSR`; `HostIo_ReadByte`'s `u32` read as a cast (`srlv`); `HostIo_Call` under `#ifndef TARGET_PORT` (was `T2_GAME_EXCLUDE`) |
| `code/libultra/gu/lookat`, `gu/perspective` (2 + 2) | `-fforce-addr` compiles both files the same; ultralib's float matrix renamed `GuMatrix` (not `rw/abu_types.h`'s `struct Matrix`) |

```
python3 tools/merge_module.py src/us/vm1/OptionsMenu --rodata-free --inline all
python3 tools/merge_module.py src/us/code/libultra/audio/synthesizer --cc1-flags auto --rodata-free \
    --inline func_00294C30,func_00294C70,func_00294D18
python3 tools/merge_module.py src/us/code/HostIo --cc1-flags auto        # also gu/lookat, gu/perspective
```

**The rest of the 31 directories, by real cause.** `merge_check.py --rodata-free` shows that 21 of
them also have the "declarations differ" problems of part n1 behind the `.rodata` refusal (only
`vm1/CPlayer` and `vm1/NameEntryMenu` compile merged), so every row below needs that header work too.

*Out of order: deferred functions* (model above; each needs its header work, then `--inline`):

| Directory | `DEFERRED` set the ROM implies |
|---|---|
| `code/CLevelActor` | `func_00215944` (`CLevelActor__LoadGeometry`/`TouchGeometry`): its strings lie between `func_002154DC`'s and `func_002156FC`'s literals. Compile: `CScene__GetObjectTypeID` `u16`/`s32`, `GameObject_GetGroundRotation` by value/out pointer, `CMtxF__*` `f32 *`/`CMtxF *` |
| `code/VM` | `func_0028E068` (index 4) and everything after it: `func_0028E60C`'s `"Virtual Region"` precedes `func_0028E068`'s literals. Compile: the `gPager_*` views, `B09SlotA`, `func_0028EE40` |
| `vm1/TotemDefense` | all (strings of indices 4 and 7 before the literals of 0..3) |
| `vm1/Pak` | the functions from index 2..6 on; `func_00426164`'s strings come after `func_004263DC`'s, so index 7 is defined before index 6 (declared in `.text` order): needs a source-order option in `merge_module.py` |

*Unowned data between the slices* (`unnamed@...` in the yaml; `python3 tools/rodata_gaps.py <dir>` classifies them) is of two kinds:

- **Linker padding** (stale buffer words, often instructions, before a slice on a 16-byte boundary):
  the next slice is the first of *another object*; `.text` cannot show it because the object before
  ended on 16. The module directory is two objects there, and `versions/modules.us.txt` needs the
  boundary (a new evidence kind for `tools/modules.py`, then the files moved): `code/LevelEvent`
  before `func_0026DFB0` (`LevelEvent_Fire`; the only 16-aligned start in range), `code/CParticle`
  before `func_002333C0`, `func_00239F00`, `func_0023D050` (unique), and ranges in `code/AI`
  (`func_00241E30` or `func_002427E0`), `code/CAudioWorld` (three), `code/CParticle` (three more),
  `vm1/MissionMenu` (`func_00424B90`), `vm1/MpMenu`, `vm1/PakMenu`, `vm1/Primagen` (two). A VM split
  must be made in `vm2` too (`modules.py` pairs the two copies; part n3's directories).
- **The module's own data that its C reads as externs**: named strings (`sCrashText_Spaces3`,
  `sCGameObjectInstanceCacheDesc_AnimObjectInfo`, `sCInstancePoolCacheDesc_*`,
  `sCStaticObjectCacheDesc_*` in `code/CMtxF`, `sMpMenuFmt_3d`, `sPakMenuText_Spaces3`), float words
  (`D_800A58F4` in `code/Anim`, `D_800A652C` in `code/CParticle`, `42700000` in `code/CEngineApp`,
  `0, 1.0, 0` in `code/CMtxF`: no 16-aligned start in range, so not padding) and the strings of
  tables in `.data` (`vm1/NameEntryMenu`: 37 one-character strings, the key table of its menu in
  `virtual_rodata_0.data.s`). A merged file must write them as literals (one copy per object: a
  string two files share can only be given back in the merged file), or define the `.data` table in
  C at its place in the source. `code/Crash` shows the size of it: its object starts at
  `0x800AA070` with the flag-name strings of its register tables (`"IP7"`, `"BD"`, `": "`), so its
  `.data` tables must become C first.

*Interleaved with another directory's* (`code/CPlayer` + `vm1/CPlayer`, `code/CScene` +
`vm1/SceneLoad`, `code/CCamera` + `vm1/Demo`): the VM functions' literals sit between the engine
functions' in one `.rodata` run, in `.text` order of each part. One original object per pair, its
functions linked partly into the engine and partly into the paged VM module: one merged file would
need its VM functions in their own text section (`__attribute__((section(...)))`, linked into the VM
segment) while their `.rodata` stays in the object's. Build work in the linker-script generation,
not attempted.

*cc1 flags:* `code/adecHuffDecode` is the one left: under `-fforce-addr` (the flags of 3 of its 18
files) every file compiles the same except `func_002A0044` (`adecHuffDecode`, a register
allocation difference); its C must be re-matched under `-fforce-addr` first.

*Name clash:* `code/CEngineApp` and `vm1/Demo` merge with `--name`, but `code/CEngineApp` has
unattributed data (`60.0f`) inside its `.rodata` and `vm1/Demo` is interleaved with `code/CCamera`.

None of this needs a ROM change.

## Round 25 merge: the state after parts n1-n4

The four parts merged in the order n3, n2, n1, n4 (`make verify` and `asm_diff --base f3e72718` 0
after each, the port compiled with clang after each). Then `tools/merge_check.py --rodata-free` over
the 45 directories left: one compiled and compared equal (`vm1/NameEntryMenu`, but `make verify`
fails after the merge: its key table's strings are `.data`'s, see its row), 36 stop at the compiler,
8 are refused. `vm1/MotherBoss` was merged after its declarations took the forms its vm2 twin's
merged file has (`AI_UpdateTargets`, `Actor_SpawnParticle`, `WeightedTable_PickStatic`,
`CLevelActorPool__Spawn` on `void *` arguments; `CAI__CallHurt` and `ImpactEvent_Send` through a
function type of their own in the one file each that passes other arguments; `asm_diff` 0 before
the merge, `make verify` after; clang needed the port-only `MotherBoss_Hurt` prototype gone).

**89 modules are one C file** (14 in round 23, 44 in round 24, 31 in round 25: n1 17, n2 5, n3 8,
the merge 1). **44 module directories still hold one function per file**, by what stops them:

| Reason | Directories |
|---|---|
| one original object over the engine and the VM (its VM functions would need a text section of their own in the merged file: linker-script work) | `code/CPlayer` + `vm1/CPlayer`, `code/CScene` + `vm1/SceneLoad`, `code/CCamera` + `vm1/Demo` (6; `vm1/Demo` also needs `--name`) |
| linker padding inside the `.rodata` run: the directory is two or more objects, `versions/modules.us.txt` needs the boundary first (part n2) | `code/AI`, `code/CAudioWorld`, `code/CParticle`, `code/LevelEvent`, `vm1/MissionMenu`, `vm1/MpMenu`, `vm1/PakMenu`, `vm1/Primagen` (8) |
| data the C reads as externs, or strings of `.data` tables in asm (they must become literals or C tables first) | `code/Anim`, `code/CEngineApp` (also `--name`), `code/CGameObjectInstance`, `code/CInstancePool`, `code/CMtxF`, `code/Crash`, `vm1/NameEntryMenu` (7) |
| deferred functions (`--inline`) plus header work | `code/CLevelActor`, `code/VM`, `vm1/TotemDefense`, `vm1/Pak` (4; `vm1/Pak` also needs a source-order option) |
| header work only (one declaration per name; no `.rodata` question) | `code/{CCache, CGameRegion, EnemyAI, GameObject, Hud, WeaponMode, __scHandleRSP}`, `vm1/BlindOne`, `vm2/{BlindOne, MpMenu, PakMenu, Primagen, SceneLoad}` (13 after round 26 o1, which merged `code/{Generator, SndQueue, SndSeq, libultra/audio/cseq}`, `vm2/Demo` and, from the cc1-flags row, `code/adecHuffDecode`) |

The rows above the last need header work too: `merge_check.py --rodata-free` stops at the compiler
for every directory it gets that far with except `vm1/NameEntryMenu`; the 8 it refuses before
compiling are the six interleaved ones, `code/CEngineApp` (its name) and `code/adecHuffDecode`. The fewest
compiler messages are in `code/CInstancePool` and `code/Crash` (5 each, both with data to give to
the C first), `code/libultra/audio/cseq` (the Rage Wars `sptr`/`uptr` typedefs and
`RuntimeState`/`DecodeResult` views), `code/Generator` (`B01Info` twice, the CScene/actor-pool
prototypes) and `vm2/Demo` (the CList family through `rw/abu_protos.h` in one file, `IntPair`,
`Triple`, `CCache__Allocate`/`Deallocate`). None needs a ROM change.

## Round 26, part o2: module boundaries, one data table in C, the engine/VM objects

**Boundaries from `.rodata` padding.** `tools/modules.py` has a new evidence kind, `rodata pad`
(`RODATA_PAD`): the stale non-zero bytes `tools/rodata_gaps.py` classifies as `PADDING` start a new
object at a 16-aligned function between the owners of the two slices. Where there is one candidate
the boundary is as hard as a `.text` pad; where there are several, the one where the name prefix
changes is taken and the note says how many there were (soft; `make verify` cannot tell them apart:
an object that ends on 16 leaves no trace in `.text`). A VM boundary is mirrored into the second copy
(`rodata pad(other copy)`), and module 2 is now paired with module 1 from its first module's twin
(the stale copy starts inside `Primagen`). 16 boundaries, 320 -> 339 modules:

| Was | Now (functions) | Candidates |
|---|---|---|
| `code/CParticle` (216) | `CParticle` (66), `Trail` (45), `ImpactEvent` (45), `Path` (4), `SweepShapes` (30), `ModelSweep` (10), `Pickup` (16) | 1, 3, 2, 1, 1, 2 |
| `code/AI` (48) | `PlatformCollide` (8), `AI` (40) | 2 |
| `code/LevelEvent` (31) | `DList` (12: DList/NodePool/WeightedTable, `GameObject_EmitHotPoints`), `LevelEvent` (19) | 1 |
| `code/CAudioWorld` (101) | `Frustum` (8), `AudioMgr` (7), `CAudioWorld` (45), `SndVoice` (41) | 3, 5, 2 |
| `vm1/Primagen` (115) | `AutomapDraw` (12), `MantidQueen` (43), `Primagen` (60) | 4, 6 |
| `vm1/MpMenu`, `vm2/MpMenu` (110) | `MpMenu` (72), `TitleMenu` (38: Single/Title/Quit/Pause/Sound menus) | 10 |
| `vm1/PakMenu`, `vm2/PakMenu` (140) | `PakMenu` (128), `Video` (12) | 3 |
| `vm1/MissionMenu`, `vm2/MissionMenu` (15) | `Mission` (1: `Mission_LevelObjectivesDone`), `MissionMenu` (14) | 1 (zeros after `" "` up to 16) |

The new names are the dominant prefix, or `RENAMES` where that prefix is another module's
(`SweepShapes`, `ModelSweep`, `PlatformCollide`, `AudioMgr`, `AutomapDraw`, `TitleMenu`; the first
part of the old `LevelEvent` is `DList`, the events keep the name). The files moved with
`tools/movemods.py` (`git mv` into each module's `// dir`, the yaml and comment paths rewritten);
`Mission_LevelObjectivesDone_2` was taken out of the merged `vm2/MissionMenu/MissionMenu_2.c` by
hand (its shared-pool line is the file's again). Every new directory passes the `.rodata` stage of
`merge_check.py --rodata-free` (none is refused); 4 still have module data read as an extern
(`code/CParticle`: an unnamed float table; `code/Trail`: `D_800A652C`; `vm1/MpMenu`: `" "`,
`sMpMenuFmt_3d`; `vm1/PakMenu`: `sPakMenuText_Spaces3`/`_Space`). The `Mission`/`MissionMenu` split
leaves `sMissionMenuText_Space` in `Mission`'s `.rodata` though only `MissionMenu` reads it: the
zero pad says the boundary is there, but it is the weakest of the 16.

```
python3 tools/modules.py --write && python3 tools/movemods.py
python3 tools/merge_module.py src/us/code/{Path,PlatformCollide,Frustum} --rodata-free   # one at a time
```

Merged after one declaration per name (pointer-only prototype changes, a renamed `Plane` view):
`code/Path`, `code/PlatformCollide`, `code/Frustum` (20 files -> 3).

**A module's `.data` in C: `vm1/NameEntryMenu`.** Merged (8 files -> 1) and its menu defined in
the file: `gNameEntryMenuItems` (45 `MenuItemDesc`, was `D_0043AD10` in
`virtual_rodata_0.data.s`) and `gNameEntryMenuDesc`, between `NameEntryMenu_CompareName` and
`NameEntryMenu_DrawName`. cc1 writes a file-scope initializer's strings when it outputs the
variable, last element first: that is the ROM's `" "`, `"9"`..`"0"`, `"z"`..`"a"` between the two
functions' literals, from the table's `"a"`..`"z"`, `"0"`..`"9"`, `" "`. The yaml gives the file a
`.data` subsegment (`[0x184D10, .data, vm1/NameEntryMenu/NameEntryMenu]`) and the stale words after
it (linker padding) an unnamed `data` one. The title and the last three keys are engine strings
(`sNameEntryText_Edit`, `_Glyph2/3/4`, now named). The port keeps the generated copy (the
definitions are `#ifndef TARGET_PORT`: the `s32` data slot holds an N64 address, a handle on the
host). This is the pattern for the other `.data` tables (below).

**`code/Crash` (not done, the plan).** Its object's `.rodata` starts at 0x800AA070 (rom 0xAAA70,
after 12 stale bytes at 0xAAA64): `""`, then the exception names `"Virtual coherency exception on
data reference"` .. `"Interrupt"`, `"IP1"` .. `"IP8"`, `"BD"`, i.e. the strings of `gCrashCauseDesc`
({mask, value, name}: BD, IP8..IP1, then the 18 exception codes) in reverse, then `": "` (read by
`func_0028B430` as an extern), then the slices. Its `.data` is in the engine's initialised data
after `.rodata` (`ABCA8.rodata.s`: the crash font, `sCrashCursorX/Y`, `sCrashBgColor`,
`gCrashCauseDesc`, `sCrashCanExit`). So: `gCrashCauseDesc` defined in C before the first function,
`": "`, `sCrashText_Spaces3`, `sCrashText_Newline` as literals, and a `.data` subsegment for the
file inside `code_rodata` (that segment is all `rodata`-typed today: whether splat links a `.data`
subsegment there in place is the open question), then the 12 stale bytes an unnamed `rodata` piece.

**The engine/VM objects (`code/CPlayer` + `vm1/CPlayer`, `code/CScene` + `vm1/SceneLoad`,
`code/CCamera` + `vm1/Demo`): not done.** Checked: SN's cc1 takes
`__attribute__((section(".text.vm")))` on a function and writes `.section .text.vm,"ax"` for it,
so one merged file can keep its VM functions in their own section while their literals stay in the
object's one `.rodata`, in source order (which is what the ROM's interleaved run is). What the build
needs: (1) in the `virtual` segment, a placeholder subsegment at the VM part's address whose
linker-script line `make setup` rewrites to `<merged>.c.o(.text.vm)` (as `tools/shared_rodata.py`
already rewrites per-part `.rodata` sections), with its auto-linked `.data`/`.bss` lines dropped;
(2) the VM part's `.rodata` slices dropped from the yaml (the merged file's one slice covers the
run); (3) `merge_module.py` writing the attribute for the functions of the VM directory and the
functions in `.rodata` order. All three pairs also need header work before they compile merged
(`merge_check.py --rodata-free`: `code/CPlayer` 245 files, `code/CScene` 96, `code/CCamera` 66 stop
at the compiler), so the build route was not written this round.

**State after o2: 93 modules are one C file; 57 module directories hold one function per file.**
The 22 directories the splits made have no `.rodata` question left (18 header work only, 4 data read
as externs: above); of round 25's 44, `vm1/NameEntryMenu` is merged and the 8 padded ones are split.

| Reason | Directories |
|---|---|
| header work only | round 25's 18, plus `code/{AI, AudioMgr (its functions are the port's: #ifndef), CAudioWorld, DList, ImpactEvent, LevelEvent, ModelSweep, Pickup, SndVoice, SweepShapes}`, `vm1/{AutomapDraw, MantidQueen, MissionMenu, Primagen, TitleMenu, Video}`, `vm2/{TitleMenu, Video}` (36) |
| data the C reads as externs, or `.data` tables in asm | `code/{Anim, CEngineApp, CGameObjectInstance, CInstancePool, CMtxF, Crash, CParticle, Trail}`, `vm1/{MpMenu, PakMenu}` (10) |
| one object over the engine and the VM | the 6 above |
| deferred functions plus header work | `code/{CLevelActor, VM}`, `vm1/{TotemDefense, Pak}` (4) |
| cc1 flags | `code/adecHuffDecode` (1) |

(`vm2/MpMenu`, `vm2/PakMenu` stay in round 25's header-work 18; part o1 works on those 18 and on the
deferred and flags rows.)

## Round 26, part o1: header work, `adecHuffDecode`, the vm1/vm2 twin structs

**Merged (6 directories, 98 files):** `code/libultra/audio/cseq`, `code/Generator`, `code/SndSeq`,
`code/SndQueue`, `vm2/Demo` (header work only) and `code/adecHuffDecode` (cc1 flags). `make verify`
after each batch and `asm_diff --base 3ffda1bb` 0 over the tree. How the declarations were made one:

- *One prototype, the definition's.* Where the files only spelled a callee differently (pointer vs
  `sptr` arguments, `void` vs `s32` results nobody reads), every file now takes the definition's
  prototype; the Rage Wars views (`RuntimeState`, `DecodeResult`, `ChannelSet`) are cast at the call.
  `__alCSeqGetTrackEvent` itself is written on `ALCSeq`/`ALEvent` (the Rage Wars `DecodeResult` is
  `ALEvent`'s `msg.midi`/`msg.tempo`).
- *ultralib's prototypes.* The `alCSP*`/`alCSeqNew` callers in `SndSeq` compile the same with
  libaudio.h's prototypes (`u8` channel and FX mix, `s16` volume): new `include/protos/libaudio.h`.
- *A result the ROM's caller reads narrower or wider than the definition returns*
  (`CScene__GetObjectTypeID` u16 read as `s32` in `Generator`; `adecReadBits` s32 read as `u16` in
  `adecHuffDecode`; `adecDecodeFrame` s32 read as `s16` in `adecDecodeFrameOk`): on N64 the call goes
  through a function type of its own (as round 25 did for `CAI__CallHurt`), the port calls directly.
- *Out-of-line twins.* `Generator_SpawnObject` and `Generator_SpawnPickup` are the out-of-line
  copies of `spawnObject`/`spawnPickup` and are now written the same way, on `CGameObjectInstance`/`CAI`.
- *`adecHuffDecode` under `-fforce-addr`:* the phrase address is an integer sum with `dictOffset`
  first, `(DecodePhrase *)(dictOffset + (sptr)table->phrases)`, instead of the `(phrases + 1) - 1`
  form the default flags needed; the module is then one file with `--cc1-flags auto`.
- *The port's replacements* in merged files go under `#ifndef TARGET_PORT` and their `link.cmake`
  lines become comments (`tools/portwrap.py MERGED.c STEM,..`: cseq 2, SndSeq 10, SndQueue 4).

**The vm1/vm2 twin structs:** `tools/hoist_twins.py` puts every local typedef that a vm1 module and
its vm2 copy (and no other module) define with one layout into `include/structs/twin_<module>.h`
(30 names in 7 modules: BlindOne, Demo, MotherBoss, PakMenu, Primagen, SceneLoad, TotemDefense;
`IntPair` skipped, `rw/abu_types.h` has it); the headers include only `common.h`/`structs/`
headers so no prototype comes along. Census: 142 of 653 -> 113 of 624 (with `B01Info`/`AdecStream`
from the header work).

**Left of the 18 header-only directories (13)**, with the first conflicts `merge_check.py
--rodata-free` reports: `code/CCache` (59: `osSendMesg`, `CHeap__Alloc/Free`,
`CDecompressor__DecompressAsync`), `code/CGameRegion` (74: `CISet__GetBlock`, the `CScene__*`
family), `code/EnemyAI` (136), `code/GameObject` (24: `CVector3__Normalize`, own basic typedefs,
`GameObject_TraceMove`/`GetGroundHeight` argument lists, `Placed`, a local `CGameRegion`),
`code/Hud` (27: `gEngineApp_m_OnScreen_m_ShadowXOff` array view, `Draw`, `ModeState`),
`code/WeaponMode` (42: the CPlayer weapon family, `Sound__Play`), `code/__scHandleRSP` (22: three
local `Sched`/`Client`/`Qn` views of `OSSched` per file: one rewrite onto `structs/sched.h`),
`vm1/BlindOne`, `vm2/BlindOne` (55/61: `gScene_m_LevelID` array view, the CCache family),
`vm2/MpMenu` (51), `vm2/PakMenu` (185: the libultra os prototypes), `vm2/Primagen` (136:
`LevelScript` views), `vm2/SceneLoad` (133: the CList family). The 4 deferred directories
(`code/CLevelActor`, `code/VM`, `vm1/TotemDefense`, `vm1/Pak`) were not reached: each needs the same
header work before `--inline`. None needs a ROM change.

## Round 26 merge: the state after parts o1-o3

Merged in the order o2 (it moves directories), o1, o3 (`--no-ff`). o1's edits to files o2 moved
(`vm1/PakMenu/func_00424008.c`, `vm2/PakMenu/func_0045CF28.c`, now in `Video`) came along with
git's rename detection; o1's six merged directories are one file each on the merged tree. After each
merge: `asm_diff --base 3ffda1bb` 0 over the tree except `vm2/MissionMenu/MissionMenu_2.c`, whose only
hunk is `Mission_LevelObjectivesDone_2`, moved to `vm2/Mission` by o2; a clean `make setup` + build +
`make verify` OK; the port compiles from a fresh gendata; `mklayout.py` regenerated where stale.

**`merge_check.py --rodata-free` over the 51 directories left:** none passed as it stood (44 stop at
the compiler, 7 refused: the 6 engine/VM objects and `code/CEngineApp`, whose name is taken). The
three smallest were taken through one declaration per name and merged (`asm_diff` 0, `make verify`):

- `vm1/Video`, `vm2/Video`: `Video_SetMode` reached the main camera through an `extern` of its local
  `NodeRecAt294` type; it now includes `globals/camera.h` and casts the address. `Video_SetMode` and
  `Video_ResizeViMode` take the definition's prototype everywhere.
- `vm1/AutomapDraw`: `Automap_Draw` had three local views (`gEngineApp_m_Settings` as an `s32`,
  `gEngineApp_m_Settings_nextMode`, `gEngineApp_m_pCurrentFrameData[]`); with `globals/game.h` it reads
  `gEngineApp_m_Settings_cheatFlags` and `gEngineApp.m_pCurrentFrameData->m_pFrameBuffer` (the
  struct member schedules as the ROM does, the plain scalar and a cast array do not). The old
  `FrameBuffers` view read the colour image at 0x110 on the host too, where `CFrameData` has it at
  0x1A0: a host bug the local view hid from `n64viewcheck.py` (it showed as `view/BAD` once the type
  was the real one). `Automap_RevealAroundPlayer` takes the automap as `void *` in both files.
- `code/CInstancePool` (not merged): its prototypes are one now, but the `--rodata-free` merge does
  not link (`sCInstancePoolCacheDesc_AnimObjectIndex`/`_AnimObjectInfo` are read as externs and the
  merged `.rodata` no longer defines them). The bytes say more: `"e\en"` stale at 0xA6C6C and stale
  bytes after `"anim object info"` up to 0xA6CA0, i.e. two more 16-aligned `.rodata` ends inside the
  module: it is likely three objects (a `rodata pad` that `tools/rodata_gaps.py` does not classify as
  `PADDING`). Left for the boundary pass.

**State: 102 modules are one C file; 48 directories hold one function per file.**

| Reason | Directories |
|---|---|
| header work only (first conflicts above and in part o1) | `code/{AI, AudioMgr, CAudioWorld, CCache, CGameRegion, DList, EnemyAI, GameObject, Hud, ImpactEvent, LevelEvent, ModelSweep, Pickup, SndVoice, SweepShapes, WeaponMode, __scHandleRSP}`, `vm1/{BlindOne, MantidQueen, MissionMenu, Primagen, TitleMenu}`, `vm2/{BlindOne, MpMenu, PakMenu, Primagen, SceneLoad, TitleMenu}` (28) |
| data the C reads as externs, or `.data` tables in asm | `code/{Anim, CEngineApp, CGameObjectInstance, CInstancePool, CMtxF, Crash, CParticle, Trail}`, `vm1/{MpMenu, PakMenu}` (10) |
| one object over the engine and the VM (`section(".text.vm")` route, part o2) | `code/{CPlayer, CScene, CCamera}`, `vm1/{CPlayer, SceneLoad, Demo}` (6) |
| deferred functions plus header work | `code/{CLevelActor, VM}`, `vm1/{TotemDefense, Pak}` (4) |

None needs a ROM change.

## Round 27, part p3: module data in C, two more boundaries, the deferred `CLevelActor`

**`code/CInstancePool` was three objects.** The stale bytes round 26 saw are linker padding that
`tools/rodata_gaps.py` does not class as `PADDING` (a string tail, `"e\en"`, at 0xA6C6C; an older
build's code words after `"anim object info"`): `.rodata` ends on 16 twice inside the module.
`tools/modules.py` has them as two more `RODATA_PAD` entries (two candidates each, the name-prefix
change taken) and `RENAMES` `DLightsLoad` (`DLights` is the next module): 339 -> 341 modules.

| Was | Now (functions) | Boundary |
|---|---|---|
| `code/CInstancePool` (18) | `CLevelActorPool` (9: `CLevelActorPool__*`, `SceneActor_Init`, `CPooledLevelActor__*`), `CInstancePool` (7), `DLightsLoad` (2: `DLights_Load`, `DLights_SwapBuffers`) | 0xA6C70 (of `func_00227AE0`, `func_00227C10`), 0xA6CA0 (of `func_002280B0`, `func_002284E0`) |

All three merged (`merge_module.py --rodata-free`); `CInstancePool__SpawnAnimInstance` passes its
cache descriptions as literals (`"anim object index"`, `"anim object info"`: the object's whole
`.rodata`, 0xA6C70-0xA6C95; the `sCInstancePoolCacheDesc_*` names stay in `globals/strings.h` and the
symbol files, unused by the C).

**Linker padding off 4: `tools/ld_fill.py`.** `CInstancePool`'s `.rodata` ends on a string at
0xA6C95, and the stale bytes up to 0xA6CA0 start off 4. splat cannot give them a subsegment there (a
`rodata` piece is disassembled as words; a `bin` piece goes into the segment's DATA group), and the
script's `SUBALIGN(4)` aligns every input section anyway. `make setup` now runs
`tools/ld_fill.py`, which writes the bytes as `BYTE()` statements right after the object's
`(.rodata)` line (`versions/ld_fill.us.txt`: `code/CInstancePool/CInstancePool 82 00 12`); the rest,
0xA6C98-0xA6CA0, is an unnamed `rodata` subsegment. `make verify` runs `ld_fill.py --check`.

**`code/Crash`: its `.data` in C, inside `code_rodata`.** The answer to round 26's open question:
splat takes a `.data` subsegment in the `rodata`-only `code_rodata` segment, but writes its line in
the segment's DATA group (before every `.rodata` line) and a second time in the `code` segment (the
`.c` subsegment's auto-linked `.data`). `tools/ld_fill.py`'s second form,
`code/Crash/Crash .data after ABCA8.rodata.s`, keeps one line and moves it after the piece before it
in ROM. So `Crash.c` defines, before its first function, `gCrashFont` (0x5F rows: ASCII
0x20-0x7E; `globals/crash.h` had 0x60, one row too many: 0x7F reads on into `sCrashCursorX`),
`sCrashCursorX/Y`, `sCrashBgColor`, `gCrashCauseDesc` (28 `FlagDesc`, the strings as literals) and
`sCrashCanExit` (`#ifndef TARGET_PORT`: the port keeps the generated copy); the yaml has
`[0xB7AA0, .data, code/Crash/Crash]` in the engine data and `[0xAAA70, .rodata, code/Crash/Crash]`
from the first cause name on: cc1 writes the table's strings first, last entry first (`""`,
`"Virtual coherency exception on data reference"` .. `"BD"`), then the functions' literals, `": "`,
`"<"`, `","`, `">\n"`, `"   "`, `"\n"` (`sCrashText_*` were the engine pool's names for the last
three and `": "`: now literals). The 12 stale bytes at 0xAAA64 stay an unnamed piece. Header work:
one prototype a name (the definitions': `Crash_PutChar(s32)`, `Crash_PrintHex8(s32)`,
`Crash_PrintString(u8 *)`, `u8 *` names), `Crash_PrintStack` takes the context as `void *` (its
`StackContext` view, the low word of `ThreadContext.sp`, no longer called `Thread`), and `gCFB` is
read as a scalar in both functions (`extern u16 *gCFB`, a local view: `globals/framebuffer.h`'s
array, written `*(u16 **)gCFB`, shares a base register between `Crash_ShowScreen`'s two reads).

**`code/CLevelActor` (deferred).** One declaration per name (the `CMtxF__*`/`CQuaternion__ToMatrix`/
`CVector3__ClampLength` callees on `void *`; `CLevelActor__LoadGeometry` as defined;
`CISet__GetBlock` from `protos/iset.h`; `CScene__GetObjectTypeID` from `protos/scene.h`, the N64
call through an `s32` function type as in `Generator`; `Actor_DrawGroundShadow` takes
`GameObject_GetGroundRotation`'s `CQuaternion` result, the port the `_sret` form), then
`merge_module.py --inline func_00215944`. `merge_module.py` writes a deferred file where its
functions are in `.text` (last); the ROM has its strings between `Actor_DrawGroundShadow`'s and
`CLevelActor__Update`'s literals, so the two `DEFERRED` functions were moved there by hand (a
`DEFERRED` definition puts its literals where it stands and its code at the end).

**State of the part's directories.** Merged: `code/{CLevelActorPool, CInstancePool, DLightsLoad,
Crash, CLevelActor}` (52 files -> 5). Left, with what stops each:

| Directory | Files | What is left |
|---|---|---|
| `code/Anim` | 49 | header work (`Bits_ReadFloat` argument lists, pointer views) before the data question |
| `code/CEngineApp` | 90 | the name is taken (`vm1/CEngineApp/CEngineApp.c`): `--name`; header work |
| `code/CGameObjectInstance` | 64 | header work (`CAnimationState__Reset`, `CGameObjectInstance__PickRandomAnim` .. in several forms); `sCGameObjectInstanceCacheDesc_AnimObjectInfo` read as an extern by 7 files |
| `code/CMtxF` | 124 | header work (the `CMtxF` parameter in `f32 *`, `f32 (*)[4]`, `CMtxF *` forms) |
| `code/CParticle`, `code/Trail` | 62, 45 | header work (about 50 conflicts each: `Morph` defined twice in `Trail`, the `CParticleSystem__Spawn` family); then the float table / `D_800A652C` as C data |
| `vm1/MpMenu` | 72 | header work (`Controller_ConfirmPressed`, `Controller_WatchPak` in `s8`/`s32` forms, `sprintf`, the `match_teamScore` scalar view); its `.rodata` is not in `.text` order, so deferred functions too |
| `vm1/PakMenu` | 128 | header work (the libultra os prototypes, `MenuPool_OpenMenu` arguments) |
| `code/VM` | 20 | header work: the `gPager_*` views (`B09SlotA`, `B09LNode`, `B09List` against `globals/pager.h`'s `PageEntry`/`PageMapping`), `func_0028EE40` in three forms |
| `vm1/TotemDefense` | 18 | header work: `gTotemDefense*` one-element array views, `CGameObjectInstance__PickRandomAnim`, `COnScreen__SetBoss*`, `Actor_SpawnParticle` |
| `vm1/Pak` | 42 | header work (the `CCache__*` family), and a source-order option in `merge_module.py` |

## Round 27, part p4: the engine/VM objects (`.text_vm`)

**Merged: `code/CCamera` + `vm1/Demo` -> `code/CCamera/CCamera.c` (66 + 28 = 94 files), `make verify`
byte-exact, `asm_diff --base 88d57333` 0 over the tree.** The original object is `CCamera.cpp`: 66
functions in the engine (0x27BD10..0x28233C) and 28 in the paged VM module (0x42EC30..0x430068:
`CCamera__Construct`, `CCameraPool__NewGame`, `CCameraPool__Construct`, `CEmitterPool__Construct`,
the demo stream decoder...), one `.rodata` run at 0x800AA3D0 with the VM functions' literals
(`CCamera__Construct`'s at 0xAA3F0, `CCameraPool__NewGame`'s strings at 0xAA600) between the engine
ones.

The route (the one `code/CScene/func_002261A8.c` already used for its one VM half, docs/VMASM.md):

- `VM_TEXT` (`include/common.h`): `__attribute__((section(".text_vm")))` on the N64 build, nothing on
  the port. cc1 writes the function into `.section .text_vm,"ax"`; its literals stay in the object's
  `.rodata`, in source order.
- `tools/merge_module.py <engine dir> --vm <vm dir>`: one source order, the two parts' `.text` orders
  interleaved by the `.rodata` run (a file without `.rodata` goes with the next one of its own part
  that has some; the rest at the end); `VM_TEXT` on every definition of the VM part, also inside its
  `#ifdef TARGET_PORT .. #else .. #endif` blocks (`CCameraPool__ResetEmitterPools`: the first build
  left that one in `.text`, 0x6C too long, and `make verify` caught it); in the yaml the engine
  part's first `c` subsegment and the run's first `.rodata` one name the merged file, the VM part's
  first `c` subsegment becomes `{start: 0x178C30, type: c, name: code/CCamera/CCamera,
  linker_section: .text_vm}` and the VM part's `.rodata` slices go with the merge (`--rodata-free`).
- No linker-script rewrite at `make setup`: splat 0.50 writes `CCamera.c.o(.text_vm)` for the dict
  subsegment itself. The `.data`/`.bss` lines it also writes for that subsegment in `virtual`'s
  sections match nothing (the engine part's lines come first in the script and take the sections).
- `tools/merge_check.py --rodata-free --margs "--vm src/us/vm1/Demo" code/CCamera`: the VM part's files
  are references too, and `.text_vm` functions are compared.

Header work (one declaration per name; every file the same assembly against 88d57333 before the
merge): `CCameraViewParams__Construct` takes `CCameraViewParams *` in both bodies (the N64 one reads it
through its `ObjInit` view); `CEmitterPool__Construct`, `CMtxF__Identity`, `CGameRegion__GetHeight`,
`CEngineApp__DrawFilledRect` the definitions' parameter types (casts at the calls); `memset` libc's;
the module does not include `protos/cache.h` (its `CCache__Flush(void)` is the definition's, every
caller passes the heap: `void CCache__Flush(s32)` in the module, with `CCache__Allocate` returning
`struct CMemEntry *` and taking the description as `char *`). The two alias-order views of
`gDefaultVisBits` (`extern s32 gDefaultVisBits[]` in `CCameraPool__Advance` and the N64
`CCameraViewParams__Construct`) are one `u32` now, read there through `CVisBits` (new in
`structs/camera.h`: the struct view `CCameraPool__Advance` already ORed `gCameraVisBits` through),
same code; `CCamera__Construct`'s `IntPair gCameraVisBits` (a store to `gScreenClearTime` addressed
off `gCameraVisBits`) is a `CCameraVisPair` view of the header's `gCameraVisBits`.

**Merged: `code/CPlayer` + `vm1/CPlayer` -> `code/CPlayer/CPlayer.c` (245 + 13 = 258 files)**, `make verify`
byte-exact, `asm_diff --base 88d57333` 0. 92 `.rodata` slices, 6 of the VM part (`CPlayer__Construct`,
`CPlayerPool__NewGame`...), in order in both parts. Header work in the one-function files first (each step
`asm_diff` 0, then `merge_check --vm` OK):

- one prototype per callee (about 40 names): the definition's where its types are
  visible, else `void *` for an object pointer the files typed differently (pointer arguments only
  warn); `protos/cache.h` out of the module as in CCamera;
- the definitions take the argument their callers pass (`CBarrel__RunButtons`' input,
  `CPlayer__UpdateFlashlight`'s body, `CPlayer__RamWithMount`/`CPlayer__TryMount`'s state flags:
  unused, the callers set the register); `CPlayer__GetFiringAmmo` returns the `AmmoInfo`, its callers
  read `->type`;
- the functions defined in `#ifdef TARGET_PORT .. #else` pairs (`CPlayerContState__Init`,
  `CBarrel__ShouldClose`, `CPlayerPool__UpdateMatchClock`, `CBarrel__Reset`) take `void *` in both
  bodies and name their view in a local;
- per-file types another file or a header defines differently renamed `<name>_<vram>` (`Record14`,
  `Controller`, `Voice`, `Tuning`, `PlayerList`, `MatchState`, `Globals`, `Plr`, `Menu`,
  `LevelActor`, `Arg1`...), two self-contained files on `common.h`'s base types;
- globals a file read through its own type: the header's declaration plus a macro view
  (`DEBUG_LINES_ON`, `CAMERA_POOL_GAME`, `SETTINGS_VIEW`, `#undef` at the end of the file) where the
  code stays the same, an asm-label name for the symbol where a view off its address changes the code
  (`gCameraPoolGameView`, `gMenuPoolGlobalsView`, `gMenuPoolGlobalsView_25492C`,
  `gCollisionHitInstance`: `extern T name __asm__("symbol")`, the precedent `vm1/MenuItem` and
  `code/CEngineApp` already use); the N64 m2c call of `Sound__Play` with its CVector3 as three words
  is `Sound__Play_Words`, an asm label of `Sound__Play`.

After the merge one string had no owner: `"player anim object info"` (`A7C94.rodata.s`, read as the
extern `sActorCacheDesc_PlayerAnimObjectInfo`) lay between `func_002525BC`'s and `func_00252E08`'s
slices; the merged file writes it as a literal (3 uses, pooled) and emits it there.
The port then crashed in the attract demos (`CPlayerPool__PreUpdate`): taking `protos/cache.h` out of
four files had left `CCache__Request`, `CCache__Unlock`, `CCache__LockForDisplayList` and
`CCache__SetFrameDelay` implicitly declared (the same N64 code; on the host an `int` return cut the
pointer). The merged file declares them; check a merged file with clang
`-Wimplicit-function-declaration` (the port build does not stop on it). Port md5s after the fix: boot +
3 demos 6ab97584, default whole game af8bd840.
`tools/merge_module.py` also learned that a top-level item does not end inside a comment (a comment
line ending in `;` had cut one in two).

**Not merged: `code/CScene` + `vm1/SceneLoad` (96 + 24 files).** About 140 distinct conflicts merged
(`CScene__*` prototypes against `protos/scene.h`, `CCache__*`, `CList__*`, `CISet__GetBlock` with 2 and
3 arguments, `ProPack__Decompress`, per-file types), and the route alone would not order its
`.rodata`: `func_00224D54`'s `"binary block"` and `func_00224DF4`'s `"graphicset"` come before
`func_00220D00`'s `0.5f` although they follow it in `.text`, and `"level name"` at 0xA6BD0 is read as an extern (no C file emits it). Two `DEFERRED` functions (`inline`: output at the end of their object, strings at the definition) would give that order only if an engine object ended after them, i.e. the engine part is two objects (0x220260..0x224ED0 and on); so the pair needs the boundary check (`tools/modules.py`) and `--inline` before `--vm`. `func_002261A8.c` already holds its own VM half (`.text_vm`).

How to proceed with a pair: `tools/merge_check.py --rodata-free --margs "--vm src/us/<vm dir>" <engine
dir>` for the conflicts, fix them in the one-function files with `asm_diff --base` 0, then
`merge_module.py <engine dir> --vm <vm dir> --rodata-free`, `make setup`, a clean build, `make verify`.

## Round 27, part p1: header work on 14 code/ directories

**Merged (6 directories, 180 files):** `code/AudioMgr`, `code/LevelEvent`, `code/DList`, `code/Hud`,
`code/WeaponMode`, `code/ImpactEvent` (header work only; `merge_check.py` OK without `--rodata-free`).
`make verify` after each batch, `asm_diff --base 88d57333` 0 over the tree, the port builds from a fresh
gendata (aligncheck 0 sites, `port_layout` regenerated); port md5s unchanged (boot+3 demos 6ab97584,
default whole-game af8bd840). How the declarations were made one:

- *A header per module for the calls it makes,* where the callers agree with each other but not with
  the definition in another module: `protos/weapon_mode.h` (WeaponMode: `Sound__Play` with an `s32`
  sound as its callers compile it, the CPlayer weapon family, `CCameraPool__AddText`,
  `CMtxF__PreRotateY`), `protos/impact_event.h` (`GameObject_TraceMove`/`GetCollisionHeight`,
  `CGameObjectInstance__GetAverageScale`). The files' own one-line declarations are dropped by
  `tools/unify_decls.py DIR HEADER NAME..` (new), then `asm_diff`.
- *ultralib's prototypes:* `alHeapDBAlloc`, `alLink`, `alUnlink` in `protos/libaudio.h` (the
  AMDMABuffer nodes cast to `ALLink`); `SndSeq`'s N64 branch lost its own `sptr` declaration.
- *Arguments the ROM's caller passes and the definition does not take, or narrows:* `__amMain` calls
  `__amHandleFrameMsg` through a two-argument type of its own; the Hud callers of
  `Hud_DrawImageFrame`/`Hud_DrawMaskedBlock`/`Hud_DrawI4Block` cast the coordinates to `s16` at the call
  (in the merged file the definition's `int` prototype is seen first, so the `s16` prototypes no
  longer narrowed: the merge check showed it as an asm difference).
- *Rage Wars by-value views replaced by the engine types:* the ImpactEvent blast handlers take
  `CVector3` and `CImpactBlast` (were `Triple`/`IntPair`/`B15Blk8`), `ImpactEvent_Rumble` passes a
  `CVector3` to `CScene__StartRumbles`; `DList_Remove` is on `rw/junk_types.h` `DLNode`/`DLList` (its
  local view had prev/next swapped by name).
- *Field symbols:* `globals/onscreen.h` now declares `gEngineApp_m_OnScreen_m_ShadowXOff[2]` (every
  reader takes the pair as an array; a cast of the scalar did not keep the code); the `m_TopR`,
  `m_FontXScale`, `gFrameCountGameplay` and camera-pool views are casts of the header symbols
  (`(*(View *) &sym)`, which keeps the code for a struct view).
- *Local types:* shared ones hoisted into `structs/hud.h` (`FrameEntry`, `FrameTable`, one
  `ModeState`) and `structs/weapon_node.h`; same-named views of different things renamed by role
  (`EventParams` x4, `Kind`, `Chunk`, the static `Draw` helper).

**Left of the 14 (8)**, with the first conflicts `errs` (merged-file compile) reports:
`code/AI` (55: `Dest`/`Source110`/`StateEntry`/`SoundSourceNode` Rage Wars views of CAI in several
layouts, `AI_StopSound` defined on `Dest`, `GameObject_TraceMove` with three `f32` for the position,
`CGameObjectInstance__GetRotation` as an sret `void (CQuaternion *, ...)`), `code/CAudioWorld` (50) and
`code/SndVoice` (59): each has one or two files carrying a whole Rage Wars type dump (`Shared_Quad`,
`MenuRules`, `ContextRecAt2BC0`, `Voice`, ... redefined against `rw/abu_types.h`), plus the
`alSndp*`/`HalfToFloat` prototypes and `__osDisableInt`/`__osRestoreInt`; `code/CCache` (59:
`osSendMesg`, `CHeap__Alloc/Free`, the `CCache__*` family, `gCache_*` views), `code/CGameRegion`
(74: the `CScene__*` family, `CISet__GetBlock`), `code/EnemyAI` (136), `code/GameObject` (24:
`CVector3__Normalize` as `f32 *`, basic typedefs, `Triple`/`TripleRecAt8` by value, a local
`CGameRegion`, `GameObject_GetGroundHeight` argument lists), `code/__scHandleRSP` (22: the per-file
`Sched`/`Client`/`Qn` views onto `structs/sched.h` `OSSched`/`OSScTask`, still to write).

## Round 27, part p2: menus and the vm1/vm2 twins

**Merged (9 directories, 358 files -> 9):** `code/ModelSweep`, `code/Pickup`, `code/SweepShapes`,
`vm1/TitleMenu` (`--rodata-free --inline all`), `vm2/TitleMenu` (`--rodata-free`), `vm1/MissionMenu`,
`vm1/BlindOne`, `vm2/BlindOne`, `vm2/MpMenu` (`--rodata-free`). `make verify` byte-exact and
`asm_diff --base 88d57333` 0 over the tree after each. (The part wrote no section of its own; this one
is from its report.) What it took:

- *Settings bytes as arrays:* `include/globals/game.h` declares `gEngineApp_m_Settings_unk14`/`15`/`16`/`18`
  as `u8 x[]` (the readers index them); `code/CAudioWorld`'s own `extern u8 gEngineApp_m_Settings_unk15[]`
  goes when that directory is merged.
- *The BlindOne scene arrays:* `src/us/vm{1,2}/BlindOne/blindone_scene.h` (`SCENE_LEVEL_ARRAYS`), one
  definition of the level tables for both VM copies.
- *Twins:* `tools/twin_text.py` (new) writes the vm2 copy of a module as its vm1 twin, file by file, kept
  at `asm_diff` 0; `tools/unify_protos.py` (new) gives a name one prototype across a directory's files.
  The `_2` menu prototypes live in `vm2/TitleMenu_2.c`; `vm2/MpMenu` reads its match state through the
  `gMatchView` asm label.

**Left of the part (5):** `vm1/Primagen` and `vm2/Primagen` (the `gLevelScript` views), `vm1/MantidQueen`
(`AI_TurnClamped`: 4th argument `f32` in some callers, `s32` in others), `vm2/SceneLoad` (goes with the
engine/VM `CScene` pair of part p4), `vm2/PakMenu`.

## Round 27 merge: the state after parts p1-p4

Merged in the order p3 (it moves directories: the `CInstancePool` split, `tools/ld_fill.py` in the
`Makefile`), p4 (`VM_TEXT`, `.text_vm`), p1, p2 (`--no-ff`). The only textual conflicts were this file
(each part's section kept) and `versions/port_layout.us.json` (regenerated with `mklayout.py` after each
merge); `include/common.h`, the `Makefile`, `merge_module.py`, the headers and the yaml merged on their
own, and `tools/modules.py --write` gave no change. After each merge: `asm_diff --base 88d57333` 0 over
the tree, a clean `make setup` + build + `make verify` OK, the port compiles from a fresh gendata, the
merged files have no implicit declaration under clang `-Wimplicit-function-declaration` (the port's
`hostified` copies for the VM-text and `_2` files), and the nested `TARGET_PORT` scan is the same 2
known sites (`CPlayer/func_002555CC.c`, `SndQueue.c`).

**`merge_check.py --rodata-free` over the 26 directories left:** none passed as it stands (23 stop at
the compiler, 3 refused: `code/CScene` and `vm1/SceneLoad`, whose `.rodata` interleave, and
`code/CEngineApp`, whose name is taken by `vm1/CEngineApp`; pass `--name`). Fewest first conflicts:
`code/Anim` 14 (`Bits_ReadFloat` argument types), `vm1/MpMenu` 16 (`Controller_ConfirmPressed`),
`code/__scHandleRSP` 22, `code/GameObject` 24, `code/CMtxF` 25 (`Actors_GetLightVolumeFalloff`),
`vm1/TotemDefense` 31, `vm1/MantidQueen` 34.

**State: 124 modules are one C file; 26 directories hold one function per file.**

| Reason | Directories |
|---|---|
| header work only (first conflicts in parts p1, p2 and above) | `code/{AI, CAudioWorld, CCache, CGameRegion, EnemyAI, GameObject, SndVoice, __scHandleRSP}`, `vm1/{MantidQueen, Primagen}`, `vm2/{PakMenu, Primagen}` (12) |
| data the C reads as externs, or `.data` tables in asm (`tools/ld_fill.py` and Crash show the way) | `code/{Anim, CEngineApp, CGameObjectInstance, CMtxF, CParticle, Trail}`, `vm1/{MpMenu, PakMenu}` (8) |
| one object over the engine and the VM (the `.text_vm` route of part p4; the engine part is likely two objects) | `code/CScene`, `vm1/SceneLoad`, `vm2/SceneLoad` (3) |
| deferred functions plus header work | `code/VM`, `vm1/{TotemDefense, Pak}` (3) |

None needs a ROM change.

## Round 28, part q1: header work on the engine (`__scHandleRSP`, `GameObject`, `AI`)

**Merged (2 directories, 79 files):** `code/__scHandleRSP` (17) and `code/GameObject` (62), each with
`tools/merge_module.py DIR --rodata-free` after the header work below. After each batch:
`asm_diff --base b9bb8125` 0 over the tree, a clean `make setup` + build + `make verify` OK, the port
builds from a fresh gendata, and the merged files have no implicit declaration under clang
`-Wimplicit-function-declaration`.

- *`__scHandleRSP`: the scheduler on `structs/sched.h`.* The 17 files' views (`Sched`, `Client`,
  `ScTask`, `Vid`/`Job`/`Qn`/`View`, `Panel`, `PendingOwner`/`PendingNode`, `NodeLists`/`Node`,
  `Stream`/`StreamOwner`, Rage Wars' `Ctx_`/`Node_`/`View_`/`Scene_func_8028F*_de`) are now
  `OSSched`, `OSScTask` and `OSScClient`; the frame a gfx task draws is a `CFrameData`. The new
  `protos/sched.h` has one prototype per scheduler function on those types (and the libultra calls only
  the scheduler makes: `osSetIntMask`, `osSpTaskYield(void)`, `osSpTaskYielded`). What the views
  hid: `Sched.retraceMsg` (0x20) is `prenmiMsg` (the pre-NMI message goes to the clients), the clients
  get `&retraceMsg` (0x0); Rage Wars' `music`/`time` are the task's `output_buff` and
  `*output_buff_size` (`osDpSetNextBuffer`); `osSpTaskYield` takes no argument (the `$a0` the m2c
  draft passed is only left over). `osScGetCmdQ`, `Sched_GetInterruptQ` and `Sched_SubmitTask` take an
  `OSSched *` and return or use its queues by member.
- *`GameObject`: `protos/game_object.h`* holds the module's functions and the calls it makes
  (CGameRegion, PlatformCollide, GameObjectMove, SweepShapes, the CMtxF vector helpers and the gu
  math), one prototype each. Eleven files had an N64 view and a port view (`#ifdef TARGET_PORT`) of the
  same function; each is now one function on `CGameObjectInstance`/`CIntelligence`/`CAI`:
  `func_00268210`, `GameObject_GetCollisionYOffset`, `GetDeadCollisionHeight`, `GetFrameDelta` and
  `GetFrameTurn` (the whole `CAIFrameMotion` on the stack, as the port had it), `IsNearFloor`,
  `IsOnGround`, `GetFloorHeightAt`, `GetFloorKind`, `GetRegionFloorHeight`, `GetAimRotation`.
  The five `Model`/`ModelBlend`/`Object` getters (`func_00267D2C`.. `GameObject_IsLeashed`) read the
  definition's `m_dwTypeFlags` (their FAKEMATCH shapes kept). `GameObject_GetFloorPointAt` traces a
  `CVector3` (Rage Wars' `TripleRecAt8` had an `s32` y): the port's `GameObject_TraceMove_fif` wrapper
  (`port/game/t2abi.c`) has no caller left. Kept as the ROM has them: `GameObject_GetTypeId` calls
  `CScene__GetObjectTypeID` through an `s32` type on the N64 (the ROM does not mask the `u16`; the
  host keeps `protos/scene.h`'s `u16`), `GameObject_IsOnGround` passes `GameObject_GetGroundHeight` a
  second argument 0 (cast at the call), `CGameRegion__GetNormal` is declared `void` (its definition
  returns the vector; with that type `GameObject_GetFloorTilt` keeps `$s1` for the result).

**Header work without the merge: `code/AI`** (40 files; 30 of them on the new headers).
`protos/cai.h` (new) declares the AI functions on their real types; `protos/ai.h` and
`protos/enemy.h` keep the untyped views the enemy modules call (`CAI__SetMode`, `CAI__CallHurt`,
`AI_TurnClamped`, `AI_ComputeFrameMotion`...), so an AI file may not include them. Done:
`AI_InitHealth`, `AI_RideMovingPlatform`, `CAI__UpdateModeAnim` and `CAI__SetMode` on `CAI` /
`CAIModeInfo` / `CGameObjectInstance` (their `Dest`/`Source110`/`StateEntry` N64 views and the
port's host-offset views joined; `m_TranqHealth` is written through an `s8` conversion, as the ROM
converts it), `AI_FindBestTarget` on `CGameObjectInstance` (the nearest player read as a `CPlayer`).
**Left (7 files)**, with what stops them: `func_00245C5C.c` and `func_0024699C.c` call
`AI_FindClearHeading`/`GameObject_TraceMove` with three `f32` for the position (the port's `_fif`
wrappers); `func_0024646C.c` and `func_00246B24.c` call `GameObject_DropPickupAt` the same way and
redefine the basic types; `func_00246310.c` (`AI_AngleToPoint`), `func_00246690.c`
(`Actor_SpawnParticle`), `func_00246860.c` (`AI_StartSound`) and `func_00246918.c` (`AI_StopSound`)
define their function twice (N64 view / port view) on `SpawnObj`/`SndSrc`/`Dest`;
`func_002449E0.c` defines `AI_GetSightTargetInfo` with an `sptr` third argument and
`func_002465F0.c` `AI_DistSqToPointAtHeight` on a `B04Obj` view. Each needs the treatment the
eleven GameObject files got (one function on the engine types, `CVector3` by value).

**Not started: `code/EnemyAI`** (119 files): out of time after AI. Its first conflicts are the same
untyped `protos/enemy.h` views and the `EnemyAI_GetBehaviorIndex`/`GetCurrentBehavior` `s8`-vs-word
returns (already written as `#ifdef TARGET_PORT` pairs).

## Round 28, part q2: CCache, CGameRegion, CAudioWorld (and what SndVoice and VM still need)

**Merged (3 directories, 154 files):** `code/CCache` (56), `code/CGameRegion` (53), `code/CAudioWorld`
(45). `merge_check.py` OK without `--rodata-free` for all three; `make verify` (clean) after each batch,
`asm_diff --base b9bb8125` 0 over the tree, the port builds from a fresh gendata. How the declarations
were made one:

- *New headers, the definitions' own signatures:* `protos/heap.h` (`CHeap__Alloc`/`CHeap__Free`,
  `CDecompressor__DecompressAsync`), `protos/hash.h` (`CHashTable__*`), `protos/osint.h`
  (`__osDisableInt`/`__osRestoreInt` with the SDK's `u32` mask), `protos/region.h` (the CGameRegion calls
  the module makes to itself), `protos/audio_world.h` (what CAudioWorld calls in the sound player, the
  speech player and the camera pool, plus `VM_Alloc`), `protos/sndp.h` (libaudio's `alSndp*` with the
  SDK's types, for SndVoice); `CVector3__Normalize(CVector3 *)` added to `protos/mtxf.h`. The files' own
  one-line declarations dropped with `tools/unify_decls.py` (the commands are in the commits).
- *Trap:* `__osDisableInt` first went into `protos/ultra.h`; the port then failed to compile
  `code/CISet/CISet.c` (it includes `protos/ultra.h` and declares an `s32` mask). A shared header that
  many modules include only takes a name no includer declares otherwise; hence `protos/osint.h`.
- *Rage Wars views rewritten over the engine types:* CCache's `DeallocateInternal` (a `CAI` view of a
  `CMemEntry`: now one form for both builds), `DetachCacheEntry` (M2C_FIELD), `TakeFreeCacheEntry`,
  `FreeCacheEntry`, `ReleaseAndReplaceData`; CCache's `DrawDebug` (FAKEMATCH) reads `globals/cache.h`'s
  `gCache_*` instead of its own array views. CGameRegion's `Node75` (= `CGameRegion`: `prev/cur/next` are
  `m_pVertices[0..2]`), the `Entry`/`Handle` views (`CGameRegion`, `CRegionInfo`), `Triangle0026CB2C`;
  `func_0026CB2C.c` no longer includes `rw/abu_protos.h` (the only includer in these directories).
  CAudioWorld's `Update` (the whole-file Rage Wars type dump: `ContextRecAt2BC0`, `MenuRules`,
  `MenuMatchGlobals`, `Shared_Quad`, ...), `UpdateAmbientTrack`, `PlaySpeech`, `StopSpeech`,
  `PickVariant`, `IsSoundBusy` (the 0xCC-byte "slots" are `pool.voices[i].soundId`), the accessors
  (`BigState2BB0`, `EntryOwner`, ...) all on `structs/sound.h` `CAudioWorld`; the envelope getters return
  `SndEnv *`. Register allocation decided two forms: a reused local in `DetachCacheEntry` and a typed
  local for the `void *` parameter of `ReleaseAndReplaceData` each moved a register; two locals and a
  cast at each use keep the code.
- *Calls the ROM makes with more arguments, or reading a void result:* cast at the call
  (`CCache__AllocateInternal` 4 args, `CCache__WaitForDisplayLists(0)`,
  `CCache__FreeOldestMemEntryWithFlags` read as `s32`, `CGameRegion__FloodSetHeight` read as `f32`,
  `CAudioWorld__StartVoice` 4 args, `SpeechPlayer_Play` 4 args, `alSndpAllocate` 3 args).
  `CGameRegion__GetCeilingNormal` is now called with the region on both builds: the ROM leaves `$a1` as
  the caller got it, which is the code passing it gives (the N64 build had a one-argument prototype).
  `BinaryRange`'s key is `u32`; the CAudioWorld callers cast to `s16` at the call (the sign extension the
  ROM does).
- *A merged module the port replaces:* 27 CAudioWorld functions are `T2_LINK_REPLACED`
  (`port/audio/t2audio_world.c`); after the merge `tools/portwrap.py` put them under
  `#ifndef TARGET_PORT` and turned their `link.cmake` lines into comments (without it the port's link
  fails with duplicate definitions). SndVoice (26 files) and VM (4) need the same after their merge.

**Left of the five (2):**

- `code/SndVoice` (41 files): `alSndp*` done (`protos/sndp.h`). Left: `func_00273EC0.c` carries a whole
  Rage Wars type dump (about 100 types: `Triple`, `Shared_Quad`, `Voice`, `ContextRecAt2BC0`,
  `GameplayCamera`, ...) for `SndVoice_Setup`; the port's typed copy in `port/audio/t2audio_world.c`
  gives the field map (`View_func_8025AE1C_de`: priority 0x28 = `vp.pan`, envelope 0x8C = `env3`,
  sweeping 0x88 = `hasEnv`, trackId 0xA8 = `soundId`, fixed 0xC0 = `reset`). Then `SndVoice_InitPitch`
  (`ViewRecAtB0`/`ControllerCode802591C0`), `SndVoice_InitVolume` (called with 3 arguments, defined with
  2), `Sound_CalcPan(sptr, u8 *)`, `HalfToFloat` (`s32` in `code/HalfToFloat`, `u16` in three callers)
  and `FloatToHalf` (`u16` vs `s16`), the `CAudioWorld__Get*Envelope`/`GetStereo` callers against the
  merged CAudioWorld's `SndEnv *` definitions. All 26 port-replaced files are under `portwrap.py` after
  the merge, so the rewrites do not touch the port's behaviour.
- `code/VM` (20 files): not reached this round; as round 27 lists (the `gPager_*` views against
  `globals/pager.h`, `func_0028EE40` in three forms, `--inline` from `func_0028E068` on).

## Round 28, part q4: data in place, `code/CEngineApp`, the CScene question

**Merged: `code/Anim` (49 files -> `Anim.c`), `vm1/MpMenu` (72 -> `MpMenu.c`), `code/CEngineApp` (90 ->
`CEngineApp_engine.c`, `--name`: `vm1/CEngineApp/CEngineApp.c` keeps its name).** Each:
`merge_check.py --rodata-free` OK on the one-function files first (every step `asm_diff --base b9bb8125`
0), then `merge_module.py --rodata-free`, a clean `make setup` + build + `make verify` byte-exact, the
merged file through clang `-Wimplicit-function-declaration` (no implicit declaration), port md5s.

Header work (one declaration per name):

- **Struct-by-value callees through their words.** The port's `#define X X_i` lines at file scope would
  rename the definition too once the module is one file (`Bits_ReadFloat` is defined in `Anim`). The
  callers that pass a struct (or an `f32`) as the words it travels in call an asm-label name instead:
  `Bits_ReadFloat_3` (`__asm__("Bits_ReadFloat")` on the N64, the real `port/game/t2abi.c` shim on the
  port), `CQuaternion__Slerp_i`, `CVector3__Lerp_i`. Same code.
- `Anim`: two files that redeclared Rage Wars basic types and `Func802608ECResult` include
  `rw/abu_types.h`; `func_00216A9C`'s `Pack` is `AnimPack` (the header's `Pack` has the same layout,
  but the file does not include it), `func_0021836C`'s `S` is `IdVec`.
- `MpMenu`: `Controller_ConfirmPressed(ControllerProfile *)`, `Controller_WatchPak(s32)`,
  `sprintf(char *, const char *, ...)` one prototype each; the blue team score read as a scalar
  (`globals/game.h` has the array) is `gMatchBlueTeamScore __asm__("gEngineApp_m_Settings_match_teamScore")`.
- `CEngineApp`: callee prototypes from their definitions (`CPlayerPool__NewGame`, `CCameraPool__NewGame`,
  `Render_SetRenderMode(u32)`, `Music_SetTrack`, `Controller_SetRumbleOptionAll`, `memset`, `protos/scene.h`'s
  `CScene__*`). Where a definition takes fewer arguments than a caller passes (`CEngineApp__Boot`,
  `CEngineApp__BeginDisplayList`, `CEngineApp__UpdateControllers`, `Credits_Draw`, `func_00289108`,
  `CScene__InitPersistentData`, the N64 one-argument `CCameraPool__NewGame` call), the definition keeps its
  prototype and the call casts the function (`((void (*)(T)) f)(x)`: the same `jal`, the same argument
  registers). The array views of scalars (`gCreditsDone`, `gCreditsTimeLeft`,
  `gMergedController_buttonsSingle`, the `MenuPool` as bytes) are asm-label names.
  `CEngineApp__SubmitGfxTask`'s N64 body takes the port body's `struct CEngineApp *`/`struct CFrameData *`
  and reads them through macro views (`#define gfx ((GfxState *) app)`): a local copy moved a register.

**Data defined by the merged file.**

- `Anim`: `D_800A58F4` (0x38000100) is `CAnimationState__GetBoneRotation`'s own literal, 1/32767
  (`3.051851e-5f`, the value `Anim_GetBoneRotation` already wrote): it was the unnamed piece between two
  slices. `D_800ACB50` (90.0f) stays an extern: it is in the engine pool (`ABCA8.rodata.s`).
- `MpMenu`: the ROM's `.rodata` for the object starts with five strings, `"%04d"`, `" "`, `"%02d"`,
  `"  %02d"`, `"none"`, before `MpMenu_StartFragTag`'s literals, and `"%3d"` sits between
  `MpMenu_StartFragTag`'s and `MpMenu_StartBloodLust`'s. The functions using them come later in `.text`,
  and other functions read them by name, so in the original an earlier definition emitted them (an inline
  label helper or the menu tables: the vm1 `.data` items point at `" "`; cc1 writes a string where it is
  first expanded and pools later uses). The merged file defines them in place as data under their
  symbol names, N64 only (the port keeps the generated copies):
  `const char sFinalScoreMenuFmt_04d_def[] __asm__("sFinalScoreMenuFmt_04d") = "%04d";` and so on
  (`D_800C0E78` `" "`, `D_800C0E7C` `"%02d"`, `sLimitLabelFmt_02d`, `sLimitLabelText_None`,
  `sMpMenuFmt_3d`); the label functions read them by name (`MPMENU_FMT_02D` for the one without a
  header name). `sMpMenuFmt_3d` is also read by `vm2/MpMenu/MpMenu_2.c`: before the merge only the
  `C1A9C` piece defined it, so it must be defined by the merged file.
- `CEngineApp`: `mainproc`'s build stamp (`"%s %s"`, `"Nov 10 1998"`, `"13:17:04"`) is in the ROM before
  `CEngineApp__Main`'s literals although `mainproc` comes later in `.text` (the MpMenu pattern): N64-only
  `const char sEngineStamp{Fmt,Date,Time}[]` before `CEngineApp__Main`, read by `mainproc` (the port
  keeps the literals). The unnamed 60.0f at 0xAA9FC is `EngineMode01_Legal_Retrace`'s first literal (the C read
  it as `D_800A9DFC`); the asm `func_002C232C` reads it too, so `D_800A9DFC` is now an absolute symbol in
  `versions/symbols_manual.us.txt`.

**`code/CScene` + `vm1/SceneLoad`: not one clean split at 0x224ED0.** Round 27 asked whether the engine
part is two objects (0x220260..0x224ED0 and on), with `CScene__LoadBinaryBlock` (`func_00224D54`) and
`CScene__LoadGraphicSet` (`func_00224DF4`) `DEFERRED` at the end of the first. For it: 0x224ED0 starts on
16; the two functions are the last before it; their strings (`"binary block"`, `"graphicset"`, 0xA6B38)
come before `func_00220D00`'s 0.5f; and cc1 only puts a function's strings before its code in two ways
(checked with the game's cc1): the function is `inline` (code at the end of the object, also when an
`extern inline` body is redefined later in the file: the real definition is still output last), or an
earlier function expanded the same string. No other function reads 0xA6B38/0xA6B48 (no inlined copy
elsewhere), so it is the first.
Against a split there: the `.rodata` of the first object would end at 0xA6BF0 (also on 16, between
`func_00223A94`'s and `func_00225354`'s slices), yet three strings in it are pooled with functions after
0x224ED0: `"level index"` (0xA6AC0, `CScene__LoadLevel`'s; read by `CScene__GetLevelName`,
`func_0022629C`), `"warp dests"` (0xA6BC4, `CScene__FindWarpDests`'; read by `func_0042C560`, whose
own `.rodata` is at 0xA6C0C, after `func_00226024`'s) and `"level name"` (0xA6BD0: no slice at all; only
`CScene__GetLevelName` reads it, and it lies before `func_00223A94`'s slice). A literal is pooled only
inside one compilation unit, so either those three were named global strings in the original (a
`.rodata` array another object reads by name), or the module is one object and the strings come from
earlier inline expansions. `CScene__GetLevelName` shows the same pattern as the two loaders (its string
far before its code; 0x226390, after it, also starts on 16). Next step: try the one-object form first
(the two loaders and `CScene__GetLevelName` as `static __inline__` helpers defined where their strings
are, each with a non-inline wrapper where its code is, `merge_check --margs "--vm src/us/vm1/SceneLoad"`
decides), then the split; the ~140 header conflicts of round 27 come either way.

**State of the part's directories.** Merged: `code/{Anim, CEngineApp}`, `vm1/MpMenu` (211 files -> 3).
Left:

| Directory | What is left |
|---|---|
| `code/Trail` (45) | `rw/abu_types.h`'s `struct Morph` (`blends`/`first`/`second`...) against `structs/morph.h`'s (`items`/`pointsA`/`pointsB`..., same layout): a merged file includes both; the five `rw/abu_types.h` files need rewriting over the real types (the q2 kind of work), then about 15 callee prototypes (`CMtxF__*`, `CVector3__*`, `DList_*`, `NodePool_Alloc`) |
| `code/CParticle` (62) | about 30 names, the `CParticleSystem__Spawn` family; then the float table as C data |
| `code/CGameObjectInstance` (64) | about 48 names (`CAnimationState__Reset`, `CGameObjectInstance__PickRandomAnim`...); `sCGameObjectInstanceCacheDesc_AnimObjectInfo` read by 7 files |
| `code/CMtxF` (124) | about 25 names in the first pass (`Actors_GetLightVolumeFalloff`, `ModelVertex`/`VertexGroup`/`PlacedProp` per-file types, `CISet__GetBlock`, `CStaticObject__*`), more behind them |
| `code/CScene`, `vm1/SceneLoad`, `vm2/SceneLoad` | the object question above, then the round-27 conflicts |

## Round 28, part q3: the VM bosses (`vm1/{TotemDefense, MantidQueen, Primagen}`, `vm2/Primagen`)

**Merged (4 directories, 179 files -> 4):** `vm1/TotemDefense`, `vm1/MantidQueen`, `vm1/Primagen`, `vm2/Primagen`.
`make verify` byte-exact and `tools/asm_diff.py --base b9bb8125` 0 over the tree; the port builds from a fresh
gendata (aligncheck 0 sites, `port_layout` regenerated), the merged files have no implicit declaration under clang
`-Wimplicit-function-declaration` (N64 and `TARGET_PORT`). Port md5s unchanged: boot + 3 demos 6ab97584, default whole game af8bd840.

*The `.rodata` order: a deferred tail written early.* In all three vm1 objects one function late in `.text`
emits its literals before an earlier function's (TotemDefense: `TotemDefense_Init`'s `"Enemies"`/`"Time"` and
`TotemDefense_DrawTotem`'s `"lit vertices"` before `TotemDefense_Complete`'s floats; MantidQueen: the `"Queen"` of
0x402BB0 before `MantidQueen_ChooseAttack`'s floats; Primagen: the `"Primagen"` of 0x407C20 before 0x405AC8's).
The source order that gives it: every function from the late one to the end of the object is `DEFERRED`
(`inline`: output after the others, in the order of their first declaration, literals at the definition) and
defined before the function whose `.rodata` follows. `--inline all` is wrong here: GCC then inlines a deferred
function into a later caller (`TotemDefense_UpdateFight` grew by `TotemDefense_Complete`/`_Fail`, +0xC14). So
the tail must not be called (only have its address taken) by the functions compiled after it; checked for the
three. `tools/merge_module.py` learned:

- `--before A,B..:C`: files A,B.. (their `.text` order kept) go right before file C in the source order;
- `--inline` also writes `DEFERRED` on the definitions inside a conditional block (`#ifdef TARGET_PORT ..
  #else .. #endif`: `TotemDefense_Init`, `MantidQueen_UpdateMode6`, `MantidQueen_NextStep`), which take their place
  in the output order (their early prototype is the block's first definition's signature, so both bodies take
  the same parameter types: `MantidQueen_NextStep(void *)`);
- an early prototype of a deferred function takes the type declarations it needs with it (`move_types`,
  recursive), and a conditional block's struct typedefs count as its types.

Header work (each step `asm_diff` 0):

- *`AI_TurnClamped`* (vm1/MantidQueen): the definition takes `f32, f32` (`protos/ai.h`, the asm passes the
  angle in a3 either way); the two callers that passed `*(s32 *)&angle` now pass `CAITargets.m_Path.m_Angle`, with
  `CAITargets`/`CAI` instead of the per-file views `Info00403144`/`Item00403144` (the port's `AI_TurnClamped_i`
  shim no longer needed there). The Rage Wars type dump of `MantidQueen_EnterDeath`'s file is gone.
- *One-element and byte views of a global*: `tools/view_label.py FILE NAME..` (new) turns a file's own
  `extern T NAME[..];` into an asm-label alias `NAME_v<stem>` and its uses (the CPlayer precedent):
  `gTotemDefense*` (TotemDefense_Init), `gTotemDefense_kills`, the player-list head, `gMantidQueen` as bytes
  (`#ifndef TARGET_PORT`), and in vm1/Primagen `gLevelScript` (3 files), `gLevelScript_spawnsLeft`,
  `_script_current`, `_timer`, `_values`, `_unk3CA`, `_armed`. No symbol or gendata change was needed.
- *Calls that pass more than the definition takes* go through a cast of their own: `COnScreen__SetBoss` with the
  CAI in a3 (`COnScreen__SetBoss_4`), `Primagen_RunSpawnStep`/`_RunSoundStep` with three arguments;
  `Sound__Play` with its CVector3 as three words is `Sound__Play_Words` (asm label).
- `LevelScriptF950` once, in `structs/twin_primagen.h`; the two `State` views renamed by file; `CInstancePool__Spawn`
  as `protos/game.h` declares it; `WeightedTable_PickStatic(void *)`, `ScriptAnimator_SetScript(void *, void *)`,
  `CGameObjectInstance__PickRandomAnim(void *, sptr, sptr)` one form each.
- *vm2/Primagen follows vm1*: `tools/twin_text.py --base b9bb8125 vm1/Primagen vm2/Primagen 8f57b138` (57 of 58
  files; `--base` is new).

**A trap: vm2 text is not a port-safe source for vm1.** `tools/twin_text.py --to-vm1` (new: each vm1 file as its
vm2 twin's text) gave vm1/Pak and 13 TotemDefense files merge-ready text at `asm_diff` 0, and the merged ROM
verified, but the port crashed at boot: vm2's `Pak_AllocDirectory` reads the cache handle's data pointer as an
`s32` (the vm2 copy never runs on the port, so its host bugs were never found). Both directories went back to
their own text; `--to-vm1` stays for a check of the N64 side only.

**Left of the part (3):** `vm1/Pak` (its own text has the CCache family in three forms: `protos/cache.h` in 6 files,
`void **CCache__Allocate(s32, s32, s32, char *)` in 3; `Pak_Mount`, `Controller_Lock` `s32`/`void`, `osMotorInit`,
`gPakFileSystems`, `Pak_DecodeText`, `Str_Compare` with two forms; its `.rodata` needs the tail from
`Pak_AllocDirectory` (0x4263DC) defined before `Pak_SaveNoteVerified` (0x426164): `--inline T --before
T:func_00426164`, verified byte-exact with the vm2-text files), `vm1/PakMenu` and `vm2/PakMenu` (128 files each,
not started: no time left).

## Round 28 merge: the state after parts q1-q4

Merged in the order q1, q2, q4, q3 (`--no-ff`). The only textual conflicts were this file (each part's
section kept) and `versions/port_layout.us.json` (regenerated with `mklayout.py` after each merge); the new
`protos/` headers (q1 `sched.h`, `game_object.h`, `cai.h`; q2 `heap.h`, `hash.h`, `osint.h`, `region.h`,
`audio_world.h`, `sndp.h`), `protos/mtxf.h`, `versions/symbols_manual.us.txt`, `shared_rodata.us.txt`,
the yaml and q3's `tools/merge_module.py` (`--before`, `--inline` inside conditional blocks) merged on
their own, with no function declared two ways. After each merge: `asm_diff --base b9bb8125` 0 over the
tree, a clean `make setup` + build + `make verify` OK, the port compiles from a fresh gendata, the merged
files have no implicit declaration under clang `-Wimplicit-function-declaration`, and the nested
`TARGET_PORT` scan is the same 2 known sites (`CPlayer.c`, `SndQueue.c`).

**One port fix at the merge.** q1 merged `code/__scHandleRSP`, whose 17 functions the port replaces
with `port/game/t2sched.c`, but `port/cmake/link.cmake` still listed the 17 old `func_*.c` names, so the
merged `__scHandleRSP.c` was linked as well. On q1 alone the archive happened not to pull it; after q4
the host link failed with 17 multiple definitions. `T2_LINK_REPLACED` now names `__scHandleRSP.c` (the
whole file is the scheduler; it still compiles, in `turok2_game_unlinked`).

**`merge_check.py --rodata-free` over the 14 directories left** (and `vm2/Pak`, one file, nothing to
merge): none passes as it stands. 12 stop at the compiler, first conflict and number of errors:
`code/CMtxF` 12 (`Actors_GetLightVolumeFalloff`, the `ModelVertex`/`PlacedProp`/`VertexGroup` views by
file, `CStaticObject__LoadGeometry`), `code/AI` 17 (`GameObject_GetRegionFloorHeight`: the 7 files
part q1 lists), `vm1/Pak` 19 (`CCache__Allocate`: the three CCache forms, then `Pak_Mount`,
`Controller_Lock`, `osMotorInit`, `gPakFileSystems`, `Pak_DecodeText`, `Str_Compare`), `code/SndVoice`
23 (`HalfToFloat`), `code/VM` 42 (`gPager_entries`), `code/Trail` 52 (`Morph` defined twice),
`code/CParticle` 54 (`CQuaternion__FromDirection`), `vm2/SceneLoad` 67 (`CList__AddHead`),
`code/EnemyAI` 68 (`CGameObjectInstance__PickRandomAnim`), `vm1/PakMenu` 72 (`Str_Compare`),
`code/CGameObjectInstance` 76 (`CAnimationState__Reset`), `vm2/PakMenu` 87 (`CCache__Deallocate`);
2 are refused, `code/CScene` and `vm1/SceneLoad`, whose `.rodata` interleave (part q4: one object with
`static __inline__` loaders is the next try).

**State: 136 modules are one C file; 14 directories hold one function per file.**

| Reason | Directories |
|---|---|
| header work only (first conflicts above) | `code/{AI, EnemyAI, SndVoice}`, `vm2/PakMenu` (4) |
| data the C reads as externs, or `.data` tables in asm (`tools/ld_fill.py`, Crash and part q4's data in place show the way) | `code/{CGameObjectInstance, CMtxF, CParticle, Trail}`, `vm1/PakMenu` (5) |
| one object over the engine and the VM (`.text_vm`; the `.rodata` of `CScene` and `vm1/SceneLoad` interleave) | `code/CScene`, `vm1/SceneLoad`, `vm2/SceneLoad` (3) |
| deferred functions plus header work | `code/VM`, `vm1/Pak` (`--inline T --before T:func_00426164`, part q3) (2) |

None needs a ROM change.

## Round 29, part r4: CMtxF, CStaticObject, Trail, CParticle, CGameObjectInstance (and the CScene header work)

**Merged: `code/CMtxF` (114 files -> `CMtxF.c`), `code/CStaticObject` (10 -> `CStaticObject.c`, a new
module), `code/Trail` (45 -> `Trail.c`), `code/CParticle` (62 -> `CParticle.c`), `code/CGameObjectInstance`
(64 -> `CGameObjectInstance.c`): 295 files -> 5.** Each: header work in the one-function files first
(`merge_check.py --rodata-free` OK, a clean `make setup` + build + `make verify` byte-exact and
`asm_diff --base b8217720` 0), then `merge_module.py --rodata-free` and the module's data in place, a clean
build + `make verify` byte-exact again. `asm_diff` prints a merged file as `NEW` and does not compare its
functions (the base has no file of that name): after a merge only `make verify` checks the code (the
CParticle merge below was caught by it, not by `asm_diff`).

**A module boundary: `code/CStaticObject` (0x2141F0..0x2150B8) out of `code/CMtxF`.** The `.rodata` of the
old CMtxF has two runs of stale linker bytes, the round-27 kind (`tools/modules.py` `RODATA_PAD`): after
`"Lit Vertices"` (0xA63BC, 13 bytes) the bytes `40 18 21` and the word `10600004` run to 0xA63D0 (16), where
`CStaticObject__ConstructCompact`'s slice starts, and after `"stat geometry"` (0xA642C) `00 10` and
`03E00008` run to 0xA6440, where `CLevelActor`'s starts. So 0x2141F0 (16-aligned, `Lighting_` gives way
to `CStaticObject__`) starts another object; `tools/modules.py` has the evidence (its `--write` keeps the
split), `tools/movemods.py` moved the 10 files. The stale bytes are `versions/ld_fill.us.txt` lines
(`code/CMtxF/CMtxF 40 18 21`, `code/CStaticObject/CStaticObject 00 10`) plus a 4-byte `rodata` piece each.

**Data in place** (q4's way: N64-only `const` definitions under the symbol's name through an asm label,
`const T name_def __asm__("symbol") = ...;`, written where the ROM has them; the port keeps gendata's copies):

| Module | Data | Where |
|---|---|---|
| CMtxF | `D_800A5404` (`CVector3` {0, 1, 0}, `CMtxF__SetLookDirection`'s up vector) | after `CMtxF__InvertAffine` |
| CMtxF | `D_800A57BC` `"Lit Vertices"` (`Lighting_BuildLitVertices` reads it as `D_800A57B8 + 4`) | at the end |
| CStaticObject | `sCStaticObjectCacheDesc_LitVertices`, `_StatObjectIndex` | after `CStaticObject__Construct` |
| CStaticObject | `sCStaticObjectCacheDesc_StatGeometry` | at the end |
| Trail | `D_800A652C` 32.0f, `D_800A6530` 1.0f (`Sprite_FromImage` reads them by name) | after `Sprite_FromImage` |
| CParticle | `D_800A6404`..`D_800A6424`, `CParticle__GetDistanceScale`'s 9 constants (its FAKEMATCH body) | before it |
| CGameObjectInstance | `sCGameObjectInstanceCacheDesc_AnimObjectInfo` `"anim object info"` | after `CGameObjectInstance__PickRandomAnim` |
| CGameObjectInstance | `D_800A5AF8` 1.0f (`CGameObjectInstance__AlignToGround`'s) | after it |

The zero words between slices (`D_800A5664`, `D_800A5794`, `D_800A648C`, `D_800A5C14`) are the
next slice's double alignment: the one object's `.rodata` reproduces them.

**A deferred tail: `code/CGameObjectInstance`.** `func_0021F358.c`'s `"anim geometry"` (0xA68E0) lies
between `func_0021E374`'s and `func_0021E6AC`'s literals although its code is at 0x21F358. The 18
functions from 0x21F358 to the end are `DEFERRED` (GCC 2.8 outputs an `inline` function after the others,
its strings at the definition, its float literals when it is output: their slices follow
`func_0021F2B0`'s, in `.text` order): `merge_module.py --inline func_0021F358,...,func_00220208 --before
func_0021F358:func_0021E6AC` (q3's recipe). The two callers of `CGameObjectInstance__LoadGeometry` before
it would expand the inline definition; they call `CGameObjectInstance__LoadGeometry_call`, the N64 symbol by
another C name (the port: a cast of the real call).

Header work (one declaration per name; the rules of rounds 27-28):

- callees defined elsewhere: one prototype per merged file, `void *` for an object pointer the files typed
  differently (pointer arguments only warn); casts where a file passed an integer (`(void *) (sptr) x`);
- a definition earlier in the file keeps its prototype: the callers cast, or the definition takes the
  argument its callers pass and does not read (`Trail_DrawUntextured`'s third, `CParticle__DrawShadow`'s
  view), or, for a body under `#ifdef TARGET_PORT .. #else` with a view type of its own
  (`CGameObjectInstance__SetupAnimation`, `Shadow_DrawBlob`, `CParticle__CheckProximity`), the earlier
  callers declare it without a prototype (pointer and integer arguments only);
- the N64 forms that pass a struct or a float in integer registers call an asm-label name, the port its
  `port/game/t2abi.c` shim: `Actors_GetLightVolumeFalloff_f3`, `CMtxF__SetRotateZ_i`,
  `CMtxF__SetRotateY_i`, `CQuaternion__Slerp_i`, `Sound__Play_f3`, `CQuaternion__FromDirection_r` (the
  struct-return form; port `_byval`), `CGameObjectInstance__GetRotation_o`, `GameObject_GetGroundRotation_o`
  (out-pointer forms; port `_sret`), `CGameObjectInstance__Construct_rec` (13 arguments with `u16` ids),
  `WeightedTable_Add_w` (`s32` words); the `#define X X_shim` lines at file scope are gone (they renamed
  the definition too in one file);
- per-file view types renamed by role (`CompactProp`, `DrawnProp`, `EventProp`; `SetupActor`, `MoveActor`,
  `DrawActor`, `OrientActor`, `RootActor`; `AnimKeyLookup`, `AnimPoseContext`; `B19Spawner`, `B19ProxObj`...),
  one shared type where the layouts agree (`structs/vertex_group.h`: `VertexGroup` over the gbi vertices,
  `Vtx_t` colours / `Vtx_tn` normals; `structs/swoosh.h` `SwooshFrame`; `AnimEventPair`);
- `code/Trail`'s five `rw/abu_types.h` files are on the real types: `Swoosh`, `SwooshNode`,
  `CSwooshSystem`, `structs/morph.h`'s `Morph`, `ScriptAnimator` (`structs/boss.h`), `CQuaternion`; the
  `ObjRecAt28V1` view's `next` at 0 is `SwooshNode.prev`; `RibbonVtxPool_Alloc` returns `sptr`;
- `CParticleSystem__Spawn` takes `protos/game.h`'s `void *` parameters and reads them through casts
  (typed locals initialised from them moved registers: the merged ROM differed in 148 bytes).

**Port.** The whole-game run faulted at frame 198660 in `Lighting_BuildLitVertices`: in the merged
`CMtxF.c` clang inlines `Model_ApplyPackedOffsets` and writes `sLitVertexOffsets` (a sized extern array,
so 16-byte aligned to clang) with `movaps`, and gendata had placed it at 4 mod 16. `tools/port/gendata.py`
`ALIGN16` has it now; `aligncheck.py` did not flag it (the access is far from the `lea`).

**`code/CScene` + `vm1/SceneLoad`: header work, not merged.** Done in the one-function files: the module
uses `protos/cache.h`, `protos/list.h`, `protos/iset.h` (not its own `CCache__`/`CList__`/`CISet__`
prototypes or `rw/abu_protos.h`); `CCache__LoadPersistantSubBlock_v` is the N64 symbol returning the
request it leaves in v0 (the header's N64 prototype is `void`); `protos/scene.h` is replaced in its 58
includers by the prototypes each one uses (every function it declares is defined in this module, so the
merged file declares them from the definitions; the header's `void *` forms conflicted with ~40 typed
definitions); externals one prototype each; per-file types `<name>_<vram>`; the camera pool and settings
array views are asm-label names (`gCameraPoolBytes`, `gSettingsView`). `merge_check --rodata-free
--margs "--vm src/us/vm1/SceneLoad" code/CScene`: 194 error lines -> 22, left: the definitions in
`#ifdef TARGET_PORT .. #else` pairs with typed parameters against callers' `void *` prototypes
(`CScene__GetRegion`, `CScene__ReleaseSectors`, `CScene__UpdateDelayedEvents`,
`CScene__RestoreFiredEvent(s)`, `CScene__ReapplyFiredEvents`, `CScene__DecompressAnimatedInstances`,
`CScene__GetIntelligence`), `CScene__InitPersistentData`'s two-argument call, a one-argument `CISet__GetBlock`
call, `gEngineApp_m_Settings_difficulty`/`gEngineApp_m_Game_m_CameraPool` read through other local types
(`ViewState`, `CGameFromCameraPool`), a type `CScene__DecompressLinks`' early prototype needs before it is
declared (`WorldRecAt1B40C`), `m_GraphicSetsKey` (two local types of one name).

The order question has an answer now, the CGameObjectInstance one: `"binary block"`/`"graphicset"`
(0xA6B38) before `func_00220D00`'s 0.5f, `"level name"` (0xA6BD0, no slice) between `func_0042C194`'s and
`func_00223A94`'s, and the slices of `func_00225354`..`func_00226EBC` after `func_00223A94`'s in `.text`
order fit **one object whose engine functions from `CScene__LoadBinaryBlock` (0x224D54) to the end are
`DEFERRED`**: the two loaders and `CScene__GetLevelName` defined where their strings are, the rest after
`func_00223A94`, all output last in `.text` order; `func_0042C560`'s slice between `func_00226024`'s and
`func_002261A8`'s puts some VM functions in the deferred group too. No split at 0x224ED0 is needed. Next:
the conflicts above, then `merge_module.py code/CScene --vm src/us/vm1/SceneLoad --rodata-free --inline
<the tail> --before <loaders>:<the VM function after their strings>` (callers of a deferred function before
its definition that would expand it call it by an asm-label name, as `LoadGeometry_call`), then
`vm2/SceneLoad` (the VM copy's twin).

| Directory | State after r4 |
|---|---|
| `code/{CMtxF, CStaticObject, Trail, CParticle, CGameObjectInstance}` | merged |
| `code/CScene`, `vm1/SceneLoad` | header work (above); the conflicts listed, then the deferred-tail merge with `--vm` |
| `vm2/SceneLoad` | untouched: follows vm1/SceneLoad |

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
| `.code` (engine) | 255 | 2,576 | 155 modules, 289 functions: libultra/libaudio/libc (149 ultralib files), libgcc (6) |
| VM module | 45 | 893 | 12 modules, 29 functions: the Controller Pak (`pfs*.c`, `contpfs.c`) |
| VM module, stale copy | 42 | 835 | the same 12 |
| all | 342 | 4,304 | |

(Round 26 o2: 19 more from `.rodata` padding, above. Round 27 p3: 2 more, `CInstancePool` is three objects.
Round 29 r4: 1 more, `CStaticObject` out of `CMtxF`.)

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
| CLevelActorPool | engine | CLevelActorPool__Spawn | CLevelActorPool__Free | 0x00227240 | 9 | 2512 | pad |
| CInstancePool | engine | CInstancePool__SpawnAnimInstance | CInstancePool__Free | 0x00227C10 | 7 | 2256 | rodata pad, rodata pad: at 0xA6C6C (stale 'e\en'; of func_00227AE0, func_00227C10: where CLevelActorPool__ gives way to CInstancePool__) |
| DLightsLoad | engine | DLights_Load | DLights_SwapBuffers | 0x002284E0 | 2 | 1064 | rodata pad, rodata pad: at 0xA6C96 (stale words after 'anim object info'; of func_002280B0, func_002284E0: where CInstancePool__ gives way to DLights_) |
| DLights | engine | DLights_GetAtPoint | CDLightSet__GetNext | 0x00228910 | 6 | 660 | pad |
| CSun | engine | CSunFrameData__Construct | func_00228C30 | 0x00228BB0 | 7 | 136 | pad |
| Color | engine | Color_ShiftHue | Color_Darken | 0x00228C40 | 3 | 1780 | pad |
| CParticle | engine | CParticle__SpawnChild | CParticleSystem__ResolveInteractions | 0x00229340 | 66 | 41088 | pad |
| Trail | engine | Swoosh_NewNode | SwooshSystem_DrawTrails | 0x002333C0 | 45 | 15072 | rodata pad, rodata pad: at 0xA7034 (one candidate) |
| ImpactEvent | engine | ImpactEvent_Send | ImpactEvent_AIStopSound | 0x00236EA0 | 45 | 9520 | rodata pad, rodata pad: at 0xA7144 (of 3 candidates: where Swoosh gives way to ImpactEvent_) |
| Path | engine | Path_ExpandNode | Path_FitsInRegion | 0x002393D0 | 4 | 2864 | rodata pad, rodata pad: at 0xA71FC (of 2 candidates: where ImpactEvent_ gives way to Path_) |
| SweepShapes | engine | Collision_MoveInstance | Circle_SegmentIntersects | 0x00239F00 | 30 | 12624 | rodata pad, rodata pad: at 0xA7208 (one candidate) |
| ModelSweep | engine | Collision_TestModels | Sweep_GetVolumeBounds | 0x0023D050 | 10 | 6096 | rodata pad, rodata pad: at 0xA72B4 (one candidate) |
| Pickup | engine | CPlayer__CollectPickup | Pickup_Feedback | 0x0023E820 | 16 | 4188 | rodata pad, rodata pad: at 0xA72D8 (of 2 candidates: where Sweep_ gives way to CPlayer__) |
| Sweep | engine | Sweep_TestBoxFaces | Collision_CompareInstanceHits | 0x0023F880 | 6 | 3964 | pad |
| Collision | engine | Collision_SweepRegionWalls | Collision_SweepMapWalls | 0x00240800 | 2 | 2052 | pad |
| CollisionRegions | engine | Collision_SweepRegions | Collision_SweepRegions | 0x00241010 | 1 | 2116 | pad |
| CCollisionSetup | engine | CCollisionInfo__Reset | CCollisionSetup__Construct | 0x00241860 | 3 | 252 | pad |
| PlatformCollide | engine | Platform_GetCollisionBox | Platform_HitFace | 0x00241960 | 8 | 3712 | pad |
| AI | engine | AI_InitHealth | AI_DropPickupsByMask | 0x002427E0 | 40 | 17372 | rodata pad, rodata pad: at 0xA7478 (of func_00241E30, func_002427E0: where Platform_ gives way to AI_) |
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
| DList | engine | DList_Init | GameObject_EmitHotPoints | 0x0026D230 | 12 | 3456 | pad |
| LevelEvent | engine | LevelEvent_Fire | LevelEvent_CheckRequiredItem | 0x0026DFB0 | 19 | 1960 | rodata pad, rodata pad: at 0xA8C3C (one candidate) |
| Rumble | engine | Rumble_GetLevelAt | RumblePool_FreeAll | 0x0026E760 | 10 | 1732 | pad |
| RumbleCurve | engine | RumbleCurve_Init | RumbleCurve_GetValue | 0x0026EE30 | 3 | 380 | pad |
| Level | engine | func_0026EFB0 | Level_GetHub | 0x0026EFB0 | 2 | 200 | pad |
| Frustum | engine | Frustum_FromCorners | Plane_TestBox | 0x0026F080 | 8 | 1760 | pad |
| AudioMgr | engine | amCreateAudioMgr | __amDMA | 0x0026F760 | 7 | 2784 | rodata pad, rodata pad: at 0xA8C94 (of 3 candidates: where Plane_ gives way to am*) |
| CAudioWorld | engine | CAudioWorld__Initialize | CAudioWorld__IsSoundBusy | 0x00270240 | 45 | 7136 | rodata pad, rodata pad: at 0xA8CB8 (of 5 candidates: where __am* gives way to CAudioWorld__) |
| SndVoice | engine | Sound_CalcPan | SndVoicePool_ReleaseVoice | 0x00271E20 | 41 | 11540 | rodata pad, rodata pad: at 0xA8CEC (of func_00271B20, func_00271E20: where CAudioWorld__ gives way to Sound_) |
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
| AutomapDraw | vm1 | Automap_RevealRegion | Automap_EdgeVisible | 0x00400000 | 12 | 7728 | region |
| MantidQueen | vm1 | MantidQueen_Init | MantidQueen_GetTurnSide | 0x00401E30 | 43 | 6528 | rodata pad, rodata pad: at 0xC0F14 (of 4 candidates: where Automap_ gives way to MantidQueen_) |
| Primagen | vm1 | Primagen_Hurt | func_00407F0C | 0x004037B0 | 60 | 18276 | rodata pad, rodata pad: at 0xC0F64 (of 6 candidates: where MantidQueen_ gives way to Primagen_) |
| TotemDefense | vm1 | TotemDefense_Complete | TotemDefense_OnEvent | 0x00407F20 | 18 | 4312 | pad |
| MotherBoss | vm1 | MotherBoss_Hurt | MotherBoss_OnDefeatCinemaDone | 0x00409000 | 66 | 14412 | pad |
| BlindOne | vm1 | BlindOne_UpdateMover | BlindOne_SumGroupHealth | 0x0040C850 | 73 | 10728 | pad |
| Cinema#2 | vm1 | Cinema_SplineSecondDerivs | Cinema_IsWaitCue | 0x0040F240 | 44 | 16360 | pad |
| MenuItem | vm1 | MenuItem_FormatBinding | MenuPool_CloseMenu | 0x00413230 | 52 | 16980 | pad |
| MpMenu | vm1 | MpMenu_StartFragTag | DeathmatchMenu_OpenWeapons | 0x00417490 | 72 | 8400 | pad |
| TitleMenu | vm1 | SingleMenu_Init | PauseMenu_Update | 0x00419560 | 38 | 3656 | rodata pad, rodata pad: at 0xC1C28 (of 10 candidates: where the multiplayer menus give way to Single/Title/Pause) |
| ContinueMenu | vm1 | ContinueMenu_Yes | ContinueMenu_No | 0x0041A3B0 | 2 | 108 | pad |
| GameOverMenu | vm1 | GameOverMenu_Init | GameOverMenu_TickCountdown | 0x0041A420 | 3 | 172 | pad |
| CheatMenu | vm1 | CheatsMenu_Init | CheatMenu_FrootyStripesText | 0x0041A4D0 | 48 | 5496 | pad |
| OptionsMenu | vm1 | GameSettings_SetDefaults | OptionsMenu_BrightnessText | 0x0041BA50 | 31 | 3844 | pad |
| StatsMenu | vm1 | StatsMenu_SaveProfile | StatsMenu_InitAfterPak | 0x0041C960 | 15 | 1772 | pad |
| NameEntryMenu | vm1 | NameEntryMenu_Init | NameEntryMenu_IsMatchOver | 0x0041D050 | 8 | 2100 | pad |
| PakMenu | vm1 | SaveGame_Build | PakConfirmMenu_UpdateNo | 0x0041D890 | 128 | 25984 | pad |
| Video | vm1 | Video_ApplyResolution | InventoryMenu_SelectOk | 0x00423E10 | 12 | 3148 | rodata pad, rodata pad: at 0xC2314 (of 3 candidates: where PakConfirmMenu_ gives way to Video_) |
| Mission | vm1 | Mission_LevelObjectivesDone | Mission_LevelObjectivesDone | 0x00424A60 | 1 | 304 | pad |
| MissionMenu | vm1 | MissionMenu_UpdateObjective1Count | func_00425710 | 0x00424B90 | 14 | 3012 | rodata pad, rodata pad: at 0xC2368 (zeros after the string " " up to the next 16; one candidate) |
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
