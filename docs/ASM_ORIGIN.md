# Code that is not C by origin

After the matching passes, 50 sized functions in `build/turok2.us.elf` were
still not defined by any linked C file (`tools/progress.py`). Three of them
are C still to finish (func_00246BC0 in `nonmatching/engine/`, and
func_0042CBE4 / func_00465B04, blocked by a shared .rodata pool:
docs/VMASM.md). The other 47 were reviewed one by one: none is compiled C.
They are hand-written asm (44) or fragments that are not functions (3), and
no C can ever be written for them.

`versions/asm_origin.us.txt` lists them, one line each with the reason.
`tools/progress.py` reads it: the listed functions get their own row,
`asm (by origin)`, and are left out of the per-region, `total` and
`readable` percentages, so those are over C-origin code and reach 100% when
every C function is matched. The `all code` row keeps the old figure
(matched C over every byte, asm included), and `--badge --raw` prints it.
A name in the list that linked C defines is reported on stderr.

## How a function was judged

- **asm (by origin)**: any of
  - cop0/cop1 control and cache instructions a C compiler does not emit:
    `mtc0`/`mfc0`, `tlbwi`/`tlbr`/`tlbp`, `eret`, `cache`, `cfc1`/`ctc1`;
  - registers outside the o32 ABI: `$k0`/`$k1` as base registers, `$ra`
    parked in `$t8`/`$t9`/`$s2` around a `jal`, arguments in `$s0`-`$s3`, a
    bit buffer living in `$t6`/`$t7` across calls;
  - frames GCC 2.8.1 does not produce: `addi $sp` (cc1 always uses `addiu`),
    `sw` of `$t0`-`$t7` to the frame, a `jalr` into the function itself;
  - the same instruction sequence as an ultralib `.s` file
    (`references/ultralib/src/os/*.s`) or as SN64's `SNMAIN.s`
    (`references/turok3/src/common/asm/SNMAIN.s`).
- **fragment**: the bytes are not a function of their own. The three in the
  .code tail are copies of code 0x39000 lower (the same distance as the VM
  modules' stale second copy, docs/VMASM.md): compared byte for byte with
  the ELF, and scanned for `j`/`jal` targets, words and `lui`/`addiu` pairs
  that point into 0x002C22D8-0x002C2474 over every section: none in code or
  data (two matches in `.assets` are compressed bytes). They are dead.
- **C**: none. A function that looked like compiler output (func_002C22D8,
  func_002C232C, func_002C23E0: `addiu $sp`, `$ra` at the frame top) turned
  out to be one of the copies.

## The 47

| Function | Size | File | Kind | Evidence |
|---|---:|---|---|---|
| boot_main | 136 | src/us/asm/boot.s | asm | SN64 `SNMAIN.s` romMain, bit-identical to Turok 3's: sets `$sp`, clears the TLB with `tlbwi`, maps 0x200000 |
| STVM_MapTLB | 120 | src/us/asm/boot.s | asm | `SNMAIN.s`: `mtc0` Index/PageMask/EntryHi/EntryLo, `tlbwi` |
| func_00200C40 | 112 | us/asm/1840.s | asm | RNC unpacker entry: `addi $sp`, arguments moved to `$s0`-`$s3` for the unpacker |
| func_00200CB0 | 364 | us/asm/1840.s | asm | RNC main loop: bit buffer in `$t6`/`$t7`, `$ra` parked in `$t8`, `$sp` scratch table |
| func_00200E1C | 164 | us/asm/1840.s | asm | RNC Huffman decode: table in `$s0`, bit buffer in `$t6`/`$t7`, no frame |
| func_00200EC0 | 40 | us/asm/1840.s | asm | RNC getbits: `and`/`srlv` on the bit buffer, `$ra` parked in `$t9` |
| func_00200EE8 | 56 | us/asm/1840.s | asm | RNC refill: register state in `$t1`/`$t6`/`$t7`/`$s3`, no arguments, no frame |
| func_00200F20 | 340 | us/asm/1840.s | asm | RNC table builder: `$ra` in `$t8`, `addi $sp` scratch |
| func_00201074 | 68 | us/asm/1840.s | asm | `mtc0` WatchLo, `mfc0` WatchHi (`__osSetWatchLo`-like) |
| func_002010C0 | 1768 | us/asm/1840.s | asm | RNC decompressor (versions/cuts.us.txt): `addi $sp`, saves `$t0`-`$t7`, `jalr` into itself at +0xA8 |
| func_0028EE40 | 4 | us/asm/8FA40.s | asm | the first word (`lw $t8, 0x10($sp)`) of the engine's STVM_MapTLB, cut off from func_0028EE44 |
| func_0028EE44 | 124 | us/asm/8FA40.s | asm | STVM_MapTLB (the same code as SNMAIN's): `mtc0` TLB registers, `tlbwi`, branches into func_0028EEC0 |
| func_0028EEC0 | 60 | us/asm/8FA40.s | asm | STVM unmap: `mtc0` Index/EntryHi/EntryLo, `tlbwi` |
| func_0028EEFC | 136 | us/asm/8FA40.s | asm | TLB dump: `mtc0` Index, `tlbr`, `mfc0` PageMask/EntryHi/EntryLo |
| __osException | 1528 | us/asm/9CBE0.s | asm | ultralib `exceptasm.s`: `jr $k0`, `sd` of the GPRs through `$k0`, `mfc0` SR/Cause |
| send_mesg | 260 | us/asm/9CBE0.s | asm | `exceptasm.s` (static): `$ra` parked in `$s2`, t-registers only |
| __osEnqueueAndYield | 412 | us/asm/9CBE0.s | asm | `exceptasm.s`: saves the context with `sd`, `mfc0` SR |
| __osEnqueueThread | 80 | us/asm/9CBE0.s | asm | `exceptasm.s`: `$t6`-`$t9` only |
| __osPopThread | 24 | us/asm/9CBE0.s | asm | `exceptasm.s` |
| func_0029C8E0 | 508 | us/asm/9CBE0.s | asm | `exceptasm.s` `__osDispatchThread`: `ld` of every GPR from `$k0`, `mtc0` SR/EPC, `ctc1`, `eret` |
| func_0029CADC | 8 | us/asm/9CBE0.s | asm | `exceptasm.s` `__osCleanupThread`: `jal osDestroyThread` with `$a0` = 0, never returns |
| func_0029DFD0 | 12 | us/asm/9EBD0.s | asm | `getfpccsr.s` `__osGetFpcCsr`: `cfc1 $31` |
| func_0029DFE0 | 12 | us/asm/9EBE0.s | asm | `getsr.s` `__osGetSR`: `mfc0` SR |
| func_0029DFF0 | 32 | us/asm/9EBF0.s | asm | `interrupt.s` `__osDisableInt`: `mfc0`/`mtc0` SR |
| __osRestoreInt | 28 | us/asm/9EBF0.s | asm | `interrupt.s` `__osRestoreInt`: `mfc0`/`mtc0` SR |
| func_0029E030 | 152 | us/asm/9EBF0.s | asm | `invaldcache.s` `osInvalDCache`: `cache` 0x15/0x11/0x01 |
| func_0029E0D0 | 104 | us/asm/9ECD0.s | asm | `invalicache.s` `osInvalICache`: `cache` 0x10/0x00 |
| func_0029E140 | 200 | us/asm/9ED40.s | asm | `probetlb.s` `__osProbeTLB`: `tlbp`, `tlbr` |
| func_0029E220 | 16 | us/asm/9EE20.s | asm | `setfpccsr.s` `__osSetFpcCsr`: `cfc1`/`ctc1 $31` |
| osSetIntMask | 168 | us/asm/9EE20.s | asm | `setintmask.s`: `mfc0`/`mtc0` SR, MI_INTR_MASK |
| func_0029E2E0 | 16 | us/asm/9EEE0.s | asm | `setsr.s` `__osSetSR`: `mtc0` SR |
| __osUnmapTLBAll | 68 | us/asm/9EEE0.s | asm | `unmaptlball.s`: `tlbwi` loop |
| func_0029E340 | 104 | us/asm/9EF40.s | asm | `writebackdcache.s` `osWritebackDCache`: `cache` 0x19/0x01 |
| func_0029E3B0 | 40 | us/asm/9EFB0.s | asm | `writebackdcacheall.s` `osWritebackDCacheAll`: `cache` 0x01 |
| func_002A3088 | 120 | us/asm/A3C88.s | asm | exception-vector stubs (`mtc0` ErrorEPC, `jr $k0`) that func_002A3100 copies to 0x80000000/0x80000180 |
| func_002A3100 | 172 | us/asm/A3C88.s | asm | swaps those stubs into the vectors; `addi $sp` |
| func_002A31AC | 28 | us/asm/A3C88.s | asm | icache invalidate-all: `cache` 0x01 |
| func_002A31C8 | 28 | us/asm/A3C88.s | asm | dcache invalidate-all: `cache` 0x00 |
| func_002A31E4 | 12 | us/asm/A3C88.s | asm | `mfc0` SR |
| func_002A31F0 | 12 | us/asm/A3C88.s | asm | `mtc0` SR |
| func_002A31FC | 132 | us/asm/A3C88.s | asm | TLB dump to D_80128A90: `tlbr` loop, `addi $sp` |
| func_002A3708 | 108 | us/asm/A3C88.s | asm | puts the vector stubs back, then the cache loops |
| func_002A3774 | 88 | us/asm/A3C88.s | asm | exception return paths: `ld` through `$k0`, `mtc0` EPC, `eret` |
| func_002A37CC | 776 | us/asm/A3C88.s | asm | context save: `sd` of every GPR and cop0 register through `$k0` |
| func_00425710 | 68 | us/asm/16F710.s | asm | `mtc0` WatchLo, `mfc0` WatchHi: the bytes of func_00201074 again |
| func_002C22D8 | 84 | us/asm/C2ED8.s | fragment | func_002892D8 but for one `jal` (0x42AB2C where the live one has 0x42AC0C): a copy from an older link, dead |
| func_002C23E0 | 148 | us/asm/C2FE0.s | fragment | func_002893D8 from +8 (`stray_002C23D8` holds its first 8 bytes); its `j` goes to 0x289428, inside func_002893D8; dead |

func_002C232C (172 bytes, us/asm/C2ED8.s), the third of the .code-tail
functions in docs/SEGMENTS.md, is func_0028932C byte for byte (its `j` goes
to 0x2893B0, inside func_0028932C): `progress.py` already leaves it out as a
second copy, and it is in the list as a fragment for completeness.

Sizes: 9,040 bytes in all (8,808 asm, 232 fragment), of which the VM-region
rows hold func_00425710 and the two fragments (`region()` puts everything at
or above 0x002A5200 there, the .code tail included).

## What this changed in the numbers

`progress.py` also counted a function twice when two linked C files defined
it (a static copy of a global helper): func_0027BC34 (3 files),
`corrupted` (2) and `__osIdCheckSum` (7): 888 bytes. Each name is now counted
once, which is why the VM modules no longer reach 100% while two of their
functions are still asm. With both fixes, the only C-origin code left is the
three C functions above (1,896 bytes).
