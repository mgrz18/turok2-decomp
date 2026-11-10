# Performance: what one frame costs on the host (Phase 7 round 7, part v4)

This page records how much host time the port spends per frame on the Mac (Apple M5, macOS 27)
and on the Linux box (Xeon Gold 6230N). It covers the boot and level-51 gameplay. It also
answers whether the Mac can hold the N64's real-time pace. Two debug switches produce the
numbers: `--bench` and `--pace` (port/app/game.cpp).

## 1. The switches

```sh
# per-VI host times from VI FROM on (default 0), printed at the end of the run
turok2 --game --draw --pak-blank [--offscreen-gl | --no-window] [--audio] --bench [FROM] --frames N
# real time: one VI every 1/60 s (the game draws every second VI: 30 fps); add --bench to measure
turok2 --game ... --pace [--bench FROM]
```

One loop iteration of `turok2 --game` is one VI retrace (60 Hz). The game draws on every second
VI, so it produces 30 frames per second, as on the N64. Per VI, `--bench` measures:

| series | what it covers |
|---|---|
| game (CPU) | `T2Port_Frame` (the game threads until all block, the t2os switches between them) minus the audio manager |
| audio | the audio manager thread's frame: the synthesizer (`alAudioFrame`: the sound player, the sequencer, the command list) and the ASP (`T2Asp_Run`, the mixer). `T2Audio_GetTimes` counts them (port/audio/t2audio_mgr.c, two clock reads per audio frame) |
| draw | dlcheck, Fast3D's interpreter (`StartFrame`/`Run`/`EndFrame`). With `--offscreen-gl` it also includes a `glFinish`, so the GPU's work is counted. In a window it includes the swap |
| total | the whole iteration (without the `--pace` sleep) |
| game frame (drawn) | wall time from one drawn frame to the next: what one displayed frame costs (2 VIs) |
| t2os switches/VI | thread switches per VI |

At the end, `--bench` measures one t2os switch. It uses the same primitive t2os uses: one mutex,
a condition variable per thread, and `pthread_cond_signal`/`pthread_cond_wait` to pass the
baton. Two host threads pass the baton 200,000 times.

`--pace` sleeps to absolute deadlines 1/60 s apart. There is no catch-up: a late VI moves the
schedule. It reports the number of late VIs and the worst ones (`VI:ms`).

## 2. The Mac (M5, 10 cores), headless `--offscreen-gl`

Builds: the usual Debug build (`-g`, no optimisation) and a RelWithDebInfo build (`-O2 -g`,
`OUT/build-rel`, the same sources). Both give identical `T2_STATE_LOG` output for the whole
level-51 journey (250 records). Runs: the boot to VI 5,400 (pak check, legal, expansion pak,
both intro cinemas, the title), and level 51 VIs 13,000-15,000 (the scripted journey: walk,
swim, turn, weapon wheel, fire).

Mean per **drawn game frame** (2 VIs), in ms. The N64 budget is 33.3 ms.

| run | Debug | Release (-O2) |
|---|---|---|
| boot, display lists not drawn (`--no-window`) | 0.066 | 0.035 |
| boot, drawn (`--offscreen-gl`, glFinish) | 1.108 (p99 4.5) | 0.657 (p99 2.5) |
| level 51, not drawn | 0.143 | 0.050 |
| level 51, drawn | 1.070 (p99 3.4) | 0.562 (p99 2.1) |
| level 51, drawn + `--audio` | 1.051 (p99 2.6) | 0.553 (p99 1.7) |

Split per VI, level 51 drawn with audio (mean, ms):

| | Debug | Release |
|---|---|---|
| game CPU | 0.074 | 0.032 |
| audio (synth + ASP) | 0.092 (0.008 + 0.084) | 0.022 (0.003 + 0.019) |
| draw (Fast3D + GL + glFinish) | 0.357 | 0.222 |
| t2os switches | 5.7 per VI x 1.6 µs = 0.009 | 5.7 x 2.2 µs = 0.012 |

- **Drawing is the main cost**, about 80% of a frame: Fast3D's interpreter and the GL work.
  The game logic and the audio together are less than 0.06 ms per VI in Release.
- The t2os switch costs 1.6-2.2 µs on the Mac. At 4-6 switches per VI, that is about 0.01 ms per
  VI, so it does not matter.
- Release unpaced runs level 51 at 3,600 VIs per second, which is 60x real time. Debug runs at
  31x.
- Level 51 only exercises a light part of the game so far: no enemies are active and the
  particles are off on this branch (v1's work). The game CPU figure will grow with combat, but
  the headroom is about 60x.

### Real time (`--pace`, with audio)

| run | late VIs | worst | game frame mean | per-VI busy (mean / p99) |
|---|---|---|---|---|
| Release, boot (5,400 VIs, 90.04 s for 90.00) | 5 | 14.1 ms (VI 3871, 3991: the title's load) | 42.6 ms (\*) | 0.87 / 7.1 ms |
| Release, new game to level 51 (15,000 VIs, 250.02 s) | 6 (0 in level 51) | 5.9 ms | 33.33 ms | 0.75 / 2.7 ms |
| Debug, boot | 24 | 147 ms (VI 1070: the expansion-pak screen loads) | 42.9 ms (\*) | 1.35 / 9.7 ms |
| Debug, new game to level 51 | 27 (2 in level 51) | 139 ms (VI 12895: level 51 loads) | 33.34 ms | 1.50 / 4.2 ms |

(\*) The boot draws less often than every second VI, on both hosts and unpaced too: 2,111 gfx tasks in 5,400 VIs, one per 2.6 VIs. The pace is the game's own, not lateness.

**The Mac holds the N64 pace with large headroom.** In Release, level 51 has no late VI. A paced
VI costs 0.75 ms of the 16.7 ms period, so the process sleeps most of the time. The few late VIs
are the mode switches and level loads (the synchronous cache/decompression work of a load), up to
14 ms in Release and up to 147 ms in Debug.

Paced, the per-VI times are 2-3 times the unpaced ones (level 51 Release: 0.75 ms against
0.28 ms). A mostly idle process runs at a lower clock and on the efficiency cores. The pacing
numbers above already include that.

## 3. The Linux box (Xeon Gold 6230N 2.3 GHz, 32 threads, shared: load average 22 during the runs)

Debug build, low memory, `SDL_VIDEODRIVER=offscreen` with a window (EGL, Mesa). Mean per drawn
game frame, in ms:

| run | no window | window |
|---|---|---|
| boot | 0.217 | 16.74 |
| level 51 | 0.446 | 16.68 |
| level 51 + audio | | 16.68 (audio 0.36 per VI: synth 0.03 + ASP 0.33) |

With a window, each drawn frame takes 16.7 ms. The window's swap paces the draws to 60 Hz, so the
GL time cannot be measured there. The game CPU without drawing is 3x the Mac's (Debug level 51:
0.446 against 0.143 ms per frame). A t2os switch costs 4.3-5.6 µs. The box is shared with
other runs, so these numbers are rough.

## 4. What would make it faster (not needed for real time)

- Release builds: they halve the frame cost. The Debug default exists for the bring-up.
- The draw path: Fast3D's interpreter runs on the CPU and dominates the frame. The `glFinish`
  in `--bench` serialises the GPU. A real presenter overlaps it.
- Level loads: the late VIs are synchronous loads (the N64 also stalls there). A paced player
  could hide them behind the fade.
