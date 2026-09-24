# Matching the engine

The goal of the decomp track is C that compiles to the exact bytes in the ROM.
This records what builds it, how to check a function, and what has been learned
about the code so far.

## The base: the ROM rebuilds byte-exact

`make verify` passes: the ROM built from the split asm has the original SHA1.
Every matched function is measured against that build, and anything committed
has to keep it passing. See the `build:` commit that fixed the three stacked
alignments (object SUBALIGN, segment `align`, and GAS section padding).

## The compiler

**`tools/sn64/cc1 -O2`**, pmret's build of SN64 GCC 2.8.1, with the flags
turok3 uses:

    -quiet -G0 -mips3 -O2 -mgas -meb -mcpu=VR4300 -mhard-float -mfp64

then `asn64` (SN's assembler, under wine) and `psyq-obj-parser` to get an ELF
object. `test/compiler_probe.c` holds eight engine functions picked at random
from `.code` that match through this pipeline. Among them are a float compare,
a byte-stream reader with a loop, an indexed struct array and member calls.

The SN `cc1n64*.exe` builds were not compared. Under wine inside the
emulated amd64 container, at least one of them hangs, and cc1 already matches.
Revisit if a function refuses to match for reasons no source change explains.

### C or C++

The original sources are `.CPP`. The LNK records name
`D:\turok2\n64\source\engine\PTIMER.CPP`, include `CPPU64.H` and
`STDAFX.H`, and declare `.ctors`/`.dtors` sections. The exported names are not
mangled (`DoUpdate`, `InitPTimers`), so at least those carry C linkage.

In practice C-style code matches with the C compiler: GCC 2.x shares one
backend between `cc1` and `cc1plus`. The member-call shape shows up already.
`func_0025FA54` receives `this` in `a0` and passes `a0`/`a1` through untouched
to `func_00243414(self, obj, 10)`, which is how a method calling another
method on the same object compiles. Genuinely C++ constructs (virtual calls
through a vtable, constructors registered in `.ctors`) are still to be seen.
If one cannot be expressed in C, `cc1plus` from the same SN release is needed,
and the repo does not have it.

`PTIMER.CPP` itself is not linked into the retail ROM. Its code is only in the
leftover LNK record, so it cannot be the first target the plan assumed.

## Linking matched C into the ROM

A matched function gets its own `c` subsegment in `versions/turok2.us.yaml`:

    python3 tools/c_subsegment.py code/func_0025FA54 func_0025FA54 --write
    make setup      # splat writes src/us/code/func_0025FA54.c with INCLUDE_ASM

then the INCLUDE_ASM line is replaced by the C, and `make verify` must still
pass. A file may hold a run of contiguous functions, some matched and the rest
INCLUDE_ASM. `tools/progress.py` counts the bytes defined in C.

Asm pulled in with INCLUDE_ASM uses numeric registers (`named_regs_for_c_funcs:
False`), since asn64 reads `$sp` as a hex literal. Splitting a file can leave
a branch aimed at a `.L` label in the neighbouring object;
`tools/function_seed.py --from-link-log build/link.log --write` defines those
absolutely, and the branch reassembles to the same word.

### Where the files begin and end

For now each matched function is its own file, named by address. The
original files cannot be recovered yet, and for `.text` it does not matter,
because the ROM's `.text` has no padding between objects. 2,852 of 2,879
functions in `.code` sit flush against the previous one, none ends in
alignment zeros, and the rate of 8-aligned starts (55%) is chance. Cutting at
any function start links byte-for-byte the same.

The boundaries start to matter when a file's `.rodata` moves into C. What is
known about that so far:

- **Jump tables are strictly in text order.** All 61 in `.code` sit in
  `0x800A51F8`-`0x800AAC60`, and their order never inverts against the
  functions that use them. They are the cleanest per-file signal available.
- **Float constants do not mark files.** GCC 2.x keeps one constant pool
  per function, not per file, so one file repeats the same float in several
  places. Treating repeats as file changes produced 364 contradictory
  constraints.
- **The block after `.code` is not all read-only.** Stores start at
  `0x800AB000`, and strings run almost to its end, so `.rodata` and `.data`
  are interleaved in some way that is still unmapped.

## Bulk matching

`tools/auto_match.py` drafts many functions with `m2c -t mips-gcc-c
--valid-syntax`, compiles them all in one container run, and keeps the ones
that come out identical:

    python3 tools/auto_match.py --max-size 300 --limit 5000           # report
    python3 tools/auto_match.py --max-size 300 --limit 5000 --accept  # wire in

then `make setup` and `make verify`. The first pass over every `.code`
function up to 300 bytes gave:

| size (bytes) | m2c drafts | match |
|---|---|---|
| up to 48 | 722 | 425 |
| 52-120 | 495 | 131 |
| 124-300 | 651 | 54 |

617 were accepted, taking the engine from 0.06% to 4.34%. The drafts keep
m2c's `M2C_FIELD(ptr, type *, offset)` for fields of structs not yet
recovered; they match because SN64 GCC compiles that pointer arithmetic to
the same load a struct member gives. Naming those structs is a separate
readability pass.

Skipped automatically:

- functions that read a jump table, since a `switch` makes the C emit its own
  `.rodata`, which still lives in asm;
- drafts m2c flags with `missing "jr $ra"`, which are fragments left by a
  wrong boundary, not whole functions;
- any object that carries data of its own.

**Check what a relocation resolves to, not only the rest of the word.** The
first bulk pass let one function through with `&D_800A5858 + 4`. m2c meant
four bytes, and C scaled it by `sizeof(s32)`. With the whole `%lo` field
masked, the words compared equal, and only the SHA1 caught it. The symbol
names encode their addresses, so `match_func.reloc_target_ok` now computes the
linked `%lo` and `jal` target and holds them against the ROM, and
`auto_match.py` rewrites the pattern as byte arithmetic.

## The fast assembler

`asn64` only runs under wine, which costs ~30 s a file in the emulated amd64
container and turns a full build into 20+ minutes. `tools/sn_as.sh` assembles
cc1's output with GNU as instead and reproduces the two things asn64 does
differently: `move` becomes `addu rd, rs, $0`, and only the c.cond -> bc1x
hazard gets a nop (cc1 marks every hazard with `#nop`; asn64 fills just that
one). All eight probe functions match either way, and a full build takes
seconds. `make ASSEMBLER=asn64` and `match_func.py --assembler asn64` keep
the original tool available as a cross-check, and `make verify` decides.

## Checking a function

    python3 tools/match_func.py path/to/file.c [-v] [--cc cc1 ...] [--opt -O2]

compiles the file in the `turok2-build` container (`tools/cc_func.sh`, GNU as
by default, `--assembler asn64` for the original), looks
each function it defines up by name in `build/turok2.us.elf`, and compares the
words with the ROM. Relocated fields (`jal` targets, `%hi`/`%lo`) are masked,
since the object is unlinked. `-v` prints both sides instruction by
instruction.

To get a first draft, extract the function's asm from `us/asm/*.s`, rename the
registers to ABI names (splat emits `$31`, m2c wants `$ra`), and run `m2c`.

## What the first functions taught

| Symptom in the diff | Cause |
|---|---|
| `lwc1`/`c.lt.s` where the draft had `lw`/`slt` | the field is `f32` |
| `lh`/`sh` where the ROM has `lw`/`sw` | the field is `s32`; the param was the `s16` |
| call passes `a0`/`a1` through unchanged | a method: add the caller's own args to the call |
| loads come out in the wrong order | flip the comparison: `o->v > G`, not `G < o->v` |
| an extra `andi 0xFF` after a byte load | the temp is `u32`, not `u8` |
| `addu` operands swapped on an array index | index the array directly (`p->slots[i].x`) |
