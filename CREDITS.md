# Credits

This project would not exist as a viable starting point without prior work
in the Acclaim Studios London engine reverse-engineering community.

## Direct dependencies (referenced locally, not redistributed here)

### [Drahsid](https://github.com/Drahsid)
- **[turok3](https://github.com/Drahsid/turok3)** — in-progress decompilation of Turok 3: Shadow of Oblivion. Foundation for the official Turok 3 Remastered. We reuse the build pipeline structure, splat config layout, and toolchain conventions.
- **[LibTEngine](https://github.com/Drahsid/LibTEngine)** — reverse-engineered structs, function signatures, and memory addresses for the Turok 2 / Turok 3 engine. ~85% complete for Turok 2.

The SN64 toolchain binaries vendored under `tools/sn64/` (and the LibTEngine struct headers under `include/types/`) are populated locally via `scripts/setup-from-references.sh` — they retain their original licensing and are not redistributed in this repository.

## Toolchain

- **[ethteck/splat](https://github.com/ethteck/splat)** — disassembler / segment splitter (MIT)
- **[matt-kempster/m2c](https://github.com/matt-kempster/m2c)** — MIPS → C
- **[simonlindholm/asm-differ](https://github.com/simonlindholm/asm-differ)** — diff visualization
- **[simonlindholm/decomp-permuter](https://github.com/simonlindholm/decomp-permuter)** — function permutation for matching
- **[Mr-Wiseguy/N64Recomp](https://github.com/Mr-Wiseguy/N64Recomp)** — static recomp (target for native ports)
- **[Mr-Wiseguy/N64ModernRuntime](https://github.com/Mr-Wiseguy/N64ModernRuntime)** — native runtime
- **[rt64/rt64](https://github.com/rt64/rt64)** — renderer with microcode handlers
- **[grumpycoders/pcsx-redux](https://github.com/grumpycoders/pcsx-redux)** — psyq-obj-parser
- **[pmret/papermario](https://github.com/pmret/papermario)** — gold-standard reference for SN64-based N64 decomp pipelines

## Acknowledgments

- **Kaiser** ([@svkaiser](https://github.com/svkaiser)) — Turok EX remasters, ecosystem knowledge.
- The N64Brew community for microcode and RDP documentation.
- ZeldaRet, n64decomp, and pmret communities for the decomp tooling that makes this kind of project tractable.
