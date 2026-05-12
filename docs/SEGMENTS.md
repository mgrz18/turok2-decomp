# Turok 2 (US) ROM segment map

Working theory of the ROM layout, derived empirically (no source). Each
boundary lists the evidence used to fix it so future iterations can
challenge or refine.

ROM: `baserom.us.z64`, 32 MB (0x2000000), sha1 `fb0400f21e3f043939ab56500c7b12a3231006f1`.

VRAM mapping (main + code): ROM 0x1000 = VRAM 0x80000400, delta `0x7FFFF400`.

## Top-level segment table

| ROM start  | Type    | Name      | Notes |
|------------|---------|-----------|-------|
| 0x00000000 | header  | header    | N64 header (0x40 bytes). |
| 0x00000040 | bin     | boot      | IPL3 boot code (0xFC0 bytes). |
| 0x00001000 | code    | main      | SN64 SNMAIN stub (0x100 bytes ending at `jr ra; nop`). |
| 0x00001100 | code    | code      | Engine + libultra/libaudio + .rodata, plus the SN64 LNK debug block. |
| 0x0013B5F0 | code    | virtual   | VM/virtual code segment (T3 calls this "virtual"). |
| 0x0020F000 | bin     | assets    | Compressed asset / texture / audio / level / model data. |
| 0x01FF7AE4 | bin     | trailer   | 0xFF ROM padding to 0x2000000. |

## Sub-segment boundaries inside `code` (high confidence)

| ROM        | Subtype | Evidence |
|------------|---------|----------|
| 0x00001100 | asm     | First instruction after SNMAIN's `jr ra; nop`. |
| 0x0001F100 | asm     | Splat `find_file_boundaries` — symbol ends with extra-nop pad. |
| 0x000438B0 | asm     | Splat `find_file_boundaries`. |
| 0x000A5FD8 | rodata  | First long ASCII string ("simp object index"). 4 KB before this, opcode density still 99%; here it drops as `.rodata` strings dominate. Confirmed by raw byte dump. |
| 0x000C3000 | asm     | Function-prologue density jumps back to ~40 prologues per 4 KB; matches libultra/libaudio/glu .text body. |
| 0x000DC000 | data    | Opcode density falls; this region holds libultra constants + alSyn voice/sample tables. Disassembling it as code yields spurious `lb $reg, -0xXXXX($0)` patterns. |
| 0x000F8000 | data    | Another rodata pocket (AI animation type names as ASCII: `AI_OBJECT_PICKUP_WEAPON_MPLAYER` etc.). |
| 0x000FC000 | data    | `.sdata` / `.data` initializer tail (pointer-heavy, ~80% opcode-shaped but with frequent 0x800XXXXX values — pointers, not code). |
| 0x00107000 | bin     | First **SN64 LNK record** at this offset (literal bytes `4c 4e 4b 02`). Section header lists `.rdata .text .data .sdata .sbss .bss .ctors .dtors`; the bytes that follow are debug strings — Windows-style source paths like `D:\turok2\n64\source\engine\STDAFX.H`. Pure linker/debug metadata. |

## Sub-segment boundaries inside `virtual` (high confidence)

| ROM        | Subtype | Evidence |
|------------|---------|----------|
| 0x0013B5F0 | asm     | Function-prologue density resumes after the LNK debug block. The VM dispatch trampolines live here. |
| 0x001E0000 | data    | Opcode density holds 100% until 0x1DFC00 then drops to ~80% with sustained low-zero stretches → end of `.text`, .rodata begins. |

## Asset region structure (low confidence — needs decoding)

- 0x0020F000 begins with what looks like a **monotonic 32-bit offset table** (`00 4a 9c 0c, 00 4a 9c d4, 00 4a 9d 9c ...`). The values point well past their own location, deep into the asset data. Likely the cartridge / asset directory.
- 8 distinct `0xFFFFFE00` markers scattered through 0x5BA104 … 0x1F71CA0 — probably compression terminators or section separators.
- 0x01FF7AE4 onwards is pure 0xFF ROM padding.

## SN64 LNK markers (linker section records)

Found at exactly three ROM offsets:

| Offset    | Sections declared | Trailing path string |
|-----------|-------------------|----------------------|
| 0x107000  | .rdata .text .data .sdata .sbss .bss .ctors .dtors | `D:\turok2\n64\source\...\STDAFX.H` |
| 0x137000  | (same) + `.vmpool` | `D:\turok2\n64\source\engine\virtual\...` |
| 0x139000  | .text .data only | `D:\TUROK2\N64\SOURCE\ENGINE\VIRTUAL\VMASM.S` |

The third marker is *inside* the LNK debug block, naming `VMASM.S` —
this is the source file for the virtual-machine assembler. The actual
VM-hosted code starts at 0x13B5F0, not at the LNK marker.

## Still-ambiguous boundaries

1. **0x000DC000 (libultra `.rodata` start)** — coarse; the real libultra builds split into per-file objects (osCont, osVi, alSyn, gu*), each with its own .text/.rodata pair. Until libultra symbol files are mapped onto T2's VRAM, treating this whole block as opaque `data` is safer than guessing.
2. **0x000F8000 (AI strings vs libultra tail)** — could legitimately split further into `.text` of `osSpTaskYield`, `osViBlack`, etc. The CSV-seeded libaudio symbols (`alSynAddPlayer = 0x800ea9c0`) appear stale (their VRAM addresses don't land on code), so they shouldn't be trusted as file-start hints.
3. **0x001E0000 (virtual .data tail)** — opcode density stays middling (78-95%) from here through 0x20F000. Mix of .rodata, .sdata, and possibly aligned-out .bss initializers. No clear sub-structure visible from byte stats alone.

## How to refine further

- Decode the asset directory at 0x20F000 (the 32-bit offset table) to confirm the directory layout and break out individual asset blobs.
- Cross-reference the LibTEngine `functions.csv` against splat's discovered function addresses; many seeded symbols are from T3 and don't apply.
- Once the first 200 engine functions match in m2c, splat will be able to detect `.text`/`.rodata` per-object boundaries automatically via `find_file_boundaries`.

## Quality metrics (current iteration)

Splat run on this YAML completes in ~3 s with:

- **0** `sd $31, -0x1($31)` garbage lines (vs 8.6 M previously).
- 5 `.asm` text segments + 5 `.data`/`.rodata` segments + 4 `bin` segments.
- 4.97 % of ROM classified as `code`/`asm` (~1.69 MB).
- 0.66 % `unknown` (the SN64 LNK debug block; intentionally unclassified).
- 94.37 % `bin` (assets + raw debug). The `asset` blob still needs an asset-directory pass to subdivide.

Compared to baseline (1 × 33 MB code segment dumping 8.6 M asm lines): **the actual game code is now 100 % isolated from the asset blob**, and the asset blob no longer pollutes the disassembler.

## Pass 3: libultra symbol refinement

Third refinement pass using libultra-specific signatures (COP0 instructions,
floating-point density, AI/string anchors) rather than just prologue density.

### Findings vs. previous boundaries

| Boundary             | Was       | Now       | Evidence |
|----------------------|-----------|-----------|----------|
| libultra `.text` start | 0xC3000 | 0xC3074 | First true `addiu $sp, $sp, -X` prologue. 0xC3000 fell mid-engine-function (continuation of float code starting `lwc1 f1, -0x6200($1) / cvt.s.w f0`). |
| libaudio / libultra-os split | (single block) | 0xC8A60 | First genuine `mtc0`/`mfc0` instruction in the segment (cluster of 23 COP0 ops in 0xC8000-0xC9000). Everything before is float-DSP with zero COP0 ops, consistent with libaudio. |
| libultra `gu*` math cluster | (unmarked) | 0xD1000 | 238 `lwc1`/`swc1` per 4 KB, vs <40 elsewhere — matches `guPerspective`/`guLookAt`/matrix helpers. Kept inside `os_text` for now (no separate file split). |
| libultra `.text` end | 0xDC000 | 0xDEDE0 | Last function prologue at 0xDE7E0 (`__osDispatchThread`-style FP register save). Final `jr ra` at 0xDEDD8. Immediately followed by string literals `"header\0ESTRUCTIB\0texture\0"` and a jump table of segment-2 addresses (`00 20 6C CC ...`). The old boundary 0xDC000 was 0x2DE0 too early — that region is still libultra interrupt/exception code (`mtc0 / eret` patterns at 0xDCBB8, 0xDD058, 0xDDCD0). |
| Trailing engine .text | (lost in rodata block) | 0xC2ED8 | Prologue scan in the supposedly-rodata range `code_rodata_tail` (0xC0950-0xC3074) surfaced 3 short engine functions: 0xC2ED8 (`jr ra` @ 0xC2F28), 0xC2F2C (ends 0xC2FD4), 0xC2FE0 (ends 0xC3070). These were previously classified as rodata, which silently misclassified ~0x19C bytes of real code. Now split as a trailing asm subsegment of `code_rodata_tail`. |

### Libultra string anchors

`strings -a -t x baserom.us.z64` confined to 0xDC000-0xF6F2C surfaces the
classic libultra/libaudio source-path strings, anchoring the .rodata
region:

- 0xE3E94 `audio/csplayer.c`
- 0xE3F84 `audio/seqplayer.c`
- 0xE408C `audio/sndplayer.c`
- 0xE40B4 `audio/synthesizer.c`
- 0xE4144 `audio/env.c`
- 0xE42C4 `audio/reverb.c`
- 0xE4334 `audio/save.c`
- 0xE4494 `io/sirawdma.c`
- 0xE44B4 `io/sprawdma.c`
- 0xE44E4 `io/pirawread.c`
- 0xE4504 `io/sirawread.c`
- 0xE4524 `io/sirawwrite.c`
- 0xE45B4 `io/epirawread.c`
- 0xE3D58 `ASSERTION FAILED!! in thread `
- 0xE3D98 `Fault in thread `
- 0xE3B9C `System call exception`
- 0xE1900 `fault` (idle/fault thread names from `osCreateThread`)
- 0xE1910 `idle`
- 0xE2090 `controller pak`

These cluster from 0xE1900..0xE45B4 within the libultra .rodata block,
confirming the data subsegment runs from 0xDEDE0 forward.

### Splat coverage (after pass 3)

Coverage: **98.59 % → 98.63 %** (+0.04 pp).

Asm-typed subsegments: 6 → 8 (added `libaudio.s`, `os_text.s`,
`os_rodata.data.s`, `C2ED8.s`). Splat runs clean (0 errors, 0 garbage `sd
$31, -0x1($31)` lines, ~5 s total).

### Things that contradicted prior assumptions

1. The 0xDC000 boundary was set as "libultra .rodata starts" but in reality
   libultra .text continues another 0x2DE0 past that — the
   exception/interrupt handler block was being misclassified as data.
2. The `code_rodata_tail` segment (0xC0950-0xC3000) was not pure rodata: it
   contained 3 small engine functions clustered at the end (0xC2ED8 onward).
   This means engine .text actually has *two* halves separated by the ucode
   blob: the main block 0x1100..0xA5FD8, and a tail 0xC2ED8..0xC3074.
3. The libaudio / libultra-os boundary is observable purely from COP0-use:
   libaudio has zero `mfc0`/`mtc0` instructions, libultra OS has hundreds.
   That gives a confident split at 0xC8A60 even without per-function symbols.

### Limitations of pass 3

- `os_text` is still a single 0x16400-byte block. Per-file splits (osCont,
  osVi, alSyn, gu*, exception handlers) would need either libultra symbol
  tables for this exact build, or a more invasive function-prologue-edge
  detector. Deferred to pass 4 once `m2c` produces the first matched
  functions.
- `make verify` does not currently produce a byte-exact ROM because the
  Makefile glob `$(wildcard $(ASM_DIR)/*.s)` doesn't pick up splat's
  `us/asm/data/*.s` (rodata/data files live in a subdirectory). That is a
  pre-existing Makefile bug surfaced by — but not caused by — pass 3 and
  belongs to the build-pipeline agent.
