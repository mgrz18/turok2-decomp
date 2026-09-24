# turok2-decomp

[![status](https://img.shields.io/badge/status-matching%20decomp-blue)](https://github.com/mgrz18/turok2-decomp/milestones)
[![ROM](https://img.shields.io/badge/ROM-rebuilds%20byte--exact-brightgreen)](docs/MATCHING.md)
[![engine matched](https://img.shields.io/badge/engine%20matched-6.59%25-orange)](tools/progress.py)
[![asm decoded](https://img.shields.io/badge/asm%20decoded-95.2%25-brightgreen)](docs/VRAM-LAYOUT.md)
[![functions](https://img.shields.io/badge/functions-13%2C275-brightgreen)](docs/VRAM-LAYOUT.md)
[![RSP ucode](https://img.shields.io/badge/RSP%20ucode-stock%20F3DEX-success)](docs/MICROCODE.md)
[![built with](https://img.shields.io/badge/built%20with-Claude%20Opus%205-D97757)](https://claude.com/claude-code)
[![license](https://img.shields.io/badge/license-CC0--1.0-lightgrey)](LICENSE)

Work-in-progress **matching decompilation** of **Turok 2: Seeds of Evil** (Nintendo 64, US 1.0): C source that compiles back to the exact original ROM.

> **Status: matching begins.** The ROM rebuilds byte-exact from split asm (`make verify` passes the SHA1), and the compiler is identified: engine code matches through SN64 GCC 2.8.1 `cc1 -O2` + `asn64`. 872 engine functions (6.59% of engine code) are matched and linked into the ROM as C so far ([docs/MATCHING.md](docs/MATCHING.md)); `tools/progress.py` tracks the share.
>
> A parallel native-port track recompiles the same ELF with N64Recomp; it runs clean, and the TLB is its open question.

## Why

This decomp is a stepping stone. The end goals are:

1. A native macOS (Apple Silicon) port via [N64Recomp](https://github.com/Mr-Wiseguy/N64Recomp) + [RT64](https://github.com/rt64/rt64).
2. Reusing the work to port **Armorines: Project S.W.A.R.M.**, which Acclaim Studios London built on the Turok 2 engine.
3. Stretch goal: Android via RT64's Vulkan backend.

If you only want to play Turok 2, an emulator is 100× less work and totally fine. This project exists for native ports, modding (HD textures, real widescreen, 60 FPS), and preservation.

## Engine

**Turok engine** (Iguana Entertainment / Acclaim Studios Austin) — descended from Turok 1's, and reused by Turok 3, Turok: Rage Wars, and by Acclaim Studios London for Armorines.

## Required input

- `baserom.us.z64` — ROM dump from your own cartridge. **Not committed**, never will be.
  - Expected SHA1: `fb0400f21e3f043939ab56500c7b12a3231006f1`
  - Internal name: `Turok 2: Seeds of Ev`, cartridge ID `NT2E`, 32 MB.

## Setup (Apple Silicon macOS)

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

# 4. Recover function entry points from the ROM and re-split with them
./.venv/bin/python tools/function_seed.py --write
make setup PYTHON=./.venv/bin/python

# 5. Build the linux/amd64 container. Only the assemble/link path needs it:
#    cc1 is an i386 ELF and asn64.exe is Win32.
docker build --platform=linux/amd64 -t turok2-build .

# 6. Assemble and link
docker run --platform=linux/amd64 --rm -v "$PWD":/work turok2-build \
    bash -c "make rom C_FILES="

# 7. Recompile to C. Use the mac-port branch of mgrz18/N64Recomp: upstream
#    assumes a jump table shares its function's section and crashes here.
path/to/N64Recomp turok2.us.toml
```

Progress is measured with `./.venv/bin/python tools/metrics.py`, which reports decode
and symbol coverage rather than matched-function counts.

## Standing on the shoulders of

- **[Drahsid/turok3](https://github.com/Drahsid/turok3)** — in-progress decomp of Turok 3 (same engine). Foundation for the official Turok 3 Remastered.
- **[Drahsid/LibTEngine](https://github.com/Drahsid/LibTEngine)** — reverse-engineered structs and function signatures for the Turok 2 / Turok 3 engine. Major head start.
- **[Mr-Wiseguy/N64Recomp](https://github.com/Mr-Wiseguy/N64Recomp)** + **[N64ModernRuntime](https://github.com/Mr-Wiseguy/N64ModernRuntime)** + **[RT64](https://github.com/rt64/rt64)** — the native-port pipeline.
- **[ethteck/splat](https://github.com/ethteck/splat)** — disassembler / segment splitter.
- **[matt-kempster/m2c](https://github.com/matt-kempster/m2c)** — MIPS → C decompiler for matching.

## Roadmap

The goal is readable C that rebuilds the original ROM byte for byte. That
source is what makes real modding possible, and it is also the way out of the
TLB for a native port: GoldenEye's and Perfect Dark's ports both stand on a
decomp.

### Phase 1 — Bootstrap (complete)
- [x] Confirm ROM dump (SHA1 verified)
- [x] Toolchain working (SN64 binaries; splat runs natively on arm64)
- [x] First splat pass produces asm + linker script
- [x] Entry point confirmed at `0x80000400` (`romMain`)

### Phase 2 — Microcode triage (complete)
- [x] Dump RSP `ucode_text` / `ucode_data` and diff against stock
- [x] **Verdict: stock `F3DEX.NoN fifo 2.05` + `aspMain`.** No custom RSP code, so
      RT64 needs no microcode work — go on the native-port path

### Phase 3 — Segmentation and symbols (in progress)
- [x] Recover the memory map: code in useg (TLB-mapped), data and libultra in KSEG0
- [x] 95.2% of the ROM decoded, 13,275 functions delimited
- [x] ELF links and the ROM rebuilds byte-exact from asm: `make verify` passes the SHA1
- [ ] Locate the `virtual` segment's real start ([#23](https://github.com/mgrz18/turok2-decomp/issues/23))

### Phase 4 — Matching decomp (in progress)
- [x] ROM rebuilds byte-exact from asm (`make verify`)
- [x] Compiler identified: SN64 GCC 2.8.1 `cc1 -O2`, confirmed on 8 engine functions
- [x] Per-function check against the ROM: `tools/match_func.py`
- [x] Matched C linked into the ROM (`make verify` still passes), with a progress metric
- [x] Fast C path: GNU as reproducing asn64 (`tools/sn_as.sh`), a full build in seconds
- [x] Bulk matching from m2c drafts (`tools/auto_match.py`): 840 functions over three passes
- [ ] Readability pass: name the structs behind `M2C_FIELD` accesses
- [ ] Hand-match the functions m2c gets close to but not exact
- [x] Functions own their `.rdata` slice from C: switch tables and float pools (`--switch`, `--rdata`)
- [ ] Recover the original source-file boundaries
- [ ] libultra from [decompals/ultralib](https://github.com/decompals/ultralib) (`libgultra`)
- [ ] Settle C vs C++: sources are `.CPP`; C-style code matches with `cc1`

### Phase 5 — Native-port track: recomp (in progress)
- [x] N64Recomp runs against the ELF
- [x] Full C output without errors: 31.4 MB of C, 5,525 functions, every sized
      function in the ELF (needs the jump-table fix in
      [mgrz18/N64Recomp@mac-port](https://github.com/mgrz18/N64Recomp/tree/mac-port))
- [ ] TLB: the engine pages code through the TLB and N64Recomp does not support it
      yet — see the note below
- [ ] First window opens with RT64

### Phase 6 — Input
- [ ] Keyboard and mouse, with a patch on the aim path for real mouse-look

## The TLB question

Turok 2 runs its code TLB-mapped: the boot stub programs a 2 MB page mapping vaddr
`0x00200000` to paddr `0x0`, and the `virtual` modules are paged in on demand by
`VMASM.S` / `VMEMORY.C`.

N64Recomp does not support TLB — its README lists relocations for TLB mapping as
planned. Every TLB-using N64 game that runs natively today got there through a full
decompilation: GoldenEye rebuilds a TLB-free ROM from a dedicated decomp branch, and
Perfect Dark ships as a decomp-based port. Neither route is open here, so this is the
project's real unknown.

## Built with

Developed with **[Claude Code](https://claude.com/claude-code)** (Opus 5) under human
direction — reverse engineering, splat configuration, tooling, and most of the prose here.

## Legal

The ROM is not distributed here. Provide your own dump from a cartridge you own. Original game code and assets are property of the Turok IP rights holders.

The reverse-engineered work in this repository (symbol names, struct definitions, splat configs, Makefiles, scripts) is released under [**CC0-1.0**](LICENSE) — public domain. See `LICENSE` for the full text.
