# The first windowed session on the Mac: a checklist

Every check of the port so far ran headless (no window). This page is the order in which to try
the things only a window can show, what each step should look like, and what to send back when it
does not. About 30-40 minutes. Background: [QUICKSTART.md](QUICKSTART.md) (install, controls,
settings), [MACOS.md](MACOS.md) 20-21 (what was checked headless and how).

## 0. Before you start

1. **The app**: a Release build of the round-19 tree, `tools/port/mkapp.sh OUT/rel` (or the one in
   `OUT/r19/Turok2.app`), then `tools/port/apptest.sh OUT/Turok2.app OUT/turok2.o2r baserom.us.z64
   /tmp/apptest` (must end with engine mode 11).
2. **Your files**: `turok2.o2r` and `baserom.us.z64` in `~/Library/Application Support/turok2`.
3. **A fresh config**: if that folder has a `turok2.cfg.json` from an earlier build, rename it
   (`turok2.cfg.json.old`): this session should see the defaults. Keep any `turok2_pak1.mpk` you care
   about somewhere else too.
4. **A terminal on the logs** (the app prints nothing in the terminal):
   `tail -F ~/Library/Application\ Support/turok2/logs/turok2-stdout.log`.
5. Have a gamepad and, if you can, headphones or AirPods (for step 9).

## 1. Launch, logos, title

- `open OUT/Turok2.app` (or double-click it). A **640x480-point window** opens; the legal screen,
  the pak check, the Acclaim and Iguana logos and the intro cinemas play with sound (about 70 s to
  the title).
- In the log, near the top: `window: renderer OpenGL`, `window: backend OpenGL ... 640x480`,
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
