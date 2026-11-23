# libultra: which one the game links, and the way to ultralib

Turok 2 links Nintendo's **libultra 2.0I, `libgultra_rom`** (the GCC build of
the library, the ROM flavour), with libaudio and libc from the same archive.
This page gives the evidence, the per-function map against
[decompals/ultralib](https://github.com/decompals/ultralib), and how the build
compiles 120 of its modules from ultralib's own sources (round 22). (The older
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

`tools/ultralib_version.py` compiles every C file of `lib/ultralib`
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

## The build: 120 modules from lib/ultralib (round 22)

ultralib is a **git submodule**, `lib/ultralib`, pinned at e24c836 (it has no
license file, so none of its sources are copied into this repository). A fresh
checkout needs `git clone --recursive` or `git submodule update --init`; the
N64 build needs nothing else from the network. The port does not use it.

**120 of the library's 173 modules** (209 game files, one function each) are
compiled from ultralib's own files and linked in place of the game's C:
96 in `.code` (libaudio, gu, io, os, libc), the 12 Controller Pak files of the
first paged VM module and their 12 copies in the second.
`versions/ultralib_objects.us.txt` lists them, each with the game files it
replaces. `make verify` is byte-exact.

How it is built:

- **Flags** (Makefile `UL_CPP_FLAGS`, `UL_CC_FLAGS`): ultralib's defines for
  2.0I `libgultra_rom` (`BUILD_VERSION=VERSION_I`, `_FINALROM`, no `NDEBUG`,
  `_MIPS_SZLONG=32`, `F3DEX_GBI`), the game's cc1 at `-O3 -mfp64 -fforce-addr
  -fno-builtin -funsigned-char`, then `tools/sn_as.sh` like every C file. The
  preprocessor runs from `lib/ultralib/src`, as ultralib's own build does:
  `assert()` passes `__FILE__`, and the ROM has the names as `audio/csplayer.c`.
- **The yaml**: one `c` subsegment per object
  (`../../lib/ultralib/src/audio/csplayer`) in place of the module's function
  files, the stray padding words after it untouched; its `.rodata` as one
  `.rodata` subsegment where the ROM has it, splitting splat's unnamed rodata
  pieces where it starts or ends inside one.
- **The second VM module's copies**: the game links the pfs objects twice, and
  the second copy's functions are `osPfsAllocateFile_2` and so on.
  `lib/ultralib_vm2/io/<file>_2.c` is a two-line wrapper (ours): it includes
  `vm2_names.h`, which `#define`s every ultralib global with a `_2` twin in the
  game to that name, then ultralib's file. Same code, the second copy's names.
- **Data**: the game keeps libultra's `.data` in the KSEG0 data that lies in
  `code_rodata`'s range, so `sl.c`, `motor.c` and `thread.c` have their
  `.data` renamed to `.rodata` on the built object and placed there, where
  their symbols are (`alGlobals`, `__osMotorinitialized`, `__osThreadTail`).
- **Names**: `versions/ultralib_aliases.us.txt` (an ld script) gives the
  ultralib names the game spells otherwise: `__osContPifRam` (the game's
  `__osContBuffer`, Rage Wars' name), `__osMaxControllers`
  (`__osContNumControllers`), motor.c's four tentative buffers
  (`__osMotorStartData` ...) and `func_0029E460` (the game's callers of
  `memcpy`, which keeps an address name in the game C).
  `versions/ultralib_renames.us.txt` is applied to the built objects
  (`objcopy`, Makefile): the game's names for two static functions
  (`__CSPSetUsptFromTempo` is csplayer.c's `__setUsptFromTempo`,
  `__osClearPage_2`), so the ELF types them as functions for
  `tools/port/gendata.py` (an untyped address became port data); the
  `.data=.rodata` renames above; and the two names the game C uses inside
  seqplayer.c's `.rodata` (`sSeqPlayerAssertExpr`, `sSeqPlayerAssertFile`,
  `objcopy --add-symbol`). Every named symbol of `build/ref.elf` is in the
  new ELF at the same address, the swapped pair excepted. Linking
  `synsetvol.c` showed that `_timeToSamples` and `_timeToSamplesNoRound` had
  their names swapped in the game (0x00294C30 cuts to 16 samples: ultralib's
  `_timeToSamples`); they are renamed.
- `tools/filter_defined_syms.py` reads `build/lib` too, and does not count a
  COMMON symbol as defined (ultralib's `pfsisplug.c` has a tentative
  `__osPfsPifRam`; the linker script discards COMMON, the symbol files place it).

`tools/ultralib_build.py` does the work: for every module whose functions are
all identical in the map, it compiles the ultralib file with the Makefile's
flags and checks the whole object against the ROM (`.text` word for word with
relocations masked, `.rodata` found where it covers the game C's own, no
`.data`/`.bss`); `--write` rewrites the yaml and the list. Re-run:
`python tools/ultralib_build.py --write && make setup`.

**What stays the game's C**, and why:

| Modules | Why |
|---|---|
| 41 (38 functions differ, + the files they share) | not identical: float code (KMC GCC 2.7.2 vs SN's 2.8.1), Acclaim's changes, a revision ultralib does not have, libc (above) |
| `controller.c`, `timerintr.c` | every function identical, but in another order than ultralib's file (the game has `__osPackRequestData` before `__osContGetInitData`, `__osTimerServicesInit` after `__osTimerInterrupt`): 2.0I's file had another order |
| `save.c`, `sirawdma.c`, `sprawdma.c`, `pirawread.c`, `sirawread.c` | their `.rodata` (assert's strings) ends inside a word, and the ROM has stale linker bytes in that word's padding: an object section is padded with zeros, and the next piece cannot start inside a word (`subalign: 4`) |
| `siacs.c`, `piacs.c` (`.data` 4 + `.bss` 4), `vimgr.c` (0x1C + 0x1220), `pimgr.c` (0x28 + 0x11CC) | `.bss` of their own: the game's BSS has no segment in the yaml (its symbols are addresses), and splat would auto-link the object's `.bss` after `.code`'s; it needs a placed NOLOAD section first |
| `aisetnextbuf.c` | its one byte of `.data` is a function-local static (`hdwrBugFlag`) no game symbol places, and it would end mid-word |
| the 23 asm functions | ultralib's `.s` need its `asm.h`/`regdef.h` through the C preprocessor; the game's hand-written asm stays |

Single functions out of a file that differs cannot come from ultralib: an
object brings the whole file, and SN's cc1 here has no `-ffunction-sections`
path the linker script could pick from.

**The port** keeps the game's C for the files of these modules it links
(libaudio, gu, libc, the first VM module's pfs). It is 64-bit: the game's C
is typed for the host (`sptr`, the port's views), ultralib's assumes 32-bit
pointers, and port/runtime replaces the kernel parts anyway. Those files are
still in `src/us/code`, compile to the same code as before and are not linked
by the N64 build (the list says which). 121 game files, which
neither build linked any more (the kernel/VI/PI/SI/AI files port/runtime
replaces, motor.c's three, the second VM module's copies, `sprintf`), are deleted.

Checked in round 22 (Linux, Release at the coordinator's `-O2`): `gendata.py` rerun with
`decls.json` deleted, `mklayout.py` (layout committed), `aligncheck.py` the 2 known
`GameNoteMenu_SelectYes` sites; the boot and the 3 demos det `6ab97584…` (52,000 lines); the
default whole-game run (300,000 frames, `aimdrive.py`) det `dc8126d3…` (600,000 lines, the one
`death:` at 263,578). `port/gen` changed only where the ELF is now exact: `alGlobals` and
`__osFaultedThread` are sized by their objects (4 bytes each, where gendata had read up to the
next label: three and two garbage pointer slots fewer).

**After the merge (round 22 coordinator).** Merged first, then k4, k3, k2 on top: every
`tools/asm_diff.py --base f980f639` run shows the same 4 files and nothing else
(func_00294540/_002949A4/_00294C30/_00294D18.c: only the `jal` targets of the swapped pair, the
old source's name now resolving to the other address), and the ROM is identical (`make
verify`). `tools/ultralib_build.py` (check) still takes 0 more modules (120 of 173);
`tools/ultralib_version.py --versions I --map` and `tools/modules.py --write` were re-run
(`versions/ultralib.us.txt`, `modules.us.txt`: the swapped pair, `memcpy`). `tools/progress.py`
counts the ultralib objects' functions as linked C now (it read 94.76% engine without them) and
resolves asm_origin's address names through the ELF's new names: engine and total 100%. A fresh
`git clone --recursive` builds the ROM (`make setup && make verify`).

**Round 23**: the 88 game files of ultralib-built modules that only the port links still sit in
`src/us/code` beside the game's own C; they move out (a port libultra directory), and the
game files move into the recovered modules (docs/MODULES.md).
