# libultra matching — progress log

Living document for the libultra/libaudio/glu segment (ROM 0xC3000 - 0xDC000).

## TL;DR — current state

- Identified **13 high-confidence libultra functions** by COP0/cache prologue.
- **0** byte-exact matches committed yet — pipeline blockers documented below.
- `make verify` status: **still passing byte-exact** (asm-only). No regression — no
  C files are linked into the rom at this stage.

## Blockers (must address before C matching can start)

### 1. splat config `vram` is wrong for the libultra segment

`versions/turok2.us.yaml` declares the `libultra` segment with `vram: 0x80000500`,
same as `code` and `code_rodata_tail`. This causes:

- All four segments to be linked at the same virtual address, overlapping.
- Splat to emit `.word` instead of decoded MIPS for the entire libultra dump
  (only **4** `.globl func_*` in `us/asm/C3000.s` over **26 114** lines, i.e.
  splat could not detect function boundaries).
- The seeded LibTEngine `osCreateThread_800ddXXX` style names cannot land on
  matching addresses because the linker doesn't know where libultra lives in
  vram.

**Splat config diff request** (for the agent that owns `versions/turok2.us.yaml`):

```diff
   - name: libultra
     type: code
     start: 0xC3000
-    vram: 0x80000500
+    vram: 0x800C2400
     subsegments:
-      - [0xC3000, asm]
+      - [0xC3000, asm, libultra_text]
       - [0xDC000, data]
```

Also `code_rodata_tail` should have its own continuous vram
(0x800BFD50, since `.code` ends at ROM 0xA5FD8 = vram 0x800A53D8 and
the rodata pocket runs to 0xC0950 = 0x800BFD50… that needs another
pass).

Delta `0x7FFFF400` is documented in `docs/SEGMENTS.md` and matches
the `main`/`code` segment mapping (ROM 0x1000 → vram 0x80000400).

Once that's in, re-run `make setup` and splat should produce ~459 `.globl`
entries instead of 4.

### 2. seeded `symbol_addrs.us.txt` is mostly Turok 3, not Turok 2

`scripts/setup-from-references.sh` ingests `LibTEngine/functions.csv`.
The libultra-range entries there (`osCreateThread_800ddb30`, etc.) point at
addresses **above** T2's libultra segment ceiling (0x80019500 with the wrong
vram, ~0x800DBxxx with the corrected vram). They are T3 vram values and do
not apply.

The function-identification table below uses **T2-corrected vrams** derived from
direct disassembly of `build/scratch/libultra.bin`. Treat the LibTEngine seed
as "engine class methods only" — drop or override the libultra entries.

### 3. SN64 cc1 vs IDO cc1 codegen differs

The public libultra C source (`pmret/papermario`, `n64decomp/sm64`) was
written for **IDO 5.3**. Turok 2 was built with **SN64 cc1** (the
toolchain shipped under `tools/sn64/`). Trial compile of
`__osDisableInt` against SN64 cc1 -O2 produces 6 instructions, while
the real T2 binary contains 8 — register allocation, instruction
ordering, and the trailing-`nop` pattern all differ. Achieving byte-exact
will require:

- Likely a mix of inline asm and per-function `#pragma` for register hints.
- Possibly different `-O` flag per function (some libultra files were built
  `-O1` historically).
- An `asm-differ` workflow set up in `tools/asm-differ/` — currently missing
  on this branch.

## Identified functions (by direct disassembly)

All offsets are **inside `build/scratch/libultra.bin`** (= ROM offset minus 0xC3000).
Vram column uses the corrected delta `0x7FFFF400` (ROM + delta = vram).

### Cache / interrupt / SR cluster — `osInt.c`, `osCache.c`

| File off | ROM     | Vram      | Function              | Evidence |
|----------|---------|-----------|-----------------------|----------|
| 0x14BE0  | 0xD77E0 | 0x800D6BE0 | `__osGetSR`          | `mfc0 v0,$12; jr ra` |
| 0x14BF0  | 0xD77F0 | 0x800D6BF0 | `__osDisableInt`     | mfc0 $12, mask bit0, mtc0 |
| 0x14C10  | 0xD7810 | 0x800D6C10 | `__osRestoreInt`     | mfc0 $12, or a0, mtc0 |
| 0x14C2C  | 0xD782C | 0x800D6C2C | `osInvalDCache`      | `cache 0x15` + `cache 0x11`, 16B stride |
| 0x14CD0  | 0xD78D0 | 0x800D6CD0 | `osWritebackDCache`  | `cache 0x10` + `cache 0x00`, 32B stride |
| 0x14E10  | 0xD7A10 | 0x800D6E10 | `__osSetCompare`     | `mtc0 a0,$11; jr ra` |
| 0x14E20  | 0xD7A20 | 0x800D6E20 | `__osSetFpcCsr`      | `cfc1 v0; ctc1 a0; jr ra` |
| 0x14E30  | 0xD7A30 | 0x800D6E30 | `osSetIntMask`       | manipulates SR + AI/SI/MI mask at 0xa4300000 |
| 0x14EE0  | 0xD7AE0 | 0x800D6EE0 | `__osSetSR`          | `mtc0 a0,$12; jr ra` |
| 0x14EF0  | 0xD7AF0 | 0x800D6EF0 | `__osInitialize_TLB` | loops 30 entries, writes zero/TLB |
| 0x14F40  | 0xD7B40 | 0x800D6F40 | `osInvalICache`      | `cache 0x19`, 16B stride |
| 0x14FB0  | 0xD7BB0 | 0x800D6FB0 | `osWritebackDCacheAll`-like | 8K writeback, no args |

### TLB cluster — `osMapTLB.c`

| File off | ROM     | Vram      | Function          | Evidence |
|----------|---------|-----------|-------------------|----------|
| 0x5A60   | 0xC8A60 | 0x800C7E60 | `osMapTLB`       | `mtc0 a0,$0` (index), `mtc0 a1,$5` (pagemask), `mtc0 a2,$10` (entryhi), `tlbwi` |
| 0x5AC0   | 0xC8AC0 | 0x800C7EC0 | `osUnmapTLB`     | clears entry: `mtc0 zero,$2`, `mtc0 zero,$3`, `tlbwi` |
| 0x5AFC   | 0xC8AFC | 0x800C7EFC | `osGetTLB`       | reads back 30 TLB entries into a buffer via `tlbr` |

### Other identified

| File off | ROM     | Vram      | Function          | Evidence |
|----------|---------|-----------|-------------------|----------|
| 0x14D40  | 0xD7940 | 0x800D6D40 | `osVirtualToPhysical` (likely) | sets EntryHi, `tlbp`, reads BadVAddr |
| 0x136D8  | 0xC96D8 | 0x800C86D8 | `__osCleanupThread` or `__osDispatchThread` | the only `eret` in the segment |

## Functions identified but not yet matching

Zero. No matching attempts have been compiled+linked yet because of blockers
1 and 2 above.

## Functions still unidentified

The libultra segment has roughly **459 `jr ra`** returns. Of those, ~30 cluster
around the cache/interrupt/TLB code above. The remaining ~430 are mostly:

- audio/synth (alSyn*, alSeq*, alLink*) — needs string-table cross-reference to name
- glu math (`guMtxXFM*`, `guNormalize`, `gu*Lookat`, `guPerspective`) — large `mul.s`/`sub.s` blocks
- thread / message-queue / timer code (`osStartThread`, `osCreateMesgQueue`, `osSendMesg`, `osSetTimer`) — needs a function-call-graph pass

A practical next step is to dump the linker's source debug section
(ROM 0x107000 — `lnk_debug` segment) and match the file paths back to
function offsets. The SN64 LNK debug records preserve per-symbol source
file names; that pass would name most of the remaining libultra symbols
without needing to disassemble each.

## Per-function C stubs prepared

Stubs are staged under `src/us/libultra/` for reference but **not linked**.
They will only be wired into the build after blockers 1 + 2 are resolved
(so each removed `.s` chunk has a matching C replacement and the link
addresses line up).

- `src/us/libultra/__osDisableInt.c` — public-source-port; codegen does
  not yet match (8 ins vs 6 with SN64 -O2). Needs ordering pragma or
  switch to inline asm.

## Splat config diff request — copy block for the YAML owner

```yaml
# in versions/turok2.us.yaml, segment `libultra`:
  - name: libultra
    type: code
    start: 0xC3000
    vram: 0x800C2400          # was 0x80000500 — overlapped with `code`
    subsegments:
      - [0xC3000, asm]
      - [0xDC000, data]
```

After applying: `make nuke && make setup` will repopulate `us/asm/C3000.s`
with real instructions and per-function `.globl func_800C2400` entries.
That unblocks per-file extraction.

## What the next session should do

1. Get the YAML diff above merged (other-agent owned).
2. Re-run `make setup`; verify `us/asm/C3000.s` now decodes as MIPS.
3. Move the 13 identified function names from this doc into a symbol map
   that splat consumes (so the dump uses real names, not `func_XXXX`).
4. Slice `us/asm/C3000.s` into per-file subsegments (libultra `osInt.s`,
   `osCache.s`, `osMapTLB.s`) so each function can be replaced one at a
   time without losing byte-exact `make verify`.
5. Set up `tools/asm-differ` (clone from `references/turok3/tools/asm-differ`).
6. Iterate cc1 flags + inline-asm forms on `__osDisableInt` (smallest function
   in the set — best test bed) until diff is zero. That validates the pipeline.
7. Then run the same playbook through the rest of the cluster.
