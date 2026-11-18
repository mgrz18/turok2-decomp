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

Addresses: command words carry `osVirtualToPhysical` (t2ultra.c). Below 2 GB (Linux
low-memory mode) that is the host address itself; any other host address gets a region handle
(`0x80000000 | slot << 24 | offset`, 16 MB regions, slots taken in pairs so a handle plus a
length or minus an alignment still resolves), and `t2asp.c`'s `ram()` resolves both through
`T2Port_PhysToVirtual` (round 5, §9).

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

### Open (round 4; all four closed in round 5, §9)

1. ~~The sfx crash without gdb in `cmd_adpcm`~~: `_decodeChunk` masked the state address with
   `& 0x1FFFFFFF` (K0_TO_PHYS), §9.
2. ~~Music~~: the compressed sequence player, §9.
3. ~~Speech~~: the adec streams, §10.
4. macOS: the handle table is in; the audio start-up runs above 4 GB (`--high-memory`) up to
   the game's own non-audio crash (`CHeap__Free` from the decompressor, also without `--audio`).

## 9. Round 5 (t4): sound effects, music

`turok2 --game --draw --pak-blank --audio --wav boot.wav --frames 5400` now runs the boot to the
title with sound: the first cinema's sounds from 26 s, song 22 over the second cinema (31-70 s),
song 16 (the title theme) from 70 s; the WAV is 90 s, peak 20,820, rms 2,088, no clipping.

What was wrong, in the order it was found:

| # | where | cause | fix |
|---|---|---|---|
| t4-1 | `cmd_adpcm` writes `ram(w1)` (sfx without gdb) | `_decodeChunk` (func_0029FCF4.c) builds the ADPCM/SETLOOP state addresses with K0_TO_PHYS (`& 0x1FFFFFFF`), not `osVirtualToPhysical`: the low-memory arena is at 0x41000000, so the state went to 0x0xxxxxxx (gdb's layout happened to fit) | `TARGET_PORT`: `osVirtualToPhysical`; `osVirtualToPhysical` names any host address (region handle table, §2), `ram()` = `T2Port_PhysToVirtual` |
| t4-2 | `__CSPSetUsptFromTempo` (music, mode 5) | the compressed sequence player, cseq.c and the seqplayer.c helpers it shares are Rage Wars views (`AnimState.frames` at 0x24 for `uspt`, `RuntimeState` for `ALCSeq`, `Node_func_802AFD6C_de` for event items) and s32 pointers (`alCSeqNew`'s base, `__alCSeqNextDelta`'s sequence, `alCSPNew`'s heap blocks) | `port/audio/t2audio_csp.c`: 31 functions from the SDK source on `structs/libaudio.h` (`alCSPNew`, `__CSPPostNextSeqEvent`, `__CSPHandleMetaMsg`, `__CSPRepostEvent`, `__CSPSetUsptFromTempo`, all of cseq.c, `__mapVoice` ... `__seqpStopOsc`); the game's files are out of the link. `__CSPVoiceHandler`, `__CSPHandleNextSeqEvent` and `__CSPHandleMIDIMsg` stay the game's (typed) |
| t4-3 | (silent) music at volume 0 | `GameSettings_SetDefaults` (music 0xFF, sfx 0xFA, ...) runs from `CEngineApp__Construct` only when `gWarmBoot` != 0, which `main` (func_0028D380, not linked) sets on power-on (the boot marker did not match: the name says the opposite) | `T2Port_Boot` sets `gWarmBoot = 1` (port/runtime/t2boot.c). This sets **every** default setting (difficulty, HUD alpha, multiplayer rules), not only the volumes |
| t4-4 | (noise) every wave decoded garbage, clipped | the resident wave data copy was taken in `amCreateAudioMgr`, "from `gAudioRomBase` on", but `CAudioWorld__Initialize` sets `gAudioRomBase` = 0x20F194 only after that call: the copy started at the stale value | the copy is the whole ROM, indexed by ROM address (`T2Audio_TblBytes`) |
| t4-5 | (garble) the resampler read the wrong samples | `A_ADPCM` writes the previous frame (16 samples, the state) before the new frames; `alAdpcmPull` skips it (`*outp += 32`, or `lastsam << 1` into it). The interpreter wrote only the new frames | `cmd_adpcm` writes the state first; `t2audio_test --direct` reads past it |
| t4-6 | `--high-memory`: `alSaveNew(NULL)` in `alSynNew` | heap/block/source pointers kept in s32: `alHeapInit` (`(s32)` casts), `alLoadNew` and `alResampleNew` (heap as s32), `alFxNew` (`alHeapDBAlloc` declared s32), `alMainBusNew`/`alAuxBusNew` (sources as s32) | pointer-wide under `TARGET_PORT` (asm_diff 0) |

### Verifying

```sh
# the voice path, bit-exact: the samples the resampler consumes = audio.py's VADPCM decode
T2_TEST_SFX=300 turok2 ... --game --pak-blank --audio --wav sfx.wav --frames 200
#   (checked with a temporary dump in cmd_resample: 2,544 samples, max difference 0)
# sequencing: T2_AUDIO_LOG=1 logs every alCSeqNew and note on (player clock, key, vel, chan)
T2_AUDIO_LOG=1 turok2 ... --game --pak-blank --audio --wav boot.wav --frames 5400 > run.log 2>&1
python3 tools/port/audio_notes_check.py . run.log 44 1C6 2AD   # song 22 (key 22)
#   order-free over 25.9 s: game 510 notes, reference 510, identical: True (0 us error)
python3 tools/port/audio_notes_check.py . run.log 44 CF 377    # song 16, the title
#   order-free over 19.3 s: game 251 notes, reference 251, identical: True
```

`audio_notes_check.py` reads the song's note-ons from the ROM with audio.py's track reader and
converts ticks the way the player does (`uspt = (s32)(tempo / division)`): every note on of the
two songs comes at the same microsecond, key and channel. The WAV itself (spectra by eye): the
cinema music is tonal (spectral flatness 0.07 at 50 s), no flat-topped clipping any more.

Debug hooks (game.cpp): `T2_TEST_SFX`, `T2_TEST_SONG`, and now `T2_TEST_SPEECH=<group>`
(`Sound__PlaySpeech` at frame 90; the group is the song key, e.g. 311). The music only advances
while the mixer runs: with `--audio` it runs with or without `--wav`.

## 10. Round 5 (t4): speech (adec streams)

The speech player (`SpeechPlayer_Update`, t2audio_world.c) now does what func_002768D0 does:
state 1 takes the song's header (u32 volume x 100, u16 FX mix) and its adec stream from the
sound data file, opens the stream, sets `sSpeechSound`'s decay time to the stream's length, and
plays `sSpeechSound` through the sound player at pitch rate / 22050. The wave table it names was
an ALWaveTable at 0x800F55F0 (= `gAudioWorld + 0x303C`, inside the host `CAudioWorld`): the
host uses its own (`sSpeechWave`), which `SpeechPlayer_Construct` puts in `sSpeechSound`.

`port/audio/t2audio_adec.c` replaces the game's decoder (`adecStreamOpen`, `adecNextFrame`,
`adecGetStopFlag`, `adecClearStopFlag`, `alAdecPull`; their files are out of the link): a C
version of tools/assets/audio.py's `AdecDecoder` (band scales, Huffman trees and phrases, band
dequantization, 128-point FFT IMDCT with overlap), with the tables read from the ROM image's
code .rodata as the Python does. The stream in memory is host-order u32 x 3 + host-order u16
words (`T2Audio_SwapSoundData`); the bit reader takes byte i from word i / 2. `alAdecPull` is
the game's (frames of 256 samples, DMEM alignment) with the frame pointer pointer-wide.

```sh
# the decoder alone vs audio.py, any stream (ROM address and size from parse_tree's songs)
t2audio_test baserom.us.z64 --adec 0xC29AB4 7464 s20.raw
#   streams 19, 20, 21, 60, 100, 158: same sample count, max difference 0
# through the game: Sound__PlaySpeech(311) at frame 90
T2_TEST_SPEECH=311 turok2 ... --game --pak-blank --audio --wav sp.wav --frames 400
#   peak 16,308, rms 3,652; correlation with the stream resampled 9450 -> 22050 Hz: 0.986
```

## 11. Next (audio)

1. A full reference render would make the WAV checks stronger: the notes match (§9), the voice
   path is bit-exact for ADPCM, but the envelopes/pan/reverb are only checked by eye.
2. The reverb output carries a small DC offset in the tail (about -25 after song 1 ends).
3. `t2audio_test`'s game path (no `--direct`/`--adec`) aborts at the sound data `VM_Alloc`
   ("no backend open": the test opens no archive); `turok2 --game --audio` covers that path.
4. macOS: rerun with `--audio` once the merged tree runs there (the handle table and the
   pointer-wide heap code are in; the AI/AudioPlayer output path is untested there).

## 12. Round 20 (i5): the port's sound against the N64's

The first comparison with the N64's own output. The N64 side is what the game hands the AI (the
DAC input) on mupen64plus (HLE RSP, which runs the game's ABI1 audio lists), recorded sample for
sample; the port side is `--audio --wav`. Both start from the same point (power-on, or
`--start-warp`/`T2_START_WARP` from the logo screen) and are lined up by the mode/game-frame
sidecars, then to the sample by cross-correlation.

### 12.1 Tools

```sh
# the N64: refshots.py --audio-dump attaches tools/port/n64audiodump.c (built with cc on the fly) as
# mupen64plus's audio plugin: OUT.raw = stereo s16 LE at the AI rate (dacrate 2207: 48,681,812 / 2208
# = 22,048 Hz); OUT.raw.txt = "frame N samples S dacrate D mode M level L gf G" per emulator frame
xvfb-run -a -s "-screen 0 1024x768x24" python3 tools/port/refshots.py --rom baserom.us.z64 \
    --out n64/w54 --frames 1900 --start-warp=-54@600 --audio-dump n64/w54.raw
# the port: --wav now also writes WAV.txt = "frame F samples S mode M gf G" per frame
T2_START_WARP=-54@1399 turok2 ... --game --no-window --pak-blank --audio --wav w54.wav --frames 4400
# the comparison (N64 start = the first mode-13 line's samples / 22048.37, port's / 22050)
python3 tools/port/audiocmp.py n64/w54.raw w54.wav --n64-from 23.45 --port-from 24.23 \
    --len 40 --maxlag 1.5 --windows 5
#   align (envelope, then waveform), rms/peak per channel, port/N64 in dB, L-R balance, DC, the
#   waveform and 10 ms envelope correlations, spectral centroid, octave band energies from 86 Hz,
#   the pitch offset (log-spectrum shift, cents)
# the sound player's events per game frame (which sounds start/stop, vol/pan/pitch/fx values)
T2_SNDP_LOG=2 ...                      # port: "sndp: gf G play|stop|pan|vol|pitch|fx target T [V]"
python3 tools/port/n64trace.py ... --calls alSndpSetSound,alSndpPlay,alSndpStop,alSndpSetVol,\
alSndpSetPan,alSndpSetPitch,alSndpSetFXMix > n64.txt
python3 tools/port/sndpcmp.py port.log n64.txt --from 1 --to 520
```

### 12.2 The host bug found: the resampler

`A_RESAMPLE` (t2asp.c) interpolated with a Catmull-Rom spline. The microcode uses its own 64-phase,
4-tap filter table, which is in the ROM (aspMain's data, `aspMainDataStart` 0x800BFF50 = ROM
0xC0B50, + 0xC0 = ROM **0xC0C10**; phase 0 = 0x0C39 0x66AD 0x0D46 0xFFDF, so even at pitch 1.0 the
N64 smooths). Every voice goes through the resampler, so **the port was 2 dB too bright from 2.7
to 5.5 kHz and 4 dB from 5.5 to 11 kHz** (the title music, octave bands from 86 Hz):

| bands (dB of the total) | 86 | 172 | 345 | 690 | 1.4k | 2.7k | 5.5k |
|---|---|---|---|---|---|---|---|
| N64 | -4.4 | -9.4 | -9.5 | -5.4 | -9.6 | -20.7 | -32.0 |
| port before | -4.7 | -9.6 | -9.4 | -5.4 | -8.9 | -18.6 | -28.2 |
| port now | -4.5 | -9.4 | -9.3 | -5.4 | -9.4 | -20.6 | -32.3 |

t2asp.c now reads the table from the ROM image at the first `A_RESAMPLE` (Catmull-Rom stays as
the fallback, with a message, when the ROM has no such table): the phase is the fraction's top 6
bits, the output `(s0*t0 + s1*t1 + s2*t2 + s3*t3) >> 15`, clamped. `audio_compare.py` filters its
reference the same way: sounds 0 and 40 through the interpreter, max difference 0.

### 12.3 The scenes compared (after the fix)

| scene | how | level port vs N64 | L-R N64 / port | spectrum | waveform corr | notes |
|---|---|---|---|---|---|---|
| title music (song 16) | power-on, mode 7, 20 s | -0.03 dB | -0.39 / -0.39 dB | bands within 0.3 dB, centroid 156 / 156 Hz | 0.91-0.96 per 4 s | same notes; the port's song starts 0.16 s later after the mode switch; tempo drift 1.8 ms in 15 s = the AI rate (22,048 vs 22,050 Hz, 91 ppm, 0.16 cents) |
| boot cinemas (intro, songs 22 etc.) | power-on, 0-66 s | within +-1 dB per 8 s | same | same | - | the N64's cinemas start up to 4 s earlier (load times), so only levels and spectra compare |
| demo 1 | mode 13 after the title, 40 s | +0.29 dB | +0.66 / +0.55 | centroid 257 / 259 Hz | - | the attract demo diverges in play (known: different random numbers), the sounds with it |
| level 53 start | warp -54, 40 s, no input | +0.97 dB | +7.4 / +7.6 | bands within 0.5 dB | **0.97-0.99** | the ambient loop's volume is a random draw at the level start (`SndVoice_InitVolume`: cue bytes a..b percent): 19,635 here, 18,500 on the N64 (sound 2: 7,209 on both); pan 28/82 and pitch 0.5/0.3124 identical |
| hub (level 60) | warp -61, 40 s | +0.07 dB | -3.56 / -3.46 | bands within 0.1 dB | 0.5-0.74 | the first second: the logo screen's sound still plays on the port (instant load), the N64 lets it decay through its ~1 s load |
| the Primagen's lair (boss) | warp 7602, 40 s, no input | +0.42 dB | +0.57 / +1.42 | bands within 0.6 dB | - | the fight's sounds follow the AI's random choices (different numbers) |
| title menu SFX | START, cursor moves, B | see 12.4 | | | | |

### 12.4 Sound events

Level 53 from a common random seed at the load (`T2_RNG_LOAD=53:12345678` / `--rng-load`), game
frames 1-520: the port plays 62 sounds, the N64 64; by sound player slot 3/4/5/6/7/8: port
23/18/9/6/3/1, N64 25/12/14/6/4/3 (slots 1 and 2, the two ambient loops, start before the N64
trace's first frame). The same slots, the same rates; the times and the volumes differ because the
two sides still draw different random numbers inside the level (the N64 updates the sound world on
its own frame cadence: 4-5 volume updates per game frame at the start). No sound kind is missing
or extra. Pan values and the per-sound pitches (0.5000, 0.6661) agree where both play the same
sound; the random pitch spreads (0.3139 / 0.3124) are draws.

### 12.5 What was checked and is not a host bug

- Sample rate: the N64's AI runs at 22,048 Hz (dacrate 2207), the port at 22,050; 368 samples
  per retrace on both. The LUS player opens at 22,050 (main.cpp), the pacer follows its buffer.
- Volume settings: the N64's option bytes at `gEngineApp_m_Settings + 0x13` are 01 80 FA FF
  (n64trace --peek), the defaults the port sets (`gWarmBoot`).
- Music: the notes were already identical (section 9); the title now also matches in level,
  pan, spectrum and waveform (0.96).
- Reverb tails and DC: the N64 has the same small negative DC (-30 to -100) as the port.
- The envelope mixer: steps every 8 samples (the HLE interpolates inside the 8); no measurable
  level difference (the title and the hub within 0.1 dB).

### 12.6 Next

1. A fully synced sound-event comparison needs the same random numbers inside the level on both
   sides (the N64 draws on its own cadence); `sndpcmp.py` is ready for it.
2. The ENVMIXER's ramp inside each group of 8 samples (linear on the RSP) for very fast
   envelopes (clicks at note starts are not audible in the recordings).
3. The speech streams (adec) were not part of these scenes; section 10's correlation stands.
