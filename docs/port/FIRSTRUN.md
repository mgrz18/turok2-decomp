# The first windowed session on the Mac: a checklist

Every check of the port so far ran headless (no window). This page is the order in which to try
the things only a window can show, what each step should look like, and what to send back when it
does not. About 30-40 minutes. Background: [QUICKSTART.md](QUICKSTART.md) (install, controls,
settings), [MACOS.md](MACOS.md) 20-21 (what was checked headless and how).

## The 10-minute test (round 20: the ready-to-play app)

The app to play is staged at **`/Users/miguel_garcia/Documents/Proyectos/turok2_macport/OUT/play/Turok2.app`**
(a Release build of the round-20 tree, branch `names-R20_i4`; your ROM and `turok2.o2r` are inside
its `Contents/Resources`, so nothing has to be copied anywhere; never zip or share this copy). It
was checked with `tools/port/apptest.sh OUT/play/Turok2.app - - /tmp/apptest-play` (a new game
reached, engine mode 11, the files taken from the bundle). The config, logs and saves go to
`~/Library/Application Support/turok2` (empty before the first launch).

1. **(0:00)** In a terminal: `tail -F ~/Library/Application\ Support/turok2/logs/turok2-stdout.log`.
   Then double-click `OUT/play/Turok2.app` in Finder (or
   `open ~/Documents/Proyectos/turok2_macport/OUT/play/Turok2.app`).
2. **(0:10)** A 4:3 window opens **centred** on the display (960x720 points on a 14" MacBook) and
   takes the focus; a **"Turok 2 controls"** box shows the controls (first launch only). Read it,
   OK. The log has `files: .../Contents/Resources/turok2.o2r, .../Contents/Resources/baserom.us.z64`,
   `window: first launch: 960x720 centred ...` and `controls: first launch: the controls box`.
3. **(0:15-1:30)** Legal screen, logos, intro cinemas with sound, then the title. Watch: sharp
   picture (retina), no stretching, no crackle.
4. **(1:30)** Title: Enter (or the gamepad's Start), then Enter / A through New Game and the
   difficulty. The hub (Port of Adia) loads.
5. **(2:30-6:00)** Click in the window (the mouse is captured; the click does not fire). Walk
   (W A S D), look (mouse), jump (Space), fire (left click), Q / E for the wheels, Tab for the map.
   Is the mouse speed right? (`CVars.gT2MouseSensitivity`, 2.0; change it with the game closed.)
   Try the gamepad too if you have one.
6. **(6:00)** Esc: the pause menu; Enter; back to the game (click to capture again).
7. **(6:30)** Cmd-Tab away and back (no movement while away), drag the window wider (black bars
   at the sides, the picture stays 4:3), F11 fullscreen and F11 back.
8. **(8:00)** Plug in or pick other headphones (Control Center > Sound): the sound follows.
9. **(9:00)** Quit with Cmd-Q. Relaunch: the window comes back where you left it, no controls box.
10. **(10:00)** Send `~/Library/Application Support/turok2/logs/*.log` with what looked or sounded
    wrong (step number, screenshot with Cmd-Shift-4). The longer checklist below covers each step in
    detail.

To start again from a first launch: quit, then
`mv ~/Library/Application\ Support/turok2 ~/Library/Application\ Support/turok2.old`.

## 0. Before you start

1. **The app**: the staged `OUT/play/Turok2.app` (above), or a Release build,
   `tools/port/mkapp.sh OUT/rel`, then `tools/port/apptest.sh OUT/Turok2.app OUT/turok2.o2r
   baserom.us.z64 /tmp/apptest` (must end with engine mode 11).
2. **Your files**: inside the staged app already; for another build, `turok2.o2r` and
   `baserom.us.z64` in `~/Library/Application Support/turok2`.
3. **A fresh config**: if that folder has a `turok2.cfg.json` from an earlier build, rename it
   (`turok2.cfg.json.old`): this session should see the defaults. Keep any `turok2_pak1.mpk` you care
   about somewhere else too.
4. **A terminal on the logs** (the app prints nothing in the terminal):
   `tail -F ~/Library/Application\ Support/turok2/logs/turok2-stdout.log`.
5. Have a gamepad and, if you can, headphones or AirPods (for step 9).

## 1. Launch, logos, title

- `open OUT/Turok2.app` (or double-click it). A **4:3 window centred on the display** opens (round 20:
  the largest multiple of 320x240 points that fits in 90% of the display, 960x720 on a 14" MacBook;
  before, 640x480 at the top left), with the controls box at the first launch; the legal screen,
  the pak check, the Acclaim and Iguana logos and the intro cinemas play with sound (about 70 s to
  the title).
- In the log, near the top: `window: renderer OpenGL`, `window: backend OpenGL ... 960x720` (the first launch's size),
  `window: pixel density 2.00, render scale 1.00` (a retina display: the game draws at the window's
  pixels), `audio: playing on "<your output>"; following changes of the default output device`,
  `audio: coreaudio device, open (22,050 Hz stereo)`, and the controls table.
- Look for: a black or garbled window, a wrong aspect (stretched logos), missing or crackling sound,
  the window not in front (it should take the focus at launch).

## 2. Menus

- At the title: **keyboard** Enter (START/confirm), Esc (START), the arrow keys move; **gamepad**
  Start, A (confirm), B (back), the left stick or the D-pad move.
- Open the options menu and back out with both. Unplug the gamepad while the menu is up and plug
  it back in: the log says `controls: gamepad disconnected` / `connected`, and it works again
  (checked headless: `padtest.py --only hotplug`).

## 3. A new game, moving and looking

- New Game, pick a difficulty, enter the hub (the first level is reached from it).
- **Keyboard**: W S A D walk and strafe, the arrow keys turn and look, Space jumps, Ctrl fires,
  Q / E hold for the weapon wheels, Tab the map.
- **Mouse**: click in the window: the cursor hides and the mouse turns and looks (that click does
  not fire). Left button fires, right button jumps. **F2** frees the mouse, F2 again (or a click)
  captures it. Try a slow and a fast turn: if the look is too slow or fast, the setting is
  `CVars.gT2MouseSensitivity` (QUICKSTART 3).
- **Gamepad**: left stick walks, right stick looks, right trigger fires, left trigger or right
  shoulder jumps, A / B the wheels.
- **Esc** pauses the game (START). LUS also frees the mouse on Esc: click to capture it again after
  unpausing (the click does not fire).

## 4. Focus: Cmd-Tab, other windows

- In a level, with the mouse captured and a key or gamepad button held, **Cmd-Tab** to another app.
  The log says `controls: the window lost the focus: the mouse released, no input to the game`; the
  pointer is free in the other app; Turok stops moving and firing (the game itself keeps running:
  pause first if you leave for long). Cmd-Tab back: `the window has the focus again`; nothing moves
  until you press something; a click in the window captures the mouse again.
- Also try a click on the Dock or another window instead of Cmd-Tab, and a notification banner.

## 5. The window: resize, aspect, retina

- Drag the window's corner wide (16:9) and tall. The game frame stays **4:3 with black bars**
  (pillarbox / letterbox) and the HUD stays inside it. Before round 19 the sides showed stale frames
  and 3D drawn past the game's edges (MACOS.md 21.3). The window does not go below 320x240 points.
- **Sharpness**: on a retina display the text and the HUD should be as sharp as the display allows
  (the game frame at the window's pixels). `CVars.gT2RenderScale` 0.5 makes it softer and lighter on
  the GPU, 2 supersamples (edit with the game closed).

## 6. Fullscreen (F11)

- **F11** goes fullscreen the macOS way (its own Space, the slide animation; the green window
  button does the same). On a 16:10 MacBook display there are black bars at the sides (4:3). F11
  again goes back to the window at its old size and place.
- In fullscreen: Cmd-Tab away and back (step 4), F1 (the controls box must show over the game),
  and quit with Cmd-Q (step 10): the next launch starts fullscreen.

## 7. Frame pacing

- Walk and turn in the hub for a minute: the game runs at 30 frames a second (the N64's rate; the
  in-engine cinemas at 15). Look for stutter, tearing (vsync is on: `CVars.gVsyncEnabled`), or the
  game running slow or fast against the music.
- At quit the log has three `pace:` lines: the late VIs (a few at level loads are normal; many more
  than in MACOS.md 19.4 is worth reporting), and `sound device buffer ... sped up / slowed N times`.
  If it stutters, try `gVsyncEnabled` 0 for one session and send both logs.

## 8. Sound

- Music and effects in the title and in a level; the volume options of the game's menu.
- Look for crackle (buffer underruns), sound that drifts behind the picture, or silence.

## 9. Change the output device while playing

- With the game running, plug in headphones, or pick AirPods / another output in Control Center >
  Sound (or System Settings > Sound). The sound moves to the new output within a fraction of a
  second (a short hitch is expected). The log says
  `audio: the default output changed ("<old>" -> "<new>"): sound player re-created, open`.
  Unplug them: back to the speakers, the same line. Before round 19 the sound stayed on the old
  device (or stopped). Checked headless only by calling the listener (`--audio-device-change-test`).

## 10. Save, quit, relaunch

- **Save**: walk into a save portal in a level and save to the Controller Pak (the file
  `turok2_pak1.mpk` in the game folder; each block is written at once, so a quit right after is
  safe). `python3 tools/port/mpk_saves.py ~/Library/Application\ Support/turok2/turok2_pak1.mpk`
  lists the saves.
- **Quit** once with **Cmd-Q** and once with the window's **close button** (and, in another session,
  from the Dock menu). The app must close within a second, with no crash report.
- **Relaunch**: the window comes back at the size, place and fullscreen state you left; Load Game
  from the title loads the save; the settings you edited in `turok2.cfg.json` are kept.

## 11. Where the logs are, what to report

- Folder: `~/Library/Application Support/turok2/`: `logs/turok2-stdout.log` (the port's lines:
  window, audio, controls, pace), `logs/turok2-stderr.log` (warnings, a missing file), `logs/turok2.log`
  (libultraship's own log), `turok2.cfg.json` (the settings), `turok2_pak1.mpk` (the saves). A crash:
  `~/Library/Logs/DiagnosticReports/turok2-*.ips`.
- For each problem: the step above, what you did and saw (a screenshot: Cmd-Shift-4), whether it
  repeats, the three log files of that session, and: the Mac and display (built-in or external, its
  refresh rate), the gamepad model, the output device.

## Known, not bugs

- No rumble. Metal is opt-in (`CVars.gT2Renderer` 1; OpenGL is the default and the checked one).
- Esc and F1 free the mouse (LUS); a click captures it again.
- The game keeps running while the window has no focus (no input reaches it).
- A ~50 ms hitch when the output device changes (the new device is opened on the game thread).
- `logs/turok2.log` has `[error] Failed add SDL game controller mappings from "./gamecontrollerdb.txt"`:
  libultraship looks for SDL's optional community mapping file (QUICKSTART 5); without it SDL's
  built-in mappings are used (every current Xbox, PlayStation and Switch pad).
