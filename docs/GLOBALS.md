# Global declarations

Every named global has **one declaration, with its real type**, in a header.
C files include the header instead of declaring the global themselves. A file
keeps its own declaration only where the header's type does not compile to the
ROM's code. That line then says so with a `/* local type: ... */` comment, or
with `/* local: ... */` when the file cannot include the header at all.

This page covers where the declarations live, the rule and its exceptions, how
to check that a declaration change keeps the code, and what is left.

## The rule

- **One declaration.** A global is declared once, in `include/globals/<subsystem>.h`.
  The few whose struct header already declared them stay in `include/structs/`
  (see below). No two headers declare the same global. No struct or typedef is
  defined twice, because GCC 2.8 rejects even an identical redefinition.
- **The real type.** The declaration has the type the code uses the global as:
  `CScene gScene`, `GameSettings gEngineApp_m_Settings`, `CCameraPool
  gEngineApp_m_Game_m_CameraPool`, `MenuDesc` descriptors, `Gfx` display lists,
  and sized arrays of row structs such as `CEnemyAnimPick sEnemyAnimIdle_AF6F0[6]`.
  Uses go through the type: `&gScene`, `gScene.m_Mode`, `gEngineApp_m_Settings.paused`,
  `gEngineApp_m_Settings.match.timeLeft`, `gCameraModeTable[mode].update`.
- **Field symbols.** Sometimes the ROM reaches a field by an address of its own.
  Either it uses a `%hi/%lo` pair where GCC would base the access on the struct,
  or only a separate object gives the right load order. In those cases the field
  keeps a symbol named after its path: `gScene_m_WarpState` is
  `&gScene.m_WarpState`. It is declared in the same header as the struct, with
  the field's type. Field symbols are the exception: every use that matches
  through the struct was rewritten to use the member.
- **Local type only with a reason.** A file may need a different view of a
  global: an array where the header has a scalar, `u8 []` where it has a struct,
  or a struct one level down. It then declares the global locally and comments
  the line with `local type:` and the reason. 238 lines do this:

  | reason | lines |
  |---|---:|
  | the field's real type or member path changes the code (field symbols read as `s32`/`u8`/arrays) | 109 |
  | the header declares it, but this view keeps the ROM's code. Usually this is GCC 2 alias order: an array is assumed to alias a store, a scalar gets hoisted past it. Sometimes it is a different base register. | 119 |
  | the file cannot include the header: its own `s32`/`u32` typedefs, preprocessed ultralib files, or a clash with another local view | 10 |

## Where the declarations live

`include/globals/` holds 40 headers with 837 declarations. Each header includes
the struct headers its types need, so any file that uses `common.h` can include it.

| header | subsystem | declares |
|---|---|---|
| `adec.h` | Acclaim's compressed-audio decoder | Huffman segments (`AdecHuffSeg`), band ends, IMDCT state (13) |
| `ai.h` | enemy AI | alert point and last noise (`CVector3`), AI mode tables (`CAIModeInfo` rows), `gEnemyBehaviorInfo` (`CEnemyBehaviorInfo *[8]`), generator picks, throwable carry joints (`JointPoint`) (27) |
| `audio.h` | sound world and audio manager | `gAudioWorld` (`CAudioWorld`), SDK audio-manager statics, voice and sequence state, `sPitchRatioTable`, `sSoundKindVolumeTable` (`SoundKindVolume`), `sSpeechSound` (`ALSound`), `sCSPlayerFile` (36) |
| `boss.h` | boss fights | `gMantidQueen` (`WaveBossState`), `gLevelScript` (`LevelScriptState`) and their kept field symbols; the VM modules' boss scripts and tables (`BossStep*`, `BossAttack`, `WaveBossMinion`, `gMotherSpawnPoints`) (95) |
| `boss_ai.h` | boss AI | the bosses' AI mode tables (5) |
| `cache.h` | resource cache (`CCache`) | the cache fields (`gCache_*`), hash sizes, entry counts (22) |
| `camera.h` | cameras | `gEngineApp_m_Game_m_CameraPool` (`CCameraPool`) and its member symbols, `gCameraModeTable` (`CameraMode`), view and visible boxes, vis bits, tint flags, split-screen margins (17) |
| `cheat.h` | cheat menu (VM) | `sCheatCodes` (3) |
| `cinema.h` | cinemas | `gCinemaPlayerP`, `gCinemaEnabled`, `sCinemaLastFrame` (3) |
| `collision.h` | collision | `gCollisionResults` (`CCollisionResults`) and nine kept field symbols, probes, collision test tables, `gCollisionSetupTable` (`CCollisionSetup`) (19) |
| `controller.h` | controllers | SI queue, first port, controller-driver statics (9) |
| `crash.h` | crash screen | `gCrashCauseDesc` (`FlagDesc[28]`), font, cursor, colour (6) |
| `demo.h` | attract demos | `gDemoTable` (2) |
| `enemy.h` | enemy animation | every enemy animation pick table, from `sEnemyAnim<Kind>_<addr>` at 0x800AF6F0 to 0x800B0600, plus `sJoypadTestAnimTable`, all as `CEnemyAnimPick` arrays (74) |
| `engine.h` | engine loop | frame timing (`gFrameIncrement`, `gFrameCount`...), `gEngineModeTable` (`EngineMode`), engine state flags, scheduler statics, thread priorities, `gBootSignature`, `gAssertFile`, mode text tables (40) |
| `event.h` | level events | `gLevelEventHandlers` (`LevelEventHandler *[14]`) (1) |
| `frame.h` | frame loop and scheduler | `gFrameData`, frame and scheduler timing, pool buffers (21) |
| `game.h` | game state | `gEngineApp_m_Game` (`CGame`), `gEngineApp_m_Settings` (`GameSettings`), their kept field symbols, `gEngineApp_m_Settings_cheatFlags` (15) |
| `hostio.h` | SN host I/O | the I/O thread and its queues, `gHostIoMemoryBounds` (`MemoryBoundaryInit`), PI and state flags, TLB page masks (12) |
| `hud.h` | `COnScreen` | font, lookup, boss-bar and glyph display-list data, `sCinemaTextFadeOut` (7) |
| `libaudio.h` | SDK libaudio | `alGlobals`, reverb presets, eqpower (8) |
| `libc.h` | SDK libc | printf digit and pad tables (4) |
| `libultra.h` | SDK libultra | `osTvType`, `osRomBase`, `osResetType`, `osMemSize`, `osAppNMIBuffer`, `__osPiDevMgr` (`OSDevMgr`), thread queues (`__OSThreadTail`), VI contexts and modes, PI handles, timers (30) |
| `math.h` | math | random seeds, acos table, light falloffs (4) |
| `menu.h` | menus | `gEngineApp_m_Game_m_MenuPool` (`MenuPool`), statics for the menu screens, sound menu and credits, fonts (21) |
| `menu_data.h` | menu module (VM) | `sPlayerSpeedTable` (`PlayerSpeedTable`) (2) |
| `menu_desc.h` | menu module (VM) | the 127 `MenuDesc` descriptors, both the module-1 and the `_2` copies (127) |
| `object.h` | game objects | intelligence type blocks, unique ids, warp fade table (`CWarpFadeColor`), animation statics (4) |
| `onscreen.h` | HUD | `gEngineApp_m_OnScreen` (`COnScreen`) (1) |
| `pager.h` | VM pager | `gPager` and its symbols (4) |
| `pak.h` | controller paks | rumble paks, pak state and errors, file systems, profiles (`PakProfile`), pak-menu state, save handles (`CMemEntry *`), note identity strings (47) |
| `particle.h` | particles | impact event handlers (`CImpactEventHandler`), projectile scales, trails (5) |
| `pickup.h` | pickups | pickup rows (`CPickupRow18/14/10`, `CDropPickup`) (8) |
| `player.h` | player | `gPlayerStateTable` (`PlayerStateInfo`), tunings (`CMoveTuning`, `CLookTuning`), button maps, HUD overlay scripts, barrel layouts, weapon actor (53) |
| `region.h` | regions | one-entry `CGameRegion` caches, surface current rates (7) |
| `render.h` | rendering | screen size, video modes, frame buffers, `gDefaultLights` (`CLights1`), init, texture and shadow display lists (`Gfx[n]`), render-mode caches, fog masks (39) |
| `save.h` | saves | `gSavedPlayers` (`CSavedPlayers`) (1) |
| `scene.h` | the level | `gScene` (`CScene`) and its kept field symbols, the bit-section buffers, the `CIntelligence` objects for level actors and particles (36) |
| `text.h` | text | `gCreditsLines`, `gLevelMessages` (`LevelMessage`) (2) |
| `ucode.h` | RSP microcode | the text and data starts of rspboot, F3DEX, L3DEX and aspMain (`u64[]`) (7) |

These are declared in `include/structs/`, next to the struct they are an
instance of, and are not repeated in `globals/`:

- `gEngineApp` (`structs/game.h`)
- `gDList` (`gfx.h`)
- `gEngineApp_m_pCurrentFrameData` (`structs/frame.h`)
- `gControllers`, `gMergedController`, `gContStatus` (`structs/controller.h`)
- `gSaveBuffer`, `gLoadBuffer` (`structs/save.h`)
- `gPakDirectories` (`structs/pak.h`)
- `gMenuTextStyles`, `gCheatEntry`, `gNameEntries` (`structs/menu.h`)
- `gDemoPlayer` (`structs/demo.h`)

`gEngineApp_m_Settings_cheatFlags` is a symbol for `GameSettings.cheatFlags`,
the first field of `gEngineApp_m_Settings`. It is defined in
`versions/symbols_manual.us.txt` for ld, not given to splat, because splat keeps
one name per address. The 32 cheat toggles (`CheatMenu_Toggle*`) xor it as a
scalar. Written as `gEngineApp_m_Settings.cheatFlags`, GCC would base the
access on a register instead.

## Checking a declaration change

`tools/match_func.py` compares an unlinked object with the ROM and **masks every
relocated field**. That hides offset errors: a `%lo(sym + 4)` that should be
`%lo(sym + 8)` still passes. Typical causes are an array of the wrong element
type, a struct view one field off, or `&x + 4` meant as bytes where C scales by
the element size. These pass `match_func.py` and only fail `make verify`.

Some changes should not move a single byte: renames, header moves, real types,
`gScene_m_X` to `gScene.m_X`. For those, use:

    python3 tools/asm_diff.py                  # working tree vs HEAD, every C file
    python3 tools/asm_diff.py --base <rev> -v  # against another revision, showing the lines

The tool does this:

1. Compiles every C file of both trees to assembly with the Makefile's cpp and
   cc1, honouring each file's `/* cc1-flags: */`.
2. Replaces every `symbol +/- offset` operand by the address it links to. The
   addresses come from the symbol table of `build/turok2.us.elf` plus
   `versions/symbols_manual.us.txt`.
3. Lists the files whose code differs and the files that do not compile.

The whole tree takes about 15 seconds on the build box, so you can run it after
every edit. It names the failing file directly, while `make verify` only says
the ROM differs. `make verify` remains the final check.

## Counts

Both columns use the same counting method: `extern` lines of data in
`src/**/*.c`, not counting function prototypes. A "named" global is anything
except a `D_`/`func_`/`jtbl_` address name.

| | at `ecba2b71` (round G2) | after round T2 |
|---|---:|---:|
| local `extern` lines of named globals | 5,269 (1,967 files, 1,204 names) | 911 (424 files, 400 names) |
| of them with `/* local type */` | 0 | 238 (156 files) |
| all local data `extern` lines, `D_` names included | 7,117 | 2,744 |
| headers in `include/globals/` | 0 | 40 (837 declarations) |

The 911 remaining lines split into two groups:

- 212 lines (92 names) redeclare a global that a header already has. 126 of
  them are annotated local types; the rest are listed under "Left" below.
- 699 lines (308 names) are globals that no header declares yet.

## Round T2

Six agents each took an address range. In that range they gave the named
globals a header declaration with the real type, dropped the local externs, and
rewrote uses as struct members where the code still matched. The branches were
merged in size order (t5, t4, t3, t1, t2, t6). After each merge,
`tools/asm_diff.py` was run against `ecba2b71` and 0 files differed. `make
verify` passed at the end.

| branch | range | headers |
|---|---|---|
| t1 | 0x80000000-0x800B00BF | libultra, render, player, cache, math, scene, object, pickup, collision, particle, enemy, audio |
| t2 | 0x800B00C0-0x800B5117 | ai, audio, region, hud, engine, pak, event |
| t3 | 0x800B5118-0x800C15FF | engine, camera, controller, text, crash, hostio, libaudio, libultra, adec, libc, ucode, cinema |
| t4 | 0x800C1600-0x8011002B | `gScene` as `CScene`; render, pak, cinema, collision, math, cache, audio, controller, menu, camera |
| t5 | 0x8011002C and up | game, pak, frame, menu, scene field symbols, boss, hostio, camera, pager, onscreen, save |
| t6 | VM module data | menu_desc, boss, boss_ai, cheat, menu_data, demo, collision |

Decisions taken during the merge:

- **Same-named headers are merged.** For `audio.h`, `boss.h`, `camera.h`,
  `cinema.h`, `collision.h`, `controller.h`, `engine.h`, `hostio.h`,
  `libultra.h`, `math.h`, `menu.h`, `pak.h`, `render.h` and `scene.h`, the
  header is the union of the branches' declarations under one guard. No global
  ended up declared twice.
- **One row type for the enemy animation pick tables.** t1's `CEnemyAnimPick`
  `{m_Flags, m_Anim}` (`structs/enemy_anim.h`) and t2's `AIAnimPick` were the
  same struct. `CEnemyAnimPick` stays because it follows Acclaim's C prefix.
  Every `sEnemyAnim*` table and `sJoypadTestAnimTable` is now in `globals/enemy.h`.
- **Struct changes from t1 and t2:**
  - `CBlendState` is gone. `gBlendState` is an `s32` and `gBlendState_m_Color`
    an `f32[5]`; the old `gBlendFade` is `&gBlendState_m_Color[3]`.
  - `CEventFlags` became `CImpactEventHandler`.
  - `LevelEventHandler` is in `structs/event.h`.
- **Local-type views win over a newly added header.** In some files, one branch
  kept a local-type view of a global and another branch added the header that
  declares the same global. These files keep the view and do not include the
  header. The other globals they need are declared locally, with a
  `/* local: <header> is not included here ... */` comment. The files are
  `func_002838A4`, `func_0041EDE0`, `func_00421528`, `func_00426814`,
  `func_004268D0`, `func_00457D00`, `func_004595D8`, `func_0045A448` and
  `func_0027ECF0`.
- **Two local views were dropped.** Once another branch gave these files the
  right includes, their local views were no longer needed: `func_00286A04`
  (`gRspInitDL`/`gRdpInitDL` as `Gfx[]`) and `func_00440C20` (the Primagen
  `BossStep` scripts).

## Left

- **Globals without a header (699 lines, 308 names).** The largest groups:
  - Objects that still need a real struct type: `gHeap` (`CHeap`), `gLoader`
    (`CLoader`), `gDecompressor` (`CDecompressor`), `gHashTable`, `gRumblePool`
    (`RumblePool`), `gEngineApp_m_SwooshSystem` (`CSwooshSystem`), `gScheduler`.
  - libultra BSS: `__osContLastCmd`, `__osContNumControllers`, `__osContBuffer`,
    `__osPfsPifRam`, `__osPiAccessQueue`, `__osSiAccessQueue`, `__osDiskHandle`.
  - Frame buffers and the scheduler: `gCFB`, `gCurrentFB`, `gGfxFrameMessageQueue`,
    `gCurrentTick`, `gNumTicks*`, `sCrashFrameBuffer`, `gVMPages`, the thread stacks.
  - Field symbols not declared yet: `gEngineApp_m_Settings_difficulty`,
    `gEngineApp_m_Game_m_PlayerList_m_*`, `gEngineApp_m_OnScreen_m_*`,
    `gLevelScript_path/_script*`, `gMantidQueen_boss`, `gMotherBoss_*`,
    `gTotemDefense_kills`, `gPager_entries/_slots`, `gPakProfileRead_character`.
  - Other subsystems: the automap, the render batcher (`sBatch*`,
    `s*SortFirst/Root`), the region normals (`sRegionNormal`...), the animation
    and pose statics (`sAnimRemap*`, `sPose*`), the audio DMA buffers, and the
    remaining adec tables.
- **Local views of header globals without a `local type` comment (86 lines):**
  - The `CCache` files read the cache through their own struct views and do not
    include `globals/cache.h`: `func_002031E8`, `func_00203330`,
    `func_002034F4`, `func_0020367C`, `func_00203CA0`, `func_002044A8`,
    `func_00204778`, `func_00204DE0`, `func_00206250`, `func_002062F8`,
    `func_00206498`, `func_00206870`.
  - The merge files listed above.
  - A few files that read `gLevelScript_bossState/_bossMode`,
    `gEngineApp_m_Game`, `gEngineApp_m_Settings`, `gRandomValue` or
    `sHostIoSavedTlb` through their own types: `func_00407410`,
    `func_004075F8`, `func_00440410`, `func_004405F8`, `func_004407AC`,
    `func_0041CE6C`, `func_0041CF68`, `func_00455DC0`, `func_00455EBC`,
    `func_00420454`, `func_0028D714`, `func_00211384`, `func_002A13F0`.
- **Two untyped views of boss globals.** `func_00402E6C` and `func_0043EC68`
  keep `u8 []`/`M2C_UNK` views of `gMantidQueen`/`gLevelScript`, which are now
  typed in `globals/boss.h`. The typed versions do not match yet.
- **The 238 `local type` lines** are the list to revisit when a struct gets a
  better layout or a function is rewritten. Most are GCC 2 alias-order cases:
  an array view keeps a load after a store, where a scalar would let GCC hoist it.
