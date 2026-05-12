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
- 5 short interrupt/SR/Compare functions identified and staged as C
  candidates under `src/us/libultra/`. **None linked, none byte-exact
  yet** — the build container is missing SN64 cc1 (blocker #4).
- `make verify` status: pre-existing failure (boot.c needs SN64 cc1).
  **No regression** from this work — the new C files live under
  `src/us/libultra/` which the Makefile wildcard `$(SRC_DIR)/*.c`
  does NOT recurse into.

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

- **Matched byte-exact**: 5 / 5 (Round 4 — all five candidates now
  produce byte-identical objects via SN64 cc1).
- **Close diff**: 0
- **Blocked**: 0

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

| Function           | Flags    | Status  | Notes |
|--------------------|----------|---------|-------|
| `__osGetSR`        | `-O2`    | MATCHED | cc1 picks `$v0` naturally (it's the return reg). Trivial. |
| `__osSetSR`        | `-O2`    | MATCHED | Inline `mtc0 %0,$12; nop` plus implicit `jr $ra` epilogue. |
| `__osSetCompare`   | `-O2`    | MATCHED | Inline `mtc0 %0,$11` only. No leading hazard nop. |
| `__osDisableInt`   | `-O2`    | MATCHED | Required register pinning: `register unsigned long ret __asm__("$2")` plus hard-coded `$8`/`$9`/`$1` in the asm body. Without pinning, cc1 picks `$3` for the read and emits `move $2,$0` in the `jr` delay slot, clobbering the result. |
| `__osRestoreInt`   | `-O2`    | MATCHED | Required hard-coded `$8` for SR temp + direct `$4` reference for the `mask` arg. cc1's default register alloc picks `$v0` and reorders. |

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

All under `src/us/libultra/`, all currently **NOT LINKED**:

- `__osDisableInt.c`
- `__osGetSR.c`
- `__osRestoreInt.c`
- `__osSetCompare.c`
- `__osSetSR.c`
