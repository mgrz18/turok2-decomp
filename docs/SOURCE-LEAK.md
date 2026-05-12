# Turok N64 source leak — analysis and policy

Repo investigated: `https://github.com/Abhisheksinha1506/Turok-N64-Source-code`
Local clone (gitignored): `references/Turok-N64-Source-code/` (~898 MB, 11,255 files).

## TL;DR — executive summary

The repository is a **re-host of the 2018 "Turok: Dinosaur Hunter" (Turok 1, 1997,
Iguana Entertainment) source leak**. It is **NOT** the Turok 2 (Seeds of Evil) or
Armorines source. The header `readme.txt` openly says:

> "Credit to Ozidual on the N64brew discord for figuring out the encrypted
> archive's password. (...) This is the first retail N64 source code that has
> been leaked to the public. 2018/08/26"

The previous microcode-investigation agent linked this repo as a
"cross-confirmation" of the F3DEX_NoN finding. That linkage is **correct** —
Turok 1's `src/PR/tengine/tengine.c` does call `gspF3DEX_NoN_fifoTextStart`
(lines 3234-3252), and Acclaim London inherited Iguana's `tengine` codebase
for Turok 2/3, so the microcode choice carried over. But this is not the
Turok 2 source.

## Legitimacy verdict

**Real leak, real source, wrong game.** Verdict: **legitimate but only
tangentially useful**.

- Not a hoax: contains 554 `.c` files, full `libultra` subdirectories,
  N64 SDK `spec`/`PRdefs` makefiles, `dosvers/` (DOS-hosted build artifacts),
  `.o` object files committed next to `.c` (typical of leaked engineering
  trees), and per-region overlay kanji blobs. Internal comments name real
  Iguana devs ("Written by Biscuit" in `boot.c`).
- Not a watermarked honeypot: matches the historically-documented 2018 leak
  shape (compressed archive cracked by Ozidual). No author-naming watermarks
  found.
- Git history is shallow (3 commits, June 2023 initial dump + Jan 2026
  README update by user `Abhisheksinha1506`, 0 stars, 0 forks). This user
  is a re-host, not the leaker. The same content is mirrored across several
  GitHub accounts (`Turok---Nintendo-64-Source-Code` etc.).

## Coverage

| Subsystem | Present? | Notes |
|---|---|---|
| Turok 2 (Seeds of Evil) | NO | Not in this leak. |
| Turok 3 | NO | Not in this leak. |
| Armorines | NO | Not in this leak. |
| Turok: Rage Wars | NO | Not in this leak. |
| **Turok 1: Dinosaur Hunter** | **YES, full tree** | `src/PR/tengine/`, ~78 .c files, AI/coll/anim/audio/render. `tengine.c` is 4825 lines. |
| libultra subset | partial | `src/PR/libultra/{gt,gu,sched,sp}` only — game-specific helpers, not full libultra. |
| RSP microcode (sources) | NO | Only references to stock `gspF3DEX_NoN_*` symbols; no ucode `.s` in the tree. Consistent with stock SDK ucode use. |
| Tools | partial | `seqtest/`, `turoksnd/` audio tools; N64 SDK `PRdefs` makefiles. |
| Build instructions | YES, partial | `spec`, `makefile`, `dosvers/*.mak`. Targets SGI IDO + N64 SDK toolchain. No modern build instructions. |
| Compiler hints | YES | `makefile` shows `-O3 -mips2 -xansi` (IDO 5.3 flags). No SN64 references. |

## Why this still matters for Turok 2 decomp

Acclaim London **forked Iguana's `tengine` source** for Turok 2 (and reused it
in Turok 3 and Armorines). Evidence:

- Class/struct names match Isghj's Turok 2 reverse-engineering work
  (LibTEngine project, also under `references/LibTEngine/`): `CEngineApp`,
  `CGameObjectInstance`, `CInstanceHdr`, `CAIDynamic`, etc., are present
  in this Turok 1 source verbatim.
- Same ucode (`gspF3DEX_NoN`) is selected the same way.
- Same scheduler pattern (`__scMain`, `OS_SC_DP|OS_SC_SP` flags) in `sched.c`.
- Same memory pool API (`i3D_initMemoryAllocation`, `i3D_freePool`) likely
  underpins Turok 2's `CHeap`.

So this leak is **the engine ancestor of Turok 2**, not Turok 2 itself.
~3 years and ~3 game cycles of Acclaim London modifications sit between
this code and the Turok 2 ROM we are decompiling.

## How we should use it in the decomp workflow

**Reference tier: "hints only, no copy."** Treat the leak the way kernel
contributors treat leaked Windows source: read it once for orientation, then
write fresh code from the disassembly. Specifically:

1. **DO** use it to:
   - Understand the overall engine architecture (thread layout, render path,
     memory pool design, AI tick structure).
   - Identify *what* a function probably does, when m2c output is ambiguous.
   - Confirm symbol meanings (`pInstance`, `CEngineApp`) and struct field
     ordering for Turok 2's evolved versions.
   - Cross-check our libultra matching: Turok 1 used the same libultra
     subset, so its scheduler/threads are byte-similar.

2. **DO NOT**:
   - Paste any code verbatim into `src/` or `include/`. The repo is CC0,
     but the leak is **not** ours to relicense.
   - Use struct definitions byte-for-byte; re-derive them from our own
     disasm with the leak as cross-check only.
   - Commit any file from `references/Turok-N64-Source-code/` into the
     tracked tree. (`references/` is already gitignored — keep it that way.)
   - Cite the leak as primary evidence in commits; cite the disasm /
     ROM offset / RAM address instead.

3. **Workflow rule of thumb**: if a function disassembly is unclear, look
   at the Turok 1 equivalent **once**, close the file, then write the
   matching C from memory based on the disasm. This is the "clean room with
   hints" pattern that other N64 decomps (Mario 64, Zelda OoT) have used
   informally for years.

## Legal / ethical notes

- The 2018 leak source archive itself is of grey legality (leaked, not
  released). It has been publicly mirrored on GitHub since 2018 and Acclaim
  Entertainment is defunct (bankrupt 2004), so practical takedown risk is
  low — but Disney/Saber/Nightdive own current Turok IP and could in theory
  object.
- Our repo is CC0 and must remain clean-room-derivable. Anyone auditing
  `src/` should see code that is plausibly the product of disassembly +
  m2c + manual matching, with no telltale phrasing from the leak.
- The `references/` directory is gitignored and stays local. This document
  (`docs/SOURCE-LEAK.md`) is the only artifact of the leak we commit, and
  it contains only metadata and policy — no leaked code.

## Top 5 files in the leak to reference during decomp

Ranked by signal-per-read for Turok 2 matching:

1. `references/Turok-N64-Source-code/src/PR/tengine/tengine.c` (4,825 lines)
   — boot path, scheduler init, ucode selection, main game loop. Turok 2's
   equivalent will have evolved but the bones are here.
2. `references/Turok-N64-Source-code/src/PR/tengine/sched.c` — RDP/RSP task
   scheduler, message queue setup. libultra-adjacent, likely closest match
   to Turok 2.
3. `references/Turok-N64-Source-code/src/PR/tengine/memory.c` —
   `i3D_initMemoryAllocation`, pool/free/alloc. Probable ancestor of
   Turok 2's `CHeap` at `0x800D1CC0`.
4. `references/Turok-N64-Source-code/src/PR/tengine/spec` — memory map
   (code segment, audio heap, DL buffer, fifo, framebuffers, z-buffer).
   Compare directly to our `docs/SEGMENTS.md` layout.
5. `references/Turok-N64-Source-code/src/PR/tengine/ai.c` (10,590 lines) —
   AI dispatcher (`AI_Event_Dispatcher`, `AI_Single_Event`). If Turok 2
   inherited the AI event model, this saves weeks of reversal.

Also worth a single read-through: `boot.c` (the RNC2 decompression bootstrap
— Turok 2 likely uses the same compression scheme for its code segment),
and `acclmid.txt` (just an "Acclaim Entertainment, Inc." string, confirms
the Acclaim publishing chain even on Iguana's tree).

## Files committed by this investigation

- `docs/SOURCE-LEAK.md` (this file).

No source from the leak is copied into the tracked tree.
