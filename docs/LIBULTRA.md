# libultra: which one the game links, and the way to ultralib

Turok 2 links Nintendo's **libultra 2.0I, `libgultra_rom`** (the GCC build of
the library, the ROM flavour), with libaudio and libc from the same archive.
This page gives the evidence, the per-function map against
[decompals/ultralib](https://github.com/decompals/ultralib), and what replacing
the game's own libultra C with ultralib's sources takes. (The older
[LIBULTRA-MATCHING.md](LIBULTRA-MATCHING.md) is the log of the first matching
rounds; its addresses predate the current layout.)

## The live library and the dead copy

The libultra that runs is linked into `.code` (useg, TLB-mapped): libaudio from
`__alCSeqGetTrackEvent` (vram 0x0028EF90), then gu, io, os and libc up to
`_Ldtob` (0x0029F188), then Acclaim's own `load.c` and ADPCM/MP3 decoder,
SN's host I/O, the exception stubs and GCC's libgcc. The block at ROM 0xC3074
(`libultra` segment in the yaml, vram 0x800C2474) is a dead duplicate
([SEGMENTS.md](SEGMENTS.md)). The Controller Pak code (`osPfs*`, `__osPfs*`,
`contpfs.c`) is in the paged VM module instead, twice like the rest of it.

## How the version was found

`tools/ultralib_version.py` compiles every C file of `references/ultralib`
(commit e24c836) once per version and flavour, with the defines ultralib's
Makefile and `makefiles/gcc.mk` give it (`BUILD_VERSION`, `NDEBUG` except for
2.0I `libgultra_rom`, `_FINALROM` for `_rom`, `-funsigned-char` up to 2.0J),
through the game's own compiler (SN64 GCC 2.8.1 `cc1`, `-fforce-addr
-fno-builtin`) under four code-generation settings, and counts the functions
whose words equal a function of `build/ref.elf` (relocations masked):

| ultralib | -O3 fp32 | -O3 fp64 | -O2 fp32 | -O2 fp64 |
|---|---:|---:|---:|---:|
| 2.0I `libgultra` | 247 | 259 | 239 | 249 |
| **2.0I `libgultra_rom`** | **263** | **280** | **253** | **268** |
| 2.0J `libgultra` | 212 | 222 | 209 | 217 |
| 2.0J `libgultra_rom` | 215 | 225 | 212 | 220 |
| 2.0K `libgultra` | 204 | 214 | 201 | 209 |
| 2.0K `libgultra_rom` | 207 | 217 | 204 | 212 |
| 2.0L `libgultra` | 204 | 214 | 201 | 209 |
| 2.0L `libgultra_rom` | 207 | 217 | 204 | 212 |

(fp32 = ultralib's `-mgp32 -mfp32`, fp64 = the game's `-mfp64`. Functions of
two words, `jr $ra; nop` and the like, count only under their own name.)

What tells them apart:

- **2.0I, not J/K/L**: 38 functions are identical only from 2.0I's source:
  the Controller Pak (`osPfsAllocateFile`, `__osPfsDeclearPage`,
  `osPfsChecker`, `contpfs.c` ...: 2.0J rewrote them), `contramread.c`,
  `contramwrite.c`, `crc.c`, `motor.c`, `osAiSetNextBuffer`,
  `osEPiRawStartDma`, `osPiRawStartDma` (2.0J renamed it
  `__osPiRawStartDma`), `osCreateViManager`, `osSetEventMesg`; 8 more
  (`strchr`, `strlen`, `_Putfld`, `osSetTimer`, `__osSetTimerIntr`,
  `_sndpVoiceHandler`, `__postNextSeqEvent`, `__CSPPostNextSeqEvent`) match
  2.0I and 2.0J but not K/L.
- **`_rom`, without `NDEBUG`**: 13 functions match only 2.0I
  `libgultra_rom`, the one archive ultralib builds with `assert()` live:
  `__CSPHandleNextSeqEvent`, `alFxPull`, `alSavePull`, `__handleNextSeqEvent`,
  `__lookupSoundQuick`, `__nextSampleTime`, `osEPiRawReadIo`,
  `osPiRawReadIo`, `__osSiRawStartDma`, `__osSiRawReadIo`,
  `__osSiRawWriteIo`, `__osSpRawStartDma`, `__osSpRawReadIo`. The ROM has the
  file-name strings their asserts pass to `__assert` (`audio/csplayer.c`,
  `audio/reverb.c`, `io/sirawdma.c` ..., 13 names, each twice: the live copy
  and the dead one), and `__assert` itself is 8 bytes (`jr $ra; nop`), as
  the `_rom` builds have it.
- **GCC, not IDO**: `-funsigned-char` is needed (gcc.mk sets it for 2.0D-J
  only), and the matched files carry `-O3 -mgp32 -mfp32 -fforce-addr
  -fno-builtin` (`/* cc1-flags */`, [WORKFLOW.md](WORKFLOW.md)).

One function does not fit: **`osDestroyThread`** is identical to ultralib's
2.0J+ (and IDO) loop (`while (pred->priority != -1)`), not the one ultralib
gives 2.0I GCC (`while (succ != NULL)`). Either ultralib's `#if BUILD_VERSION
>= VERSION_J || !defined(__GNUC__)` is wrong for this archive, or the game has
a patched 2.0I (ultralib lists a `2.0I_patch` it does not build). Nothing else
moves the verdict: no other function matches J but not I.

## The map: versions/ultralib.us.txt

`tools/ultralib_version.py --versions I --map` writes one line per game
function of the library range (and the Controller Pak copies in the VM
modules): the ultralib file and function, and whether ultralib's C gives its
bytes. Today:

| Status | Functions | Notes |
|---|---:|---|
| identical | 280 | 222 in `.code` (libaudio, gu, io, os, libc) + 58 in the two VM copies (pfs) |
| asm (an ultralib `.s`) | 23 | `exceptasm.s` (7), `interrupt.s`, `getsr.s`, `setsr.s`, the cache and TLB files, `getcount.s`, `sqrtf.s`; already hand-written asm in `us/asm/` ([ASM_ORIGIN.md](ASM_ORIGIN.md)) |
| differs | 38 | see below |
| not in ultralib | 0 in the range | `__CSPSetUsptFromTempo` is csplayer.c's static `__setUsptFromTempo` under Rage Wars' name |

The 38 that differ, by cause:

- **Float code** (24): libaudio's `cseq.c` (4), `seq.c` (3), `env.c` (4),
  `reverb.c` (3), `resample.c`, `drvrnew.c` (`alFxNew`), `synthesizer.c`'s
  two time conversions, gu's `lookat.c` (2), `perspective.c` (2), `cosf`, `sinf`.
  The bytes differ by a few words, mostly in single/double conversions and
  FPU register choice: ultralib's archive was built by KMC GCC 2.7.2, the game
  by SN's 2.8.1, and for float code the two do not agree. The game's C for
  these (from the Rage Wars decomps) matches with the game's flags instead.
- **Other code** (10: Acclaim's changes, or a revision ultralib does not
  have): `osInitialize` (84 of 132 words equal: the game's boot path),
  `_handleEvent` (sndplayer.c), `alSynSetFXMix` (36 words, the game's 44),
  `alSynStartVoiceParams` (59, the game's 65), `__osViInit`, `osCreateThread`,
  `_VirtualToPhysicalTask`, `osCartRomInit`, `osLeoDiskInit`,
  `osDestroyThread` (above). The scheduler (`sched.c`, at 0x0028A020, before
  libaudio, with Acclaim's message ids: docs/NAMES.md) is outside the range.
- **libc** (4): `_Printf`, `_Litob`, `lldiv` are other code; `_Ldtob` is an
  8-byte stub (the game has no float printf).

The game's own `load.c` (`alAdpcmPull`, `alRaw16Pull`, `alAdecPull`,
`alLoadParam`, `_decodeChunk`, at 0x0029F190 after libc) is Acclaim's: it adds
the ADEC decoder and is linked apart from libaudio.

## Replacing the game's libultra C with ultralib

The task (README Phase 5) is to build these functions from ultralib's sources
instead of the game's own per-function C. What the map says about it:

- **What can come from ultralib as is**: the 291 identical functions. Grouped
  by object (versions/modules.us.txt: the linker pads every object to 16
  bytes, so each ultralib file is one module), 132 of the library's 173
  modules (232 functions, 24 of the modules in the VM copies) are whole
  ultralib files every function of which is identical; their game files can
  be deleted and the ultralib file compiled in their place, all of them with
  one config: `-O3 -mfp64` with ultralib's 2.0I `_rom` defines and
  `-funsigned-char`.
- **What stays the game's**: the 38 that differ, plus the 23 asm functions
  until the `.s` files are wired the same way (ultralib's `.s` need its
  `asm.h`/`regdef.h` and `gas` with `-P`, see its Makefile).
- **Licensing**: ultralib has **no license file** (checked at e24c836). A
  vendored copy cannot be added to this repo; it has to be a git submodule
  (`lib/ultralib`, pinned) that the build checks out, like
  `references/` today but tracked.
- **Build**: the game is one function per file and splat's yaml lists every
  file; an ultralib object brings several functions, and the 16-byte padding
  between objects is stale linker bytes (the `stray_*.s` files) that the
  ultralib object will not reproduce. Each replacement is: delete the game
  files of the module, replace their yaml subsegments with one entry for the
  ultralib object, keep the stray words as they are (the strays sit after the
  object, so `make verify` still holds), and add a Makefile rule that compiles
  `lib/ultralib/src/<file>.c` with the flags of the map.
- **The port**: `port/cmake/game.cmake` compiles every `src/us/code/*.c` for
  the host, and link.cmake swaps the kernel/VI/PI parts for `port/runtime`;
  libaudio, gu and the pfs code it uses as the game's C. Files that move to
  ultralib have to be compiled from `lib/ultralib` by the port as well (with
  its `include/PR` replaced by the port's types), or stay in
  T2_LINK_REPLACED.

This round did the study and the map; the submodule and the first
replacements are a build-system change for the coordinator to schedule (they
touch the yaml, the Makefile and the port's CMake, which other parts change
too).
