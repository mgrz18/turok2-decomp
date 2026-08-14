/*
 * src/us/asm/boot.s — hand-written boot routine for Turok 2 (US).
 *
 * Mirrors the original SN64 SNMAIN.s (boot_main + STVM_MapTLB) at
 * ROM 0x1000 / VRAM 0x80000400. Verified byte-exact against
 * baserom.us.z64 bytes 0x1000..0x10FC by scripts/verify_boot_asm.sh.
 *
 * Wired into the main build via `hasm` in versions/turok2.us.yaml
 * (main segment, subsegment ../src/us/asm/boot) — splat now emits
 * the ld script referencing build/src/us/asm/boot.s.o instead of an
 * auto-generated 1000.s.
 *
 * Why hand-written asm and not C:
 *   - The routine reads/writes CP0 registers (Index, EntryHi,
 *     EntryLo0/1) and issues `tlbwi`. None of these are expressible
 *     in C; SN64-era cc1 has no intrinsics for them.
 *   - It manipulates $sp before any prologue runs, so even an
 *     `__attribute__((naked))`-style C function would have to be
 *     100% inline asm — at which point .s is cleaner and matches
 *     what every other Acclaim-London decomp (turok3, etc.) does.
 *
 * Sibling references (do not modify):
 *   references/turok3/src/common/asm/SNMAIN.s   — bit-identical
 */

.include "macro.inc"

.set noat
.set noreorder
.set gp=64

.section .text, "ax"

# ----- boot_main (== romMain at 0x80000400) -------------------------------

# `.ent`/`.end` are what make gas emit these as STT_FUNC with a size. Without
# them the symbols land as NOTYPE/size 0, and N64Recomp — which only walks FUNC
# symbols — reports "Could not find entrypoint function" for 0x80000400.
# They are metadata: the emitted .text bytes are unchanged.
.globl boot_main
.ent boot_main
boot_main:
/* 1000 80000400 */  la         $sp, 0x80400000 - 0x40    # sp = 0x803EFFC0
/* 1008 80000408 */  addiu      $a0, $zero, 0x1E          # TLB clear loop counter
.Lclrlp:
/* 100C 8000040C */  mfc0       $t0, $10                  # save EntryHi
/* 1010 80000410 */  mtc0       $a0, $0                   # Index
/* 1014 80000414 */  lui        $t1, 0x8000               # invalid EntryHi
/* 1018 80000418 */  mtc0       $t1, $10
/* 101C 8000041C */  mtc0       $zero, $2                 # EntryLo0
/* 1020 80000420 */  mtc0       $zero, $3                 # EntryLo1
/* 1024 80000424 */  nop
/* 1028 80000428 */  tlbwi
/* 102C 8000042C */  nop
/* 1030 80000430 */  nop
/* 1034 80000434 */  nop
/* 1038 80000438 */  nop
/* 103C 8000043C */  mtc0       $t0, $10                  # restore EntryHi
/* 1040 80000440 */  nop
/* 1044 80000444 */  bnez       $a0, .Lclrlp
/* 1048 80000448 */   addi      $a0, $a0, -1

/* 104C 8000044C */  addiu      $a0, $zero, 0x1F          # MapTLB: idx 31
/* 1050 80000450 */  li         $a1, 0x001FE000           # PageMask
/* 1058 80000458 */  li         $a2, 0x00200000           # vaddr
/* 105C 8000045C */  li         $a3, 0                    # paddr0
/* 1060 80000460 */  li         $t1, 0x00100000           # paddr1
/* 1064 80000464 */  sw         $t1, 0x10($sp)
/* 1068 80000468 */  li         $t0, 7                    # flags = V|G|D
/* 106C 8000046C */  jal        STVM_MapTLB
/* 1070 80000470 */   sw        $t0, 0x14($sp)
/* 1074 80000474 */  lui        $a0, %hi(main)            # main @ 0x0028D380 (D_28D380)
/* 1078 80000478 */  addiu      $a0, $a0, %lo(main)
/* 107C 8000047C */  jalr       $a0
/* 1080 80000480 */   nop
/* 1084 80000484 */  .word      0x0001008d                # break 1, 2
.end boot_main

# ----- STVM_MapTLB (== func_80000488) -------------------------------------

.globl STVM_MapTLB
.ent STVM_MapTLB
STVM_MapTLB:
/* 1088 80000488 */  lw         $t8, 0x10($sp)            # paddr1 (caller slot)
/* 108C 8000048C */  addi       $t0, $zero, 7
/* 1090 80000490 */  lw         $t9, 0x14($sp)            # flags
/* 1094 80000494 */  addi       $t7, $zero, -1
/* 1098 80000498 */  and        $t9, $t9, $t0
/* 109C 8000049C */  ori        $t9, $t9, 0x18            # | C=cached
/* 10A0 800004A0 */  mfc0       $t0, $10                  # save EntryHi
/* 10A4 800004A4 */  mtc0       $a0, $0                   # Index
/* 10A8 800004A8 */  mtc0       $a1, $5                   # PageMask
/* 10AC 800004AC */  mtc0       $a2, $10                  # EntryHi
/* 10B0 800004B0 */  beq        $a3, $t7, .L800004C8
/* 10B4 800004B4 */   addi      $t6, $zero, 1
/* 10B8 800004B8 */  srl        $t5, $a3, 6
/* 10BC 800004BC */  or         $t5, $t5, $t9
/* 10C0 800004C0 */  b          .L800004CC
/* 10C4 800004C4 */   mtc0      $t5, $2
.L800004C8:
/* 10C8 800004C8 */  mtc0       $t6, $2
.L800004CC:
/* 10CC 800004CC */  beq        $t8, $t7, .L800004E0
/* 10D0 800004D0 */   mtc0      $t6, $3
/* 10D4 800004D4 */  srl        $t5, $t8, 6
/* 10D8 800004D8 */  or         $t5, $t5, $t9
/* 10DC 800004DC */  mtc0       $t5, $3
.L800004E0:
/* 10E0 800004E0 */  tlbwi
/* 10E4 800004E4 */  nop
/* 10E8 800004E8 */  nop
/* 10EC 800004EC */  nop
/* 10F0 800004F0 */  nop
/* 10F4 800004F4 */  mtc0       $t0, $10                  # restore EntryHi
/* 10F8 800004F8 */  jr         $ra
/* 10FC 800004FC */   nop
.end STVM_MapTLB
