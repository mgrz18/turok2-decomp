# The engine's runtime on PC (Phase 7, part p4)

What the N64 runtime around the game code is (boot, threads, message queues, the frame loop,
audio, controllers and pak, the VM paging), and how each piece becomes host code in the native
build on top of libultraship (LUS). Ends with a dependency-ordered plan to the first frame and the
title screen.

Sources: the matched C in `src/us/code/` (function names from docs/NAMES.md), docs/VMASM.md,
docs/ASM_ORIGIN.md, docs/assets/PORT.md §10, libultraship `759b134` (2026-09-02), Ghostship
`aefb89b`, SpaghettiKart `ce0f935`, Starship `3322643`, and Rage Wars' Windows port
(`references/rw_junk/port`, its `os_shim.c`, `audio_hle.c`, `game_glue.c`).

## TL;DR

- **Threads stay threads.** The engine is built around eight libultra threads that talk only by
  message queues, and it relies on libultra's scheduling (one CPU, strict priority, a send to a
  higher-priority thread runs it before the send returns, `__osDisableInt` as the lock in 55
  files). LUS does not provide threads: its `osSendMesg`/`osRecvMesg` are non-blocking stubs, and
  the HM ports (Ghostship, SpaghettiKart, Starship) delete their game's threads and comment out the
  blocking waits. For Turok 2 that would mean rewriting the loader, cache, decompressor,
  scheduler and audio-manager protocols (56 game files call `osRecvMesg`, 60 `osSendMesg`).
  Instead: **`port/runtime/t2os.c`** (written this round) runs every OSThread on a host thread
  and passes a baton so exactly one runs at a time under libultra's rules. The LUS main loop posts
  the interrupts (VI, SP/DP done, AI, SI) once per host frame and calls `t2os_run_until_idle()`,
  which returns when every game thread is blocked. Test (`port/tests/t2os_test.c`, Turok 2's
  thread graph with the real priorities and queue protocols): 600 ticks, 600 frames, 600 audio
  frames, 600 cache round trips, preemption and priority boost as on hardware, the same
  switch-order hash on every run (deterministic), clean under ThreadSanitizer, 10 µs per switch
  (13 switches per frame: 0.14 ms per frame).
- **Only the first VM module is linked.** The second module (`virtual_1`, 629 C files named
  `*_2` / from 0x0043CC14) is never executed: nothing in the engine or the first module calls or
  points into it (0 `jal` targets from 7,736 engine calls and 2,436 module-1 calls), and
  `VM_Initialize` maps only ROM 0x14A000+ at 0x400000. The native build links the engine (2,504
  files) and module 1 (792 files) and drops the pager (`VM_HandleTLBMiss`, `VM_ManagerThread`,
  `VM_ReaderThread`, the TLB asm). `VM_Alloc` becomes a resident asset read (PORT.md §10.4).
- **Rendering**: the game's own scheduler (`__scMain`, kept) calls `osSpTaskStartGo`; the shim
  records the gfx task and the platform loop hands `data_ptr` to Fast3D
  (`Fast3dWindow::DrawAndRunGraphicsCommands`) after `t2os_run_until_idle()`, on the main thread,
  while every game thread is blocked (no RDRAM snapshot needed, unlike Rage Wars). SP/DP done
  are posted at the next tick. The game's pacing (`gNextTicks` >= 2 retraces per frame, a
  variable time step) then gives 30 fps as on the console.
- **Audio**: keep `__amMain` and libaudio on the CPU as they are; the shim models the AI FIFO on
  the host clock (`osAiGetStatus` full/not full, as Rage Wars does), runs each audio task's Acmd
  list through an ABI1 HLE (Rage Wars' `audio_hle.c`, adapted to little-endian) and pushes the
  PCM to LUS's `AudioPlayerPlayFrame`. Acmd words stay 32-bit: DMA addresses become offsets in a
  host "RDRAM arena" plus a few registered static buffers.
- **First frame** (mode 0 pak check / mode 1 legal screen) needs: link closure, t2os + device
  shims, boot glue, the asset path (loader/cache/decompressor on `T2Asset`, byte swap, the
  64-bit fix-ups of what mode 0 loads: it already calls `CScene__LoadLevel(&gScene, 0, 0)`), and
  the gfx path. Estimate 3-4 weeks of agent rounds to the legal screen, +1 week to the title
  (mode 7), audio +1-2 weeks in parallel. Details in §9.

## 1. The N64 runtime as built

### 1.1 Boot chain

| step | function (file) | what it does | native |
|---|---|---|---|
| 1 | `boot_main` (src/us/asm/boot.s, SN64 `SNMAIN.s`) | `$sp` = 0x803EFFC0, clears TLB entries 0-30, maps 0x200000-0x3FFFFF to physical 0 (2 MB pages, `STVM_MapTLB`), `jalr` main | gone: the port's `main()` |
| 2 | `func_0028D380` "main" | warm-boot test on `gBootSignature` ("sexysteve"), on a cold boot re-copies 1 MB of ROM 0x1000 to 0x80000400 word by word, clears BSS skipping protected areas (`isRestorable`, `Boot_CanOverwrite`), `osInitialize()`, `Engine_Boot()` | gone; host statics start zeroed, `gWarmBoot` = 0 |
| 3 | `Engine_Boot` -> `CEngineApp__Boot` | creates the idle thread (id 0, pri 10) on `gThreadStackIdle` | gone |
| 4 | `CEngineApp__Idle` | creates `mainproc` (id 1, pri 13), drops itself to pri 0, **spins forever** (`goto loop_1`) | **must not run**: a spinning thread never blocks, so the baton never comes back (§2.3) |
| 5 | `mainproc` | build string into `gEngineTimestamp`, `__osSetFpcCsr(csr \| 0x400)` (enables the divide-by-zero FPU trap), `CEngineApp__Main(&gEngineApp)` | port starts this thread directly; FPCSR call is a no-op |
| 6 | `CEngineApp__Main` | `InitializeSystemResources` (heap, decompressor, loader, VM, cache threads), `osCreateViManager(0xFE)`, `GfxPool_Alloc` (frame buffers, display lists; **module-1 code** at 0x425220), `CEngineApp__Construct` (scheduler, gfx client, frame data, `Sound__Initialize` -> audio manager), pools, `CScene__Construct`, `Cont_InitAll`, pak directory, `CEngineApp__SetMode(0)`, then the message loop | kept, with §3 patches |

### 1.2 Threads

All created by game C through `osCreateThread`; the id is the second argument, priorities are the
`gThreadPriority*` table (Turok 3's names).

| id | thread (entry) | pri | created by | waits on | needed on PC |
|---|---|---|---|---|---|
| 0 | idle (`Engine_Idle` -> `CEngineApp__Idle`) | 10, then 0 | `CEngineApp__Boot` | nothing: spins | **no** |
| 1 | main (`mainproc` -> `CEngineApp__Main`) | 13 | `CEngineApp__Idle` | `gGfxFrameMessageQueue` (retrace type 1, frame done type 2, pre-NMI type 4) | yes |
| 7 | scheduler (`__scMain`) | 18 | `osCreateScheduler` in `CEngineApp__Construct` | its queue: VI 0x29A, SP 0x29B, DP 0x29C, pre-NMI 0x29D, command 6 | yes (game code; drives frame pacing and `Controller_Service`) |
| 4 | loader (`CLoader__Main`) | 17 | `CLoader__Construct` | `gLoader.queue` (+ PI event 8) | yes, PI DMA replaced by `T2Asset_ReadInto` (PORT.md §10.4) |
| 5 | cache (`CCache__Main`) | 15 | `CCache__Construct` | `gCache_queue` | yes |
| 3 | decompressor (`CDecompressor__Main`) | 11 (14 while a sync request runs) | `CDecompressor__Construct` | its queue | yes |
| 8 | audio manager (`__amMain`) | 16 | `amCreateAudioMgr` | `gAudioFrameMsgQ` (retrace client, AI event 6) | yes (or a stub while audio is off) |
| 0x63 | VM manager (`VM_ManagerThread`) | 0x93 | `VM_Initialize` | `gPager.completion` (OS_EVENT_FAULT = TLB miss, region alloc/free) | **no** |
| 0x62 | VM reader (`VM_ReaderThread`) | 0x94 | `VM_ManagerThread` | `gPager.pending` | **no** |
| 0 | libultra VI manager (`viMgrMain`) | 0xFE | `osCreateViManager` | VI/counter events | **no**: the shim posts VI itself |
| - | PI manager (`__osDevMgrMain`) | - | `osCreatePiManager`: **never called** (the loader uses `osPiRawStartDma`) | - | no |
| 0x4653 | SN host I/O (`HostIo_ThreadMain`) | - | `HostIo_StartThread` <- `PCinit`: **never called** in retail | - | no (drop `HostIo_*`, `PC*`) |

Events registered (`osSetEventMesg` / `osViSetEvent`): SP 4, DP 9, PRENMI 14 and VI (every
retrace) -> scheduler queue; AI 6 -> `gAudioFrameMsgQ` (message 7); SI 5 -> `gSiMessageQueue`;
PI 8 -> `gLoader.piReplyQueue`; FAULT 12 -> `gPager.completion` (VM); VI 7 / COUNTER 3 -> libultra's
VI manager.

### 1.3 The frame loop

```
VI retrace ─► __scMain (0x29A) ─► __scHandleRetrace:
                 lowers the decompressor back after a boost, appends submitted tasks,
                 swaps a finished frame when its m_nPredictTicks retraces have passed (osViSwapBuffer),
                 starts the next gfx task if the RSP/RDP are free, watchdog (*(u8*)0 = 0 after 1200
                 retraces with no gfx task done), sends `sc` (type 1) to every client,
                 Controller_Service()  ◄── controllers are read on the scheduler thread
              ─► Video_UpdateResolution()  (module-1 code, called every retrace)
main (type 1) ─► CEngineApp__Retrace: mode's Retrace callback (gEngineModeTable[18]),
                 and if gDisplayListCount > 0: CEngineApp__Update
main (type 2: a frame finished) ─► CEngineApp__Update (mode 17: count only)
CEngineApp__Update: gNextTicks = mean retraces of the last frames (min 2; 3 with 3+ players;
                 4 in cinemas), gFrameIncrement = gNextTicks*15/refresh (variable time step),
                 CCache__Advance, CEngineApp__Draw (builds gDisplayList; the automap into the
                 line list), CEngineApp__SubmitGfxTask
CEngineApp__SubmitGfxTask: OSTask type 1, F3DEX.NoN fifo 2.05 (+ a second L3DEX task for the
                 line list when twoTasks), yield buffer, fifo; flags 0x23 on the last task;
                 Sched_SubmitTask (cmd queue + message 6), advances gFrameCount/gEvenOdd
scheduler: __scExec ─► osSpTaskLoad/osSpTaskStartGo; SP done ─► __scHandleRSP; DP done ─►
                 __scHandleRDP: queue or swap the frame, func_00228BBC(frame), task done
                 message (type 2) to gGfxFrameMessageQueue
```

Three frame buffers (`gTotalFramebuffers`), two display lists in flight
(`gDisplayListCount` = 2 at boot). Frame buffers and the Z buffer are carved out of the top of
RDRAM by `GfxPool_Alloc` (with the expansion pak: Z at top-1 MB, colour at top-2 MB). CPU touches
of the frame buffers: only clears (`memset` in `CEngineApp__Main`/`GfxPool_Alloc`) and the crash
screen; display lists name them as colour/depth images (`(u32) gDepthBuffer` in
`func_0027D160`, `func_00286E14`, ...).

### 1.4 The VM paging

docs/VMASM.md asked whether the paging is relocation or overcommit. The runtime code answers it
for the native build:

- `VM_Initialize` (`func_0028D820`): one page region, vaddr 0x400000-0x47FFFF (0x80 pages) backed
  by ROM 0x14A000, 24 page frames at `gVMPages` (0x80132000), LRU chain. With more than 5 MB
  (expansion pak) it instead `CLoader__LoadSync`s 0x3CAE0 bytes of ROM 0x14A000 (module 1, up to
  0x43CAE0) to `extra` = top of RAM - 512 KB (`InitializeSystemResources` keeps that 512 KB out
  of the heap) and maps it with one TLB entry pair (2 x 256 KB): **module 1 is resident and no
  code is paged**. On 4 MB, code pages fault in through `VM_HandleTLBMiss` (OS_EVENT_FAULT ->
  `VM_ManagerThread` -> `VM_ReaderThread` -> `CLoader__LoadSync` of one 4 KB page).
- `VM_Alloc(size, 0, romAddr)` (`CAudioWorld__Initialize`, `CAudioWorld__LoadSfxDataFile`) adds
  regions after 0x480000 (`VM_AllocRegion`), backed by the audio data (0x1E0000, 0x2F194 bytes;
  the sound data 0x3EA7A0) and paged on demand like code.
- **The second module is dead.** Its ROM bytes would sit at vaddr 0x43CC14+ in the paged region
  (and are not loaded at all with the expansion pak), and nothing goes there: scanning the ROM,
  the engine's 7,736 `jal`s and module 1's 2,436 have **0** targets in 0x43CC14-0x46A000, no
  `lui/addiu` pair builds such an address (the 8 hits are constants like 0x43FFFF), and no
  dispatch table holds one. Module 2 is linked at 0x403C14 (over module 1, docs/VMASM.md), which
  is consistent with a stale copy from another link, like the three dead `.code`-tail fragments
  (docs/ASM_ORIGIN.md). Script: §10.
- So the native build links **module 1 only**, at no particular address. The `.text_vm` trick of
  `func_002261A8.c` (engine + VM function in one file) needs nothing on the host; its module-2
  twin `func_00465B04.c` is dropped with the rest of module 2.

### 1.5 Audio

`amCreateAudioMgr` (SDK audio manager, Turok's variant): AI at 22,050 Hz (`ai.rate = 0x5622` in
`CAudioWorld__Initialize`), frame size rounded to 16 samples, 60 DMA buffers of 0x300 bytes,
two Acmd lists, three `AMAudioInfo` (output buffers), the audio thread (id 8, pri 16).
`__amMain` adds itself as a scheduler client and on message 7 (AI event) or every 21st retrace
message produces frames **while `osAiGetStatus()` is not full** (`__amHandleFrameMsg`:
`__clearAudioDMA`, `alAudioFrame` under `Sound__Lock`, submit the audio task (type 2, aspMain),
`Sound__Update`, wait for the task on `gAudioReplyMsgQ`, `osAiSetNextBuffer`). Sample DMA
(`__amDMA`) goes through a cache of 0x300-byte windows filled by
`CLoader__LoadAsync(direct = 1)` at `gAudioRomBase + offset` and returns
`osVirtualToPhysical(buffer)`: a 32-bit physical address that ends up in Acmd words.

Acclaim's decoder (`adec*`, `mdct*`; an MP3-like transform codec) runs on the CPU inside
`alAdecPull` (the loader filter of the "adec" wave type): `adecNextFrame` decodes into one of six
0x200-byte slots in BSS (`D_80125EF0`) and `alAdecPull` emits `A_SETBUFF`/`A_LOADBUFF` with
`sAdecFrame & 0x1FFFFFFF`. The ABI commands Turok's libaudio emits are the standard ABI1 set
(ADPCM, RESAMPLE, ENVMIXER, MIXER, INTERLEAVE, POLEF, LOADADPCM, SETLOOP, DMEMMOVE, CLEARBUFF,
LOADBUFF, SAVEBUFF, SETBUFF, SETVOL, SEGMENT), written as expanded macros in 13 libaudio files
(64 `words.w1` stores, all `(unsigned int)` casts).

### 1.6 Controllers, rumble, Controller Pak

- `Cont_InitAll` (module 1): `gSiMessageQueue` (SI event 5), `gPakAccessQueue` (the pak lock),
  `osContInit`, `Cont_ResetSlot` per port.
- `Controller_Service`, **on the scheduler thread every retrace**: takes the pak lock
  (non-blocking), `Controller_ReadAll` (blocks on `gSiMessageQueue` for the transfer started the
  previous retrace, then `osContGetReadData` or `osContGetQuery`), rumble update, starts the next
  transfer (`osContStartReadData` or `osContStartQuery` when a pak is watched). So the SI
  completion must be posted, or the scheduler blocks.
- Rumble: `osMotorInit/Start/Stop` via `Controller_*Rumble`, `Pak_ProbeRumble`.
- Saves: Controller Pak only (no `osEeprom*` caller). `Pak_*` (module 1) over libultra's pfs C
  (`osPfsInitPak`, `osPfsFindFile`, `osPfsReadWriteFile`, `osPfsAllocateFile`, ...; module 1
  0x427180-0x429xxx), which goes down to `__osContRamRead/Write` and `__osSiRawStartDma`
  (PIF RAM). Engine mode 0 is the pak check: it probes every port before the legal screen.

## 2. Threads and message queues

### 2.1 What LUS and the HM ports do

- LUS (`src/libultraship/libultra/os_mesg.cpp`): `osCreateMesgQueue`, `osSendMesg`, `osJamMesg`,
  `osRecvMesg`, `osSetEventMesg` exist but **never block** (a full or empty queue returns -1);
  there is no `osCreateThread`/`osStartThread`. `os_vi.cpp`: `osCreateViManager` starts a 16 ms
  SDL timer that posts the VI event from SDL's timer thread; the other VI calls are no-ops.
  `os.cpp`: `osContInit`/`osContGetReadData` on its ControlDeck, `osGetTime`/`osGetCount` from
  `steady_clock` at 46.875 MHz, `osMotorInit`/`__osMotorAccess`, `osAiSetNextBuffer` and
  `osSetTimer` stubs. `os_vm.cpp`: `osVirtualToPhysical` = identity, `osMapTLB` no-op. No pfs, no
  SP/DP/AI model.
- Ghostship (SM64), SpaghettiKart (MK64), Starship (SF64): the threads are removed. `main()`
  creates the `GameEngine`, then `while (WindowIsRunning()) push_frame();` where `push_frame`
  runs one iteration of the game loop (`thread5_iteration` / `Graphics_ThreadUpdate`) with the
  blocking `osRecvMesg`s commented out, and the display list goes straight to
  `GameEngine::ProcessGfxCommands` -> `Fast3dWindow::DrawAndRunGraphicsCommands` (with frame
  interpolation). Audio runs on a host `std::thread` (`HandleAudioThread`) that calls the game's
  synthesis (`create_next_audio_buffer`) with the C mixer (`mixer.c`) and
  `AudioPlayerPlayFrame`. Their games have one game thread plus audio; the rewrite is small.
- Rage Wars' port (`os_shim.c`): one Windows thread per OSThread, a baton so only the
  highest-priority runnable one runs, interrupts posted from the platform loop
  (`port_vi_tick`), RSP tasks completed by the shim, an AI FIFO model on the clock, a virtual PIF
  for the Controller Pak, and an "oracle" mode that waits until every game thread is blocked so a
  frame is reproducible. It works (the game is playable); its remaining problems were 32-bit
  guest/big-endian specifics and lost wake-ups that its 10 ms timeout and watchdog paper over.

### 2.2 Why not the HM rewrite for Turok 2

The engine's subsystems are written as message protocols between threads of fixed priorities:
`CLoader__LoadSync` jams a request and blocks (the loader, 17, runs before the caller, 13, gets
back); `CCache__Request` sends to the cache thread, which hands entries to the decompressor and
gets them back; `CDecompressor__DecompressSync` raises the decompressor to 14 and the scheduler
lowers it again two retraces later; the cache's `CMutex` is `__osDisableInt` + a counter + a
message queue, which is only atomic because one thread runs at a time; `VM_Alloc` and the audio
manager wait on reply queues. Counted over the engine and module 1: `osRecvMesg` in 56 files,
`osSendMesg` 60, `osCreateMesgQueue` 14, `__osDisableInt` 33 (55 with libultra), `osJamMesg` 3,
`osSetThreadPri` 5. Making all of that synchronous is a large, error-prone change to matched C, and
every change has to stay out of the matching build. Keeping the threads costs one shim.

### 2.3 t2os (this round)

`port/runtime/t2os.h/.c`, plain C11 + pthreads (macOS, Linux, MinGW winpthreads; on MSVC use
winpthreads or swap in SRWLOCK/CONDITION_VARIABLE, the code is 400 lines).

- Every `osCreateThread` makes a host thread with an 8 MB stack (the game's stack pointer is
  ignored; macOS gives secondary threads only 512 KB by default). A thread runs only when it holds
  the baton (`sCurrent`), handed over through one mutex and a condition variable per thread.
- Scheduling is libultra's: highest priority runs; equal priorities in FIFO order; a running thread
  is preempted only by a strictly higher one (`osStartThread`, `osSendMesg`/`osJamMesg` waking a
  waiter, `osSetThreadPri`, an event posted from a game thread); blocking on an empty/full queue
  hands the baton on. `osYieldThread`, `osStopThread`, `osDestroyThread`, `osGetThreadPri/Id`.
- Queues are the game's `OSMesgQueue` fields (`validCount`, `first`, `msgCount`, `msg`), so code
  that polls with `OS_MESG_NOBLOCK` or reads `validCount` behaves as on hardware.
- `__osDisableInt`/`__osRestoreInt` are no-ops: switches happen only inside the calls above.
- Platform side: `t2os_post_event(e)` (dropped when the queue is full, as hardware does),
  `t2os_vi_retrace()` (OS_EVENT_VI + the `osViSetEvent` client every `retraceCount`),
  `t2os_post(mq, m)`, `t2os_run_until_idle()`.
- Run-until-idle makes each host frame a deterministic step: no game thread runs while the
  platform renders, so Fast3D can read display lists, vertices and textures in place, and a
  recorded input stream replays to the same frames (the basis for comparing against an
  emulator, as Rage Wars' oracle does).

Test `port/tests/t2os_test.c` (`make -C port/runtime test`, box, gcc 14 and clang 19 -O2):

| check | result |
|---|---|
| loader (17) runs inside the caller's (13) `osJamMesg`, 8 boot loads | OK |
| sync decompress runs at the boosted priority 14 | OK |
| 600 ticks: frames / gfx tasks / audio frames / cache round trips | 600 / 600 / 600 / 600 |
| switch-order hash, 3 runs x 2 compilers | identical (`2b2775877e0d226b`) |
| ThreadSanitizer (clang -fsanitize=thread, 200 ticks) | no reports |
| cost | 13 switches per tick, 10.4 µs per switch (gcc), 135 µs per tick |

The real engine will make more switches per frame (cache requests, loader chunks while a level
streams); even 200 per frame is ~2 ms, and only while loading.

Rules the port must follow (from the model, and from Rage Wars' fixes):

1. **No spinning game thread.** `CEngineApp__Idle` spins at priority 0; under the baton it would
   hold the baton whenever everything else is blocked, and `t2os_run_until_idle` would never
   return. The port starts `mainproc` directly and never creates the idle thread. Other spins:
   the reset blank in `__scMain` (`while (1) {}` 20 retraces after pre-NMI: never post PRENMI,
   or exit there), `VM_HandleTLBMiss`'s 2-second waits (not linked), the scheduler watchdog
   (`*(u8 *)0 = 0` after 1200 retraces without a gfx task done: the shim must always deliver
   completions).
2. **Every hardware completion is posted**: SP/DP done for each task started, SI after each
   `osContStartReadData`/`osContStartQuery` (the scheduler blocks in `Controller_ReadAll`
   otherwise), PI after `osPiRawStartDma` if any caller still uses it, AI while buffers are
   queued (Rage Wars: posting AI only on the 21st retrace left 350 ms gaps).
3. Rage Wars lost an SI completion when the game re-created `gSiMessageQueue` with a transfer in
   flight (`Cont_InitAll` again); its shim re-posts it. t2os should do the same for the SI queue.
4. Posting from host threads other than the platform thread (an SDL timer, the audio callback)
   is not allowed: they would race the baton. LUS's `osCreateViManager` SDL timer must not be
   used; VI is posted by the platform loop.

### 2.4 Symbol clash with LUS

LUS's static library defines `osCreateMesgQueue`, `osSendMesg`, `osJamMesg`, `osRecvMesg`,
`osSetEventMesg`, `osViSetEvent`, `osCreateViManager`, `osViSwapBuffer`, ... (os_mesg.cpp,
os_vi.cpp) and has no CMake option to leave them out. A static archive member is only pulled in
for an unresolved symbol, but os.cpp (which the port wants for `osContInit`/`osGetTime`) also
defines `osAiSetNextBuffer` and `osSetTimer`, which the port overrides. To avoid duplicate
definitions on every linker, the port's host ultra64 header (p1's `port/include`) should rename
the functions the port implements itself: `#define osSendMesg t2_osSendMesg` and so on for the
thread/queue/event/VI/SP/DP/AI/PI/SI set, and the shim defines the `t2_` names. LUS's
`osContInit`, `osContGetReadData`, `osGetTime`, `osGetCount`, `osMotorInit`, `__osMotorAccess`
keep their names and come from LUS.

## 3. Boot and memory on the host

Replacement boot (port code, e.g. `port/src/main.cpp`):

```
main(): LUS Context (window, config, controllers, ArchiveManager) ─► T2Asset_SetBackend(LUS)
      ─► t2os_init ─► osCreateThread(&gThreadMain, 1, mainproc, NULL, NULL, 13); osStartThread
      ─► loop: completions, t2os_vi_retrace(), t2os_run_until_idle(), render, present
```

Game C that must change (each under `#ifdef TARGET_PORT`, or as a port-side replacement file
that the CMake list uses instead of the original; never a change to the matching build):

| function | N64 behaviour | port |
|---|---|---|
| `func_0028D380`, `Engine_Boot`, `CEngineApp__Boot`, `CEngineApp__Idle` | ROM copy, BSS clear, idle thread | not linked |
| `mainproc` | `__osSetFpcCsr(... \| 0x400)` | `__osGetFpcCsr`/`__osSetFpcCsr` no-ops (division by zero gives inf instead of a trap) |
| `InitializeSystemResources` | heap = `D_8014A000` (image end) .. 0x80400000 or top - 512 KB; `GetMemSize` | heap = one host allocation (the "RDRAM arena", §5.3), sized for 64-bit structs (start with 32 MB) |
| `GetMemSize` (10 game files) | `osMemSize` | returns 0x800000: the expansion-pak paths (resident VM module, high-res modes, `GfxPool_Alloc`'s layout) |
| `VM_Initialize` | pager, TLB, threads, module-1 load | empty (or sets `gPager.expanded = 1` for the code that tests it) |
| `VM_Alloc` / `VM_Free` | paged region | resident `T2Asset_Read` (PORT.md §10.4), returns `void *` |
| `GfxPool_Alloc` | `CCache__AllocateAt` at fixed top-of-RDRAM addresses | ordinary cache allocations (Fast3D needs no real frame buffer; the addresses only have to be distinct and stable) |
| `CLoader__LoadSync/LoadAsync/Main` | PI DMA in 0x2000 chunks | PORT.md §10.4 code, plus one mutex around `T2Asset_*` (§10.5) |
| `osInitialize`, `osCreateViManager`, `osViSetMode/SpecialFeatures/YScale/Black` | hardware | no-ops; the resolution (`gScreenWidth/Height`) goes to `GfxSetNativeDimensions` |

## 4. Rendering

- Keep the game's scheduler. The shim's `osSpTaskLoad` copies the OSTask; `osSpTaskStartGo` for
  a gfx task (type 1) records `data_ptr` (and the L3DEX line-list task); the platform loop, after
  `t2os_run_until_idle`, calls `DrawAndRunGraphicsCommands` on the main thread (Fast3D's OpenGL /
  Metal / D3D context lives there), then posts SP done and DP done at the start of the next tick.
  The scheduler's `osViSwapBuffer` and `osDpSetNextBuffer` are no-ops (Fast3D presents).
- The two-task frame (`twoTasks`: F3DEX.NoN, then L3DEX for the automap's line list, flags 3 then
  0x23): both must land in one Fast3D frame: queue the first, and on the task with flag 0x20
  (`OS_SC_LAST_TASK`) run a wrapper list `gSPDisplayList(dl0); gSPDisplayList(dl1)`. LUS's
  interpreter has no `G_LINE3D` handler (only the opcode constants): the automap lines need one
  (draw each line as a thin quad). Not needed for the first frame.
- `rspbootTextStart`, `gspF3DEX_NoN_fifoTextStart`, `gspL3DEX_fifoTextStart`, `aspMainTextStart`
  and their data symbols are only addresses in OSTasks: give the port dummy arrays.
- Pacing: one VI per host frame at 60 Hz; the game submits every `gNextTicks` (>= 2) retraces, so
  30 fps, 20 with 3-4 players, 15 in cinemas, as on the console. A frame that was not
  rendered this tick is not presented again (Fast3D keeps the last image).
- Later, 60 fps: the engine has a variable time step (`gFrameIncrement = gNextTicks * 15 /
  gRefreshRate`), so forcing `gNextTicks` = 1 should run the logic at 60 Hz without
  interpolation; to test once the game runs.

## 5. Audio

### 5.1 Plan

Keep `amCreateAudioMgr`, `__amMain`, `__amHandleFrameMsg`, `__amDMA` and all of libaudio and
adec as matched C on the audio thread. The shim provides:

- `osAiSetFrequency(f)`: store, return f (22,050).
- `osAiSetNextBuffer(buf, n)`: convert the frame to the host format (the HLE writes it, so it can
  write little-endian directly) and `AudioPlayerPlayFrame`; advance the modelled FIFO end time.
- `osAiGetStatus()`: bit 31 (FULL) while the queued audio exceeds ~2 frames plus the player's
  buffer (`AudioPlayerBuffered` against `AudioPlayerGetDesiredBuffered`), which is how
  `__amMain` regulates its production. Post OS_EVENT_AI once per tick while audio is queued.
- `osSpTaskStartGo` for a type-2 task: run the Acmd list through the ABI1 HLE at once and post
  SP done immediately (Rage Wars delivers it when the scheduler next waits; with t2os, posting
  from the scheduler thread is the same thing).

### 5.2 The HLE

Rage Wars' `audio_hle.c` (406 lines, ABI1 including POLEF and the ENVMIXER state layout,
differential-tested there; CC0 per that repo, p5 confirms) is the starting point. Changes for
Turok 2 native: DMEM stays a byte array, but RDRAM is host memory in host byte order, so
LOADBUFF/SAVEBUFF of 16-bit samples, ADPCM state, RESAMPLE/ENVMIXER state and the codebook need
explicit conversions where RDRAM data is little-endian; the ADPCM wave data from the ROM stays
big-endian bytes (it is a byte stream). LUS has no libaudio HLE (the HM ports use their games'
own C mixers), so this is port code.

### 5.3 Addresses in Acmd words

Acmd words stay 32-bit (LUS's `Awords` is `u32 w0, w1`, and the 64 macro stores in matched C cast
to `unsigned int`). Every DRAM address in an audio command is `osVirtualToPhysical(p)` or
`p & 0x1FFFFFFF`, so define the "physical address" as an offset into a host **RDRAM arena**: one
allocation that holds the game heap (`CHeap` from `InitializeSystemResources`, which is where the
audio heap, the DMA windows, the output buffers and the command lists come from).
`osVirtualToPhysical(p)` = `p - arena` for pointers inside it; the HLE's translate is `arena +
a`. The few static buffers outside the heap that reach Acmd (the adec slots `D_80125EF0`,
anything else the HLE logs as untranslatable) are registered as windows above the arena size
(LUS's `AudioDmaRegistry` is the same idea). This also keeps any other `& 0x1FFFFFFF` use
working. The 64-bit fix-ups of the audio banks (`alBnkfNew`) are p2's.

### 5.4 Bring-up order

Audio is not needed for the first frame. A `T2_NO_AUDIO` switch that skips `amCreateAudioMgr`
and makes `Sound__*`/`Music_*` no-ops (or lets `__amMain` run with `osAiGetStatus` always full)
removes the bank fix-ups from the critical path.

## 6. Controllers, rumble, Controller Pak

- Pads: LUS's `osContInit` / `osContGetReadData` (ControlDeck: SDL gamepads, keyboard mapping,
  config UI) as they are. The port's `osContStartReadData`/`osContStartQuery` post
  OS_EVENT_SI to the queue given (Rule 2); `osContGetQuery` reports the pads LUS has
  connected and the pak bit from the pak model; `osContReset` returns 0.
- Rumble: LUS's `osMotorInit` / `__osMotorAccess` (`osMotorStart/Stop` are macros over it in
  the LUS headers; Turok 2 calls the functions, so define them over `__osMotorAccess`). Turok 2
  also probes rumble with `Pak_ProbeRumble` through pfs reads of pak address 0x8000: answer it
  from the pak model.
- Controller Pak saves: keep the game's pfs C and the `Pak_*` menus (repair, damaged-pak, note
  manager all work as on hardware) and emulate the hardware below them: a virtual PIF behind
  `__osSiRawStartDma` (controller read, status, pak read/write blocks of 32 bytes with the
  address/data CRCs) over a 32 KB image per port, saved as `saves/turok2_pak<N>.mpk` (the
  emulator-standard raw format, so saves move between the port and emulators). Rage Wars'
  `pak.c` + `pak_os.c` (903 + 260 lines) do exactly this; adapt them. Bring-up: report "no pak"
  and press through mode 0's pak menu, or open the pak menus later.

## 7. What is not needed

`__osException` and the exception stubs, `__osEnqueueAndYield`/`__osDispatchThread`,
TLB code (`STVM_MapTLB`, `func_0028EE40`...), cache ops, `osSetIntMask`, the watch-register
code, the crash screen (`Crash_ShowScreen` is never called), SN host I/O (`HostIo_*`, `PC*`),
64DD (`osLeo*`), libgcc's 64-bit helpers (`__divdi3`, `__moddi3`, `__udivdi3`, `__umoddi3`,
`__cmpdi2`, `__floatdisf`: the host compiler has 64-bit arithmetic), the RNC asm (replaced by
`port/third_party/rnc`), all of module 2.

Of the 359 libultra/libaudio/libc C functions in the tree (0x28EF90-0x2A5200 and module 1's
pfs), the port **compiles as is**: libaudio (cseq, csplayer, seqplayer, sndplayer, synthesizer,
envmixer, resample, reverb/fx, load filters incl. `alAdecPull`, heap, event queue, `alAudioFrame`),
adec/mdct, gu, pfs and the pak data packing (`__osContRamRead/Write`, `__osPack*`, CRCs), libc
(`sprintf`/`_Printf`/`_Litob`/`_Ldtob`, or the host's). It **replaces**: threads, queues, events,
timers, time, VI, PI, SP/DP, AI, raw SI, `osInitialize`, `osVirtualToPhysical`, cache, interrupts
(t2os + device shims).

## 8. Summary per subsystem

| subsystem | game functions | host replacement | LUS provides? |
|---|---|---|---|
| boot | `boot_main`, `func_0028D380`, `osInitialize`, `CEngineApp__Boot/Idle` | port `main()` starts `mainproc` | window/context only |
| threads, queues, events | `osCreateThread`... `osRecvMesg`, `osSetEventMesg`, `__osDisableInt` | **t2os** (done, tested) | no (non-blocking stubs; clash, §2.4) |
| VI / frame loop | `__scMain`, `__scHandleRetrace/RSP/RDP`, `CEngineApp__Main/Retrace/Update/Draw/SubmitGfxTask` | kept; VI posted by the platform loop; VI calls no-ops | VI timer (do not use) |
| RSP/RDP | `osSpTaskLoad/StartGo`, `osDpSetNextBuffer` | gfx -> Fast3D on the platform thread; completions next tick | Fast3D interpreter (F3DEX yes; L3DEX lines no) |
| ROM / loader | `CLoader__*`, `osPiRawStartDma` | `T2Asset_ReadInto` (PORT.md §10.4) | ArchiveManager (p3) |
| VM paging | `VM_*`, TLB asm | not built; module 1 linked; `VM_Alloc` resident | `osMapTLB` no-op |
| heap / RDRAM | `InitializeSystemResources`, `GetMemSize`, `GfxPool_Alloc` | host arena, 8 MB answer | no |
| audio | `amCreateAudioMgr`, `__amMain`, libaudio, adec | kept; AI model, ABI1 HLE, arena addresses | `AudioPlayerPlayFrame` (output only) |
| controllers | `Cont_InitAll`, `Controller_Service/ReadAll`, `osCont*` | LUS read + SI completion posting | yes (ControlDeck) |
| rumble | `osMotor*` | over `__osMotorAccess` | yes |
| Controller Pak | `Pak_*`, `osPfs*`, `__osContRam*` | virtual PIF + `.mpk` files (adapt Rage Wars) | no |
| time | `osGetTime` (`CEngineApp__Update`, scheduler profiling) | LUS's, or a frame-based virtual clock for replay tests | yes |
| crash, host I/O, 64DD | `Crash_*`, `HostIo_*`, `PC*`, `osLeo*` | not linked | - |

## 9. Plan to the first frame and the title

Dependency order; each step lists what it needs and how to know it is done. Estimates are
agent-rounds of work (one part, ~3-4 hours each), assuming p1's host compile and p3's LUS skeleton.

1. **Link closure** (needs p1). CMake target `turok2` with the engine + module-1 file list (2,504 +
   792; drop module 2's 629, `HostIo`, `PC*`, crash screen optional, replaced libultra), t2os,
   and a generated stub file for every still-unresolved symbol that logs its name once and
   aborts. Done when it links on Linux and macOS. **1-2 rounds.**
2. **Runtime shims** (needs 1). t2os in the build with the §2.4 renames; device shims: events,
   VI, SP/DP (tasks recorded, completions next tick), AI model (stub first), SI posting, PI,
   cache/TLB/interrupt no-ops, `osVirtualToPhysical` on the arena. **1 round.**
3. **Boot glue** (needs 2, p3's LUS skeleton). `main()` per §3; the §3 patches; `GetMemSize`
   = 8 MB; `T2_NO_AUDIO`. Done when `CEngineApp__Main` reaches its message loop and the
   scheduler delivers retraces (log mode 0's Retrace calls), with no stub hit. **1-2 rounds.**
4. **Assets to mode 0** (needs 3, p2's 64-bit audit, b2's swap spec). Loader/cache/decompressor on
   `T2Asset` (PORT.md §10.4), the byte-swap hook, and the 64-bit fix-ups for what modes 0-1
   load: `EngineMode00_PakCheck_Init` already runs `CScene__LoadLevel(&gScene, 0, 0)`, plus the
   menu/font/texture data of the pak menus and the legal screen. This is the long pole and the
   size depends on p2's findings. Done when mode 0 runs its 120-frame timer and opens the pak
   menu without a crash. **4-8 rounds.**
5. **First frame** (needs 4, p1's GBI). Gfx task -> Fast3D on the main thread; GBI words that
   hold pointers (p1/p2: `(u32)` casts in hand-built display lists such as `gDepthBuffer`'s);
   segment table; F3DEX ucode selected in LUS. Done when the pak-check screen or the legal
   screen (mode 1) shows on screen. **2-3 rounds.**
6. **Input and pak** (parallel with 5). Pads through LUS, SI posting, then the virtual PIF and
   `.mpk` saves. Done when START/A move through the pak menu and a save note survives a restart.
   **1 round** (pads) + **2 rounds** (pak).
7. **Title** (needs 5, 6). Mode 1 -> 4 is the intro cinema: `CEngineApp__StartWarp(0x64)` loads a
   scene, so the whole level renderer (models, animation, particles, cinema) must work. For the
   title alone, a debug option that enters mode 7 (`EngineMode07_Title_*`: one full-screen picture,
   0x386, plus music track 0x10) skips that. **1 round** with the skip; the intro cinema is the
   start of "levels run".
8. **Audio** (parallel from 3). HLE adapted to LE and the arena, AI model with LUS's player, adec,
   bank fix-ups (p2), then remove `T2_NO_AUDIO`. Done when the title music plays. **3-5 rounds.**

Total to the legal screen: about 9-16 rounds (2-4 weeks); title +1 round; audio in parallel.

## 10. Reproducing

```bash
# t2os prototype + test
make -C port/runtime test          # 3 runs: checks, then "deterministic: same hash on 3 runs"
clang -O1 -g -fsanitize=thread port/runtime/t2os.c port/tests/t2os_test.c -lpthread -o /tmp/t && /tmp/t 200

# module 2 is unreachable: jal targets / lui+addiu pairs into the named module-2 range
python3 - <<'EOF'
import struct
rom = open("baserom.us.z64", "rb").read()
LO, HI = 0x43CC14, 0x46A000
for name, (a, b, va) in {"engine": (0x1100, 0xA5FD8, 0x200500), "mod1": (0x14A000, 0x186C14, 0x400000)}.items():
    jal = hit = 0
    for o in range(a, b, 4):
        w = struct.unpack(">I", rom[o:o+4])[0]
        if w >> 26 == 3:
            jal += 1
            t = ((va + o - a) & 0xF0000000) | ((w & 0x3FFFFFF) << 2)
            hit += LO <= t < HI
    print(name, "jal", jal, "into module 2:", hit)
EOF
```

Expected: `engine jal 7736 into module 2: 0`, `mod1 jal 2436 into module 2: 1` (that one decodes
from module 1's data at 0x43588C, not code).

## 11. Open points

- Whether anything in the engine reads `gPager` state that `VM_Initialize` no longer sets
  (`VM_GetStats` is a debug readout); check when linking.
- `CEngineApp__Main` case 2 (frame done) renders the next frame at once; with completions posted
  at the next tick the rate is one frame per tick at most. If the game ends up rendering more
  often than its `gNextTicks` asks (it submits a frame per completion), the shim should hold gfx
  completions until the frame's `m_nPredictTicks` retraces have passed, as the RDP time did.
- `gRefreshRate` is the word after the pointer `gCurrentFB` (`#define gRefreshRate (*(f32 *)
  (&gCurrentFB + 1))`, and `RumbleCurve_Advance` reads `gCurrentFB[1]` as a float): on a 64-bit
  host that word is inside the pointer. Give it its own variable in the port (p2's list).
- Pre-NMI (reset button): map to a "soft reset" hotkey later; until then never posted.
- L3DEX lines in Fast3D (automap).
- Windows: t2os needs pthreads (MinGW) or a Win32 port of its 20 lines of locking.
