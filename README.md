# turok2-decomp

[![status](https://img.shields.io/badge/status-native%20port%20(phase%207%20closed)-blue)](#roadmap)
[![next](https://img.shields.io/badge/goal-native%20PC%20source%20port-8A2BE2)](#roadmap)
[![ROM](https://img.shields.io/badge/ROM-rebuilds%20byte--exact-brightgreen)](docs/MATCHING.md)
[![total matched](https://img.shields.io/badge/total%20matched-100%25-brightgreen)](tools/progress.py)
[![engine matched](https://img.shields.io/badge/engine%20matched-100%25-brightgreen)](tools/progress.py)
[![VM modules matched](https://img.shields.io/badge/VM%20modules%20matched-100%25-brightgreen)](tools/progress.py)
[![readable](https://img.shields.io/badge/readable%20C-99.83%25-blue)](tools/progress.py)
[![asm decoded](https://img.shields.io/badge/asm%20decoded-95.2%25-brightgreen)](docs/VRAM-LAYOUT.md)
[![functions](https://img.shields.io/badge/functions-13%2C275-brightgreen)](docs/VRAM-LAYOUT.md)
[![functions named](https://img.shields.io/badge/C%20functions%20named-92%25-blue)](docs/NAMES.md)
[![globals named](https://img.shields.io/badge/globals%20named-1%2C204-blue)](docs/NAMES.md#round-g2-the-globals)
[![RSP ucode](https://img.shields.io/badge/RSP%20ucode-stock%20F3DEX-success)](docs/MICROCODE.md)
[![built with](https://img.shields.io/badge/built%20with-Claude%20Opus%205-D97757)](https://claude.com/claude-code)
[![license](https://img.shields.io/badge/license-CC0--1.0-lightgrey)](LICENSE)

Work-in-progress **matching decompilation** of **Turok 2: Seeds of Evil** (Nintendo 64, US 1.0): C source that compiles back to the exact original ROM.

> **Status: all the C is matched.** The ROM rebuilds byte-exact from C (`make verify` passes the SHA1): 2,531 engine functions and 1,311 functions of the paged VM modules, built with SN64 GCC 2.8.1 (`cc1 -O2 -mno-fix4300`) + `asn64`. 51 files are marked FAKEMATCH, each documented (round 21's audit found 58 unmarked fakes and de-faked 25 functions; [docs/MATCHING.md](docs/MATCHING.md)), and 99.83% is readable C with named fields. The badges count C-origin code; the 9 KB of hand-written asm (libultra's `.s` files, SN64's boot and TLB code, the RNC unpacker, the exception stubs) is tracked apart ([docs/ASM_ORIGIN.md](docs/ASM_ORIGIN.md)).
>
> **Native PC port (Phase 7, closed):** the same C runs as a 64-bit program on libultraship, on Linux and macOS. A new game plays through to the good ending, frame for frame the same on both hosts, and a self-contained Mac app is ready for its first session in a real window. See [docs/port/STATUS.md](docs/port/STATUS.md) and [docs/port/QUICKSTART.md](docs/port/QUICKSTART.md).

## Screenshots

The native PC port (Phase 7) running the game's own decompiled C on libultraship, captured from the port's framebuffer:

| | |
|:-:|:-:|
| <img src="docs/media/pc_title.png" alt="Title screen on the PC port" width="400"> | <img src="docs/media/pc_legal.png" alt="Legal screen on the PC port" width="400"> |
| Title screen (mode 7, frame 4214) | Legal screen (mode 1) |
| <img src="docs/media/pc_expansion_pak.png" alt="Expansion Pak screen with the 3D pak model" width="400"> | <img src="docs/media/pc_pak_menu.png" alt="Controller Pak menu" width="400"> |
| Expansion Pak screen, a 3D model drawn by Fast3D (mode 3) | Controller Pak menu (mode 0) |

Assets decoded from the ROM by `tools/assets/` (Phase 6), with no game data in the repository:

| | | |
|:-:|:-:|:-:|
| <img src="docs/media/asset_model_posed.png" alt="Animated model posed with its skeleton" width="220"> | <img src="docs/media/asset_model_textured.png" alt="Static model with textures" width="220"> | <img src="docs/media/asset_textures.png" alt="Texture sample" width="260"> |
| Animated model, posed with its skeleton | Static model, textured | Textures (CI4/CI8 with palettes) |

<p align="center"><img src="docs/media/asset_level_map.png" alt="Top-down map of a level" width="640"><br>A level's collision regions and actor placements, top down</p>

Screenshots of copyrighted game content, shown to document the project's progress. The images are generated from a ROM the user provides; the ROM itself is never included.

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
# 0. The libultra sources the build compiles: decompals/ultralib, a pinned
#    submodule (clone with --recursive, or:)
git submodule update --init

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
- [x] Locate the `virtual` segment's real start: ROM 0x14A000 = vaddr 0x400000, from `VM_Initialize`'s page region; `gVirtualRomStart`, module 1 is `gVirtualModule1Size` = 0x3CAE0 bytes ([SEGMENTS.md](docs/SEGMENTS.md#the-virtual-segments-real-start-round-21))

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
- [ ] Hoist the remaining repeated local structs: 148 names in 2+ files (291 before round L, 228 before round 21, 172 before round 22); host-view twins 137 -> 99 -> 63 (the swoosh, M04 sound, collision-response, turret and level-record families done; left: FAKEMATCH files, field-symbol views, animation views; `tools/hoist_structs.py`, `tools/unview.py`; [docs/STRUCTS_TODO.md](docs/STRUCTS_TODO.md))
- [ ] Name the functions: 4,058 of the 4,146 C functions named (4,187 names with the asm ones); 88 C `func_` left: empty or constant stubs, setters/getters and type or flag tests of unknown structs, sector sums, unclear Rage Wars imports, and five whose real name changes the code (`cosf`, `sinf`, `sqrtf`, `memcpy` are GCC builtins: renaming changes 88 files for `sqrtf`, 9 for `cosf`/`sinf`; `main`) ([docs/NAMES.md](docs/NAMES.md#left-as-func_-round-21))
- [ ] Name the globals: 1,676 data names (`versions/names/data_*.us.txt`); 129 `D_` left in the C (11 labels in code, 10 VM data, 52 pool words kept for a reason, 64 interiors of named BSS objects that want their struct's members; round 22 made 3 of them members) and 4 absolute ROM offsets ([docs/NAMES.md](docs/NAMES.md#round-21-part-j2-the-d_-left-in-the-c))
- [x] Float and string pools written as C literals: 471 of the 1,076 `D_` symbols the C read are gone, each converted file owning its `.rodata` slice (849 subsegments); checked with `tools/rodata_check.py` / `tools/rodata_match.py` and `make verify` ([docs/MATCHING.md](docs/MATCHING.md))
- [ ] One declaration with the real type for every global: 48 headers in `include/globals/`; local `extern`s of named globals 5,269 -> 911 -> 337 -> 278 lines (248 with a `local` comment; 51 lines, 27 names, without a header: field symbols whose header a base-sharing view clashes with, one-file struct views, non-game symbols; 0 redeclare a header global without a comment). `gScheduler` (`OSSched`), `gEngineApp_m_SwooshSystem` (`CSwooshSystem`) and the audio manager's DMA state (SDK types) have their headers (`tools/hoist_externs.py`, `tools/asm_diff.py`; [docs/GLOBALS.md](docs/GLOBALS.md))
- [ ] De-fake the FAKEMATCH functions: after an honest audit (register pins, empty asm barriers, identical if/else arms), 51 files marked, 25 functions de-faked; each says why the fake works and what it misses by without it ([docs/MATCHING.md](docs/MATCHING.md))
- [x] One body for the N64 and the host where the host's compiles to the same N64 code: 496 `TARGET_PORT` alternatives dropped (`tools/unify_host_bodies.py`, re-runnable)
- [ ] Recover the original source-file boundaries: the module map is done (320 modules from the linker's object padding, ultralib's files and the two VM copies; `tools/modules.py`, [docs/MODULES.md](docs/MODULES.md)); the files are not moved yet
- [x] libultra from [decompals/ultralib](https://github.com/decompals/ultralib): the game links **2.0I `libgultra_rom`**; ultralib is a pinned submodule (`lib/ultralib`, no files copied) and **120 of the library's 173 modules are built from its own sources** (209 game files no longer linked, 121 of them deleted; `tools/ultralib_build.py`, `versions/ultralib_objects.us.txt`). The other 53 stay the game's C, each with its reason: 41 differ, 2 have another function order, 5 end their `.rodata` in stale linker bytes, 5 have `.bss` or an unplaced static of their own ([docs/LIBULTRA.md](docs/LIBULTRA.md)). The port keeps the game's C for what it links

### Phase 6 — Assets
- [x] Map every asset in the ROM: models, textures, levels, animations, sounds, music, text. 100% of the asset region (0x1E0000-0x1FF7AEC, 31.5 MB) classified, 99.7% exported to files: 2705 models, 1839 textures + 114 graphic sets, 490 skeletons / 1857 animations, 165 levels, 197 cinemas, 2 sound banks (866 waves), 1319 effects, 19 sequences, 140 streams. See [docs/assets/README.md](docs/assets/README.md)
- [x] Extractor that builds a resource archive from the user's ROM (the port ships no game data): `tools/assets/mkarchive.py baserom.us.z64 -o turok2.o2r` checks the ROM (z64/v64/n64, SHA1) and writes a libultraship `.o2r`: 26,124 load units as standard Blob resources keyed by ROM address (`t2/u/XXXXXXXX`), RNC pre-decompressed, 33.9 MB, deterministic, ~8 s; `tools/assets/archive_check.py` verifies every entry against the ROM ([docs/assets/ARCHIVE.md](docs/assets/ARCHIVE.md)). Byte order for little-endian hosts: `tools/assets/swap_spec.json`, 99.5% of unit bytes typed ([docs/assets/BYTEORDER.md](docs/assets/BYTEORDER.md)). Torch module for distribution: next
- [x] The port loads every asset through the archive instead of fixed ROM offsets (`port/assets/t2asset.c`, [docs/assets/PORT.md](docs/assets/PORT.md)); the matching build keeps its ROM offsets

### Phase 7 — Native build (closed)
The round-by-round history is in [docs/port/STATUS.md](docs/port/STATUS.md).
- [x] Builds with a modern compiler alongside the matching build: `port/` (CMake + Ninja, clang), 64-bit, without changing the matching build (`tools/asm_diff.py`: 0 differences) ([docs/port/BUILD.md](docs/port/BUILD.md))
- [x] Game data from the user's ROM: typed host-layout C generated by `tools/port/gendata.py` and the `.o2r` archive; a fresh clone builds with only the ROM ([docs/port/DATA.md](docs/port/DATA.md))
- [x] Platform layer on libultraship 2.0: Fast3D rendering, audio, controllers and an N64-compatible Controller Pak ([docs/port/LIBULTRASHIP.md](docs/port/LIBULTRASHIP.md))
- [x] The whole game playable: a new game through levels 1-6, every boss, to the good ending, with the N64's random stream and the same result on Linux and macOS ([docs/port/PLAYTHROUGH.md](docs/port/PLAYTHROUGH.md))
- [x] Checked against the N64: attract demos 1 and 3 identical to the N64's RAM; render and audio compared scene by scene ([docs/port/FIDELITY.md](docs/port/FIDELITY.md))
- [x] A self-contained `Turok2.app` for macOS ([docs/port/QUICKSTART.md](docs/port/QUICKSTART.md))
- [ ] 64-bit clean everywhere: the known pointer-width classes are fixed and guarded by checkers (`n64viewcheck.py`, `outptr.py`, `abicheck.py`); some sites remain ([docs/port/64BIT.md](docs/port/64BIT.md))
- [ ] First session in a real window with a person and a gamepad ([docs/port/FIRSTRUN.md](docs/port/FIRSTRUN.md))

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
