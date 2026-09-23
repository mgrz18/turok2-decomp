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

## Checking a function

    python3 tools/match_func.py path/to/file.c [-v] [--cc cc1 ...] [--opt -O2]

compiles the file in the `turok2-build` container (`tools/cc_func.sh`), looks
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
