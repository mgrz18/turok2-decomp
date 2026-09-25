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

### Later passes (#43)

Three more passes took the engine from 4.34% to 6.27% (852 functions):

- **Inferred prototypes** (`tools/gen_prototypes.py`) gave the largest
  gain, 155 of 163 matches in one pass. m2c drafts a call from the registers
  the caller sets, and a C++ method calling another on the same object sets
  only `$a1`, leaving `this` in `$a0`. Knowing each callee's parameter count
  (argument registers read before written, stack slots above the frame for
  five and on), m2c passes `this` through.
- **Draft rewrites**: leading parameters m2c leaves out (`f(s32 arg3)`),
  stack locals it forgets to declare, `x << n` for its `x * 2^n` (GCC 2.8
  orders the following `addu` differently), and `&sym + n` as byte
  arithmetic.
- **-mfp64** (`tools/m2c_fp64.py`): odd float registers hold whole values in
  this engine, and stock m2c reads `$f1` as half of a double. The wrapper lets
  m2c draft every pending function, though that added few matches by itself.

What is left, by engine bytes: 53% of drafts are far off (over 25% of words
differ), 18% do not compile, 10% have no usable draft (often fragments of a
wrong boundary), 8% use `switch` (they need their file's `.rodata`), and 3%
are near misses. That is hand work, or `.rodata` migration.

**Delay slots are GCC's call, not the assembler's.** Letting gas fill delay
slots (`-O2`) matched one function and broke 87 that already matched. asn64
never moves a load into a slot, never fills a conditional branch's slot, and
leaves stores in place too. Where the ROM has a filled return slot and a
draft does not, GCC filled it (emitting the pair under `noreorder`), so the
fix is in the C.

**`li.d` is an asn64 macro.** cc1 emits `li.d $f0, 4.294967296e9`; asn64
puts the double in `.rodata` and loads it with `lui`/`ldc1`, which is what the
ROM has, while gas builds it in registers. `tools/sn_as.sh` now does what
asn64 does. Such a function carries its own `.rodata`, so it waits for the
file's rodata to move to C.

### Switch functions and their .rdata (#45)

A `switch` emits its jump table into .rdata, so a function with one needs its
slice of .rdata to come from C too. Each function's slice is what its code
references in `0x800A51F8`-`0x800AB000`, contiguous and in text order for 634
of 662 functions. The C file gets a second subsegment, `[rom, .rodata,
code/<name>]`, in the code_rodata segment (`auto_link_sections` keeps splat
from also listing it beside the .text). Slices can end on 4-byte boundaries,
since floats pack at 4 within a source file. `tools/fix_data_align.py` turns
the data asm's `.align` lines into `.org` at each label's exact offset and
switches off gas's auto-alignment of `.double`, so the asm that resumes after
a slice lands exactly.

`auto_match.py --switch` hands m2c the jump tables, checks the object's
.rodata against the ROM with the table relocations resolved, and wires both
subsegments.

**Float pools come from `li.s`.** cc1 writes float constants as
`li.s $f0, 6.0`, and asn64 expands that into a `.rodata` literal plus
lui/lwc1, which is what the ROM has, where gas builds the value in registers.
`tools/sn_as.sh` does what asn64 does, so a function's float pool comes from
its C literals in order of appearance. `auto_match.py --rdata` hands m2c the
pool values too, so it writes `75.0f` rather than `D_800A7948`. The first
pass matched 18 functions that way, with their slices.

**Splat cuts switches after their last `jr ra`.** A case placed after the
function's final return looks to splat like a new function. When a table
entry points past the end, splat also stops the table early and reads the
rest as data (jtbl_800A7C18 lost its fifth entry to a `.double`).
`versions/function_sizes.us.txt`, tracked and read by splat, pins those
functions' real extent.

**Some accepted C "functions" are fragments.** 81 of the functions the bulk
passes put in C are reached from another function by `j` or a branch. GCC 2.8
has no sibling calls, so these are tails of larger functions cut at false
boundaries: right bytes, wrong unit. Three have been folded back so far
(func_002682FC, func_00264160, func_00264168). The rest fold in when the
function they belong to is matched, and `auto_match` no longer accepts a jump
or branch target, or a jump-table target, as a function.

### Hand-matching patterns (#46)

Recurring fixes when a draft is a word or two off, in rough order of
frequency:

| Diff | Fix |
|---|---|
| call sets `$a1`/`$a2` and leaves `$a0` alone | a method: pass `this` (and any other passthrough arguments) first; the inferred prototype under-counted |
| `sw` where the ROM has `sb`/`sh` | the global is a byte/halfword array: `extern u8 D_X[]; D_X[i] = ...` |
| `addu` operands swapped on an index | write `(i << n)`, not `i * 2^n`; the tools do this for any operand now |
| `srav` where the ROM has `srlv` | the shifted value is unsigned (the callee returns `u32`) |
| store `$a3` to its home slot, read above the frame | a struct passed by value that starts in `$a3` |
| a loaded field lands in a different register | load it into a local first (`s32 v = x->c8; v += n; ...`) |
| statements in a different order | reorder them: GCC 2.8 keeps source order for independent stores |
| m2c's pointer offsets 4 or 8 times too far | m2c writes offsets in bytes; `byte_arith` rewrites them |

A float argument after an int one travels in an integer register (`lui $a1,
0x4160` is 14.0f). cc1 writes `li.s $5, 14.0` for it, SN's cc1n64 exactly the
same, and asn64 loads the float's bits into $5. `tools/sn_li.py` does that for
an integer destination and the .rodata literal for a float one; before, the
float case was forced on both and the argument came out in the wrong place.

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
