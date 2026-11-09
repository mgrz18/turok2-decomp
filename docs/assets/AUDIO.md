# Audio assets (Turok 2, US)

Tool: `python tools/assets/audio.py baserom.us.z64 --out DIR` (stdlib only; ~100 s, almost all of it
in the adec stream decoder). Output: `banks.json`, `soundtree.json`, `music_waves/*.wav`,
`sfx_waves/*.wav`, `sequences/*.mid`, `streams/*.wav`, `stats.json`. Nothing it writes may be committed.

## Where audio lives

All audio is one contiguous ROM range, **0x1E0000..0xFCBFB4** (14.3 MB), not reached through
CCache__Request but through the VM pager (`VM_Alloc(size, 0, romAddr)`) and `__amDMA`:

| ROM range | size | what | established by |
|---|---|---|---|
| 0x1E0000..0x20F194 | 0x2F194 | **ctl region**: header + 2 ALBankFiles (music, sfx) | `CAudioWorld__Initialize`: `VM_Alloc(0x2F194, 0, 0x1E0000)`, `r->f4` = music bank file (`m->seq.data`), `r->bankFile` (+0xC) = sfx bank file |
| 0x20F194..0x60068C | 0x3F14F8 | **tbl**, music samples (VADPCM) | `gAudioRomBase = 0x20F194`; `__amDMA` reads `offset + gAudioRomBase` |
| 0x60068C..0xBE1814 | 0x5E1188 | **tbl**, sfx samples (VADPCM) | sfx ALWaveTable.base starts at 0x3F14F8 |
| 0xBE1814..0xFCBFB4 | 0x3EA7A0 | **sound tree file** (sfx effect table + envelopes + 159 songs/streams) | `CAudioWorld__LoadSfxDataFile`: `VM_Alloc(0x3EA7A0, 0, 0xBE1814)` |

So of the asset region 0x20F178..0x1FF7AEC (0x1DE8974 bytes), **0x20F178..0xFCBFB4 = 0xDBCE3C bytes
(46.0 %) is audio** and is fully accounted for (every byte of the tbl lies inside some wave; the tree
file is a CISet whose last offset equals its size). The 0x1C bytes at 0x20F178 are the tail of the
ctl region (the sfx ALBank header at 0x20F17C).

**Important for a1/other areas:** the "913-entry assets_index" at 0x20E334 is *not* a directory: it is
`ALInstrument.soundArray[913]` of the sfx bank (instrument at 0x20E324, soundCount = 0x391 = 913).

### ctl region pointers

The ctl region's pointers are pre-relocated to the virtual address the pager maps it at, **0x00480000**:
`rom = ptr - 0x480000 + 0x1E0000`. ALWaveTable.flags = 1 everywhere (= already relocated, as
`alBnkfNew` leaves it). Header at 0x1E0000:

```
s32 count = 2
{ ALBankFile *file; s32 tblOffset; } [2]   = {0x480014, 0}, {0x48AD0C, 0x3F14F8}
```

## Banks (.ctl), standard libaudio "B1" layout (include/structs/libaudio.h)

`ALBankFile{s16 revision=0x4231; s16 bankCount; ALBank*[]}` →
`ALBank{s16 instCount; u8 flags; u8 pad; s32 sampleRate=44100; ALInstrument *percussion; ALInstrument *inst[]}` →
`ALInstrument{u8 volume, pan, priority, flags, trem*4, vib*4; s16 bendRange; s16 soundCount; ALSound *sounds[]}` →
`ALSound{ALEnvelope*; ALKeyMap*; ALWaveTable*; u8 samplePan, sampleVolume, flags}` →
`ALWaveTable{u8 *base (tbl offset); s32 len; u8 type; u8 flags; {ALADPCMloop *loop; ALADPCMBook *book}}`.

| bank | file | ALBank | instruments | sounds | unique waves | tbl |
|---|---|---|---|---|---|---|
| music | 0x1E0014 | 0x1EAC74 | 33, no percussion | 202 | 188 | 0x20F194..0x60068C |
| sfx | 0x1EAD0C | 0x20F17C | 1 | 913 | 678 | 0x60068C..0xBE1814 |

All 866 waves are type 0 (AL_ADPCM_WAVE), order-2 books. No RAW16, and no type-2 (adec) wave
tables in the banks: type 2 is only used by the runtime `sSpeechSound` (see streams).

**Sound index order:** `CAudioWorld__Initialize` *sorts* the sfx instrument's soundArray by pointer
value at start-up. Game sound indices (the `group` field of an effect layer, `alSndpAllocate(..,
bank->inst[0]->sounds[i])`) index the sorted array. `banks.json` gives each sfx sound its
`runtime_index`.

**VADPCM** (decoded by `vadpcm_decode`, the standard N64 algorithm): 9-byte frames → 16 samples;
header byte = `scale_log2 << 4 | predictor`; the 16 signed nibbles × scale are fed with the two
previous outputs through the `order × npredictors × 8` s16 codebook (2.11 fixed point). Loops
(`ALADPCMloop{start,end,count,state[16]}`) are written into a WAV `smpl` chunk.

**Sample rate.** The synth's output rate is 22050 Hz (`ai.rate = 0x5622`); a pitch ratio of 1.0
plays a sample at 22050 Hz. WAVs are written at 22050 Hz = the sound at pitch 1.0. Music notes play
at `alCents2Ratio((key - keyBase)*100 + detune)` (csplayer); sfx play at the effect's pitch in
cents (`unk10` below, through `sPitchRatioTable`). The ALBank.sampleRate (44100) is unused by libaudio.

## Sound tree file (0xBE1814)

A CISet (`s32 n; s32 off[n+1]`, offsets relative to the set, last = size; `CISet__GetBlock`):

```
tree = set{ sfx, songs }
sfx  = set{ t0, t1, effects, volEnv, pitchEnv, panEnv }         (CAudioWorld__LoadSfxDataFile)
  t0, t1      {u32 elemSize=4; u32 count=1319; s32[]}           m->ranges / m->weights (t0 is all 10)
  effects     set[1319] of set{ cue[8 bytes], layers }           m->variants; m->rangeCount = 1319
     layers   {u32 stride=0x18; u32 count; SoundEffectInfo[count]}  (CAudioWorld__PlaySoundIndex)
  volEnv/pitchEnv/panEnv {u32 elemSize=0xC; u32 count; SndEnv[]}  141 / 85 / 6 entries
songs = set{ keys, pairs }                                       (SndSeq_SelectSong, SpeechPlayer_Update)
  keys        {u32 4; u32 count=159; s32 key[]} sorted, BinarySearch'ed (key 417 appears twice)
  pairs       set[318]: header[8] = {s32 volume; u16 half-float tempo/scale; u16}, then the data
```

SoundEffectInfo (0x16 bytes + 2 pad, include/structs/sound.h), observed ranges over the 1918 layers:
`group` 0..912 = **sfx bank sound index** (runtime order); `sample` 0..349 (non-zero → queued through
`SndQueue_Add`, a delay); `priority`, `chance` 1..100; `flags` (low 14 bits flags, top 2 pan);
`unk8`/`unkA`/`unkC` = volume/pitch/pan envelope indices (-1 = none; maxima 140/84/5 match the
table sizes); `pitch`, `unkF` (s8); `unk10` = pitch in cents (-4000..1000, SndVoice_InitPitch);
`randomRange0/1`. 53 effects have no layers. Effect ids used by game code = index into `effects`.

### Songs: 19 alCSeq + 140 adec

The 159 entries share one table; the data's first word tells them apart (0x44 = the first
`ALCMidiHdr.trackOffset`):

| keys | count | kind | rate |
|---|---|---|---|
| 1..11, 15..22 | 19 | alCSeq music (SndSeq_Load: `alCSeqNew`, played with the music bank) | — |
| 310..386 | 68 | adec stream | 9450 Hz (speech, up to 3.7 s) |
| 399..517 | 60 | adec stream | 22050 Hz (cinema/ambient/jingles, up to 46.6 s) |
| 600..614 | 12 | adec stream | 9450 Hz (speech, up to 39 s) |

**alCSeq** (libaudio compressed MIDI, ultralib `cseq.c`): `s32 trackOffset[16]; s32 division (480)`,
then per track: var-len delta, MIDI events with running status, *note-on carries a var-len
duration and there are no note-offs*, `FF 51` tempo, `FF 2F` end, `FF 2E xx xx` loop start,
`FF 2D loopCt curCt s32 backOffset` loop end, and the byte-level back-reference compression
`FE hi lo len` (copy `len` bytes from `hi:lo + 4` bytes back; `FE FE` = literal 0xFE).
`cseq_to_midi` turns it into SMF format 1 (note-offs synthesized from durations, loop points as
`FF 06` marker events "loopStart"/"loopEnd N"; loops are not unrolled). Program numbers are music
bank instrument indices (all < 33 in all 19 songs).

**adec** (Acclaim's MDCT codec; `adecStreamOpen`, `adecNextFrame`, `adecDecodeFrame`,
`adecHuffDecode`, `adecDequantize`, `adecImdctFrame`, ported 1:1 in `AdecDecoder`):

```
u32 sampleRate (9450 / 22050); u32 frameCount; u32 bitstreamBytes; bitstream (16-bit BE words, MSB first)
```
Each frame = 256 samples. Frame 0 starts with 16 raw 6-bit scale indices; later frames carry 16
Huffman-coded scale deltas. Then 256 Huffman-coded coefficients in 3 segments (0..0x22, 0x23..0x7B,
0x7C..0xFF), each with its own tree. Huffman leaves: `(v & 0xF00) in {0, 0xF00}` literal (s16),
`0x900` escape (next 16 bits raw), `0xAxx` dictionary phrase xx (a run, e.g. "11 0's"), `0xB00`
end of frame (rest zero). Trees/phrases/segments are in code .rodata: `gAdecHuffSegments`
(0x800B8C54), tables 0x800B8BC0/BE4/C08/C2C, `gAdecBandEnds`, `gAdecStepSizes` (2^(k/4) steps);
VRAM→ROM delta for that segment is 0x7FFFF400. Dequantize `coef / step[scale[band]] / 32768` over
16 bands, then a 256-point IMDCT through a 128-point complex FFT with pre/post twiddles and a
128-sample overlap, output ×23170.475, rounded and clamped to s16. Played as the runtime
`sSpeechSound` (wave type 2, `alAdecPull`) at pitch `rate / 22050`.

## Verification (listen-free)

- adec: for **all 140 streams the decoder consumes exactly `bitstreamBytes`** (header field 3) after
  `frameCount` frames: the Huffman trees, escapes, phrases and segment switching are right.
  clipping 0.0005 % on average; waveform plots show speech-like syllable envelopes
  (9450 Hz) and smooth periodic waveforms when zoomed.
- VADPCM: 866 waves decoded; samples are continuous (no frame-boundary jumps), sustained
  instruments have zero-crossing rates of 75-260 /s, cymbal/noise-like ones up to ~8700 /s;
  mean clipping 0.05 % (music) / 0.004 % (sfx), what N64 VADPCM normally gives.
- alCSeq: all 19 parse to their `FF 2F` end on every track with no unknown events; 139..9053 notes,
  12-16 tracks, tempos 90-200 BPM, program numbers within the 33-instrument bank.

## Open questions

- The 8-byte effect `cue` block and t0/t1 (1319 × s32 each) are not fully named (game side:
  `CAudioWorld__PlaySoundIndex`, `CAudioWorld__PickVariant`).
- Song header u16 at +4 (`HalfToFloat`, 0x4200 = 3.0 / 0x000A for streams) and +6: tempo scale vs
  stream volume semantics differ between SndSeq_SelectSong and SpeechPlayer_Update.
- Names: song keys / effect indices → human names must come from game code (Music_SetTrack call
  sites, level data, cinema scripts) — cross-area (a4 levels/cinema reference these ids).
- For a port: banks can be shipped pre-decoded (WAV/PCM) or VADPCM-as-is for libultraship's audio
  (libaudio HLE); adec streams are easiest pre-decoded to PCM at extraction.
