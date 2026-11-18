# Quick start: play Turok 2 on the native port

**Have a ready-made `Turok2.app` (or its zip)?** Skip the build: section 0. Otherwise build it
yourself (sections 1-3).

What you need: **your own** Turok 2: Seeds of Evil ROM (N64, USA, `baserom.us.z64`, sha1
`fb0400f21e3f043939ab56500c7b12a3231006f1`) and this repository. The port reads the game data from
the ROM at build time (the archive) and at run time. Nothing from the ROM is in the repository.

Machines checked: macOS 27 on Apple silicon (rounds 13-16, MACOS.md sections 15-18) and Linux x86-64
(STATUS.md). The commands below are for the Mac; on Linux, drop `brew` and use your distribution's
packages, with LLVM 19's libclang (`/usr/lib/llvm-19/lib/libclang-19.so.1`, or `--libclang`)
and `pip install clang==19.1.7`, and leave out `-DCMAKE_OSX_DEPLOYMENT_TARGET`.

## 0. Install the app (no build, no Homebrew)

The app from `tools/port/mkapp.sh` (round 15) carries every library it needs (SDL3, GLEW,
libzip, tinyxml2, liblzma, libzstd in `Contents/Frameworks`): it runs on an Apple silicon Mac
with macOS 27 or later and nothing else installed. It holds no game data.

1. **Unzip** `Turok2-<version>-macos-arm64.zip` (double-click) and **drag `Turok2.app` to
   `/Applications`** (any folder works, paths with spaces too).
2. **Your game files**: make the folder and copy the ROM and the archive into it:

   ```sh
   D=~/Library/Application\ Support/turok2
   mkdir -p "$D" && cp baserom.us.z64 turok2.o2r "$D"/
   ```

   `turok2.o2r` is made once from your ROM with `tools/assets/mkarchive.py` (section 1, the
   archive line; it needs only Python). Finder: Go > Go to Folder... >
   `~/Library/Application Support`, make a folder `turok2`, drop the two files in. (They may
   also go in the app's `Contents/Resources`, but a new copy of the app would lose them. A
   bundle that carries them uses its own first (round 20): `mkapp.sh` with `T2_APP_ARCHIVE` and
   `T2_APP_ROM`, section 2.)
3. **The first launch (Gatekeeper)**: the app is signed ad hoc, not notarized by Apple, so a
   double-click on a downloaded copy says it cannot be checked. Either **right-click (or
   Control-click) the app > Open > Open**, or try once, then **System Settings > Privacy &
   Security > "Turok2" was blocked... > Open Anyway**. In a terminal instead:
   `xattr -dr com.apple.quarantine /Applications/Turok2.app`. Only the first launch asks.
   A copy you built yourself is not quarantined and opens directly.
4. **Play**: the logos, the intro and the title come up with sound in a 4:3 window centred on
   the display (round 20: the largest multiple of 320x240 points that fits, 960x720 on a 14"
   MacBook; the N64's own boot: about 70 s of logos and intro cinemas before the title). At the
   first launch a box shows the controls (section 4; **F1** shows it again, and they are in
   `logs/turok2-stdout.log`). If the ROM or the archive is missing the app says which file and
   where it looked.

Where things go: the config `turok2.cfg.json`, the logs (`logs/`) and the saves
(`turok2_pak1.mpk`) are all in `~/Library/Application Support/turok2`. To remove the game, drag
the app to the Bin and delete that folder.

## 1. Build once

Checked from zero on a fresh clone of the GitHub repository in round 16 (MACOS.md 18), and again
on a fresh clone of the round-19 tree in round 20 (MACOS.md 22): these commands, in this order, in
the clone's directory, with only the ROM added. You need
**Homebrew** (https://brew.sh; its installer also installs Apple's Command Line Tools: `clang`,
`git`) and about 2 GB of disk. The N64 toolchain is **not** needed.

```sh
git clone https://github.com/mgrz18/turok2-decomp && cd turok2-decomp
brew install cmake ninja sdl3 libzip glew libpng nlohmann-json tinyxml2 python@3.13
python3.13 -m venv .venv && .venv/bin/pip install pyelftools 'clang==21.*'
cp /path/to/your/rom.z64 baserom.us.z64          # sha1 fb0400f2... (above)

# the game data archive from the ROM (~5 s; any zlib gives the same units)
.venv/bin/python tools/assets/mkarchive.py baserom.us.z64 -o OUT/turok2.o2r -j 4

# a Release build (-O2): the game library, the generated data, the executable
cmake -S port -B OUT/rel -G Ninja -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ \
      -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_FLAGS_RELEASE="-O2 -DNDEBUG" \
      -DCMAKE_CXX_FLAGS_RELEASE="-O2 -DNDEBUG" -DCMAKE_OSX_DEPLOYMENT_TARGET=27.0
ninja -C OUT/rel turok2_game t2runtime t2audio
.venv/bin/python -I tools/port/gendata.py --build OUT/rel --cache OUT/decls_rel.json -j 8
cmake OUT/rel && ninja -C OUT/rel turok2
```

Times on an M5 (round 16): the clone 35 s; `brew install` depends on what is missing; the
first `cmake` **7.5 min** (it downloads libultraship, ImGui and the other dependencies from
GitHub: network time, only once per build directory); the game library 27 s; gendata 27 s;
the last line (libultraship and the link) 65 s. Round 20, the same M5 with everything from
Homebrew already there: the archive 5 s, the first `cmake` 1 min 42 s, the rest 1.5 min:
**3 min 20 s** from the clone to the executable.

- `brew install` also **upgrades** any of these that is already installed and outdated
  (`HOMEBREW_NO_INSTALL_UPGRADE=1` in front keeps what you have; the versions used are in
  MACOS.md 18 and 22). With it, brew prints `Error: <name> <version> is already installed` for
  each outdated one and exits with 1: harmless, go on with the next line.
- gendata reads the game's data layout from `versions/port_layout.us.json` (the matching N64
  build's symbol table, committed) and the bytes from your ROM; a ROM other than the US one
  stops it with a message. A developer with the N64 build (`build/turok2.us.elf`, BUILD.md)
  gets the same files from the ELF; `tools/port/mklayout.py` refreshes the json after a change
  to `src/`.
- `-DCMAKE_OSX_DEPLOYMENT_TARGET=27.0` matches Homebrew's libraries (built for 27.0): without
  it the executable, and so the app, asks for the macOS you build on (27.2 here).
- Optional: `-DT2_LTO=ON` builds with ThinLTO (same game, not faster: MACOS.md 18).

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
T2_APP_ZIP=1 tools/port/mkapp.sh OUT/rel          # also OUT/Turok2-0.20-macos-arm64.zip, to give away
tools/port/apptest.sh OUT/Turok2.app OUT/turok2.o2r baserom.us.z64 /tmp/apptest   # the check below
# round 20: a ready-to-play app for YOUR Mac, the game files inside (never give this one away)
T2_APP_ARCHIVE=OUT/turok2.o2r T2_APP_ROM=baserom.us.z64 tools/port/mkapp.sh OUT/rel OUT/play/Turok2.app
tools/port/apptest.sh OUT/play/Turok2.app - - /tmp/apptest-play             # "-": the bundle's files
```

With `T2_APP_ARCHIVE` / `T2_APP_ROM` the two files go into `Contents/Resources` before the
signature (it stays valid) and play mode uses them before the Application Support folder's, so a
double-click is all it takes (the config, logs and saves still go to Application Support).

The bundle is self-contained (round 15): the executable, `t2port.o2r`, and Homebrew's dylibs
copied into `Contents/Frameworks` and relinked to `@rpath`; the script stops if anything still
points to `/opt/homebrew`. It is signed ad hoc (`codesign --deep -s -`), not notarized: see
section 0 for the first launch. `apptest.sh` runs a copy under a path with spaces with
`/opt/homebrew` hidden and starts a new game headless.

## 3. Play

```sh
OUT/rel/app/turok2                               # or open OUT/Turok2.app
```

With no arguments (or `--play`) the game opens a window (the first launch: 4:3, sized to the
display and centred; then the size and place you left), plays the sound and runs in real time (the N64's 60 fields a second, 30 drawn frames) until you close the window.
`--play --no-audio` plays without sound.

- **Saves**: Turok 2 saves to its Controller Pak, the file `turok2_pak1.mpk` in the folder of
  step 2. Save at a save portal in the game, quit, start again and load from the title menu.
  `--pak-dir DIR` keeps the pak elsewhere.
- **Settings** go to `turok2.cfg.json` in the same folder: the window size (`Window.Width`,
  `Window.Height`; the first launch writes N x 320x240 points, the largest N that fits in 90% of
  the display, centred: 960x720 on a 14" MacBook; headless runs 640x480), fullscreen (`Window.Fullscreen.Enabled`, default off; **F11**
  toggles it), vsync (`CVars.gVsyncEnabled`, default on), the sound backend
  (`Window.AudioBackend`: `coreaudio`, the macOS default, `sdl` or `null`), the mouse and the
  look (below) and the controller mappings (`CVars.gControllers`). The window's size, position
  and fullscreen state are saved when you quit and come back at the next launch. Delete the
  file to get the defaults back. The look settings (written with their defaults at the first
  launch; edit them while the game is closed):

  | key (`CVars.`) | default | what |
  |---|---|---|
  | `gT2MouseSensitivity` | 2.0 | stick units per pixel of mouse motion (0-40; 1 = slower, 4 = faster) |
  | `gT2MouseDeadZone` | 3 | added to any mouse motion: the game ignores the first 3 stick units, so a 1-pixel motion still turns |
  | `gT2MouseInvertY` | 0 | 1: mouse up looks down |
  | `gT2StickInvertY` | 0 | 1: the gamepad's right stick / the arrow keys look inverted (in a level; menus are not inverted) |
  | `gT2ShowControlsAtStart` | 1 | the controls table in the log at each start, and the controls box once at the first launch (round 20; then `gT2ControlsShown` 1); 0: neither |
  | `gT2Renderer` | 0 | 0: OpenGL (the renderer every check used); 1: Metal (macOS, opt-in). Sets `Window.Backend.Id` at each launch |
  | `gT2RenderScale` | 1.0 | the game frame's resolution as a multiple of the window's pixels (0.25-4): 1 = full pixel resolution on a retina display, 0.5 = half (faster), 2 = supersampled |
  | `gAdvancedResolution.Enabled` | 1 | (round 19) the game frame stays 4:3 with black bars in a window of any shape and in fullscreen; 0 fills the window (the sides then show stale pictures: not recommended) |
- **Logs** go to `logs/` in the same folder: `turok2-stdout.log`, `turok2-stderr.log` (the
  port's messages) and `turok2.log` (libultraship's). Nothing is printed in the terminal except
  where the logs are.
- **Sound**: CoreAudio on macOS; it follows the system's output device (headphones, AirPods)
  while the game runs (round 19). If it cannot open, the game tries SDL's audio, then plays
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
- **Fullscreen**: F11 (remembered at the next launch).
- **Another app** (Cmd-Tab): the mouse is freed and the game gets no input until you come back;
  click in the window to capture the mouse again. Esc (pause) also frees it.
- **The first session with a window**: [FIRSTRUN.md](FIRSTRUN.md), what to try and in what order.
- **F1** shows the controls (a message box; they are also in `logs/turok2-stdout.log` at start).
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
