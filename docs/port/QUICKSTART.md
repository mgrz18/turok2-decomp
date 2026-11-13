# Quick start: play Turok 2 on the native port

What you need: **your own** Turok 2: Seeds of Evil ROM (N64, USA, `baserom.us.z64`, sha1
`fb0400f21e3f043939ab56500c7b12a3231006f1`) and this repository. The port reads the game data from
the ROM at build time (the archive) and at run time. Nothing from the ROM is in the repository.

Machines checked: macOS 27 on Apple silicon (round 13, MACOS.md section 15) and Linux x86-64
(STATUS.md). The commands below are for the Mac; on Linux, drop `brew` and use your distribution's
packages.

## 1. Build once

```sh
brew install cmake ninja sdl3 libzip glew libpng nlohmann-json tinyxml2 python@3.13
python3.13 -m venv .venv && .venv/bin/pip install pyelftools 'clang==21.*'
cp /path/to/your/rom.z64 baserom.us.z64

# the game data archive from the ROM (~10 s; any zlib gives the same units)
.venv/bin/python tools/assets/mkarchive.py baserom.us.z64 -o OUT/turok2.o2r -j 4

# the matching N64 build's ELF: gendata reads the game's data layout from it. Build it with
# `make setup && make build` on a Linux box with the N64 toolchain (BUILD.md) and copy
# build/turok2.us.elf here. It does not need to be rebuilt unless src/ changes.
cp /path/to/turok2.us.elf build/

# a Release build (-O2): the game library, the generated data, the executable (~1 min on an M5)
cmake -S port -B OUT/rel -G Ninja -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ \
      -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_FLAGS_RELEASE="-O2 -DNDEBUG" \
      -DCMAKE_CXX_FLAGS_RELEASE="-O2 -DNDEBUG"
ninja -C OUT/rel turok2_game t2runtime t2audio
.venv/bin/python -I tools/port/gendata.py --build OUT/rel --cache OUT/decls_rel.json -j 8
cmake OUT/rel && ninja -C OUT/rel turok2
```

## 2. Play

```sh
OUT/rel/app/turok2 --archive OUT/turok2.o2r --rom baserom.us.z64 \
    --game --draw --audio --pace --frames 100000000
```

- `--game --draw` runs the game and shows it in a window; `--audio` plays the sound; `--pace`
  holds the N64's 60 fields a second (without it the game runs as fast as the machine allows).
- `--frames N` stops after N fields (the default is 3, for tests): 100,000,000 is about 19 days.
  Closing the window also quits.
- Saves go to the Controller Pak file `turok2_pak1.mpk` in `~/Library/Application Support/turok2`
  (Linux: `~/.local/share/turok2`). `--pak-dir DIR` keeps it somewhere else.

## 3. Controls

A gamepad is used when SDL finds one (most current pads are known). Turok 2's N64 layout becomes
a twin-stick layout:

| N64 | gamepad | keyboard |
|---|---|---|
| stick (turn, look) | left stick | W A S D |
| C-up / C-down (walk) and C-left / C-right (strafe) | right stick | arrow keys |
| Z (fire) | left trigger | Z |
| R (jump) | right trigger | R |
| A / B (weapon wheel) | A (south) / B (east) | X / C |
| L (map) | left shoulder | E |
| START (pause, menus) | Start | Space |
| D-pad (menus) | D-pad | T F G H |

The mapping can be changed in the port's settings and is kept in `turok2.cfg.json`. There is no
rumble.

## 4. If something goes wrong

- The window stays black and the log says the archive or the ROM was not found: pass absolute
  paths to `--archive` and `--rom`.
- A crash: run the same command under `lldb -o run -k "bt 25" -- ...` and report the backtrace
  (STABILITY.md).
- The debug switches (`T2_*` environment variables, `--input`, `--no-window`, `--offscreen-gl`)
  are for the automated runs in STATUS.md, not for playing.
