# Function names

Every function starts as `func_VRAM`. A real name goes into a names file with
the evidence for it, and `tools/apply_names.py` puts it everywhere at once:

```
versions/names.us.txt          names with outside evidence (Turok 3 matches, libultra, LNK)
versions/names/<part>.us.txt   one file per address range, so parallel work never collides
```

Each line is `Name = 0xVRAM; // evidence`. `apply_names.py` refuses a name or an
address given twice, writes `versions/symbols_names.us.txt` (splat's symbol
file), drops the address scan's entry for each named function, and rewrites
`func_VRAM` to the name in every C file and header, in
`versions/function_sizes.us.txt` and in `versions/manual_stubs.us.txt`. File
names stay `func_VRAM.c`, so a file can always be found by address. Then
`make setup` and `make verify`.

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

Names that differ from ours:

| Acclaim (Turok 3) | Our struct | Object |
| --- | --- | --- |
| `CScene` | `CWorld` (structs/world.h) | D_800F7078, the level |
| `CPlayerPool` | `CPlayerList` (structs/player_list.h) | D_80119870.m_PlayerList |
| `CAudioWorld` | `SndMgr` | D_800F2A00, sound and music |

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

## Data names for the next round

Not applied (the tool names functions only). From `versions/names/data_*.txt`,
the parts' notes and their file comments:

| Address | Name | Evidence |
| --- | --- | --- |
| 0x800ACB60 | `gIntelligenceTypeInfo` | per-AI-type {setup, advance, handlers} blocks (n4) |
| 0x800B1790 | `gEnemyBehaviorInfo` | CEnemyBehaviorInfo *[8] by behaviour (n4) |
| 0x800B2628 | `gLevelEventHandlers` | [kind] -> {onActor, onEvent} (n4) |
| 0x800B6D78 | `gEngineModeTable` | Turok 3's name; {Init, Retrace, Draw} x 18 (n5) |
| 0x800F2A00 | `gAudioWorld` | CAudioWorld, Turok 3 audioworld.c (n5) |
| 0x800E6A90 | `gLoopSounds` | looping sounds: free/used CLists (n5) |
| 0x800F478C | `gMusicTrack` | the playing track (n5) |
| 0x800ADF54 | player state table | {enter, update, tuning, flags, anim} rows (n3) |
| 0x800F1ADC / 0x800F1CE0 | `gCollisionResultsP` / `gCollisionResults` | Turok 3's names (n3) |
| 0x0042D510 | `gCollisionSetupTable` | n3 |
| 0x80119878 | player pool `m_nPlayers` | CPlayerPool (n3) |
| 0x800B0720, 0x800B0A80, 0x800B0EA0, 0x800B1220 | enemy mode tables | base table and the three locomotion tables (n3) |
| 0x800AE394 | default weapon mode table | WeaponActor_InitAI (n3) |
| 0x800D81D0 | `gCache` | Turok 3 cache.c (n1) |
| 0x800F7078 | the CScene | n2 (our CWorld) |
| 0x8010ABA0 | the CInstancePool | n2 |
| 0x8010FF34 | the dynamic light set | n2 |
| 0x800AD080 | impact event handlers | n2 |
| 0x801102B8 | the swoosh system | n2 |
| 0x801100F0 | `g_OnScreen` | COnScreen (n5) |
| 0x800F56B0 | the demo player | n5, n8 |
| 0x800C1BB0 | the cinema state pointer | n5 |
| 0x800F5EC0 | ControllerProfile[4] | n5 |
| 0x8011F290 | CSavedPlayers | n5 |
| 0x8012F3A0 | Mantid Queen state (WaveBossState) | n7 |
| 0x8012F608 | Primagen state (LevelScriptState) | n7 |
| 0x8012FB70 | Mother state | n7 |
| 0x80130200 | Blind One state | n7 |
| 0x80130A58 | GfxPool | n8 |
| 0x80130C60 / 0x80130C70 / 0x800C2224 | pak kind / status / cached directory per port | n8 |

## Open questions

- Generic names that may clash with Turok 3's or a library's later:
  `Random_Next`, `CVector3__Add`, `CMtxF__Multiply`, `Angle_Wrap`,
  `RoundToPowerOfTwo` (n1).
- `CEngineApp__SetupFadeTo`: Turok 3's own comment there is
  "CEngineApp__SetupFadeTo ?" (n5).
- Names by behaviour only, with no Acclaim source to confirm them: most of the
  bosses, menus and AI-type callbacks (both Rage Wars decomps leave those
  `func_`).
- Module-2 C files still call many functions by their link address as
  `func_XXXXXXXX` (about 390 calls, e.g. `func_004263A0` for
  `Pak_ProbeSlot_2`); they could use the module-2 names, as the two fixed cases
  above do.
- `func_00430500` clears the automap's counts and vertex buffers, but is not
  named; the automap struct is still `CDebugLines`.
- 323 C functions are still `func_`.
