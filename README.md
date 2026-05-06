# turok2-decomp

Work-in-progress decompilation of **Turok 2: Seeds of Evil** (Nintendo 64, US 1.0).

> ⚠️ **Status: bootstrap.** No matched code yet. Workspace is being set up.

## Why

This decomp is a stepping stone. The end goals are:

1. A native macOS (Apple Silicon) port via [N64Recomp](https://github.com/Mr-Wiseguy/N64Recomp) + [RT64](https://github.com/rt64/rt64).
2. Reusing ~70% of the work to port **Armorines: Project S.W.A.R.M.**, which shares the Acclaim Studios London engine.
3. Stretch goal: Android via RT64's Vulkan backend.

If you only want to play Turok 2, an emulator is 100× less work and totally fine. This project exists for native ports, modding (HD textures, real widescreen, 60 FPS), and preservation.

## Engine

**Acclaim Studios London engine** — also used by Turok 3, Armorines, Re-Volt, Forsaken, parts of NBA Jam.

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

# 3. Build the linux/amd64 container (needed because cc1 is i386 ELF
#    and asn64.exe is Win32 — Apple Silicon runs both via Rosetta-for-Linux)
brew install orbstack   # or Docker Desktop with Rosetta-for-Linux enabled
docker build --platform=linux/amd64 -t turok2-build .

# 4. Disasm the ROM
docker run --platform=linux/amd64 --rm -it -v "$PWD":/work turok2-build \
    bash -c "pip install -r tools/requirements.txt && make setup"
```

## Standing on the shoulders of

- **[Drahsid/turok3](https://github.com/Drahsid/turok3)** — in-progress decomp of Turok 3 (same engine). Foundation for the official Turok 3 Remastered.
- **[Drahsid/LibTEngine](https://github.com/Drahsid/LibTEngine)** — reverse-engineered structs and function signatures for the Turok 2 / Turok 3 engine. Major head start.
- **[Mr-Wiseguy/N64Recomp](https://github.com/Mr-Wiseguy/N64Recomp)** + **[N64ModernRuntime](https://github.com/Mr-Wiseguy/N64ModernRuntime)** + **[RT64](https://github.com/rt64/rt64)** — the native-port pipeline.
- **[ethteck/splat](https://github.com/ethteck/splat)** — disassembler / segment splitter.
- **[matt-kempster/m2c](https://github.com/matt-kempster/m2c)** — MIPS → C decompiler for matching.

## Roadmap

### Phase 1 — Bootstrap
- [x] Confirm ROM dump (SHA1 verified)
- [ ] Initial `splat.yaml` (header, boot, main, gfx segments)
- [ ] Identify entry points and code segment size
- [ ] Dump RSP `ucode_text` / `ucode_data` and diff against stock F3DEX

### Phase 2 — Microcode triage
- [ ] Quantify delta vs F3DEX
- [ ] Decision: go/no-go on the native-port path

### Phase 3 — Matching
- [ ] Import structs from LibTEngine
- [ ] First 50–100 matched functions
- [ ] Byte-exact `.z64` build (`sha1sum build/turok2.us.z64 == sha1sum baserom.us.z64`)

### Phase 4 — Recomp
- [ ] N64Recomp on the matched ELF
- [ ] First window opens with RT64

## Legal

The ROM is not distributed here. Provide your own dump from a cartridge you own. Original game code and assets are property of the Turok IP rights holders.

The reverse-engineered work in this repository (symbol names, struct definitions, splat configs, Makefiles, scripts) is released under [**CC0-1.0**](LICENSE) — public domain. See `LICENSE` for the full text.
