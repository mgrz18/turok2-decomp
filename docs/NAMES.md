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
names stay `func_VRAM.c`, so a file can always be found by address. Then
`make setup` and `make verify`. `python3 tools/apply_names.py --check` runs every
check and writes nothing.

Round N2 (eight parallel parts, then a merge) brought the count from 90 to
**3,932 names**: 3,823 of the 4,146 functions the C files define (649 of those
are module-2 copies), the rest asm. 323 C functions are still `func_`
(643 `func_` text symbols in the ELF, asm included).

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
  (`__osPfsDeclearPage_2.c`, `osPfsChecker_2.c` renamed with them).
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
| g1's literal pools | `Tuning`, `Unk800A71A8`, `Unk800A7990`, `func_8020CA10_G1` structs | float pools, not structs |

The full list comes from scanning every `extern` of a named symbol (round G2
coordinator's script; easy to redo: group the declarations by name, keep the
names with more than one type).

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
- 323 C functions are still `func_`.
