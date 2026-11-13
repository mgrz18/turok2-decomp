# Quick start: play Turok 2 on the native port

What you need: **your own** Turok 2: Seeds of Evil ROM (N64, USA, `baserom.us.z64`, sha1
`fb0400f21e3f043939ab56500c7b12a3231006f1`) and this repository. The port reads the game data from
the ROM at build time (the archive) and at run time. Nothing from the ROM is in the repository.

Machines checked: macOS 27 on Apple silicon (rounds 13-14, MACOS.md sections 15-16) and Linux x86-64
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

## 2. Put your files where the game finds them (once)

```sh
D=~/Library/Application\ Support/turok2          # Linux: ~/.local/share/turok2
mkdir -p "$D"
cp OUT/turok2.o2r baserom.us.z64 "$D"/
```

The game looks for `turok2.o2r` and `baserom.us.z64` in that folder, then next to the executable
(in an app bundle: `Contents/Resources`), then in the current directory. `--archive FILE` and
`--rom FILE` override it. Both are needed: the ROM holds the sound banks.

Optional, a macOS app (Finder, Dock, Launchpad):

```sh
tools/port/mkapp.sh OUT/rel                       # -> OUT/Turok2.app; move it to /Applications if you like
```

The bundle holds the executable and `t2port.o2r` only. It uses Homebrew's SDL3, libzip, GLEW,
libpng and tinyxml2, so it runs on a Mac that has them (step 1). It is signed ad hoc, not
notarized: the first launch may need right-click > Open.

## 3. Play

```sh
OUT/rel/app/turok2                               # or open OUT/Turok2.app
```

With no arguments (or `--play`) the game opens a 640x480 window, plays the sound and runs in
real time (the N64's 60 fields a second, 30 drawn frames) until you close the window.
`--play --no-audio` plays without sound.

- **Saves**: Turok 2 saves to its Controller Pak, the file `turok2_pak1.mpk` in the folder of
  step 2. Save at a save portal in the game, quit, start again and load from the title menu.
  `--pak-dir DIR` keeps the pak elsewhere.
- **Settings** go to `turok2.cfg.json` in the same folder: the window size (`Window.Width`,
  `Window.Height`; default 640x480), fullscreen (`Window.Fullscreen.Enabled`, default off; **F11**
  toggles it), vsync (`CVars.gVsyncEnabled`, default on), the sound backend
  (`Window.AudioBackend`: `coreaudio`, the macOS default, `sdl` or `null`), the mouse
  (`CVars.gT2MouseSensitivity`, default 2.0, `CVars.gT2MouseInvertY`, default 0) and the
  controller mappings (`CVars.gControllers`). Delete the file to get the defaults back.
- **Logs** go to `logs/` in the same folder: `turok2-stdout.log`, `turok2-stderr.log` (the
  port's messages) and `turok2.log` (libultraship's). Nothing is printed in the terminal except
  where the logs are.
- **Sound**: CoreAudio on macOS. If it cannot open, the game tries SDL's audio, then plays
  without sound (and says so in `turok2-stderr.log`); the next launch tries your choice again.

## 4. Controls

A gamepad is used when SDL knows it (Xbox, PlayStation, Switch Pro and most current pads); it can
be plugged in or out while the game runs. Turok 2's N64 layout (stick = look, C buttons = walk)
becomes a twin-stick layout:

| action (N64 button) | gamepad | keyboard and mouse |
|---|---|---|
| walk forward / back, strafe (C-up / C-down, C-left / C-right) | left stick | W S A D |
| turn and look (the stick) | right stick | mouse (once captured) or the arrow keys |
| fire (Z) | right trigger | left mouse button or Ctrl |
| jump (R) | left trigger or right shoulder | Space or right mouse button |
| weapon wheels (hold A or B, pick with the look stick) | A / B (south / east) | Q / E |
| menu confirm / back (A / B) | A / B | Enter / E |
| pause, menus (START) | Start | Enter or Esc |
| map (L) | left shoulder or Back/Select | Tab |
| D-pad | D-pad | T F G H |

- **The mouse**: click in the window to capture it (the cursor hides and the mouse turns and
  looks); **F2** releases and captures it again. The click that captures does not fire.
- **Fullscreen**: F11.
- Any binding can be changed in `turok2.cfg.json` (`CVars.gControllers`). A config written by an
  older build keeps its old mappings (left stick = look): delete the file to get these.
- There is no rumble.

## 5. First run checklist

1. `OUT/rel/app/turok2 --bogus` prints the usage: the executable runs (the libraries are found).
2. `turok2.o2r` and `baserom.us.z64` are in `~/Library/Application Support/turok2` (step 2).
   If one is missing, the game says which one (a dialog when started from Finder) and stops.
3. Start `OUT/rel/app/turok2` (or the app). The logos, the intro and the title
   come up with sound. No sound: check `logs/turok2-stderr.log` and the macOS output device.
4. Press Start (gamepad) or Enter, then A or Enter through the menus: a new game starts in the
   Port of Adia.
5. Gamepad not reacting: `logs/turok2-stdout.log` lists it (`controls: gamepad ...`); if it is
   not there, SDL does not know it: put a `gamecontrollerdb.txt` (SDL's community mapping file)
   in the folder of step 2.
6. To save: walk into a save portal, confirm. Quit, start again, Load Game from the title.

## 6. If something goes wrong

- A crash: run the same command under `lldb -o run -k "bt 25" -- OUT/rel/app/turok2 --play` and
  report the backtrace with the logs (STABILITY.md).
- The debug switches (`T2_*` environment variables, `--input`, `--pad-test`, `--no-window`,
  `--offscreen-gl`, `--frames`) are for the automated runs in STATUS.md, not for playing.
  Without `--play` (or with any other argument) the executable is the test tool: it runs 3
  frames unless `--frames N` says otherwise.
