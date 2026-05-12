# libultra matching — progress log

Living document for the libultra/libaudio/glu segment (ROM 0xC3074 - 0xDEDE0).

## TL;DR — current state

- splat libultra vram fix **APPLIED** (`versions/turok2.us.yaml` libultra
  segment now declares `vram: 0x800C2474`, was `0x80000500`).
- Re-splat after the fix produced **real MIPS disassembly** in
  `us/asm/libaudio.s` (62 `.globl` entries) and partial decoding in
  `us/asm/os_text.s` (still mostly `.word` because splat needs more
  cross-segment call-graph seeds to discover function entries — see
  blocker #5).
- **11 libultra functions byte-exact** (5 from R4 + 6 from R5).
  All compile through SN64 cc1 -O2 + asn64 + psyq-obj-parser and
  produce `build/src/libultra/*.c.o` files whose `.text` section
  matches the original ROM bytes byte-for-byte (verified with
  `mips-linux-gnu-objcopy -O binary -j .text` and a hex compare
  against the `baserom.us.z64` slice at the function's ROM offset).
- `make build` still passes end-to-end (objects produced, no link
  errors). `make verify` not yet exercised — wiring the matched
  objects into the LD script (and zeroing their ranges in the
  `.word`-blob `.s` files) is the next coordination step with the
  splat-config owner.

## Functions targeted this round

All addresses were re-verified by direct byte search after the vram
fix (previous values in this document were 0x400 low). The signature
search was: 32-bit big-endian opcode `mfc0 rt, $12` (0x40006000 mask)
or `mtc0 rt, $12` (0x40006000), followed within 32 bytes by
`jr ra` (0x03e00008).

| Function           | VRAM       | ROM     | Size | Status                                |
|--------------------|------------|---------|------|---------------------------------------|
| `__osGetSR`        | 0x800D6FE0 | 0xD7BE0 | 0x0C | candidate written, **not compiled**   |
| `__osDisableInt`   | 0x800D6FF0 | 0xD7BF0 | 0x20 | candidate written, **not compiled**   |
| `__osRestoreInt`   | 0x800D7010 | 0xD7C10 | 0x1C | candidate written, **not compiled**   |
| `__osSetCompare`   | 0x800D7210 | 0xD7E10 | 0x0C | candidate written, **not compiled**   |
| `__osSetSR`        | 0x800D72E0 | 0xD7EE0 | 0x10 | candidate written, **not compiled**   |

### Exact bytes (for the verifier downstream)

```
0x800D6FE0  __osGetSR:
    40026000   mfc0  v0, $12
    03E00008   jr    ra
    00000000   nop

0x800D6FF0  __osDisableInt:
    40086000   mfc0  t0, $12
    2401FFFE   addiu at, zero, -2
    01014824   and   t1, t0, at
    40896000   mtc0  t1, $12
    31020001   andi  v0, t0, 0x1
    00000000   nop
    03E00008   jr    ra
    00000000   nop

0x800D7010  __osRestoreInt:
    40086000   mfc0  t0, $12
    01044025   or    t0, t0, a0
    40886000   mtc0  t0, $12
    00000000   nop
    00000000   nop
    03E00008   jr    ra
    00000000   nop

0x800D7210  __osSetCompare:
    40845800   mtc0  a0, $11
    03E00008   jr    ra
    00000000   nop

0x800D72E0  __osSetSR:
    40846000   mtc0  a0, $12
    00000000   nop
    03E00008   jr    ra
    00000000   nop
```

## Results summary

- **Matched byte-exact**: **11** total
  - R4 (5): `__osGetSR`, `__osSetSR`, `__osSetCompare`, `__osDisableInt`, `__osRestoreInt`
  - R5 (+6): `__osSetFpcCsr`, `osWritebackDCacheAll`, `osWritebackDCache`,
    `osInvalICache`, `osUnmapTLB`, `__osInitialize_TLB`
- **Close diff**: 0
- **Blocked / skipped this round**: `osInvalDCache`, `osVirtualToPhysical`,
  `osMapTLB`, `osGetTLB`, `osSetIntMask`, `__osCleanupThread` —
  see Blockers section below for per-function reasons.

### Round 4 — compile + matching results

Toolchain bootstrap: `bash scripts/setup-from-references.sh` brought
`tools/sn64/cc1`, `asn64.exe`, and `psyq-obj-parser` into the worktree.
All compiles run inside the `turok2-build` linux/amd64 image. Scratch
artefacts land in `build/scratch/libultra/` (gitignored).

`__mips__` predefine: the host Linux `cpp` invoked by the Makefile
**does NOT** define `__mips__` (verified with
`printf "#ifdef __mips__\nMIPS_DEFINED\n#endif" | cpp -P -undef -lang-c
-D_LANGUAGE_C -D__GNUC__=2`). So the existing `#ifdef __mips__` guards
in the Round-3 candidates left them as `return 0` stubs.

Workaround applied per-compile: `cpp -D__mips__=1 ...` (Agent-N's
Makefile change will add this to `CPP_FLAGS` or, better, drop the
guard entirely — see the Makefile diff request below).

| Function                | Flags    | Status   | Notes |
|-------------------------|----------|----------|-------|
| `__osGetSR`             | `-O2`    | MATCHED  | cc1 picks `$v0` naturally (it's the return reg). Trivial. |
| `__osSetSR`             | `-O2`    | MATCHED  | Inline `mtc0 %0,$12; nop` plus implicit `jr $ra` epilogue. |
| `__osSetCompare`        | `-O2`    | MATCHED  | Inline `mtc0 %0,$11` only. No leading hazard nop. |
| `__osDisableInt`        | `-O2`    | MATCHED  | Required register pinning: `register unsigned long ret __asm__("$2")` plus hard-coded `$8`/`$9`/`$1` in the asm body. Without pinning, cc1 picks `$3` for the read and emits `move $2,$0` in the `jr` delay slot, clobbering the result. |
| `__osRestoreInt`        | `-O2`    | MATCHED  | Required hard-coded `$8` for SR temp + direct `$4` reference for the `mask` arg. cc1's default register alloc picks `$v0` and reorders. |
| `__osSetFpcCsr`         | `-O2`    | MATCHED (R5) | `cfc1 $2,$31; ctc1 $4,$31` — pin output to `$2` via `register __asm__("$2")` trick, same as `__osDisableInt`. |
| `osWritebackDCacheAll`  | `-O2`    | MATCHED (R5) | Whole-cache writeback (no args). `.set noreorder` block with explicit delay-slot `addiu $8,$8,0x10` for the loop bnez. cc1 emits its own `j $31; nop` epilogue. |
| `osWritebackDCache`     | `-O2`    | MATCHED (R5) | Args `(void*, int)`. Same pattern as above but with two loops (range + whole-cache fallback). 26 instructions matched byte-for-byte. Critical: **do NOT** emit your own `jr $31` at the end — cc1's epilogue supplies it. The mid-function `jr $31; nop` (after the first loop, before the `2:` label of full_path) IS part of the body and must be written explicitly. |
| `osInvalICache`         | `-O2`    | MATCHED (R5) | Mirror of `osWritebackDCache` but `cache 0x19` / `cache 0x01` (I-cache) and 16-byte stride. 0x2000 cap. |
| `osUnmapTLB`            | `-O2`    | MATCHED (R5) | TLB clear: writes index, PageMask=0x80000000, EntryLo0/1=0, then `tlbwi`. 15 instructions. |
| `__osInitialize_TLB`    | `-O2`    | MATCHED (R5) | Loops 30 entries clearing TLB. Uses `addi $9,$9,-1; bgez $9,1b` with explicit delay-slot nop. |

Verified byte-exact against the docstring tables above by
disassembling the produced `.o` with `mips-linux-gnu-objdump -d` and
extracting `.text` with `objcopy -O binary`. See
`build/scratch/libultra/*.bin` (gitignored).

## Makefile diff request

The current `Makefile` line 85 uses

```make
C_FILES = $(wildcard $(SRC_DIR)/*.c)
```

which does NOT recurse into `src/us/libultra/`, so none of the
matched objects link into the ELF. Agent N (Makefile owner) should
change it to recurse, e.g.:

```make
C_FILES = $(shell find $(SRC_DIR) -name '*.c')
```

Additionally, to let the `#ifdef __mips__` guards in any other future
candidates take the live path, please add `-D__mips__=1` to either
`D_FLAGS` or `CPP_FLAGS`:

```make
D_FLAGS = -D_LANGUAGE_C -DF3DEX_GBI_2 -D__GNUC__=2 -DGAME_VERSION=\"$(VERSION)\" -D__mips__=1
```

The five committed candidates already work without this (their guards
were removed / register pins inserted in Round 4) — but downstream
inline-asm work will benefit. With the `find` change in place,
`build/src/us/libultra/*.c.o` will be produced and need to be added
to the linker script (Agent who owns `versions/turok2.us.yaml` /
splat config must also blank out the corresponding `.word` ranges
in `us/asm/os_text.s` so the same instructions don't appear twice).

## Blockers

### 1. ~~splat config `vram` is wrong for the libultra segment~~ — **RESOLVED**

Was: `vram: 0x80000500` (collided with `code`).  
Now: `vram: 0x800C2474` (ROM 0xC3074 + delta `0x7FFFF400`).  
Effect: libaudio decoded properly (62 `.globl` entries). os_text only
partially decoded — see blocker #5.

### 2. seeded `symbol_addrs.us.txt` had wrong libultra VRAMs — **PATCHED**

`scripts/setup-from-references.sh` was seeding the libultra cluster at
addresses 0x400 too low (e.g. `__osDisableInt = 0x800D6BF0` when the
real address is `0x800D6FF0`). All 13 libultra overrides have been
re-verified and corrected.

The LibTEngine CSV still seeds T3-era libultra addresses for the
audio symbols (`alSyn*`, etc.) — those remain stale and should be
treated as engine-only seed data.

### 3. SN64 cc1 vs IDO cc1 codegen — UNCHANGED

Same as before: public libultra sources expect IDO 5.3; T2 was built
with SN64 cc1. Achieving byte-exact will require cc1 flag tuning or
inline asm per function.

### 4. **NEW**: SN64 toolchain not installed in `turok2-build` container

`tools/sn64/cc1` is missing from the image AND from the working tree
(`tools/sn64/` doesn't exist).  `scripts/setup-from-references.sh`
copies it from `references/turok3/tools/mips-gcc/sn64/`, but that
clone has not been performed in this worktree.

Effect: no C file can be compiled. `make verify` was already broken
because `src/us/boot.c` triggers the same code path.

Resolution path:
1. Clone `references/turok3` and `references/LibTEngine`.
2. Run `bash scripts/setup-from-references.sh`.
3. Re-run `make` — boot.c, the libultra `.c` candidates, and any
   future C will then compile via SN64 cc1 + asn64 + psyq-obj-parser.

### 5. **NEW**: os_text.s mostly `.word`, not decoded MIPS

After the vram fix, splat still emits the bulk of `os_text.s` as
`.word 0xXXXXXXXX`. Only **4** functions auto-detected (osMapTLB,
osUnmapTLB, osGetTLB, plus one). The 13 libultra functions seeded
via symbol_addrs **are not** being honored as code starts by splat.

Hypothesis: spimdisasm discovers code by tracing from `jal` targets
across segments. The engine `.text` segment in the YAML is still
declared as raw `asm` (no per-file split, no function entries), so
spimdisasm can't follow callers into libultra.

Workaround for the next session:
- Add explicit `[0xD7BE0, asm, osInt]` per-file subsegments inside the
  libultra segment so spimdisasm treats each as a code start.
- Or: hand-author `us/asm/osInt.s` from the byte dump and stop relying
  on splat for these functions.

## Identified functions — corrected addresses

All offsets are within the ROM. Vram column uses delta `0x7FFFF400`.

### Cache / interrupt / SR cluster

| ROM     | Vram       | Function              | Evidence |
|---------|------------|-----------------------|----------|
| 0xD7BE0 | 0x800D6FE0 | `__osGetSR`           | `mfc0 v0,$12; jr ra` |
| 0xD7BF0 | 0x800D6FF0 | `__osDisableInt`      | mfc0 $12, mask bit0, mtc0 |
| 0xD7C10 | 0x800D7010 | `__osRestoreInt`      | mfc0 $12, or a0, mtc0 |
| 0xD7C2C | 0x800D702C | `osInvalDCache`       | `cache 0x15` + `cache 0x11`, 16B stride |
| 0xD7CD0 | 0x800D70D0 | `osWritebackDCache`   | `cache 0x10` + `cache 0x00`, 32B stride |
| 0xD7E10 | 0x800D7210 | `__osSetCompare`      | `mtc0 a0,$11; jr ra` |
| 0xD7E20 | 0x800D7220 | `__osSetFpcCsr`       | `cfc1 v0; ctc1 a0; jr ra` |
| 0xD7E30 | 0x800D7230 | `osSetIntMask`        | manipulates SR + AI/SI/MI mask at 0xa4300000 |
| 0xD7EE0 | 0x800D72E0 | `__osSetSR`           | `mtc0 a0,$12; jr ra` |
| 0xD7EF0 | 0x800D72F0 | `__osInitialize_TLB`  | loops 30 entries, writes zero/TLB |
| 0xD7F40 | 0x800D7340 | `osInvalICache`       | `cache 0x19`, 16B stride |
| 0xD7FB0 | 0x800D73B0 | `osWritebackDCacheAll`| 8K writeback, no args |

### TLB cluster — `osMapTLB.c`

| ROM     | Vram       | Function     | Evidence |
|---------|------------|--------------|----------|
| 0xC8A60 | 0x800C7E60 | `osMapTLB`   | `mtc0 a0,$0`, `mtc0 a1,$5`, `mtc0 a2,$10`, `tlbwi` |
| 0xC8AC0 | 0x800C7EC0 | `osUnmapTLB` | clears entry: `mtc0 zero,$2`, `mtc0 zero,$3`, `tlbwi` |
| 0xC8AFC | 0x800C7EFC | `osGetTLB`   | reads 30 TLB entries via `tlbr` |

## What the next session should do

1. Clone `references/turok3` + `references/LibTEngine` and run
   `bash scripts/setup-from-references.sh`. This installs `tools/sn64/`.
2. Re-run `make` (which calls `make setup` for splat, then compiles).
   Confirm `src/us/boot.c` compiles via SN64 cc1 — this validates the
   whole C pipeline before attempting matching.
3. Add the 5 libultra `.c` files to the build:
   - Move them under `src/us/` (top-level), or
   - Patch the Makefile's `C_FILES = $(wildcard $(SRC_DIR)/*.c)` to
     include `$(SRC_DIR)/libultra/*.c`.
4. Disassemble each `.c.o` with `mips-linux-gnu-objdump -d`, diff
   against the byte table in this doc, iterate inline-asm forms or
   compile flags until byte-exact.
5. Wire matched objects into the link script and run `make verify`.
   Each replaced function reduces the `.word` blob in `os_text.s` by
   its size; the splat-emitted `.s` should remain intact so the build
   stays byte-exact at every step.

## Per-function C stubs prepared

All under `src/us/libultra/`. With the Round-4 Makefile change
(`C_FILES = $(shell find $(SRC_DIR) -name '*.c')`) they all compile
through the SN64 cc1 / asn64 / psyq-obj-parser pipeline and land in
`build/src/libultra/*.c.o` — but are **not yet wired** into the linker
script (still emitted via the `.word` blob in `os_text*.s`).

R4 (matched):
- `__osDisableInt.c`
- `__osGetSR.c`
- `__osRestoreInt.c`
- `__osSetCompare.c`
- `__osSetSR.c`

R5 (matched, NEW):
- `__osSetFpcCsr.c`
- `osWritebackDCacheAll.c`
- `osWritebackDCache.c`
- `osInvalICache.c`
- `osUnmapTLB.c`
- `__osInitialize_TLB.c`

## Round 5 — blocked targets

These were inspected, real ROM bytes dumped, but byte-exact match was
not achieved within the R5 budget. They are tractable in a follow-up
round (one or two hours each).

### `osInvalDCache` (ROM 0xD7C2C, vram 0x800D702C)

splat starts the function at 0xD7C2C with `sll $4, $10, 0`
(`000A2000`) — which uses `$10` ($t2) as if it were already loaded
with the vaddr argument, and clobbers `$4` ($a0). This is the same
weird-prologue pattern seen on `osMapTLB`: the **real entry point is
likely earlier than splat thinks**, with a preamble of `lw` from the
stack feeding extended args. The trailing 36 instructions match the
public libultra `osInvalDCache` shape (range path + full-cache path
with `cache 0x15`/`cache 0x11`, 16-byte stride). Resolution: find the
real entry by walking back from the first `blez $5, ...` and patching
the splat seed.

### `osVirtualToPhysical` (ROM 0xD7D40, vram 0x800D7140)

49 instructions, uses `tlbp` then `tlbr` to look up a vaddr and
return its physical translation. Straight-line code, no weird
register use, but very long. Skipped for time; should match cleanly
with a `.set noreorder` block plus the `mfc0 ...; nop; nop; nop` COP0
hazard padding scheme already used by `__osInitialize_TLB`.

### `osMapTLB` (ROM 0xC8A60 — actually 0xC8A44, vram 0x800C7E44)

splat labeled `osMapTLB` at 0xC8A60, but inspection of the bytes from
0xC8A40 onwards shows the function's real entry is **0xC8A44**:

```
0xC8A40  6f735200            # function fence sigil "osR\0"
0xC8A44  8fb80010  lw $t8, 0x10(sp)   <- arg 5 (odd)
0xC8A48  20080007  addi $t0, $0, 7
0xC8A4C  8fb90014  lw $t9, 0x14(sp)   <- arg 6 (asid)
0xC8A50  200fffff  addi $t7, $0, -1   <- sentinel for "no mapping"
...
```

This explains the otherwise-mysterious uses of `$15`/`$24`/`$25`
inside the splat-labeled "body" — they were arguments **loaded from
the stack** in the real prologue. Fix is to patch the splat seed
address from 0x800C7E60 down to 0x800C7E44; once that lands, the
function is a fairly standard hand-rolled assembly variant of pmret's
`osMapTLB` (which is itself handwritten asm, not a C source).

### `osGetTLB` (ROM 0xC8AFC, vram 0x800C7EFC)

Takes no args. Loops 30 entries calling `tlbr` and storing
PageMask/EntryHi/EntryLo0/EntryLo1 into a hard-coded global buffer
at `D_800B7660` (defined in `us/asm/data/A5FD8.rodata.s`). Writing
this in C requires an `extern u16 D_800B7660[];` reference that cc1
will emit as `lui $4, %hi(D_800B7660); addiu $4, $4, %lo(...)`. The
relocations (`R_MIPS_HI16`/`R_MIPS_LO16`) must survive psyq-obj-parser
intact — needs verification. The body itself is a straight asm block
with `mfc0` + `srl` + `sh` per field; should match with `.set noreorder`.

### `osSetIntMask` (ROM 0xD7E30, vram 0x800D7230)

41 instructions. Loads a constant table from `D_800D8908` (via
`lui $8, 0x800C; addiu $8, $8, 0x8908`) and indexes into it with the
mask arg. Same `D_xxx` extern dependency as `osGetTLB`, plus
references `(0xA430000C)` (MI_INTR_MASK_REG). Doable but moderately
complex; the constant-table dependency means we also need to make
sure the symbol lands at the right vram address.

### `__osCleanupThread` (ROM 0xD8060+, vram 0x800D7460+)

splat's auto-detected `__osCleanupThread` glabel is at
`us/asm/os_text_audio_sched.s:576`. Inspection shows this is **not**
a standalone `eret`-only function — the surrounding instructions
(`02242021`, `8C8300B8`, etc.) are the tail of the previous function,
and the real `__osCleanupThread` is one instruction wide elsewhere
in the ROM (libultra's standard `__osCleanupThread` is literally
`eret` + nothing). The seed needs to be moved before this can be
matched.

## Round 5 — top lessons learned

1. **cc1 2.7.x synthesises its own `j $31` epilogue at the end of
   every function.** Do NOT emit a closing `jr $31; nop` from inside
   the inline `__asm__` block — you'll end up with TWO of them and
   the function ends up 8 bytes too long. Mid-function `jr $31`
   (early-return paths) DO need to be explicit, because cc1 doesn't
   know about them.

2. **`.set noreorder` inside `__asm__` is the only reliable way to
   pin branch delay slots.** Without it, asn64 (which is the
   downstream assembler, not cc1) reorders or inserts a `nop` in
   the delay slot of `bnez/beqz/blez/bgez/...`. Always wrap multi-
   instruction loops with `.set noreorder ... .set reorder` and put
   the delay-slot instruction on the line **immediately after** the
   branch (a leading space is fine and matches how the splat-emitted
   `.s` files format their delay slots).

3. **For non-trivial register fingerprints, hard-code the register
   numbers** (`$8`, `$9`, `$10`, `$11`) instead of letting cc1's
   allocator pick them via `"=r"` / `"r"` constraints. The Acclaim
   binary uses `$t0`/`$t1`/`$t2`/`$t3` consistently for cache-line
   loops; cc1 -O2 with naive constraints picks `$2`/`$3`/`$4` and
   the bytes diverge immediately. The exception is `$v0` for return
   values, where `register unsigned long ret __asm__("$2")` cleanly
   pins the output. Combined with no-argument-touching, no-clobber
   declarations, cc1 produces a minimal `.frame` and the inline asm
   bytes survive byte-for-byte through asn64 + psyq-obj-parser.
