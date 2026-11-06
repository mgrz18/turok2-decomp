# turok2-decomp

[![status](https://img.shields.io/badge/status-matching%20decomp-blue)](https://github.com/mgrz18/turok2-decomp/milestones)
[![ROM](https://img.shields.io/badge/ROM-rebuilds%20byte--exact-brightgreen)](docs/MATCHING.md)
[![total matched](https://img.shields.io/badge/total%20matched-100%25-brightgreen)](tools/progress.py)
[![engine matched](https://img.shields.io/badge/engine%20matched-100%25-brightgreen)](tools/progress.py)
[![VM modules matched](https://img.shields.io/badge/VM%20modules%20matched-100%25-brightgreen)](tools/progress.py)
[![readable](https://img.shields.io/badge/readable%20C-99.83%25-blue)](tools/progress.py)
[![asm decoded](https://img.shields.io/badge/asm%20decoded-95.2%25-brightgreen)](docs/VRAM-LAYOUT.md)
[![functions](https://img.shields.io/badge/functions-13%2C275-brightgreen)](docs/VRAM-LAYOUT.md)
[![RSP ucode](https://img.shields.io/badge/RSP%20ucode-stock%20F3DEX-success)](docs/MICROCODE.md)
[![built with](https://img.shields.io/badge/built%20with-Claude%20Opus%205-D97757)](https://claude.com/claude-code)
[![license](https://img.shields.io/badge/license-CC0--1.0-lightgrey)](LICENSE)

Work-in-progress **matching decompilation** of **Turok 2: Seeds of Evil** (Nintendo 64, US 1.0): C source that compiles back to the exact original ROM.

> **Status: all the C is matched.** The ROM rebuilds byte-exact (`make verify` passes the SHA1), and the compiler is identified: engine code matches through SN64 GCC 2.8.1 `cc1 -O2 -mno-fix4300` + `asn64`. Every function that was C in the original is now matched C linked into the ROM: 2,531 engine functions and 1,311 functions of the paged VM modules, which build with the same toolchain (100% of the C-origin code; 99.07% of all code bytes, the rest being hand-written asm, below). 16 of them are FAKEMATCH, documented in their files and in [docs/MATCHING.md](docs/MATCHING.md). The VM modules' second copy (81 KB, the first copy's bytes again, which cannot be C of its own) is left out of both sides of that figure. 99.83% of the C-origin code is also *readable* C, with named fields rather than m2c's offset macros ([docs/MATCHING.md](docs/MATCHING.md)); `tools/progress.py` tracks the share. The matched badges are over C-origin code: the 9 KB that is hand-written asm by origin (libultra's `.s` files, SN64's boot and TLB code, the RNC unpacker, the exception stubs) and three dead fragments can never be C, so `tools/progress.py` lists them in their own `asm (by origin)` row ([docs/ASM_ORIGIN.md](docs/ASM_ORIGIN.md)) and the badges reach 100% when all the C is matched; its `all code` row (`--badge --raw`) keeps the share of every byte.
>
> **Next: a native PC port built from this source**, in the line of Ship of Harkinian, 2 Ship 2 Harkinian, Starship, SpaghettiKart and Ghostship. Before that, the code has to be readable end to end: named functions, globals and structs. See the [roadmap](#roadmap).

## Why

This decomp is a stepping stone. The end goals are:

1. **A native PC port built from the decompiled source** (macOS on Apple Silicon first, then Windows and Linux), in the same class as the source ports of Ocarina of Time, Majora's Mask, Star Fox 64, Mario Kart 64 and Super Mario 64: the game's own C compiled for the host, with the N64 hardware layer (graphics, audio, input) replaced, assets extracted at runtime from the user's ROM, and modern enhancements (widescreen, high frame rate, mouse-look, mods).
2. Reusing the work to port **Armorines: Project S.W.A.R.M.**, which Acclaim Studios London built on the Turok 2 engine.
3. Stretch goal: Android.

If you only want to play Turok 2, an emulator is 100× less work and totally fine. This project exists for native ports, modding (HD textures, real widescreen, 60 FPS), and preservation.

## Engine

**Turok engine** (Iguana Entertainment / Acclaim Studios Austin) — descended from Turok 1's, and reused by Turok 3, Turok: Rage Wars, and by Acclaim Studios London for Armorines.

## Required input

- `baserom.us.z64` — ROM dump from your own cartridge. **Not committed**, never will be.
  - Expected SHA1: `fb0400f21e3f043939ab56500c7b12a3231006f1`
  - Internal name: `Turok 2: Seeds of Ev`, cartridge ID `NT2E`, 32 MB.

## Setup (Apple Silicon macOS)

On an x86_64 Linux host the same steps run without containers: install
`binutils-mips-linux-gnu`, `cpp`, `dos2unix` and the i386 C runtime
(`libc6:i386`), and `cc1` runs natively, far faster than emulated.

```bash
# 1. Clone reference projects (gitignored, not part of this repo)
mkdir -p references && cd references
git clone --recursive https://github.com/Drahsid/turok3.git
git clone https://github.com/Drahsid/LibTEngine.git
cd ..

# 2. Pull SN64 toolchain + LibTEngine headers into the working tree
./scripts/setup-from-references.sh

# 3. Disasm the ROM. splat is pure Python and runs natively on arm64 in ~7s —
#    no container needed for this step.
python3 -m venv .venv && ./.venv/bin/pip install -r tools/requirements.txt
make setup PYTHON=./.venv/bin/python

# 4. The recovered function entry points (versions/symbols_from_scan.us.txt,
#    undefined_funcs_extra.us.txt) are committed; re-derive them only when
#    changing the seeding: ./.venv/bin/python tools/function_seed.py --write

# 5. Build the linux/amd64 container. Only the assemble/link path needs it:
#    cc1 is an i386 ELF and asn64.exe is Win32.
docker build --platform=linux/amd64 -t turok2-build .

# 6. Assemble and link
docker run --platform=linux/amd64 --rm -v "$PWD":/work turok2-build \
    bash -c "make rom C_FILES="

```

Progress is measured with `./.venv/bin/python tools/metrics.py`, which reports decode
and symbol coverage rather than matched-function counts.

## Standing on the shoulders of

- **[Drahsid/turok3](https://github.com/Drahsid/turok3)** — in-progress decomp of Turok 3 (same engine). Foundation for the official Turok 3 Remastered.
- **[AbuN64/ragewars-decomp](https://github.com/AbuN64/ragewars-decomp)** — matching decomp of Turok: Rage Wars, the same engine a year later. Hundreds of Turok 2 functions compile from its C unchanged; those files carry its C, cut down to the one function and with Turok 2's symbol names (`tools/xmatch_ref.py`, `tools/import_ref.py`), and say so in their header. Hundreds more are *near* copies: the same code over structs laid out differently (Turok 2's level actor is 0x288 bytes, Rage Wars' 0x2E8) or with other constants; those were retyped by hand to Turok 2's layouts.
- **[thejunkman9000/Turok-Rage-Wars-Decomp](https://github.com/thejunkman9000/Turok-Rage-Wars-Decomp)** — another Rage Wars decomp, with a native PC port; Turok 2 functions taken from its C the same way say so in their header.
- **[Drahsid/LibTEngine](https://github.com/Drahsid/LibTEngine)** — reverse-engineered structs and function signatures for the Turok 2 / Turok 3 engine. Major head start.
- **[libultraship](https://github.com/Kenix3/libultraship)** (Harbour Masters) — the platform layer (Fast3D renderer, audio, input, resource archives) behind Ship of Harkinian, 2 Ship 2 Harkinian, Starship, SpaghettiKart and Ghostship; the leading candidate for this port's platform layer, and the model for it either way.
- **[Mr-Wiseguy/N64Recomp](https://github.com/Mr-Wiseguy/N64Recomp)** + **[RT64](https://github.com/rt64/rt64)** — static recompilation; tried early here (see the shelved track below).
- **[ethteck/splat](https://github.com/ethteck/splat)** — disassembler / segment splitter.
- **[matt-kempster/m2c](https://github.com/matt-kempster/m2c)** — MIPS → C decompiler for matching.
- **[simonlindholm/decomp-permuter](https://github.com/simonlindholm/decomp-permuter)** — randomized C rewrites for near-matches (`tools/permuter/`).
- **[Ghidra](https://github.com/NationalSecurityAgency/ghidra)** — analysis and readable decompilation for understanding systems (HUD, camera, player) before they are matched. Every name, prototype, struct and comment lives in the repo (`versions/names.us.txt`, `tools/ghidra/annotations.json`, `include/structs/`) and is re-applied to the Ghidra project by `tools/ghidra/`, so the project is a disposable working copy.

## Roadmap

The goal is a **native PC port built from the decompiled source**. The order is
deliberate: a byte-exact decomp first, then code that is readable end to end,
then assets and a 64-bit build, then the port. Each step is what makes the next
one possible.

### Phase 1 — Bootstrap (complete)
- [x] Confirm ROM dump (SHA1 verified)
- [x] Toolchain working (SN64 binaries; splat runs natively on arm64)
- [x] First splat pass produces asm + linker script
- [x] Entry point confirmed at `0x80000400` (`romMain`)

### Phase 2 — Microcode triage (complete)
- [x] Dump RSP `ucode_text` / `ucode_data` and diff against stock
- [x] **Verdict: stock `F3DEX.NoN fifo 2.05` + `aspMain`.** No custom RSP code, so
      a stock F3DEX renderer (such as Fast3D) can draw the game's display lists

### Phase 3 — Segmentation and symbols (complete)
- [x] Recover the memory map: code in useg (TLB-mapped), data and libultra in KSEG0
- [x] 95.2% of the ROM decoded, 13,275 functions delimited
- [x] ELF links and the ROM rebuilds byte-exact from asm: `make verify` passes the SHA1
- [ ] Locate the `virtual` segment's real start ([#23](https://github.com/mgrz18/turok2-decomp/issues/23))

### Phase 4 — Matching decomp (complete)
- [x] Compiler identified: SN64 GCC 2.8.1 `cc1 -O2`
- [x] Per-function check against the ROM (`tools/match_func.py`) and a progress metric (`tools/progress.py`)
- [x] Bulk matching from m2c drafts, hand matching, decomp-permuter batches
- [x] Functions own their `.rdata` slice from C: switch tables and float pools
- [x] **All C-origin code matched**: 2,531 engine functions + 1,311 VM-module functions, `make verify` passes
- [x] 99.83% readable C (named struct fields, no m2c offset macros)

### Phase 5 — Readable code (in progress)
What a source port needs: code a person can read and change without breaking it.
- [x] Shared struct headers in `include/structs/` (offsets checked with `tools/check_structs.py`)
- [ ] Hoist the remaining repeated local structs (291 names; [docs/STRUCTS_TODO.md](docs/STRUCTS_TODO.md))
- [ ] Name the functions: 3,823 of the 4,146 C functions named (92%; 3,932 names with the asm ones), from Turok 3's and Rage Wars' decomps, libultra, the ROM's strings and SN64 debug records (`versions/names.us.txt`, `versions/names/`, `tools/apply_names.py`; [docs/NAMES.md](docs/NAMES.md))
- [ ] Name the globals (about 2,000 `D_80XXXXXX`)
- [ ] De-fake the 16 FAKEMATCH functions ([docs/MATCHING.md](docs/MATCHING.md))
- [ ] Recover the original source-file boundaries and group functions into real modules
- [ ] libultra from [decompals/ultralib](https://github.com/decompals/ultralib) (`libgultra`)

### Phase 6 — Assets
- [ ] Map every asset in the ROM: models, textures, levels, animations, sounds, music, text
- [ ] Extractor that builds a resource archive from the user's ROM (the port ships no game data)
- [ ] Code loads assets through the archive instead of fixed ROM offsets

### Phase 7 — Native build
- [ ] Builds with a modern compiler (Clang/GCC) alongside the matching build
- [ ] 64-bit clean: pointer-size assumptions in structs, fixed `0x80…` addresses, the TLB-paged VM modules linked as ordinary code
- [ ] Platform layer: renderer for the F3DEX display lists, audio mixer in place of the N64 synthesizer, input, saves (libultraship is the leading candidate)
- [ ] First window opens; title screen; first level playable

### Phase 8 — Enhancements
- [ ] Widescreen, high frame rate, mouse-look and keyboard
- [ ] Mod support (texture packs, settings menu)
- [ ] Windows and Linux builds; Armorines on the same base

### Shelved: recompilation track
N64Recomp was run early against the ELF and produced full C output (31.4 MB,
5,525 functions, with the jump-table fix in
[mgrz18/N64Recomp@mac-port](https://github.com/mgrz18/N64Recomp/tree/mac-port)).
It is not the goal: a recomp keeps the game an N64 program in translation, while
a source port makes it a PC program that can be changed freely. It also stalls on
the TLB (below).

## The TLB question

Turok 2 runs its code TLB-mapped: the boot stub programs a 2 MB page mapping vaddr
`0x00200000` to paddr `0x0`, and the `virtual` modules are paged in on demand by
`VMASM.S` / `VMEMORY.C`.

N64Recomp does not support TLB. A source port does not need it: with the whole
game in C, the paged modules are linked as ordinary code and the pager goes away.
That is how the TLB-using N64 games that run natively got there (GoldenEye
rebuilds a TLB-free ROM from its decomp, Perfect Dark ships as a decomp-based
port), and it is the route this project takes.

## Built with

Developed with **[Claude Code](https://claude.com/claude-code)** (Opus 5) under human
direction — reverse engineering, splat configuration, tooling, and most of the prose here.

## Legal

The ROM is not distributed here. Provide your own dump from a cartridge you own. Original game code and assets are property of the Turok IP rights holders.

The reverse-engineered work in this repository (symbol names, struct definitions, splat configs, Makefiles, scripts) is released under [**CC0-1.0**](LICENSE) — public domain. See `LICENSE` for the full text.
