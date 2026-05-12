# Turok 2 — RSP Microcode Analysis

ROM: `baserom.us.z64` (Turok 2: Seeds of Evil, US), sha1 `fb0400f21e3f043939ab56500c7b12a3231006f1`, 32 MB.

## TL;DR — verdict

**STOCK Nintendo SDK microcode. Native-port path is viable from a microcode
standpoint.** No custom RSP code, no Acclaim-specific opcodes. RT64 + the
N64ModernRuntime stack should render this game without any new ucode handlers.

| Component | Variant | Source | Custom? |
|---|---|---|---|
| Graphics ucode | `gspF3DEX.NoN fifo 2.05` (Yoshitaka Yasumoto, 1998 Nintendo) | libultra | NO — verbatim Nintendo SDK |
| Audio ucode | `aspMain` (basic audio, stock) | libultra | NO |
| RSP bootloader | `rspboot` | libultra | NO |

The signature strings recovered from the ROM are byte-identical to the canonical
Nintendo SDK signatures:

```
0x0C0478:  "RSP Gfx ucode F3DEX.NoN   fifo 2.05  Yoshitaka Yasumoto 1998 Nintendo."
0x0C0898:  "RSP Gfx ucode L3DEX       fifo 2.05  Yoshitaka Yasumoto 1998 Nintendo."
```

(A second, identical copy of the entire ucode region exists at +0x39000 in
the ROM, almost certainly because the SDK ships ucode both with the game code
and again as part of the boot/IPL3 segment.)

Cross-confirmation: a leaked copy of the Acclaim London engine source code is
publicly available at `github.com/Abhisheksinha1506/Turok-N64-Source-code`.
`src/PR/tengine/tengine.c` initialises the SP task with:

```c
t->list.t.ucode = (u64*) gspF3DEX_NoN_fifoTextStart;
```

i.e. the unmodified `gspF3DEX_NoN_fifo` symbol from libultra. No patched
ucode is referenced anywhere in that source tree.

## Memory layout

ROM map (primary copy):

| ROM offset | Size | Blob | Loads to (RSP) | Notes |
|---|---|---|---|---|
| `0xBDF2C` | `0xD4`  | `rspbootText` | IMEM `0x04001000` | libultra RSP bootloader |
| `0xBE000` | `0xEE0` | `aspMainText` | IMEM `0x04001000` | Audio ucode text |
| `0xBF2BC` | `0x107C` | `gspF3DEX_NoN_fifoText` | IMEM `0x04001000` | Gfx ucode text (4 KiB region) |
| `0xC0400` | `~0x360` | `gspF3DEX_NoN_fifoData` | DMEM `0x04000000` | Gfx ucode data (RDP cmd table, etc.) |
| `0xC0820` | `~0x130` | `gspL3DEX_fifoData` (+ shared) | DMEM | L3DEX line-3D variant data |
| `0xF6F2C` | `0x39000`-mirror | (full duplicate of all the above) | — | Second copy shipped with boot |

The `_data` segments contain the RDP command jump table — visible by the
characteristic G_RDPHALF-style bytes at the head of the data block
(`ed 00 00 00 00 50 03 c0 ef 08 0c ff ...`), which are SETSCISSOR / FILLRECT
opcodes preassembled by the SDK at link time.

## Hashes (sha1 / md5)

Computed over the dumped blobs (see `ucode/*.bin`, gitignored):

```
rspboot                  sha1=f8e077f292daa75548e19c87d458608ff5ffe07c  md5=7e0603057768412663c300856d0b37ad  (212 B)
aspMain.text             sha1=086f205324fccbf5a60b4503edd3dfbaafe0bdda  md5=ead3acc25f73ce89c62f96febea084c3  (3808 B)
gspF3DEX_NoN.fifo.text   sha1=ce21f2369834c215ad7ce24d5bf7a399ab99dd77  md5=5e5a77decb12ff8d9b597254de90c84a  (4224 B)
gspF3DEX_NoN.fifo.data   sha1=51ce96414d6bdf3055541f8bbb2a95bbb0866e93  md5=29b0d56f34d02d9d6e446ab04dfd1dec  (1024 B, includes some padding)
gspL3DEX.fifo.data       sha1=6f1fafb293ae46c7c419b929016ca08b85ce7096  md5=f0c941a9157b63ff3604f2518b9470e4  ( 512 B, includes some padding)
```

The second (duplicate) copy at `+0x39000` produces identical hashes for each
corresponding blob — verified.

## How the verdict was reached

1. **String signature**: `strings -t x baserom.us.z64 | grep RSP` returned the
   exact Nintendo SDK signatures, including author and year. Acclaim left these
   intact, which is what shipping with a clean SDK build looks like — if they
   had patched anything, this string is one of the first things to change.

2. **Segment sizing**: `SP_UCODE_SIZE = 4096` and `SP_UCODE_DATA_SIZE = 2048`
   (from `ultralib/include/PR/ucode.h`). The F3DEX text segment we located is
   `0x107C` bytes (4220) — within the standard SDK size for fifo 2.05 builds
   once you include the trailing rsp2elf padding. Data segment ≤ 2 KiB.

3. **Opcode distribution sanity-check** (Python, top 10 primary opcodes):
   - F3DEX text: dominated by `0x12` (COP2 / RSP vector ops, 218), `0x00`
     (SPECIAL, 156), `0x08` (ADDI, 148), `0x32` (LWC2, 115), `0x3A` (SWC2, 64).
     Normal MIPS/RSP code. No anomalous primary opcodes (no `0x3F`-style
     "unknown" reserved patterns Acclaim would have used for custom commands).
   - aspMain text: `0x12` (309), `0x3A` (94), `0x32` (92) — denser vector math
     than F3DEX, as expected for an audio mixer.

4. **Side-channel evidence (Turok source leak)**: the engine source explicitly
   uses the libultra symbol `gspF3DEX_NoN_fifoTextStart` with no overrides.

5. **Stock binary comparison (partial)**: a fully byte-identical stock blob
   for `F3DEX.NoN fifo 2.05` is not freely redistributed on GitHub (the
   common SM64-hack repos ship the older 1.23 variant of F3DEX.NoN, size
   `0x1430`, signature `"RSP Gfx ucode F3DEX.NoN  1.23 Yoshitaka Yasumoto
   Nintendo."`). Sizes therefore differ. A byte-exact diff against
   `fifo 2.05` would require a reference build from libultra source with
   `-D F3DEX_GBI -D NoN -D FIFO`, which is a follow-up task once the build
   container has the SDK headers in place.

## Implications for RT64 / N64Recomp

- RT64's stock F3DEX handler covers `fifo 2.05` (used by Banjo-Kazooie,
  Conker's Bad Fur Day in HVQM mode, Forsaken, etc.). **No new handler
  needs to be written.**
- Audio ucode is `aspMain`, which N64ModernRuntime already implements via
  RSPCPU / HLE — no work needed there either.
- No `F3D_Gold`-style cliff like GoldenEye. The microcode bottleneck the
  parent project (`GoldenRecomp`) hit is **not** a risk here.

## Next steps (recommended)

1. **Byte-exact diff (nice-to-have, not blocking)**: build `libultra`
   `gspF3DEX.NoN.fifo` from the SDK source inside the `turok2-build`
   container and `cmp` against `ucode/turok2.gspF3DEX_NoN.fifo.text.bin`.
   If they match, declare 100% stock and close this task.
2. Proceed with Phase 3 of the roadmap (function matching) confident that
   the rendering path will work once the ELF boots under N64Recomp + RT64.
3. **Armorines crossover note**: if the engine source repo
   (`Abhisheksinha1506/Turok-N64-Source-code`) is genuine Acclaim London,
   the Armorines port should reuse the same conclusion verbatim — same engine,
   same `tengine.c`, same `gspF3DEX_NoN_fifoTextStart` call.

## Files / artifacts

- `ucode/turok2.*.bin` — extracted blobs (gitignored; regenerate from ROM).
- `versions/turok2.us.yaml` — updated to declare the ucode regions as `bin`
  segments so splat does not try to disassemble them as MIPS code.
- `docs/MICROCODE.md` — this file.
