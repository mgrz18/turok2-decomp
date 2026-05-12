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

## Pass 4 — Findings

Pass 4 is an **append-only investigation round**. It does not modify
`versions/turok2.us.yaml`; the proposed unified diff at the bottom is
hand-off material for a future edit round. Five investigation areas:

1. libultra OS thread / exception cluster ROM offsets
2. ROM trailer boundary verification (off-by-N from pass 3)
3. Asset directory table decode (the so-called "monotonic offset table")
4. Virtual / VM region internal sub-module structure
5. Engine `.text` / `.rodata` finer-grained split refinement

### 4.1 OS thread / exception cluster

**Evidence**:

`symbol_addrs.us.txt` seeds (from LibTEngine, T3 origin) place
`osCreateThread` at VRAM `0x800ddb30` (ROM `0xDE730`). With the existing
yaml's `libultra` segment VRAM=0x80000500 / ROM=0xC3074 (delta
`0x7FFFF400`), that lands **after** pass-3's libultra `.text` end at
`0xDEDE0`. **The seeds are stale and do not map onto T2's libultra
build.** They should not be treated as ground truth for T2 function
locations.

What *is* observable directly:

- **COP0-density clustering** (mfc0/mtc0 + eret) within the libultra
  segment — the only places those instructions appear:

  | Cluster ROM range  | COP0 ops | eret | Probable cluster                  |
  |--------------------|----------|------|-----------------------------------|
  | 0xC8800-0xC9000    | 20       | 0    | `__osDisableInt` / `__osRestoreInt` (interrupt mask) |
  | 0xD5800-0xD6800    | 13       | 1    | `__osDispatchThread` / `osStartThread` (mtc0 STATUS @ 0xD5C10, 0xD6538; eret @ 0xD66D8) |
  | 0xD7800-0xD8000    | 23       | 0    | `__osEnqueueAndYield` / `__osCleanupThread` (mtc0 STATUS @ 0xD7BFC, 0xD7C18, 0xD7EC4, 0xD7EE0) |
  | 0xDC800-0xDDD00    | 61       | 2    | Exception / TLB / fault handlers (eret @ 0xDD218, 0xDD3C8) |

- **Concrete prologues** found via `addiu sp, sp, -X` scan inside the
  libultra `.text` body. Total 224 prologues over 0xC8A60..0xDEDE0
  (~10 functions per KB on average — consistent with libultra's many
  short helpers).

- Last prologue at **0xDE7E0** with `addiu sp, sp, -8` followed by FP
  register saves — likely `__osDispatchThread` or a context-restore
  routine. Final `jr ra` at 0xDEDD8, immediately followed by rodata
  strings (matches pass 3).

**Confidence**: medium for cluster identification (COP0 ops bound the
thread/exception code well), low for individual function naming (no
T2-specific symbol map yet).

**Proposed action**: split the existing `os_text` (0xC8A60..0xDEDE0)
into named asm subsegments at the cluster boundaries so future m2c
matching can target one cluster at a time. Suggested cuts:

| ROM      | Suggested name        | Rationale                              |
|----------|-----------------------|----------------------------------------|
| 0xC8A60  | `os_text_audio_sched` | Audio post-processing + scheduler glue (no COP0 yet, sparse prologues until 0xCB000) |
| 0xCB000  | `os_text_io`          | osPi*, osSi* I/O wrappers (28-prologue cluster 0xCB000..0xD0000) |
| 0xD1000  | `os_text_gu`          | guPerspective / guLookAt / matrix math (238 lwc1/swc1 per 4 KB; established in pass 3) |
| 0xD5000  | `os_text_thread`      | Thread dispatch + queue helpers (COP0 cluster 0xD5800-0xD8000) |
| 0xD8000  | `os_text_msg_timer`   | osSendMesg / osRecvMesg / osSetTimer (back to pure non-COP0 prologues) |
| 0xDC800  | `os_text_exception`   | Exception vectors & fault handler (COP0 + eret cluster) |
| 0xDDE00  | `os_text_tail`        | Trailing helpers up to 0xDEDE0          |

These boundaries are confidence-medium. They are based purely on
opcode-shape signatures; per-file naming would need libultra source
line-up.

### 4.2 ROM trailer boundary (correction)

**Evidence** (`xxd -s 0x1ff7a40 -l 192`):

```
01ff7ae0: 0000 0001 0000 0000 0000 0000 ffff ffff
01ff7af0: ffff ffff ffff ffff ffff ffff ffff ffff
```

Backwards scan from end of ROM: last non-`0xFF` byte at **0x1FF7AEB**
(value 0x00). First 0xFF padding byte at **0x1FF7AEC**.

**Pass 3 said `0x1FF7AE4`. Pass 4 corrects it to `0x1FF7AEC`** — off by
8 bytes. The 8-byte gap is two trailing zero words
(`0000 0000 0000 0000`) which are *data*, not padding. They look like a
table terminator (the preceding region is a long run of
`00000001 / 00000007 / 0000000b / ...` words — possibly a small
checksum table or refcount array).

**Confidence**: high.

**Proposed action**: change `trailer` segment start from `0x1FF7AE4` to
`0x1FF7AEC`. The 8 bytes preceding belong to the `assets` segment.

### 4.3 Asset directory at 0x20F000 — actually starts at 0x20E334

**Evidence**:

The `xxd -s 0x20F000 -l 256` dump shows 64 monotonic 32-bit big-endian
values (`00 4a 9c 0c, 00 4a 9c d4, …`). But scanning backward from
0x20F000 shows the monotonic run *continues* into the preceding bytes:

```
0x20EFF0:  00 4a 98 ec  00 4a 99 b4  00 4a 9a 7c  00 4a 9b 44
0x20F000:  00 4a 9c 0c  00 4a 9c d4  00 4a 9d 9c  00 4a 9d bc
```

A **913-entry, monotonic, strictly-increasing 32-bit table** runs from
ROM `0x20E334` to `0x20F178` (0xE44 bytes). After 0x20F178 the byte
stream becomes high-entropy compressed data with no further structure
visible.

Table characteristics:
- entries: **913**
- first value: `0x0048AD2C`, last: `0x004AE26C`
- value spread: **0x23540 bytes** (~144 KB)
- spacings (sample): 0xC8, 0xC8, 0x20, 0x20, 0xC8, 0xC8, 0xC8, 0xC8, …
  Very stride-y with 0xC8 / 0x20 dominating; min diff 0x20, max diff
  0x100.

The values are **relative offsets**, not ROM-absolute pointers
(absolute interpretation lands at ROM 0x48AD2C, mid-engine-text, which
makes no sense). With base = end-of-table (0x20F178), targets land at
ROM 0x699EA4..0x6BD3E4 — deep inside the asset blob, plausible record
references.

No `MIO0`/`Yay0`/`Yaz0` magic bytes anywhere in the asset region. The
pointed-to records are dense high-entropy data — probably raw
compressed payload of an Acclaim-proprietary format. Without a separate
record-header decoder we can't subdivide further from this pass.

**Contradiction with prior passes**: pass 3 said "0x0020F000 begins
with what looks like a monotonic 32-bit offset table." That table
actually starts 0xCCC bytes earlier, at **0x20E334**. The
`virtual_rodata` subsegment ends ~0xE334 sooner than the yaml claims.

**Confidence**: high for the table extent; medium for the
end-of-virtual-data boundary (the segment between 0x1E0000 and 0x20E334
is still .rodata / .sdata and probably mixes string pools + the
trailing portion of the VM dispatcher).

**Proposed action**: split the existing `virtual.rodata` (0x1E0000) and
`assets` (0x20F000) boundary so the directory becomes its own
subsegment:

- `virtual.rodata`: 0x1E0000 .. 0x20E334 (rodata)
- `assets_index`:   0x20E334 .. 0x20F178 (data — typed as `data` so
  splat surfaces the 913 uint32s as a labeled blob)
- `assets`:         0x20F178 .. 0x1FF7AEC (bin, the compressed payload)

This recovers ~0xCCC bytes of mis-classified virtual rodata and the
0x178-byte directory becomes addressable.

### 4.4 Virtual / VM region — internal modules

**Evidence**: 4 KB-window prologue-density sweep over 0x13B000..0x210000:

| ROM range          | Status   | Activity (avg prologues/4KB) |
|--------------------|----------|------------------------------|
| 0x13B5F0..0x17A600 | TEXT     | 8-31 (dense, sustained)      |
| 0x17A600..0x186000 | DATA     | 0 over 11×4KB (.rodata pool) |
| 0x186000..0x1B3400 | TEXT     | 9-36 (dense)                 |
| 0x1B3400..0x1BF9FF | DATA     | 0 over 12×4KB (.rodata pool) |
| 0x1BFA00..0x1DEE00 | TEXT     | 5-29 (dense)                 |
| 0x1DEE00..0x20E334 | DATA     | <2 (rodata; ends at directory) |

So `virtual` is **three sub-modules**, not one. Fine-scan with 0x200
windows nails the gaps:

- text→data flip at **0x17A600** (prologue count drops from 1 at
  0x17A400 to 0 at 0x17A600)
- data→text flip at **0x186000** (prologue density resumes — 18
  prologues in [0x18A000..0x18B000])
- text→data flip at **0x1B3400** (drops to 0)
- data→text flip at **0x1BF9FF** (jumps to 4 at 0x1BFA00)
- text→data flip at **0x1DEE00** (last prologue at ~0x1DE7E0)

The three modules look like overlay-style blocks the VMASM dispatcher
swaps in. Without the VM table decoded we can't name them — labelling
them `virtual_text_0`, `virtual_text_1`, `virtual_text_2` is safe.

**Confidence**: high for the three-module decomposition; medium for the
exact 0x200-aligned boundaries (sub-page alignment may shift a few
bytes once function-end markers are checked).

**Proposed action**: replace single `virtual` asm subsegment with three
asm + three rodata subsegments.

### 4.5 Engine `.text` / `.rodata` split refinement (0x1100..0xA5FD8)

**Evidence**: 4 KB-window prologue-density sweep 0x1000..0xA8000.

Density holds 4-43 prologues per 4 KB across the entire engine `.text`
range. **No multi-page rodata pockets** between 0x1100 and 0xA5000 —
unlike the virtual region, the engine .text is one continuous code blob
with TU-internal rodata small enough that splat's
`find_file_boundaries` already handles it at sub-page granularity.

Notable density features:
- Local minima at 0x004000 (4 prologues), 0x008000-0x00C000 (2-4), 0x01D000
  (2), 0x02A000-0x031000 (0-5), 0x03B000-0x036000 (3-5), 0x07A000-0x07F000
  (1-4), 0x09C000-0x0A0000 (mixed). These are likely TU `.rodata` /
  small-data tails inside individual `.o` files, not inter-file boundaries.
- 0x0A4000-0x0A5000 prologues collapse to 1-2 — this is the runup to
  the `simp object index` ASCII anchor at 0xA5FD8, confirming pass 3.

This investigation could not surface major TU boundaries beyond pass 3's
three coarse cuts (0x1100, 0x1F100, 0x438B0). Per-file granularity will
have to come from splat's `find_file_boundaries` output (the report
says it found ~140 sub-files) once the `make setup` artifact is
committed and inspectable. The current splat run in this worktree is
clean (`us/asm/1100.s`, `us/asm/1F100.s`, `us/asm/438B0.s` produced),
but splat's per-function file-edge log isn't dropped to disk for
inspection.

**Confidence**: low for any new sub-file cuts in this range. **No change
proposed** for the engine .text segmentation in pass 4.

### 4.6 Proposed unified diff (hand-off for next edit round)

```diff
--- versions/turok2.us.yaml.before
+++ versions/turok2.us.yaml.after
@@ -38,8 +38,10 @@
 #   0x107000 - 0x13B5F0 : SN64 LNK source-debug section (paths, line info)
 #   0x13B5F0 - 0x1E0000 : virtual / VM-hosted code (T3-equivalent `virtual`)
-#   0x1E0000 - 0x20F000 : post-virtual .rodata/.data tail
-#   0x20F000 - 0x1FF7AE4: compressed assets (textures, audio, levels)
-#   0x1FF7AE4- 0x2000000: 0xFF trailer (ROM padding)
+#   0x13B5F0 - 0x17A600 : virtual_text_0 (VM module 1 .text)
+#   0x17A600 - 0x186000 : virtual_rodata_0
+#   0x186000 - 0x1B3400 : virtual_text_1 (VM module 2 .text)
+#   0x1B3400 - 0x1BFA00 : virtual_rodata_1
+#   0x1BFA00 - 0x1DEE00 : virtual_text_2 (VM module 3 .text)
+#   0x1DEE00 - 0x20E334 : virtual_rodata_2 / .sdata
+#   0x20E334 - 0x20F178 : assets_index (913 × uint32 offset table)
+#   0x20F178 - 0x1FF7AEC: compressed assets (textures, audio, levels)
+#   0x1FF7AEC- 0x2000000: 0xFF trailer (ROM padding)

@@ libultra segment subsegments @@
   - name: libultra
     type: code
     start: 0xC3074
     vram: 0x80000500
     subsegments:
       - [0xC3074, asm, libaudio]
-      - [0xC8A60, asm, os_text]
+      - [0xC8A60, asm, os_text_audio_sched]
+      - [0xCB000, asm, os_text_io]
+      - [0xD1000, asm, os_text_gu]
+      - [0xD5000, asm, os_text_thread]
+      - [0xD8000, asm, os_text_msg_timer]
+      - [0xDC800, asm, os_text_exception]
+      - [0xDDE00, asm, os_text_tail]
       - [0xDEDE0, data, os_rodata]

@@ virtual segment @@
   - name: virtual
     type: code
     start: 0x13B5F0
     vram: 0x8013AAF0
     subsegments:
-      - [0x13B5F0, asm]
-      - [0x1E0000, data]
+      - [0x13B5F0, asm,  virtual_text_0]
+      - [0x17A600, data, virtual_rodata_0]
+      - [0x186000, asm,  virtual_text_1]
+      - [0x1B3400, data, virtual_rodata_1]
+      - [0x1BFA00, asm,  virtual_text_2]
+      - [0x1DEE00, data, virtual_rodata_2]

@@ assets / trailer @@
+  - name: assets_index
+    type: data
+    start: 0x20E334       # 913 × uint32 BE monotonic offset table
+
   - name: assets
     type: bin
-    start: 0x20F000
+    start: 0x20F178       # immediately after the 913-entry index

-  - name: trailer
-    type: bin
-    start: 0x1FF7AE4
+  - name: trailer
+    type: bin
+    start: 0x1FF7AEC      # last non-FF byte at 0x1FF7AEB
```

Rough diff size: ~50 lines added, ~10 removed.

### 4.7 Contradictions with passes 1-3

1. **Trailer start was wrong by 8 bytes.** Pass 3 said 0x1FF7AE4; truth
   is 0x1FF7AEC. The 8 bytes between are not 0xFF padding (they're
   `00 00 00 00 00 00 00 00`, a table terminator).
2. **Assets directory does NOT start at 0x20F000.** It begins 0xCCC
   bytes earlier at **0x20E334**, with 913 entries, ending at 0x20F178.
   The yaml's `assets` segment should start at 0x20F178, not 0x20F000.
3. **Virtual region has three internal text modules**, not one. Two
   sizable .rodata gaps (0x17A600-0x186000, 0x1B3400-0x1BFA00) separate
   them.
4. **Seeded `osCreateThread` / `osStartThread` VRAM addresses from
   `symbol_addrs.us.txt` are stale** (they map to ROM offsets *past*
   the end of T2's libultra `.text`, which is itself a strong
   confirmation that the seeds are T3's, not T2's). They should be
   stripped before they confuse the next m2c run.

### 4.8 Limitations of pass 4

- No per-function libultra naming. The COP0 / eret clustering gives
  *region* certainty but not *symbol* certainty.
- The 913-entry assets index points into compressed payloads with no
  visible magic. Decoding individual asset records requires reverse
  engineering Acclaim's compression scheme (out of scope for this
  segmentation pass).
- Splat's `find_file_boundaries` per-edge log was not dumped to a
  file by the current `make setup`, so the "~140 sub-files" claim
  could not be verified from this worktree.

## Pass 4 — Applied

The pass-4 unified diff was applied to `versions/turok2.us.yaml` in this
round. Concrete results:

### YAML changes (applied verbatim, except one fix-up)

- Header comment block updated with the new layout breakdown.
- `libultra` segment: single `os_text` subsegment replaced with the
  7 named asm cuts (`os_text_audio_sched`, `os_text_io`, `os_text_gu`,
  `os_text_thread`, `os_text_msg_timer`, `os_text_exception`,
  `os_text_tail`).
- `virtual` segment: single `[0x13B5F0, asm] / [0x1E0000, data]` pair
  replaced with 3 asm + 3 rodata-as-data subsegments
  (`virtual_text_0/1/2`, `virtual_rodata_0/1/2`).
- New `assets_index` segment (ROM 0x20E334, **type `bin`** — splat 0.40
  rejects standalone `data` segments without a vram; `bin` is the
  closest typed alternative that does not require a vram. The byte
  range is preserved; only the spimdisasm labelling differs).
- `assets` start moved from 0x20F000 → 0x20F178.
- `trailer` start moved from 0x1FF7AE4 → 0x1FF7AEC.

### Build pipeline status

- `make setup` (splat): clean run, 17 segments produced, **98.33 %
  defined coverage** (vs 98.63 % before — the small drop is because
  `virtual_rodata_*` is now typed `data` and `assets_index` is `bin`;
  no asm/rodata content moved into `bin` that wasn't already opaque).
- `make build`: **35 object files compile** (was 9 before this pass).
  Every new asm subsegment assembles cleanly through `mips-linux-gnu-as`.
- `make rom` (link): **still fails with 2 676 `undefined reference`
  errors over 1 820 distinct func symbols.** SHA1 not reached.

### Why the link still fails (and where pass-4 left it)

Distribution of the unresolved symbols by /64 KB VRAM prefix:

| Prefix    | Count |
|-----------|-------|
| 0x8028    | 279   |
| 0x8024    | 234   |
| 0x8029    | 223   |
| 0x8022    | 218   |
| 0x8027    | 202   |
| 0x8025    | 196   |
| 0x8023    | 175   |
| 0x8026    | 163   |
| 0x802A    | 83    |
| 0x8021    | 33    |
| 0x8020    | 8     |
| other     | 7     |

Every one of these unresolved targets lives in the **0x80210000 ..
0x802A50C4** vram range. Crucially, that range is **above** the highest
function splat actually defines (max `.globl` is `func_8020D274` in
`virtual_text_2.s`). With the current `virtual` vram base of
`0x8013AAF0` and delta `0x7FFFF500`, vram `0x802A0000` back-maps to ROM
`0x2A0B00` — deep inside the `assets` blob, which is not code.

This is strong evidence that the **three `virtual_text_*` modules are
overlay-style and do not share a single linear vram base**. Splat is
assigning them sequential vrams (continuing from 0x8013AAF0), but the
runtime VMASM dispatcher almost certainly **relocates** each module to
a different vram at load time. The `j func_802AXXXX` instructions in
`virtual_text_2` are real jumps within the module *as it would be
loaded at runtime*, but they reference an address space that doesn't
exist statically in the ROM.

Resolving this requires either:

1. Per-module overlay vram tagging in the YAML (splat's
   `subalign`/`vram` per subsegment) — needs the VM dispatch table at
   ROM ~0x13B5F0..0x13C000 decoded to learn each module's load vram.
2. Treating `virtual_text_1/2` as opaque `bin` until the dispatcher is
   reverse-engineered. That gets the link to pass but loses the
   disassembly of ~600 KB of game code.

Neither is a YAML-only tweak — both depend on first understanding the
VMASM relocation table. That's a separate investigation pass and is
handed off to the next round (probably Agent N or a dedicated
"virtual-overlay decode" sub-task).

### Surprises encountered

- Splat 0.40.3 requires `vram` for any segment typed `data`. The
  pass-4 diff specified `type: data` for `assets_index`; this had to be
  downgraded to `type: bin`. Conceptually identical for byte-exact
  rebuild; only the label/symbol granularity is reduced.
- The 2 668 → 2 676 unresolved-symbol count is virtually unchanged
  because the unresolved set was never *bounded* by the old
  single-`virtual` segment — they're all jumps into a vram range
  splat cannot reach with a linear ROM→VRAM mapping. Splitting
  `virtual` into three text modules did not change that root cause.
- Build now yields 35 objects (was 9), which confirms the new YAML
  drives the toolchain correctly end-to-end; only the link/relocation
  stage is blocked by the overlay-vram issue.

### Files committed in this pass

- `versions/turok2.us.yaml` (the unified diff, with `assets_index`
  type adjusted from `data` to `bin`).
- `docs/SEGMENTS.md` (this section appended).
