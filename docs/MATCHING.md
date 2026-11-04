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

**Stray words before an object, and other wrong cuts (`tools/fix_cut.py`).**
SN's linker starts each object's `.text` on a 16-byte boundary and fills the
gap with whatever its buffer held, not zeros: one to three words that are
often a copy of instructions from elsewhere (the three before `__umoddi3`
repeat code from 0x26BBD4). splat reads them as the start of the next
function, so its `func_X` begins a few words early and the real function
starts at the next 16-aligned address, where callers' `jal`, data pointers
and thread entries point (`D_002972E0` was alSavePull's real start). A junk
word can also branch into the function after it, which made `accept_manual`
see a false boundary at osJamMesg, osRecvMesg and osSendMesg, or stand as a
little "function" of its own. A run of junk words therefore marks an
original object boundary, which helps recover the source files.

`versions/cuts.us.txt` lists the fixes and `tools/fix_cut.py --write` applies
them (idempotent; then `make setup`, `make verify`):

- `stray <from> <to>`: the words are kept as `src/us/asm/stray_<from>.s`,
  `.word` by `.word` from the ROM, wired in as a `hasm` subsegment. Its symbol
  (`func_<from>`, which C may still name) is an object, so no tool counts it
  as code and N64Recomp does not translate it; `<to>` gets a function symbol.
- `func <start> <size>`: a function's true extent. One line per start splits
  a cut that holds several functions (func_0028E7A4 held three), trims junk
  words at an end (func_0029D690), or joins a fragment back, whose C then
  goes back to asm (func_0026BBB8, `return 0`, was the exit of
  func_0026BB9C's loop, and that loop had opened an asm file, making the four
  functions after it labels inside one asm function).

The entries live in a block of `versions/function_sizes.us.txt` the tool
owns, and `symbols_from_scan` loses the ones they override. Two `asm`
subsegments left in a row (from C taken out earlier) are folded into one,
since splat cuts a function at a file boundary: that alone made
func_0041BC50 and func_00454BB0 whole. `fix_cut.py --find` lists candidate
`stray` lines from a build (a function not 16-aligned whose next 16-aligned
address is reached or opens with a prologue, with nothing reaching the words
before); each wants a look before it goes in the list.

The list holds 250 such runs so far: the ones found by hand while matching,
then every `--find` candidate in the engine, libultra and the VM modules'
first copy whose words are plainly not a function. Left alone: eleven runs
already matched as C "functions" of their own (`jr ra; nop` and the like,
the same bytes either way), twelve that are pieces of strings (splat named
them `D_`, and `rejected_boundaries` holds them), and three in the VM
modules whose old start the second copy names. A stub can split an asm file
between a branch and its label: the label then goes in
`undefined_funcs_extra`, as for any cross-file branch (`.L0041A490`).

Matched C for the real function goes in under the real name, one function a
file (`integrate.py` wires a file by its name). A matched caller that named
a real start as data (`extern u8 D_002972E0[]`) has to name the function
instead once the symbol is a function.

Two kinds of exclusion in `build/integrate/excluded.txt` were tool bugs,
fixed along the way. A named function is in `extract_asm`'s bodies twice
(osJamMesg and func_0029B5B0), and `entered_from_outside` took its own
branches for another function's, so every named function with a branch
looked "entered from outside". And `rodata_slice` started a slice only at
the lowest address the code reads, so C that leaves a table in asm through an
`extern` (cosf's polynomial) and owns just the literals after it never
matched; it now tries each address the code reads.

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
| `lbu $v0, D_X($a0)` in the ROM, a register-built address in the draft | index the global as an array, `extern u8 D_X[]; D_X[i]` (`global_arrays` variant) |
| a qsort comparator's loads in the wrong order | write the comparison `a > b`, not `b < a` |
| ROM keeps a global's address in a register (`lui/addiu $a0`, then `0($a0)`) | the global is an array element or struct field: `extern T D[]; D[0]` (`scalars_as_arrays` variant) |
| a load scheduled earlier than the draft's | read it into a local before the statements it precedes |

A float argument after an int one travels in an integer register (`lui $a1,
0x4160` is 14.0f). cc1 writes `li.s $5, 14.0` for it, SN's cc1n64 exactly the
same, and asn64 loads the float's bits into $5. `tools/sn_li.py` does that for
an integer destination and the .rodata literal for a float one; before, the
float case was forced on both and the argument came out in the wrong place.

### Register allocation and loops

When a draft's only difference is which pseudo gets which register, the
cause is usually how GCC 2.8 ranks them, and the ranking can be read and
steered:

- **References are weighted by loop depth.** local-alloc and global.c
  order pseudos by their use counts, each scaled by the depth of the loop it
  sits in. An empty `do { } while (0)` opens a loop as far as that count goes
  (cc1 keeps its loop notes), so wrapping a few statements in one raises the
  priority of what they use. It is how func_00220D00 and func_00213A70 get
  the ROM's `$s` order, and most likely what the original's macro bodies
  were. `cc1 -dg -dl` writes the `.greg` and `.lreg` dumps with each pseudo's
  refs, priority and the register it got.
- **A local used in more than one block goes to the global allocator.** One
  function-wide temporary that several blocks share is allocated by global.c,
  with its whole-function priority, where a temporary declared in each block
  stays a local-alloc pseudo. Which of the two the source had shows in the
  registers: func_0025CAC8's level and delay are shared by its three
  reactions; func_0026AD94 needs its own temporary in a block, since a
  function-wide one costs a register copy.
- **Loop invariants are hoisted by a count.** loop.c moves an invariant out
  of a loop only when threshold x savings x lifetime reaches the loop's insn
  count (threshold is 2 x (1 + the non-fixed registers), halved when the
  loop calls a function). So the size of the loop body decides what is
  hoisted: a constant kept in `$s1` across a loop in the draft but
  rematerialised in the ROM means the original loop had more insns, and a
  dead store can keep a constant out (func_0024D344's `ok`).
- **A redundant duplicated block can be original.** Rage Wars' sources carry
  blocks repeated under both arms of a test, which cross-jumping merges into
  one in the output. They leave no trace in the code but still count insns
  and references, for the hoisting above and the priorities. The engine
  thread (func_00286674) matches only with such a duplicate in case 4;
  func_0026B4A4 and func_0021D39C come close with one.

### Globals and branches (the VM pass)

Five more from matching the VM modules by hand:

- **Globals read inside a `static __inline__` helper are addressed by
  symbol** at each use, which reproduces the ROM's `lui`+`lw` reloads where
  the same reads written in the caller share one base register.
- **`State *s = &GLOBAL;` at the top stops GCC folding `sym+off`**: the
  fields are then reached from one base register, as the ROM has them,
  instead of each through its own `%hi`/`%lo`.
- **`x.cnt++;` as its own statement, then reading the field again,** gives
  the ROM's store and reload of the counter.
- **`if (i != -1) { ... } else { ... }`** gave the ROM's `beql` where the
  test written the other way round did not.
- **A pool word one ulp off is the literal's spelling.** func_00406598 has
  0x3C27C5AD where `0.01024f` gives 0x3C27C5AC; `0.0102400007f` rounds to
  the ROM's bits. A function excluded because "its .rodata is not in the ROM
  where the code reads it" may be only that: compare the object's .rodata
  with the ROM before suspecting the placement.

The second VM module is linked 0x39000 below the addresses splat names it at
(docs/VMASM.md), so its calls and jumps name the first module's addresses.
When a cut fix or C in the first module stops defining one of them (it was a
function start or a label there), the integrate step reports it as called or
held; define the address absolutely in `versions/undefined_funcs_extra.us.txt`.

The second module was also linked against the first module's .rodata: its
copies read the originals' literals, strings and jump tables at their
addresses. C for a second-module function is written with its own literals
and switches, and `versions/shared_rodata.us.txt` places its .rodata NOLOAD
over the first module's; `make verify` checks the two agree, jump-table
entries included (docs/VMASM.md). A function excluded because its .rodata
"has no place in the build" or "is not in the ROM where the code reads it"
in the second module usually goes in that way. When the yaml gave a pool
word to the copy, give it to the original and put the copy over it.

### The X pass

From the sixth hand-matching batch:

- **Gotos out of a test give the ROM's `bnez`/`bnel` + `j` layout** where
  the ROM jumps from a row test to a body placed later (func_00424978).
- **An inline helper keeps GCC from strength-reducing** `&s->a[i]` across
  per-element calls (func_0042F344), and re-addresses a global by symbol
  at each call (func_00424A60), as the VM pass found.
- **Store order follows the declared shape**: a global written as an array
  (`D_8012FA04[k] = ...`) kept the order that leaves `li a2, 0x13` in the
  jal's delay slot (func_00405AC8); one scratch pointer reused for a call's
  result and an array kept a load in `$v0` (func_0042AC0C).
- **A loop written out in the function, not as an inline,** let two loops
  share one counter register (func_0042EF2C).
- **FAKEMATCH, accepted where the permuter or a register pin is the only
  way found**: a lookup written twice under an always-true test that GCC
  merges but whose doubled references move the `$s` registers
  (func_0040457C), and `register T *p asm("$2")` pins standing for a copy
  the ROM makes (func_0041EA84, func_004579A4). Each says so in its first
  comment.
- **Matching alone is not the whole check for a wrong cut**: a function
  that matches "over its real extent" still needs that extent in
  `versions/cuts.us.txt` (and, at a segment's end, the next subsegment
  moved: func_00469374 ran 0x1C bytes into what the yaml called
  virtual_rodata_1). match_func also masks the targets of local `j`s, so
  a `j` that lands on the wrong block (func_0026C148's first draft, where
  GCC did not thread an inline's NULL return to the epilogue) shows only at
  `make verify`.

### One file with its neighbour

A function that matches alone can still be cut wrong: if its `.rodata`
starts 4 past an 8-byte boundary and holds a double or a jump table, its
object (8-aligned in `.rodata`, linked with SUBALIGN 4) puts those 4 bytes
off. Its pool continues the previous function's, so the two are one file,
and the file starts at the nearest pool that opens 8-aligned; every pool
between is 4 past a boundary for the same reason (func_0024C47C to
func_0024D344). A function of the file that does not match yet stays in it
as INCLUDE_ASM, with its pool in `src/us/asm/rodata/` through
INCLUDE_RODATA, ahead of the pools the C emits (func_0022D2FC until the F pass). Merged
parts written apart need one view of the globals they share: reading one
through a cast (`*(T *) &D_X`) instead of its declared type changed
func_0022F350's register allocation. Two more signs of one file: a string
literal two functions name at the same address (GCC shares a string only
within a file), and a non-static inline function, whose own copy GCC 2.8
emits at the end of the file (func_00219A20 and func_00219ACC, the out-of-line
copies of what func_00218DCC and func_00218B10 inline).

### The E pass (the last engine functions)

- **One float variable through a lerp chain.** `level = ai->intensity;
  level *= (1.0f / 255); level = level * -45.0f + 105.0f;`, reusing one
  `f32` for every step, gives the ROM's FP registers where a temporary per
  step does not (func_0025F29C, func_0025F9FC, func_0025FCB8). func_0025F928
  wants the opposite, 240.0f passed through two variables, for its
  alternating `$f0`/`$f1`; func_0025F2F8 and func_0025FBB8 are still a
  register off in the final add.
- **Copy Rage Wars' redundant re-reads.** func_0024B88C tests
  `pad->stickX != 0` and returns 5 either way, as Rage Wars' source does;
  the dead test shapes the branch layout. When a reference decomp has such
  a read, keep it (func_00246BC0's doubled load of `controlMode` looks like
  the same thing).
- **libgcc.** `__divdi3`, `__moddi3`, `__udivdi3` and `__umoddi3` are GCC
  2.8.1's libgcc2.c units with longlong.h's MIPS `umul_ppmm`/`udiv_qrnnd`,
  built `-mgp32 -mfp32 -msplit-addresses` (`__floatdisf` without the last),
  given in each file's `/* cc1-flags: */` line. Each unit is its own object
  with its own static copy of `__clz_tab`, 256 bytes of .rodata at
  0x800AAC90, 0x800AAD90, 0x800AAE90 and 0x800AAF90. `__floatdisf`'s
  doubles follow at 0x800AB090, past what `auto_match` takes for the
  engine's .rdata, so its .rodata subsegment was wired by hand.
- **`case 2: default:` first.** COnScreen__DrawText switches on the
  language twice; written with the shared `case 2: default:` as the first
  label pair, the switch comes out in the ROM's test order and layout.
- **A pool read through externs may own less than the ROM's slice.**
  func_00228C40 read its three 1/85 weights as `D_800A60D0`..`D8`, so its
  object held only the three 2^31 of its `(u32)` conversions; as literals the
  object's pool is the whole 0x800A60D0-0x800A60E8. A misspelt `D_` is
  worse: `integrate.py` defines an undefined `D_X` at the address its name
  gives, so `D_800BB0D0` for `D_800AB0D0` (func_002150C0) linked and only
  `romdiff` caught it.
- **Wrong cuts at hand-written asm.** splat's func_002010C0 was the RNC
  decompressor's asm, a string's tail and a 0x24-byte C function;
  func_0029C8E0 was `__osDispatchThread`, `__osCleanupThread`, fill and the C
  `__osLeoInterrupt`. The asm keeps its `func` extent in
  `versions/cuts.us.txt`, and the C after it goes in under its own start.
  The fill at the end of `.code` (after `__floatdisf`) is a stray that runs to
  the segment's end, which `fix_cut.py` now accepts.

### The N pass

Twenty-one close drafts from `nonmatching/` finished by hand:

- **Where a variable lives decides its pseudo.** Declared inside each loop's
  block, a variable is a separate pseudo per loop (func_0021375C); one
  declared for the whole function lives longer, which keeps CSE from
  merging it with a nearby load (func_00226490: one actor variable for both
  loops keeps the kind-test load in `$a0` apart from it in `$s0`). Reusing a
  variable as a later loop's index can be the ROM's shape too
  (func_004004D8, the automap: `nVtx`).
- **`goto next;` with the increment at the end of the body**, in place of
  `continue`, lets reorg fill a delay slot from the loop test's target
  (func_004004D8).
- **Copy a matched sibling's exact statement shape.** func_0025F2F8 and
  func_0025FBB8, a register off in the E pass, match with their siblings'
  `level = x; level *= 1/255.f; level = level*K + B;`.
- **`err = r;` before a second identical test** stops jump threading from
  joining the two tests before cse1 (func_0041F0C8 and its twin
  func_00457FE8).
- **Write both arms of an if/else in full** when the ROM's tails are shared:
  cross-jumping merges them as the original did.
- **Initialisations placed after the calls** fix sched2's order: insns that
  tie are taken by LUID, so the statement's place in the source decides
  (func_00248A8C).
- **`extern s32 G[]` and `G[0] = ...` instead of a scalar** keeps the store
  after pointer reads: GCC assumes a scalar global does not alias them and
  hoists it (func_00421528 and its twin func_0045A448).
- **`if (x == NULL) { A } else if (c) { A }`** is a natural form of a
  duplicated-branch fake (func_0026B4A4).
- **One file with its neighbour, again.** func_00409D4C's pool opens 4 past
  an 8-byte boundary with a jump table, so it went into func_00409A64.c; its
  1228.8 is `0x44999999` in the ROM, written `1228.7999f`, and the zero word
  after the pool stays in asm. Second-module copies that pass the module's own
  functions as handlers name them by link address, defined absolutely in
  `versions/undefined_funcs_extra.us.txt` (func_00467E4C).

### The B pass

Seven agents on the big drafts; six matched, among them CCamera Advance
(func_0027D160, 3744 bytes) and the level-geometry display lists
(func_00221D3C, func_002222E0):

- **One variable per block.** Reusing one variable across unrelated
  blocks makes a local pseudo a global one, which the other allocator
  places; give each block its own locals (func_0027D160's block-local
  temporaries).
- **Calls duplicated into each branch.** Where the ROM has the call in both
  arms of an if/else, write it in both, not once after it.
- **loop.c's invariant hoisting depends on the inner loop's insn count**,
  and a `memcpy` counts as one insn: Rage Wars' `memcpy` of a Vec3 in place
  of three stores is what gives the ROM's hoists (func_0042BA40).
- **An index as the induction variable** (`spawns[i].f`) gives other givs
  than a walking pointer (func_0042BA40).
- **Spill slots follow declaration order.**
- **`do { } while (0)` raises a pseudo's priority** (refs are weighted by
  loop depth), but placed around call-argument moves its NOTE_INSN_LOOP_END
  is a scheduling barrier there; put the block where it leaves the moves
  free (func_00221D3C, func_002222E0).
- **Table pointers written as `base + off + n`** let loop.c and cse2 create
  the copies the ROM has (func_00209AF4).
- **Reading a field twice** gives reload-CSE's `mov.s` copy of the first
  load (func_0027E000's m_WarpXScale).
- **Struct-typed globals** are MEM_IN_STRUCT, which changes alias decisions
  and so the order of stores.
- **Cross-jumping compares REG_EQUAL notes**: two moves with the same note
  (a0 = D_801198B0) are merged; the ROM keeps the tag branch's apart, so its
  move must carry no constant note (func_002478C8, matched in the F pass).
- **flow deletes a frame store overwritten before an aliasing read**, and a
  read through a pointer argument counts as aliasing: the ROM keeps both
  halfword stores of each pair (func_00424008, matched in the F pass).
- func_00209AF4 starts at 0x209AF4, not 16-aligned, right after
  func_002097AC, but has no .rodata: its own file links byte-exact.
  func_0042BA40 owns its pool at 0x800A5F54 and links alone; its
  second-module twin func_00464960 goes NOLOAD over it.

### The F pass

Seven agents on the last eleven drafts in `nonmatching/`; eight matched
(func_00204778, func_0022D2FC, func_002429BC, func_002478C8, func_00248720,
func_0024ED54, func_00255D70, func_00424008):

- **The base symbol decides the REG_EQUAL notes.** func_002478C8 reaches the
  game struct through the settings pointer, `(Game *)((u8 *)&D_8011AAD8 -
  0x1228)`, instead of naming `D_801198B0`: the a0 moves of the tagged and
  head-shot messages then carry another note than the "you killed" one, and
  cross-jumping no longer merges their tails (the B pass's 134 words).
- **Strings outside the pool are named by address.** The same function's
  messages sit in the data after `.rodata` (0x800B53D0-0x800B543C, "head
  shot!" among the hit-region names), not in its pool: as literals the
  object carried them in its own `.rodata` and no ROM slice matched, so the
  file declares them `extern char D_800B5420[];` and its `.rodata` is the two
  1.5f alone.
- **A hidden argument changes the prologue.** func_00255D70 calls
  func_0042EB04 with the player (as func_00249118 does): the value already in
  `$a0` at the call is what lets sched2 keep the mask's lui/ori together.
- **One return after the if/else, not one per branch.** With a `return 1` in
  each arm, reorg sees `$v0` live on the fall-through and annuls the slot
  (`bnel`); the shared return gives the ROM's plain `bne` (func_0024ED54).
- **An empty `do { } while (0)` before a `return`** in an inlined search
  loop (a debug print compiled out) leaves loop notes that stop
  find_and_verify_loops from moving the found block out of the loop
  (func_00248720). The inline's result goes in a user variable.
- **A constant in a user variable** (`s32 readyAnim = 1;` compared with
  `p->anim`) gets a global register (`$a2`, held across the block) and
  fills a delay slot, as in the ROM (func_00248720).
- **A dead store can be a CSE fence.** func_00204778's `else { upper =
  lower; }` is dead and flow deletes it after cse2, but until then the
  if/else stops -fcse-skip-blocks from running past the `upper < lower`
  block, so `b` is not known to be 0xFF at the join and `(b * 3) >> 1` stays
  a run-time multiply (FAKEMATCH; after Rage Wars' func_002529E4, which
  needs empty asms for the same).
- **Unions written by halves.** The VI registers are `Span` unions (one
  `u32`, or two `s16`); writing `h.h.start` and `h.h.end` on a copy held in
  a register gives the ROM's and/or inserts (func_00424008).
- **CSE's canonical register is the one used last.** Of two pseudos with the
  same value, cse makes canonical the one whose last use comes later:
  func_00424008's `h` must die before `oldH`, or the centre is computed
  from the load instead of `oldH`'s copy.
- func_00248720's pool opens at 0x800A6AB4, 4 past a boundary, after a
  1.5707964f only the VM's func_0042DE74 reads; it holds floats only, so its
  own file links and no merge was needed. func_0022D2FC took its place at the
  head of the particle file (`src/us/code/func_0022D2FC.c`): left
  undeclared until the later parts, its matrix helpers took their float
  arguments as doubles, so it declares them with the later parts' `Mtx44 *`
  prototypes, and its own types are `Mtx`-prefixed.

Left after the F pass: func_0027E000 (54 of 476 words, a sched1 tie: ulx's
HI move, with no birth boost since the clamp assigns ulx twice, and the
m_PixelXScale store are both priority 3, and the store wins on potential
hazard), func_00246BC0 (6 of 330) and func_0026C148 (1 of 62). The first and
the last matched in the G pass (below).

**Where the FAKEMATCH functions are.** Matches that rely on something the
original source surely did not have (a `register ... asm` pin, an
always-true test, a dead store) say so in a `/* FAKEMATCH: ... */` comment,
on the first line or at the trick. 22 files in `src/` have one
(`grep -rl FAKEMATCH src`): seventeen from earlier passes (register pins,
doubled code, always-true tests), func_00409A64.c for func_00409D4C, and this pass's
func_00204778, func_0022D2FC, func_002429BC and func_00424008. They match
byte for byte; the marker is for whoever looks for the original source.

### The G pass (the last drafts)

Two of the three drafts left in `nonmatching/` matched:

- **An inline setter, then re-read the field.** func_0027E000 (CCamera
  DisplayListSetup) stood at 54 of 476 words on a sched1 tie. Written as
  the source likely had it, a `SetPixelScale(cam, cam->m_WarpXScale,
  cam->m_WarpYScale)` inline that stores both scales, with the function then
  reading `cam->m_PixelXScale`/`m_PixelYScale` back, reload_cse turns the
  read into the ROM's `mov.s` and the tie goes away. Its `.rodata` starts at
  0x800A99E4, 4 past an 8-byte boundary, after a 5.0f that is
  func_0027D160's: it went into func_0027D160's file (one object, one pool;
  "One file with its neighbour").
- **One shared `fail:` label instead of several `return 0`.** func_0026C148
  had its failure returns cross-jumped after reload into the last one, so a
  `j` landed on the wrong block. With the first `return 0` under a label
  and the later failures `goto fail`, the ROM's single `v0 = 0; j epilogue`
  block sits right after the region test. match_func masks `j` targets, so
  the target (0x8026C224, the epilogue) was checked in the linked ROM.

Only func_00246BC0 (6 of 330 words) is left in `nonmatching/`.

### The readability pass

1,140 matched files of `src/us/code/` still reached fields through m2c's
offset macros (`M2C_FIELD`, `M2C_UNK`, `M2C_BITWISE`...). They were
rewritten with named struct fields, each checked with `tools/match_func.py`
(every function MATCH, no `M2C_` outside comments) and then all linked at
once: the clean build verified on the first try, func_0044C230 (the second
VM module copy, which match_func cannot check) included. Readable C went
from 88.2% to 98.8% of the code; matched bytes did not move. Where no header
fit, files got local structs: docs/STRUCTS_TODO.md lists them, and the
player.h offsets that need fixing before they can be hoisted. What the
rewrite taught about the compiler:

- **A struct store can move past a global load.** GCC 2.8's alias
  heuristic (MEM_IN_STRUCT against a scalar) lets the scheduler reorder a
  store through a struct field against a load of a global, where the
  m2c macro's store stayed in place. Storing through a local pointer to the
  field (`s32 *f = &p->field; *f = v;`) restores the ROM's order.
- **An extern's type is part of the address computation.** Declaring a
  global under another type (a struct instead of an array, or the reverse)
  can change how its address and offsets are computed, so keep the
  declaration the matched file had and name the fields around it.
- **Negative offsets stay as they are.** A field read below a symbol keeps
  `(&sym)[-1]` or a documented macro on the symbol the matched file used,
  rather than switching to the neighbouring symbol.
- **A `CVector3` passed by value** gives the same three `lw a1-a3` as the
  three floats m2c passed as words, so the by-value prototype can be used.

## Shared headers

The readability pass left each file with local structs where no header fit;
the struct consolidation then moved the biggest families into shared
headers, in two passes of seven branches each, merged with every function
still matching
(docs/STRUCTS_TODO.md has what stayed local and why, and what is next). A
file includes the header of the object it touches rather than declaring a
view of its own. `tools/check_structs.py structs/<name>.h` checks every
`/* 0xNN */` offset and `/* size */` comment in a header with the game's
compiler; `tools/struct_census.py` counts the local views that are left.

| Header | Holds |
|---|---|
| `structs/vector.h` | CVector3, CQuaternion, CMtxF: the engine math types (included by game_object.h) |
| `structs/game_object.h` | CGameObjectInstance (0x288, an "actor") and its parts: CInstanceHeader, CAnimationState, CPathTrack, CAI (the AI state at 0x140, with typed views of its per-type m_VarBuffer and CEnemyAIVars), CAIModeInfo (a mode-table row), CAITarget / CAITargets (what the AI aims at), CListedInstance (an instance with its world-list links and owner counter, 0x298), CInstancePool (D_8010ABA0: 32 of them with the free list D_8010FEA0 and the active list D_8010FEB4; CWorld.m_InstancePool) |
| `structs/intelligence.h` | CIntelligence, the definition `header.m_pIntelligence` points at: a common header and a union of per-type bodies (CEnemyIntelligence, CTurretIntelligence, CDoorIntelligence, CPlatformIntelligence, CGeneratorIntelligence, CActionIntelligence, CPlayerIntelligence...), Rage Wars' names (included by game_object.h) |
| `structs/player.h` | CPlayer (0x1328) and the HUD: COverlay, COnScreen, CPlayerOverlay, CPlayerContState, CPlayerLoadSaveData (the save blob), CSavedPlayers (D_8011F290, the players saved for a restart); includes controller.h, player_list.h and weapon.h |
| `structs/player_list.h` | CPlayerList / CPlayerCList: the player list at the start of CGame (D_80119870; its active list is D_80119890) |
| `structs/player_options.h` | CPlayerOptions (0x40): a player's options and per-opponent counts, `CPlayer::m_pOptions`; GameSettings holds five (included by player.h and game.h) |
| `structs/game.h` | GameSettings (D_8011AAD8, the game settings and the multiplayer setup: the HUD alpha, CSun m_Sun at 0x184, the engine's mode block at 0x1A8), GameMatch (D_8011ACB0, the play clock and the running match's rules); CGame (D_80119870: the player list, the camera pool D_801198B0 and its menu pool D_80119E2C, CDebugLines, the settings) and CGameFromCameraPool (CGame seen from D_801198B0); CEngineApp (D_800F6CB0, Turok 3's names: CFrameData m_FrameData[3], m_pCurrentFrameData D_800F7070, the world D_800F7078 as bytes, CGame m_Game at 0x22BC0 with its settings also as m_Settings) |
| `structs/world.h` | CWorld (D_800F7078, 0x19078): the resource files, the loaded level, its actor lists and pools, the warp state; CWarpPoint, CWorldSector, CWorldRecord, CLevelHeader, CGeomHeader |
| `structs/region.h` | CGameRegion (the floor triangles an object's `m_pCurrentRegion` points to, linked to their neighbours) and CRegionInfo (the attributes they share by index) |
| `structs/collision.h` | the collision code: CCollisionInfo (an object's collision response, Rage Wars' CCollisionInfo2_t), CCollisionResults (D_800F1CE0, what the last test hit; D_800F1ADC points at the one being filled) and its HitFrame node record, Sweep / SweepHit (a move as func_00239F00 tests it) with ModelSweep, MapSweep, SweepBox, CollisionQuad (a box face), DamageEvent (func_00245828), CBox (D_800F5A10) |
| `structs/camera.h` | CCamera and CCameraPool, with CCameraViewParams, CCameraViewport, CShake, CCameraText, CPlane |
| `structs/list.h` | CList, Acclaim's intrusive list (the IntrusiveList of the Rage Wars decomp; included by camera.h) |
| `structs/cache.h` | the resource cache: CMemEntry (a loaded block: data, size, lock count, flags, age) and CCacheEntry (a load request), Turok 3's names |
| `structs/pak.h` | the Controller Pak menus: PakDirectory (D_800C2224, the cached note directory per channel), RumblePak (D_80130AA0), PakProfile (the 0x18-byte "T2 MULTI" note, D_80130968), PakQuery; the OSPfs per channel is D_80130C80 |
| `structs/menu.h` | the menu system: Menu (a window), MenuItem, MenuItemDesc, MenuDesc, MenuItemLayout, MenuColor, MenuPool (a window's controller is a ControllerProfile) |
| `structs/controller.h` | ControllerProfile (D_800F5EC0[4], 0x224, and the merged fifth D_800F5C78): a controller's buttons (held, single, double, repeat...), stick and Rumble Pak state, `CPlayer::m_ContState.m_pContState`; rw/abu_types.h includes it |
| `structs/sound.h` | the sound manager: SndMgr, SndVoice (0xCC) and SndVoiceParams, SndEnv, SndSeq, SndVoicePool, SndFx, SndSource, SoundEffectInfo (includes libaudio.h) |
| `structs/libaudio.h` | libultra 2.0I's libaudio with its own names (PR/libaudio.h, abi.h and the synthesizer / sequence player / sound player internals): Acmd, ALHeap, the bank and wave table types, ALSynth and ALGlobals (alGlobals is D_800B7760), ALPlayer, ALVoice, PVoice, ALParam, the filters (ALFilter, ALLoadFilter, ALResampler, ALEnvMixer, ALSave, ALFx, ALDelay, ALLowPass, ALMainBus, ALAuxBus), ALEvent and ALEventQueue, ALSeqPlayer, ALCSPlayer, ALSndPlayer, ALSoundState, ALVoiceState, ALChanState, ALSeq, ALCSeq |
| `structs/weapon.h` | the weapons: WeaponInfo (the weapon table D_800AF618) and its AmmoInfo, CBarrel (the weapon wheel, CPlayer m_Barrel) with CBarrelItem and CBarrelIcon (included by player.h) |
| `structs/cinema.h` | the cinema (cutscene) state Cinema (*D_800C1BB0, 0x100) and CinemaEntry / CinemaTable |
| `structs/boss.h` | the boss and level-script state of the VM modules: LevelScriptState (D_8012F608, with the BossState / BossMode views at D_8012F9D8 / D_8012F9DC, BossStep and the three script runners), MotherBossState (D_8012FB70, its BossWeakSpot), BlindOneState (D_80130200) with the Blind One's script VM (BlindOneStep, BlindOnePhase: the phase tables D_0043240C / D_0043232C, BlindOneMover), ScriptAnimator (the 0x64-byte value/colour animator: lights, weak spots, hit flashes) |
| `structs/particle.h` | the particle system: CParticle (0x1D8), CParticleSystem (D_800F8928: 128 particles and their free, draw and active lists), the templates CROMParticleEffect / CROMParticleImpact / CROMParticleParams / CROMParticleDistance, CEventValue (an impact event's value), CDLight (a dynamic light, D_8010FF34) |
| `structs/gbi.h` | libultra graphics types at the size the structs embed them (Mtx, Vp) |
| `ultra_types.h` | libultra's OSThread, OSMesgQueue, OSTimer, OSContPad, OSPfs, OSPfsState and the Controller Pak internals, the VI context (__OSViContext: __osViCurr / __osViNext are D_800B7AC0 / D_800B7AC4, OSViMode, __OSViScale), plus the engine's CMutex |
| `gfx.h` | Gfx (w0/w1) and the display-list pointer the engine appends to |

The imported Rage Wars files still take their own types from `include/rw/`
(abu_types.h, junk_types.h, ultralib_types.h), but those headers include
the shared ones above instead of repeating them, so a file can include an
rw header together with any shared header. Where a Rage Wars type had the
same name and another layout it was renamed (abu's float matrix is RW_Mtx,
not gbi.h's Mtx). Their libaudio types are structs/libaudio.h's, and their
intrusive lists are structs/list.h's CList, so junk_types.h and abu_types.h
can be included together.

## The fast assembler

`asn64` only runs under wine, which costs ~30 s a file in the emulated amd64
container and turns a full build into 20+ minutes. `tools/sn_as.sh` assembles
cc1's output with GNU as instead and reproduces the two things asn64 does
differently: `move` becomes `addu rd, rs, $0`, and only the c.cond -> bc1x
hazard gets a nop (cc1 marks every hazard with `#nop`; asn64 fills just that
one). All eight probe functions match either way, and a full build takes
seconds. `make ASSEMBLER=asn64` and `match_func.py --assembler asn64` keep
the original tool available as a cross-check, and `make verify` decides.

**Multiplies, divides and the HI/LO hazard.** asn64 keeps an mfhi/mflo two
instructions ahead of the next mult/div/mthi/mtlo, in noreorder regions too,
counting every instruction a macro expands to, and it never leaves an integer
multiply or divide in a delay slot. Probed with wine asn64 on small inputs,
the rules are:

- a mult/multu/div/divu (or a d- form) cc1 put in a delay slot moves in front
  of its branch or jump, of any kind (likely, `jal`, `jr $31`), with a nop in
  the slot; a label on the branch stays on the moved instruction;
- unless the instruction before the branch is an mfhi/mflo: then the slot
  keeps it and the one hazard nop goes before the branch;
- the moved instruction is not checked again (`mflo; addu; bgez; mult`
  becomes `mflo; addu; mult; bgez; nop`), but the mul.s -> multiply nop of
  the erratum still applies to it;
- a section directive in the source pads the hazard pending at that point
  (gas does the same), but `li.s`, expanded inside asn64, has none.

gas differed three ways, each fixed in the fast path: it read `lw $2, D_X`
as a possible one-instruction $gp load when counting the distance (gas now
runs with `-G0`); `tools/sn_li.py` wrote a `.section .rodata` after every
`li.s`, so an `li.s` right after an mfhi got two nops asn64 never wrote (the
pool is now written at cc1's next section directive); and gas neither moves
the delay-slot multiplies nor pads the hazard inside noreorder regions, which
`tools/sn_hilo.py` does. None of the already-matched functions changed (every object's
.text and .rodata came out the same), and two functions held back by them
matched: func_00282340 and func_00279528.

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
| `bnez v; addu v0, v` then a default | early return: `if (v != 0) return v; return DEF;`, not `if (!v) v = DEF;` |
| `jal` then `j .Lend` + `nop`, else branch sets `v0` | one result variable: `if (c) r = f(); else r = 0; return r;` |
| a NULL check's `return 0` placed after the loop body | spell the `else`: `if (e != NULL) { ... } else { return 0; }` |
| a float pool `[-pi, 2pi, pi, 2pi]` read as `D_X` and `D_X + 4` | write every value as a literal, in order of use: GCC 2.8 does not merge equal float literals, so a repeated one gets its own slot (func_002118B0) |
| only the frame size differs (`addiu $sp, -0x90` vs `-0x30`) | a local buffer m2c typed as a scalar: give it the size, `u8 sp10[0x68]` (the permuter scores frame size near zero, so its "matches" need this check) |
