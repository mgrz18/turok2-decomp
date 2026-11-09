# Audio on the host (Phase 7 round 3, part r4)

The game's sound runs on the host behind `--audio` (`T2PortConfig.audio`); without it
`Sound__Initialize` returns at once and nothing below runs, so boot runs are unaffected.
Code: `port/audio/` (library `t2audio`, compiled with the game's flags so it uses
`include/structs/libaudio.h`), `port/tests/t2audio_test.c`, `tools/port/audio_compare.py`.

## 1. Pieces

| piece | N64 | host |
|---|---|---|
| sound bank (ROM 0x1E0000, 0x2F194 bytes) | `VM_Alloc` maps it at VM 0x480000; header `s32 n; {ALBankFile *file; s32 tbl}[n]`; pointers pre-relocated to 0x480000; no `alBnkfNew` | `T2Audio_LoadCtl` (t2audio_bank.c) rebuilds the graph as host structs in host order, shared objects shared; `CAudioWorld__Initialize` takes it under `TARGET_PORT` |
| sound order | `CAudioWorld__Initialize` sorts each `soundArray` by pointer (= ROM order) | each bank file's sounds are allocated in one array in ROM-address order; the sort runs on `uptr` |
| wave data (`gAudioRomBase` = 0x20F194 on) | `__amDMA`: 0x300-byte windows via `CLoader__LoadAsync` | resident copy below 2 GB; the DMA callback returns the bytes' address |
| sound data file (ROM 0xBE1814) | used in place, big-endian | `T2Audio_SwapSoundData` swaps ISet/USet headers, sfx layers, envelopes, song headers, ALCMidiHdr, adec u16 words (1,937,194 words) from VM_Alloc's hook; skipped when the copy is already host order (r2's archive path, see §7) |
| audio manager (`amCreateAudioMgr`, `__amMain`, `__amHandleFrameMsg`, `__amDmaNew`, `__amDMA`) | RSP task through the scheduler, AI DMA | t2audio_mgr.c: one synthesizer frame (368 samples at 22050 Hz) per retrace (`osSetTimer` on the virtual clock), `alAudioFrame` -> `T2Asp_Run` on the audio thread -> AI queue |
| `alAuxBusNew`'s handler | `func_00294D54 + 0xC` (a stray word) | `alAuxBusPull` itself (`TARGET_PORT`); `alAuxBusPull`'s bus view in the host layout |
| aspMain (ABI1) | RSP microcode | `t2asp.c`: interpreter written from PR/abi.h's command layout and the game's command builders (see §2) |
| output | AI DMA | `T2Audio_TakeSamples` (stereo s16, host order) -> `AudioPlayerPlayFrame` in `game.cpp` each frame (no-op without an audio backend) |
| sound player API (`alSndpAllocate` ... `alSndpStop`, 10 files) | m2c views (`PoolRecAt44`, `SlotRecAt2F`, `ObjRecAt40V2`, local Pool/Slot) with N64 offsets, 0x30 strides, s32 state pointers in events | `t2audio_sndp.c`: typed versions with the game's event numbers (0 play, 1 stop, 2 pan, 3 vol, 4 pitch, 8 fx); the game files are out of the link (link.cmake) |

Matching-safe source changes (`asm_diff --base 3798e01e`: 0 differences over the 656 files that
include `structs/sound.h`, `globals/audio.h`, `rw/abu_types.h` or `rw/junk_types.h`):
`CAudioWorld__Initialize`, `alAuxBusNew`, `alAuxBusPull`, `SndSeq_Construct` (typed with
`SndSeq`/`CAudioWorld` under `TARGET_PORT`), `alCSPNew` (sizes by `sizeof`), `alHeapInit` /
`alHeapDBAlloc` (host `ALHeap` layout, `uptr` arithmetic), `alEvtqNew` / `alEvtqNextEvent`
(typed under `TARGET_PORT`), `CAudioWorld__Lock` (typed like `CAudioWorld__Unlock`),
`SndSeq.csp` is `sptr`.

## 2. The ABI1 interpreter (t2asp.c)

All 16 commands: SPNOOP, ADPCM (VADPCM order 2, A_INIT / A_LOOP via SETLOOP), CLEARBUFF,
ENVMIXER (dry L/R + wet L/R with A_AUX, Q15 volumes, 16.16 ramps applied every 8 samples as
`_getVol` assumes), LOADBUFF, RESAMPLE (Q1.15 pitch, 4-sample history + fraction in the state,
Catmull-Rom), SAVEBUFF, SEGMENT (recorded; the game only sets segment 0 to 0), SETBUFF (main
and A_AUX), SETVOL (A_VOL / A_RATE / A_AUX), DMEMMOVE, LOADADPCM, MIXER (Q15 gain), INTERLEAVE,
POLEF (one-pole low-pass as `_init_lpfilter` sets it up), SETLOOP.

DMEM is host-order s16 for samples; everything the commands load or save is written by the
interpreter or by host C, so it is host order; VADPCM frames are read byte by byte. The state
layouts (ENVMIX/RESAMPLE/POLEF) are the interpreter's own (only the microcode reads them).
Not bit-exact with the RSP (resampler filter, envelope rounding), which nothing depends on.

Addresses: command words carry `osVirtualToPhysical` = the low 32 bits of the host address,
exact only in low-memory mode (Linux `T2_LOW_MEMORY`). macOS (r5) needs a 32-bit handle table
in `osVirtualToPhysical` that `t2asp.c`'s `ram()` resolves (one place to change).

## 3. Verification

```sh
ninja -C OUT/build t2audio_test
# the interpreter alone on known sounds (no game code): sfx bank instrument 0 sound K at pitch
# 1.0 through LOADADPCM/LOADBUFF/ADPCM/RESAMPLE/SETVOL/ENVMIXER/INTERLEAVE/SAVEBUFF
OUT/build/app/t2audio_test baserom.us.z64 --direct 0 OUT/d0.raw --wav OUT/d0.wav
python tools/port/audio_compare.py baserom.us.z64 0 OUT/d0.raw
# sound 0: wave ROM 60068C, 6704 samples compared, reference peak 32433, max difference 0: ok
# sound 40: wave ROM 649764, 1088 samples, peak 30190, max difference 0: ok
# the game path: Sound__Initialize, then sfx through the game's sound player, then a song
OUT/build/app/t2audio_test baserom.us.z64 --frames 120 --sfx 0 --song 0 --wav OUT/t.wav
```

`--direct` output equals `tools/assets/audio.py`'s `vadpcm_decode`, delayed by the resampler's
3 history samples, with **0** difference after the two Q15 gain stages.

The game path (`t2audio_test` without `--direct`, and `turok2 --game --audio`):
`Sound__Initialize` completes (bank converted: 2 bank files, 2 banks, 34 instruments, 1,115
sounds, 866 wave tables; sound data swapped; audio manager 22050 Hz / 368 samples / 2048
commands; `alInit`, `SndSeq_Construct`, `alCSPNew`, `alSndpNew`, the queue, voice pool and
speech player constructed; `gAudioWorldReady` = 1). The first audio frame enters
`alAudioFrame` and spins in the sound player's handler (§4), so the game-path test hangs there
for now (run it under `timeout`). `turok2 --game --audio` reaches r1's `CCameraPool__Construct`
crash first, like the run without `--audio` (the audio thread has not run a frame by then).

## 4. Where it stops (open)

The sound player's event queue (`gAudioWorld.player.evtq` and `nextEvent`) holds stack-like
garbage by the first `_sndpVoiceHandler` call (thread stack addresses, return addresses into
`Sound__Lock`/`am_frame`), so the handler's `do ... while (nextDelta == 0)` loop spins. Suspects:
one of the m2c views that `CAudioWorld__Initialize` runs after `alSndpNew`
(`SndQueue_Construct`, `SndVoicePool_Construct`, `SpeechPlayer_Construct`: `rw/abu_types.h`
views with N64 offsets) or another m2c view of `CAudioWorld`; next step: a hardware watchpoint
on `&gAudioWorld.player.evtq` from `alSndpNew` on. Also `gAudioWorld`'s generated symbol is
0x3048 bytes and the host `CAudioWorld` 0x3050 (gendata sizes it from the N64 layout: 8 bytes
short; fix in gendata or after r1's struct work).

## 5. Next steps

1. The evtq corruption above, then the sfx phase of `t2audio_test` (non-silent check), then the
   song phase (`Music_SetTrack` -> `SndSeq_Update` -> `alCSeqNew`/`alCSPSetSeq`: `SndSeq_Update`
   and the CSP code still use m2c views).
2. The rest of the audio code with m2c views (CAudioWorld__* m2c files, SndQueue, voice pool,
   adec stream player `alAdecPull`); `audit64.py` lists them.
3. `alSndpSetPitch`'s callers pass a float: check their prototypes (the host ABI passes it in
   xmm0).
4. Raw16 waves (`alRaw16Pull`) would load big-endian samples: none seen in the two banks yet.
5. Output rate: LUS's AudioPlayer is opened by the context with its own rate; resample 22050 ->
   the player's rate in game.cpp if they differ.

## 6. Interfaces for merging (r2's archive path)

- The ctl region: `T2Audio_LoadCtl` reads the **ROM image** (`t2_rom`), not `VM_Alloc`'s copy,
  so it does not care whether r2's `VM_Alloc` returns the control block swapped. With r2's
  branch, `CAudioWorld__Initialize`'s `TARGET_PORT` path still calls `T2Audio_LoadCtl` and never
  `VM_Alloc(.., 0x1E0000)`; an archive-only build (no ROM) would need `T2Audio_LoadCtl` to take
  the host-order unit instead (the converter's `be32`/`be16` readers become native reads).
- The sound data file: `VM_Alloc` calls `T2Audio_FixRegion(romAddr, p, size)` after filling the
  copy; it swaps only when `T2Audio_SoundDataIsHostOrder` says the copy is still big-endian
  (top ISet count read natively != 2), so r2's pre-swapped unit passes through untouched.
- The wave table stays raw bytes in both (VADPCM is byte data).

## 8. Round 4 (s4): the sound world and the synthesizer typed

The game path now runs: `turok2 --game --audio` mixes a frame per retrace through mode 0 and a
sound effect played through the game's API comes out of the mixer (STATUS.md, part s4).

What was wrong was not the converted bank or the interpreter but the game's C between them:
the sound world (Rage Wars' audio code) and most of libaudio's synthesizer were imported with
N64-offset views. Two port files hold the typed versions; the originals are listed in
`port/cmake/link.cmake` and stay out of the link:

- `port/audio/t2audio_world.c` (81 functions): CAudioWorld (StartVoice, StopHandle, PlayLayer,
  PlaySound, PlaySoundIndex, Update, the one-liners with N64 offsets), SndVoice (InitPitch,
  InitVolume, InitPan, Setup, Update, UpdatePan, Set*/Get*/Play/Stop/Release), SndVoicePool
  (FindSlot, AllocVoice, ...), SndQueue (Add, Update, RequeueVoice, InitNode), SndSeq (Update,
  UpdateFade, SelectSong, Duck/Unduck/Stop/Load/...), the speech player (on `SndFx`, named
  through `T2SpeechPlayer`), LoopSounds (on `LoopSounds`, now with its paused word),
  `Sound__IsPlaying` (declared void in the game: its result is CountHandleVoices' v0).
  Each function's comment names the N64 offsets it replaces. Things the views hid:
  big-endian halves (`((u8 *)&pan)[1]`, `*(u16 *)(m + 0x132)`, `volume.h.lo`), floats passed
  as int bits (`alSndpSetPitch`), arguments m2c dropped (StartVoice passes PlayLayer's
  position to RequeueVoice in a3), functions declared void that return a value.
- `port/audio/t2audio_lib.c`: libaudio's synthesizer driver (`alSynAddPlayer`,
  `__nextSampleTime`, `_timeToSamples*`, `_collectPVoices`, `_allocatePVoice`, `_freePVoice`),
  the voice calls (`alSynSet*`, `alSynStartVoice`, `alSynStopVoice`, `alSynFreeVoice`), the
  event helpers (`alEvtqFlush`, `alEvtqFlushType`, `_removeEvents`), the filter constructors
  that stored function pointers as s32 (`alFilterNew`, `alSaveNew`, `alSaveParam`),
  `alResampleParam`, the CSP API posts (`alCSPSetSeq/Bank/Vol/ChlFXMix/Play/Stop`,
  `alCSPGetState`) and the codebook cache (`AudioDMA_CacheBlock`, aged by
  `T2Audio_DmaCacheAge` from t2audio_mgr.c as `__amMain` did).

In the game's own files, under `TARGET_PORT`: `FloatToHalf` (the float's top half),
`alFxPull`/`_doModFunc` (u32 delays negated as s32). Headers: `ALParam` gets a host pad (the
start-voice update casts from it carry a pointer), `ALSave.dramout`, `SndSeq.unkC` and
`SndSource.flags` are `sptr`, `LoopSounds.m_Paused` (0x28).

### Verifying

```sh
# mode 0 with sound, its output as a WAV and a one-line summary (peak, rms, non-zero samples)
turok2 --archive ... --rom baserom.us.z64 --game --wav m0.wav --frames 300
# debug hooks (game.cpp): T2_TEST_SFX=<id> -> Sound__Play2D(id) at frame 30,
# T2_TEST_SONG=<n> -> Music_SetTrack(n) at frame 60; both set the volume options to 0xC0
# when they are still 0 (they are, before the settings load)
T2_TEST_SFX=300 turok2 ... --wav sfx.wav --frames 300
```

Mode 0 alone is silent (volume options 0, nothing played). With `T2_TEST_SFX=300` (under gdb)
the sound is there: per 10-frame window peaks 0, 0, 1021, 1029, 1037, ...; peak 1037, rms 88.

### Open

1. Without gdb (ASLR on) the playing sound crashes in `t2asp.c` `cmd_adpcm` writing the ADPCM
   state to `ram(w1)`: check the addresses `alAdpcmPull`/`_decodeChunk` hand
   `osVirtualToPhysical` (a pointer above 4 GB, or the loop state).
2. Music: the compressed sequence player is still m2c (`__CSPVoiceHandler`,
   `__CSPSetUsptFromTempo` crashes first, `__CSPHandleNextSeqEvent`, `__CSPHandleMIDIMsg`,
   `__CSPRepostEvent`, `alCSeq*`): type them from libultra's csplayer.c/cseq.c like
   t2audio_lib.c does.
3. Speech (adec streams: `SpeechPlayer_Update` state 1, `adecStreamOpen`, `D_800F55F4`) is
   stubbed.
4. macOS: `osVirtualToPhysical` addresses (section 2) and the s32 pointer arguments still in
   the game's libaudio files that are typed but take `s32` (e.g. `alLoadNew`'s heap).
