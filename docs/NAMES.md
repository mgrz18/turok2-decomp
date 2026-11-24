# Function and data names

Every function starts as `func_VRAM` and every data symbol as `D_VRAM`. A real
name goes into a names file with the evidence for it, and
`tools/apply_names.py` puts it everywhere at once:

```
versions/names.us.txt              function names with outside evidence (Turok 3 matches, libultra, LNK)
versions/names/<part>.us.txt       function names, one file per address range, so parallel work never collides
versions/names/data_<part>.us.txt  data names (see "Data names" below)
```

Each line is `Name = 0xVRAM; // evidence`. `apply_names.py` refuses a name or an
address given twice, writes `versions/symbols_names.us.txt` (splat's symbol
file), drops the address scan's entry for each named function, and rewrites
`func_VRAM` to the name in every C file and header, in
`versions/function_sizes.us.txt` and in `versions/manual_stubs.us.txt`. File
names stay `func_VRAM.c`, so a file can always be found by address, in its module's
directory since round 23 (`src/us/code/<module>/`, `src/us/vm1|vm2/<module>/`; `python3
tools/srcfiles.py func_VRAM` prints the path, and a function of a merged module is in
`<module>.c`; module names are in [MODULES.md](MODULES.md)). Then
`make setup` and `make verify`. `python3 tools/apply_names.py --check` runs every
check and writes nothing.

Round N2 (eight parallel parts, then a merge) brought the count from 90 to
**3,932 names**: 3,823 of the 4,146 functions the C files define (649 of those
are module-2 copies), the rest asm. 323 C functions are still `func_`
(643 `func_` text symbols in the ELF, asm included).

Round 21 (part j1) named **245 more** C functions (`versions/names/n9.us.txt`, one line of
evidence each): 168 engine and first-module functions and 77 module-2 copies,
plus 10 asm-origin ones. The C files now define **88** `func_` functions (333 before, counted as the
definitions in `src/`), and the ELF has 128 sized `func_` text symbols (372
before), asm included. What is left and why is under "Left as `func_`" below.

## Conventions

- **`CClass__Method`**, Acclaim's own style, for methods of a class Acclaim
  named: `CCache__Request`, `CScene__LoadLevel`, `CPlayerPool__LevelStart`.
  Use Turok 3's or Rage Wars' class name when the object is the same, even if
  our struct has another name (see the table below).
- **`Subsystem_Name`** for code whose class is not known, or that is not a
  method: `Pickup_`, `Trail_`, `Automap_`, `PakMenu_`, `EnemyAnim_`.
- **Library names as the library spells them**: libultra (`osPfsChecker`,
  `__osSiRawStartDma`), libaudio (`alSeqpNew`, `__CSPHandleMIDIMsg`), gu, libc
  (`sprintf`, `strchr`, `_Printf`), Acclaim's audio decoder (`adec*`, `mdct*`)
  and the SN host-I/O API (`HostIo_*`).
- **`_2`** for the second VM module's copy of a first-module function. The
  second module (`virtual_1`, named from 0x0043CC14) is linked 0x39000 lower,
  over the first: each copy is the same code as the first-module function at
  its link address (named address - 0x39000) or a little further on (0xE0 for
  the copies from 0x00459000 up). The few second-module functions that are not
  copies (variant polls) take the name of the copy that would sit there, + `_2`.
- Mode callbacks are named from their table: `PlayerWalk_Enter`/`_Update`
  (player states), `EnemyMode_Move_Update` (EnemyModes), `EnemyAnim_Idle_AF6F0`
  (anim flag + the low address bits of its table, which tell the variants
  apart), `EngineMode00_PakCheck_Init`/`_Retrace`/`_Draw`.

Structs renamed to Acclaim's names in round N3 (each header notes the old
name; the header files keep theirs):

| Now | Was | Header | Object |
| --- | --- | --- | --- |
| `CScene` (+ `CSceneSector`, `CSceneRecord`) | `CWorld` (`CWorldSector`, `CWorldRecord`) | structs/world.h | `gScene`, the level |
| `CPlayerPool` | `CPlayerList` | structs/player_list.h | D_80119870, CGame's first member |
| `CAudioWorld` | `SndMgr` (and its inlines `SndMgr_Lock`/`_Unlock`) | structs/sound.h | `gAudioWorld`, sound and music |
| `CAutomap`, CGame's `m_Automap` | `CDebugLines`, `m_DebugLines` | structs/game.h | CGame+0x1230 (no Turok 3 or Rage Wars name) |

## Where the evidence comes from

- **Turok 3's decomp** (Drahsid/turok3): the same engine two years on.
  `tools/xmatch_t3.py` finds functions identical word for word; many more are
  recognisable by structure and by Turok 3's names in asm and headers
  (`virtual/scene.c`, `audioworld.c`, `cinema.c`, `tengine.c`, `cache.c`).
- **Rage Wars' decomps** (AbuN64/ragewars-decomp and the `rw_junk` tree): the
  same engine a year on. Their `func_XXXX_de` files carry many of the same
  functions; their file paths (`MENU/PAK/...`, `RESMGR/...`, `FRONTEND/...`)
  name the subsystem, and the libultra/libaudio files there come in link order.
- **LibTEngine** and ultralib (`references/ultralib`): libultra, gu, libaudio
  and libc by code, line for line.
- **Strings.** `CCache__Request`'s last argument is a description of what is
  loaded ("anim object info", "texture"...), which names the caller. Menu
  items carry their text ('texture set:', 'crosshairs', 'controller pak: #x'),
  so every menu callback is named from its MenuDesc/MenuItemDesc slot. Crash
  screen and assert strings name their functions.
- **SN64 debug type records** in the `lnk_debug` bin segment (ROM
  0x107000-0x13B5F0; see [LNK-FORMAT.md](LNK-FORMAT.md)). Besides the LNK object
  records, the ROM keeps SN64 type records with Acclaim's names, roughly ROM
  0x12EC00-0x12FE00: the struct `s_CEnemyBehaviorInfo` (ROM 0x12F044:
  `m_SetupFunction`, `m_AdvanceFunction`, `m_pGroundModeTable`,
  `m_pAirModeTable`, `m_pUnderwaterModeTable`), the light types
  (`e_LightTypes`, `CROMLight_t`), `CLinkEffectInfo_t`, and enums with their
  values: `EnemyModes` (ROM 0x12F193; `ENEMY_WAIT_FOR_TRIGGER_MODE` = 0 at
  0x12F1AB, through `ENEMY_NUKED_FROZEN_MODE` 0x3D), `EnemyActions`,
  `ThrowableModes` (0x12EDEF), `DoorModes` (0x12FB35), `ActionModes`,
  `DestructibleModes`, `VidModes`. Each enum member's string is preceded by its
  value. (Part n4's notes give these at ROM 0x14E400-0x14F3F0; the strings are
  at the offsets above.) The enemy mode callbacks are named from `EnemyModes`.
- **Dispatch tables**, which say what each entry is for:
  - D_800ACB60 `[CIntelligence.m_Type]`: per AI type a block {setup, advance,
    then {onActor, onEvent} pairs}; the enemy's block has D_800B1790
    `[behaviour]`, one CEnemyBehaviorInfo each, and the setups install
    CAIModeInfo tables of {mode, enter, update} rows.
  - D_800B2628 `[event kind]`: points 8 bytes into those blocks, the level
    event handlers (`*Remote` = the onEvent half, which only records the change
    for another level).
  - D_800ADF54: the player state table, 0x18-byte rows {enter, update, tuning,
    flags, anim...} (`Player<State>_Enter`/`_Update`).
  - D_800B6D78 `gEngineModeTable`: 18 rows {Init, Retrace, Draw}
    (`EngineModeNN_*`).
- **Copies**: the second VM module was paired with the first by code (every
  word compared with immediates and jump targets masked, against ref.elf).

## Subsystems by address

| Span | Part | Subsystems | Main prefixes |
| --- | --- | --- | --- |
| 0x00200500-0x0021FFFF | n1 | lists, RNC/ProPack, sets, mutex, decompressor, loader, heap, hash table, cache, textures and render batching, matrices/vectors/quaternions, lighting, models, animation | `CList__`, `ProPack__`/`RNC_`, `CISet__`/`CUSet__`, `CLoader__`, `CHeap__`, `CHashTable__`, `CCache__`, `Texture_`, `Render_`, `CMtxF__`, `CVector3__`, `CQuaternion__`, `CStaticObject__`, `CLevelActor__`, `Anim_`, `CAnimationState__`, `CGameObjectInstance__` |
| 0x00220000-0x0023FFFF | n2 | the level (CScene), object pools, dynamic lights, particles, impact events, trails/swooshes, paths, collision sweeps, pickups | `CScene__`, `CInstancePool__`, `CLevelActorPool__`, `CDLightSet__`, `CParticle__`, `CParticleSystem__`, `ImpactEvent_`, `Trail_`, `Swoosh_`, `Path_`, `Sweep_`, `Collision_`, `Pickup_` |
| 0x00240000-0x0025FFFF | n3 | collision, platforms, AI core, the player and its states, weapon wheel, player pool, weapon actors, enemy animation pickers and modes | `Collision_`, `Platform_`, `CAI__`, `AI_`, `CPlayer__`, `Player<State>_`, `CBarrel__`, `CPlayerPool__`, `WeaponActor_`, `WeaponMode_`, `EnemyAnim_`, `EnemyAI_`, `EnemyMode_` |
| 0x00260000-0x0026FFFF | n4 | AI types (enemy behaviours, turret, generator, platform, throwable, door, destructible, action, interactive anim), regions, level events, rumble, frustum, the SDK audio manager, libc memory | `EnemyAI_`, `Turret_`, `Generator_`, `Platform_`, `Throwable_`, `Door_`, `Destructible_`, `Action_`, `CGameRegion__`, `Event_`/`LevelEvent_`, `Rumble_`, `Frustum_`, `am*`, `memset`/`memmove`/`memcmp` |
| 0x00270000-0x0028FFFF | n5 | sound and music, HUD and overlays, cameras, emitters, cinema, path tracks, controllers, the engine app and its modes, scheduler, crash screen, VM pager, libaudio cseq | `CAudioWorld__`, `Sound__`, `SndVoice_`, `Music_`, `COnScreen__`, `COverlay__`, `Hud_`, `CCamera__`, `CCameraPool__`, `CEmitterPool__`, `Cinema_`, `CPathTrack__`, `Controller_`, `CEngineApp__`, `EngineModeNN_`, `Crash_`, `VM_` |
| 0x00290000-0x002AFFFF | n6 | libaudio, gu, libultra io/os, libc, Acclaim's audio decoder, SN host I/O | library names, `adec*`, `mdct*`, `HostIo_` |
| 0x00400000-0x0041FFFF (+ copies) | n7 | first VM module: automap, bosses (Mantid Queen, Primagen, Mother, Blind One), totem defense, cinema system, menu windows and menus, cheats, options, stats, name entry, pak menus | `Automap_`, `MantidQueen_`, `Primagen_`, `MotherBoss_`, `BlindOne_`, `TotemDefense_`, `Cinema_`, `Menu_`, `MenuItem_`, `MenuPool_`, `*Menu_` |
| 0x00420000-0x004307A8 (+ copies) | n8 | first VM module: pak menus and messages, pak driver, string helpers, libultra pfs, CScene level load, player pool, camera pool, demo playback, video modes; module-2 non-copies | `PakMenu_`, `PakMessage_`, `Pak_`, `Str_`, `os*`/`__os*`, `CScene__`, `CPlayer__`, `CCameraPool__`, `Demo_`, `Video_` |

Count per file (after the merge): names.us.txt 90, n1 337, n2 284, n3 522,
n4 329, n5 517, n6 280, n7 1,076 (485 of them `_2`), n8 497 (239 `_2`).

## What the merge fixed

- **Clash**: n2 and n4 both used `Plane_IntersectSegment`. 0x0023C478 (writes
  the hit point) keeps it; 0x0026F400 (returns the fraction) is
  `Plane_SegmentFraction`.
- **Module-2 names in splat** (`tools/apply_names.py`,
  `tools/filter_defined_syms.py`): a module-2 name is defined by its C file at
  the link address, 0x39000 below the name's address. Given to splat as a
  plain symbol it broke the ROM twice: vertex data that merely looks like a
  0x0045xxxx pointer became a reference that linked to the lower address, and
  splat listed C-defined names in `undefined_funcs_auto` at the named address,
  which overrode the C definition and moved every call. Module-2 names now go
  to splat with `can_be_referenced:False`, and the linker scripts drop
  whatever a C object defines. A size line in `function_sizes.us.txt` under the
  old `func_` name also kept splat using that name; it is renamed too now.
- **libultra pfs**: the plain names of seven pfs functions moved to the first
  module's originals (n8); the second module's are `_2`
  (`__osPfsDeclearPage_2.c`, `osPfsChecker_2.c` renamed with them; since round 22 those
  copies are built from ultralib under the `_2` names, docs/LIBULTRA.md, and the files are gone).
- **Module-2 calls by link address**: where the copies sit 0xE0 lower, the
  first module's name at a link address can be another function. `Str_Length`
  (0x00426D8C) is `Pak_BytesToPages_2` for module-2 callers, and 0x0042EB50
  (`COnScreen__Construct`) is `Demo_ReadRunLength_2`; those calls use the
  module-2 names now.
- **Builtins**: `memset`, `memmove`, `memcmp`, `sprintf`, `strlen` and `strchr`
  build and match under those names. `memcpy`, `sinf`, `cosf` and `sqrtf` stay
  unnamed: GCC 2.8 treats them as builtins and the callers stop matching (n6).
- Wrong comments fixed: `EnemyAnim_PickDeath` (not an attack picker),
  `CPathTrack__Advance` (not a sprite animation), COnScreen's boss bars (not
  an arena timer), `__readVarLen` (not `__alCSeqNextDelta`), the Mantid Queen
  (not the Lord of the Dead), CGame+0x1230 (the automap), and GameSettings
  +0x23/+0x28/+0x2D (texture sets) and +0x24/+0x29/+0x2E (crosshairs), now
  named so.

## Data names

Data names live in `versions/names/data_<part>.us.txt`, same line format
(`gName = 0x800XXXXX; // evidence`). `data_seed.us.txt` holds round N3's 24
certain ones (it absorbed n4's `data_n4.txt`).

Conventions:

- **`g` + CamelCase** for a global object or table: `gScene`, `gEngineApp`,
  `gCache`, `gEngineModeTable`. Turok 3's own name when the object is the same
  (`gFrameIncrement`, `gHeap`, `gCollisionResultsP`).
- **`s` + CamelCase** for data only one source file uses: a string or table in
  a file's own `.rodata` (`sCSPlayerFile`, "audio/csplayer.c").
- **`gObject_<field>`** for a symbol that points inside a named object (the
  code takes the field's address directly): `gEngineApp_m_pCurrentFrameData`
  (0x800F7070), `gScene_m_InstancePool` (0x8010ABA0). Field names as the
  struct header spells them.
- **`_2`** for the second VM module's copy, at its *named* address (the copy of
  `gMotherSpawnPoints` 0x00431D20 is `gMotherSpawnPoints_2` 0x0046AC40,
  linked at 0x00431C40), as for functions.
- Library data as the library spells it.
- Skipped: jump tables (`jtbl_`), float and string pools a C file owns that
  nothing else reads, and anything whose object is not certain. A wrong name
  costs more than `D_`.

What `apply_names.py` does with them:

- **splat**: each name becomes a plain symbol in `versions/symbols_names.us.txt`,
  with no type and no size. splat accepts only its own types and spimdisasm's
  (`f32`, `asciz`...), which change how the bytes are disassembled. With a size,
  spimdisasm writes every reference inside the object as `name + 0xN`, and the
  `D_` labels inside it that C still uses vanish (and interior names would
  not be used). The regenerated asm defines and uses the label under the name,
  and `undefined_syms_auto` lists it under the name.
- **Text**: `D_VRAM` becomes the name in every `.c`, `.h` and hand-written
  `.s` under `src/` and `include/` (comments included), and in the tracked
  files that spell data symbols: `symbols_manual`, `foreign_labels`,
  `undefined_funcs_extra`, `symbol_addrs`, `function_sizes`, `manual_stubs`,
  `tools/ghidra/annotations.json`. The generated ones (`undefined_syms_auto`,
  `undefined_funcs_auto`, the linker script, `us/asm`) come back from
  `make setup`. `cuts.us.txt`, `asm_origin.us.txt` and the yaml mention `D_`
  addresses in comments only and keep them; so do the names files.
- **Duplicates**: an address or a name given twice, a name both a function and
  data, an address both: refused, across all the files.
- **`--check`** also reports names that collide with a C keyword, a typedef,
  a struct/union/enum tag, a macro, an `extern` global or a function (ours,
  `references/ultralib`'s, splat's include), syntax errors, names already used
  as fields or locals, addresses nothing refers to by `D_`, `_2` names outside
  the second module (and second-module names without `_2`), and first-module
  files that would take a `_2` name (below).
- `tools/fix_data_align.py` turns `.align` into `.org` by the label's address,
  which it read from the `D_`/`jtbl_` name; named labels are looked up in
  `symbols_names.us.txt`. `tools/ghidra/annotate.py` applies the data names as
  data labels.

Traps:

- **Second-module data.** Its names do not go to splat: a symbol inside
  `virtual_rodata_1` changed how spimdisasm split that pool, and with it a
  word 0x00470100 in libultra's data became a reference to a new label there,
  which links 0x39000 lower (one changed word in the ROM;
  `can_be_referenced:False` does not help, the reference is to the new
  label). They go to ld instead, at the link address
  (`versions/symbols_names_ld.us.txt`, generated and tracked, filtered by
  `filter_defined_syms.py`, which drops the ones a hand-written `.s` defines,
  like `gCollisionSetupTable_2`).
- **Link addresses.** The second module's code and C reach its data at the
  link address, which lies in the first module's range (its data overlaps
  0x4307B0-0x43CA00). `D_<link>` becomes the `_2` name when the link address
  has no name of its own; in a module-2 file that declares `extern ...
  D_<link>` itself, even when it has one (a header's declaration is shared and
  keeps the first module's name). `--check` lists first-module files that would
  get a `_2` name this way.
- **Data inside C.** No C object defines a global data symbol: a function's
  `.rodata` slice in C has only local labels, so a name there (`sCSPlayerFile`)
  is still defined by ld (`foreign_labels`, `undefined_syms_auto`) and never
  twice. `filter_defined_syms.py` drops any script definition an object does
  define.
- **Interior and negative offsets.** Names at addresses inside another named
  object are plain symbols too; nothing is sized, so splat keeps every label.
  C that reaches an object from another symbol (`D_801102B8 - 0x1C8` is
  0x801100F0) keeps working when both are renamed; rewrite it to the right
  name by hand when one is.
- **Tools that read `D_` names**: `auto_match.py` (m2c drafts, `%lo(D_X)`
  rodata ownership, `M2C_FIELD(&D_X...)`) and `fix_cut.py` (`.word D_X`) only
  see `D_` names; a named symbol is invisible to those heuristics.

### Round G2: the globals

Seven parts (g1-g7, one address range each) named the data, then a merge.
**1,204 data names** now: data_seed 24, g1 5, g2 182, g3 102, g4 117,
g5 261, g6 304, g7 228 (19 lines repeat a seed name at the same address),
105 of them `_2`. Of the 2,266 distinct `D_` symbols the C and headers used
before the round, 1,179 are names now; **1,087 are left**, on purpose:

| Left as `D_` | Count | Why |
| --- | --- | --- |
| strings in `.rodata` | 539 | literals one C file owns (`CCache__Request` descriptions, printf formats, asserts): should become C string literals |
| float pools and other `.rodata` | 445 | float/double literals and small constant tables one function owns: should become C literals |
| interiors of `unk*` fields, unclear BSS, labels in code | 103 | listed in the part files' "not named" comments (data_g5, data_g7) |

Jump tables are `jtbl_` and were never in scope.

Round L wrote the pool rows as C literals (docs/MATCHING.md, ".rodata
literals"). Counted the same way over `src/` (1,076 distinct `D_` at
`b4bb727f`), **605 are left**: 91 pool words (53 in the engine's .rdata, 38 in
the VM tail pool), each kept for a reason the MATCHING.md section lists
(a string several files read, linker fill after it, a pool another file
owns); and 514 that are data, not literals (419 in the menu and pak text
tables after .rodata, 95 other data and BSS).

The data by address (each part file has the detail, with the evidence per line):

| Span | Part | What is there |
| --- | --- | --- |
| 0x80000300-0x8000033F | g1 | libultra boot parameters: `osTvType`, `osRomBase`, `osResetType`, `osMemSize`, `osAppNMIBuffer` |
| 0x800A51F8-0x800A81A7 | g1 | the engine's `.rodata`: float pools, `CCache__Request` description strings, jump tables (nothing named) |
| 0x800A81A8-0x800B00BF | g2 | hash/cache constants (Turok 3), scene lights, render/blend state, falloff tables, AI drop tables, `gPlayerStateTable`, `gWeaponInfoTable`, 43 enemy animation pick tables `sEnemyAnim<Kind>_<addr>` |
| 0x800B00C0-0x800B5117 | g3 | AI mode tables (`gEnemyGroundModeTable`...), `gEnemyBehaviorInfo`, `gLevelEventHandlers`, CAudioWorld state, the SDK audio manager's statics, HUD font tables, boot screens' text tables, pak note identity |
| 0x800B5118-0x800C15FF | g4 | cameras, cinema, controllers, visibility, engine timing (Turok 3's tengine.c: `gFrameCount`, `gFrameIncrement`...), thread priorities, scheduler, crash screen, `gBootSignature` ("sexysteve"), libultra/libaudio/libc globals, Acclaim's audio decoder, SN host I/O, RSP microcode symbols |
| 0x800C1600-0x800F7077 | g5 | module-1 boss data in `.data`, `gCinemaPlayerP`, menus, controller pak menus, video modes, frame buffers, stacks, `gHeap`/`gLoader`/`gDecompressor`, `gCache` and its fields, texture/model batching, random numbers, animation statics, level buffers, paths, collision, the audio manager, `gAudioWorld`, `gDemoPlayer`, controllers and rumble, threads |
| 0x800F7078-0x8011002B | g5 | `gScene` (CScene) and the fields the code takes the address of (`gScene_m_InstancePool`...) |
| 0x8011002C-0x8FFFFFFF | g6 | the rest of `gScene`, `gEngineApp_m_OnScreen`, `gEngineApp_m_SwooshSystem`, `gEngineApp_m_Game_*` (CGame), `gEngineApp_m_Settings` (891 uses) and ~50 of its fields, Turok 3's scheduler/frame globals, `gPager`, libultra BSS (Rage Wars' names), the audio decoder, host I/O, the automap, boss states (`gMantidQueen`, `gLevelScript` (Primagen), `gMotherBoss`, `gBlindOne`, `gTotemDefense`), menu state, `gPoolBuffers`, framebuffers, pak state |
| 0x0020xxxx-0x002Axxxx, 0x0042D510-0x0043CAD7 (+ copies) | g7 | the first VM module's data: `gCollisionSetupTable`, boss scripts and tables, `gDemoTable`, ~75 MenuDesc (`g<Menu>MenuDesc`), `sCheatCodes`; and their module-2 copies (`_2`) |

What the merge decided:

- **g7's `_2` addresses** were the link addresses (where the module-2 code
  reaches them, 0xE0 below the module-1 original in that range). They are the
  named addresses now (link + 0x39000), like every other `_2` name; each line
  notes its link address. Interior `_2` names end in `_2`
  (`sBlindOnePhases_update_2`, `sPlayerSpeedTable_scale_2`, `sCheatCodes_unk4_2`).
- **One address, two data**: module 1's `sPrimagenTossTableStage3`
  (0x00431CF0) is also the link address of `sMotherBossDropTablePhase2_2`
  (named 0x0046ACF0). `MotherBoss_UpdateSpotsAndPickups_2` declares the
  token itself, so the tool gives it the `_2` name there; both symbols are
  defined at 0x00431CF0.
- **Seed vs part**: g7's `sMotherBossSpawnPoints(_2)` are the seed's
  `gMotherSpawnPoints(_2)`, kept. The seed's `gMusicTrack` is
  `gAudioWorld_seq_track` (gAudioWorld+0x1D8C, `SndSeq.unk28` renamed `track`).
- **`D_` tokens that are functions** (g7): `CCache__Main`, `Generator_Step`,
  `InteractiveAnim_Step`, `InteractiveAnim_DrawAllModels`, `__amMain`,
  `__amDmaNew`, `alEnvmixerParam`, `viMgrMain`, `__osException`,
  `HostIo_ThreadMain` are spelled so in C now (the `D_` lines in
  `symbols_manual`/`foreign_labels` stay, aliases at the same address).
- **Library names**: data named as libultra spells it (`__osRunQueue`,
  `alGlobals`, `osViModeTable`...) meets ultralib's own `extern`; `--check`
  reports that as a warning now, and the build checks the declarations agree.
- `osMemSize` stays a constant-address `#define` in `HostIo_Init`
  (func_002A2104.c): `extern u32 osMemSize` schedules differently.
- Field renames, all still matching: `CScene.unk_18FDC` -> `m_PendingWarpID`
  (`gScene_m_PendingWarpID`), `SndSeq.unk28` -> `track`. Function renames:
  `AI_SpawnParticlesOnce`/`ByMask` -> `AI_DropPickupsOnce`/`ByMask` (they call
  `GameObject_DropPickupAt`). CPlayer__SetCharacter's local `sound` is
  `modelId`.

Field names the round found but did not apply (each needs a struct change or
is not certain):

- `CEngineApp` +0x19440 is a COnScreen (`COnScreen__Construct((u8 *)e +
  0x19440)`) and +0x19608 the swoosh system: the symbols are
  `gEngineApp_m_OnScreen` and `gEngineApp_m_SwooshSystem`, but CEngineApp
  still has `unk19440` bytes there; the members are to add.
- `CScene.m_PreviewState` / `m_PreviewTimer` (+0x18FE0/+0x18FE4) are read as
  the first usable weapon - 1 and the weapon count (`CBarrel__Open`,
  `CPlayer__GetNextUsableWeapon`): the names look wrong; the symbols keep them.
- `CScene.m_pSections`/`m_nSections` and `m_bStreamAll` look misnamed
  (`CEmitterPool__Advance` reads the first as the emitter definition table, g5).
- `GameSettings` +0x0C is the brightness (`gEngineApp_m_Settings_unkC`);
  +0x16 is the speech volume per SoundMenu_StepSpeechVolume, which sound.h's
  copy calls `musicVolume`: one of the two is wrong.
- `gTotemDefense_kills` (+0x10) lies past the local BossFight struct.
- `D_800D7BE0` is declared `B12Mgr` but only used at offsets >= 0x5F0: it is
  `gCache - 0x5F0`; those accesses belong to gCache.

Names to accept or revisit: g2's `gPlayerStateTable_tuning`/`_anim` (fields
`unk8`/`unk14` in the local structs) and `gWeaponInfoTable_22`
(`&gWeaponInfoTable[22]`); g4's seven aliases into array rows are left as
comments, not names.

Wrong declared types, the input for a types round. 341 data names are declared
with more than one type across the C; many of those are only address uses
(`u8 []`, `char`) and harmless. The ones that matter, where the real type is
known:

| Symbol | Declared as (count) | Real type |
| --- | --- | --- |
| `gScene` | `u8 []` 126, `s32` 71, `CScene` 25, `s32 []` 11, `char`... | `CScene` |
| `gEngineApp_m_Settings` | `s32` 383, `u32` 5, `B19bG8 []`, `GameSettings` 1 | `GameSettings` |
| `gEngineApp` | `u8 []` 13, `s32` 7, `GameSession` 3, `View []`, `FrameSchedule []`, `CEngineApp` 1 | `CEngineApp` |
| `gEngineApp_m_Game_m_PlayerList_m_ActiveList_m_pHead` | `CPlayer *` 13, `void *` 6, `u8 *` 3, `CGameObjectInstance *` 2, `B18Ent *`, `ProjNode *`, `s32` | `CPlayer *` |
| `gEngineApp_m_Game_m_CameraPool` | `u8 []` 16, `char []` 9, `MenuMatchGlobals` 4, `CGameFromCameraPool` 3, `CCameraPool` 1 | `CCameraPool` (the settings are read through it as `CGameFromCameraPool`) |
| `gEngineApp_m_Game_m_MenuPool` | `u8 []` 30, `char []` 15, `s32` 8, `MenuPool` 2, `Globals` 2 | `MenuPool` |
| `gEngineApp_m_Settings_unlockedCheats` | `s32` 179, `u32` 3, `Shape_8011AADC` | `u32` |
| `gEnemyBehaviorInfo` | `ModeHandler *[]` | `CEnemyBehaviorInfo *[8]` (ROM debug type `s_CEnemyBehaviorInfo`) |
| `gBlendState_m_Color` | `f32 [5]`, `O06Cam` | the `m_Color` field of the blend state |
| `gHeap` | `u8 []` 4, `char` 3, `s32` 2 | `CHeap` |
| `gLoader` | `s32` 4, `char []` 4, `u8 []` 2 | `CLoader` |
| `gHashTable` | `s32` 4, `EntryRecAtCV1 *` 2, `ResourceHashEntry *`, `HashEntry *` | the cache's hash table |
| `gCinemaPlayerP` | `Cinema *` 28, `void *`, `func_802457D0_S1 *`, `Shared_MenuContext *` | `Cinema *` |
| `gAudioWorld` | `s32` 23, `u8 []` 3, `char` | `CAudioWorld` |
| `gCache_mutex` | `s32` 8, `OSMesgQueue` 6, `u8 []` 3 | CCache's `mutex` member |
| `gSiMessageQueue` | `u8 []` 8, `OSMesgQueue` 4, `u8` 2 | `OSMesgQueue` |
| `__osContBuffer`, `__osPfsPifRam` | `s32`, `char`, `u8 []`, `OSPifRam` | `OSPifRam` |
| `__osRunningThread` | `OSThread *` 4, `void *` 2, `ThreadNode *`, `Unk0029B150 *` | `OSThread *` |
| `__osPiDevMgr` | `OSDevMgr`, `int`, `s32`, `State` | `OSDevMgr` |
| `gPakDirectories` | `int *` 2, `s32`, `RecordRecAt4V2 *`, `PakDirectory *` | `PakDirectory *` |
| `gPakProfile` | `PakProfile` 7, `u16 []`, `u8 []` | `PakProfile` |
| `gSavedPlayers` | `char`, `s32 []`, `s32`, `CSavedPlayers` | `CSavedPlayers` |
| `gEngineApp_m_Settings_match` | `s32` 7, `S10Game []`, `GameMatch` | `GameMatch` |
| `gCollisionInfoLineOfSight` | `u8 []` 3, `CCollisionInfo` 2, `struct CollisionInfo` 2 | `CCollisionInfo` |
| `gLevelScript` | `s32` 4, `ModeState0040457C` 2, `LevelScriptState` | `LevelScriptState` |
| g1's literal pools | `Tuning`, `Unk800A71A8`, `Unk800A7990`, `func_8020CA10_G1` structs | float pools, not structs (round L: literals now, the pool views are gone) |

The full list comes from scanning every `extern` of a named symbol (round G2
coordinator's script; easy to redo: group the declarations by name, keep the
names with more than one type).

### Round 21, part j2: the `D_` left in the C

`versions/names/data_j2.us.txt`, applied with `tools/apply_names.py`. Counted as
distinct `D_<addr>` tokens in `src/**/*.c` (617 at `7340fcdd`; the method above,
which skipped comments, gave 605): **617 -> 132** (140 with the headers).

| | names | how |
| --- | ---: | --- |
| menu and pak text tables, menu formats | 390 | `s<Owner>Text_<words of the string>`, `s<Owner>Fmt_<spec>`; `_<addr>` where two strings would get the same name (the pak messages repeat lines) |
| engine strings | 78 | `s<Class>CacheDesc_<words>` (`CCache__Request` descriptions), `s<File>Assert{Expr,File}` (libultra/libaudio `assert(EX)`: the literal `"EX"` and `__FILE__`), crash-screen texts, pickup names and messages, kill/cheat messages, `sPakCompanyCode` ("51") |
| other data and BSS | 12 | `gAssertLine`/`gAssertExpr` (the crash screen's assertion), `gNumTicks1` (Turok 3), `gVideoDitherFilter`, `sAnimDecodeRotation`/`sAnimRawRange`, `sHashDebugLongestChain`, `sOnScreenFontTextureBuffer`, `sCameraDefaultText` (a NULL `u8 *`), `sCCacheDesc_Header`, `sTextureCacheDesc_Texture` |
| interiors of `gEngineApp_m_Settings` | 14 `D_` | not names: the code reads them as members now (`weaponEnabled[n]`, `slots[n].m_Deaths/m_Kills/m_Tags/m_Name/m_nCharacter`, `match.teamScore[1]`) |

`tools/asm_diff.py` now resolves names the names tables just added
(`symbols_names*.us.txt`) and a base tree's `D_<addr>` that the rebuilt ELF has
renamed, so a names round compares against the old tree without a rebuild.

**The 140 left** (C files and headers), by kind:

- 11 labels inside `.code` (`D_00200AE0`, `D_0028A020`, `D_002972E0`...): the
  middle of a function another file jumps into or reads as data
  (`ProPack__Decompress`, `__scMain`, `alSavePull`); and 10 in the VM module
  (`D_0042B304`..., the `Primagen_RunSoundStep` pick lists `D_004315C8`-`D_00431618`,
  two `MenuDesc` `D_004388E0`/`D_0043916C`, the module-2 page buffer `D_00469400`), named with the module's data in a later pass;
- 52 pool words (float pools of one function such as `CParticle__GetDistanceScale`'s
  nine, the trail tables `D_800ACE70`-`D_800ACF38`, `D_800C1144` the cheat table
  the data also points at, `D_800C1BC4`/`C8` the cinema words): the MATCHING.md
  ".rodata literals" reasons hold (a pool another file owns, linker fill, data
  the code reads in the pool); they become literals or names when their files merge;
- 67 interiors of BSS objects that already have a name: `gCollisionInfoTestMoveWater+0x20..`
  (`D_800F1B60`-`D_800F1F50`, the collision info records), `gScene` (`D_8010AB90`...,
  `D_8010FED4`, `D_8010FF34`), `gLevelScript` (`D_8012F604`...), `gContStatus`
  (`D_800F677A`/`B`), `gPakWatched+3`, the camera boxes, the pager and host-I/O
  buffers: each wants the member access of its struct (j3's structs), not a name;
  the rest are BSS whose role is not certain yet (`D_8011B114`/`D_8011F144` pak boot
  flags, `D_801309A8`/`AC` note-menu flags, `D_8011AD48` a quad's vertices).

### Round 22, part k4: `D_` interiors

Three `D_` left the C as members of their object: `D_800F283A` is the low half of
`gAudioFrameSize` (`__amHandleFrameMsg` stores `gAudioFrameSize` into a `s16`: GCC
narrows the load), `D_8012F9A8` is `gLevelScript.values[1]` (Primagen_UpdateMode2),
`D_8012F628` is `gLevelScript.targetAngle` (a new `LevelScriptState` field at 0x20,
was `pad020`; Primagen_UpdateMode20 and its copy). **133 left** (129 + the 4 ROM offsets).
The largest group still in the C is `D_8012F604` (65 uses): the word before `gLevelScript`
(`LevelScriptBlock.m_Word`, a port-only type so far).

### Round 24, part m2: the BSS `D_` as members or names

51 of the `D_` round 22 counted as "interiors of named BSS objects" or "BSS whose role is not
certain" left the C. Each batch was checked with `tools/asm_diff.py` (0 differ), the whole tree
against `d2d07205` too (0 of 3,759), and a clean `make setup` + build + `make verify` is OK.
The count of distinct `D_` tokens in `src/` and `include/` (code and comments) went from **142 to
93**; data names from 1,676 to **1,704**.

- **Members of their owner** (the view is gone, the code reads the struct): `gLevelScriptBlock`
  (was `D_8012F604`: `LevelScriptBlock` is an N64 type now, and Primagen_UpdateMode19 and its twin
  read `gLevelScriptBlock.m_Script.<member>`; the `LevelScriptF604` views are gone),
  `gLevelScript.found[1]`/`.bossMode` (`D_8012F978`), `gTotemDefense.warned` (`D_8012FB64`),
  `gScene.unk_30` (`D_800F70A8`, 17 save-menu files), `gScene.m_DLightSet` (`D_8010FF34`),
  `gScene.m_ParticleSystem.m_DrawList.m_pHead` (`D_8010753C`), `gScene.unk_18E5C` (`D_8010FED4`),
  `gScene.m_Geom[4]` (`D_801100D4`), `gEngineApp_m_OnScreen.m_CharacterSmallLookUpPos[0x80 + ch]`
  (`D_80110238`), `gEngineApp_m_Game.m_MenuPool` / `.m_Settings.match` (`D_80119850`, an anchor 0x20
  bytes before CGame), `gEngineApp_m_Settings.match` (`D_8011A6D8`), `.multiplayer` (`D_8011AAD4`),
  `.slots[n].m_Name` (`D_8011AB8B`), `.slots[1]` (`D_8011AB98`), `gEngineApp_m_Game.m_Automap.m_vPlayerPos`
  (`D_8011AAB4`: a new anonymous union over `CAutomap.m_Counts` names Automap_SetPlayer's three
  words), `gPager.stat*` (`D_801206BC`: four new members over `Pager.unkC`) and
  `gPager.slots[0].age` (`D_80120FCC`), `gContStatus[n].status/.errno` (`D_800F677A/B`),
  `&gPakWatched[3]` (`D_800F5EA3`), `gContPadData[n].button` (`D_800F5C66`), the camera boxes'
  `m_vMax` (`D_800F5A18`, `D_800F5A34`), `&gCollisionInfoTrace.m_GroundBehavior` (`D_800F1CC6`).
- **Not interiors: objects of their own, now named** (the named symbol before each ends earlier).
  The thirteen `CCollisionInfo` slots `gCollisionSetupTable` fills (`D_800F1B60`-`D_800F1F50`,
  0x20 apart, not inside `gCollisionInfoTestMoveWater` or `gCollisionResults`):
  `gCollisionInfoTestMoveClimb`, `Particle`, `TestMoveLand`, `Enemy`, `EnemyB`, `NoInstances`,
  `Trace`, `PlatformRider`, `NoGravity`, `ModeDefault`, `PooledActor`, `Gravity`, `FlyingEnemy`
  (data_g5; typed `CCollisionInfo` in `globals/collision.h`, and their users' `s32 []`/`char []`
  views became `.m_dwFlags` / `&info`). And `gUnitQuadVtx`, `gPakMenuConfirmed`,
  `gPakBootPending`, `sAdecOutputFrames`, `sHostIoThreadStackEnd`, `sHostIoSavedSR`,
  `gCharNoteMenuSaving`, `sSavePortalDone` (data_g6, evidence in the table).
- **Field symbols, where the member moves the ROM's registers** (the reason in each case: GCC
  shares one base register for the struct's members, `lw $2,%lo(sym)` becomes `la $3,sym` +
  `lw $2,0($3)`): `gScene_m_ActorPool_m_pFree/m_pHead/m_pTail` (`D_8010AB90/94/98`,
  CPooledLevelActor__Release), `gScene_unk_30` (three of the 20 `D_800F70A8` files:
  PakMenu_InitSaveGame and its twin, and SaveGame_Build_2, where the member keeps the address in a
  register across the store into the save buffer), `gTotemDefense_warned` (TotemDefense_StartAnim's
  one-element array view, like the other `gTotemDefense_*`), `gContStatus_errno` (Controller_CountConnected: the N64 code matches with `gContStatus[n].errno`, but on the host clang then vectorizes the loop into an aligned 16-byte load of `gContStatus`, whose host address is not 16-aligned: aligncheck found a third site and the whole-game run crashed in the title menu; the `u8 []` field symbol keeps it a byte loop). `values[0][2]` (`D_8012F990`) is read
  through the existing `gLevelScript_values` as a one-row `s32 []`: the real `s32 [2][8]` changes
  Primagen_HandleAnimEvent.
- Two comments named Rage Wars addresses as `D_` (`D_8010C080`, `D_8010F328` in `structs/sound.h`,
  `structs/controller.h`): they now say the address.

**Left after m2** (distinct tokens; several only in comments):

- *BSS still as `D_`, more time*: `D_800D7BE0` (5 CCache files: an anchor 0x5F0 bytes before
  `gCache` whose views reach both; needs the cache manager's real struct), `D_800F55F4` (the
  speech player's queue record, a `QueueRec []` view 0xE68 into `gAudioWorld_seq_track`'s
  block), `D_800E6790` (`sLitVertexOffsets + 0x9000`, the end of that table, 2 Anim files),
  `D_800D8E13` (the low byte of a render pass word, one use in func_0020A568),
  `D_801462C8`/`D_801468A0` (`structs/game.h` comments inside `gVMPages`), `D_800F1402` (a
  comment about splat's old labels in `gDLightBuffer`), `D_800DD794`/`D_800DE1E0` (a
  `globals/render.h` comment on `sLitVertexOffsets`).
- *Not data of the game*: `D_8014A000` (the end of the loaded image/BSS: a linker bound read by
  InitializeSystemResources and main), `D_800C894C` (the main stack's bottom the recursion guard
  compares with), `D_8F128560` (an address offset the host-I/O code adds, outside RDRAM), and
  the four absolute ROM offsets.
- *Comments only*: `D_8012F990` (the comment in Primagen_HandleAnimEvent that says where it went),
  `D_8012F9A8` (`structs/boss.h`), `D_80119850` (`structs/game.h`).
- The 52 pool words, 11 labels and 10 VM data names of round 22 are unchanged (not re-done in
  round 24: no time); their reasons above still hold.

### Round 25, part n4: the last BSS views, the VM data names, every `D_` re-checked

`versions/names/data_g8.us.txt` (22 names), the view rewrites and the comment fixes below. Each batch:
`tools/asm_diff.py --base f3e72718` over the whole tree 0 differ, a clean `make setup` + build +
`make verify` OK, the port rebuilt (gendata from scratch, `mklayout.py` rewritten); the demos keep their det md5, the
whole-game run did not until the merge kept the voice tables' old reading ("For the port" below). Distinct `D_` tokens in `src/` and `include/` (`.c`/`.h`, code and comments): **88 -> 42**;
data names 1,704 -> **1,726**.

**The BSS views** (all four are gone):

- `D_800D7BE0` (the "anchor 0x5F0 before `gCache`", 45 uses): not an anchor. The ROM never loads
  0x800D7BE0 (no `%hi/%lo` pair in `build/ref.elf` reaches 0x800D7BE0-0x800D81BF): every access of
  `CCache__Construct` is its own `%hi/%lo` of `gCache` + the member offset, so the Rage Wars
  `B12Mgr` view (its fields at `0x5F0 +`) was only a spelling of `gCache`. The N64 body now writes
  `gCache.memEntries`, `.memAgeList`, `.mutex.queue`, `.blocker`, `.queue`, `.stack`... (the
  running-pointer loops and the `dlistValid` stores through a pointer stay: the ROM's register use).
  The four other CCache files only declared the view; the declarations went, and the symbol left
  `symbols_manual.us.txt`. The cache manager's struct is `CCache` itself; nothing in the ROM uses the
  0x5E0 bytes before `gHashMask`.
- `D_800F55F4` (SpeechPlayer_Update): `SndFx` (`gAudioWorld.fx`) gets its real tail, a `stream`
  record at 0x30 (`sequence`, `head`, `kind`, `tail`: rw_abu's `QueueRec`) over the old pad; the code
  writes `gAudioWorld.fx.stream.head/.kind/.tail/.sequence` and matches.
- `D_800E6790` is `sAnimDecodeUnkC`, a BSS word of its own (the f32 `AnimHeader.fC`, stored by
  Anim_DecodeAnimation and Anim_Decompress before each `Anim_DecodeTracks` pass and read by no code),
  not "the end of `sLitVertexOffsets`" (that table only ends there). Declared in `globals/object.h`.
- `D_800D8E13`: `((u8 *) &sBatchPass)[3]`, the pass word's low byte on the N64. The port read the
  same byte offset before (gendata aliased the symbol to `sBatchPass + 3`), the high byte on a
  little-endian host; kept so the port's output does not change (noted for the port, see below;
  round 26 o3: the host reads the low byte by default, `T2_LEGACY_BATCHKEY` keeps the old byte).

**The VM data names** (round 22's ten):

- `D_004315C8`, `D_004315DC`, `D_00431608`, `D_00431618`: `sPrimagenSoundScript1Picks` ...
  `4Picks`, the `Pick[]` tables the type-2 steps of the Primagen's sound scripts name (read from
  the step args in the VM data: script 1's step 0 is `{2, 0x004315C8}` etc.); declared in
  `globals/boss.h`.
- `D_004388E0`, `D_0043916C`: `gTeamMenuDesc` (the team pick, TeamMenu_*) and `gSingleMenuDesc`
  ('single', SingleMenu_Init), the two `MenuDesc` CPlayer__NewGame opens; in `globals/menu_desc.h`,
  and CPlayer__NewGame passes `&gTeamMenuDesc` / `&gSingleMenuDesc` (the `func_` macros went). The
  module-2 twin (`vm2/CPlayer/func_00466B38.c`) still calls them `func_00438800` / `func_0043908C`
  (link addresses), not `D_` tokens.
- `D_0042B304`, `D_0042B5D0`, `D_0042C9D4`: labels inside the first module's `CScene__LoadLevel` /
  `CScene__Construct` that the second module's asm calls at their link addresses (callbacks); only
  comments of `vm2/SceneLoad` (C since part n3: `func_00462EC0.c`, `func_00463B2C.c`) and
  `vm1/SceneLoad/func_0042AC0C.c` mention them: comments that say what the original code called by address. They stay as
  mentions.
- `D_00469400` (the module-2 page buffer) is no longer in the C.

**Main-code data that round 22 counted as pool words but are objects** (each reached by its own
`%hi/%lo` or by a data pointer, not part of a function's literal pool): `gAIBaseModeTable`
(`D_800B0700`, `CAIModeInfo[27]`, `globals/ai.h`), `sTrailTexture0Points`-`3Points` (`D_800ACE70`,
`98`, `C0`, `D_800ACF38`: the records `sTrailTextures` points at, `globals/particle.h`),
`sDecalSetupDList` (`D_800ACFC8`, a `Gfx[]`), `sVoiceTrackSounds` / `sVoiceNoUnkC4Sounds`
(`D_800B2A34` / `44`, `globals/audio.h`; the port's `t2audio_world.c` uses the names),
`gFinalScoreMenuDesc` (`D_800C1E54`), `sEnterCheatKeyLabels` (`D_800C1144`), `sPakMenuText_Space`
(`D_800C140C`), `sMissionMenuText_Space` (`D_800C1768`), `sBossOverlayScript` / `sBoss2OverlayScript`
(`D_800B2C30` / `40`), `sDeferredAlphaTexture` (`D_800AB680`). Each is declared in its subsystem's header
(docs/GLOBALS.md, "Round 25, part n4").

**Comment mentions** rewritten as the address or the function's name (they named splat labels that
are named functions now: `ProPack__Decompress`, `Texture_DecompressCallback`, `__scMain`,
`VM_ReaderThread`, `alMainBusPull`, `alSavePull`...): the 11 code labels, `D_0042D50C` (14 file
heads), `D_800DD794`/`D_800DE1E0`, `D_80119850`, `D_801462C8`, `D_801468A0`, `D_8012F9A8`,
`D_8012F990`, `D_800F1402`, `D_800B6D1B`.

**The 42 left, each with its reason:**

| `D_` | what | why it stays `D_` |
| --- | --- | --- |
| `D_800A57B8` | f32 1.0 + "Lit Vertices" | in `CMtxF/func_0021375C.c`'s `.rodata`, read by `CMtxF/func_00213A70.c`: a pool another C file owns; a literal when CMtxF is one C file (not merged in round 25: unowned data and header work, docs/MODULES.md) |
| `D_800A5EB0` | f32 127.0 | in `vm1/SceneLoad/func_0042AA80.c`'s `.rodata`, read by the vm2 twin `func_004639A0.c`: the vm2 copy reads the first module's pool (n3's per-part placement keeps that; neither SceneLoad is merged) |
| `D_800C1064` | f32 120.0 | in `vm2/GameOverMenu_2.c`'s `.rodata`, read by `vm1/GameOverMenu.c`: the same, the other way round |
| `D_800C1348` | f32 15.0 | in `vm1/NameEntryMenu/func_0041D2C4.c`'s `.rodata`, read by the vm2 twin: vm2 shared pool |
| `D_800A5404`, `D_800A58F4`, `D_800A5AF8`, `D_800A6404`-`D_800A6424` (9), `D_800A652C`, `D_800A9DFC`, `D_800ACB50`, `D_800ACE68`, `D_800ACE6C`, `D_800ADDB8`, `D_800ADDE4`, `D_800AE344`, `D_800C1BC4`, `D_800C1BC8` (22) | float literals (and `{0, 1, 0}`) of one function each | in `.rodata` no C object owns yet (`asm/data/*.rodata.s` blobs: A6004, A64F4, A66F8, A7004, A712C, AA9FC, ABCA8, C24E0): written as a literal the word would move into the C file's own pool and the blob would keep a copy; they become literals when the blob gets an owner (round 25 n2 found the causes per directory: linker padding = object boundaries to move, data the C reads as externs; none of these modules was merged in round 25) |
| `D_800AE312`, `D_800AE9D2`, `D_800C1CD2`, `D_800C1E7A`, `D_800C203B`, `D_800C21AA` | interiors: +2 of the per-character `s16` pairs of CPlayer's climbing sounds; an `s16` WeaponMode_Charge_Update stores the shot count into; `MenuColorInit[]` +2; `gCharacterSelectSounds` +2; the low byte of `gResolution` (OptionsMenu); the `u16` VI-mode table PakMenu indexes by flags and region | interiors of tables in the unowned blobs (C24E0, ABCA8): base + offset needs the table's type and an owner, and on the host gendata generates the data in host order, where a byte or halfword view of a word reads the other half; they stay symbols of their own until the blob is a C object |
| `D_800B7E60` | `osViModeTable[8]` (NTSC HPN1) | EngineMode17_Switch_Init needs its own `%hi/%lo`; an array element other than `[0]` is not named by convention |
| `D_800B6E60`, `D_800B6E64`, `D_800B7658` | words `__scExec` (two flags) and `VM_HandleTLBMiss` (-1) write, read by no C | inside the unowned ABCA8 blob between other objects' data: their owners (the scheduler's / VM's statics) are not split out, so the role is a guess |
| `D_800C894C` | the bottom of the main stack (`assets/C3074.bin` `.data`) | not a game object: the recursion guard compares the stack pointer with its address |
| `D_8014A000` | the end of the loaded image/BSS | a linker bound (InitializeSystemResources, main) |
| `D_8F128560` | an address offset outside RDRAM | HostIo_MemoryTransfer adds it; not data |
| `D_0042B304`, `D_0042B5D0`, `D_0042C9D4` | code labels (comments of the vm2 SceneLoad C copies and vm1 `func_0042AC0C.c`) | see above |

**For the port:** `Render_SortDrawItems` (`func_0020A568`) keys the sort tree with byte 3 of
`sBatchPass`, which is the low byte on the N64 and the high byte (0) on the host, as before this
round. Using `(u8) sBatchPass` on the host would be the faithful key, but it changes the draw
order and so the det md5s; left for a port round that re-baselines. **Round 26 (part o3):** the host
now keys with `(u8) sBatchPass` (the N64's byte) by default; `T2_LEGACY_BATCHKEY=1` (written by
`--legacy`) keeps the old byte. The det log does not see the draw order: the reference md5s did not
move for it (docs/port/STATUS.md, round 26).

`sVoiceTrackSounds` / `sVoiceNoUnkC4Sounds` are declared without a size on purpose: with `[4]` / `[6]`
clang vectorized the port's scan of them into an aligned 16-byte load and the demos crashed (the
host objects are 4-aligned); docs/GLOBALS.md has the detail.

**Merge (C40): the voice tables changed the port's runs.** Before n4 the two tables had no type, so
gendata emitted them as raw N64 bytes and the host's `SndVoicePool_AllocVoice`
(`port/audio/t2audio_world.c`) read every id byte-swapped (0x78 as 0x78000000): no voice ever got
`hasTrack` or lost `unkC4`. Typed, the host matches the ids as the N64 does, the track voices stop
with the music, and the game's random stream differs from frame 117,323 of the default run on
(`af8bd840` instead of `cd2d68ce`; END 2, 0 lives; the stick combination `aaa175f2`, 1 life). The
other gendata differences of n4 (`gAudioWorld` 0x3050 -> 0x3058 with `SndFx.stream`, the menu
descriptors typed) change nothing: a build with only the two reads swapped back matches the old
run's det log through frame 118,000 and ends on `cd2d68ce`. The merge keeps that swap
(`T2_VOICE_ID`, with a comment) so the reference runs keep their md5s, as n4 did for `sBatchPass`;
the faithful reading is for a port round that re-baselines. **Round 26 (part o3)** made the
faithful reading the default; `T2_LEGACY_VOICE=1` (written only by `fullrun.py`/`playthrough.py
--legacy`) keeps the byte-swapped one, and the reference runs were re-baselined (docs/port/STATUS.md,
round 26).

## Declarations

The named globals now have one declaration each, with the real type, in
`include/globals/<subsystem>.h` (round T2). The layout, the rule for local
views, the strict check (`tools/asm_diff.py`, which closes the relocation-addend
blind spot of `match_func.py`) and what is left are in
[docs/GLOBALS.md](GLOBALS.md).

## Open questions

- Generic names that may clash with Turok 3's or a library's later:
  `Random_Next`, `CVector3__Add`, `CMtxF__Multiply`, `Angle_Wrap`,
  `RoundToPowerOfTwo` (n1).
- `CEngineApp__SetupFadeTo`: Turok 3's own comment there is
  "CEngineApp__SetupFadeTo ?" (n5).
- Names by behaviour only, with no Acclaim source to confirm them: most of the
  bosses, menus and AI-type callbacks (both Rage Wars decomps leave those
  `func_`).
- Module-2 calls (round N3): `apply_names.py` rewrites a module-2 C file's
  `func_<link>` or first-module name at a link address to the name at link +
  0x39000, in code only (comments keep the first module's). Unnamed targets now
  say `func_<named>`. Left: `__osIdCheckSum` in the pfs copies (no function at
  link + 0x39000, in the 0xE0-shifted region) and three `func_` uses that are
  data labels (0x00438728, 0x00438800, 0x0043908C).
- `func_00430500` clears the automap's counts and vertex buffers, but is not
  named.
- 88 C functions are still `func_`: see "Left as `func_`" below.

## Absolute symbols named (round 21 merge)

`D_14A000` and `D_3CAE0`, the two absolute symbols `VM_Initialize`
(func_0028D820) reads, are `gVirtualRomStart` (ROM 0x14A000, where the virtual
segment starts) and `gVirtualModule1Size` (0x3CAE0, the bytes of VM module 1
the expansion pak path copies), defined in `versions/symbols_manual.us.txt`
([SEGMENTS.md](SEGMENTS.md#the-virtual-segments-real-start-round-21)). Four
absolute ROM offsets keep their `D_` (`D_94F`, `D_11059`, `D_29E60`, `D_A4694`).

## Function counts after round 22

k1 deleted 121 libultra game files (one function each, all named): their functions come from
`lib/ultralib` now (docs/LIBULTRA.md). The game C defines **4,025** functions, 3,937 named and
**88** `func_` (unchanged: none of the deleted files was a `func_`); 4,187 function names in all
(asm and ultralib ones included). `_timeToSamples` / `_timeToSamplesNoRound` were swapped and
are fixed (k1); 0x0029E460 is `memcpy` in the ELF (ultralib's alias; the game C still calls it
by its address name, see below). `tools/apply_names.py --check`: 0 errors, 0 warnings on the
merged tree, and a re-run renames nothing.

## Left as `func_` (round 21)

The 88 C functions round 21 did not name, by reason:

- **Empty or constant stubs with no telling caller or table slot** (about 45):
  `void f(void) {}`, `return 0;`, `return 1.0f;`, the empty loop
  `func_0020FF68`, and their module-2 copies (0x00407F0C, 0x0040B308,
  0x0040ED8C, 0x00416474, 0x0042EBB4 and their copies at + 0x39000; 0x0045A240
  and 0x0045DE14 are copies of 0x00421320 and 0x00424EF4, whose results nothing
  reads). Naming them needs a caller or a table that says what they stand in for.
- **Unreferenced setters and getters of unknown structs** (`func_00207908`,
  `func_0020BFB0`, `func_0021810C`, `func_00218218`, `func_002182BC`,
  `func_0021836C`, `func_00218490`, `func_0021851C`, `func_002188AC`,
  `func_0021F5B8`, `func_002267A0`): the code says what they store, not what.
- **Object-type tests with unknown type sets** (`func_00232EEC`,
  `func_00232F38`, `func_00232FBC`, `func_00232018`, `func_002322C0`) and
  **flag tests of unknown flags** (`func_00267D2C`..`func_00267E4C`).
- **Sums over the scene's sectors** (`func_00214108`, `func_00214188`,
  `func_00226D54`): the sector block they add up (CISet block 2, word 4) has no
  name yet.
- **Rage Wars imports whose Rage Wars name is also `func_`** and whose use in
  Turok 2 is unclear: `func_0021B39C`, `func_002322D8`, `func_00238258`,
  `func_00253934`, `func_002551B8`, `func_00268210`.
- **Library functions whose real name changes the code**: `func_002974C0`
  (`cosf`), `func_002982F0` (`sinf`), `func_00298470` (`sqrtf`) and
  `func_0029E460` (`memcpy`). GCC knows these names as builtins and would
  inline or fold the calls, so the build would no longer match
  (`versions/names.us.txt` notes it for `sqrtf`). Tried in round 22 (k4), all
  callers renamed, `tools/asm_diff.py`: `sqrtf` changes 88 files (GCC 2.8 inlines
  `sqrt.s`); `cosf`/`sinf` (through `apply_names.py`, so the names resolve) change
  9 of 102 (`func_0023BE24`, `func_00244E84`, `func_0024FE10`, `func_00250EC0`,
  `func_00259944`, `func_0027D160`, `func_002A073C`, `func_004004D8`,
  `func_00406598`): not inlined, but GCC treats the libm names as `const` calls and
  schedules them and the floats around them differently. A per-file `-fno-builtin`
  would have to go on every caller (and also turns off `memcpy`/`abs`/`fabs` there);
  a `#define cosf func_002974C0` in a header only moves the address name. The port
  would also need its own spelling for each (`port_shim.h`), since a game function
  called `cosf` would take libm's place for the whole host program.
- **`func_0028D380` is `main`**: SN64's `boot_main` (`src/us/asm/boot.s`,
  Turok 3's SNMAIN.s) jumps to `main`, and `versions/symbols_manual.us.txt`
  already gives the address that name. The C keeps `func_0028D380` because GCC
  treats a function called `main` specially and the port links the game's C
  into a host program that has its own `main`.

Asm by origin: round 21 named the SN host-I/O exception code in
`us/asm/A3C88.s` (`HostIo_VectorStubs` .. `HostIo_SaveContext`, 10 functions).
Left: the RNC unpacker's inner routines (0x00200CB0-0x00200F20: hand-written
asm with their own register conventions, in a cut file), the STVM TLB routines
in the engine copy (0x0028EE40-0x0028EEFC: `STVM_MapTLB` is boot.s's name for
the boot copy, and the engine copy starts one word before its cut), and the
watch-register helpers 0x00201074/0x00425710.
