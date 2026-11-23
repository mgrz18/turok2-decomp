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

`include/globals/` holds 46 headers with 1,066 declarations. Each header includes
the struct headers its types need, so any file that uses `common.h` can include it.

| header | subsystem | declares |
|---|---|---|
| `adec.h` | Acclaim's compressed-audio decoder | Huffman segments (`AdecHuffSeg`), band ends, IMDCT state, the decoder BSS: `sAdecState` (`DecodeState`), the MDCT twiddles and overlap (`f32[128]`), `sMdctFft` (`FftTables *`); the types are `structs/adec.h`'s (23) |
| `ai.h` | enemy AI | alert point and last noise (`CVector3`), AI mode tables (`CAIModeInfo` rows), `gEnemyBehaviorInfo` (`CEnemyBehaviorInfo *[8]`), generator picks, throwable carry joints (`JointPoint`) (27) |
| `audio.h` | sound world and audio manager | `gAudioWorld` (`CAudioWorld`), SDK audio-manager statics, voice and sequence state, `sPitchRatioTable`, `sSoundKindVolumeTable` (`SoundKindVolume`), `sSpeechSound` (`ALSound`), `sCSPlayerFile` (36) |
| `boss.h` | boss fights | `gMantidQueen` (`WaveBossState`), `gLevelScript` (`LevelScriptState`) and their kept field symbols; the VM modules' boss scripts and tables (`BossStep*`, `BossAttack`, `WaveBossMinion`, `gMotherSpawnPoints`) (95) |
| `boss_ai.h` | boss AI | the bosses' AI mode tables (5) |
| `audiomgr.h` | SDK audio manager (round 22) | `gAudioDmaBuffers` (`AMDMABuffer[0x3C]`), `gAudioDmaState` (`AMDMAState`) and its `_firstUsed`/`_firstFree`, `gAudioDmaCache` (`AMCacheEntry *`), `gAudioCmdLists` (`Acmd *[2]`), `gAudioInfo` (`AMAudioInfo *[3]`); types in `structs/audiomgr.h` (7) |
| `automap.h` | automap | the automap matrices, line indices and double-buffered line vertices (`Vtx_t`, `structs/gbi.h`) (8) |
| `batch.h` | render batcher | `sBatch*`, `sDrawItems`, `sSortNodes`, the opaque and translucent sort trees (`Material`, `DrawItem`, `SortNode`, `structs/batch.h`) (14) |
| `cache.h` | resource cache (`CCache`) | `gCache` (`CCache`, `structs/cache.h`), the kept field symbols (`gCache_*`), `gHashTable` (`HashEntry *`), hash sizes, entry counts (24) |
| `camera.h` | cameras | `gEngineApp_m_Game_m_CameraPool` (`CCameraPool`) and its member symbols, `gCameraModeTable` (`CameraMode`), view and visible boxes, vis bits, tint flags, split-screen margins (17) |
| `cheat.h` | cheat menu (VM) | `sCheatCodes` (3) |
| `cinema.h` | cinemas | `gCinemaPlayerP`, `gCinemaEnabled`, `sCinemaLastFrame` (3) |
| `collision.h` | collision | `gCollisionResults` (`CCollisionResults`) and nine kept field symbols, probes, collision test tables, `gCollisionSetupTable` (`CCollisionSetup`) (19) |
| `controller.h` | controllers | SI queue, first port, controller-driver statics (9) |
| `crash.h` | crash screen | `gCrashCauseDesc` (`FlagDesc[28]`), `sCrashFrameBuffer` (`u16 *`), font, cursor, colour (7) |
| `demo.h` | attract demos | `gDemoTable` (2) |
| `enemy.h` | enemy animation | every enemy animation pick table, from `sEnemyAnim<Kind>_<addr>` at 0x800AF6F0 to 0x800B0600, plus `sJoypadTestAnimTable`, all as `CEnemyAnimPick` arrays (74) |
| `engine.h` | engine loop | frame timing (`gFrameIncrement`, `gFrameCount`...), `gEngineModeTable` (`EngineMode`), engine state flags, scheduler statics, thread priorities, `gBootSignature`, `gEngineTimestamp`, `gAssertFile`, the pak check timer, mode text tables (43) |
| `event.h` | level events | `gLevelEventHandlers` (`LevelEventHandler *[14]`) (1) |
| `framebuffer.h` | frame buffers | `gCFB` (`u16 *[]`), `gCurrentFB` (`u16 *`) and the `gRefreshRate` macro for the word after it (2) |
| `frame.h` | frame loop and scheduler | `gFrameData`, frame and scheduler timing (`gCurrentTick`, `gNumTicks*`, `gWaitingFrames` as `CFrameData *[3]`), the engine queue `gGfxFrameMessageQueue`, `gScheduler` (`OSSched`) and `gGfxClient` (`OSScClient`, `structs/sched.h`), `gGfxMsg`, the thread stacks and threads, the RDP fifo, `gIdentityMtx` (`Mtx`), pool buffers (41) |
| `game.h` | game state | `gEngineApp_m_Game` (`CGame`), `gEngineApp_m_Settings` (`GameSettings`), their kept field symbols, `gEngineApp_m_Settings_cheatFlags` (15) |
| `hostio.h` | SN host I/O | the I/O thread and its queues, `gHostIoMemoryBounds` (`MemoryBoundaryInit`), PI and state flags, TLB page masks, the DMA message (`OSIoMesg`) and the queue buffers (18) |
| `hud.h` | `COnScreen` | font, lookup, boss-bar and glyph display-list data, `sCinemaTextFadeOut` (7) |
| `libaudio.h` | SDK libaudio | `alGlobals`, reverb presets, eqpower (8) |
| `libc.h` | SDK libc | printf digit and pad tables (4) |
| `libultra.h` | SDK libultra | `osTvType`, `osRomBase`, `osResetType`, `osMemSize`, `osAppNMIBuffer`, `__osPiDevMgr` (`OSDevMgr`), thread queues (`__OSThreadTail`), VI contexts and modes, PI handles, timers; the BSS of the controller, VI, PI, timer and 64DD code with ultralib's types (`OSPifRam`, `OSIoMesg`, `OSPiHandle`, `__OSEventState`, now in `ultra_types.h`); the cosf/sinf constants (`du`/`fu`) (77) |
| `math.h` | math | random seeds, acos table, light falloffs (4) |
| `menu.h` | menus | `gEngineApp_m_Game_m_MenuPool` (`MenuPool`), statics for the menu screens, sound menu and credits, fonts (21) |
| `menu_data.h` | menu module (VM) | `sPlayerSpeedTable` (`PlayerSpeedTable`) (2) |
| `menu_desc.h` | menu module (VM) | the 127 `MenuDesc` descriptors, both the module-1 and the `_2` copies (127) |
| `object.h` | game objects | intelligence type blocks, unique ids, warp fade table (`CWarpFadeColor`), animation statics (4) |
| `onscreen.h` | HUD | `gEngineApp_m_OnScreen` (`COnScreen`) (1) |
| `pager.h` | VM pager | `gPager` and its symbols (4) |
| `pak.h` | controller paks | rumble paks, pak state and errors, file systems, profiles (`PakProfile`), pak-menu state, save handles (`CMemEntry *`), note identity strings (47) |
| `particle.h` | particles | impact event handlers (`CImpactEventHandler`), projectile scales, trails (5) |
| `path.h` | region path search | `sPathSearch` and the recursion's working variables (`PathNode`, `PathSearch`, `PathCandidate`, `structs/path.h`) (13) |
| `pickup.h` | pickups | pickup rows (`CPickupRow18/14/10`, `CDropPickup`) (8) |
| `player.h` | player | `gPlayerStateTable` (`PlayerStateInfo`), tunings (`CMoveTuning`, `CLookTuning`), button maps, HUD overlay scripts, barrel layouts, weapon actor (53) |
| `region.h` | regions | one-entry `CGameRegion` caches, surface current rates (7) |
| `render.h` | rendering | screen size, video modes, frame buffers, `gDefaultLights` (`CLights1`), init, texture and shadow display lists (`Gfx[n]`), render-mode caches, fog masks (39) |
| `rumble.h` | rumble | `gRumblePool` (`RumblePool`, `structs/rumble.h`), `gRumbleEntries` (2) |
| `save.h` | saves | `gSavedPlayers` (`CSavedPlayers`) (1) |
| `scene.h` | the level | `gScene` (`CScene`) and its kept field symbols, the bit-section buffers, the `CIntelligence` objects for level actors and particles (36) |
| `swoosh.h` | swoosh/trail system (round 22) | `gEngineApp_m_SwooshSystem` (`CSwooshSystem`, `structs/swoosh.h`) (1) |
| `sysrc.h` | system resources | `gHeap` (`CHeap`), `gLoader` (`CLoader`), `gDecompressor` (`CDecompressor`) (3) |
| `text.h` | text | `gCreditsLines`, `gLevelMessages` (`LevelMessage`) (2) |
| `ucode.h` | RSP microcode | the text and data starts of rspboot, F3DEX, L3DEX and aspMain (`u64[]`), `rspbootTextEnd` (8) |

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

All columns use the same counting method: `extern` lines of data in
`src/**/*.c`, not counting function prototypes. A "named" global is anything
except a `D_`/`func_`/`jtbl_` address name. (Recounted at `b4bb727f` with the
round L script the T2 column comes out at 902 lines, 392 names: a few lines
declare two names.)

| | at `ecba2b71` (round G2) | after round T2 | after round L |
|---|---:|---:|---:|
| local `extern` lines of named globals | 5,269 (1,967 files, 1,204 names) | 911 (424 files, 400 names) | 330 (202 files, 165 names) |
| of them with `/* local type */` | 0 | 238 (156 files) | 223 |
| all local data `extern` lines, `D_` names included | 7,117 | 2,744 | 1,573 |
| headers in `include/globals/` | 0 | 40 (837 declarations) | 46 (1,066 declarations) |

After round L the 330 remaining lines split into two groups:

- 211 lines (114 names) redeclare a global that a header already has. All
  but 15 carry a `local type` or `local:` comment; the 15 are listed under
  "Left" below.
- 119 lines (51 names) are globals that no header declares yet.

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

## Round L

Three agents gave the rest of the named globals their header declarations: k2
the SDK, scheduler and frame statics (part A), k3 everything else without a
header (part B), and k1 the engine objects that first needed a struct. They
were merged after the FAKEMATCH branches and before the literal branches;
`tools/asm_diff.py` found 0 differing files after each merge. Merge decisions:

- `gGfxMsg` is k2's `OSScMsg *` (`globals/frame.h`); CEngineApp__Main (k4's
  rewrite on `CEngineApp`) compares it with `(OSScMsg *) &e->m_FrameData[n].m_Msg`.
- `gPendingFB` is declared once, k1's `u16 *` in `globals/frame.h`;
  `__scHandleRetrace` swaps it as a `u16 *`.
- CEngineApp__Main and `Crash_ResetFrameBuffer` (func_0028CEBC) now include
  `globals/framebuffer.h`: `gRefreshRate = 50.0f`, `gCurrentFB = NULL`,
  `sCrashFrameBuffer = gCFB[0]` compile as the local views did.
- `PauseMenu_Close_2` (func_0045328C) frees `&gRumblePool` with its real type
  (k3 had left an `s32` view for k1); the TotemDefense functions
  (func_00407F20, func_0040813C, func_004082A0) take k3's `TotemDefenseState`
  instead of their local `BossFight` view.

### Part A: the SDK, the scheduler and the frame (k2)

libultra's types moved to `include/ultra_types.h` (`OSPifRam`, `OSTask`,
`OSPiHandle`, `OSIoMesg`, `__OSEventState`, `OSIntMask`, `du`/`fu`) and out of
`rw/abu_types.h` and eleven files' local copies; `structs/sched.h` has `OSScMsg`
and `OSScClient`, `structs/adec.h` the decoder's state. The libultra, adec,
HostIo, crash-screen, frame and microcode statics are in their headers (see the
table above); the `al*` filter handlers are prototypes, not `char` views.

### Engine objects (k1)

The system resources and the cache now have their real structs, and their users read them
through those structs: `gHeap` (CHeap), `gLoader` (CLoader), `gDecompressor` (CDecompressor)
in `globals/sysrc.h`; `gHashTable` (HashEntry *) and `gCache` (CCache, Turok 2's layout: one
more CList, memLockedList, than Turok 3's) in `globals/cache.h`; `gRumblePool` / `gRumbleEntries`
in `globals/rumble.h`; `gCFB` / `gCurrentFB` (u16 *) in `globals/framebuffer.h`, with
`gRefreshRate` for the word after `gCurrentFB`; `gPendingFB` is `u16 *` in `globals/frame.h`.
Local externs of these objects (and of the `gCache_*` field symbols): 141 -> 31.

- The CCache functions that kept views of the cache (a `Manager` at gCache_queue, `Pool` at
  gCache_freeEntryCount, `Counters`, `ResourceLists`, `(&gCache_memAgeList)[-2]`) read
  `gCache.<member>`. GCC 2 treats a struct member and a scalar global as two different kinds
  of memory, so where a view kept a load behind a store, `gCache.<member>` keeps it too, and the
  field symbols (`gCache_ageCount`...) do not. `gCache_memAgeList_m_pTail`,
  `gCache_memAgeList_m_nCount` and `gCache_cacheFreeList_m_nCount` have no user left.
- The CHeap__*, CLoader__* and CDecompressor__* functions take the structs (the Rage Wars
  imports' HeapRecAtC/BlockRecAt20/Pool80255720/CartridgeReadWorker views are gone from them);
  CDecompressor__DecompressSync lost its permuter temporaries.
- `CCache__ClearDisplayListLocks` / `CCache__ReleaseDisplayList` keep one `do { } while (0)`
  each: it only changes the assembler's label numbers (`tools/asm_diff.py` compares them).
- Left: `gScheduler` (OSSched, 4 files) waits for `structs/sched.h` (k2 has OSTask in
  ultra_types.h); `gEngineApp_m_SwooshSystem` (17 files) needs a CSwooshSystem layout (the
  ribbon vertex pool at 0, the swoosh list at 0x7514, the timers at 0x7580, the swoosh set
  lists at 0x94D8/0x95A0) that is not worked out yet; `func_00204778` (a FAKEMATCH file)
  still declares the `gCache_*` fields and `gHeap_usedHead` locally;
  `func_0028C0B0` and `func_0026EE40` keep a `local type` of `gCFB`/`gCurrentFB`.

### Part B: globals without a header (k3)

Part B covered every named global still declared only by local `extern`s,
except the SDK/scheduler/frame ones (part A) and the engine objects that
needed new structs (`gHeap`, `gLoader`, ..., the `CCache` files). Counted with
the method above, against `b4bb727f`:

| | before | after |
|---|---:|---:|
| part-B names with no header declaration | 175 (372 lines) | 38 (91 lines) |
| local lines of names a header declares | 213 | 241 (the field symbols' kept views now count here) |
| ... of them without a `local` comment | 59 | 54 (`gCache_*` in the `CCache` files, and older views listed under "Left") |

New headers:

| header | declares |
|---|---|
| `globals/automap.h` | the automap matrices, line indices and double-buffered line vertices (`Vtx_t`, now in `structs/gbi.h`) |
| `globals/batch.h` | the render batcher: `sBatch*`, `sDrawItems`, `sSortNodes`, the opaque and translucent sort trees (`structs/batch.h`: `Material`, `DrawItem`, `SortNode`) |
| `globals/path.h` | the region path search: `sPathSearch` and the recursion's working variables (`structs/path.h`: `PathNode`, `PathSearch`, whose mover is a `Sweep`, `PathCandidate`) |

Added to existing headers: texture-load state, lit-vertex offsets, the
expansion-pak matrices and `sViModeIndex` (`render.h`); the region normal
caches (`region.h`); the animation decoder and skeleton poser statics
(`object.h`); the blast shield, cinema players and death tint (`player.h`);
menu statics, `gCharacterSelectSounds` (`menu.h`, `CharacterSound` in
`structs/menu.h`); `gLoopSounds` (`LoopSounds`, `structs/sound.h`) and its
entries (`audio.h`); `gContPadData` (`OSContPad[4]`) and `gControllers_buttons`
(`controller.h`); demo player and throwable collision field symbols; the
boss field symbols, `gTotemDefense` (`TotemDefenseState`, `structs/boss.h`),
the Mother's statics, `gPrimagenPaths`, the Blind One's mode tables
(`boss.h`, `boss_ai.h`); the pager's field symbols and `gVMPages` (`pager.h`);
the profile field symbols (`pak.h`); `gEngineApp_m_Settings_*`,
`gEngineApp_m_OnScreen_*` field symbols (`game.h`, `onscreen.h`).

A field symbol is declared with the field's type even where every user keeps
an array or struct view (`/* local type */`): the header documents it, and a
file that drops its view gets the right type. 13 older local redeclarations
were dropped because the header type compiles the same.

What part B left (field symbols read through views, the audio DMA state, struct
views local to one or two files) is listed under "Left" below.

## Round 21, part j2

The 468 strings named this round (docs/NAMES.md "Round 21, part j2") have their
declarations in two new headers, as `char []`, with the string in a comment:

| header | declares |
|---|---|
| `menu_text.h` | the menu and controller pak texts: option lines, mission, note, pak-message and cheat-menu lines, multiplayer level and texture-set labels, the menus' printf formats (390) |
| `strings.h` | the engine's strings read by address: `CCache__Request` descriptions, libultra/libaudio `assert(EX)` literals and file names, crash-screen texts, pickup names and messages, kill and cheat messages, `sPakCompanyCode` (78) |

The other new names went to the headers of their subsystem: `gNumTicks1`
(`frame.h`), `gAssertLine`/`gAssertExpr` (`engine.h`), `gVideoDitherFilter`
(`render.h`), `sOnScreenFontTextureBuffer` (`hud.h`), `sAnimDecodeRotation`/
`sAnimRawRange` (`object.h`), `sHashDebugLongestChain` (`cache.h`),
`sCameraDefaultText` (`camera.h`, a NULL `u8 *`).

**`tools/hoist_externs.py HEADER`** drops every local declaration of a global
the header declares (lines without a `local` comment) and includes the header;
`--mark WHY` comments the lines instead, for a file that cannot include it. It
was run over every `include/globals/*.h`; `tools/asm_diff.py --base 7340fcdd`
then failed 12 files (views of a header global: the 11 lines listed under "Left"
below, the two `PakMenu_FreePagesText` that copy `sPakMenuText_Spaces3` as a
`Chars4`, the FAKEMATCH `CCache__DrawDebug`'s `gCache` fields); those files were
restored and redone with the headers they can take or with `--mark`. Functions
declared as data to take their address (`Texture_DecompressCallback`, `__amDMA`,
`__CSPVoiceHandler`, `PlayerDeath_CinemaDone`, `PlayerCinema50_Done`,
`ProPack__Decompress`, `Model_DecompressCallback`,
`CStaticObject__BuildLitVerticesCallback`, `EnemyAI_HeadNodeCallback`) are
prototypes now. One port branch changed with a header (`CCameraViewParams__Construct`
reads `gDefaultVisBits` as the header's scalar).

Counts (a script that counts every name of a multi-name `extern` line, so the
numbers are a little above the method of the table above):

| | at `7340fcdd` | after j2 |
|---|---:|---:|
| local `extern` lines of named globals | 348 (169 files) | 341 (163 files) |
| ... of them with a `local` comment | 251 | 265 |
| ... of names no header declares | 132 (54 names) | 122 (46 names) |
| ... redeclaring a header global without a `local` comment | 20 | 11 |
| local lines of the round's new names (strings and data) | (970 as `D_` externs) | 8, all `local` (the two `PakMenu_FreePagesText`) |

The 970 `D_` extern lines the strings had (856 of the menu texts, 114 of the
engine strings) went to the two headers. Left without a header: the same names
as below minus the callbacks, plus `gT2BitStreamBase` (9 lines, a port-only
symbol in `#ifdef TARGET_PORT` code).

After the round-21 merge (j3's retyped menu files took the j2 rewrites again):
337 local `extern` lines of named globals (261 with a `local` comment), 123
lines (48 names) without a header, 10 redeclaring a header global without a
`local` comment (same counting script).

## Round 22, part k4

The globals round 21 left without a header, and the redeclarations without a comment.

- **`gEngineApp_m_SwooshSystem`** is `CSwooshSystem` (`structs/swoosh.h`), declared in the new
  `globals/swoosh.h` (not `globals/game.h`: the N64 `structs/game.h` does not include
  `structs/swoosh.h`, whose names clash with local views elsewhere). The 17 local `u8 []`/`s32`
  lines in 16 files are gone; callers pass `&gEngineApp_m_SwooshSystem`. The two N64 COnScreen
  bases stay `(u8 *) &gEngineApp_m_SwooshSystem - 0x1C8`: the ROM reuses the swoosh address
  register, and `&gEngineApp_m_OnScreen` makes GCC load a new one.
- **`gScheduler`** is `OSSched` (`globals/frame.h`). `structs/sched.h` now defines PR/sched.h's
  `OSScTask` (tag `OSScTask_s`) and `OSSched` (with Acclaim's 0x80 bytes before the client list)
  for both targets; they were the port-only `T2ScTask`/`T2Sched`. `rw/abu_types.h` dropped its
  `OSSched`/`OSScTask_s`/`OSScClientWork` views for them (`task->list.type` is
  `task->list.t.type`). `port/game/t2sched.c` uses the header; gendata generates `gScheduler`
  typed, not as a 2 x 0x304-byte view.
- **The audio manager** (`structs/audiomgr.h`, `globals/audiomgr.h`): the SDK `AMDMABuffer`,
  `AMDMAState`, `AMAudioInfo` (with `OSScTask`) and Turok's `AMCacheEntry`. `gAudioDmaBuffers`,
  `gAudioDmaState` and its two field symbols, `gAudioDmaCache`, `gAudioCmdLists`, `gAudioInfo`
  had 15 local views in 7 files; all seven use the header now (`amCreateAudioMgr`, `__amMain`,
  `__amHandleFrameMsg`, `__amDmaNew`, `__amDMA`, `__clearAudioDMA`, `AudioDMA_CacheBlock`).
  `globals/audiomgr.h` is separate from `globals/audio.h` because files that include the latter
  still view some of these.
- **Field symbols read through views.** A struct member keeps an array view's code: GCC 2 marks
  both as in-struct memory, so the alias order is the same, while a scalar is not. So
  `gMotherBoss_phase[0]` became `gMotherBoss.phase` (also `_activeMask`, `_liveMask`,
  `_counter`, `gMantidQueen.waveActive`, `gLevelScript.values[0][i]`). Block views became the
  owner's members: `gLevelScript_script2_cursor.unk0/unk4/unk8` and `[-1]` are
  `gLevelScript.script2.cursor/current`, `gLevelScript.script2On`, `.script2.steps`; the
  script3 and scriptCOn blocks likewise. The f32 cooldown view is
  `gLevelScript_script.timer`. Where the view only changed the signedness, the use takes a
  cast: `(u32) gEngineApp_m_Game_m_PlayerList_m_ActiveList_m_nCount`, `(s8)
  gPakProfileRead_character`. The headers now declare these field symbols with the field's
  type (`globals/boss.h`, `globals/game.h`, `globals/pak.h`); users that do not include the
  header keep their `local type` lines.
- **The 10 redeclarations without a comment:** `Primagen_EnterMode6`/`_2` include
  `globals/boss.h` and store `gLevelScript_bossState.state`; the others cannot include their
  header (a clashing view in the same file) and say so with `local:`.

Counts (the round-21 script: `extern` data lines in `src/**/*.c`, named globals only):

| | at `f980f639` | after k4 |
|---|---:|---:|
| local `extern` lines of named globals | 337 | 278 |
| ... with a `local` comment | 261 | 248 |
| ... of names no header declares | 123 (48 names) | 51 (27 names) |
| ... redeclaring a header global without a `local` comment | 10 | 0 |

### After the merge (round 22 coordinator)

Counted again on the merged tree (`count_ext.py`, the method above): **269** local `extern`
lines of named globals in 157 files (k4 left 278: k1 deleted 121 libultra files, and 9 of
their lines with them), 239 with a `local` comment, 51 lines / 27 names without a header, 0
header globals redeclared without a comment.

## Round 24, part m2

The BSS `D_` that were interiors of named objects became member accesses or names (docs/NAMES.md,
"Round 24, part m2", has the list); the new names are declared in their headers:

- `globals/boss.h`: `LevelScriptBlock gLevelScriptBlock` (both targets now; it was a port-only
  `D_8012F604`), `gTotemDefense_warned`.
- `globals/collision.h`: the thirteen `CCollisionInfo gCollisionInfo*` slots.
- `gContStatus_errno` stays a local `u8 []` in Controller_CountConnected (host alignment, NAMES.md).
- `globals/scene.h`: `gScene_unk_30`, `gScene_m_ActorPool_m_pFree/m_pHead/m_pTail` (field symbols).
- `globals/pak.h`: `gPakMenuConfirmed`, `gPakBootPending`, `gCharNoteMenuSaving`, `sSavePortalDone`;
  `globals/adec.h`: `sAdecOutputFrames`; `globals/hostio.h`: `sHostIoThreadStackEnd`, `sHostIoSavedSR`.
- New struct members (additive): `CAutomap`'s union over `m_Counts` (`m_bHasRegion`,
  `m_PlayerYaw`, `m_vPlayerPos`), `Pager`'s `statMaxAge`/`statMinAge`/`statChainAge`/
  `statRegionBytes` over `unkC`.

Left without a header (new): `gUnitQuadVtx` (three files; `globals/render.h` has no `Vtx_t`),
the two PakMenu files that keep `extern s32 gCharNoteMenuSaving` with a `local:` comment
(they view `gPakProfileRead_character`, which `globals/pak.h` types otherwise). The 27 names of
round 22 without a header were not revisited.

## Left

- **Globals without a header (51 lines, 27 names, after round 22 k4).**
  - Field symbols one of whose users includes the owner's header and needs a view that
    shares a base register across fields, so the header cannot declare the field's type:
    `gEngineApp_m_Settings_difficulty` (6 lines; `CScene__BuildActorLists` reads `CGame` and
    the camera pool at negative offsets from it), `gEngineApp_m_Game_m_PlayerList_m_bCinema`
    (4; `Cinema_Update` reads `match.unk44` 0x1450 bytes on from it),
    `gEngineApp_m_Settings_match_seconds/_minutes/_hours` (6; `GameNoteMenu_SelectYes`: with
    `gEngineApp_m_Settings.match.seconds` or `gEngineApp_m_Settings_match.seconds` the
    registers move), `gLevelScript_swayPhase` (2; its files view `gLevelScript` as `s32`).
  - Struct views local to one or two files: `gParticleSpriteVtx` (3), `gVideoModeFlags`,
    `gScreenViewport` (2 each), `gPoolBuffers`, `gDLightBuffer`, `sResizedViMode`,
    `gHeap_usedHead`, `sBootMarker`, `sBoneMatrixProfile`/`sBoneMatrixCount`.
  - Not globals of the game: `gT2BitStreamBase` (9, port-only code), `__executable_start`/`etext`
    (the host linker's), `gVirtualRomStart`/`gVirtualModule1Size` (absolute symbols),
    `CList__Construct`/`Automap_RevealRegion` (code bounds in `IsCodePointer`),
    `ProPack__Decompress` (a function read as `u8 []` in `func_00224DF4`), `CartRomHandle`
    (osCartRomInit's `OSPiHandle`, a `PiHandle` view in `func_0029D5D0`),
    `sBlindOnePhases_update_2` (declared in `globals/boss.h` as a function pointer; the counting
    script misses that form) and a `value` the script takes for a name (`func_0025BC70`'s
    anonymous `struct { u8 value; }` view of `gEngineApp_m_Settings_multiplayer`).
- **Header globals redeclared without a `local` comment:** none (round 22 k4).
- **The 223 `local type` lines** are the list to revisit when a struct gets a
  better layout or a function is rewritten. Most are GCC 2 alias-order cases:
  an array view keeps a load after a store, where a scalar would let GCC hoist it.
