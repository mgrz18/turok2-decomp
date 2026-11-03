# VMASM — the TLB-paged code system

The SN64 LNK records name three source files that matter here:

```
engine/PTIMER.CPP
engine/Virtual/VMEMORY.C
engine/Virtual/VMASM.S
```

`VM` is **virtual memory**, not virtual machine. Turok 2 runs its engine code
through the R4300's TLB and pages parts of it in from ROM at runtime. Earlier
passes of this document read the subsystem as a conventional overlay swapper
and drew conclusions that turned out to be wrong; those are recorded at the
bottom so nobody repeats them.

## What the boot stub proves

`src/us/asm/boot.s` — hand-written, byte-verified against the ROM — programs a
TLB entry before jumping into the game:

```
li  $a0, 0x1F           # TLB index 31
li  $a1, 0x001FE000     # PageMask: 2 MB pages
li  $a2, 0x00200000     # vaddr
li  $a3, 0              # paddr0
li  $t1, 0x00100000     # paddr1
```

So virtual `0x00200000`–`0x003FFFFF` maps to physical `0x0`–`0x001FFFFF`. The
engine is DMA'd to physical 0 and executes at `0x00200500`, which is why the
`code` segment's VRAM is what it is (see `VRAM-LAYOUT.md`).

That entry covers the engine. It does **not** cover the `virtual` modules,
which live at `0x00400000` and up — outside the 2 MB window. Those need
further TLB entries, and `VMASM.S` is what installs them.

## Memory map

| region | ROM | address | space |
|---|---|---|---|
| boot stub | 0x1000 | `0x80000400` | KSEG0, unmapped |
| engine `.text` | 0x1100 | `0x00200500` | useg, TLB-mapped |
| engine `.rodata` | 0xA5FD8 | `0x800A53D8` | KSEG0, direct |
| libultra | 0xC3074 | `0x800C2474` | KSEG0, direct |
| `virtual` modules | 0x14A000 | `0x00400000` | useg, TLB-mapped |

libultra has to stay unmapped: it holds the exception vectors and the TLB
refill handler itself.

Turok 3 uses the same layout — `references/turok3/versions/turok3.us.yaml`
places its `code` at `0x00200500` and its `virtual` at `0x00400000`, with the
comment "some functions are mapped as virtual, and get hot-loaded from rom".

## The second module is linked over the first

The second module (`virtual_1`, ROM 0x186C14-0x1BFA00) is named by splat at
0x43CC14, right after the first, but it was linked 0x39000 lower, at
0x403C14: every `j`, every `jal` into its own code and every jump table it
reads points there. Scored by how many of its 613 `jal`s into
0x400000-0x440000 land on a prologue, the distance 0x39000 gets 457 and no
other distance more than 75, and it holds from the module's first code to
its last. The first module occupies 0x400000-0x43CC14, so the two cover the
same addresses: the second is paged in over the first.

That answers the question below for these two: **overcommit**. A TLB-free
build cannot just load everything at its address, since two ROM regions
want the same one.

What this means for the build:

- `versions/turok2.us.yaml` still names `virtual_1` at 0x43CC14 (its
  symbols keep their names), and `make setup` writes the link address,
  0x403C14, into the linker script (`VIRTUAL_1_LINKED` in the Makefile;
  splat has no way to name a segment at one address and link it at another).
  Before that, C for the second module linked but built other bytes: its
  `j` and jump-table words came out 0x39000 too high. 118 functions went in
  once it linked at 0x403C14.
- ROM 0x186000-0x186C14, which splat had put at the start of `virtual_1`, is
  the end of the first module's .rodata: descriptor records (`0x0000FF01`,
  pointers to the first module's functions) that the engine and the first
  module pass as 0x43C05C ... 0x43CAB4. `virtual_rodata_0` now runs to
  0x186C14.
- With the modules overlapping, an address does not say which one a symbol
  is in. `tools/match_func.py` (`sym_rom`), `tools/c_subsegment.py` and
  `tools/progress.py` go through the symbol's section; `progress.py`'s
  second-copy rule compares a `virtual_1` function with the first module's
  function at its link address (or 0xE0 on).
- The second module still calls and jumps to addresses that are labels or
  function starts of the first module's layout (0x426D24, 0x429D1C,
  `.L004237FC`, ...): they are its own code at its link address. When a cut
  fix or C in the first module stops defining one, it goes in
  `versions/undefined_funcs_extra.us.txt` as an absolute address.
- Many of the second module's functions are byte copies of the first's at
  the same link address (`progress.py` counts them once).
- `func_0045007C` does not live at 0x0045007C any more, and the one word in
  `virtual_rodata_2` with that value is data: the symbol has
  `can_be_referenced:False`.

## The second module shares the first module's .rodata

The second module's code reads the first module's .rodata: the literals and
strings of its copies and even jump tables, whose entries point at the first
module's code. Evidence:

- The cinema file (0x40F240-0x413228) and the file 0x4143DC-0x415CE8 are in
  the second module byte for byte at the same link addresses (named
  0x448240-0x44C228 and 0x44D3DC-0x44ECE8), and every load of a literal in
  them names the first module's pool (0x800C0950-0x800C0AE8, 0x800C0BE8-).
- The menu block (named 0x450490 on, linked at 0x417490 like the first
  module's) is a variant build: some functions are shorter (0x28 bytes each
  at 0x417E84, 0x417F18, ...), so from 0x417E84 on the copies sit 0x50 below
  their originals. They still read the originals' strings ("  %02d",
  "none", " %3d", ...) at the first module's addresses.
- func_00452960 (link 0x419960) indexes jtbl_800C1048, func_00419A00's
  table: its six entries 0x419A28 ... 0x419A78 are the original's case
  labels, past the end of the copy. The copies 0xE0 below their originals
  (func_00465EB0/func_0042CF90, func_0045BDAC/func_00422E8C, ...) do the
  same.

So the second module's objects were compiled from the same sources (with
some differences) and linked against the first module's data, which the ROM
holds once. The build models it so:

- C for a second-module function is written as its source was, with its
  literals, strings and switches, and emits its own .rodata, laid out like
  the first module's for the same source.
- `versions/shared_rodata.us.txt` names the file and the first-module
  address its .rodata stands for. `make setup` (`tools/shared_rodata.py`)
  gives each one an output section at that address, NOLOAD: the code's
  relocations resolve into the first module's pool, and the bytes come from
  the first module's object only.
- `make verify` then runs `shared_rodata.py --check`: each such object's
  .rodata has to equal the ROM at its address, and each jump-table entry has
  to keep its distance from the table's first entry (the object's entries
  name the copy's labels, the ROM's the original's). That check caught
  func_0045BDAC, whose Rage Wars case list linked byte-exact but did not
  give Turok 2's table.
- Where the copy is the original file byte for byte, the copy is one C file
  made from the original's source with the second-module names
  (src/us/code/func_00448240.c from func_0040F240.c, func_0044C230.c from
  func_00413230.c); its .rodata alignment then works as the original's did.
- A string the original file emits in an earlier function stays an extern
  (func_0045190C's " %3d", as func_004189AC has it).
- The second module's own data (virtual_rodata_1) is named by link address
  in its code (`D_00438824`, named 0x471824): defined absolutely in
  `versions/undefined_funcs_extra.us.txt`.

Which of the two owns a pool word in the yaml is the first module's call
now: func_0042CF90 and func_0042D120 own their literals in the engine's
.rodata (0x800A6050, 0x800A60B8) and their copies func_00465EB0 and
func_00466040 go over them.

The VM pools are interleaved with the engine's in its .rodata (0x800A600C
is func_0042C560's, 0x800A6010 the engine's func_002261A8's, 0x800A6014
func_0042CBE4's). The word at 0x800A6010 (-1.0f) is the engine's: only
func_002261A8 reads it (`bestIdx = -1.0f`, matched C), and no VM code names
it. But func_0042CBE4 cannot open an object of its own at 0x800A6014: cc1
puts a jump table after `.align 3`, so its .rodata is 1.0f, a pad word, then
the table at offset 8, and an object there puts the table at 0x800A601C
(all 61 named jump tables in the ROM are 8-aligned). The ROM's 1.0f at
0x800A6014 and table at 0x800A6018 fit only a pool that is already 4 bytes
in at 0x800A6014, that is, one .rodata section holding func_002261A8's
-1.0f and func_0042CBE4's literal and table: one source file whose
functions went to two segments, the engine's .text and the VM's. cc1 can do
that (`-ffunction-sections` names each function's section `.func_X`, and
splat can link a subsegment's `linker_section`), but nothing else in the
build works that way, the object's other members are unknown, and the
second module's copy func_00465B04 would need the same object with its
engine half thrown away. So func_0042CBE4 and func_00465B04 stay asm (both
have matching C in build/manual/X), and this is the first evidence that
some engine and VM functions share a source file.

The X batch moved eight pools from the second module's C to the first
module's: func_00405AC8, func_00406000, func_00409500, func_00409784,
func_00409A64, func_0040D5C4, func_0041760C and func_00424780 own them now
(and func_004088B0 its strings), and their copies go NOLOAD over them.
Second-module code also names first-module addresses inside functions that
are still asm (func_0042CAB4 + 0x50, .L0042B960, func_0042CBE4 + 0x40) as
callees or callbacks; those are defined absolutely too. The second module's
text does not end at 0x469400: func_00469374 runs to 0x46941C, and the
words after it still decode as code, so virtual_rodata_1 likely starts
later still.

Object boundaries in the VM modules: each object starts 16-aligned in .text
and in .rodata, with junk words before (often copies of instructions). The
cinema object ends at the junk at 0x413228 in .text and at 0x800C0AE8 in
.rodata, and func_004131B0, its last function, is the out-of-line copy of
an inline helper (GCC 2.8 emits a non-static inline function's own copy at
the end of the file). By the same rule the file holding func_004143DC opens
at 0x413230 (.rodata 0x800C0AF0) and runs to 0x417484 (.rodata to
0x800C0E64): the string "menu" at 0x800C0C54 is read by func_00415FB8,
func_00416644, func_00416760, func_00416894 and func_004172F8. That file is
now one C file, src/us/code/func_00413230.c: 46 parts each matched alone
first, put together with their own types (a name two parts both define gets
the part's address as a suffix) and their own prototypes (where two parts
declare one symbol differently, the later one names it through an alias,
`extern T D_8011AAF0_413F74 __asm__("D_8011AAF0");`, so each part compiles
as it did alone). "menu" is a string constant again, and the file's .rodata
is the ROM's 0x374 bytes from 0x800C0AF0, gaps included.

## What is still open

Whether the paging is **relocation** or **overcommit**. That is the question
that decides whether this project can produce a TLB-free build:

- If each ROM region maps to one distinct virtual address and nothing is ever
  swapped, the TLB is only relocating. Load everything at its address on an
  8 MB machine and the TLB stops being necessary.
- If two ROM regions share a virtual address, code really is paged in and out,
  and removing the TLB means restructuring the game's memory model.

`VM_PhysicalPool`, exported from `VMEMORY.C`, is the allocator to read. Its
size relative to the ~800 KB of `virtual` code is the answer.

This matters because N64Recomp does not support TLB — its README lists
relocations for TLB mapping as planned, not implemented. Every TLB-using N64
game running natively today got there through a full decompilation: GoldenEye
builds a TLB-free ROM from a dedicated decomp branch, Perfect Dark ships as a
decomp-based port. Neither route is available here, so a TLB-free build made
at the assembly level would be new ground.

## Refuted hypotheses

Kept on record so they are not retried. Each is also a closed issue with the
full evidence.

**The modules are compressed.** No `RNC`/`Yay0`/`Yaz0`/`MIO0`/`LZ77` magic
anywhere in the ROM, entropy is ordinary (173–193 unique bytes per 4 KB against
180 for known-good code), and `virtual_text_0` opens on `27bd ffe0`, a valid
prologue.

**The dispatch table lives in the rodata pools.** The `0000ff01`-delimited
records in `virtual_rodata_0` (292) and `virtual_rodata_1` (310) looked like a
linkage table, and 602 records sat suspiciously close to the ~620 unresolved
references then being reported. But the `0x0040xxxx`–`0x0043xxxx` values in
those pools have **zero** intersection with the unresolved functions in that
cluster. Coincidence.

**VMASM builds the table at runtime from the LNK records.** A shipping game
does not parse linker debug records at runtime; they are build leftovers, as
`LNK-FORMAT.md` says. The volumes do not work either — 3 records totalling
~10 KB inside a 214 KB region. This hypothesis was built on the premise that
the blocker was VRAM-related, which was itself wrong.

**The blocker is a dual-VRAM conflict.** What actually stalled the project was
that the whole address space was shifted by 0x200000 and assumed to be KSEG0
when the code runs in useg. Correcting that took the disassembly from 48.8% to
95.2% and made the `jal` graph close on itself.

## Regenerating the layout read-out

```bash
python3 tools/function_seed.py            # jal targets, prologues, unplaced banks
python3 tools/metrics.py                  # decode and symbol coverage
```

## Implication for Armorines

Same engine, so the same `VMASM.S` / `VMEMORY.C` pair and almost certainly the
same split of useg code against KSEG0 data. The ROM offsets will differ; the
method in `VRAM-LAYOUT.md` recovers them without any prior knowledge.
