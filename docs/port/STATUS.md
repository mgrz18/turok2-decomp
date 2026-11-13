# Port status after Phase 7 round 12 (merged state)

Round 12 merged five branches, in this order: a5 (the N64-offset checker `n64viewcheck.py` and
24 host-view fixes, `CPlayerIntelligence` typed: a new archive), a4 (the round-11 tree on macOS:
Mac == Linux in every scenario, arm64 flush-to-zero verified), a1 (level 5 to its end), a2 (level 6
to its end, the Mother, the Primagen, the ending) and a3 (one continuous run through levels 1-4
and the hub's key doors). Only a5 and a2 changed `src/` (no file in both); the shared
debug files and docs were unioned (section 3). On the merged tree **levels 5 and 6
were hooked into the continuous run**, so **one process now plays the whole game: a new game,
levels 1 to 6 in order, every totem and every boss, the Primagen, the ending cinema and the
credits, back to the hub** (`playthrough.py RUN --all`, section 2). Detail pages:
[PLAYTHROUGH.md](PLAYTHROUGH.md) sections 9.4 (a1), 9.5 (a2), 10 (a3) and 11 (this merge),
[STABILITY.md](STABILITY.md) section 8 and [64BIT.md](64BIT.md) part 6 (a5), [MACOS.md](MACOS.md)
section 14 (a4). Round 11 and earlier follow as history.

The matching build is unchanged: `tools/asm_diff.py --base 046a67f9 -j 8` reports 0 differences
over all 3,938 files after every merge and every fix, and a clean `make clean && make setup &&
make build && make verify` gives the identical ROM (`build/turok2.us.z64: OK`).

## Round 13 b3: the talismans, the nuke parts, the 100% ending

Detail: [PLAYTHROUGH.md](PLAYTHROUGH.md) section 12. Port-only (tools/port, port/game/t2debug.c,
port/app/game.cpp); no `src/`/`include/` change.

- **The 100% ending is decided by the five totem flags, not by engine mode 16.** Mode 16
  (`EngineMode16_Ending_Init`) is only the cheat menu's level-select "credits" entry. With all five
  totem defences won `Primagen_EnterDeath` plays cinema 0x172 "PD2-P tired" and its chain (the
  totems strike), level 10's "END 2", the credits, and unlocks the cheats 0x7FF0019; otherwise
  0x173 "Primagen Death", level 11's "END 1b", level 10, the credits (cheat 2). **Both play on the
  port** (the Primagen recipe + `T2_TOTEMS=11111` / `11011`: credits and the hub after each).
- **Talisman chambers 73-77**: a pedestal action needs the level's talisman piece (4400-4404),
  then a cinema, a door, the talisman (4380-4384) on the dais; no fight. **All five picked up** by
  `tools/port/extras.py OUT --talisman N` (walked, the piece given). In the continuous run
  `fullrun.py RUN --talismans [2,3]` collects levels 2 and 3's pieces and walks their portals and
  chambers (**Leap of Faith and Breath of Life picked up**, 4,800 frames each).
- **Flesh-eater levels 78-83**: **all six nuke parts picked up** (`extras.py OUT --nuke N`,
  teleport-assisted).
- New DEBUG-ONLY switches: `T2_PROGRESS_LOG=1` (a line per change of the totem/boss flags,
  talismans, Primagen keys, nuke parts, unlocked cheats, ending flags, cinema script),
  `T2_TOTEMS=abcde` (the five totem flags at the first gameplay frame).
- **The totem fights decide the ending and are timing-sensitive**: the base `--all` run loses the
  City Totem (110, 5/8, twice identically); with the level-2 and 3 chamber trips the City Totem is
  won (11/8) but the Marsh Totem (111) lost (7/8): one kill short at ~1 kill per 15 s.

## 1. Build and run

```sh
# 0. the matching build first: gendata and asm_diff read its ELF; keep a copy as build/ref.elf
make setup && make -j8 build && make verify && cp build/turok2.us.elf build/ref.elf

# 1. the archive, format 2.0. NEW THIS ROUND (a5 typed CPlayerIntelligence 0x1C-0x34: the old
#    archive with this binary cannot jump)
python tools/assets/mkarchive.py baserom.us.z64 -o OUT/turok2.o2r -j 4     # ~12 s, sha1 a9d14a47...
python tools/assets/archive_check.py OUT/turok2.o2r baserom.us.z64          # OK, format 2.0

# 2. configure IN A FRESH BUILD DIR (a PIE is the Linux default; an old cache may keep
#    T2_LOW_MEMORY=ON); the game library AND the runtime/audio libraries (gendata reads all three)
rm -rf OUT/build
cmake -S port -B OUT/build -G Ninja -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ \
      -DCMAKE_BUILD_TYPE=Debug                      # -DT2_LOW_MEMORY=ON: the old -no-pie build
ninja -C OUT/build -j8 turok2_game t2runtime t2audio

# 3. the game data as C into port/gen/ (gitignored). Drop the cache after ANY header change.
#    RERUN IT this round: a5 changed structs/intelligence.h.
rm -f OUT/decls.json
python3 tools/port/gendata.py --build OUT/build --cache OUT/decls.json -j 4      # ~1 min

# 4. re-run cmake so data.cmake sees port/gen/*.c, then everything (turok2, t2audio_test and the
#    compile-only module-2 library, which checks that every C file builds for the host)
cmake OUT/build && ninja -C OUT/build -j8

# 5. run headless (Linux: SDL's offscreen driver, no window). High memory (and a PIE) is the default.
cd OUT && SDL_VIDEODRIVER=offscreen T2_SHOT=shot.ppm T2_SHOT_EVERY=300 \
    build/app/turok2 --archive turok2.o2r --rom ../baserom.us.z64 \
    --game --draw --pak-blank --frames 26000
```

The archive: 26,124 units, sha1 `a9d14a47dba3f75ab30a0e4cdfdb1cee56977d22` (`archive_check.py`:
OK, format 2.0).

**The whole game in one run** (section 2; [PLAYTHROUGH.md](PLAYTHROUGH.md) sections 10 and 11):

```sh
python3 tools/port/playthrough.py RUN --all        # = tools/port/fullrun.py RUN [--upto N] [--hop F]
cd OUT && . RUN/teleport.env && SDL_VIDEODRIVER=offscreen T2_EVENT_LOG=1 T2_PICKUP_LOG=1 \
  T2_STATE_LOG=500 build/app/turok2 --archive turok2.o2r --rom ../baserom.us.z64 --game \
  --pak-dir RUN/pak --frames FRAMES --input @RUN/playthrough.txt
python3 ../tools/port/mpk_saves.py RUN/pak/turok2_pak1.mpk      # the saves after levels 1-5
```

Use an EMPTY `--pak-dir` (the save menu's inputs assume a fresh pak: an old note puts an
"overwrite?" dialog in the way). **Kill a stuck run with SIGKILL**: the game ignores SIGTERM
(`timeout -s KILL`).

**The scripted runs** (`turok2 --archive turok2.o2r --rom ../baserom.us.z64 --game --pak-blank`
plus `--draw` for pictures; "new game" is `START@4400 A@4500 A@4700 A@4800`; "boss env" is
`T2_GIVE_WEAPONS=1 T2_WEAPON=10 T2_AIM_NODE=boss T2_CHEATS=1 T2_FILL_AMMO=600 T2_BOSS_LOG=1`).
The level runs come from `tools/port/playthrough.py DIR [--level N] [--direct]`, which writes
`DIR/playthrough.txt` and `DIR/teleport.env`; run them as
`. DIR/teleport.env && T2_EVENT_LOG=1 T2_STATE_LOG=500 turok2 ... --input @DIR/playthrough.txt`:

| what | how | frames |
|---|---|---|
| boot + the 3 attract demos | (none) | 26,000 |
| **the whole game** (new game -> levels 1-6 -> the Primagen -> the ending -> the credits -> the hub) | `playthrough.py DIR --all`, `--pak-dir` | 266,000 |
| **level 1, the Port of Adia** | `playthrough.py DIR --fight 10` ([PLAYTHROUGH.md](PLAYTHROUGH.md) sections 1-5) | 27,000 |
| **level 2, the River of Souls** | `playthrough.py DIR --level 2` (sections 6, 6.5) | 64,000 |
| **level 3, the Death Marshes** | `playthrough.py DIR --level 3` (section 7) | 36,000 |
| **level 4, the Lair of the Blind** (with the Blind One) | `playthrough.py DIR --level 4` (section 8) | 46,000 |
| **level 5, the Hive of the Mantids** (with the Mantid Queen) | `playthrough.py DIR --level 5` (section 9.4; tools/port/routes56.py) | 48,000 |
| **level 6, the Lightship** (the Mother, the Primagen, the ending, the credits) | `playthrough.py DIR --level 6` (section 9.5) | 112,000 |
| a level straight from its first sub-level | `--level 3 --direct` / `--level 4 --direct` (no hub, no cinemas) | |
| **the Mother** | the input file below, `T2_WARP_TO=-4` + boss env | 36,000 |
| **the Blind One** | new game + `Z@14000..90000`, `T2_WARP_TO=7401 T2_TELEPORT=14000:-400,204.8,0` + boss env | 25,000 |
| **the Mantid Queen** | new game + `Z@14000..200000`, `T2_WARP_TO=7501 T2_BOSS_FRONT=450,200` + boss env | 30,000 |
| **the Primagen, the ending, the credits** | new game + `Z@14000..200000`, `T2_WARP_TO=7602 T2_BOSS_FRONT=450` + boss env | 62,000 |
| save | `--pak-dir DIR`, new game + `A@13250 A@13400`, `T2_SAVE_PORTAL=13100` | 14,100 |
| load | `--pak-dir DIR` (the same DIR), `START@4400 A@4500 DD@4700 A@4800 DU@4900 A@5000 A@5200 A@5400` | 6,500 |
| demos against the N64 | below | 26,000 |

`T2_BOSS_FRONT=D[,EVERY[,K[,1]]]` puts the player D units in front of the Queen's open weak spot
every EVERY frames (between her waves), or beside the Primagen's lair (D to the side, K x D in
front, default 2.5) after EVERY frames of a lair wait; `,1` (this round) tries another spot of a
list at each placement ([GAMEPLAY.md](GAMEPLAY.md) section 12, PLAYTHROUGH.md 9.5 and 11).

The Mother recipe (one input entry per line, a single line is cut at its buffer):

```sh
cd OUT && (echo "START@4400 A@4500 A@4700 A@4800 Z@14000..90000 CL@19000..90000";
  for s in $(seq 19000 600 89000); do echo "CD@$s..$((s+250))"; done) > mother_end.txt
SDL_VIDEODRIVER=offscreen T2_WARP_TO=-4 T2_GIVE_WEAPONS=1 T2_WEAPON=10 T2_AIM_NODE=boss \
  T2_CHEATS=1 T2_FILL_AMMO=600 T2_BOSS_LOG=1 T2_STATE_LOG=200 \
  build/app/turok2 --archive turok2.o2r --rom ../baserom.us.z64 --game --pak-blank \
  --frames 36000 --input @mother_end.txt
```

The demos against the N64's RAM ([FIDELITY.md](FIDELITY.md) sections 5-6; the emulator side
runs ~250 frames a minute on a loaded box, once per demo range):

```sh
xvfb-run -a -s "-screen 0 1024x768x24" python3 tools/port/n64trace.py --rom baserom.us.z64 \
    --from 1980 --to 3300 --actors > n64_d1.det                  # demo 1
xvfb-run -a -s "-screen 0 1024x768x24" python3 tools/port/n64trace.py --rom baserom.us.z64 \
    --from 4915 --to 7870 --actors > n64_d23.det                 # demos 2 and 3
cd OUT && SDL_VIDEODRIVER=offscreen T2_DET_LOG=1 T2_DET_GF=1 T2_DET_ACTORS=1 T2_DET_FROM=5400 \
    build/app/turok2 --archive turok2.o2r --rom ../baserom.us.z64 --game --pak-blank \
    --frames 26000 > port.log
python3 ../tools/port/detdiff.py ../n64_d1.det port.log --shift 0 --skip rng,flags
python3 ../tools/port/detdiff.py ../n64_d23.det port.log --level 131 --shift 0 --skip rng
python3 ../tools/port/detdiff.py ../n64_d23.det port.log --level 109 --shift 0 --skip rng
```

**The tools** (Linux box):

- `tools/port/playthrough.py OUTDIR [--fight WEAPON] [--level 1..6] [--direct] [--all]`: the
  level runs above (levels 5 and 6 from `tools/port/routes56.py`), `--all` the whole game
  (`tools/port/fullrun.py`: levels 1-4 from playthrough.py's segment lists, 5 and 6 from
  routes56.py through `LEVEL_HOOKS`, moved in time and re-anchored with `T2_ANCHOR` at each
  sub-level the route enters); `tools/port/approach.py` gives a teleport point facing a warp
  region or a point.
- `tools/port/n64viewcheck.py --build OUT/build -j 4` (a5): libclang over the host-compiled side
  of every linked `src/` file: raw N64 byte offsets, N64 strides and pad-placed views of engine
  structs whose host offsets differ (classes BAD / suspect / moves / global / untyped / local /
  same; [STABILITY.md](STABILITY.md) section 8).
- `tools/port/soak.py --exe OUT/build/app/turok2 --archive OUT/turok2.o2r --rom baserom.us.z64
  --out DIR --kind attract|newgame|warp|levels --seeds A:B [--frames N] [--both-memory] [--jobs N]
  [--debugger gdb|lldb]`: random but valid input under gdb (lldb on macOS), crashes grouped by
  their top frames, hangs interrupted and classed by the busy thread (`crashes.txt`,
  `summary.json`, a `cmd.sh` per run). `attract` (no input: use `--frames 100000`), `newgame`
  (random play from 13,000), `warp` (random warp ids), `levels` (every level id 1-164 warped
  into, four per seed: seeds 0-40, `--frames 25000`) ([STABILITY.md](STABILITY.md)).
  **Do not relink `turok2` while a soak runs**: gdb then reads a half-written file and every run
  counts as `EXIT255`.
- `tools/port/n64trace.py` + `tools/port/detdiff.py`: the N64's per-game-frame state from RDRAM
  (mupen64plus's debugger, a breakpoint at `CEngineApp__Update`; `--actors` adds the actors and
  the `cam` line, `--calls NAME|ADDRESS --args REG:TYPE:MEMBER[:f|d|x]` traces a function's
  arguments) against the port's `T2_DET_LOG=1 T2_DET_GF=1 T2_DET_ACTORS=1` trace, aligned by
  `gFrameCountGameplay`. `xvfb-run -a ... tools/port/refshots.py ...`: the N64 reference
  screenshots and RDRAM dumps. Captured frames, traces and dumps are never committed.

Options (`turok2 --game`):

- `--input`: `BUTTON@F`, `BUTTON@F1..F2` (held), `SX=V@F1..F2` / `SY=V@...` (stick), `@FILE`
  ([INPUT.md](INPUT.md) section 6). The stick turns, C-up/C-down move, C-left/C-right strafe,
  R jumps, Z fires, A/B hold the weapon wheel, L the map, START pauses; the D-pad moves in menus.
- `--pak-blank`, `--pak-dir DIR` (`DIR/turok2_pak<N>.mpk`, N64 byte order; `tools/port/mpk_saves.py
  FILE.mpk` checks the notes), `--no-pak`.
- `--audio` / `--wav out.wav`; **high memory and a PIE are the default** (`-DT2_LOW_MEMORY=ON`
  brings back `-no-pie` and `--low-memory`); `--offscreen-gl` (**macOS**: pixels with no window);
  `--no-window`; `--bench [FROM]`, `--pace` (`--pace --audio`: a simulated 22,050 Hz device counts
  underruns) ([PERF.md](PERF.md)).
- Debug switches (port/game/t2debug.c, t2bosslog.c, t2det.c and game.cpp, **debug only**). The
  frame-scheduled ones read the *schedule frame* (the host frame until the first `T2_ANCHOR`):
  - state: `T2_STATE_LOG=N`, `T2_ACTOR_LOG=N` (+ the `actor+:` line: body flags, an action's auto
    radius and needed pickup, a destructible's health, the AI callbacks by name), `T2_ACTOR_TYPES=t,...`,
    `T2_BOSS_LOG=1` (the boss, its hits, a `bosstrace:` line on every boss mode/phase change),
    `T2_PICKUP_LOG=1`, `T2_EVENT_LOG=1` (every level event with the objective counters of the
    level's own hub; the `totem:` line: state 0 fight, 1 saved, 2 failed), `T2_SCENE_LOG`,
    `T2_MENU_LOG=1`, `T2_PARTICLE_LOG=N`;
  - determinism: `T2_DET_LOG=N` (`T2_DET_FROM`, `T2_DET_ACTORS`, `T2_DET_GF=1`,
    `T2_DET_PARTICLES=1`), `T2_RNG_TRACE=F1..F2`;
  - moving and combat: `T2_WARP_TO=<id>[@frame][,...]` (a negative id is `~level`),
    `T2_TELEPORT=F:x,y,z[,rotY][!][;...]`, `T2_WAKE_ENEMIES=R`, `T2_FACE_NEAREST=R[@L1,L2,...]`
    (+ `T2_FACE_PITCH=1`, `T2_FACE_STICKY=1`), `T2_AIM_NODE=MASK|boss|-1|actor:N[,R][@L1/L2/...]`,
    `T2_BOSS_FRONT=D[,EVERY[,K[,1]]]` (+ `T2_BOSS_FRONT_PROBE=1`, `T2_BOSS_FRONT_SCAN=1`),
    `T2_AIM_AT=F1-F2:x,y,z[;...]`, `T2_AIM_ENEMY=F1-F2[,R[,A]][;...]`, `T2_CHEATS=MASK`
    (1 invincibility), `T2_FILL_AMMO=N`, `T2_GIVE_WEAPONS=1`, `T2_WEAPON=<n>`, `T2_WEAPON_LIST=1`,
    `T2_GIVE_ITEMS=ID[:N][,...][@F][;...]` (the special pickup counts: level 6's capacitors and
    laser cells);
  - **frame-scheduled values** (this round): `T2_WEAPON`, `T2_FILL_AMMO` and `T2_BOSS_FRONT` also
    take `V@F;V@F...` (the last entry whose F has passed), so one run can use the shredder on
    level 5 and the plasma rifle elsewhere;
  - progression: `T2_SAVE_PORTAL=F1,F2,...`, `T2_DOOR_OPEN=F`, `T2_ANCHOR=L@N[;L1/L2@N...]` (the
    schedule clock re-anchored when level L, or any of L1/L2, is reached; from 3,000 frames before
    N);
  - capture: `T2_SHOT=FILE T2_SHOT_EVERY=N T2_SHOT_FROM=F`, `T2_DLDUMP_AT=<frame>`
    (+ `T2_DLDUMP_VTX=1/2/3`), `T2_ANIM_EVENTS=0`, `T2_V2P_TRAP=1`, `--skip-pak-check`,
    `--start-mode N`.

## 2. What the game does now (Linux, headless, PIE, high memory, final binary)

Every run below exits 0.

- **The whole game in one process** (`playthrough.py RUN --all`, 266,000 frames, `--pak-dir`,
  [PLAYTHROUGH.md](PLAYTHROUGH.md) section 11.2): a new game; level 1 with its keys, the Harbor
  Totem won, the hub (save 1), the level-2 key slots ("access to level 2 granted.") and its door;
  level 2, the City Totem (**failed, 5/8**, this run), the hub (save 2); level 3, the Marsh Totem
  won (save 3); level 4, the Cave Totem won, **the Blind One dead at 136,092** (save 4); **level 5**
  ("force field down." 144,388, "all embryos destroyed. mission complete." 154,682, "master computer
  destroyed. mission complete." 156,834), the Hive Totem won (9/8), **the Mantid Queen dead at
  167,646**, the hub (save 5); **level 6** ("all assembly plants destroyed." 187,389, "all power
  generators recalibrated." 188,245, "force field down." 189,029), **the Mother dead at 211,535**,
  the hub, the hop, **the Primagen dead (mode 29) at 240,814**, **the ending cinema (levels 11
  and 10) 242,000-246,822**, **the credits 246,830-260,920**, **the hub at 260,928**. Exit 0,
  the same on two runs; the pak's note checksum OK (clock 0:45:15).
  **What is still debug-assisted**: `T2_CHEATS=1` (invincibility), `T2_FILL_AMMO`,
  `T2_GIVE_WEAPONS`, the aim assists (`T2_FACE_NEAREST`, `T2_AIM_*`, `T2_BOSS_FRONT`), the
  teleports between route steps inside the sub-levels, the teleports past the hub's level-5 and
  level-6 doors (not all their keys are picked up), `T2_GIVE_ITEMS` (level 6's crystal ion
  capacitors and two red laser cells) and the hop to the Primagen (`T2_WARP_TO=7602`: the hub's
  way to him needs Primagen keys 5 and 6). Every objective, totem, boss, cinema and warp between
  levels is the game's own.

- **The boot loop** (26,000): the pak check, legal, the expansion pak, Acclaim, the Iguana jungle,
  the title, the three demos and the title again, as in round 11. **Demo 1 identical to the N64's
  RAM from game frame 1 to its end (gf 1,128)** (`detdiff.py`: "no difference over the common game
  frames", fresh N64 traces on this tree), **demo 3 identical from gf 1 to its end (gf 344)**, demo 2
  identical to gf 607 (from gf 608 only the player's health: the N64's uninitialised heap read,
  [FIDELITY.md](FIDELITY.md) 6.3), as in round 11.
- **Level 1** (`--fight 10`, 27,000): "all distress beacons activated. mission complete." (14,751),
  "the children are safe. mission complete." (17,499), the Harbor Totem won (10/8), **the hub by
  22,000**.
- **Level 2** (`--level 2`, 64,000): "all soul gates destroyed. mission complete." (42,063), "all
  sisters of despair banished. mission complete." (52,707; 52,827 in round 11: a5's blast fixes
  move the fights), the City Totem won (11/8), **the hub by 62,000**.
- **Level 3** (`--level 3`, 36,000): "all prisoners free. mission complete." (24,243), "armories
  destroyed. mission complete." (25,097), the Marsh Totem won, **the hub by 31,500**.
- **Level 4** (`--level 4`, 46,000): "blind ones trapped. mission complete." (25,199), the Cave
  Totem won, **the Blind One dead (mode 9) at 40,513** (41,113 in round 11), **the hub by 41,500**.
- **Level 5** (`--level 5`, 48,000; a1): "force field down." (the four pylons), the three embryos
  (24,607 / 27,307 / "all embryos destroyed. mission complete." 29,207), **"master computer
  destroyed. mission complete." (31,359)**, the Hive Totem won (21/8), **the Mantid Queen dead
  (mode 16) at 42,095**, **the hub by 43,500**.
- **Level 6** (`--level 6`, 112,000; a2): "all assembly plants destroyed. mission complete."
  (29,625), **"all power generators recalibrated. mission complete." (30,481)**, **"force field
  down." (31,265)**, **the Mother dead (mode 30) at 48,847**, 114, **the hub (53,000)**, the hop to
  the Primagen (54,000), **his death (mode 29) at 84,218**, the ending cinema (levels 11 and 10),
  **the credits (engine mode 15) 90,234-104,324**, **the hub at 104,500**.
- **The four boss recipes**, the same frames as round 11: **the Mother** (36,000): her death
  (mode 30) at 27,459, the defeat cinema at 27,635, the hub by 29,000; **the Blind One**
  (25,000): her death at 22,623, the hub by 23,500; **the Mantid Queen** (30,000): her death (mode
  16) at 26,907, the hub by 28,500; **the Primagen** (62,000): his death (mode 29) at 38,714, the
  ending cinema from 40,000, the credits 44,730-58,820, the hub at 58,828.
- **Save -> title -> load**: the note (`mpk_saves.py`): NT2E 51 "TUROK 2" ext A, 90 pages,
  big-endian, checksum OK, health 100, lives 3, clock 0:02:20, 663 ISets of level flags; a new
  boot with that pak loads it: level 51 at 5,500.
- **Audio** (`--audio --wav`, 9,000 frames): 150.17 s, peak 32,768, RMS 3,399.5 (= round 11).
- **Soak** (PIE, no `--draw`): attract 100,000 (1 run, 99,960 frames), new game seeds 0-19 x
  36,000 (20 runs, 720,000 frames), levels seeds 0-40 x 25,000 (every level id 1-164, 41 runs,
  1.02 M frames): **0 crashes, 0 hangs**.
- **macOS** (a4, the round-11 tree + a4): Mac == Linux frame for frame in every scenario of round
  11's battery; this merged tree was not rerun on the Mac.

Screenshots of the continuous run (640x480, drawn only in frame ranges with the new
`T2_DRAW_FROM=A-B;...`, checked by eye, not committed): level 5's "FORCE FIELD DOWN." on the hive
top (144,500); the master computer's room with "WARNING. FIVE SECONDS UNTIL DETONATION."
(156,700; the room's screens draw as a many-coloured pattern, not compared with the N64);
level 6's "POWER GENERATOR RECALIBRATED." at a coupling of generator 1 (178,001); 114 as the
force field goes (189,001: the screen washes blue) and its corridor after (189,101); the ending
cinema (the Primagen's ship: Adon 242,601, Adon and Turok 244,601); the credits' "TUROK 2 TEAM"
page (248,103); the hub after the credits, by a portal's green glow (261,600).

Port tests: `rnc_test` (14,426 streams, 0 failed), `t2asset_test` (26,124 units, 0 mismatches,
the archive and the ROM backend), `t2asset_typed_test` (1,585 checks, 0 errors, three passes),
`gbi_test` (0 mismatches), `t2os_test` (trace hash `2b2775877e0d226b` on 3 runs),
`t2audio_test --direct 0` and `--direct 40` (audio_compare: max difference 0),
`t2audio_test --adec 0xC29AB4 7464` (27,136 samples), the swoosh self test (OK at the end of
every `--game` run).

ABI (`tools/port/abicheck.py --build OUT/build -j 4`: 3,077 TUs, 11,604 calls, 11,073 checked
against a definition, 181 function-pointer calls): harmful direct sites **0**, indirect 1 (the
known `Destructible_Hurt` store). `ptrdecls.py`: 0 left in `src/` (the strlen returns and
`include/functions.h` as before); `ptrargs.py` on a clean game-library build log: 0.
`f2ucheck.py`: 78 float -> unsigned conversions (79 in round 11). `n64viewcheck.py`: raw BAD 10,
stride BAD 3, suspect 44, a5's final list (each one's reason: [STABILITY.md](STABILITY.md) 8).

## 3. Merge resolutions and fixes (round 12)

| where | branches | resolution |
|---|---|---|
| docs/port/STATUS.md | all | every part section kept, in merge order (a5, a4, a1, a2, a3) |
| docs/port/PLAYTHROUGH.md | a1 (9.3 items, 9.4), a2 (9.3, 9.5), a3 (10) | all; 9.3's open list says what a1/a2 closed (one item 4) |
| port/game/t2debug.c `T2Port_LogOne` | a1 (the `actor+:` line inside `if (info != NULL)`), a2 (the action line) | both, a1's block first |
| tools/port/routes56.py | a1 (level 5: `L5_TOTEM`, `L5_BOSS`, the shredder, the `("actor", A)` aim), a2 (level 6: `_couplings`, `L6_GIVE`, the Mother, the hop) | both: `build_level56` keeps a1's per-level weapon/ammo and `extra_env`, then a2's `level == 6` block |
| port/app/game.cpp | a2 (`T2Port_GiveItems(f + 1)`), a3 (every frame-scheduled switch on `sFrame + 1`) | `T2_GIVE_ITEMS` on `sFrame + 1` too |
| the archive | a5 | rebuilt: sha1 `a9d14a47...` (a5's), `archive_check` OK; gendata rerun |

No `src/` file came from two branches; the nested `TARGET_PORT` scan finds no new nesting (the
13 nested blocks in `src/` are the same as at 046a67f9, none a second host body).

Fixes and additions on the merged tree (asm_diff 0):

| commit | what |
|---|---|
| 984f5daf | **`InventoryMenu_DrawItem`** (func_00424780, a3's finding): the key set 6 counts and the nuke parts were read at CPlayer's N64 offsets (0xA05/0xA06, 0x9FC); now `m_Data`'s `m_KeySet6LevelFlags` / `m_NukePartLevelFlags` at host offsets. **`Primagen_EnterMode24_2`** (func_004407AC, the module-2 twin of a2's func_004077AC fix): members instead of the raw 0x1FC/0xD4 actor offsets and the N64 deltas of `LevelScriptState` (compile-only library) |
| cc909b7f | `T2Port_Scheduled`: `T2_WEAPON`, `T2_FILL_AMMO`, `T2_BOSS_FRONT` as frame-scheduled values; `T2_GIVE_WEAPONS` and `T2_GIVE_ITEMS` on the schedule frame |
| 037dc74a | **fullrun.py levels 5 and 6** (`LEVEL_HOOKS`), `T2_ANCHOR` `L1/L2@N`, `T2_BOSS_FRONT` `,K,1` (a spot cycle for the Primagen's lair wait) |
| 0c3697ca, d5536688 | fullrun.py: **the hop to the Primagen 1,300 frames after the hub** (`--hop`): from 1,000 he rested where no spot ended his stage-3 wait; `T2_DRAW_FROM=F` or `A-B;...` (draw and shoot only there: the pictures of a 266,000-frame run) |

## 4. Next

1. **Remove the remaining debug aids of the continuous run**, in this order: the real Primagen
   entrance (Primagen keys 5 and 6 from levels 5 and 6 into their hub slots, then the hub's own
   way to 7602 instead of the hop); the hub's level-5 and level-6 doors by their keys (62's and
   68's level-5 keys: anchor those segments on the warp's load; 103's level-6 key); the items'
   real sources (the red laser cells in level 5's 91; the crystal ion capacitors: the ROM places
   one, in level 3's 66, for 16 couplings: **check on the N64** how many the couplings use);
   walk the teleported parts; then `T2_CHEATS` off.
2. **The City Totem** (110): failed 5/8 in this merge's continuous run (a3: won 2 of 4); it
   decides one of the ending's five totem flags.
3. **Hub door persistence** (a3: a door opened by its key slots is closed again on a later
   visit). What the code says: `Door_OpenEnter`/`Door_OpenUpdate` set the door's instance bit
   (`CScene__SetInstanceSavedBit`, host-correct index) and `Door_Advance` opens a door whose bit
   is set when the level loads, so the bit is lost between visits. fullrun.py saves *before* it
   fills the slots: test whether a later reload of the level flags from the last save (the pak
   save's `gLoadSavePersistentData`) drops it (fill the slots, then save), then watch level 60's
   bit block across a level.
4. **The talisman and flesh-eater portals and the 100% ending** (engine mode 16; PLAYTHROUGH.md
   10.5 maps the portals).
5. **Against the N64**: the Primagen's lair waits (where he rests decides whether the stage-3 wait
   ends: PLAYTHROUGH.md 11.1), the Mother's death warp (the hub straight in the continuous run,
   114 in `--level 6`), the capacitor count.
6. **The 4 views a5's checker left unresolved** (STABILITY.md 8) and the 119 jump (a2: the jump
   peaks 2 units under the couplings; not rechecked with a5's typed `m_JumpScale`, which reads the
   same value as z5's big-endian read did).
7. **This merged tree on the Mac** (a4 ran round 11's tree + its fix) and **windowed play with a
   real gamepad** (MACOS.md 12.6 documents the mapping; only headless runs so far).
8. `t2audio_test`'s game-path mode: open the archive (or the ROM backend) before the boot.

## Round 12, part a5: the N64-offset sweep, the player intelligence typed (branch names-R12_a5)

Detail: [STABILITY.md](STABILITY.md) section 8, [64BIT.md](64BIT.md) part 6.

- **`tools/port/n64viewcheck.py --build OUT/build -j 4`**: a libclang checker of the
  host-compiled code (only the `TARGET_PORT` side) for raw N64 offsets, N64 strides and
  pad-placed views of engine structs; classes BAD / suspect / moves / global / untyped / local /
  same. **24 functions fixed** (asm_diff 0): the blasts' push and reach, the mount and dismount,
  the projectile muzzle (its region pointer was cut to 32 bits), the turret's aim, the level
  actors' matrix and pooled timer, the impact sounds, **the water flood across region
  neighbours (it walked the vertex pointers)**, the camera viewport/hit flash/scope,
  `CCameraText__Reset` (`m_nId` pointer-wide), `CPlayer__CheckJumpOn`, the menu pool word in
  `CPlayer__Update`, and the sound voice pool (handles at `+0x7C`, the 0xCC voice stride).
  The rest is listed with a reason; 4 views are not resolved (STABILITY.md 8).
- **`CPlayerIntelligence` 0x1C-0x34 typed** (`m_SpeedScale`, `m_JumpScale`, `m_Unk24`,
  `m_Unk2C`, `m_Unk34`): the archive swaps them, the big-endian reads of z5 are gone.
  **New archive sha1 `a9d14a47dba3f75ab30a0e4cdfdb1cee56977d22`**: rebuild it (mkarchive) and
  rerun gendata with this tree; the old archive + this binary cannot jump.
- Results (Linux, PIE): the 3 demos' `T2_DET_LOG` identical to 046a67f9 (523,287 lines);
  `playthrough.py --level 1..4` all to the hub (levels 1, 3 identical logs; 2 and 4 differ
  from the blast fixes in fights, same outcomes); soak: levels 40 runs x 25,000 + new game 10 x 36,000, **0 crashes**.
- **Merge notes**: `include/structs/intelligence.h` (whole-tree asm_diff done), 27 files in
  `src/us/code` (none of them a level/boss script); nothing in `port/` or `playthrough.py`.

## Round 12, part a4: the round-11 merged tree on macOS, flush-to-zero, level 2 (branch names-R12_a4)

No `src/`/`include/` change. Detail: [MACOS.md](MACOS.md) section 14. One commit in `port/`:

- 8e12748c **`T2_AIM_AT` used libm's `atan2f`** (port/game/t2debug.c `T2Port_AimAt`): its last bit
  differs between macOS and glibc, so level 2's scripted shots flew 1 ulp apart (the Mac/Linux
  divergence at 42,124 in round 11, 42,184 on this tree). Now `T2Port_Atan2f`, like `T2_AIM_NODE`
  and `T2_FACE_PITCH`. **Linux's level-2 det trace changes from frame 20,416** (the events and
  the hub frame do not). Both level-2 routes (round 11's and this one) are then identical on both
  hosts to their end.
- **arm64 flush-to-zero works**: the game thread runs with FPCR FZ set; boot + the 3 demos are
  identical to Linux (`T2_DET_LOG`); with FZ cleared by lldb demo 2 diverges at gf 426 (z5's
  denormal velocity), so the path is live.
- **The whole round-11 battery on the Mac** (headless `--offscreen-gl`, exit 0, 0 dlcheck): levels
  1-4 to the hub, the Mother, the Blind One, the Mantid Queen, the Primagen with the ending cinema
  and the credits, save/load, `--audio` (RMS 3,399.5 = Linux); **Mac = Linux frame for frame in
  every one** (0.84 M det lines with round 11's level-2 route, 0 differ). Soak (lldb): 49 runs, 1.31 M frames, 0 crashes.
- Merge notes: `port/game/t2debug.c` (one call in `T2Port_AimAt`; a1-a3 may touch the same file
  elsewhere). The scripts are not committed (`OUT/r12/`, box `/root/R12_a4mac/r12/`).

## Round 12, part a1: level 5 complete (branch names-R12_a1)

**Level 5, the Hive of the Mantids, plays from the hub portal back to the hub** with both
objectives, the Hive Totem saved and the Mantid Queen dead, by the game's own logic (no debug
warp; [PLAYTHROUGH.md](PLAYTHROUGH.md) section 9.4):

```sh
python3 tools/port/playthrough.py DIR --level 5 && . DIR/teleport.env
SDL_VIDEODRIVER=offscreen T2_EVENT_LOG=1 T2_STATE_LOG=500 build/app/turok2 --archive turok2.o2r \
    --rom ../baserom.us.z64 --game --pak-blank --frames 48000 --input @DIR/playthrough.txt
```

124 at 17,300; "force field down." 18,913 (the four pylons); the game's warp 5201 into 84 (no
hop); the three level 6 keys (84, 87, 89); "embryo destroyed." 24,607 (94) and 27,307 (95); "all
embryos destroyed. mission complete." 29,207 (96); the four satchel charges from 91 into the
computer's sockets in 93: "warning, five seconds until detonation." 31,001, "master computer
destroyed. mission complete." 31,359; the exit 7500 to the Hive Totem (113, 32,000), saved (10/8
kills); the Mantid Queen (33,598), her death (mode 16) at 42,095; **the hub at 43,500**; exit 0.

**No host bug was behind level 5's blocks**: each was a route gap. The pylons are turrets whose
hit boxes are ~170 units up (z4 aimed at 100); the embryos sleep inside cocoons (destructibles)
whose death wakes them; the computer is not shot but fed four satchel charges (item 0x100E,
object 599, in level 91) at its four sockets (actions needing that item); "the instant death in
93" was the route's aim point over the pit round the computer room (a fall, which kills whatever
`T2_CHEATS` says). The route runs on the shredder (weapon 9) with `T2_FILL_AMMO=100`.

Tools (debug only): `T2_ACTOR_LOG` adds an `actor+:` line per actor (body flags, an action's auto
radius and needed pickup, a destructible's health, the AI's hurt/step/mode callbacks as offsets
from `T2Port_LogOne`); `routes56.py` steps take `"aim": ("actor", A)`.

Files others may touch: `tools/port/routes56.py` (a2 edits the level-6 half: my changes are the
level-5 half, `build_level56`'s weapon/ammo per level and its level-5 totem/boss block, and the
`("actor", A)` aim); `port/game/t2debug.c` (`T2Port_LogOne` only); `docs/port/PLAYTHROUGH.md`
(9.3 items 2 and 4, new 9.4). Nothing in `src/` or `include/`.

Next (level 5): its level key and Primagen key pickups; the flesh-eater portal 82 and the talismans
76 and 92 (a3's talisman work).

## Round 12, part a2: level 6 to its end, the Mother, the Primagen (branch names-R12_a2)

Detail: [PLAYTHROUGH.md](PLAYTHROUGH.md) section 9.5. One `src/` change (TARGET_PORT, `tools/asm_diff.py
--base 046a67f9 -j 8` 0); no `include/` or archive change.

- **`playthrough.py DIR --level 6` completes the Lightship** (112,000 frames, Linux headless PIE):
  the four power generators (115, 117, 119, 123: "all power generators recalibrated. mission
  complete." 30,481), the three assembly plants (121, 122, 123: "all assembly plants destroyed.
  mission complete." 29,625), the four force field generators ("force field down." 31,265), the
  exit 7601 to **the Mother** (level 3, ~31,700; her death ~48,800), back to 114, its pad 7601
  to **the hub** (53,000), a hop (`T2_WARP_TO=7602@54000`) to **the Primagen**: his death (mode 29)
  at 84,218, **the ending cinema** (86,000), **the credits** 90,234-104,324, **the hub** at 105,000.
  Exit 0.
- **How the objectives work**: the couplings (object 288) and the force field generators (object
  232) are actions the player walks into (`CScene__CheckAutoActivation`, radius 41); each coupling
  uses a **crystal ion capacitor** (item 4231, `Action_CheckRequiredItem`), plant 3 is two laser
  power crystal slots (a red laser cell, 4229, each). The items come from levels 3-5; the route
  gives them with the new `T2_GIVE_ITEMS=ID[:N][,...][@F][;...]`. **Open**: the game places one
  capacitor for 16 couplings (one used per coupling, a nibble holds 15): N64 check needed.
- **Fixed: `Primagen_EnterMode24`** (func_004077AC): an N64-delta view of `LevelScriptState` from
  its `timer` (`dest.y`, `home.y`, `bossState`) missed `dest.y` on the host; stage 2's landing
  path (bossState 6, mode 24) then hung for good. z2's fresh recipe never took that path.
- **Recipes**: 119's couplings sit 154 units above the floor (approached from the air: the jump
  peaks 2 units too low, open); the Primagen's stage-2 lair wait is a fragile line of sight past the
  lair door, and after the Mother he rested where no safe spot saw him with a hop at 53,700: the
  hop frame is tuned (54,000: the wait ends by itself). Debug: `T2_BOSS_FRONT=D,EVERY,K`,
  `T2_BOSS_FRONT_PROBE`, `T2_BOSS_FRONT_SCAN`, `T2_AIM_NODE=...@3/4`, the action line of
  `T2_ACTOR_LOG`.
- **Merge notes**: `tools/port/routes56.py` (level 6 only: `L6_*`, the `level == 6` block of
  `build_level56`; part a1 edits the level-5 half), `port/game/t2bosslog.c` (`T2Port_GiveItems`,
  `T2Port_BossFront`, `T2Port_AimNode`'s `@`), `port/game/t2debug.c` (`T2Port_LogOne`),
  `port/app/game.cpp` (one call), `docs/port/PLAYTHROUGH.md` (appended 9.5; 9.3 untouched).

## Round 12, part a3: one continuous playthrough, the keys and the hub (branch names-R12_a3)

Detail: [PLAYTHROUGH.md](PLAYTHROUGH.md) section 10. Port/tools only (no `src/`, `include/` or
archive change).

- **One process from a new game through levels 1, 2, 3 and 4 in order**:
  `tools/port/playthrough.py RUN --all` (tools/port/fullrun.py), 146,000 frames, ~76 s headless,
  `--pak-dir`. Every level through the hub's own portal, every objective scored by the game, the
  Harbor, Marsh and Cave Totems won, **the Blind One dead**, back to the hub, and **a save after
  each level by walking into the hub's save portal** (warp 999 -> 97, Adon's cinema, SAVE, GO
  BACK): 4 saves, note "TUROK 2" checksum OK, clock 0:37:26. **The City Totem (110) is the weak
  step**: won 11/8 in two runs, failed (4/8, 7/8) in two (its flag matters for the ending only).
- **How the hub portals open**: not in code (the portal warps are free) but by doors: each
  portal's way out of the central plaza is a door (object 426) that opens when its key slots
  (actions needing items 4310-4350) are filled ("access to level N granted."). The old
  `--level 2/3/4` routes teleported past them. The run collects the 3 level-2, 3 level-3 and 3
  level-4 keys, Primagen keys 1-4, places them, and walks through the level 2 and 3 doors into
  their portals (level 4: the door is walked through, the last stretch to the portal is still a
  teleport).
- `T2_ANCHOR=L@N;...` (a schedule clock re-anchored when a level is reached: totems ending at a
  different frame no longer shift the rest of the route), `T2_FACE_NEAREST=R@L1,L2,...`,
  `T2_SAVE_PORTAL=F1,F2,...`.
- Found: a hub door opened in one visit is closed again in a later one (its slots stay used):
  likely a port bug in the door's persistent state; `InventoryMenu_DrawItem` reads CPlayer at N64
  offsets (0xA05/0xA06/0x9FC). Not fixed.
- Not done: the talisman and flesh-eater portals (mapped in section 10.5), the level-5 keys of 62
  and 68, the level-6 key of 103.

Merge notes: port/app/game.cpp (the debug calls now take `sFrame + 1`, the schedule frame; other
branches adding a frame-scheduled switch should pass the same), port/game/t2debug.c
(`T2Port_ScheduleFrame`, the two list parsers), tools/port/playthrough.py (`--all`, 8 lines),
tools/port/fullrun.py (new; levels 5/6 append through `LEVEL_HOOKS`), PLAYTHROUGH.md section 10.

## Round 11: the merged state

Round 11 merged five branches, in this order: z5 (fidelity: demos 1 and 3 identical to the N64's
RAM to their end, the view bob, static object events, the font colour, flush-to-zero, the jump),
z1 (level 2 to its end), z2 (every boss beaten, the Primagen's ending and the credits), z3
(levels 3 and 4 to the hub) and z4 (the round-10 tree on macOS, levels 5 and 6 mapped). Nothing
came twice in `src/`; the shared debug files were unioned (section 3). The merged tree needed
one real fix of its own: **the level-122 crash** that z4 found on both hosts was a family of N64
instance views whose flags fell on the host's geometry size (a destroyed object then loaded a
model unit cut short); fixing it and its siblings lets level 6 run on past 122. Two recipes were
re-tuned (the City Totem's aim, the Blind One's teleport in `--level 4`, the Mantid Queen's
`T2_BOSS_FRONT` period). This page says how to build and run the result, what the game does now,
and what comes next. The part sections below keep each branch's own notes (detail pages:
[FIDELITY.md](FIDELITY.md) section 6 (z5), [PLAYTHROUGH.md](PLAYTHROUGH.md) sections 6.5-6.6 (z1),
7 and 8 (z3) and 9 (z4), [GAMEPLAY.md](GAMEPLAY.md) section 12 (z2), [MACOS.md](MACOS.md) section
13 (z4)); round 10 and earlier follow as history.

The matching build is unchanged: `tools/asm_diff.py --base 2df5517d -j 8` reports 0 differences
over all 3,938 files after every merge and every fix, and a clean `make clean && make setup &&
make build && make verify` gives the identical ROM (`build/turok2.us.z64: OK`).

## Round 11: build and run

```sh
# 0. the matching build first: gendata and asm_diff read its ELF; keep a copy as build/ref.elf
make setup && make -j8 build && make verify && cp build/turok2.us.elf build/ref.elf

# 1. the archive, format 2.0. Unchanged this round (rebuilt from the merged tree: the same bytes)
python tools/assets/mkarchive.py baserom.us.z64 -o OUT/turok2.o2r -j 4     # ~12 s, sha1 b6b9d484...
python tools/assets/archive_check.py OUT/turok2.o2r baserom.us.z64          # OK, format 2.0

# 2. configure IN A FRESH BUILD DIR (a PIE is the Linux default; an old cache may keep
#    T2_LOW_MEMORY=ON); the game library AND the runtime/audio libraries (gendata reads all three)
rm -rf OUT/build
cmake -S port -B OUT/build -G Ninja -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ \
      -DCMAKE_BUILD_TYPE=Debug                      # -DT2_LOW_MEMORY=ON: the old -no-pie build
ninja -C OUT/build -j8 turok2_game t2runtime t2audio

# 3. the game data as C into port/gen/ (gitignored). Drop the cache after ANY header change.
#    RERUN IT this round: z3 changed structs/camera.h (CCameraText.m_nId is sptr).
rm -f OUT/decls.json
python3 tools/port/gendata.py --build OUT/build --cache OUT/decls.json -j 4      # ~1 min

# 4. re-run cmake so data.cmake sees port/gen/*.c, then everything (turok2, t2audio_test and the
#    compile-only module-2 library, which checks that every C file builds for the host)
cmake OUT/build && ninja -C OUT/build -j8

# 5. run headless (Linux: SDL's offscreen driver, no window). High memory (and a PIE) is the default.
cd OUT && SDL_VIDEODRIVER=offscreen T2_SHOT=shot.ppm T2_SHOT_EVERY=300 \
    build/app/turok2 --archive turok2.o2r --rom ../baserom.us.z64 \
    --game --draw --pak-blank --frames 26000
```

The archive: 26,124 units, sha1 `b6b9d484e6f97718e48ddc6d7f0ce9188dbb11c5` (round 9's bytes;
`archive_check.py`: OK, format 2.0). gendata: 1,534 data symbols (1,529 typed from a C
declaration), 402 pointer targets, 386 zero .bss symbols, 3 typed .bss structs with their N64
tail.

**The scripted runs** (`turok2 --archive turok2.o2r --rom ../baserom.us.z64 --game --pak-blank`
plus `--draw` for pictures; "new game" is `START@4400 A@4500 A@4700 A@4800`; "boss env" is
`T2_GIVE_WEAPONS=1 T2_WEAPON=10 T2_AIM_NODE=boss T2_CHEATS=1 T2_FILL_AMMO=600 T2_BOSS_LOG=1`).
The level runs come from `tools/port/playthrough.py DIR [--level N] [--direct]`, which writes
`DIR/playthrough.txt` and `DIR/teleport.env`; run them as
`. DIR/teleport.env && T2_EVENT_LOG=1 T2_STATE_LOG=500 turok2 ... --input @DIR/playthrough.txt`:

| what | how | frames |
|---|---|---|
| boot + the 3 attract demos | (none) | 26,000 |
| **level 1, the Port of Adia** | `playthrough.py DIR --fight 10` ([PLAYTHROUGH.md](PLAYTHROUGH.md) sections 1-5) | 27,000 |
| **level 2, the River of Souls** | `playthrough.py DIR --level 2` (sections 6, 6.5) | 64,000 |
| **level 3, the Death Marshes** | `playthrough.py DIR --level 3` (section 7) | 36,000 |
| **level 4, the Lair of the Blind** (with the Blind One) | `playthrough.py DIR --level 4` (section 8) | 46,000 |
| level 5, the Hive of the Mantids (a tour) | `playthrough.py DIR --level 5` (section 9; tools/port/routes56.py) | 33,000 |
| level 6, the Lightship (a tour) | `playthrough.py DIR --level 6` (section 9) | 33,000 |
| a level straight from its first sub-level | `--level 3 --direct` / `--level 4 --direct` (no hub, no cinemas) | |
| **the Mother** | the input file below, `T2_WARP_TO=-4` + boss env | 36,000 |
| **the Blind One** | new game + `Z@14000..90000`, `T2_WARP_TO=7401 T2_TELEPORT=14000:-400,204.8,0` + boss env | 25,000 |
| **the Mantid Queen** | new game + `Z@14000..200000`, `T2_WARP_TO=7501 T2_BOSS_FRONT=450,200` + boss env | 30,000 |
| **the Primagen, the ending, the credits** | new game + `Z@14000..200000`, `T2_WARP_TO=7602 T2_BOSS_FRONT=450` + boss env | 62,000 |
| save | `--pak-dir DIR`, new game + `A@13250 A@13400`, `T2_SAVE_PORTAL=13100` | 14,100 |
| load | `--pak-dir DIR` (the same DIR), `START@4400 A@4500 DD@4700 A@4800 DU@4900 A@5000 A@5200 A@5400` | 6,500 |
| demos against the N64 | below | 26,000 |

`T2_BOSS_FRONT=D[,EVERY]` (round 11 z2) puts the player D units in front of the Queen's open
weak spot every EVERY frames (between her waves), or beside the Primagen's lair after EVERY
frames of his stage-3 wait ([GAMEPLAY.md](GAMEPLAY.md) section 12).

The Mother recipe (one input entry per line, a single line is cut at its buffer):

```sh
cd OUT && (echo "START@4400 A@4500 A@4700 A@4800 Z@14000..90000 CL@19000..90000";
  for s in $(seq 19000 600 89000); do echo "CD@$s..$((s+250))"; done) > mother_end.txt
SDL_VIDEODRIVER=offscreen T2_WARP_TO=-4 T2_GIVE_WEAPONS=1 T2_WEAPON=10 T2_AIM_NODE=boss \
  T2_CHEATS=1 T2_FILL_AMMO=600 T2_BOSS_LOG=1 T2_STATE_LOG=200 \
  build/app/turok2 --archive turok2.o2r --rom ../baserom.us.z64 --game --pak-blank \
  --frames 36000 --input @mother_end.txt
```

The demos against the N64's RAM ([FIDELITY.md](FIDELITY.md) sections 5-6; the emulator side
runs ~1,000 frames a minute, once per demo range):

```sh
xvfb-run -a -s "-screen 0 1024x768x24" python3 tools/port/n64trace.py --rom baserom.us.z64 \
    --from 1980 --to 3300 --actors > n64_d1.det                  # demo 1
xvfb-run -a -s "-screen 0 1024x768x24" python3 tools/port/n64trace.py --rom baserom.us.z64 \
    --from 4915 --to 7870 --actors > n64_d23.det                 # demos 2 and 3
cd OUT && SDL_VIDEODRIVER=offscreen T2_DET_LOG=1 T2_DET_GF=1 T2_DET_ACTORS=1 T2_DET_FROM=5400 \
    build/app/turok2 --archive turok2.o2r --rom ../baserom.us.z64 --game --pak-blank \
    --frames 26000 > port.log
python3 ../tools/port/detdiff.py ../n64_d1.det port.log --shift 0 --skip rng,flags
python3 ../tools/port/detdiff.py ../n64_d23.det port.log --level 131 --shift 0 --skip rng
python3 ../tools/port/detdiff.py ../n64_d23.det port.log --level 109 --shift 0 --skip rng
```

**The tools** (Linux box):

- `tools/port/playthrough.py OUTDIR [--fight WEAPON] [--level 1..6] [--direct]`: the level runs
  above (levels 5 and 6 from `tools/port/routes56.py`); `tools/port/approach.py` gives a
  teleport point facing a warp region or a point.
- `tools/port/soak.py --exe OUT/build/app/turok2 --archive OUT/turok2.o2r --rom baserom.us.z64
  --out DIR --kind attract|newgame|warp|levels --seeds A:B [--frames N] [--both-memory] [--jobs N]
  [--debugger gdb|lldb]`: random but valid input under gdb (lldb on macOS), crashes grouped by
  their top frames, hangs interrupted and classed by the busy thread (`crashes.txt`,
  `summary.json`, a `cmd.sh` per run). `attract` (no input: use `--frames 100000`), `newgame`
  (random play from 13,000), `warp` (random warp ids), `levels` (every level id 1-164 warped
  into, four per seed: seeds 0-40, `--frames 25000`) ([STABILITY.md](STABILITY.md)).
  `--both-memory` needs a `-DT2_LOW_MEMORY=ON` build.
- `tools/port/n64trace.py` + `tools/port/detdiff.py`: the N64's per-game-frame state from RDRAM
  (mupen64plus's debugger, a breakpoint at `CEngineApp__Update`; `--actors` adds the actors and
  the `cam` line, `--calls NAME|ADDRESS --args REG:TYPE:MEMBER[:f|d|x]` traces a function's
  arguments) against the port's `T2_DET_LOG=1 T2_DET_GF=1 T2_DET_ACTORS=1` trace, aligned by
  `gFrameCountGameplay`. `xvfb-run -a ... tools/port/refshots.py ...`: the N64 reference
  screenshots and RDRAM dumps. Captured frames, traces and dumps are never committed.

Options (`turok2 --game`):

- `--input`: `BUTTON@F`, `BUTTON@F1..F2` (held), `SX=V@F1..F2` / `SY=V@...` (stick), `@FILE`
  ([INPUT.md](INPUT.md) section 6). The stick turns, C-up/C-down move, C-left/C-right strafe,
  R jumps (**the jump leaves the ground now**, z5: any recipe that presses R moves differently
  from round 10), Z fires, A/B hold the weapon wheel, L the map, START pauses; the D-pad moves in
  menus.
- `--pak-blank`, `--pak-dir DIR` (`DIR/turok2_pak<N>.mpk`, N64 byte order; `tools/port/mpk_saves.py
  FILE.mpk` checks the notes), `--no-pak`.
- `--audio` / `--wav out.wav`; **high memory and a PIE are the default** (`-DT2_LOW_MEMORY=ON`
  brings back `-no-pie` and `--low-memory`); `--offscreen-gl` (**macOS**: pixels with no window);
  `--no-window`; `--bench [FROM]`, `--pace` (`--pace --audio`: a simulated 22,050 Hz device counts
  underruns) ([PERF.md](PERF.md)).
- Debug switches (port/game/t2debug.c, t2bosslog.c, t2det.c and game.cpp, **debug only**):
  - state: `T2_STATE_LOG=N`, `T2_ACTOR_LOG=N`, `T2_ACTOR_TYPES=t,...`, `T2_BOSS_LOG=1` (the boss,
    its hits with the boss's `m_VisBits` and AI mode, a `bosstrace:` line on every boss
    mode/phase change), `T2_PICKUP_LOG=1`, `T2_EVENT_LOG=1` (every level event with the
    objective counters of the level's own hub: levels 1-6 use `Mission_LevelObjectivesDone`'s
    link group types 2/3, 4/5, 6/7, 8 (the second number -1), 9/10, 11/12; the `totem:` line:
    state 0 fight, 1 saved, 2 failed), `T2_SCENE_LOG`, `T2_MENU_LOG=1`, `T2_PARTICLE_LOG=N`;
  - determinism: `T2_DET_LOG=N` (`T2_DET_FROM`; `T2_DET_ACTORS`: the actors, the weapon actor, the
    camera (`cam`) and the live particles; `T2_DET_GF=1`: one line set per game frame;
    `T2_DET_PARTICLES=1`: the particle pool; the line ends with an actor hash and a particle-pool
    hash, which now also covers the pool's lists), `T2_RNG_TRACE=F1..F2`;
  - moving and combat: `T2_WARP_TO=<id>[@frame][,...]` (a negative id is `~level`),
    `T2_TELEPORT=F:x,y,z[,rotY][!][;...]` (`!`: also fire the region's enter triggers),
    `T2_WAKE_ENEMIES=R`, `T2_FACE_NEAREST=R[@LEVEL]` (+ `T2_FACE_PITCH=1`: the camera pitch too;
    `T2_FACE_STICKY=1`: keep the last target while it is at most 25 % farther),
    `T2_AIM_NODE=MASK|boss|-1|actor:N[,R][@LEVEL]`, `T2_BOSS_FRONT=D[,EVERY]`,
    `T2_AIM_AT=F1-F2:x,y,z[;...]`, `T2_AIM_ENEMY=F1-F2[,R[,A]][;...]` (A: level actor A first),
    `T2_CHEATS=MASK` (1 invincibility), `T2_FILL_AMMO=N`, `T2_GIVE_WEAPONS=1`, `T2_WEAPON=<n>`,
    `T2_WEAPON_LIST=1`;
  - progression: `T2_SAVE_PORTAL=F`, `T2_DOOR_OPEN=F`;
  - capture: `T2_SHOT=FILE T2_SHOT_EVERY=N T2_SHOT_FROM=F`, `T2_DLDUMP_AT=<frame>`
    (+ `T2_DLDUMP_VTX=1/2/3`), `T2_ANIM_EVENTS=0`, `T2_V2P_TRAP=1`, `--skip-pak-check`,
    `--start-mode N`.

## Round 11: what the game did (Linux, headless, PIE, high memory, final binary)

Every run below exits 0.

- **The boot loop** (26,000): the pak check (2), legal (298), expansion pak (1,070), Acclaim
  (1,362), the Iguana jungle (1,850), the title (4,212), **demo 1 5,467 -> legal 8,157, demo 2
  13,037, demo 3 19,922, the title again 24,751** (round 10's frames). **The legal screen's and
  every on-screen text's colour is the N64's** (z5: the font combiner's ENV colour).
- **The demos against the N64's RAM** (the merged tree, against z5's N64 traces): **demo 1
  identical from game frame 1 to its end (gf 1,128)** and **demo 3 identical from gf 1 to its
  end (gf 344)** (`detdiff.py`: "no difference over the common game frames"); **demo 2
  identical to gf 607**: from gf 608 only the player's health differs (24,695 against the N64's
  24,717), the N64's uninitialised heap read in `CGameObjectInstance__Construct` (an enemy's
  `m_AI.m_dwFlags` before `CAI__Setup`: [FIDELITY.md](FIDELITY.md) 6.3, not matchable).
- **Level 1, the Port of Adia** (`--fight 10`, 27,000): "all distress beacons activated. mission
  complete." (14,751), "the children are safe. mission complete." (17,499), the Harbor Totem from
  19,000, **the defence won (8/8) by 21,000**, **the hub by 22,000**.
- **Level 2, the River of Souls** (`--level 2`, 64,000): soul gate 1 (21,313), the first sister
  banished (133, 29,863), the second (134, 36,603), **"all soul gates destroyed. mission
  complete."** (42,063), the crypt keys, **"all sisters of despair banished. mission complete."**
  (135, 52,827), the exit 7200 to **the City Totem (110) at 55,500**, its defence **won (8/8)
  by 61,500**, **the hub by 62,000**. Without `T2_FACE_STICKY` the defence failed at 6/8 on the
  merged tree (section 3).
- **Level 3, the Death Marshes** (`--level 3`, 36,000): the prisoners ("all prisoners free.
  mission complete." 24,243), the armories ("armories destroyed. mission complete." 25,097), the
  Marsh Totem (111) from 26,500, its defence won (state 1), **the hub by 31,500**.
- **Level 4, the Lair of the Blind** (`--level 4`, 46,000): the vent chambers ("thermal vent
  sealed." 19,979 / 22,277 / 25,199: **"blind ones trapped. mission complete."**), the Cave
  Totem (112) from 27,000, its defence won by ~31,000, the Blind One's level (31,264), **her
  death (mode 9) at 41,113**, **the hub by 42,000**.
- **Level 5** (`--level 5`, 33,000): the portal, the cinemas, 124 at 17,500; the pylons stay up, so a debug hop
  (`T2_WARP_TO=5201`) enters 84 at 20,500; then the game's own warps through 84-91, 94-96 and
  **93 (28,500)**, where the player dies (level 164, back to 91 at 30,000 with a life less), and
  the exit 7500 back to 124 (30,500). No objective is scored yet (z4's route, unchanged).
- **Level 6** (`--level 6`, 33,000): the portal, the cinemas, 114 at ~19,000, the sub-levels
  115-121, **122: "assembly plant destroyed." (27,793) and "door open." (27,821) with no crash**
  (z4's crash, fixed: section 3), then **123 (29,000)** and back to 114 (31,500). The
  generators' couplings and the force field generators score nothing yet.
- **The Mother** (36,000): her death (mode 30) at 27,459, the defeat cinema (31) at 27,635,
  **the hub by 29,000** (z5's frames: round 10's 30,641 moved with z5's fixes).
- **The Blind One** (25,000): **her death (mode 9) at 22,623**, the cave's collapse, **the hub by
  23,500**.
- **The Mantid Queen** (`T2_BOSS_FRONT=450,200`, 30,000): steps 1-8, **her death (mode 16) at 26,907**, her
  death cinema, **the hub by 28,500** (z2's branch, with the default period: 33,507 and 35,000).
- **The Primagen** (`T2_BOSS_FRONT=450`, 62,000): stage 2's wait passes at 26,410 and
  stage 3's at 34,042, **his death (mode 29) at 38,714**, his death cinema, **the ending cinema
  (levels 11 and 10) from 40,000**, **the credits (engine mode 15) from 44,730 to 58,820**, **the
  hub at 58,828** (z2's branch: 32,828, 39,000 and 53,000; the recipe needs 62,000 frames now).
- **Save -> title -> load**: "SAVE WAS SUCCESSFUL"; the note (`mpk_saves.py`): NT2E 51 "TUROK 2"
  ext A, 90 pages, big-endian, checksum OK, health 100, lives 3, clock 0:02:20, 663 ISets of
  level flags; a new boot with that pak loads it: level 51 at 5,500.
- **Audio** (`--audio --wav`, 9,000 frames): 150.17 s, peak 32,768, RMS 3,399.5 (round 10:
  3,402.2; demo 1 now plays as on the N64).
- **Soak** (PIE, no `--draw`): attract 100,000 (1 run, 99,960 frames), new game seeds 0-19 x
  36,000 (20 runs, 720,000 frames), levels seeds 0-40 x 25,000 (every level id 1-164 warped into,
  41 runs, 1.02 M frames): **0 crashes, 0 hangs**.
- **macOS** (round 12 a4): the whole battery above runs on the Mac and equals Linux frame for
  frame (level 2 after the `T2_AIM_AT` fix); z5's arm64 flush-to-zero is live (MACOS.md 14).

Screenshots (640x480, checked by eye, not committed): the legal screen in the N64's text colour (701);
level 3's "ARMORY DESTROYED." (25,501); level 4's "VENT CHAMBER LOCATED. THERMAL VENT SEALED."
(20,001); the City Totem's defence (56,001-61,501) and the hub after it (62,501); the Mantid
Queen under plasma fire (26,800), her fall (26,900), Turok beside her body in the death cinema
(27,402) and the hub portal (28,501); the Primagen under plasma fire (38,501), his death cinema
(39,001), the ending (Adon 41,501, Turok 43,501) and the credits (47,003, 50,003).

Port tests: `rnc_test` (14,426 streams, 0 failed), `t2asset_test` (26,124 units, 0 mismatches,
the archive and the ROM backend), `t2asset_typed_test` (1,585 checks, 0 errors), `gbi_test` (0
mismatches), `t2os_test` (trace hash `2b2775877e0d226b` on 3 runs), `t2audio_test --direct 0`
and `--direct 40` (audio_compare: max difference 0), `t2audio_test --adec 0xC29AB4 7464 out.raw`
(27,136 samples), the swoosh self test (OK, 0 failed).

ABI (`tools/port/abicheck.py --build OUT/build -j 4`: 3,077 TUs, 11,551 calls, 11,048 checked
against a definition, 181 function-pointer calls): harmful direct sites **0**, indirect 1 (the
known `Destructible_Hurt` store). `ptrdecls.py`: 0 left in `src/` (the strlen returns and
`include/functions.h` as before); `ptrargs.py` on a clean game-library build log: 0.
`f2ucheck.py`: 79 float -> unsigned conversions, none new.

## Round 11: merge resolutions and fixes

| where | branches | resolution |
|---|---|---|
| port/game/t2bosslog.c `T2Port_AimNode` | z1 (int return, `actor:N`), z2 (`T2Port_BossFront`), z3 (`SPEC@LEVEL`) | all three; z3's early `return;` is `return 0;` |
| port/app/game.cpp | z1 (`int T2Port_AimNode`), z2 (`T2Port_BossFront` and its call) | both declarations |
| port/game/t2debug.c `T2Port_LogEvent` | z3 (`kTypes`), z4 (`kObjTypes`) | the same per-level types: one table, z4's (`T2Port_HubLevelOf` maps 124 to level 5; level 4's missing second counter prints -1, z3 printed 0); z1's `T2_FACE_PITCH` and `T2_AIM_ENEMY ,A` kept |
| port/game/t2det.c | z5 (the `cam` line), z4 (the particle pool's lists in the hash) | both (git merged them) |
| tools/port/playthrough.py | z1 (level 2 to the end), z3 (levels 3/4, `--direct`), z4 (levels 5/6) | `--level 1..6` and `--direct`; level 5/6 through routes56.py |
| docs/port/PLAYTHROUGH.md | z1 (6.5, 6.6), z3 (7, 8), z4 (its section 7) | z4's section is **9** here (routes56.py and MACOS.md point to it) |
| docs (STATUS.md) | all | every part section kept, in merge order |
| the archive | none | rebuilt from the merged tree: the same sha1 `b6b9d484...`; gendata rerun (z3's camera.h) |

No C file the merges touched has a second host body of one function (the nested `TARGET_PORT`
check of round 10: z5's ten files, z1's `Shadow_DrawBlob`, z2's two files).

Fixes on the merged tree (asm_diff 0):

- d129e748 **The level-122 crash** (z4: `Model_Decompress` on the decompressor thread after
  "assembly plant destroyed.", both hosts). A watchpoint on the plant's geometry size found the
  writer: `Destructible_GoEnter` (func_00265B58) views the instance with the N64 pads, and after
  the pointer-wide `def` at 0x14 its `flags` (0xD4) fall 8 bytes late on the host, on
  `m_rpGeometryHeaderSize`. `flags &= ~0x100` cut the plant's geometry size from 0x160 to 0x60;
  the next model request loaded 0x60 of the unit's 0x2E0 unpacked bytes and `Model_Decompress`
  read its level-of-detail table past them. The same view in `Destructible_GoDeathEnter`,
  `Destructible_GoUpdate` (+ its CAI byte 0xC7, `m_AnimRepeatedCount`),
  `Destructible_GoDeathUpdate`'s CAI byte, `EnemyAI_NukedFrozenRemove` (also its bit-fields,
  laid out from the other end on a little-endian host) and `func_00268210` (+ the owner at
  0x1A8): members at their host offsets.
- 47890c05 **The same pattern elsewhere** (a sweep for 0xD4 views): `func_00238258`,
  `EnemyAI_StopSoundAndFree`, `GameObject_IsNearFloor` (it read `m_rpObjectHeader & 0x1000`),
  `func_00268BE0`, `PlayerShock_Enter` (every field was an N64 offset: the shock never set its
  timer) and `func_0025359C` (the player list and the controllers' rumble state by member).
- 234e21a7 **Recipes**: `T2_FACE_STICKY=1` for the City Totem (two attackers at the same distance
  took the aim in turns: the defence failed at 6/8); `--level 4`'s boss teleport at 32,000 (the
  Cave Totem now ends ~31,000 and the warp to the Blind One's level lands at 31,264, after the old
  31,000 teleport).
- The Mantid Queen's recipe: `T2_BOSS_FRONT=450,200` (every 200 frames). With the default 600
  she stayed in step 4 (spot 1) from 18,087 to the end of a 40,000-frame run (one spot-1 hit every
  ~2,000 frames: her body and spot 2 took the rest); z2's branch passed it by 32,607.

## Round 11: next (as written then)

1. **Level 5's objectives**: the pylons (what brings the hive's force field down: object 139),
   the three embryos and the master computer with real aims; the instant death in level 93.
2. **Level 6**: the generators' couplings (object 288, "recalibrate": a shot, a touch or the use
   button), the force field generators (object 232), then 7601 -> the Primagen from the level.
3. **Walk the teleported parts** of every route (the sub-levels hop by `T2_TELEPORT`: level 2's
   125 with its 45 enemies, level 3's 104 and the vent chamber doors of 106-108).
4. **The talisman and flesh-eater portals and the level keys** (73-77/92, 79-83), the totem 113.
5. ~~The Mac's level-2 divergence at frame 42,124~~ and z5's arm64 flush-to-zero: done in round
   12 (a4: libm's `atan2f` in `T2_AIM_AT`; FZ verified).
6. **More N64-pad instance views**: this round's sweep covered the 0xD4 flags; the same pattern
   (a pointer field before a padded member, in a file with no `TARGET_PORT`) is worth one more
   pass (`tools/port/hostview.py`).
7. The intelligence's speed and jump fields (`CPlayerIntelligence` 0x1C/0x20 are `u8 pad08[]`,
   left big-endian by the archive's swap plan): type them in `swap_spec` and drop the big-endian
   reads.
8. **The 100% ending** (engine mode 16) and the Primagen's stage-3 line of sight against the N64.
9. **A windowed run with a real gamepad** (MACOS.md 12.6 documents the mapping; only headless runs
   so far).
10. `t2audio_test`'s game-path mode: open the archive (or the ROM backend) before the boot.

## Round 11, part z5: demo fidelity, static object events, the font colour (branch names-R11_z5)

Detail: [FIDELITY.md](FIDELITY.md) section 6. Every `src/` change is TARGET_PORT;
`tools/asm_diff.py --base 2df5517d -j 8` 0 on every touched file; no `include/` or archive change.

- **Demo 1 identical to the N64 to its end.** Round 10's "camera 0.18 off at gf 412" started at
  gf 74: `CPlayer__BobViewStride` / `CPlayer__BobView` read the body's yaw through an N64-offset
  view (0 on the host), so the walk's side bob went along the world x axis. With the fix, every
  traced field (player, camera, weapon actor, actors, pooled instances, particles) is
  bit-identical from gf 1 to gf 1,128 (health 12,814 on both, the HUD's 51).
- **Static object events work on the host**: y3's `CScene__AdvanceListedInstances` host body
  again, plus the static objects' region at the host `CGameRegion` stride (0x40, not the N64's
  32: demo 2's crash in `Collision_SweepRegions`). Demo 1's ambient particle now matches.
- **The font colour**: `COnScreen__BeginText`'s combiner mixes PRIM to ENV by TEXEL1 from a
  line-0 tile, which the RDP reads as row 0 (1.0) and Fast3D as an empty texture (0): every text
  was in the dark primitive colour. The host emits the equivalent ENV combiner: the legal text
  peaks at (200, 200, 136), the N64 reference (200, 200, 138), was (80, 64, 40).
- **Trace tools**: a `cam` line in `T2_DET_ACTORS` / `n64trace.py --actors`, compared by
  `detdiff.py` (rebuild both sides of a Mac/Linux comparison).
- **Demos 2 and 3**: every game thread now runs with the N64's flush-to-zero (the R4300 FS bit;
  `t2os.c`): demo 2 left at gf 426 on a denormal velocity. The player's jump had no upward speed
  on the host (the jump scale in the big-endian `CPlayerIntelligence.pad08`, and an N64 view of
  the body in `PlayerJump_Update` / `PlayerDodge_Enter`): demo 3 left at gf 71. **Demo 3 is now
  identical to the N64 to its end; demo 2 to gf 607**, where an enemy's tilt comes from stale
  heap memory read by `CGameObjectInstance__Construct` (an N64 uninitialised read, not matchable).
- Also: placed level actors' region at the host stride (`CLevelActor__Construct`, the same
  32-byte bug as the static objects).
- Regression (Linux headless, exit 0 each): boot + 3 demos 26,000 (demo 1 5,467, demo 2 13,037,
  demo 3 19,922, title 24,751, as before); level 1 playthrough: the hub at 21,600 with 10/8 totem
  kills; level 2 playthrough: both soul gates (21,313 and 42,063), as before; the Mother: death
  27,459 (was 30,641), hub 29,000; the Blind One: death 22,623, hub 23,200. The jump fix changes
  every scripted run that presses R (jumps now leave the ground as on the N64).
- Not run on the Mac: the arm64 flush-to-zero path (FPCR FZ) compiles only there; a Mac == Linux
  check of this branch needs it (Linux had denormals before, the Mac too: both change together).

Files others may touch: func_0024FC08.c / func_0024FE10.c (the view bob), func_002555CC.c /
func_00255670.c / func_00255840.c (jump, dodge), func_002263C0.c, func_00214564.c,
func_002141F0.c, func_002150C0.c (static objects and level actors), func_00277F6C.c (the font),
port/runtime/t2os.c (thread start), port/game/t2det.c + tools/port/n64trace.py +
tools/port/detdiff.py (the trace format: a Mac/Linux comparison needs both sides rebuilt).

## Round 11, part z1: level 2 to the end (branch names-R11_z1)

Branch names-R11_z1 from 2df5517d. Detail: [PLAYTHROUGH.md](PLAYTHROUGH.md) section 6.5.
Commits (`src/`: asm_diff 0 against 2df5517d; no `include/`, archive or gendata change):

- 6ab4362d **`Shadow_DrawBlob`** (func_0021E03C) by member on the host: through the N64 view the
  blob shadow's alpha (0x10E) was written into byte 2 of `m_CurrentAnimation.pmeAnimData`; the
  first-person weapon's blob shadow (`CPlayerPool__ReloadAnimInfo`) turned a released animation
  into 0x630000 and `CAnimationState__FindKeys` crashed (level 135, frame ~50,770 of the run).
  Every actor drawn with a blob shadow had that byte rewritten; its region, ground quaternion and
  collision y offset were also read at N64 offsets.
- 6e772517 **`playthrough.py --level 2` to the end**: the three Sisters of Despair, the City
  Totem (110) and the hub. New debug options: `T2_AIM_ENEMY=F1-F2,R,A` (level actor A first,
  the nearest live actor when A is dead or out of range; `T2_AIM_NODE` accepts `actor:N`), and
  `T2_FACE_PITCH=1` (`T2_FACE_NEAREST` also sets the camera pitch, the portable atan2f).
  The third sister needed the level's own route: the actions 46/47 want a graveyard key each
  (pickup 4025) from the two crypts (zombies 45/44 open the crypt doors 75/76).

Results (Linux, headless, PIE, high memory; every run exits 0):

- **Level 2 complete** (`playthrough.py --level 2`, 64,000 frames): soul gate 1 (21,313), sister
  1 (133, 30,165), sister 2 (134, 37,483), "all soul gates destroyed. mission complete." (42,063),
  the crypt keys (47,281 / 50,121), the actions, door 60 (52,245), **"all sisters of despair
  banished. mission complete."** (52,767), 7200 -> **the City Totem (110) at 55,250**, its defense
  won **8/8 at 62,250**, **the hub (60) at 63,000**.
- **Level 1** (`--fight 10`): unchanged, the totem 8/8 at 20,800, the hub at 21,500.
- No port bug in the sisters' AI: with the aim on her each falls ~400-500 frames after waking
  (133: door 28 at 29,801, banished 30,165).

**Merge notes**: `port/game/t2debug.c` (`T2Port_AimEnemy`, `T2Port_FaceNearest`),
`port/game/t2bosslog.c` (`T2Port_AimNode` now returns int; `T2Port_AimMask` `actor:N`;
`T2Port_Atan2f`), `port/app/game.cpp` (one declaration), `tools/port/playthrough.py` (the level-2
segments 133/134/135/132, the totem fire window and env), `src/us/code/func_0021E03C.c`. z2 (the
bosses) also uses `T2_AIM_NODE` in t2bosslog.c: keep both. z3 (9c791996) adds an early `return` for `SPEC@LEVEL` at the top of
`T2Port_AimNode`: with the int return it must be `return 0;`.

Next: walk level 2's sub-levels instead of hopping; the City Totem's kill pace against the N64
(PLAYTHROUGH.md 6.6). The other files that define a `shadowAlpha` (0x10E) view never touch it.

## Round 11, part z2: the bosses finished, the ending (branch names-R11_z2)

Detail: [GAMEPLAY.md](GAMEPLAY.md) section 12. **All four bosses die and the game returns to the hub;
the Primagen's death plays the ending cinema and the credits, then the hub.** Linux headless, PIE,
high memory; every run exits 0. `src/` changes: 2 files, `tools/asm_diff.py --base 2df5517d -j 8`
reports 0 for both; no header changes (no gendata or archive rerun needed).

| boss | recipe (new game + boss env, `Z@14000..200000`) | frames | result |
|---|---|---|---|
| **the Mantid Queen** | `T2_WARP_TO=7501 T2_BOSS_FRONT=450` | 40,000 | death (mode 16) at 33,507, her cinema, **the hub at 35,000** |
| **the Primagen** | `T2_WARP_TO=7602 T2_BOSS_FRONT=450` | 56,000 | stage 3 at 27,566, **death (mode 29) at 32,828**, the ending cinema, **the credits from 39,000**, **the hub at 53,000** |
| the Mother | unchanged (section 1) | 36,000 | death 30,641, the hub at 32,200 (as round 10) |
| the Blind One | unchanged (section 1) | 25,000 | death 22,683, the hub at 23,500 (as round 10) |

Boot + 3 demos (26,000 frames): `T2_DET_LOG=30` identical, line for line, to the round-10 merged
binary.

Commits:

- 19a1b9be **the ending after the Primagen**: `EngineMode15_Ending_Init` (func_00289A9C) read
  `m_bPrimagenBossComplete` at N64 offset 0x9F0 of CPlayer (other data on the host: the credits
  never ended); `Credits_Update` (func_002862A4) took the engine as `s32` (cut pointer: crash in
  `CEngineApp__StartGameAtWarp` when the credits ended).
- 6f6871cd + ec910bfe **`T2_BOSS_FRONT=D[,EVERY]`** (DEBUG-ONLY, t2bosslog.c, game.cpp): the
  Queen's open spot faces one way and her body's boxes (mask 1, spot 0 has no health) take every
  shot from her side (why step 7 stopped at 3,072 and step 4 took 73,000 frames): the player is put
  D units in front of the open spot every EVERY frames. The Primagen: after EVERY frames of a mode-25
  wait, the player beside the lair (its eye hovers behind the node boxes of `found[0]`, type 0x641:
  no line of sight from the middle of the floor; stage 2 passed only on a sight target kept from the
  flight). `bosshurt:` lines print the boss's `m_VisBits` and mode; boss lines its type, bounds,
  scale.

The Queen's death sprite key (0x81ACAD9C, y2's abort): no abort with `--draw`; S16 covers it.

**Not done:** the Primagen's line of sight against the N64 (the emulator cannot warp to the fight,
and no recorded run reaches it): whether the N64's door boxes block the same lines is open. The
final ending (engine mode 16, `EngineMode16_Ending_Init`, the 100% ending) was not reached.

Merge notes: port/game/t2bosslog.c (`logActor`, `T2Port_LogBossHurt`, the new `T2Port_BossFront`
before `T2Port_AimMask`), port/app/game.cpp (one getenv hook after `T2_AIM_NODE`),
docs/port/GAMEPLAY.md (section 12 appended).

Next: the Primagen stage-3 line of sight against the N64 (a RAM-poke warp in `n64trace.py`, or a
save state at the Lightship); the 100% ending (engine mode 16); boss recipes in a script
(`playthrough.py --boss NAME`).

## Round 11, part z3: levels 3 and 4 (branch names-R11_z3)

Branch names-R11_z3 from 2df5517d. Detail: [PLAYTHROUGH.md](PLAYTHROUGH.md) sections 7 and 8.
Commits (`include/` change: `asm_diff --base 2df5517d -j 8` over the whole tree = 0 of 3,938):

- dfd3216c **`CCameraText.m_nId` is `sptr`** (include/structs/camera.h). `CCameraPool__AddMultiLineText`
  drops a camera's texts with the same id before it adds the new one. On the host the id (the
  owning object's pointer) was cut to s32 and never matched, so a message re-added every frame,
  like "satchel charge required." while the player pushes against the charge point, stacked into
  a full screen of copies. decls.json dropped and gendata rerun (no data uses the struct).
- c2f3976f `T2_EVENT_LOG`: the "objectives A/B" counters now use `Mission_LevelObjectivesDone`'s
  own link-group types (level 3: 6/7, level 4: 8 alone, level 5: 9/10, level 6: 11/12; levels 1-2
  unchanged).
- 9c791996 `T2_AIM_NODE=SPEC@LEVEL`: aim only in that level (the level-4 route ends at the Blind One).
- d60bafb7 **`playthrough.py --level 3` and `--level 4`** (+ `--direct`: no hub, no cinemas, for
  quick runs; `--level 1/2` output is byte-identical to before).

Results (Linux, headless, `--draw`, PIE, exit 0, no crash):

- **Level 3, the Death Marshes, to completion** (36,000): hub portal 3000 -> the intro cinemas
  -> 61 at ~18,200. **5/5 prisoners freed** ("all prisoners free. mission complete." at 24,243)
  and **3/3 ammo storage facilities destroyed** with the 3 satchel charges ("armories destroyed.
  mission complete." at 25,097). Then exit 7300 -> **the Marsh Totem (111) at ~26,500, defence
  won 8/8 at 31,600** (totem untouched), **the hub at ~32,400**.
- **Level 4, the Lair of the Blind, to completion** (42,000): portal 4000 -> 98 at ~16,900. The 3
  vent chambers: key -> switch ("vent chamber located.") -> satchel charge -> "thermal vent
  sealed.", three times: **"blind ones trapped. mission complete."** at 25,199. Then exit 7400 ->
  **the Cave Totem (112), defence won 8/8 at 29,600**, then 7401 -> **the Blind One's arena
  (level 1) at ~30,500**. The rest of round 10 y2's recipe (`T2_AIM_NODE=boss@1`, fire held): the minion phases, **the Blind One's death (mode 9) at 39,693**, the cave, **the hub at ~40,250 with "NEW CHEAT ENABLED"** (at the portal, warp record 4999).
- How the objectives work (found this round): the prisoners, the armories' charge points and the
  vents are type-10 actions. A bump by the player fires their mask-2 records. An action whose
  intelligence names a `m_GoPickupNeeded` item (+0x2A; 4100/4111 satchel charge, 4020 key, 4025
  level 2's graveyard key) uses one or shows its text (`Action_CheckRequiredItem`). The items are
  level actors in the grid sections; their id is the object's type (cart 2, a u16 per object).
- No port crash or data/ABI blocker was hit in levels 3 or 4. All sub-levels on the routes
  (61-71, 98-103, 105-108, 111, 112, 1) load, draw and run their events. 104 was not entered.

Merge notes: `port/game/t2bosslog.c` (`T2Port_AimNode`: the early `@LEVEL` return; z1 changes the
function to return int and adds `actor:N`, so the return needs a value there) and
`port/game/t2debug.c` (the `kTypes` table in `T2Port_LogEvent`; z1 touches `AimEnemy`/`FaceNearest`
in the same file). `tools/port/playthrough.py`: z1 and z4 extend it too (z3 adds
`build_hub_level`, `L3_*`, `L4_*`, `--direct`). The counter change shifts the "objectives A/B"
numbers quoted in other parts' docs for levels 3+ (levels 1 and 2 are unchanged).
`include/structs/camera.h` (z5 works on the camera: one line). Not committed:
captures, logs.

Next for levels 3/4: walk the sub-levels instead of hopping (the doors behind the switches of
106-108; 104; the enemies of the routes are mostly not fought). Also: the talisman (74, 75) and
flesh-eater (80, 81) portals and the level keys (4340/4362, 4350); the "level not complete"
screen's mirrored title against the N64 (PLAYTHROUGH.md 7.2); and levels 5 and 6 (z4).

## Round 11, part z4: the round-10 merged tree on macOS, levels 5 and 6 (branch names-R11_z4)

No `src/`/`include/` change. Detail: [MACOS.md](MACOS.md) section 13, [PLAYTHROUGH.md](PLAYTHROUGH.md)
section 9 (section 7 on the branch).

- **The Mac runs the merged tree** (headless, `--offscreen-gl`): boot + 3 demos, level 1 (hub at
  21,500), level 2 (Linux's events), the Mother, the Blind One, save/load, `--audio` (RMS 3,402.2),
  a new-game soak (8 x 36,000, 0 crashes). **Mac = Linux frame for frame** (`T2_DET_LOG`) in the
  boot, level 1, the Mother, the Blind One and the fight; **level 2 diverges at frame 42,124**
  (a `CParticleSystem__Spawn` allocates one effect fewer on the Mac; open, MACOS.md 13.2). The det
  line's `ptcl` hash now also covers the particle pool's lists (port/game/t2det.c).
- **Levels 5 and 6 mapped and entered**: `playthrough.py --level 5 | 6` (tools/port/routes56.py)
  walks into the hub portals 5000 / 6000, plays the intro cinemas and tours the sub-levels by
  their own warps: level 5 all of 124, 84-91, 93-96 (one debug hop past the hive's force field);
  level 6 114-122, then **a crash on both hosts** in `Model_Decompress` after "assembly plant
  destroyed." in 122 (a queued model decompression reads clobbered cache memory while the font is
  loaded synchronously; open). No objective is scored yet (pylons, embryos, the computer,
  couplings: open).
- `T2_EVENT_LOG`: the objective counters of levels 4-6 (types 8, 9/10, 11/12); level 124 counts as
  level 5.
- Merge notes: `tools/port/playthrough.py` (z1/z3 also edit it): only the `--level` choices and a
  4-line branch calling `routes56.build_level56`; `port/game/t2debug.c` (`T2Port_HubLevelOf`,
  `T2Port_LogEvent`'s counters: z3 may touch the level 3/4 mapping); `port/game/t2det.c` (the
  list hash: z5 also works on the det log).

## Round 10: the merged state

Round 10 merged five branches, in this order: y4 (the merged tree on macOS, host determinism,
real time), y5 (stability: S14-S23, no raw `+ 0x140` CAI address on the host, the levels soak,
**a PIE by default on Linux**), y3 (fidelity: the N64's RAM game frame by game frame,
`n64trace.py` / `detdiff.py`), y2 (the other bosses: the Blind One beaten, the Mantid Queen, the
Primagen) and y1 (level 2, the River of Souls). Four fixes came twice (the `gImpactEventHandlers`
0xFFFF read from y4 and y3, the menu text fields from y4 and y5, `BlindOne_Draw` /
`BlindOne_PhaseVulnerableInit` from y2 and y5, the spawn chain's `sptr` from y1 and y5): one
body each (section 3). The merged tree needed two small fixes of its own (a compile-only
module-2 file, `t2audio_test` on the PIE build). This
page says how to build and run the result, what the game does now, and what comes next. The part
sections below keep each branch's own notes (detail pages: [MACOS.md](MACOS.md) section 12 and
[PERF.md](PERF.md) section 5 (y4), [STABILITY.md](STABILITY.md) section 7 and [64BIT.md](64BIT.md)
part 5 (y5), [FIDELITY.md](FIDELITY.md) section 5 (y3), [GAMEPLAY.md](GAMEPLAY.md) section 11 (y2),
[PLAYTHROUGH.md](PLAYTHROUGH.md) section 6 (y1)); round 9 and earlier follow as history.

The matching build is unchanged: `tools/asm_diff.py --base cc0c322b -j 8` reports 0 differences
over all 3,938 files after every merge and every fix, and a clean `make clean && make setup &&
make build && make verify` gives the identical ROM (`build/turok2.us.z64: OK`).

## Round 10: build and run

```sh
# 0. the matching build first: gendata and asm_diff read its ELF; keep a copy as build/ref.elf
make setup && make -j8 build && make verify && cp build/turok2.us.elf build/ref.elf

# 1. the archive, format 2.0. Unchanged this round (rebuilt from the merged tree: the same bytes)
python tools/assets/mkarchive.py baserom.us.z64 -o OUT/turok2.o2r -j 4     # ~12 s, sha1 b6b9d484...
python tools/assets/archive_check.py OUT/turok2.o2r baserom.us.z64          # OK, format 2.0

# 2. configure IN A FRESH BUILD DIR (the Linux default changed to a PIE this round: an old cache
#    keeps T2_LOW_MEMORY=ON); the game library AND the runtime/audio libraries (gendata reads all three)
rm -rf OUT/build
cmake -S port -B OUT/build -G Ninja -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ \
      -DCMAKE_BUILD_TYPE=Debug                      # -DT2_LOW_MEMORY=ON: the old -no-pie build
ninja -C OUT/build -j8 turok2_game t2runtime t2audio

# 3. the game data as C into port/gen/ (gitignored). Drop the cache after ANY header change.
#    RERUN IT this round: y2 changed structs/boss.h, globals/boss_ai.h and gendata's count hints.
rm -f OUT/decls.json
python3 tools/port/gendata.py --build OUT/build --cache OUT/decls.json -j 4      # ~1 min

# 4. re-run cmake so data.cmake sees port/gen/*.c, then everything (turok2, t2audio_test and the
#    compile-only module-2 library, which checks that every C file builds for the host)
cmake OUT/build && ninja -C OUT/build -j8

# 5. run headless (Linux: SDL's offscreen driver, no window). High memory (and a PIE) is the default.
cd OUT && SDL_VIDEODRIVER=offscreen T2_SHOT=shot.ppm T2_SHOT_EVERY=300 \
    build/app/turok2 --archive turok2.o2r --rom ../baserom.us.z64 \
    --game --draw --pak-blank --frames 26000
```

The archive: 26,124 units, sha1 `b6b9d484e6f97718e48ddc6d7f0ce9188dbb11c5` (round 9's bytes;
`archive_check.py`: OK, format 2.0). gendata: 1,534 data symbols (1,529 typed from a C
declaration), 402 pointer targets, 386 zero .bss symbols, 3 typed .bss structs with their N64
tail.

**The scripted runs** (`turok2 --archive turok2.o2r --rom ../baserom.us.z64 --game --pak-blank`
plus, `--draw` for pictures; "new game" is `START@4400 A@4500 A@4700 A@4800`; "boss env" is
`T2_GIVE_WEAPONS=1 T2_WEAPON=10 T2_AIM_NODE=boss T2_CHEATS=1 T2_FILL_AMMO=600 T2_BOSS_LOG=1`):

| what | how | frames |
|---|---|---|
| boot + the 3 attract demos | (none) | 26,000 |
| **level 1, the Port of Adia, start to finish** | `python3 ../tools/port/playthrough.py DIR --fight 10 && . DIR/teleport.env`, then `T2_EVENT_LOG=1 T2_STATE_LOG=100 ... --input @DIR/playthrough.txt` ([PLAYTHROUGH.md](PLAYTHROUGH.md)) | 27,000 |
| **level 2, the River of Souls** (y1) | `python3 ../tools/port/playthrough.py DIR --level 2 && . DIR/teleport.env`, then `T2_EVENT_LOG=1 T2_STATE_LOG=250 ... --input @DIR/playthrough.txt` (PLAYTHROUGH.md section 6) | 49,500 |
| **the Mother defeated** | the input file of the Mother recipe below, `T2_WARP_TO=-4` + boss env, `T2_STATE_LOG=200` | 36,000 |
| **the Blind One defeated** (y2) | new game + `Z@14000..90000`, `T2_WARP_TO=7401 T2_TELEPORT=14000:-400,204.8,0` + boss env | 25,000 |
| the Mantid Queen (y2) | new game + `Z@14000..200000`, `T2_WARP_TO=7501` + boss env | 120,000 |
| the Primagen (y2) | new game + `Z@14000..90000`, `T2_WARP_TO=7602` + boss env (no strafing) | 30,000 |
| the player kills enemies (round 8 w2) | `--input @fight.txt` (new game, `Z@13320..13420` and every 140 frames to 14,680), `T2_WARP_TO=1098 T2_GIVE_WEAPONS=1 T2_WEAPON=4 T2_TELEPORT="13300:-1143.6,0,-2034.7" T2_WAKE_ENEMIES=500 T2_FACE_NEAREST=1500` | 14,800 |
| save | `--pak-dir DIR`, new game + `A@13250 A@13400`, `T2_SAVE_PORTAL=13100` | 14,100 |
| load | `--pak-dir DIR` (the same DIR), `START@4400 A@4500 DD@4700 A@4800 DU@4900 A@5000 A@5200 A@5400` | 6,500 |
| demo 1 against the N64 (y3) | below | 8,300 |

The Mother recipe (one input entry per line, a single line is cut at its buffer):

```sh
cd OUT && (echo "START@4400 A@4500 A@4700 A@4800 Z@14000..90000 CL@19000..90000";
  for s in $(seq 19000 600 89000); do echo "CD@$s..$((s+250))"; done) > mother_end.txt
SDL_VIDEODRIVER=offscreen T2_WARP_TO=-4 T2_GIVE_WEAPONS=1 T2_WEAPON=10 T2_AIM_NODE=boss \
  T2_CHEATS=1 T2_FILL_AMMO=600 T2_BOSS_LOG=1 T2_STATE_LOG=200 \
  build/app/turok2 --archive turok2.o2r --rom ../baserom.us.z64 --game --pak-blank \
  --frames 36000 --input @mother_end.txt
```

Demo 1 against the N64's RAM (y3, [FIDELITY.md](FIDELITY.md) section 5; the emulator side takes
~2 minutes):

```sh
xvfb-run -a -s "-screen 0 1024x768x24" python3 tools/port/n64trace.py --rom baserom.us.z64 \
    --from 1980 --to 3300 --actors > n64.det
cd OUT && SDL_VIDEODRIVER=offscreen T2_DET_LOG=1 T2_DET_GF=1 T2_DET_ACTORS=1 T2_DET_FROM=5400 \
    build/app/turok2 --archive turok2.o2r --rom ../baserom.us.z64 --game --pak-blank \
    --frames 8300 > port.log
python3 ../tools/port/detdiff.py ../n64.det port.log --shift 0 --skip rng,flags   # + ,prt: actors only
```

**The tools** (Linux box):

- `tools/port/playthrough.py OUTDIR [--fight WEAPON] [--level 2]` writes `playthrough.txt` and
  `teleport.env` (level 1 by default; `--level 2`: the River of Souls from the hub);
  `tools/port/approach.py` gives a teleport point facing a warp region or a point.
- `tools/port/soak.py --exe OUT/build/app/turok2 --archive OUT/turok2.o2r --rom baserom.us.z64
  --out DIR --kind attract|newgame|warp|levels --seeds A:B [--frames N] [--both-memory] [--jobs N]
  [--debugger gdb|lldb]`: random but valid input under gdb (lldb on macOS), crashes grouped by
  their top frames, hangs interrupted and classed by the busy thread (`crashes.txt`,
  `summary.json`, a `cmd.sh` per run). `--kind levels` warps through every level id 1-164, four
  per seed ([STABILITY.md](STABILITY.md)). `--both-memory` needs a `-DT2_LOW_MEMORY=ON` build now.
- `tools/port/n64trace.py` + `tools/port/detdiff.py`: the N64's per-game-frame state from RDRAM
  (mupen64plus's debugger, a breakpoint at `CEngineApp__Update`; `--calls`, `--args`) against
  the port's `T2_DET_LOG=1 T2_DET_GF=1 T2_DET_ACTORS=1` trace, aligned by `gFrameCountGameplay`
  (y3). `xvfb-run -a ... tools/port/refshots.py ...`: the N64 reference screenshots and RDRAM
  dumps (round 9 x3). Captured frames, traces and dumps are never committed.

Options (`turok2 --game`):

- `--input`: `BUTTON@F`, `BUTTON@F1..F2` (held), `SX=V@F1..F2` / `SY=V@...` (stick), `@FILE`
  ([INPUT.md](INPUT.md) section 6). The stick turns, C-up/C-down move, C-left/C-right strafe,
  R jumps, Z fires, A/B hold the weapon wheel, L the map, START pauses; the D-pad moves in menus.
- `--pak-blank`, `--pak-dir DIR` (`DIR/turok2_pak<N>.mpk`, N64 byte order; `tools/port/mpk_saves.py
  FILE.mpk` checks the notes), `--no-pak`.
- `--audio` / `--wav out.wav`; **high memory and a PIE are the default** (`-DT2_LOW_MEMORY=ON`
  brings back `-no-pie` and `--low-memory`); `--offscreen-gl` (**macOS**: pixels with no window);
  `--no-window`; `--bench [FROM]`, `--pace` (`--pace --audio`: a simulated 22,050 Hz device counts
  underruns, y4) ([PERF.md](PERF.md)).
- Debug switches (port/game/t2debug.c, t2bosslog.c, t2det.c and game.cpp, **debug only**):
  - state: `T2_STATE_LOG=N`, `T2_ACTOR_LOG=N`, `T2_ACTOR_TYPES=t,...`, `T2_BOSS_LOG=1` (the boss,
    its hits, and a `bosstrace:` line on every boss mode/phase change), `T2_PICKUP_LOG=1`,
    `T2_EVENT_LOG=1` (every level event with the objective counters of the level's own hub; the
    `totem:` state line), `T2_SCENE_LOG`, `T2_MENU_LOG=1`, `T2_PARTICLE_LOG=N`;
  - determinism: `T2_DET_LOG=N` (`T2_DET_FROM`; `T2_DET_ACTORS`: the actors, the weapon actor and
    the live particles; `T2_DET_GF=1`: one line set per game frame; `T2_DET_PARTICLES=1`: the
    particle pool; the line ends with an actor hash and a particle-pool hash), `T2_RNG_TRACE=F1..F2`
    (every RNG draw with its caller);
  - moving and combat: `T2_WARP_TO=<id>[@frame][,...]` (a negative id is `~level`),
    `T2_TELEPORT=F:x,y,z[,rotY][!][;...]` (`!`: also fire the region's enter triggers),
    `T2_WAKE_ENEMIES=R`, `T2_FACE_NEAREST=R[@LEVEL]`, `T2_AIM_NODE=MASK|boss|-1[,R]`, `T2_BOSS_FRONT=D[,EVERY]` (round 11 z2: the player in front of the Queen's open spot / beside the Primagen's lair),
    `T2_AIM_AT=F1-F2:x,y,z[;...]`, `T2_AIM_ENEMY=F1-F2[,R][;...]`, `T2_CHEATS=MASK`
    (1 invincibility), `T2_FILL_AMMO=N`, `T2_GIVE_WEAPONS=1`, `T2_WEAPON=<n>`, `T2_WEAPON_LIST=1`;
  - progression: `T2_SAVE_PORTAL=F`, `T2_DOOR_OPEN=F`;
  - capture: `T2_SHOT=FILE T2_SHOT_EVERY=N T2_SHOT_FROM=F`, `T2_DLDUMP_AT=<frame>`
    (+ `T2_DLDUMP_VTX=1/2/3`), `T2_ANIM_EVENTS=0`, `T2_V2P_TRAP=1`, `--skip-pak-check`,
    `--start-mode N`.

## Round 10: what the game did (Linux, headless, PIE, high memory, final binary)

Every run below exits 0. The frames moved against the branches' own runs: y3's impact-value fix
(a particle hit did the wrong damage with the wrong radius) and `AI_InitHealth` (every enemy had
aggression 0) and y4's N64 hit flags change every fight, so the merged tree's fights end sooner
(or later) than on any one branch.

- **Level 1 from start to finish.** The Port of Adia playthrough (`--fight 10`, 27,000): 51 ->
  52 -> 53 -> 52 -> 55 -> 54 -> 56 -> 57 -> 58 -> 59 -> 109 -> 60, "all distress beacons
  activated. mission complete." (14,751), "the children are safe. mission complete." (17,499), the
  Harbor Totem from 18,800, **the defence won with 8/8 kills by 20,800** (y4's branch 22,700),
  **the hub (60) at 21,500 with "NEW CHEAT ENABLED"** (y4: 23,400).
- **Level 2, the River of Souls** (49,500): the hub portal and the intro cinemas, level 125 at
  18,750, the sub-levels 125 -> 129, **soul gate 1 destroyed** (21,313), the first graveyard (133:
  the zombies die, door 28 opens at 29,801), the second graveyard (134: "graveyard located."
  31,355, **the second Sister of Despair banished** at 37,623), **"all soul gates destroyed.
  mission complete."** (132, 42,063), the big graveyard (135: "graveyard located." 42,950, both
  "graveyard gate open." at 45,107 and 45,909), then the exit 7200 -> level 125 ("level not
  complete"). **The first sister (133) is no longer banished**: she wakes when door 28 opens
  (y1's branch: 29,141, banished at 29,743), drives the player back ~650 units and keeps ~1,100
  away, and the scripted aim (the nearest live actor) stays on the zombies the generators keep
  sending; a longer window and a teleport into her hall did not change that (the merged AI has
  its real aggression now, y3). No crash.
- **The Mother defeated** (36,000): phase 1 at 16,525, phase 2 at 24,386, her death (mode 30) at
  30,641, the defeat cinema (31) at 30,817, **the hub at 32,200** (y4: 32,400).
- **The Blind One defeated** (25,000): the minion phases, the vulnerable phase, **her death
  (mode 9) at 22,683** (y2: 23,403), the cave collapses, **the hub at 23,400 with "NEW CHEAT
  ENABLED"** (y2: ~24,000).
- **The Mantid Queen** (run to 117,000): steps 1-3, step 4 (spot 1: 65,280 -> 0, a hit every few
  thousand frames) to 91,407, steps 5 and 6 (spot 2: 38,400 -> 0 by 94,200), step 7 at 94,107
  (spot 3: 7,680 -> 3,072 by 94,600, mode 15 at 94,413), then **no further hit on spot 3** to the
  end of the run: she does not reach her death mode on the merged tree (y2's branch: step 8 and
  mode 16 at 67,947), so whether y5's S16 fixed the texture-key abort after it is still open
  (S16 is the same key pattern: the draw layer in the top bits). No crash.
- **The Primagen** (70,000): stage 1 (the spawner's waves, the parts), stage 2 at 20,306, **mode
  25 -> 26 at 20,906: the stage-2 wait for the player's line of sight that stopped y2 now passes**
  (attacks, the parts hit again), **stage 3 at 27,566**; then mode 25 again at 27,728 and the same
  wait to the end of the run: stage 3 is where it stops now.
- **The boot loop** (26,000): the pak check (2), legal (298), expansion pak (1,070), Acclaim
  (1,362), the Iguana jungle (1,850), the title (4,212), **demo 1 5,467 -> legal 8,157, demo 2
  13,037, demo 3 19,922, the title again 24,751** (y3's frames).
- **Demo 1 against the N64's RAM** (`n64trace.py` + `detdiff.py`): every traced field (the player,
  the weapon actor, the 24 level actors, the pooled instances, the particles) is **bit-identical
  from game frame 1 to 416**, except y3's known transients (generator 21's mode one frame late at
  gf 236-238; the ambient particle the N64 spawns from gf 202 on, see next steps). At gf 417 the
  first plasma shots start ~0.17 units off in x/z (the camera's view translation, gf 412); the
  first actor field to differ is enemy 7's health at gf 590 (the shot hits enemy 6 first).
- **Save -> title -> load**: SAVE NOTE, "SAVE WAS SUCCESSFUL"; the note (`mpk_saves.py`): NT2E 51
  "TUROK 2" ext A, 90 pages, big-endian, checksum OK, health 100, lives 3, clock 0:02:20, 663
  ISets of level flags; a new boot with that pak loads it: level 51 at 5,500.
- **Audio** (`--audio --wav`, 9,000 frames): 150.17 s, peak 32,768, RMS 3,402.2 (round 9 and
  y4's branch: 3,480.8; the run includes demo 1, whose fight now plays out differently).
- **Soak** (y5's harness, PIE, no `--draw`, `--jobs 2`): attract 100,000 (1 run, 99,960 frames),
  new game seeds 0-39 x 36,000 (40 runs, 1.44 M frames), levels seeds 0-40 x 25,000 (every level id
  1-164 warped into, 41 runs, 1.02 M frames): **0 crashes, 0 hangs**.
- **macOS** (y4's branch, not rerun on this merged tree): Mac == Linux frame for frame
  (`T2_DET_LOG`, particle hash included) in the boot with the 3 demos, the playthrough, the
  Mother, the warp and the fight.

Screenshots (640x480, checked by eye, not committed): the Port of Adia's totem defence (a Raptoid
warping in under plasma fire, 20,401 and 20,701; the totem after the win, 21,001; the hub with
"NEW CHEAT ENABLED", 21,601); the Blind One's closed eye (22,620), the eye open under plasma
fire as she dies (22,680; death at 22,683), the cave (22,710) and its collapse with falling rocks
(22,770), the warp fade with "NEW CHEAT ENABLED" (23,250) and the hub (23,550); level 2's first
soul gate under fire (21,001) and "SOUL GATE DESTROYED." (21,501), 134's graveyard gates (37,501),
132's soul gate (42,001); the Mantid Queen under plasma fire (93,000-110,000); the Primagen's
arena (21,001-29,001); level 51 after the load (5,500).

Port tests: `rnc_test` (14,426 streams, 0 failed), `t2asset_test` (26,124 units, 0 mismatches,
the archive and the ROM backend), `t2asset_typed_test` (1,585 checks, 0 errors), `gbi_test` (0
mismatches), `t2os_test` (trace hash `2b2775877e0d226b` on 3 runs), `t2audio_test --direct 0`
and `--direct 40` (audio_compare: max difference 0; this needed the PIE fix below), `t2audio_test
--adec 0xC29AB4 7464 out.raw` (27,136 samples), the swoosh self test (OK, 0 failed).

ABI (`tools/port/abicheck.py --build OUT/build -j 4`: 3,077 TUs, 11,503 calls, 11,018 checked
against a definition, 181 function-pointer calls): harmful direct sites **0**, indirect 1 (the
known `Destructible_Hurt` store). `ptrdecls.py`: 0 left in `src/` (the strlen returns and
`include/functions.h` as before); `ptrargs.py` on a clean game-library build log: 0.
`f2ucheck.py`: 79 float -> unsigned conversions, none new.

## Round 10: merge resolutions and fixes

| where | branches | resolution |
|---|---|---|
| func_0022D2FC (`CParticleSystem__Spawn`, `gImpactEventHandlers[0xFFFF]`) | y4 627b899b, y3 6716be2e | the same fix (the N64 word 0x00C800FF): y4's body (it also bounds every other type) |
| func_0041AAA0 / func_0041D2C4 (the cheat and name entry fields) | y4 4623ef71, y5 fbaf3697 (S23) | the same fix: y4's body (`T2_MENU_LEFT/RIGHT` through `Menu.controller`, also in func_00453A08); `Menu.cursor` and the view's `state` are both the s16 at 0 |
| func_0040EDB0 (`BlindOne_Draw`) | y5 758a5afc (S19), y2 31106a44 | y5's body (a `CDrawArgs` view under `TARGET_PORT`; y2 had retyped the N64 view itself) |
| func_0040EA24 (`BlindOne_PhaseVulnerableInit`) | y5 cc7a8bc8, y2 1caa520d | **git merged both silently**: y5's host body ended up nested in the `#else` of y2's. One body: y2's (`BlindOneState *`), the `#else` is the N64 original. (`func_00447A24`, the module-2 copy, has y5's.) |
| func_00227C10 / func_00239194 / t2abi.c (the spawn chain's region and intelligence as `sptr`) | y5 89f298d3 (S18), y1 85dd95b3 | the same code; t2abi.c's comment from y5 |
| port/game/t2det.c | y4, y3 | the union: y3's `T2_DET_GF`, `gf`/`ticks` on the header line, the `wpn` and `prt` lines (what `detdiff.py` reads); y4's particle-pool hash at the end of the line, `T2_DET_PARTICLES`, `T2_RNG_TRACE` |
| tools/port/soak.py | y4, y5 | both: y4's lldb path (`--debugger`) and y5's hang classification (`hang_frames`, SIGINT to the game) |
| port/app/game.cpp, tools/port/playthrough.py | y2, y1, y4 | both sets of declarations; the `--fight` ammo note (y4) and the `--level 2` text (y1) |
| docs (STATUS.md) | all | every part section kept, in merge order |
| the archive | none | rebuilt from the merged tree: the same sha1 `b6b9d484...`; gendata rerun (y2's headers) |

A script checks every C file the merges touched for a `#if... TARGET_PORT` inside another
`TARGET_PORT` block (either branch): only func_0040EA24 had one, fixed as above.

Fixes on the merged tree (asm_diff 0):

- 8ef7f691 **func_0043F000** (`Primagen_UpdateMode19_2`, module 2, compile-only on the host): its
  local `extern LevelScriptF604 D_8012F604` clashed with `globals/boss.h`'s `LevelScriptBlock`
  (since 87e899f3): `turok2_game_unlinked` did not compile (the documented `ninja ... turok2`
  never built it). The view is a cast on the host.
- d4f1d5e3 **`t2audio_test` on the PIE build**: `--direct` put its buffers in the command words
  as a plain 32-bit cast (crash in `A_LOADADPCM`): now `osVirtualToPhysical`; `gAudioHeapBase` is
  pointer-wide as `globals/frame.h` declares it. (The game-path mode, `--frames/--sfx/--song`,
  still aborts: it opens no asset backend since 21090d75, `VM window ...: no backend open`.)

## Round 10: next (as written then)

1. **Level 2's Sisters of Despair**: the first (133) again (on the merged tree she keeps away
   from the player and the scripted aim; an aim at her actor, or her flight and pushback against
   the N64 with `n64trace.py --calls`), the third (135: the action objects 46/47, object 156, do
   not fire when walked into and a teleport into her hall draws no frame); then 7200 -> the City
   Totem (110), its defence and the hub.
2. **The Mantid Queen**: step 7 stops at spot 3 = 3,072 in mode 15 (no more hits: where the
   recipe's aim goes in that mode, a `T2_AIM_NODE=boss` entry for her spots); then her death
   (mode 16), the S16 check on the texture key that aborted y2's run, her cinema and the warp
   0x176F.
3. **The Primagen's stage 3**: the same mode-25 wait for the player's line of sight that y2 saw
   in stage 2 (now passed): compare where the N64 player stands, the lair doors and the eye
   height with `n64trace.py`; then the death cinema.
4. **Demo 1 at gf 412**: the camera's view translation is (0.18, 0.03) off in x/z
   (`CCamera__Update`, func_0027D160: the tremor and `CShake__ApplyOffset`); then demos 2 and 3
   against the N64 with `n64trace.py`.
5. **Static objects' events**: `CScene__AdvanceListedInstances` (func_002263C0) reads CScene at
   N64 offsets, so no listed static object fires its events (demo 1's ambient particle); y3's
   host body crashed demo 2 in `Collision_SweepRegions` (a static object's region): fix both.
6. **The legal screen's text colour** (~2x dimmer and more orange than the N64's).
7. **Levels 3-6**: the hub's other portals and keys.
8. The intelligence's speed field (`CPlayerIntelligence` 0x1C is `u8 pad08[]`, left big-endian
   by the archive's swap plan): type it in `swap_spec` and drop the big-endian read.
9. **A windowed run with a real gamepad** (MACOS.md 12.6 documents the mapping; only headless runs
   so far), and this merged tree on the Mac.
10. `t2audio_test`'s game-path mode: open the archive (or the ROM backend) before the boot.

## Round 10, part y4: the merged tree on macOS, host determinism, real time (branch names-R10_y4)

Detail: [MACOS.md](MACOS.md) section 12, [PERF.md](PERF.md) section 5. Commits (asm_diff 0 vs
cc0c322b on every src file touched):

- 627b899b **`CParticleSystem__Spawn` read `gImpactEventHandlers[0xFFFF]`** (m_ImpactEventType[4],
  no 0xFFFF check): 1 MB past the table on the host, so the particle's hit flag (the 1.5x node
  boxes) came from host memory and **the Mother recipe and the totem defence diverged between macOS
  and Linux**. The fix uses the N64's value: RAM 0x8012D07C = 0x00C800FF in every reference dump.
  This changes gameplay on both hosts (N64-faithful hit boxes for those effects).
- 00867058 `T2_AIM_NODE`'s pitch from a portable atan2f: libm's last bit differs between hosts.
- 755b1fc7 `TotemDefense` `stunHelper`: the DamageEvent buffer at its host size (the Mac aborted
  with `__stack_chk_fail` right after the totem was won; Linux overwrote its stack silently).
- 4623ef71 the cheat-entry and name-entry text fields: `Menu.controller`/`player` by member, not an
  s32 at N64 0x20 through an s32 call (13 of 26 Mac soak runs crashed; Linux non-PIE hid it).
- 68a74023 `playthrough.py --fight` adds `T2_FILL_AMMO=600`: with the N64's flags the plasma
  rifle runs dry at 4 of 8 totem kills (round 9's Linux win came from the y4-1 overread).
- 1768ff13 `T2_DET_LOG` ends with a particle-pool hash (`T2_DET_PARTICLES=1`: one line per particle);
  `T2_RNG_TRACE=F1..F2`: every RNG draw with its caller (a `#ifdef TARGET_PORT` hook in Random_Next,
  Random_Float, Random_Blend, RandomInPackedRange).
- 5081df6f `--pace --audio`: a simulated 22,050 Hz device counts audio underruns headless.
- b44c5388 + 5081df6f `soak.py` under lldb on macOS (`--debugger lldb`, the default without gdb),
  `--draw` -> `--offscreen-gl` there.

Results: **Mac = Linux frame for frame** (`T2_DET_LOG=1`, final binaries: boot + 3 demos, the
playthrough, the Mother, the warp, the fight: 0 differing lines). On the Mac: the playthrough to the
hub (totem 8/8 at 22,700, hub 23,400, "NEW CHEAT ENABLED"), the Mother (hub at 32,400), warp, fight,
save -> load, `--audio` (150.17 s, RMS 3,480.8): all exit 0. Mac soak (lldb): 30 runs, 1.06 M frames,
1 crash (the `T2Port_ReadAsset` warp class, fixed by y5's S16). Real time (Debug): the whole
playthrough at 60 VI/s in 450.52 s, 12 late VIs in gameplay (loads), 2.6 ms of work per 16.7 ms VI,
1 audio underrun (a load, none in gameplay). The SDL gamepad path is documented (MACOS.md 12.6:
left stick = turn/look, right stick = C buttons (move), LT = Z (fire), RT = R (jump); a window is
needed).

**Merge notes**: y3 carries the same 0xFFFF fix (its 6716be2e: keep this one); y5 carries the menu
fix for 0041AAA0/0041D2C4 only (keep this one, it also covers func_00453A08). The particle fix and
the stunHelper fix change the Linux playthrough's totem timing (8/8 at 22,700, hub at 23,400
instead of 24,200) and the Mother's hub frame (32,400). Files: src func_0022D2FC, func_004085A0,
func_0041AAA0, func_0041D2C4, func_00453A08, func_002113A4, func_0021149C, func_002119FC,
func_0026D128; port/app/game.cpp (the --pace audio sink), port/game/t2det.c, port/game/t2bosslog.c;
tools/port/soak.py, tools/port/playthrough.py (y1 also extends it).

## Round 10, part y5: stability and 64-bit debt (branch names-R10_y5)

Branch names-R10_y5 from cc0c322b. Detail: [STABILITY.md](STABILITY.md) section 7,
[64BIT.md](64BIT.md) part 5. `src/` changes are `TARGET_PORT` or `s32` -> `sptr`
(`asm_diff --base cc0c322b -j 8` = 0 on every touched file); no `include/`, archive or gendata
change.

- **The 9 open soak classes of round 9 are closed**, plus 3 new ones the level-warp soak found
  (S14-S21): the throwables (`func_00264994 + 0xC`, a splat mis-cut, stored as a callback; their
  owner pointer truncated), the save portal menu hang, the weapon wheel's slot layout writing
  over the HUD overlays (the death-cinema overlay crash), the particle draw layer in a texture
  key (the PI ignores the top bits; the loader aborted), the ending without a body
  intelligence, the spawn's region/intelligence as `s32`, the Blind One's draw view.
- **`CPlayer__UpdateSpeedScale` was all N64 offsets and ran every frame**: it wrote the speed
  scale into the player's save data (`m_Data.m_AdonAmmoFlagHelped[0]` = 1.0f) and read the mode
  from it; `m_CameraMovementScaler` (the head bob phase) was never set. Host body now. Its speed
  byte field is unswapped in the archive (read big-endian). The attract demos' `T2_DET_LOG` is
  identical to round 9.
- **No raw `+ 0x140` CAI address is compiled on the host any more** (BlindOne_PhaseVulnerableInit,
  the throw pickup, CPlayer__Dismount, GameObject_GetFloorKind, the riding view,
  ImpactEvent_Rumble).
- **soak.py**: `--kind levels` (every level id 1-164 through `T2_WARP_TO=~level`), hangs
  interrupted with SIGINT and classed by the busy thread, timeout 600 s.
- **PIE**: the Linux build is now a PIE by default (`T2_LOW_MEMORY` OFF; `-DT2_LOW_MEMORY=ON` for the old `-no-pie` + `--low-memory`). One PIE-only class (S23) fixed; S22 (`CPlayerPool__GetIndex` with the N64 CPlayer size, cinema events as `s32`) found by the levels soak.
- The 42 gendata aliases: classified, none needs a change (STABILITY.md 7.4).

Soak (final binaries): non-PIE 246 runs (attract, newgame, warp, levels; both memories) and final PIE 123 runs: 0 crashes. Round 9 baseline: 21 warp crashes in 6 classes.

**For the merge**: shared files touched that others may also touch: `func_0040EA24.c` /
`func_00447A24.c` (BlindOne_PhaseVulnerableInit) and `func_0040EDB0.c` (BlindOne_Draw) (y2's
Blind One), `func_00254640.c` (CPlayer__UpdateSpeedScale: player movement code, y1/y3),
`func_0025E488.c`, `func_0025EB80.c`, `func_0025F380.c`, `func_0025F3A4.c` (enemy grab/ride
modes), `func_00413230.c` (Menu_MoveCursor), `func_0041AAA0.c`/`func_0041D2C4.c` (S23: keep y4's body, it covers the `_2` copy too), `func_0025340C.c`, `func_0026E348.c`, `func_0026E378.c`, `func_0024BF1C.c` (CBarrel__Fill),
`port/runtime/t2loader.c`, `port/game/t2abi.c`, `port/app/CMakeLists.txt` (T2_LOW_MEMORY default),
`tools/port/soak.py`.

## Round 10, part y3: demo fidelity against the N64's RAM (branch names-R10_y3)

Detail: [FIDELITY.md](FIDELITY.md) section 5. Every `src/` change is TARGET_PORT;
`tools/asm_diff.py --base cc0c322b -j 8` 0 on every touched file; no `include/` or archive change.

- **Tools**: `tools/port/n64trace.py` (the N64's per-game-frame state from RDRAM through the
  mupen64plus debugger: a breakpoint at `CEngineApp__Update`, offsets from `include/` with clang
  for mips, `--calls`/`--args` to log function entries with their arguments),
  `tools/port/detdiff.py` (aligns it with the port's `T2_DET_LOG=1 T2_DET_GF=1 T2_DET_ACTORS=1`
  by gFrameCountGameplay and prints the first differing fields); the port trace gained `gf`,
  `ticks`, the weapon actor and the particles.
- **Fixed** (the first divergent field each time): impact event values read by position
  (`Blast` radius/damage swapped: every particle hit did the wrong damage; also the floor and team
  sound handlers), `AI_InitHealth`'s aggression written at an N64 offset (every enemy had
  aggression 0: attack delay 60 instead of 15), and y4's `gImpactEventHandlers[0xFFFF]` read
  (same fix as y4 627b899b: keep y4's on merge).
- **Result**: demo 1 is bit-identical to the N64 (player, weapon actor, all actors, pooled
  instances, particles) from game frame 1 to 416; it ends at **50 health (N64 51; before 31)**.
  Next divergence: the camera's view translation is 0.18 units off in x/z by gf 412 (tremor or
  shake in `CCamera__Update`, func_0027D160), so the first plasma shots leave from a slightly
  different point and later hit another enemy.
- **Open**: `CScene__AdvanceListedInstances` (func_002263C0) reads CScene at N64 offsets, so no
  static object fires its events on the host (demo 1's ambient particle, gf 202). A host body
  fixed demo 1 there but crashed demo 2 (level 131) in `Collision_SweepRegions` through a static
  object's spawn event; reverted (3c790a0b + b12d96c7). The legal screen's text colour was not
  worked on.
- Regression (Linux headless, 26,000 frames, exit 0): demo 1 5,467 -> legal 8,157, demo 2 13,037,
  demo 3 19,922, as before.

Files others may touch: func_00237AFC.c, func_002374AC.c (blast), func_00239254.c,
func_00238EE4.c, func_002427E0.c (AI health init: y2/y5's AI code), func_0022D2FC.c (y4 too),
port/game/t2det.c (the trace format: Mac/Linux comparisons need both sides rebuilt).

## Round 10, part y2: the other bosses (branch names-R10_y2)

Detail: [GAMEPLAY.md](GAMEPLAY.md) section 11. Every `src/` change is `TARGET_PORT` (host bodies or
views) or a type that compiles the same: `tools/asm_diff.py --base cc0c322b -j 8` reports 0 over all
3,938 files. Two headers changed (`structs/boss.h`, `globals/boss_ai.h`) and gendata changed:
**delete `decls.json` and rerun gendata**; no archive rebuild.

| boss | result (Linux headless, high memory) |
|---|---|
| **the Blind One** (7401) | **beaten**: phases 3-6 (minions, groups 1-3), the vulnerable phase, death (mode 9) at 23,403, the cave collapses, **the hub at 24,000 with "NEW CHEAT ENABLED"** |
| **the Mantid Queen** (7501) | all 8 steps run, the 5 spots go to 0, **her death (mode 16) at 67,947**; then an abort: a particle sprite asks for a texture at a garbage ROM key (`0x81ACAD9C`, `CParticle__DrawSprite` -> `Texture_Request`), with or without `--draw` (STABILITY's "texture key" class): her death cinema and the warp are not seen yet |
| **the Primagen** (7602) | stage 1 complete (waves, parts hit, stage 2 at 24,206); **stuck in stage 2**: in mode 25 it flies to its lair (0, 937, 2929) and waits to see the player; the line of sight is blocked (first by lair door 0xC9 (found[0]), then by the level geometry even with the doors opened). Not solved |

The platform fall is **not** a bug: each fall costs one life and respawns the player on the
platform; x2's recipe held strafe-left, so he walked off three times in a row (GAME OVER).

Recipes (new game, `T2_GIVE_WEAPONS=1 T2_WEAPON=10 T2_AIM_NODE=boss T2_CHEATS=1 T2_FILL_AMMO=600
T2_BOSS_LOG=1`, input `START@4400 A@4500 A@4700 A@4800 Z@14000..90000`):
Blind One `T2_WARP_TO=7401 T2_TELEPORT=14000:-400,204.8,0`, 25,000 frames;
Queen `T2_WARP_TO=7501`, 70,000 frames; Primagen `T2_WARP_TO=7602` (no strafing), 30,000.

Fixes: BlindOne_Draw (the model handle as s32), func_0022594C (scene list at N64 offsets),
**AI_SetTargetOverride** (wrote over every CAI's m_pParent: shared AI code), BlindOne vulnerable and
victory phases; the Queen's Pick tables (incomplete `struct Pick` in boss.h: raw big-endian bytes),
gendata count hints, **her mode table cut at 12 of 19 rows** (overlapping module sections),
modes 13/14 (actor as s32), UpdateTurn/UpdatePush/UpdateDeath views; Primagen_Hurt (DamageEvent at
N64 offsets: no part was ever hit), UpdateMode6 (bossMode by a wrong delta: idled for good),
Effects, UpdateMode0, EnterDeath, the sound pick tables (never emitted: NULL crash).
Debug: `T2_BOSS_LOG` prints a `bosstrace:` line on every boss mode/phase change; `T2_AIM_NODE=boss`
covers the Blind One (per phase) and the Primagen (waves first, then partsDone).

Regression: boot to 26,000 with the same mode switches (demo 1 5,467, legal 8,157, demo 2 13,035,
title 24,749).

**Merge notes.** Shared files: func_002460FC.c (AI_SetTargetOverride, every enemy),
func_0022594C.c, tools/port/gendata.py (pointer-target count hints), include/structs/boss.h,
include/globals/boss_ai.h, port/game/t2bosslog.c, port/app/game.cpp.

Next: the Queen's death texture key (then her cinema and warp 0x176F); the Primagen stage-2 line of
sight (check against the N64: where the player stands, the doors, the eye height); the Primagen
death cinema.

## Round 10, part y1: level 2, the River of Souls (branch names-R10_y1)

Detail: [PLAYTHROUGH.md](PLAYTHROUGH.md) section 6. From the hub through the level-2 portal
(warp 2000) and its intro cinemas into level 125, then every sub-level 125-135 by its own warp
regions; **both soul gates destroyed ("all soul gates destroyed. mission complete.")**, **two of
the three Sisters of Despair banished** (133 at 29,743, 134 at 38,763), the third graveyard (135) up to its two
"graveyard gate open." gates; the exit 7200 then sends the player back to 125 ("level not
complete": one sister left). No crash in any level-2 run (Linux, headless, `--draw`, high memory).

```sh
cd OUT && python3 ../tools/port/playthrough.py . --level 2 && . ./teleport.env
SDL_VIDEODRIVER=offscreen T2_EVENT_LOG=1 T2_STATE_LOG=250 build/app/turok2 --archive turok2.o2r \
    --rom ../baserom.us.z64 --game --draw --pak-blank --frames 49500 --input @playthrough.txt
```

Commits (asm_diff 0 against cc0c322b for the `src/` one):

| commit | what |
|---|---|
| 85dd95b3 | **Y1-1** `CInstancePool__SpawnAnimInstance` (func_00227C10) / `ImpactEvent_SpawnAnimInstance` (func_00239194) / `CInstancePool__SpawnAnimInstance_i` (t2abi.c): the owner region and the effect's intelligence as `sptr` (they were `s32`: with high memory the River of Souls intro cinema crashed on its first impact effect in `CGameRegion__GetNormal`) |
| b159beea | debug: `T2_AIM_AT=F1-F2:x,y,z`, `T2_AIM_ENEMY=F1-F2[,R]`, `T2_TELEPORT=...!` (fire the region's enter triggers, mask 1 as the player's move does), the event log's objectives per hub (`T2Port_HubLevelOf`) |
| (next) | `tools/port/playthrough.py --level 2`, PLAYTHROUGH.md section 6, this section; the face log's index of pooled instances (-1) |

Findings: the level's event records are keyed by the **owning actor index** (first table: an
actor's death (mask 1), a door opening (0x4000), an action "go" (0x80000)) and by the **region
info index** (second table: the player entering one of its regions, mask 1); the key byte is the
link group a record counts down. Level 2's objectives are link groups 18 (soul gates, type 4) and
16 (sisters, type 5).

**Merge notes**: port/game/t2debug.c and port/app/game.cpp (new switches next to x2's
`T2_AIM_NODE`; the event log line's `objectives` now follows the level's hub: level 1 runs print
the same numbers), tools/port/playthrough.py (`--level 1` output unchanged, checked by diff),
func_00227C10.c / func_00239194.c / t2abi.c (the spawn chain: y5's 64-bit work may touch them).
No header and no archive change; gendata unchanged (decls.json stays valid).

Next: the third sister (135: the action objects 46/47, object 156, do not fire when walked into,
and a teleport into her hall draws no frame), then the exit 7200 to the City Totem (110), its
defense and the hub; levels 3-6 entry (not reached this round).

# Round 9, merged state (history)

Round 9 merged five branches, in this order: x4 (macOS and host determinism: the attract demos'
input, the weapon wheel, Mac crashes on the save and the hit paths), x5 (stability: the soak
harness, 13 crash classes, high memory by default), x3 (fidelity: the N64 reference oracle and
the translucent draw list), x2 (the first boss defeated: the Mother from start to end) and x1
(the first level played through: the Port of Adia from a new game to the hub, with the totem
defence won). Two fixes came twice (`CBarrel__ShouldClose` from x4 and x5,
`CScene__GetRegionIndex` from x1 and x2): one body each (section 3). The merged tree needed no
other fix: every scenario below runs to its end. This page says how to build and run the result,
what the game does now, and what comes next. The part sections below keep each branch's own
notes (detail pages: [PLAYTHROUGH.md](PLAYTHROUGH.md) (x1), [GAMEPLAY.md](GAMEPLAY.md) section 10
(x2, x1), [STABILITY.md](STABILITY.md) (x5), [FIDELITY.md](FIDELITY.md) (x3), [MACOS.md](MACOS.md)
section 11 (x4)); round 8 and earlier follow as history.

The matching build is unchanged: `tools/asm_diff.py --base 5437cfc0 -j 8` reports 0 differences
over all 3,938 files after every merge, and a clean `make clean && make setup && make build &&
make verify` gives the identical ROM (`build/turok2.us.z64: OK`, 123 shared .rodata objects agree).

## Round 9: build and run

```sh
# 0. the matching build first: gendata and asm_diff read its ELF; keep a copy as build/ref.elf
make setup && make -j8 build && make verify && cp build/turok2.us.elf build/ref.elf

# 1. the archive, format 2.0. Unchanged this round (no swapped struct changed): sha1 b6b9d484...
python tools/assets/mkarchive.py baserom.us.z64 -o OUT/turok2.o2r -j 4     # ~12 s
python tools/assets/archive_check.py OUT/turok2.o2r baserom.us.z64          # OK, format 2.0

# 2. configure; the game library AND the runtime/audio libraries (gendata reads all three)
cmake -S port -B OUT/build -G Ninja -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ \
      -DCMAKE_BUILD_TYPE=Debug
ninja -C OUT/build -j8 turok2_game t2runtime t2audio

# 3. the game data as C into port/gen/ (gitignored). Drop the cache after ANY header change.
#    RERUN IT this round: gendata changed (x4's HEADER_POINTEES, x5's grow_bss_tails + audit).
rm -f OUT/decls.json
python3 tools/port/gendata.py --build OUT/build --cache OUT/decls.json -j 4      # ~1 min

# 4. re-run cmake so data.cmake sees port/gen/*.c, then the executable
cmake OUT/build && ninja -C OUT/build -j8 turok2

# 5. run headless (Linux: SDL's offscreen driver, no window). High memory is the default now.
cd OUT && SDL_VIDEODRIVER=offscreen T2_SHOT=shot.ppm T2_SHOT_EVERY=300 \
    build/app/turok2 --archive turok2.o2r --rom ../baserom.us.z64 \
    --game --draw --pak-blank --frames 26000
```

The archive: 26,124 units, sha1 `b6b9d484e6f97718e48ddc6d7f0ce9188dbb11c5` (rebuilt from the
merged tree: the same bytes); `turok2` without `--game` reports fnv1a64 `88179a9b862ded6c` on all 3
backends. gendata: 1,532 data symbols (1,526 typed from a C declaration), 69 pointer targets
typed as tables, **24 pointer targets typed as a header and raw bytes** (`HEADER_POINTEES`: the
attract demos), 20 struct tables extended over splat labels, 387 zero .bss symbols, 3 typed .bss
structs given their N64 tail (`__osTimerCounter`, `gBlindOne` + the cinema player,
`sMdctFft`), 42 typed symbols larger than their N64 extent listed by the audit, 4,195 pointer
slots (3,059 relocated, 1,108 null, 28 unresolved).

**The scripted runs** (`turok2 --archive turok2.o2r --rom ../baserom.us.z64 --game --draw
--pak-blank` plus; "new game" is `START@4400 A@4500 A@4700 A@4800`):

| what | how | frames |
|---|---|---|
| boot + the 3 attract demos | (none) | 26,000 |
| **the Port of Adia, start to finish** (x1) | `python3 ../tools/port/playthrough.py . --fight 10 && . ./teleport.env`, then `T2_EVENT_LOG=1 T2_STATE_LOG=100 ... --input @playthrough.txt` ([PLAYTHROUGH.md](PLAYTHROUGH.md)) | 27,000 |
| **the Mother defeated** (x2) | the input file of the "boss recipe" below, `T2_WARP_TO=-4 T2_GIVE_WEAPONS=1 T2_WEAPON=10 T2_AIM_NODE=boss T2_CHEATS=1 T2_FILL_AMMO=600 T2_BOSS_LOG=1 T2_STATE_LOG=200` | 36,000 |
| the player kills enemies (round 8 w2) | `--input @fight.txt` (new game, `Z@13320..13420` and every 140 frames to 14,680), `T2_WARP_TO=1098 T2_GIVE_WEAPONS=1 T2_WEAPON=4 T2_TELEPORT="13300:-1143.6,0,-2034.7" T2_WAKE_ENEMIES=500 T2_FACE_NEAREST=1500` | 14,800 |
| a real warp to level 52 and back | `T2_WARP_TO=1813 --input @exit52.txt` (round 7) | 16,000 |
| save | `--pak-dir DIR`, new game + `A@13250 A@13400`, `T2_SAVE_PORTAL=13100` | 14,100 |
| load | `--pak-dir DIR` (the same DIR), `START@4400 A@4500 DD@4700 A@4800 DU@4900 A@5000 A@5200 A@5400` | 6,500 |
| enemies hurt the player, a door, the bow journey | round 8's recipes (history below) | |

The boss recipe (x2; one input entry per line, a single line is cut at its buffer):

```sh
cd OUT && (echo "START@4400 A@4500 A@4700 A@4800 Z@14000..90000 CL@19000..90000";
  for s in $(seq 19000 600 89000); do echo "CD@$s..$((s+250))"; done) > mother_end.txt
SDL_VIDEODRIVER=offscreen T2_WARP_TO=-4 T2_GIVE_WEAPONS=1 T2_WEAPON=10 T2_AIM_NODE=boss \
  T2_CHEATS=1 T2_FILL_AMMO=600 T2_BOSS_LOG=1 T2_STATE_LOG=200 \
  build/app/turok2 --archive turok2.o2r --rom ../baserom.us.z64 --game --pak-blank \
  --frames 36000 --input @mother_end.txt
```

**The tools** (Linux box):

- `tools/port/playthrough.py OUTDIR [--fight WEAPON]` writes `playthrough.txt` and
  `teleport.env` for the level-1 run; `tools/port/approach.py` gives a teleport point facing a
  warp region or a point (x1).
- `tools/port/soak.py --exe OUT/build/app/turok2 --archive OUT/turok2.o2r --rom baserom.us.z64
  --out DIR --kind attract|newgame|warp --seeds A:B [--frames N] [--both-memory] [--jobs 4]`:
  random but valid input under gdb, crashes grouped by their top frames (`crashes.txt`,
  `summary.json`, a `cmd.sh` per run) ([STABILITY.md](STABILITY.md), x5).
- `xvfb-run -a -s "-screen 0 1024x768x24" python3 tools/port/refshots.py ...`: the N64
  reference (mupen64plus + Rice, headless): screenshots and RDRAM dumps at chosen emulator frames,
  with input ([FIDELITY.md](FIDELITY.md), x3). Captured frames and dumps are never committed.

Options (`turok2 --game`):

- `--input`: `BUTTON@F`, `BUTTON@F1..F2` (held), `SX=V@F1..F2` / `SY=V@...` (stick), `@FILE`
  ([INPUT.md](INPUT.md) section 6). The stick turns, C-up/C-down move, C-left/C-right strafe,
  R jumps, Z fires, A/B hold the weapon wheel, L the map, START pauses; the D-pad moves in menus.
- `--pak-blank`, `--pak-dir DIR` (`DIR/turok2_pak<N>.mpk`, N64 byte order; `tools/port/mpk_saves.py
  FILE.mpk` checks the notes), `--no-pak`.
- `--audio` / `--wav out.wav`; **high memory is the default** (Linux: host pointers above 4 GB;
  `--low-memory` is the Linux debug flag; the executable is still non-PIE); `--offscreen-gl`
  (**macOS**: pixels with no window); `--no-window`; `--bench [FROM]`, `--pace` ([PERF.md](PERF.md)).
- Debug switches (port/game/t2debug.c, t2bosslog.c, t2det.c and game.cpp, **debug only**):
  - state: `T2_STATE_LOG=N`, `T2_ACTOR_LOG=N`, `T2_ACTOR_TYPES=t,...`, `T2_BOSS_LOG=1` (the boss,
    the hits its hurt callback gets, the Queen's members and the instance pool), `T2_PICKUP_LOG=1`,
    `T2_EVENT_LOG=1` (every level event with the objective counters; the `totem:` state line),
    `T2_SCENE_LOG`, `T2_MENU_LOG=1`, `T2_PARTICLE_LOG=N`; `T2_DET_LOG=N` (`T2_DET_FROM`,
    `T2_DET_ACTORS`: a per-frame trace with exact float bits, the RNG seeds and an actor hash, to
    diff two hosts: x4);
  - moving and combat: `T2_WARP_TO=<id>[@frame][,...]` (a negative id is `~level`),
    `T2_TELEPORT=F:x,y,z[,rotY][;...]`, `T2_WAKE_ENEMIES=R`, `T2_FACE_NEAREST=R[@LEVEL]` (also
    pooled instances), `T2_AIM_NODE=MASK|boss|-1[,R]` (aim at a hit box with those node bits),
    `T2_CHEATS=MASK` (1 invincibility), `T2_FILL_AMMO=N`, `T2_GIVE_WEAPONS=1`, `T2_WEAPON=<n>`,
    `T2_WEAPON_LIST=1`;
  - progression: `T2_SAVE_PORTAL=F`, `T2_DOOR_OPEN=F`;
  - capture: `T2_SHOT=FILE T2_SHOT_EVERY=N T2_SHOT_FROM=F`, `T2_DLDUMP_AT=<frame>`
    (+ `T2_DLDUMP_VTX=1/2/3`), `T2_ANIM_EVENTS=0`, `T2_V2P_TRAP=1`, `--skip-pak-check`,
    `--start-mode N`.

## Round 9: what the game did (Linux, headless, final binary)

Every run below exits 0, with the default high memory unless it says `--low-memory`.

- **Level 1 is playable from start to finish.** The Port of Adia playthrough (x1's script with
  `--fight 10`, 27,000 frames): 51 -> 52 -> 53 -> 52 -> 55 -> 54 -> 56 -> 57 -> 58 -> 59 -> 109 ->
  60, every level event at x1's frames ("cage open." 13,715, "child rescued." 13,857, "all
  distress beacons activated. mission complete." 14,751, "portal activated." 15,951, "the
  children are safe. mission complete." 17,499, the last portal switch 18,601), the Harbor Totem
  at 18,800 ("DEFEND THE TOTEM"), the generator Raptoids come over the walls and die under the
  plasma rifle (kills 1/8 at 19,500 ... 8/8 at 23,500), **`TotemDefense_Complete` with 3 s left,
  "MISSION COMPLETE"**, then the hub (level 60) at 24,200 with "NEW CHEAT ENABLED", Turok at the
  1999 spawn (-101.9, -255.7, -2138.0), health 70. (The merged tree wins the defence ~2,300
  frames sooner than x1's branch run: same kills, other spawn timing.)
- **The first boss is defeated.** The Mother (x2's recipe, 36,000): phase 0 (the three arms,
  then the three heads, parts 6/6 -> 0/6), phase 1 at 16,600, phase 2 (the chest, 128,000 to 0)
  at 25,800, her death (mode 30) at 31,200, the defeat cinema (mode 31) at 31,400, then **the hub
  at 32,600 with "NEW CHEAT ENABLED"** (x2's branch: 35,000).
- **The boot loop** (26,000): the pak check (2), legal (298), expansion pak (1,070), Acclaim
  (1,362), the Iguana jungle (1,850; **its three iguana shots now draw**: x3), the title (4,212),
  **demo 1 5,467 -> legal 8,157, demo 2 13,035, demo 3 19,920, the title again 24,749** (the new
  reference: the demos now play their real input, x4). **Demo 1 follows the N64** (checked
  against x3's mupen64plus reference): the plasma rifle from the wheel, through the door, the
  first enemy in the same view; after that the enemies attack sooner than on the N64 (the port
  ends the demo at 31 health, the N64 at 51). `--low-memory`: the same mode switches at the same
  frames.
- **The player kills enemies** (level 52, 14,800): enemies 11 and 12 20 -> 0, the death
  animations (mode 52), the corpses melt (mode 60).
- **The warp 51 -> 52 -> 51** (16,000): level 52 at 13,400, level 51 again at 15,200.
- **Save -> title -> load**: SAVE NOTE (free 123, needed 90), "SAVE WAS SUCCESSFUL"; the note
  (`mpk_saves.py`): NT2E 51 "TUROK 2" ext A, 90 pages, big-endian, checksum OK, health 100, lives
  3, clock 0:02:20, 663 ISets of level flags. A new boot with that pak: LOAD NOTE "1. TUROK 2 A
  90", NOTE INFO (Port of Adia, 0:02:20, normal), level 51 at 5,500.
- **Audio** (`--audio --wav`, 9,000 frames): 150.2 s, peak 32,768, RMS 3,480.8 (sound from 22 s,
  the Iguana cinema, on).
- **Soak** (x5's harness on the merged binary, no `--draw`): attract 100,000 frames x 2 (high and
  low memory): 0 crashes; new game seeds 0-19 x 36,000 frames, both memories (40 runs, 1.44 M
  frames): **0 crashes**. (x5's branch totals: 400 k attract and 2.88 M new-game frames clean, 2
  open new-game classes and 7 in the debug-warp runs: STABILITY.md section 4.)
- **macOS** (x4's branch, not rerun on this merged tree): Mac == Linux frame for frame
  (`T2_DET_LOG`) in the boot with the 3 demos, the journey, the warp, the fight, the Mother, the
  door and demo 1.

Screenshots (640x480, checked by eye, not committed): the Iguana cinema with the iguana on the logo
and the iguana with the gun (2,251, 3,251); demo 1 with the plasma rifle at the door, the first
enemy and its plasma hit (6,000-7,500), next to the N64's frames of the same scenes; the Port of
Adia's "DISTRESS BEACON ACTIVATED.", "CAGE OPEN." with the child behind the bars, "ALL DISTRESS
BEACONS ACTIVATED. MISSION COMPLETE.", the rescued child at the ladder, "DEFEND THE TOTEM" with a
Raptoid warping in (18,901); a Raptoid in the plasma rifle's sights (21,001), "MISSION
COMPLETE." (23,501), a Raptoid's corpse at the totem (24,001), the hub with "NEW CHEAT ENABLED"
(24,201); the Mother under plasma fire (15,000, 30,000), her defeat cinema (32,000), the hub with
"NEW CHEAT ENABLED" (32,601); SAVE NOTE, SAVE WAS SUCCESSFUL, LOAD NOTE, NOTE INFO.

Port tests: `rnc_test` (14,426 streams, 0 failed), `t2asset_test` (26,124 units, 0 mismatches),
`t2asset_typed_test` (1,585 checks, 0 errors), `gbi_test` (0 mismatches), `t2os_test` (trace hash
`2b2775877e0d226b` on 3 runs), `t2audio_test --direct 0` (19 frames, peak 32,433; audio_compare:
max difference 0), `t2audio_test --adec 0xC29AB4 7464` (27,136 samples), the self test with the
swoosh system (OK, 0 failed).

ABI (`tools/port/abicheck.py --build OUT/build -j 4`: 3,077 TUs, 11,434 calls, 10,970 checked
against a definition, 185 function-pointer calls): harmful direct sites **0**, indirect 1 (the
known `Destructible_Hurt` store). `ptrdecls.py`: 0 left in `src/` (the strlen returns and
`include/functions.h` as before); `ptrargs.py` on a clean game-library build log: 0.
`f2ucheck.py`: 79 float -> unsigned conversions, none new.

## Round 9: merge resolutions

| where | branches | resolution |
|---|---|---|
| func_00254F24 (`CBarrel__ShouldClose`) | x4, x5 | both rewrote it by member (x4: the wheel closed on its first frame, demo 1's stick turned the player; x5 S6: the stick wheel crashed on a garbage controller). One body: x4's, typed `(CBarrel *, CPlayer *)` as its caller (func_0024BD18) declares it; both reasons in its comment |
| func_002242B4 (`CScene__GetRegionIndex`) | x1, x2 | both divided by the host `CGameRegion` stride (x1: map bits written into the next level's block, the warp into 54 crashed; x2: `Cinema_Start` restored the player into another region, the camera crashed after the Mother's phase-2 cinema). One body: `(uptr) result / sizeof(CGameRegion)` (unsigned, like the N64 shift), `structs/region.h` included once |
| tools/port/gendata.py | x4, x5 | the union: x4's `HEADER_POINTEES` (`header_only` targets) and x5's `ptr_offs` / `grow_bss_tails` / the size audit, in the same pointer-target branch |
| docs (STATUS.md, GAMEPLAY.md) | all | every part section kept, in merge order; GAMEPLAY.md section 10 has x2 then x1 |
| the archive | none | no header in `include/` changed: rebuilt from the merged tree, the same sha1 `b6b9d484...` |

No nested `#ifdef TARGET_PORT` blocks and no duplicate host bodies after any merge (checked over
every file the merges touched). No fix was needed on the merged tree: every scenario of section
2 ran to its end on the first build.

## Round 9: next (as written then)

1. **Level 2 from the hub**: the hub's portals and level keys (the keys lying in 52/55/58/59),
   level 1's optional areas (72, the talisman; 78, the flesh-eater portal), walking level 1
   with its enemies awake instead of teleporting.
2. **The other bosses**: the Mantid Queen's minions (spawned on the walls with `anim -1`, they pile
   up to 32 and her spots stay shut), the Blind One's minions, the Primagen (strafing off its
   platform is an instant GAME OVER; its fall); then their death cinemas.
3. **Demo 1 after the first enemy**: the port's enemies attack sooner than the N64's (31 vs 51
   health at the end). Compare the RNG seeds and the AI's hearing/sight against an RDRAM dump
   (`refshots.py`, `T2_DET_LOG`).
4. **The legal screen's text colour**: ~2x dimmer and more orange than the N64's (x3).
5. **The 9 open soak classes** (STABILITY.md section 4): `SetupAnimation`'s CAI callback after a
   warp, a texture key that is a host address (level 46), the death-cinema HUD overlay script and
   the debug-warp classes; `--draw` soak runs (the GL path).
6. **A PIE build** on Linux (the statics still below 2 GB) and the foreign-alias list of the
   gendata audit (42).
7. Raw `+ 0x140` CAI addresses left in non-boss files (func_00254640, 002553BC, 0025E488,
   0025F380, 0025F3A4, 00264CEC, 0026508C, 00268E60, 004082A0, 0040EA24): each a crash or a
   wrong mode on the host when reached; the totem's other N64 views (`TotemDefense_KillsLeft`,
   `TotemDefense_TryHitEnemyAfterDelay`).
8. **macOS**: run this merged tree on the Mac (the playthrough and the Mother defeat have only run
   on Linux).

## Round 9, part x4: macOS and host determinism (branch names-R9_x4)

Detail: [MACOS.md](MACOS.md) section 11. Commits (asm_diff 0 vs 5437cfc0): 8b8697a6 (`SaveGame_Build`:
the save block pointer; the Mac crashed on the first save), 3f6328bc (`CPlayer__OnParticleHit(Callback)`
pointer-wide; `CCameraPool__ReleaseAllTexts` pointer-wide links: Mac crashes when an enemy shot hits the
player and on the death cinema), 792b59ec (`T2_DET_LOG=N`, `T2_DET_FROM`, `T2_DET_ACTORS`: per-frame
trace with float bits, RNG and an actor hash), e96538c2 (**gendata `HEADER_POINTEES`**: the attract
demos were typed as tables of `DemoHeader` and their input streams swapped as s16 pairs: every demo
played noise), 11492a55 (**`CBarrel__ShouldClose` host body**: the stick wheel closed on its first frame
and the stick turned the player). **Mac and Linux give the same per-frame trace** in every run (the
boot with the 3 demos, journey, warp, fight, Mother, door, teleported demo 1): no float divergence.
The round-8 "demo 1 kills on the Mac" was the Mac crashing on an enemy's hit (x4-2), not a divergence.
**Demo 1 now follows the N64** (x3's reference): plasma rifle, straight through the door, the first
enemy in the same view; then the enemies attack sooner than on the N64 and the player dies (7,920).
**Merge notes**: `tools/port/gendata.py` (drop decls.json, rerun; report line "pointer targets typed
as a header and raw bytes: 24"); the journey reference changes to (352.5, -780.7, 879.4) rotY -1.55;
the boot's mode switches change (5,467 demo 1 -> 8,157; 13,035 demo 2; 19,920 demo 3; title 24,749).
Files: src func_0041D890, func_0025689C, func_00253770, func_002819E0, func_00254F24; port/app/game.cpp
(one hook), port/game/t2det.c (new).

## Round 9, part x5: stability: soak runs, 13 crash classes, high memory by default (branch names-R9_x5)

Detail: [STABILITY.md](STABILITY.md). Every src change is `TARGET_PORT` or `s32` -> `sptr`;
`tools/asm_diff.py --base 5437cfc0 -j 8` 0 on every touched file; no `include/` or archive change.
**Merge needs: gendata changed (`grow_bss_tails`, two audit lists in data_report.txt): drop
decls.json and rerun gendata.**

- `tools/port/soak.py`: attract / newgame / warp runs with random but valid input, under gdb,
  crashes deduplicated by their top frames (`crashes.txt`, `summary.json`, a `cmd.sh` per run).
- Fixed classes S1-S13 (STABILITY.md section 3): the audio-off music stop (quit to title), the
  second camera (`NEXT_CAMERA`), **the cinema player laid over the menu globals** (gendata: the
  0x100 bytes after `gBlindOne`: every cinema end could call garbage), `Menu::controller` written
  over `Menu::desc`, `CBarrel__ShouldClose`, `CAITargets` locals smaller than the host struct
  (stack smash in the enemy idle modes and the boss modes), the strip wheel past 36 slots, the
  ladder jump with no intelligence, and the `--high-memory` truncations of the menu callbacks
  (31 files), `Turret_Hurt`, the delayed level events and `Player_DrawNodeCallback`.
- Totals (final binary): attract 100,000 frames x 4 runs (high and low memory): 0 crashes;
  160 new games of 36,000 frames (seeds 0-79, both memories): 156 clean, 2 open classes (a texture
  key that is a host address in level 46, the death cinema HUD overlay); 80 debug-warp runs: 7 open
  classes (STABILITY.md section 4). At the start: 6 of 8 new games and both attract loops crashed.
- **`--high-memory` is the default** (port/app/main.cpp); `--low-memory` is the Linux debug flag
  (the executable is still non-PIE: the statics stay below 2 GB). Checked with it: the boot loop
  with `--draw` (the same mode switches, the title again at 25,778), w2's level-52 kills.
- gendata audit (w4's): the list of typed symbols larger than their N64 extent with foreign aliases
  (42; `__osThreadTail`, `gCrashFont`, `gThreadStackMain`... none used by the port's paths) is
  printed, not changed.

Files others may touch (merge): tools/port/gendata.py (local: `grow_bss_tails`, the report),
port/app/main.cpp (the memory flags), func_00413230.c (`MenuItem_SelectSubmenu`'s signature only),
func_0025F068.c / func_00403144.c / func_004032E4.c / func_0040DC38.c / func_00446C38.c /
func_00442A64.c (boss/enemy AI: x2), func_00250EC0.c (player climb), func_0024BF1C.c (the wheel).
Scripts that pass `--high-memory` still work; the docs' "low memory" runs are now `--low-memory`.

Next: the open classes (SetupAnimation's CAI callback after a warp, the texture key, the overlay
script), a PIE build on Linux, `--draw` soak runs (the GL path), the foreign-alias list.

## Round 9, part x3: fidelity oracle and the actors' lighting (branch names-R9_x3)

Detail: [FIDELITY.md](FIDELITY.md). An N64 reference now runs headless on the box:
mupen64plus 2.6 with the Rice video plugin under Xvfb, driven by **`tools/port/refshots.py`**
(screenshots and 8 MB RDRAM dumps at chosen emulator frames, and button presses). Captured images
and dumps are never committed.

Commits (asm_diff 0 against 5437cfc0 for each `src/` one):

| commit | what |
|---|---|
| 714792dd | `CScene__DrawSortedList` (func_0022077C) read the deferred list at N64 offsets 0x1064/0x1068 of `CScene`, so **no translucent (flag 4) instance or deferred static object was ever drawn**. The Iguana cinema had no iguanas; now its three iguana shots match the N64 |
| 4b0d13a6 | `Model_AdvanceMorph` (func_0020B4F0) read the model handle through `s32 *`. The morphing level actors (type 12), now reached through that list, crashed with `--high-memory` |
| (this) | `tools/port/refshots.py`, FIDELITY.md |

Findings:

- **The black silhouettes are not a port bug.** The light volumes in RDRAM equal the port's.
  At the same point, `CScene__ComputeLights` gives the N64 instance's lights (level 81, demo 1:
  ambient 2d1a05, colour fff80b on both). The dark enemies stand where the volumes give almost
  nothing, and the N64 draws its own dark-corridor enemies the same way (red eyes only).
- **Demo 1 diverges from its first frame on both hosts**: on the N64 the player faces a door,
  takes the plasma rifle and is hurt down to 51 health. On the port he spins with the wheel open
  and keeps 100. Reported to the coordinator for x4.
- **The legal screen's text is ~2x dimmer and more orange** than the N64's (open).

**Merge notes**: func_0022077C.c, func_0020B4F0.c and func_002152AC.c are only touched here. The
deferred list now draws objects that never drew before (translucent level actors, morphers and
static objects), so rerun every scenario: x1 (Port of Adia) and x5 (soak) may meet new paths in
it. Smoke runs after both fixes, all exit 0: the boot and 3 demos (26,000 frames; the mode
switches are those of the coordinator's 5437cfc0 binary. Note that demo 3 ends at 22,148 there
too, not near 25,000 as section 2 says), the same with `--high-memory`, and w2's fight with
`--high-memory` (14,800: enemies 11 and 12 at 0/20, melting, mode 60).

## Round 9, part x2: finishing a boss (branch names-R9_x2)

Detail: [GAMEPLAY.md](GAMEPLAY.md) section 10. Every `src/` change is `TARGET_PORT` (host bodies
or host views); `tools/asm_diff.py --base 5437cfc0 -j 8` 0 for every touched file. No header in
`include/` changed: no archive rebuild, no gendata rerun (`decls.json` stays valid).

**The Mother can be beaten on the host.** The damage did not stop after one part for a bug: the
scripted fire hit her body (node bits 0x201), and only the arms (0xC00), the heads (0x5000) and in
phase 2 the chest (0x2000) count. Aimed at them (`T2_AIM_NODE=boss`, new), every part dies in
turn; then four host bugs stood between her phases and her death (fixed). The run, Linux headless
(new game; one input entry per line, a single line is cut at its buffer):

```sh
cd OUT && (echo "START@4400 A@4500 A@4700 A@4800 Z@14000..90000 CL@19000..90000";
  for s in $(seq 19000 600 89000); do echo "CD@$s..$((s+250))"; done) > mother_end.txt
SDL_VIDEODRIVER=offscreen T2_WARP_TO=-4 T2_GIVE_WEAPONS=1 T2_WEAPON=10 T2_AIM_NODE=boss \
  T2_CHEATS=1 T2_FILL_AMMO=600 T2_BOSS_LOG=1 T2_STATE_LOG=200 \
  build/app/turok2 --archive turok2.o2r --rom ../baserom.us.z64 --game --pak-blank \
  --frames 36000 --input @mother_end.txt
```

| frame | what |
|---|---|
| 14,800-16,700 | phase 0: the three arm parts, then the three heads, each 10,240 to 0 with its stagger; `parts 6/6 -> 0/6` |
| 16,800-26,800 | the phase-1 cinema, phase 1 (arm and head, 38,400 each), her teleport attack moves the player |
| 27,000-33,500 | the phase-2 cinema, phase 2 (the chest, 128,000 to 0) |
| 33,600 | mode 30 (death), 31: the defeat cinema (the Mother with her arm and head stumps) |
| 34,800 | `MotherBoss_OnDefeatCinemaDone`: `m_bMotherBossComplete`, unlocked cheat 4, warp 6999 |
| 35,000 | **level 60, the hub**, "NEW CHEAT ENABLED" |

The same frames with `--draw`; the same fight with `--high-memory` (after x2-7). Screenshots
(640x480, checked by eye, not committed): her chest under plasma fire (33,600), the defeat cinema
(34,200), its fade (34,800), the hub with "NEW CHEAT ENABLED" (35,000).

Fixes (each `asm_diff` 0):

| id | where | bug | fix |
|---|---|---|---|
| x2-1 | `MotherBoss_Mode16_TeleportTarget` (func_0040B488) | raw N64 offsets (`m_AnimRepeatedCount` 0xC7, `gMotherBoss.counter` 0x67C): she stayed in mode 16 for good in phase 1 | host body by member |
| x2-2 | `MotherBoss_TeleportTarget` (func_00409A64) | `obj + 0x140` as the CAI: crashed in `AI_UpdatePathHeading` | `&obj->m_Instance.m_AI` |
| x2-3 | `CScene__GetRegionIndex` (func_002242B4) | `>> 5` (N64 stride) on the host's 0x40-byte regions: twice the index; `Cinema_RestorePlayer` put the player in another region or past the table (crash after the phase-2 cinema); `Automap_RevealRegion` set the wrong level bits | `/ sizeof(CGameRegion)` |
| x2-7 | `CPlayer__SetPosition` (func_00253D68) | `CScene__FindRegionAt` called through an int-returning one-argument cast: the region cut to 32 bits (`--high-memory` crash at the end of the phase-1 cinema) | the real call |
| x2-4 | `MotherBoss_PartEffects` (func_0040A25C) | the boss mode at N64 0x174, the pose context's bone at 0x90 | host views |
| x2-5 | `MantidQueen_Step` (func_00402A10) | the mode update dispatched through an N64 CAI view: **no Mantid Queen mode update ever ran** | host body |
| x2-6 | the rest of the Mantid Queen module (00401FCC, 00402404, 004028F4, 00402ADC, 00402BB0, 00402CE8, 00402D50, 00402E0C, 00402F4C, 004030D4, 0040340C, 00403430, 00403520, 004035B8, 004035CC, 004035F8) | N64 views of the CAI, the instances, `WaveBossState`/`WaveBossStep`, `DamageEvent`; `gMantidQueen_boss` as s32; the members sorted as 4-byte words; `EnterStart` wrote two callbacks at N64 offsets into other members | host views / bodies |

**The Mantid Queen** (7501) now leaves her intro, takes her place and sends her first wave
(`toSpawn 3 -> 0`); the wave never ends: the 0x2C3 minions the members spawn stay at their spawn
points on the walls with no animation (`anim -1`), pile up to 32, and her spots stay shut.
**The Blind One** (7401): phase 3, its 8 minions not killed by the scripted fire. **The Primagen**
(7602): its waves run; strafing off its platform is an instant GAME OVER (lives 3 -> 0).
No crash in any of them (30,000 frames).

Regression (this branch, Linux headless): the boot to 26,000 frames gives round 8's mode switches
(demo 1 5,467, legal 8,800, demo 2 13,680, demo 3 20,946, title 25,778) and the same demo-1 player
states as the round-8 binary. Round 8's runs on this binary, exit 0: the warp 51 -> 52 -> 51 (52 at 13,400, 51 again by
15,200), w2's level-52 kills (the corpses melt, mode 60), w1's deaths, respawns and GAME OVER (the
legal screen at 19,500: the respawn restores the saved region through the fixed index).

Debug (DEBUG-ONLY, port/game/t2bosslog.c, game.cpp): `T2_AIM_NODE=MASK|boss|-1[,R]` (aim the
player, yaw and pitch, at a hit box with those node bits of the nearest live actor or spawned
instance), `T2_CHEATS=MASK` (the game's cheat flags: 1 invincibility), `T2_FILL_AMMO=N`;
`T2_BOSS_LOG` prints the hits the Mother's/Queen's hurt callbacks get, the Queen's members and the
instance pool.

**Merge notes.** Files others may touch: func_002242B4.c (`CScene__GetRegionIndex`: x5 stability /
x1 cinemas and saves: `CSavedPlayers.m_Region` and `Cinema_Start` use it), func_00253D68.c
(`CPlayer__SetPosition`), port/app/game.cpp and port/game/t2bosslog.c (debug switches),
GAMEPLAY.md (new section 10 at the end). Raw `+ 0x140` CAI addresses remain in non-boss files
(func_00254640, 002553BC, 0025E488, 0025F380, 0025F3A4, 00264CEC, 0026508C, 00268E60, 004082A0,
0040EA24): x5 may meet them.

Next: the Queen's minions (their spawn and animation), then her spots; the Blind One's minions and
the Primagen's parts with `T2_AIM_NODE`; the boss death cinemas of the other three.

## Round 9, part x1: the Port of Adia played through (branch names-R9_x1)

Level 1 runs from a new game to its end and back to the hub, the game's own logic doing every
step: details, the level graph and the script in [PLAYTHROUGH.md](PLAYTHROUGH.md).

**The run** (Linux, headless, exit 0; `--high-memory` the same to 22,500):

```sh
cd OUT && python3 ../tools/port/playthrough.py . --fight 10 && . ./teleport.env
SDL_VIDEODRIVER=offscreen T2_EVENT_LOG=1 T2_STATE_LOG=100 build/app/turok2 --archive turok2.o2r \
    --rom ../baserom.us.z64 --game --draw --pak-blank --frames 27000 --input @playthrough.txt
```

51 -> 52 -> 53 -> 52 -> 55 -> 54 -> 56 -> 57 -> 58 -> 59 -> 109 (the Harbor Totem) -> 60 (the
hub): 3 power cells, 3 distress beacons ("all distress beacons activated. mission complete." at
14,751), 4 cage switches and 4 children ("the children are safe. mission complete." at 17,499),
the two portal switches ("portal activated."), the exit warp 7100 (objectives done: the totem,
18,800), the totem defense won 8/8 with 3 s left ("MISSION COMPLETE", 25,800), the warp 1999 to
the hub (26,500). Without `--fight` the defense times out (`TotemDefense_Fail`) and the same
warp takes him to the hub (26,400).

**Fixes** (all `TARGET_PORT`, `asm_diff --base 5437cfc0` 0 for every touched file):

| id | where | bug (what it stopped) |
|---|---|---|
| X1-1 | `PlayerClimb_Enter` | N64 view of the body: cleared the header's `m_pIntelligence`; **every climb crashed** (`PlayerClimb_Update`) |
| X1-2 | `CScene__GetRegionIndex` | `>> 5` (the N64 region stride): map bits written past the level's block of `gGamePersistentData` into the next level's header; **the warp into level 54 crashed** (only with ASLR: gdb hid it). Also every saved region index |
| X1-3, X1-4 | `Action_GoUpdate`, `Action_ChildVanishUpdate` | N64 CAI/instance offsets: switches never back to idle; the rescued child's fade wrote into other fields |
| X1-5, X1-6 | `TotemDefense_Init`, `_UpdateFight`, `_Complete`, `_Fail`, `_StopEnemies` | N64 views of the totem, its CAI, `gTotemDefense`, the player (camera cut to s32, flee time) |
| X1-7 | **`CInstancePool__UpdateVisibility`** | a Rage Wars view of the pool: **generator spawns never ran their AI** (every generator in the game: the totem's waves stood still) and never freed their slots |
| X1-8 | `EnemyAI_DrawFaded` | the N64 byte 0x1B of `CDrawArgs` (crash drawing a teleporting enemy) |
| X1-9 | `EnemyAI_GetBehaviorAt`, `EnemyAnim_PickStart` | `m_Enemy.m_Behavior` at N64 CAI 0x130 |

**Tools and switches (debug only)**: `T2_EVENT_LOG=1` (every level event: message, actor, kind,
handler, the objective counters), the `totem:` state line, `T2_TELEPORT=F:x,y,z,rotY` (a
facing), `T2_FACE_NEAREST=R@LEVEL` (and pooled instances), the actor log's AI animation fields;
`tools/assets/levels.py` exports `warp_id`; `tools/port/approach.py` (a teleport point facing a
warp region or a point), `tools/port/playthrough.py` (the script).

Checked again: the boot loop (26,000, exit 0, the same mode switches; demo 3 is the totem level
and its Raptoids now attack), demo 1 unchanged on Linux (no enemy hurt).

**For the merge**: src files touched: func_00255C5C, func_0026DFB0 (a `T2Port_LogEvent` hook),
func_002242B4, func_00266380, func_002664A0, func_00407F20, func_0040813C, func_004082A0,
func_004088B0, func_00408DCC, func_0022828C, func_0025BA80, func_0025E46C, func_0025E6E0; port:
port/game/t2debug.c (`gT2HostFrame`, the event/totem logs, teleport facing, face-nearest) and
port/app/game.cpp (sets `gT2HostFrame`); no header and no archive change. X1-7 changes every
level with generators (enemies thrown in now move and fight): other parts' scenarios with
generators may now see more enemies.

**Next**: the optional areas (72, the talisman; 78, the flesh-eater portal), the level keys lying
in 52/55/58/59, walking the level with its enemies awake instead of teleporting, the other N64
views of the totem code not on this path (`TotemDefense_KillsLeft`,
`TotemDefense_TryHitEnemyAfterDelay` and the module-2 twins), then level 2 from the hub.

# Round 8, merged state (history)

Round 8 merged five branches, in this order: w1 (enemies hurt the player: death, respawn, game
over), w4 (macOS, and four host bugs both hosts had), w2 (the player kills enemies), w3 (the boss
fights) and w5 (progression: saves, loads, pickups, menus). Then it fixed what only the merged
tree reaches (C23-1..3 below: two `--high-memory` crashes on the kill path, and the doors). This
page says how to build and run the result, how far it gets, and what comes next. The part
sections below keep each branch's own notes (detail pages: [GAMEPLAY.md](GAMEPLAY.md) section 9
(w1, w2, w3, w5), [MACOS.md](MACOS.md) section 10 (w4), [ABI.md](ABI.md) section 7 (w3),
[INPUT.md](INPUT.md) section 7 (w5)); round 7 and earlier follow as history.

The matching build is unchanged: `tools/asm_diff.py --base 5bf03fd0 -j 8` reports 0 differences
over all 3,938 files after every merge and every fix, and a clean `make clean && make setup &&
make build && make verify` gives the identical ROM (`build/turok2.us.z64: OK`, 123 shared
.rodata objects agree).

## Round 8: build and run

```sh
# 0. the matching build first: gendata and asm_diff read its ELF; keep a copy as build/ref.elf
make setup && make -j8 build && make verify && cp build/turok2.us.elf build/ref.elf

# 1. the archive, format 2.0. REBUILD IT this round: include/structs/intelligence.h (w2: the
#    enemy drop masks at 0x0C/0x10 are u32) changes the swap plans.
python tools/assets/mkarchive.py baserom.us.z64 -o OUT/turok2.o2r -j 4     # ~12 s
python tools/assets/archive_check.py OUT/turok2.o2r baserom.us.z64          # OK, format 2.0

# 2. configure; the game library AND the runtime/audio libraries (gendata reads all three)
cmake -S port -B OUT/build -G Ninja -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ \
      -DCMAKE_BUILD_TYPE=Debug
ninja -C OUT/build -j8 turok2_game t2runtime t2audio

# 3. the game data as C into port/gen/ (gitignored). Drop the cache after ANY header change
#    (this round: structs/intelligence.h, structs/region.h, structs/boss.h, globals/boss.h;
#    gendata itself changed too: w3's table extents, w4's display-list words and extents).
rm -f OUT/decls.json
python3 tools/port/gendata.py --build OUT/build --cache OUT/decls.json -j 4      # ~1 min

# 4. re-run cmake so data.cmake sees port/gen/*.c, then the executable
cmake OUT/build && ninja -C OUT/build -j8 turok2

# 5. run headless (Linux: SDL's offscreen driver, no window)
cd OUT && SDL_VIDEODRIVER=offscreen T2_SHOT=shot.ppm T2_SHOT_EVERY=300 \
    build/app/turok2 --archive turok2.o2r --rom ../baserom.us.z64 \
    --game --draw --pak-blank --frames 26000
```

The archive of this tree: 26,124 units, sha1 `b6b9d484e6f97718e48ddc6d7f0ce9188dbb11c5`;
`turok2` without `--game` reports fnv1a64 `88179a9b862ded6c` on all 3 backends. gendata: 1,536
data symbols (1,530 typed from a C declaration), 93 pointer targets typed as tables, 20 struct
tables extended over splat labels, 391 zero .bss symbols, 4,195 pointer slots (3,059 relocated,
1,108 null, 28 unresolved).

**The runs** (`turok2 --archive turok2.o2r --rom ../baserom.us.z64 --game --draw --pak-blank`
plus; "new game" is `START@4400 A@4500 A@4700 A@4800`):

| what | input and switches | frames |
|---|---|---|
| boot + the 3 attract demos | (none) | 26,000 |
| the bow journey | `--input @journey_fire.txt` (round 7 below), `T2_STATE_LOG=60` | 15,000 |
| a real warp to level 52 and back | `T2_WARP_TO=1813 --input @exit52.txt` | 16,000 |
| **enemies hurt the player** (w1) | new game + `SX=80@13310..13358`, `T2_WARP_TO=-82 T2_WAKE_ENEMIES=1500 T2_TELEPORT="13300:2334.7,307.2,-1054.7;15400:...;17500:..."` (the same point 3 times) | 20,000 |
| **the player kills enemies** (w2) | `--input @fight.txt` (new game, `Z@13320..13420` and every 140 frames to 14,680), `T2_WARP_TO=1098 T2_GIVE_WEAPONS=1 T2_WEAPON=4 T2_TELEPORT="13300:-1143.6,0,-2034.7" T2_WAKE_ENEMIES=500 T2_FACE_NEAREST=1500` | 14,800 |
| **a boss fight** (w3) | new game + `Z@14800..15100 Z@15300..15600 ...` (every 500 to 17,600), `T2_WARP_TO=-4` (Mother; `7501` Mantid Queen, `7602` Primagen, `7401` Blind One) `T2_GIVE_WEAPONS=1 T2_WEAPON=10 T2_BOSS_LOG=1 T2_STATE_LOG=60` | 18,000 |
| **save** (w5) | `--pak-dir DIR`, new game + `A@13250 A@13400`, `T2_SAVE_PORTAL=13100` | 14,100 |
| **load** (w5) | `--pak-dir DIR` (the same DIR), `START@4400 A@4500 DD@4700 A@4800 DU@4900 A@5000 A@5200 A@5400` | 6,500 |
| a door (C23-3) | new game + `SX=80@13110..13130 CU@13300..13800`, `T2_TELEPORT="13100:1082.4,-300.0,0.0"` | 14,000 |

Options (`turok2 --game`):

- `--input`: `BUTTON@F`, `BUTTON@F1..F2` (held), `SX=V@F1..F2` / `SY=V@...` (stick), `@FILE`
  ([INPUT.md](INPUT.md) section 6). The stick turns, C-up/C-down move, C-left/C-right strafe,
  R jumps, Z fires, A/B hold the weapon wheel, L the map, START pauses; the D-pad moves in menus.
- `--pak-blank` (a blank formatted Controller Pak in memory), `--pak-dir DIR` (the paks are
  `DIR/turok2_pak<N>.mpk`, written on the first save, N64 byte order: `tools/port/mpk_saves.py
  FILE.mpk` checks the save notes), `--no-pak`.
- `--audio` / `--wav out.wav`; game memory above 4 GB by default since round 9 (x5), `--low-memory` (Linux debug: below 2 GB; `--high-memory` still accepted);
  `--offscreen-gl` (**macOS**: pixels with no window); `--no-window`; `--bench [FROM]`,
  `--pace` ([PERF.md](PERF.md)).
- Debug switches (port/game/t2debug.c, t2bosslog.c and game.cpp, **debug only**):
  - state: `T2_STATE_LOG=N` (every N frames: mode, level, the player, health, lives, the weapon,
    ammo, the wheel), `T2_ACTOR_LOG=N` (with `T2_STATE_LOG`: the N nearest enemies: AI mode,
    animation, health, target, drop masks), `T2_ACTOR_TYPES=t,...` (`T2_ACTOR_LOG` for other
    intelligence types, e.g. 6 = doors), `T2_BOSS_LOG=1` (the boss: phase, health, parts, spots),
    `T2_PICKUP_LOG=1` (every pickup touched), `T2_EVENT_LOG=1` (every level event, the objective counters; round 9 x1), `T2_SCENE_LOG`, `T2_MENU_LOG=1`,
    `T2_PARTICLE_LOG=N`;
  - moving and combat: `T2_WARP_TO=<id>[@frame][,...]` (a negative id is `~level`),
    `T2_TELEPORT=F:x,y,z[;...]`, `T2_WAKE_ENEMIES=R`, `T2_FACE_NEAREST=R` (turn to the nearest
    live enemy every frame: scripted firing), `T2_GIVE_WEAPONS=1`, `T2_WEAPON=<n>`,
    `T2_WEAPON_LIST=1`;
  - progression: `T2_SAVE_PORTAL=F` (the save portal menu at frame F), `T2_DOOR_OPEN=F` (the
    open event to the doors within 800 units);
  - capture: `T2_SHOT=FILE T2_SHOT_EVERY=N T2_SHOT_FROM=F`, `T2_DLDUMP_AT=<frame>`
    (+ `T2_DLDUMP_VTX=1/2/3`), `T2_ANIM_EVENTS=0`, `T2_V2P_TRAP=1`, `--skip-pak-check`,
    `--start-mode N`.

## Round 8: how far it got (Linux, headless)

Every run below exits 0; low memory unless it says `--high-memory`.

- **The boot loop** (26,000): the same mode switches at the same frames as round 7 (2 pak check,
  298 legal, 1070 expansion pak, 1362 Acclaim, 1850 Iguana jungle, 4212 title, 5467 demo 1,
  13680 demo 2, 20946 demo 3, 25778 the title again). The legal screen has all its lines (w4).
  Demo 1 follows its recorded input (the weapon wheel, the bow, the pistol, the shredder, the
  scorpion launcher with its lock-on; screenshots 7900-8500); on Linux no enemy dies in it (the
  six nearest stay at full health, the same as w4's own Linux build: w4's kill at ~8,200 was on
  the Mac). `--audio --wav` (8,000 frames): 133.5 s, peak 32,675, RMS 2,596.9.
- **The bow journey** (15,000): the bow drawn, arrows 20 -> 19 -> 18, down the ramp and the
  swim; it now ends at **(795.2, -781.5, -37.4), rotY -0.69** (the new reference): w2's
  `REGION_WORD` makes the closed water-region edges stop the player (round 7: (315.9, -793.4,
  -1352.3)), and w4's `ControllerProfile` extents change the stick's turn (rotY -0.94 -> -0.69 on
  w4's own branch too).
- **Enemies hurt the player** (w1's script, 20,000): the type-0x190 enemies hear him, run at him,
  melee and shoot; the HUD health falls 100 -> 97 -> ... -> 0 with the red hit flash and pain
  sounds; the death camera, the death cinema, the respawn at the level start with `x2`, again to
  `x1`, the third death: **GAME OVER** (frame ~19,000), then the legal screen (19,389).
- **The player kills enemies** (w2's level-52 fight, 14,800): pistol hits with blood, enemy 12
  20 -> 15 -> 10 -> 0, enemy 11 20 -> 15 -> 10 -> 5 -> 0, the death animations (mode 52) and the
  corpses melt (mode 60). The same with `--high-memory` (after C23-1/2).
- **The Mother** (`T2_WARP_TO=-4`, 18,000): she attacks and **hurts the player** (w4's s16
  picker: health 100 -> 0 between 14,640 and 17,040, the death cinema in the slime pit, the
  respawn with 2 lives), and **the boss loses health for the first time**: 61,440 -> 57,600 ->
  53,760 -> 51,200 at frames 14,900-15,000, one of her 6 parts destroyed (`parts 5/6`, one spot
  10,240 -> 0). No more damage after that with the scripted fire (it keeps hitting the same
  place). Same result with `--high-memory`.
- **Save -> title -> load** (w5): the save portal menu, SAVE GAME, "SAVE WAS SUCCESSFUL"; the .mpk
  note is big-endian (`mpk_saves.py`: NT2E 51 "TUROK 2" ext A, 90 pages, checksum OK, health 100,
  lives 3, clock 0:02:20, 663 ISets of level flags). A new boot with that pak: LOAD NOTE shows
  "1. TUROK 2 A 90", the rumble notice, then level 51 (by frame 5,520).
- **The warp 51 -> 52 -> 51** (16,000): level 52 at 13,680, level 51 again by 15,480.
- **Doors open** (C23-3): level 51's door 27 at (1382.4, -307.2, 0.0) goes to mode 0 (open) when
  the player comes near, and he walks through it into the room behind (x 1082 -> 1650; before,
  he stopped 78 units from it).
- **`--high-memory`**: the boot loop (26,000), the journey (the same end), the level-52 kills,
  the Mother: no crash (before C23-1 the boot crashed in demo 3 at ~21,800 and the fight at its
  first hits; before C23-2 the fight crashed when a corpse melted, ~14,100).

Screenshots (640x480, checked by eye; not committed): the Acclaim logo and the title; demo 1's
weapon wheel and the scorpion launcher's lock-on; the hit enemy's blood spray and its black
silhouette in level 52 (13,500), the muzzle flash on a dying enemy (13,800), the empty corridor
after the corpses melted (14,500, also `--high-memory`); the HUD at 85 after a hit, the death
camera with the enemy over Turok (14,400), the respawn with `x2` (15,320), GAME OVER (19,000);
the Mother hit with plasma fire, the red flash, HUD 70 (14,900), HUD 20 (16,500), Turok's death
cinema in the slime pit (17,100); SAVE NOTE (free 123, needed 90), SAVE WAS SUCCESSFUL; LOAD
NOTE with the note, level 51 after loading; level 52 after the warp; the open door and the room
behind it.

Port tests: `rnc_test` (14,426 streams, 0 failed), `t2asset_test` (26,124 units, 0 mismatches),
`t2asset_typed_test` (1,585 checks, 0 errors), `gbi_test` (0 mismatches), `t2os_test` (trace hash
`2b2775877e0d226b` on 3 runs), `t2audio_test --direct 0` (19 frames, peak 32,433),
`t2audio_test --adec 0xC29AB4 7464` (27,136 samples), the swoosh self test (OK, 0 failed).

ABI (`tools/port/abicheck.py --build OUT/build -j 4`: 3,076 TUs, 11,363 calls, 10,932 checked
against a definition, 186 function-pointer calls): harmful direct sites **0** (the new
`narrow_ret` class included), indirect 1 (the known `Destructible_Hurt` store). `ptrdecls.py`: 0
left in `src/` (the strlen returns and `include/functions.h` as before); `ptrargs.py` on a clean
game-library build log: 0. `f2ucheck.py`: 79 float -> unsigned conversions, none new.

## Round 8: merge resolutions and fixes

| where | branches | resolution |
|---|---|---|
| func_002683CC (`GameObject_GetDeadCollisionHeight`) | w1, w2, w3 | each added a host body; git merged them without a conflict into **nested** `#ifdef TARGET_PORT` blocks (no compile). One body kept: w1's (a1d24455: the definition by member, its N64 byte offsets; the signature stays `u8 *`) |
| func_0025E52C (`AI_HearNoise`) | w1, w2 | w1's 8afda2c4 (w2's identical fix was never committed) |
| tools/port/gendata.py | w3, w4 | merged cleanly: w3's `split_table_extent` and w4's display-list words (6d67f0c8) and `N64_EXTENT` (ff7785b9) all in |
| port/app/game.cpp, port/game/t2debug.c | w2, w5 (w3) | all debug switches kept (`T2_FACE_NEAREST`, `T2_SAVE_PORTAL`, `T2_DOOR_OPEN`, `T2_PICKUP_LOG`, `T2_ACTOR_TYPES`, `T2_BOSS_LOG`) |
| docs (STATUS.md, GAMEPLAY.md) | all | every part section kept; GAMEPLAY.md section 9 has the four parts in merge order |
| the archive | w2 | one rebuild from the merged headers: sha1 `b6b9d484...` (w2's; w3 and w5 changed no swapped struct) |

Fixes on the merged tree (each `asm_diff` 0):

| id | where | bug | fix |
|---|---|---|---|
| C23-1 | `CParticle__HitTarget` (func_0022C488) | its target view read the hit actor's region pointer as `s32 unk10`: a shot that hit an enemy stored a truncated region into the particle; with `--high-memory` `CParticle__IsUnderWater` crashed (boot demo 3 ~21,800, the level-52 fight) | `sptr unk10` |
| C23-2 | `EnemyAI_DrawFadeOut` (func_002605AC), `EnemyAI_DrawNode` (func_0025ECDC), `Action_DrawFade` (func_00266644) | the melting corpse's draw callback took the actor and the draw args as `s32` (`--high-memory` crashed in `CGameObjectInstance__DrawModelImmediate`) and read `CAI::m_ModeTime` at the N64 0x40 (the fade); `EnemyAI_DrawNode` read the args, the draw matrices and the lights at N64 offsets | host bodies by member, `sptr` |
| C23-3 | `Door_OpenUpdate` (func_00265200), `Door_IsOpen` (func_00265808) | the door's CAI read through an N64 `ActionState` view (timer 0x40, duration 0x64, 0xC6/0xC7): on the host the duration test was true on the first update, so a door that opened went straight back to mode 1: **no door ever opened** | host body on `CAI` (`m_ModeTime`, `m_ConditionTimer`, `m_AnimIndex`, `m_AnimRepeatedCount`); `Door_IsOpen(sptr)` |

## Round 8: next (as written then)

1. **The bosses' weak spots**: the Mother lost one part to the scripted fire; aim at the others
   (`HitFrame.unk_6C` spot bits) and follow her phases; the Mantid Queen and the Primagen hits on
   their spots/parts, the Blind One after its minions (now killable) die. Then the boss death
   cinemas.
2. **The enemies' black silhouettes** near the player (level 52; lit only by the muzzle flash):
   compare `CScene__GetLightsAt`/`CScene__ComputeLights` (the level light volumes) and the
   `G_MOVEMEM` lights with an emulator.
3. **Host views with pointers in `s32`**: C23-1 was found by `--high-memory`; the same pattern is
   left in a few views (`T2_HOFF_*` members over `m_p*` typed `s32`: func_0022D2FC's
   `renderModel`/`bigShadow`/`sound`/`unk1CC`, func_00231D98, func_0024C47C's `unk12AC`): check
   whether each one only tests for NULL. Run every scenario with `--high-memory` each round.
4. **Doors and triggers**: switches, levers, keyed doors (level 2 key), `Door_OpenNow`/
   `Door_CloseNow` (a Rage Wars view, position at 4: fine on the host) from the level events.
5. **Demo 1** on the Mac: w4 saw an enemy die at ~8,200 there; Linux does not. Compare the two
   hosts' demo-1 logs frame by frame (input, then the player's path).
6. **macOS**: run this merged tree on the Mac (w4 ran its branch; the four fixes above and w1/w2/
   w3/w5's paths have not run there).
7. gendata: w4's audit of other structs with `TARGET_PORT` members that may be mis-sized
   (aliases inside foreign bases: `__osThreadTail` holding `gAdecBandEnds`, `gCrashFont` holding
   `sCrashCursorX`).
8. Gore deaths, the cerebral bore and the dart reactions; an enemy with a drop; health pickups
   with a damaged player (now possible: w1's damage path).

## Round 8, part w1: enemies hurt the player, death and game over (branch names-R8_w1)

Detail: [GAMEPLAY.md](GAMEPLAY.md) section 9. All src changes are `TARGET_PORT` (host bodies or
macros); `tools/asm_diff.py --base 5bf03fd0 -j 8` 0 for every touched file. No header in
`include/` changed, no archive or gendata change (`decls.json` stays valid). One new port header,
`port/include/t2_player_offsets.h`.

The script (new game, then demo 1's level and its two type-0x190 enemies):

```sh
cd OUT && SDL_VIDEODRIVER=offscreen T2_STATE_LOG=60 T2_WARP_TO=-82 T2_WAKE_ENEMIES=1500 \
  T2_TELEPORT="13300:2334.7,307.2,-1054.7;15400:2334.7,307.2,-1054.7;17500:2334.7,307.2,-1054.7" \
  build/app/turok2 --archive turok2.o2r --rom ../baserom.us.z64 --game --pak-blank --frames 20000 \
  --input "START@4400 A@4500 A@4700 A@4800 SX=80@13310..13358"     # (+ --draw for pixels)
```

(`T2_WARP_TO=-82` is level 81: a negative id is `~level`; the stick input turns the player
toward the enemy for the screenshots.)

What runs (Linux headless, low memory and `--high-memory`, no crash to frame 20,000):

| frame | what |
|---|---|
| 13300 | the teleport onto the enemy's spawn point; the enemy wakes, is alerted (`act 1`) |
| 13400-14340 | **15 hits**: 14 melee (mode 14, `ImpactEvent_AreaDamage` from its animation event) and 1 projectile (`CParticle__HitTarget`); health 100 -> 99 -> 97 -> 90 ... 6; each hit the red flash (`CCamera__SetHitFlash` 128/30), the pain sound 0x3AC0, rumble; the low-health speech 0x156 below 15 |
| 14400 | health 0: `CPlayer__Kill`, player mode 19, the death camera falls to the floor (letterboxed) |
| 14700 | the death cinema (level 164: Turok in the spirit void) |
| 15300 | **respawn** at level 81's checkpoint (1884.2, -153.6, -2601.0), health 100, **lives 3 -> 2** (HUD `x2`) |
| 16500 / 18660 | second and third deaths (lives 2 -> 1 -> 0; with 1 life left there is no cinema, as in the ROM) |
| 18960 | player mode 15: the **GAME OVER** window (lives 0) |
| 19500 | its countdown runs out: back to the legal screen and the boot sequence |

`--audio --wav` over the same run to 15,500 frames: no crash; the death cinema is the loudest
stretch (RMS 10,000-15,600).

Fixes (in the order found):

| # | where | cause | fix |
|---|---|---|---|
| w1-1 | `AI_HearNoise` (func_0025E52C) | `AlertState` view: the behaviour table/index at N64 CAI 0x130/0x133 | host body on CAI's members. **Enemies were never alerted** (`m_ActionTime` never rose, `m_Action` stayed 0): `AI_FindBestTarget` skips unalerted enemies for path targets, so `EnemyAI_PickAttack` never saw a path target and never chose melee; they only fired projectiles from point-blank range, which pass over the player (the hand is 121 units up, the template aims 11 degrees above the line: the ROM's behaviour) |
| w1-2 | `GameObject_GetAimRotation` (func_002673CC) | `LevelActor.lockOn` (N64 0x1C0, `m_pSightTarget`) and the target's y offset (0x54, `m_CollisionYOffset`) read at N64 offsets | host body (correctness: the fallback to the nearest player hid it in demo 1) |
| w1-3 | `GameObject_GetDeadCollisionHeight` (func_002683CC) | `m_pIntelligence` at N64 0x14: **the player's death crashed** (from `GameObject_GetCollisionHeight` once health is 0) | by member |
| w1-4 | 9 player-state files with `P_S32(player, 0xAB8)`-style raw N64 offsets (`CPlayer__ResetControlState`, `CPlayer__ResetHeadSway`, `PlayerDeathWait_Update`, `PlayerState15_Enter`, `PlayerState17_Enter`, `PlayerJumpPad_Enter`, `PlayerWade_Enter(Alt)`, `CPlayer__RegenerateHealth`) | the host CPlayer is larger: N64 0xAB8 (`m_DesiredJumpAngle`) is `m_Data.m_CheckPointWarpID` on the host, so **every cinema reset the checkpoint to 0** and a death respawned in level 0; `PlayerState15_Enter` read the lives at 0x52A to pick game over / continue | the P_* macros map the N64 offset to the host member under TARGET_PORT (`port/include/t2_player_offsets.h`, `T2PlayerOff`; an unknown offset traps); the JumpPad region store and `Player_OnMultiplayerKill`'s `player + 0x140` too; `PlayerState17_Enter`'s controller pointer pointer-wide |
| w1-5 | `GameOverMenu_Init` (func_0041A420), `CheatsMenu_Init` (func_0041A4D0) | the window's items + 0x30 / + 0x2D8 (MenuItem is larger on the host): **the game over window crashed** | `items[1].flags`, `items[18].flags` |
| w1-6 | `CScene__Blast` (func_00223A94) | `Entity` view (the AI, health and mode at N64 0x140/0x144/0x174) | hostview.py on CListedInstance (not reached in a run) |

Files others may touch (merge): func_0025E52C.c (AI_HearNoise: w2's enemy reactions depend on
it), func_002673CC.c, func_00223A94.c, func_0041A420.c / func_0041A4D0.c (menus: w5),
func_00255C80.c and func_00255E4C.c (the continue / multiplayer menus), func_002568CC.c.

Open (next):

1. **Level 52's type-0xC8 enemies** woken next to the player stay in mode 10
   (`EnemyMode_Move_Update`) with the player as sight target and do not move (w2's subject).
   The type-0x190 enemies of level 81 now walk, melee and fire.
2. `Menu_Construct_2` (func_0044C230.c, the paged copy) still puts the items at N64 0x1D8 after
   the window (func_00413230.c uses `MENU_SIZE` on the host): the menus built through it overlap
   their window on the host.
3. The level-81 enemies draw as black silhouettes with red eyes: check against the N64 (the
   level is dark; lighting suspected).
4. Region damage (lava, `AI_UpdateRegionFlags`' `m_DeathRegionTimer` view, func_00242E34) and
   drowning were not run; fall damage was (health 100 -> 99).
5. The continue path (a continue left: `gContinueMenuDesc`) was not reached: the game gives
   none on a fresh game.

Screenshots (640x480, checked by eye; not committed): the red hit flash over the enemy's
silhouette with the HUD at 91 (13481), the enemy's glowing eyes at point-blank range (13511),
the HUD falling 100 -> 98 -> 91 -> 45 -> 7, the letterboxed death camera on the floor (14401),
the death cinema (14801), the respawn at the level start with `x2` and 100 (15321), the GAME
OVER window over the level with `x0` and health 0 (18960-19320), the legal screen after it.

## Round 8, part w4: macOS, and host-dependent bugs both hosts had (branch names-R8_w4)

Detail: [MACOS.md](MACOS.md) section 10. Commits (asm_diff 0 against 5bf03fd0 for each `src/` one):
baea6826 (DirectDamage/func_00239058 sptr: the Mac crashed at frame 2400), dbf89598
(`CAI__UpdateModeAnim` takes a picker's result as s16: the Mother's attack anim now plays and hurts
the player on both hosts), 96845732 (`CScene__ComputeLights` reads `dirs[k]`, not `w + 128`),
6d67f0c8 (gendata: display-list w1 operands stay numbers: sShadowQuadDL's geometry mode was a code
address), ff7785b9 (gendata: `gMergedController`/`gControllers` N64 extents: Cont_InitAll wrote 4
host profiles over the following data; the legal screen lost lines; demo 1 now follows its input).
After them the journey, the warp and the Mother runs give identical logs and matching screenshots
on the Mac and on Linux. **Merge notes**: gendata.py also changed in w3 (558c0f04): both edits are
local (N64_EXTENT/RUNTIME_BSS, the Gfx w1 branch, the report line); drop decls.json and rerun
gendata. func_00243170.c (CAI__UpdateModeAnim) may meet w2's death-path work. With ff7785b9 the
boot's demo 1 kills an enemy at ~8200 and needs w2 58b5668a (EnemyAnim_PickDeath).

## Round 8, part w2: the player kills enemies (branch names-R8_w2)

Detail: [GAMEPLAY.md](GAMEPLAY.md) section 9. All src/include changes are `TARGET_PORT` or
`s32` -> `sptr`; `tools/asm_diff.py --base 5bf03fd0 -j 8`: 0 over all 3,938 files.

**Merge needs:** the archive rebuilt (`structs/intelligence.h`: `CEnemyIntelligence` 0x0C/0x10
are the u32 drop masks `m_DropMask`/`m_DropMask2`): sha1
`b6b9d484e6f97718e48ddc6d7f0ce9188dbb11c5`; `decls.json` deleted. `structs/region.h`
(`REGION_WORD` on the host) rebuilds most of the game library.

- **Shots hit enemies**: `Sweep_TestBox` read the node boxes' centres from the host `Mtx` as
  N64 halves; fixed, the pistol/shotgun/grenade launcher hit, wound and kill level 52's 0xC8
  enemies: blood, flinch (mode 36), death animations (modes 52/56), the corpse, the melt
  (mode 60). The death path is on host types (`EnemyAI_OnDamage`, `EnemyAI_OnHit`,
  `EnemyAI_PickDeathMode`, `EnemyAnim_PickDeath`, `EnemyMode_Death_Update`,
  `EnemyAnim_ExtremeDeath`, `GameObject_GetDeadCollisionHeight`, `AI_StartSound/StopSound`,
  `AI_UpdateRegionFlags`).
- **The 0xC8 "stuck in mode 10"**: `REGION_WORD` (a big-endian `{u16 info, u16 flags}` word)
  read as a host u32: every region edge looked closed to `Sweep_CrossRegionEdge`, so no enemy
  found a path to the player (path kind 3, the home point). With it and the alert fix (w1's
  `AI_HearNoise`, the same as mine) they run at the player and melee him: in level 52 his
  health goes 100 -> 20 in 1,600 frames; in demo 1 (v3's teleport) the two 0x190 enemies kill
  him (health 0 at frame 6,840), no crash to 8,600. Two more crashes on the melee path fixed
  (`EnemyAnim_PickAttack_B0A60`, `EnemyAI_CurrentHasComboEndAnim`).
- Debug-only: `T2_FACE_NEAREST=R` (aim at the nearest live enemy), the drop masks in
  `T2_ACTOR_LOG`.
- Regression (this branch, Linux headless, low memory): the 26,000-frame boot loop exit 0 (the
  same mode switches, the title again at 25,778); demo 1 with v3's enemy switches to 8,600 exit 0;
  **the firing journey (`journey_fire.txt`) exit 0 but its swim now ends at (795.2, -780.0,
  -37.4) against a wall instead of round 7's (315.9, -793.4, -1352.3)**: the divergence starts
  between 13,620 and 13,800 and goes away with the old `REGION_WORD` (checked), i.e. the closed
  water-region edges now stop the player as `Sweep_CrossRegionEdge` intends (its test read the
  info index before). Update the journey's reference positions after the merge.

Files others may touch (merge): **func_0025E52C.c (`AI_HearNoise`): take w1's 8afda2c4, mine is
the same fix and is not committed**; func_00242E34.c (`AI_UpdateRegionFlags`, region damage: w1
may touch the player's side of it); func_0025B2D4.c (`EnemyAI_OnDamage`); include/structs/region.h,
include/structs/intelligence.h; port/game/t2debug.c and port/app/game.cpp (debug switches).

Next: GAMEPLAY.md 9.5 (enemy lighting near the player, `EnemyAI_DrawNode` still N64 offsets,
the gore deaths, an enemy that drops something).

## Round 8, part w3: the boss fights and the ABI tail (branch names-R8_w3)

Detail: [GAMEPLAY.md](GAMEPLAY.md) section 9, [ABI.md](ABI.md) section 7. Every src/include change
is `TARGET_PORT`; `tools/asm_diff.py --base 5bf03fd0 -j 8` 0 over all 3,938 files after the
include edits, 0 on each later file.

- **The four boss levels run their fights** (Linux headless, low and `--high-memory`): the Primagen
  past its old stop (the spawner spawns its waves), the Blind One to its minion phase, the Mother
  takes plasma hits (`MotherBoss_Hurt` reached; the first hit used to crash), the Mantid Queen's
  8-wave script. No spot damage yet (the hits land on the Mother's body node).
- **gendata**: struct tables cut by splat labels now get their whole extent (20 tables: all the
  boss scripts, two mode tables, `gLevelMessages`, `gAdecHuffSegments`).
- **abicheck**: `narrow_ret` class, 0 sites; the K&R indirect calls reviewed (14, none harmful in
  linked code).
- Debug: `T2_BOSS_LOG=1` (port/game/t2bosslog.c, with `T2_STATE_LOG`).

**Merge notes.** Header changes: `include/structs/boss.h` (`LevelScriptState.region` sptr on
the host, `BossStep2` 0x08/0x0C typed, `BossMode` packed on the host), `include/globals/boss.h`
(sized toss tables): **drop decls.json and rerun gendata**; gendata itself changed (more data:
check `data_report.txt`'s "struct tables extended" list). No swap_spec change: the archive is
unchanged. Files others may touch: port/game/t2debug.c (one `T2_BOSS_LOG` hook after
`T2_ACTOR_LOG`), func_00225C84.c (`CScene__MarkGeneratorsSaved`), func_002683CC.c
(`GameObject_GetDeadCollisionHeight`: used by every dead actor's collision height: w1/w2),
tools/port/hostview.py, tools/port/abicheck.py. w4 (Mac) reported the Primagen spawner crash:
fixed here, w4 does not touch func_0040457C.

Next: the bosses' weak spots (GAMEPLAY.md 9.3), the Blind One after w2's kills.

## Round 8, part w5: progression: pickups, saves, the pause and options menus (branch names-R8_w5)

Detail: [GAMEPLAY.md](GAMEPLAY.md) section 9, [INPUT.md](INPUT.md) section 7. All src changes
are `TARGET_PORT`; `tools/asm_diff.py --base 5bf03fd0 -j 8` = 0 on every file changed (no
include/ change). **Merge needs: gendata again (delete decls.json)**: eight menu strings are now
declared `char[]` (they were swapped as words). The archive is unchanged.

- **Saves are big-endian on the pak** (port/game/t2save.c): the save game and the multiplayer
  profile swapped field by field, the level flags by their ISet plan, checksums over the
  big-endian bytes; a pre-round-8 host-order note still loads. `tools/port/mpk_saves.py` checks
  a .mpk the console's way.
- **Save -> quit -> load works**: `T2_SAVE_PORTAL` + the game's own save menus write the note
  ("SAVE WAS SUCCESSFUL"); a new boot, LOAD GAME, the note, NOTE INFO (Port of Adia, 0:02:20,
  Normal, 100, 3), YES: level 51 again; a save made in level 52 loads level 52 at its entry.
- **Pickups**: walking onto level actors collects them: life force (HUD counter 0 -> 16), the
  level 2 key ("LEVEL 2 KEY"), pistol bullets ("10 PISTOL BULLETS (CLIP)"); health is left at
  full health. Collecting crashed before (W5-9).
- **Pause, sound options, options** work: the sliders step (music volume to its minimum: the
  mix RMS ~1,700 -> ~40), the options toggle (control type, side, look spring...), GO BACK and
  RESUME (W5-10: crashed).
- Regression (final build, Linux low memory): the 26,000-frame boot loop (title again at 25,778),
  the firing journey (15,000: the player at (315.9, -793.4, -1352.3) as before), the warp to 52
  and back (16,000): exit 0.
- Debug-only: `T2_SAVE_PORTAL=F`, `T2_PICKUP_LOG=1`, `T2_ACTOR_TYPES=...`, `T2_DOOR_OPEN=F`.

Screenshots (640x480, by eye): the PAUSED menu; INVENTORY; the save portal menu (SAVE / HEALTH
AMMO / GO BACK) over level 51 and over Adon in the portal room; CONTROLLER PAK #1 SAVE NOTE (free
123, needed 90); SAVE WAS SUCCESSFUL; LOAD NOTE with "1. TUROK 2 A 90"; NOTE INFO; level 51 and
level 52 after loading; the life force counter 11 underwater; LEVEL 2 KEY; 10 PISTOL BULLETS
(CLIP); SOUND OPTIONS (music 4, SFX 13); OPTIONS (ARCADE, L, LOOK SPRING OFF).

Files others may touch (merge): func_00413230.c (`Menu_Construct` and the open functions: shared
menu code), func_0023F05C.c (`CPlayer__TouchPickup`: a debug hook), port/game/t2debug.c and
port/app/game.cpp (debug switches appended), docs. **Not changed here**: func_002683CC.c
(`GameObject_GetDeadCollisionHeight`): w1 fixed it (a1d24455); a fall to death in level 52
crashed there on this branch without it. Not touched: func_0041A420.c/func_0041A4D0.c (w1).

Next: doors (level 51's door stays closed when walked into, and `CAI__SetMode(door, 0)` does not
take: GAMEPLAY.md 9.4), switches/levers, health pickups with a damaged player (after w1), a
console/emulator save loaded on the port, `--high-memory` and macOS on these paths,
`MenuPool_OpenMenuParam`'s s32 pointers.


# Round 7, merged state (history)

Round 7 merged five branches, in this order: v1 (the particle system on the host), v4 (macOS
arm64 float conversions, `--bench`/`--pace`), v2 (weapons and firing), v5 (real level warps and
the boss levels) and v3 (level triggers, enemy AI, damage). Then it fixed what only the merged
tree reaches (C22-1..9 below: the enemies' attack path, the path tracks, `--high-memory`). This
page says how to build and run the result, how far it gets, and what comes next. The part
sections below keep each branch's own notes (detail pages: [GAMEPLAY.md](GAMEPLAY.md) sections
6 (v2), 7 (v5) and 8 (v3), [RENDER.md](RENDER.md) section 6 (v1), [PERF.md](PERF.md) and
[MACOS.md](MACOS.md) section 9 (v4), [ABI.md](ABI.md) section 6 (v5)); round 6 and earlier
follow as history.

The matching build is unchanged: `tools/asm_diff.py --base d3f05a74 -j 8` reports 0 differences
over all 3,938 files after every merge and every fix, and a clean `make clean && make setup &&
make build && make verify` gives the identical ROM (`build/turok2.us.z64: OK`, 123 shared
.rodata objects agree).

## Round 7: build and run

```sh
# 0. the matching build first: gendata and asm_diff read its ELF; keep a copy as build/ref.elf
make setup && make -j8 build && make verify && cp build/turok2.us.elf build/ref.elf

# 1. the archive, format 2.0. REBUILD IT this round: tools/assets/swap_spec.json changed (v1:
#    the particle block 0 stays big-endian; v3: the LevelEvent trigger mask is a u32; C22: the
#    path group header {s16 mode, s16 count} and the path point's tail as f32 + four u16).
python tools/assets/mkarchive.py baserom.us.z64 -o OUT/turok2.o2r -j 4     # ~12 s
python tools/assets/archive_check.py OUT/turok2.o2r baserom.us.z64          # OK, format 2.0

# 2. configure; the game library AND the runtime/audio libraries (gendata reads all three)
cmake -S port -B OUT/build -G Ninja -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ \
      -DCMAKE_BUILD_TYPE=Debug
ninja -C OUT/build -j8 turok2_game t2runtime t2audio

# 3. the game data as C into port/gen/ (gitignored). Drop the cache after ANY header change
#    (this round: structs/controller.h, structs/boss.h, globals/boss.h, structs/event.h,
#    rw/abu_protos.h).
rm -f OUT/decls.json
python3 tools/port/gendata.py --build OUT/build --cache OUT/decls.json -j 4      # ~1 min

# 4. re-run cmake so data.cmake sees port/gen/*.c, then the executable
cmake OUT/build && ninja -C OUT/build -j8 turok2

# 5. run headless (Linux: SDL's offscreen driver, no window)
cd OUT && SDL_VIDEODRIVER=offscreen T2_SHOT=shot.ppm T2_SHOT_EVERY=300 \
    build/app/turok2 --archive turok2.o2r --rom ../baserom.us.z64 \
    --game --draw --pak-blank --frames 26000
```

The archive of this tree: 26,124 units, sha1 `71317b96cefb3b321aad3516d29ac6a0b98fd7a1`;
`turok2` without `--game` reports fnv1a64 `952c0748e8d5f4a4` on all 3 backends. gendata: 1,537
data symbols (1,531 typed from a C declaration), 93 pointer targets typed as tables, 386 zero
.bss symbols, 4,041 pointer slots (2,932 relocated, 1,077 null, 32 unresolved).

**The runs** (`turok2 --archive turok2.o2r --rom ../baserom.us.z64 --game --draw --pak-blank`
plus):

| what | input and switches | frames |
|---|---|---|
| boot + the 3 attract demos | (none) | 26,000 |
| new game, the bow fired on the dock, the harbour swim | `--input @journey_fire.txt` ([GAMEPLAY.md](GAMEPLAY.md) section 6), `T2_STATE_LOG=60` | 15,000 |
| a real warp to level 52 and back | `T2_WARP_TO=1813 --input @exit52.txt` (section 7) | 16,000 |
| a boss level | new game, `T2_WARP_TO=-4` (Mother, level 3), `7501` (Mantid Queen), `7602` (Primagen) | 15,000 |
| enemies in demo 1 | `T2_TELEPORT=5600:2334.7,307.2,-1054.7 T2_WAKE_ENEMIES=1500 T2_ACTOR_LOG=4 T2_STATE_LOG=60` (section 8) | 8,600 |
| every weapon | new game, `T2_GIVE_WEAPONS=1` (+ `T2_WEAPON=<n>`, `T2_WEAPON_LIST=1`), Z to fire | |

`journey_fire.txt` (the bow from the wheel, two arrows on the dock, then the harbour):

```
START@4400 A@4500 A@4700 A@4800    # title -> main menu -> single player -> new game -> OK
A@12950..12990 SX=80@12960..12990  # weapon wheel, stick right: the bow (weapon 2)
Z@13060..13120                     # draw (hold Z), release: the arrow flies (20 -> 19)
Z@13200..13204                     # a quick second shot (19 -> 18)
CU@13300..13740                    # down the ramp, into the harbour water
SX=80@13750..13774                 # turn right about 90 degrees
CU@13780..14300                    # swim across the basin
```

`exit52.txt`: `START@4400 A@4500 A@4700 A@4800`, `CU@13100..13120` (out of the exit strip
`T2_WARP_TO=1813` put him in), `CD@13140..13300` (back into it: the game's own warp 1098 to level
52), `CU@15000..15300` (level 52's exit back to level 51).

Options (`turok2 --game`):

- `--input`: `BUTTON@F`, `BUTTON@F1..F2` (held), `SX=V@F1..F2` / `SY=V@...` (stick), `@FILE`
  ([INPUT.md](INPUT.md) section 6). The stick turns, C-up/C-down move, C-left/C-right strafe,
  R jumps, Z fires, A/B hold the weapon wheel, L the map.
- `--audio` / `--wav out.wav`; `--high-memory` (Linux: host pointers above 4 GB);
  `--offscreen-gl` (**macOS**: pixels with no window); `--no-window`.
- `--bench [FROM]` (per-VI host times: game CPU, audio, draw, per drawn frame, percentiles) and
  `--pace` (real-time 60 Hz VI pacing): [PERF.md](PERF.md).
- Debug switches (port/game/t2debug.c and game.cpp, **debug only**): `T2_STATE_LOG=N` (mode,
  level, the player, the weapon and its ammo, the wheel), `T2_ACTOR_LOG=N` (with
  `T2_STATE_LOG`: the N nearest enemies/turrets: AI mode, animation, health, target, the mode
  callback), `T2_TELEPORT=F:x,y,z[;...]`, `T2_WAKE_ENEMIES=R`, `T2_WARP_TO=<id>[@frame][,...]`
  (a negative id is `~level`), `T2_GIVE_WEAPONS=1`, `T2_WEAPON=<n>`, `T2_WEAPON_LIST=1`,
  `T2_SHOT_FROM=<frame>`, `T2_PARTICLE_LOG=N` (particle spawn calls), `T2_ANIM_EVENTS=0`
  (the animation events and hot points are ON by default now), `T2_SCENE_LOG`, `T2_MENU_LOG=1`,
  `T2_DLDUMP_AT=<frame>` (+ `T2_DLDUMP_VTX=1/2/3`: vertices, G_MOVEMEM bytes, image hashes),
  `T2_V2P_TRAP=1`, `--skip-pak-check`, `--start-mode N`.

## Round 7: how far it got (Linux, headless)

The boot loop (26,000 frames, no crash; the same mode switches at the same frames as round 6):

```
frame 2: mode 0 pak check;  298: mode 1 legal;  1070: mode 3 the 3D expansion pak
frame 1362: mode 4 the Acclaim logo;  1850: mode 5 the Iguana jungle;  4212: mode 7 TITLE
frame 5467: demo 1 (level 81);  13680: demo 2 (level 131);  20946: demo 3 (level 109)
frame 25778: the title again; frame 26000
```

**Firing** (the journey above, 15,000 frames, no crash): 12960 the wheel open; 13020 weapon 2,
the tek bow, ammo 20; 13080 the fire mode (weapon AI 4, the bow drawn); 13140 the arrow flies,
ammo 19; 13260 the second arrow, 18; 13300-13740 down the ramp into the water; 15000 the player
at (315.9, -793.4, -1352.3). With `T2_GIVE_WEAPONS=1`, 13 land weapons fire (v2's table in
GAMEPLAY.md section 6); the pistol in level 52 fires 24 rounds (50 -> 26) with the muzzle flash,
the shell casings and the bullet decals on the wall.

**The warp** (16,000 frames): 12955 `T2_WARP_TO=1813` reloads level 51 at its exit strip; 13200
the player walks back into it (warp state 1, the game's own warp 1098); **13260 level 52** at
(1280.0, 0.0, -3225.6), its stone corridor with ladders and walkways drawn; 15060 level 52's exit;
15120 level 51 again at (3819.5, 307.2, 614.4). The same run passes with `--high-memory`.

**A boss level**: `T2_WARP_TO=-4` loads level 3: the Mother's intro cinema draws the boss and
Turok in the slime pit (frame 14003), then gameplay at (83.4, 0.0, -723.9) to frame 15,000.
The Mantid Queen and the Primagen as in v5's table (GAMEPLAY.md section 7; the Primagen still
stops in `Path_ExpandNode` at 13,260: not rerun this round).

**Enemies** (demo 1, the switches above): the two enemies (type 0x190) wake, walk to the player,
take him as their sight target, **turn to face him and attack**: mode 18 (the projectile attack,
`EnemyMode_Attack_Update`), its animation (index 10, anim type 4300) plays and loops, and its
animation event spawns the projectile (`Actor_SpawnParticle` -> `CParticleSystem__Spawn`), which
flies at the player. **The player's health stays 100**: the projectile passes over his collision
cylinder (top 92 units above his feet) on a downward line from the enemy's hand and the sweep
tests only the cylinder's side (`Sweep_HitCylinder` with `frontOnly`), so it hits a static object
behind him (`CParticle__HitInstanceBody`). In level 52 (13 enemies, `T2_ACTOR_LOG`), an enemy of type
0xC8 woken next to the player takes him as its target and stays in mode 10 (its idle animation,
backing off); the pistol fired straight ahead does not reach it. **No enemy has died yet**:
next steps 1-2.

- `--audio --wav` through the boot (6,000 frames): 100.1 s, peak 22,928, RMS 2,226.2 (round 6:
  21,621 / 1,998.6; the animation events and the particles now play their sounds).
- **Linux `--high-memory`**: the 26,000-frame boot loop (all three demos, after C22-7..9), the
  firing journey (15,000) and the warp to level 52 and back (16,000): no crash.
- macOS: not rerun on the merged tree this round (v4 ran its branch and a scratch merge with v1:
  MACOS.md section 9; open there: some actor parts not drawn on the Mac with display lists
  identical to Linux, a post-display-list difference, G_MOVEMEM lights/viewports suspected).

Screenshots (640x480, checked by eye; not committed): the bow drawn with an arrow nocked on the
dock, the counter 20 (13121), the arrow gone and the counter 19 (13131); level 52's corridor with
its ladder and walkways (13401); the Mother's cinema: the boss and
Turok in the yellow-lit slime pit (level 3, 14003); level 52, the pistol's muzzle smoke, a shell
casing in the air and bullet decals on the wall, counter 49 (13381); demo 2 (14002); demo 1's
enemy test (the player faces the wall; the enemy is beside him).

Performance (v4, [PERF.md](PERF.md), Apple M5, Release): about 0.55 ms per drawn game frame in
level 51 with audio (the N64 budget is 33.3 ms), drawing is ~80% of it; unpaced the game runs
~60x real time; `--pace` holds 60 Hz VIs.

The port tests pass: `rnc_test` (14,426 streams, 0 failed), `t2asset_test` (26,124 units, 0
mismatches, ROM backend + the archive's plans identical), `t2asset_typed_test` (1,585 checks,
0 errors), `gbi_test` (0 mismatches), `t2os_test` (trace hash `2b2775877e0d226b`, the same on 3
runs), `t2audio_test --direct 0` (19 frames, peak 32,433), `t2audio_test --adec 0xC29AB4 7464`
(27,136 samples), the swoosh self test (OK, 0 failed).

ABI (`tools/port/abicheck.py --build OUT/build -j 4`: 3,074 TUs, 11,217 calls, 10,880 checked
against a definition, 186 function-pointer calls): harmful direct sites **0** (v5 fixed the 11),
indirect 1 (a `Destructible_Hurt` store, declaration only). `ptrdecls.py`: 0 left in `src/` (it
still lists the strlen returns and the uncompiled `include/functions.h`); `ptrargs.py` on a
clean game-library build log: 0. `f2ucheck.py`: 79 float -> unsigned conversions in 3,894
files, none new on a value that can be negative.

## Round 7: merge resolutions and fixes

| where | branches | resolution |
|---|---|---|
| func_00223F50 (`CScene__GetSwoosh`) | v1, v4 | v1's `protos/iset.h` include (5e724e40); v4's duplicate local declaration dropped |
| func_00226390 (`CScene__ClearListCounts`) | v1, v4 | already v1's on v4's branch (37a8cf99) |
| func_00257D48 (`WeaponActor_Update`) | v2, v4 | the same commit on both: one body |
| func_0022BA88, func_0022BFA0, func_0022C228 (u8 surface getters) | v1, v2 | v1's declarations (146bc42e); v2's reverted |
| func_00268460 (the wall radius) | v5, v3 | both gave it a host body and git merged them **without a conflict** into nested `#ifdef`s that did not compile: v3's kept (typed `CIntelligence`) |
| port/app/game.cpp, port/game/t2debug.c | v2, v5, v3 | every debug switch kept |
| GAMEPLAY.md | v2, v5, v3 | v2 is section 6, v5 section 7, v3 section 8 |
| STATUS.md | all | every part section kept |

Fixed on the merged tree (asm_diff 0 for each):

| # | symptom | cause | fix |
|---|---|---|---|
| C22-1 | ptrdecls: `IsCodePointer` declared `(u32)` in func_0028BDFC | v2 retyped its definition | `(uptr)` |
| C22-2 | the warp back into level 51 crashed in `Action_GoEnter` | N64 offsets (0x14, 0xD4); reached once v3's triggers matched | host body on `CGameObjectInstance`/`CActionIntelligence` |
| C22-3 | demo 2 crashed in `Path_ExpandNode` (an enemy on its path track) | the path points' regions resolved at the N64 0x20 stride into host `CGameRegion`s (0x40), stored in a 4-byte slot (`--high-memory`); the path header `{s16 mode, s16 count}` was unswapped, so `CPathTrack__Advance` never wrapped and walked off the points | the point keeps its ROM index, `CScene__GetPathRegion` resolves it; swap_spec: the header and the point's tail (f32 rotY + four u16 flags) typed. **Archive rebuilt** |
| C22-4 | the demo-1 enemies turned in place forever beside the player | `AI_AngleToPoint` read the yaw at N64 0x50: the angle to a target ignored the AI's own rotation | host body |
| C22-5 | then they picked an attack and nothing happened (mode 0x42BF0012) | `EnemyAI_PickAttack` returns s16, its callers declared s32: the host leaves the upper bits of the return register undefined (MIPS extends them) | 49 declarations of s16/u16/s8/u8 functions declared s32/int (11 functions: `EnemyAI_PickAttack`, `EnemyAI_GetBehaviorIndex`, `WeightedTable_Pick`, `CScene__GetObjectTypeID`, ...) take the narrow type on `TARGET_PORT` |
| C22-6 | enemy projectiles without their target | `Actor_SpawnParticle` read the velocity at N64 0x18 and the target at 0x1C0 (`m_pSightTarget`) | host body |
| C22-7 | `--high-memory`: crash in `CScene__GetInstanceSavedBit` at the demo-1 load | `Generator_Step(s32 actor, ...)` | sptr; `Throwable_Step`/`Action_Step`/`Door_Step` too, and on the host they pass the actor and AI to the mode callback (MIPS leaves a0/a1 untouched for it) |
| C22-8 | `--high-memory`: crash in `GameObject_TraceMove` in demo 3 (a door opening) | the impact handler func_00238824 took the actors as s32 | sptr |
| C22-9 | ptrdecls: `MantidQueen_Step` | its definition retyped in C22-7 | `(sptr, ...)` |

## Round 7: next (as written then)

1. **Enemy hits on the player**: the demo-1 projectile passes over the player's cylinder.
   Compare the aim point and the muzzle height with the N64 (the hot point the anim event
   spawns at, `CParticleSystem__Spawn`'s aim at `target`, the player's collision height 92.16 and
   y offset), and check `Sweep_HitCylinder`/`Sweep_TestInstance` against the ROM for the cap
   test. Then `ImpactEvent_*` -> `CPlayer__Hurt` (the HUD health).
2. **A kill**: aim at an enemy (the player's facing under `T2_TELEPORT`, or a teleport with a
   rotation) and fire; the death path is still N64 views: `AI_UpdateRegionFlags`,
   `DamageEvent_Set`, `EnemyAI_OnHit`, `EnemyAI_PickDeathMode` (func_0025DF04's `AttackState`
   view), the death animations (func_0025DF04, func_0025E248, func_0025FD98).
3. **The narrow-return class** is only checked by a regex scan (C22-5's script); teach abicheck
   the return width, and look at `int`-returning callbacks stored in tables.
4. **K&R indirect calls** (`temp_v0_2()` in mode-step functions: C22-7 fixed four) rely on MIPS
   leaving a0/a1 for the callee; search for the rest.
5. The Primagen fight (`Path_ExpandNode` at 13,260; recheck after C22-3), the boss fights with
   the boss on screen, the Blind One (level 1).
6. **macOS**: rerun the merged tree on the Mac (particles, firing, warps, enemies) and chase the
   undrawn actor parts (post-display-list: G_MOVEMEM lights/viewports).
7. `--high-memory` was run on the boot, the journey and the warp; run the enemies and the boss
   levels with it too.

## Round 7, part v1: the particle system (branch names-R7_v1)

`CParticleSystem__Spawn` and the particle path run on host types, so the **animation events
and hot points are on by default** (`T2_ANIM_EVENTS=0` turns them off). Details:
[RENDER.md](RENDER.md) section 6. All src/include changes are `TARGET_PORT`;
`tools/asm_diff.py --base d3f05a74 -j 8` reports 0 over all 3,938 files.

**Merge needs:** the archive rebuilt (`swap_spec.json`: particle-effects block 0 kept
big-endian, `"block0": "byte"`, honoured by `swapcheck.py`): sha1
`3ad7d24031060de1b4dad8df85b8c321227fe77c`; `decls.json` deleted (`structs/particle.h` changed:
`CPackedRange`, `CROMParticleDistance/Oscillation/Colors/Motion`; DLights_Load's
`gDLightBuffer[0x40]`).

What was wrong, in the order found:

| # | where | cause | fix |
|---|---|---|---|
| P1 | the templates (block 2) | nine 32-bit offsets per 0x3C record, relocated in place: no room for host pointers | `port/game/t2particles.c` builds a host copy per load (`CScene__DecompressParticles` and its twin); Spawn indexes it |
| P2 | block 0 sub-records | swapped as f32 (base and range of every `CPackedRange` exchanged); records overlap across types | kept big-endian, decoded per slot by the builder |
| P3 | func_0022D2FC.c (FAKEMATCH) | every view at N64 offsets (`EffectSystem.resource` at 0xEC64: `CISet__GetBlock(NULL/garbage, 1)`, the crash of round 6) | hostview twins of all 17 views, host aimEffect, player list head by name |
| P4 | 22 particle files | views at N64 offsets (projectile updates, HitTarget, Attach, StopLoopSound...) | hostview / host bodies |
| P5 | `CParticle__BuildCameraMatrix` | wrote the matrix at the N64 0x48, over `m_dwFlags`: freed particles left in the draw list, an endless loop | `m_mtxDraw[gEvenOdd]` |
| P6 | `CCamera__DrawGlares` | `CGlareSource` view over the particle draw list | hostview |
| P7 | `CScene__DrawParticles` | `scene + 0x18B0` | `&scene->m_ParticleSystem` |
| P8 | collision sweeps (particles move with `Collision_MoveInstance`) | static objects at the N64 stride, actor/box/face sweeps' views, the `{pointer, f32}` hit sorts | host CStaticObject stride, hostview, host-size comparators/swaps |
| P9 | `CScene__ClearListCounts` | N64 view: the scene's per-frame list counts (`m_nMovers`, `m_nKind1`, `m_nRegistered`...) were never reset on the host | hostview |
| P10 | `gDLightBuffer` | 16 bytes on the host (splat labels inside the 0x400 buffer): DLights_Load overran it | sized declaration for gendata |
| P11 | host template copies | freed on a reload while particles still used them | kept (one per scene load) |
| P12 | `CParticle__HitFloor/HitWall/HitCeiling` | the surface getters return u8, declared s32: the impact table indexed with eax garbage | u8 declarations (ABI class not caught by abicheck: see next steps) |
| P13 | `CParticleSystem__Construct` (and _2) | the particle data passed through the `s32 *m_pParticleEffects` handle: truncated with `--high-memory` | `*(sptr *)`, an `sptr` parameter |
| P14 | `CScene__GetSwoosh` | `CISet__GetBlock` without a prototype (implicit int): the trail record pointer truncated with `--high-memory` | `protos/iset.h` |
| P15 | func_0020B9F4 (particle models) | the cache entries' data pointers read as `*(s32 *)` (`--high-memory`) | pointer-wide |

How far it gets with the events on (Linux headless, low memory, `--game --draw --pak-blank`):

- **The boot loop, 26,000 frames: no crash (exit 0).** The mode switches are at the same frames as
  round 6 (1362 Acclaim, 1850 Iguana, 4212 title, 5467 demo 1, 13680 demo 2, 20946 demo 3,
  25778 the title again). The animation events fire from the Acclaim logo on (kinds 217/218
  from its hot points, 442...).
- **The scripted journey, 15,000 frames: no crash (exit 0).** Level 51 at frame 12,895, the same
  path as round 6 (13080 (-1540.7 -735.8 -982.4) state 3; 13440 (-1164.7 -780.7 -29.0);
  13800 (358.3 -780.7 -60.3); 14400 (623.2 -780.7 198.3); 14640 (795.2 -780.7 233.0)),
  health 100, the wheel requesting weapon 2. `T2_PARTICLE_LOG=50`: 350 spawn calls, all in
  the cinemas (none in level 51 on this path: no footstep hot points in first person; v2's
  firing spawns the arrow and its splash).
- **`--high-memory`: the boot to 12,000 frames, no crash** (after P13-P15).
- Particles on screen (RENDER.md 6.4): against round 6's binary, 16 of 138 frames from 1 to
  2770 differ, each where particles show (the Acclaim logo's cyan ring burst and sparks/dust,
  the Iguana jungle's mist and fireflies).
- Debug: `T2_PARTICLE_LOG=N` prints every N `CParticleSystem__Spawn` calls that reach templates.

Files others may touch (merge): func_0024032C.c, func_0023F880.c, func_0023FDF0.c,
func_0023DF70.c, func_0023D050.c, func_0023DBBC.c, the hit sorts (func_002407D8, func_0024079C,
func_0023DB98, func_0023DB5C) and func_002682CC (v3's collision area: agreed with v3);
func_00226390.c (`CScene__ClearListCounts`: changes what every list-driven system sees);
func_002284E0.c (`gDLightBuffer[0x40]`: v2 made the same fix, keep one); func_0022D2FC.c.

Next:

1. **The narrow-return ABI class** (P12) is not caught by abicheck: a regex scan finds 17
   functions defined with a u8/s8/u16/s16 return and declared s32/u32/sptr elsewhere
   (EnemyAI_GetBehaviorIndex, EnemyAI_PickAttack, WeightedTable_Pick/_PickStatic,
   CScene__GetObjectTypeID, Controller_IsPakWatched, __osContDataCrc...). Check each call site
   (some are in `#else` branches) and teach abicheck the return width.
2. The particle files not touched because no run reached them: func_00236A34, the Trail/Swoosh
   drawing (`Trail_DrawTextured`, `Trail_PlaceNodes`: typed but not checked against particles
   with trails), func_00232F38's big view (only `id` is read), `CParticle__SpawnAlongPath`.
3. Firing (v2) and enemy hits (v3) spawn projectiles and impacts through this path: their
   particles (bow arrows, bullets, blood) were not seen in this round's runs.
4. macOS with the events on was not run (v4); `--high-memory` was run to 12,000 frames only.

## Round 7, part v2: weapons and firing (branch names-R7_v2)

Detail: [GAMEPLAY.md](GAMEPLAY.md) section 6. The weapon change completes and every land weapon
tested fires in level 51 **once merged with v1** (the fire is an animation event ending in
`CParticleSystem__Spawn`): tested on a scratch merge of names-R7_v1 (events on, v1's archive
`3ad7d240`), not committed here. On this branch alone (events off) the boot (26,000 frames) and
the journey (15,000) run without a crash; the weapon AI switches and runs its fire modes.

- Weapon actor and modes on host types (v2-1..6: `WeaponActor_Update` is v4's commit
  cherry-picked, the same text on both branches), the barrels' pose callbacks on
  `T2PoseContext`, the player views in the switch/ready/select code.
- Rumble on host types: `include/structs/controller.h` (ControllerProfile's rumble pool and
  effects at host sizes, under TARGET_PORT): **gendata again (delete decls.json)**; the first
  shot crashed in `RumbleCurve_Advance` without it.
- Mode animation pickers: `IsCodePointer`/`IsDataPointer` host bodies (the executable's text),
  `CGameObjectInstance__PickAvailableAction` reads its untyped tables big-endian (pistol,
  mag 60, shredder, grenade launcher... fire). Enemies use the same pickers (v3 told).
- Impact-event values are two host-order words (agreed with v1, a9c15e5e): FlashScreens and
  the shakes extract their bytes/halves from them.
- Pickups typed (`Pickup_FindWeaponEntry`, `HasRoomForAmmo`, `CollectPickup`), the weapon loop
  sounds' node, `ImpactEvent_Send`'s registered list.
- Debug-only: `T2_GIVE_WEAPONS=1`, `T2_WEAPON=<n>`, `T2_WEAPON_LIST=1`, `T2_SHOT_FROM=<frame>`,
  the ammo in `T2_STATE_LOG`.

Results (scratch merge): 13 weapons spend ammo when fired (tek bow, bow, pistol, mag 60, charge
dart, shredder, plasma, firestorm, sunfire, PFM, grenade, scorpion, flame thrower), the HUD
counter follows, muzzle flashes / casings / smoke / the arrow and its splash draw, `--audio`
firing sounds (RMS 200 -> 2,500-4,200), the Talon swings; no crash over 17,200 frames.

Merge notes: files others touch: func_00257D48.c (v4, identical commit), func_0022BA88/0022BFA0/
0022C228.c (my u8 getter fix was reverted: keep v1's 146bc42e), func_002284E0.c (gDLightBuffer:
only v1's change, I dropped mine). The journey's swim after 13450 differs from round 6 (see
GAMEPLAY.md section 6: round 6's `WeaponMode_Ready_Update` wrote into the player). Open:
`--high-memory` + events dies in `Blast_CanHit` (v3's damage path).

## Round 7, part v5: level warps, bosses, ABI (branch names-R7_v5)

[GAMEPLAY.md](GAMEPLAY.md) section 7, [ABI.md](ABI.md) section 6. **A real warp works end to
end**: from level 51 the player walks into the harbour's exit region (warp id 1098, read by
`CPlayer__Update` from the region info) and level 52 loads and draws at its arrival point; level
52's exit takes him back (Linux low and `--high-memory`). `T2_WARP_TO=<id>[@frame],...` (debug)
calls `CPlayer__Warp` from gameplay. The boss levels load: the Mantid Queen (level 2, its intro
cinema and the arena), and with v1's particles the Mother (level 3) and the Primagen (level 4)
arenas draw; the Primagen stops at 13,260 in `Path_ExpandNode`. abicheck: **0 harmful direct
sites** (was 11), 1 indirect store. The remaining player-path views of round 6 are done.

Regressions checked: the scripted journey (15,000 frames) gives the same positions as round 6
(358.3/-60.3 at 13800, 795.2/233.0 at 15000), alone and merged with v1.

**Merge notes.**
- **Merge v1 (particles) with or before v5.** v5's `Generator_Setup` fix installs the
  generators' step callback, which the host never had: their spawns now reach
  `CParticleSystem__Spawn`, so this branch ALONE crashes the boot in attract demo 3 (level 109,
  `Generator_SpawnEnter`). With v1 merged (scratch test, v1 at 146bc42e) the boot runs 26,000
  frames clean and the journey is unchanged.
- **Header change**: `include/structs/boss.h` (TotemDefenseState moved above LevelScriptState and
  placed in it as `totemDefense`; `LevelScriptBlock` under TARGET_PORT), `include/globals/boss.h`
  (`D_8012F604` declared under TARGET_PORT): drop decls.json and rerun gendata. No swap_spec or
  asset change: the archive is unchanged (v1's archive, 3ad7d240, for the merge).
- Files others may touch: func_0021A500.c (one call, the IsOnGround argument), func_00219BC0.c
  (one call), func_0026841C.c, func_00268460.c, func_002685F0.c, func_00268798.c,
  func_002687FC.c (actor helpers used by AI/collision: v3), func_00262D74.c (generators),
  func_00238DF8.c + port/game/t2abi.c (boss impact handlers: v3 converted the neighbours),
  port/game/t2debug.c and port/app/game.cpp (`T2_WARP_TO`).

Next: the Primagen fight (`Path_ExpandNode`; the level-script views anchored at the
`gLevelScript_*` symbols with N64 deltas, 41 files), the Blind One (level 1), boss fights with
the boss on screen (scripted triggers or a debug switch).

## Round 7, part v3: enemies and damage (branch names-R7_v3)

Details, the fix table v3-1..14 and next steps: [GAMEPLAY.md](GAMEPLAY.md) section 8.

- **Level triggers work on the host.** The trigger mask (word 0 of every `LevelEvent`) reached
  the host byte-reversed (the archive copied it as bytes), so no region or actor trigger ever
  matched; `include/structs/event.h` types it (`u32 m_Mask`, host only) and **the archive must
  be rebuilt** (this branch alone: sha1 `01311a4b2908acfbd3e9247c9aec27e3f246e4a9`; v1 also
  changes swap_spec, so the coordinator builds one archive from the merged headers). Plus the
  CScene record-table and bit-section reads and the trigger state test (v3-2..4), and the
  delayed-event queue's s32 event casts (v3-14, `--high-memory`).
- **Enemies run their AI**: level 51 has no enemies (doors, destructibles, actions only); the
  attract demos have them (level 81: 9, level 131: 53), asleep until their level event
  (`EnemyMode_WaitForTrigger`). Ported on the way: the wall radius, `AI_FindBestTarget`,
  `AI_UpdatePathHeading`, `AI_UpdateTargets`, the player-pool walks (`CPlayer.m_pNext`), the area
  damage handlers' actor params (v4's macOS crash), `Blast_CanHit`/`ApplyBlast` (v2's
  `--high-memory` crash).
- Results (scratch merge with names-R7_v1, Linux headless): the boot to 26,000 frames (all three
  demos, the AI running in demo 2), the new-game journey to 15,000 frames in low and high memory
  (unchanged end (795.2, -780.7, 233.0)). Demo 1 with `T2_TELEPORT=5600:2334.7,307.2,-1054.7
  T2_WAKE_ENEMIES=1500`: two enemies wake, walk to the player and keep him as their target;
  they **do not attack yet** (their animation index stays 0: v2's ce8cb3da `CAI__UpdateModeAnim`
  pickers are the lead), so no health change and no death was reached this round.
- Debug (DEBUG-ONLY): `T2_ACTOR_LOG=N`, `T2_TELEPORT=F:x,y,z`, `T2_WAKE_ENEMIES=R`.

Merge notes: **merge v1 before (or with) v3**: on this branch alone the boot crashes in demo 2
(frame ~13,700) in `Collision_SweepActors` (func_0024032C, ported by v1) as soon as an enemy
looks for the player. Files: include/structs/event.h (archive), port/game/t2debug.c,
port/app/game.cpp (two debug hooks after T2_STATE_LOG), src func_0022515C, func_002252A0,
func_00224CF8, func_0026E4B8, func_002253F8, func_00225354, func_00268460, func_00243C90,
func_002438C0, func_002399F8, func_00239C28, func_00246108, func_0025EEA4, func_002532EC,
func_00253370, func_00253018, func_002534E0, func_0023876C, func_002387C4, func_00238B2C,
func_00238B84, func_00238BDC, func_00238C34, func_00238C8C, func_00238CE4, func_0023726C,
func_00237AFC. The scratch merge with v1 was conflict-free.

## Round 7, part v4: macOS, arm64 float conversions, performance (branch names-R7_v4)

Commits (on the Mac clone `~/Documents/Proyectos/turok2_macport`, branch `names-R7_v4` from
d3f05a74). Every `src/` change: asm_diff 0 against d3f05a74 on the box. No `include/` changes.
The archive is unchanged (`47e8f73c…`).

| commit | what |
|---|---|
| 6b74c760 | `WeaponActor_Update` (func_00257D48) host body. The N64 AI view zeroed the high half of `m_pfStep`, which crashed macOS entering mode 13. It also ran the mode table's row 0 instead of the current mode, **on Linux too**, and that is why the weapon switch never completed. Now the level-51 journey switches to weapon 2 (the bow, 20 arrows in the HUD) and Z enters the fire mode |
| de2fe1cd | `--bench [FROM]` and `--pace` (debug): see [PERF.md](PERF.md) |
| b07b8d5d, 37a8cf99 | `func_00225C08` (noise to kind-1 actors) host body: its FAKEMATCH tests an uninitialised local, which clang -O2 compiles to a trap. `CScene__ClearListCounts`: names-R7_v1's version taken byte for byte, so it merges clean |
| 18150edd | **arm64**: negative float -> u32 conversions saturate to 0 (`fcvtzu`); the R4300 and x86-64 wrap. `T2_F2U` (TARGET_PORT: through s32) in the bone-matrix packing (packMtx: the Acclaim logo and every skinned actor lost their negative matrix elements on the Mac), the vertex-pattern hash, the scene light direction, the fill-rectangle corners. `tools/port/f2ucheck.py` lists all 99 float -> unsigned conversions. `-fsanitize=float-cast-overflow` confirms that none is left on the level-51 journey |
| 321d1c9b, 37a8cf99 | `T2_DLDUMP_VTX=2/3`: every vertex, G_SETTIMG image hashes/bytes, G_MOVEMEM bytes (debug) |
| ac622745 | `CScene__GetSwoosh`: `CISet__GetBlock` declared (an implicit `int` return truncated it on macOS once v1's particles spawn trails) |

Results ([MACOS.md](MACOS.md) section 9, [PERF.md](PERF.md)):

- macOS, this branch: the new game with the scripted journey runs to 15,000 frames with
  `T2_STATE_LOG` identical to Linux. Debug and an `-O2` build agree. 26 of 30 screenshots match
  Linux within 1% of the pixels. The other 4 are actor parts that Linux draws and the Mac does
  not (the boat and Turok's hand in level 51), although the display lists are identical, every
  vertex and matrix included. That is open.
- The boot stops entering attract demo 1 (frame ~5470) **on both hosts**: v4-1 makes the demo's
  weapon fire, and firing needs v1's `CParticleSystem__Spawn`. With v1 merged (scratch), the Mac
  runs into the demo and the new game into the mode-12 cinemas. Truncations found there were
  fixed by v1 (`ModelSweep_TestModel`, b8bca0e3) and v3
  (`ImpactEvent_AreaDamage` & co., e26d87e9; `ImpactEvent_LevelSpecial` fixed by v5 in 06477da5); `RumbleCurve_Advance` (`gCurrentFB[1]` NULL,
  `--offscreen-gl` only) is open: retest with v2's rumble fixes (be9c4bf8).
- Performance (M5): level 51 costs 0.55 ms per drawn frame in Release and 1.05 ms in Debug (the
  N64 budget is 33.3 ms). Drawing (Fast3D + GL) is about 80% of that. Paced at 60 Hz VIs / 30
  fps, Release has no late VI in level 51; the late VIs are level loads only.

To merge: `src/us/code/func_00257D48.c` (v2 may write the same function: keep one; mine runs
the current mode), `func_00226390.c` (identical to v1's), `func_00223F50.c` (one line; v1 fixed it too in 5e724e40: keep either),
`func_0021B95C.c`/`func_0021285C.c`/`func_0027C4E4.c`/`func_0027F64C.c` (a `T2_F2U` macro at the
top of each), `port/app/game.cpp`/`main.cpp`/`game.h`/`offscreen_gl.*`,
`port/audio/t2audio_mgr.c`/`t2audio.h` (`T2Audio_GetTimes`). After the merge, rerun
`tools/port/f2ucheck.py` on new host bodies, and run the merged tree on the Mac and with Linux
`--high-memory`.

# Round 6, merged state (history)

Round 6 merged five branches, in this order: u1 (gameplay draws the level: the cache hash
bucket stride, the camera visibility bits, the HUD), u5 (ABI and pointer width: indirect calls,
`--high-memory`), u4 (macOS parity and `--offscreen-gl`), u2 (animated actors) and u3 (stick
input, turning, the water, the weapon wheel). Then it fixed what only the merged tree reaches.
This page says how to build and run the result, how far it gets, and what comes next. The part
sections below keep each branch's own notes (detail pages: [RENDER.md](RENDER.md) sections 4-5,
[ABI.md](ABI.md) section 5, [64BIT.md](64BIT.md) part 4, [MACOS.md](MACOS.md) section 8,
[GAMEPLAY.md](GAMEPLAY.md) section 5, [INPUT.md](INPUT.md) section 6); round 5 and earlier
follow as history.

The matching build is unchanged: `tools/asm_diff.py --base 6d04fabb -j 8` reports 0 differences
over all 3,938 files after every merge and fix, and a clean `make clean && make setup && make
build && make verify` gives the identical ROM (CRCs good, 123 shared .rodata objects agree).

## Round 6: build and run

```sh
# 0. the matching build first: gendata and asm_diff read its ELF; keep a copy as build/ref.elf
#    (the asm_diff/gendata tools expect it, and `make clean` deletes it)
make setup && make -j8 build && make verify && cp build/turok2.us.elf build/ref.elf

# 1. the archive, format 2.0. REBUILD IT this round: include/structs/region.h types the warp
#    ids (CRegionInfo 0x48/0x4C as u16), so the swap plans changed.
python tools/assets/mkarchive.py baserom.us.z64 -o OUT/turok2.o2r -j 4     # ~12 s
python tools/assets/archive_check.py OUT/turok2.o2r baserom.us.z64          # OK, format 2.0

# 2. configure; the game library AND the runtime/audio libraries (gendata reads all three)
cmake -S port -B OUT/build -G Ninja -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ \
      -DCMAKE_BUILD_TYPE=Debug
ninja -C OUT/build -j8 turok2_game t2runtime t2audio

# 3. the game data as C into port/gen/ (gitignored). Drop the cache after ANY header change
#    (this round: structs/model.h, structs/pose_host.h, structs/region.h).
rm -f OUT/decls.json
python3 tools/port/gendata.py --build OUT/build --cache OUT/decls.json -j 4      # ~1 min

# 4. re-run cmake so data.cmake sees port/gen/*.c, then the executable
cmake OUT/build && ninja -C OUT/build -j8 turok2

# 5. run headless (Linux: SDL's offscreen driver, no window). --pak-blank: a blank formatted
#    Controller Pak in memory (--pak-dir DIR: turok2_pak<N>.mpk files; --no-pak: none).
cd OUT && SDL_VIDEODRIVER=offscreen T2_SHOT=shot.ppm T2_SHOT_EVERY=300 \
    build/app/turok2 --archive turok2.o2r --rom ../baserom.us.z64 \
    --game --draw --pak-blank --frames 26000
```

Options (`turok2 --game`):

- `--input` (INPUT.md section 6): `BUTTON@F` (4 frames), `BUTTON@F1..F2` (held), `SX=V@F1..F2` /
  `SY=V@...` (stick, -128..127), `@FILE` (a script, `#` comments). Buttons: A B Z START DU DD DL
  DR L R CU CD CL CR. Controls: the stick turns/looks, C-up/C-down move, C-left/C-right strafe,
  R jumps, Z fires, A/B hold the weapon wheel (the stick picks the slot), L the map.
- **The scripted journey** (a new game to level 51 and through the harbour), as a file:

  ```
  START@4400 A@4500 A@4700 A@4800     # title -> main menu -> single player -> new game -> OK
  CU@13000..13440                     # level 51: down the ramp, into the harbour water
  SX=80@13450..13474                  # turn right about 90 degrees
  CU@13480..14000                     # swim across the basin to the far wall
  A@14050..14090 SX=80@14060..14090   # weapon wheel open, stick right: weapon 2 requested
  Z@14150..14250                      # fire
  SX=-80@14300..14324                 # turn back left
  CU@14330..14800
  ```

  `T2_STATE_LOG=60 build/app/turok2 ... --game --draw --pak-blank --input @journey.txt --frames 15000`
- `--audio` / `--wav out.wav`: sound (sfx, music, speech).
- `--high-memory` (Linux): no 32-bit arena, no low stacks: host pointers above 4 GB, the
  closest thing to macOS on the box.
- `--offscreen-gl` (**macOS**): Fast3D's OpenGL renderer on a CGL context with no drawable and
  no window; `T2_SHOT` reads the game framebuffer (MACOS.md section 8). `--no-window` (any
  host): a virtual pad on port 1, no pixels.
- Logs: `T2_STATE_LOG=60` (mode, level, the player: position, rotation, state, region, floor,
  the requested weapon, the wheel, the weapon actor's AI and animation), `T2_SCENE_LOG` (the
  camera view box, the visible box, the loaded grid sections), `T2_MENU_LOG=1`,
  `T2_DLDUMP_AT=<frame>` (+ `T2_DLDUMP_VTX=1`, `T2_DLDUMP_MAX`), `T2_V2P_TRAP=1` (trap truncated
  addresses), `T2_ANIM_EVENTS=1` (animation events on: crashes, section 4). Debug:
  `--skip-pak-check`, `--start-mode N`, `gdb -batch -ex run -ex bt --args ...`.

gendata on this tree: 1,537 data symbols (1,531 typed from a C declaration), 93 pointer targets
typed as tables, 386 zero .bss symbols, 4,041 pointer slots (2,932 relocated, 1,077 null, 32
unresolved). The archive: 26,124 units, sha1 `47e8f73c2bb7304f7f6c3e0ac34ad478f1bdf15f`;
`turok2` without `--game` reports fnv1a64 `c7909f8a7049feac` on all 3 backends.

## Round 6: how far it got (Linux, headless; macOS per u4)

The boot loop, `--game --draw --pak-blank --frames 26000` (158 s, no crash):

```
frame 2:     mode 0    pak check;   frame 298: mode 1, the legal screen
frame 1070:  mode 3    "EXPANSION PAK FOUND!", the 3D pak
frame 1362:  mode 4    cinema 100: THE ACCLAIM LOGO RENDERS AND ANIMATES (an animated actor)
frame 1850:  mode 5    cinema 90: the Iguana jungle
frame 4212:  mode 7    TITLE SCREEN
frame 5467:  mode 13   attract demo 1 (level 81): the level, the Talon in Turok's hand, the HUD
frame 8800:  the boot again; 13680 attract demo 2 (level 131); 20946 attract demo 3 (level 109)
frame 25778: the title again; frame 26000
```

The new game with the scripted journey (`T2_STATE_LOG=60`, 15,000 frames, no crash):

```
frame 4455:  mode 8    main menu;  4555 mode 13, the single-player menu
frame 4855:  mode 11   new-game cinemas: Adon and Turok, the Port of Adia, soldiers, enemies
frame 8973:  mode 12   the next cinemas (levels 47, 45, 46, 48, 49), the warp portal
frame 12895: mode 13   LEVEL 51 GAMEPLAY: the Port of Adia harbour draws with the HUD
                       (health 100) and the Talon in Turok's hand
13080 (-1540.7 -735.8  -982.4) state 3, walking down the ramp (C-up)
13200 (-1530.8 -789.8  -485.4) state 9, in the harbour water
13440 (-1164.7 -780.7   -29.0) turn right (SX=80): rotY -3.14 -> -1.55
13800 (  358.3 -780.7   -60.3) swum across the basin to the far wall
14100 the weapon wheel opens (A), its 3D weapon models draw; the stick picks weapon 2
      (requested 2; the switch waits, the Talon stays: section 4); Z: no crash
14400 (  623.2 -780.7   198.3) turned back left (rotY -2.28), swimming on to (795.2, 233.0)
```

The path differs from u3's branch only where u1's loaded grid sections add collision (the ramp
now takes him into the water at x -1530 instead of -1157).

- `--audio --wav` through the boot (6,000 frames): 100.1 s, peak 21,621, RMS 1,998.6 (round 5:
  21,304 / 1,996).
- **Linux `--high-memory`**: the 26,000-frame boot loop (all three demos, the same mode switches
  at the same frames as low memory) and the new game to level 51 (15,000 frames), no crash, after
  one fix (C21-1 below).
- macOS (u4, on its branch before the merge): the boot to 26,000 frames, the new game into level
  51 and `--audio` identical to Linux; `--offscreen-gl` screenshots match Linux (29/29 new-game
  frames). The merged tree was not rerun on the Mac this round.

Screenshots (640x480, `T2_SHOT_EVERY`, checked by eye; not committed): frame 1801 the Acclaim
logo (the letters in place, the rainbow swoosh); 2403 the IGUANA letters in the jungle; 4500 the
title; 6500 demo 1 (a blue-grey corridor with a door, the Talon in hand, the weapon models,
health 100); 21200 demo 3 (green brick walls, the Talon); 8800 a new-game cinema with Adon and
Turok standing on a blue backdrop; 10302 soldiers fighting in a stone hall; level 51: 13001 the
harbour from the ramp in fog (towers, a moored boat, the Talon and the HUD), 13451 the boat's
hull and mast at the dock, 13601 the brick arches across the basin, 14101 the open weapon wheel
with the weapons in 3D.

The port tests pass: `rnc_test` (14,426 streams, 0 failed), `t2asset_test` (26,124 units, 0
mismatches; ROM backend + the archive's plans identical), `t2asset_typed_test` (1,585 checks x3,
0 errors), `gbi_test` (0 mismatches), `t2os_test` (trace hash `2b2775877e0d226b`, the same on 3
runs), `t2audio_test --direct 0` (19 frames, peak 32,433), `t2audio_test --adec 0xC29AB4 7464`
(27,136 samples), the swoosh self test (OK, 0 failed).

ABI (`tools/port/abicheck.py --build OUT/build -j 4`: 3,073 TUs, 10,824 direct calls checked,
186 function-pointer calls): harmful direct sites **11** (ptr_from_int 4, void_used 2,
missing_args 5; u5's branch: 12), indirect 4 (stores, declaration-only); struct_class,
struct_scalar, float_int 0. `ptrdecls.py`: 0 left in `src/` after this round's Throwable_Step
fix (it still lists the strlen returns and the stale, uncompiled `include/functions.h`);
`ptrargs.py` on a clean game-library build log: 0.

## Round 6: merge resolutions and fixes

| where | branches | resolution |
|---|---|---|
| func_00279158, func_0027A2C4, func_0027A4DC (`G_SETTIMG` image words) | u5, u4 | the same fix (`(GBI_ADDR(p))` without the truncating cast); u5's text kept |
| func_00213A70 (`Lighting_BuildLitVertices`) | u5, u4 | u5's `sptr model` (one signature on both targets) |
| func_00214F14, func_00220208 (lit-vertex callbacks) | u5, u4, u2 | u4's host bodies: the model from `CCacheEntry.memEntry->data`, the box and matrices by member (u5's and u2's `*(sptr *) *arg1` read the same pointer); the N64 path unchanged |
| func_00227184 (`Sort_Insertion`) | u5, u4 | u5's `t2_uptr` retype of the matched body |
| func_00252ADC, func_00254B04 (camera word 0x54C) | u2, u3 | the same fix (`m_Menus.count` by member): u2's text |
| func_0021ABC8 (`FireAnimEvents`) | u2, u3 | u2's version, a superset of u3's (the ObjectLinks view, the pointer-wide `CISet__GetBlock`, the events gate) |
| func_0021A150 (`m_nAnims`) | u2 | u2's committed fix (u3's was uncommitted) |
| port/app/main.cpp (usage) | u4, u3 | both lines |
| STATUS.md, RENDER.md | all | every part section kept |

Fixed on the merged tree (asm_diff 0):

| # | symptom | cause | fix |
|---|---|---|---|
| C21-1 | `--high-memory` crashed at frame 1362 (the Acclaim cinema) in `CISet__GetBlock` from `CAnimationState__Load` | `CAnimationState__GetData` (func_0021793C) read the cache entry's first word, a host pointer, as `s32` (reached only once u2's actors animate) | `*(sptr *)` |
| C21-2 | ptrdecls.py: `Throwable_Step` declared `(s32, sptr)` in func_00264C94 | u5 retyped the step callbacks after its last ptrdecls run | `(sptr, sptr)` |
| C21-3 | warp ids between levels would read byte-reversed | `CRegionInfo` 0x48/0x4C were `u8 unk_48[8]` (copied as bytes) but `CPlayer__Update` reads u16 there | `u16 m_WarpID` (0x48), `u16 m_PendingWarpID` (0x4C), bytes 0x4A/0x4E unchanged; the archive rebuilt; the level-51 journey is unchanged |

Demo 1's darkness (u1) is gone on the merged tree: with u2's actors, lights and shadows its
frames show the corridor, the door and the weapon models at normal brightness.

## Round 6: next (as written then)

1. **`CParticleSystem__Spawn`** (func_0022D2FC.c, 1,752 lines of local N64-layout views):
   firing, impacts, footsteps, blood and the animation events all end there. With
   `T2_ANIM_EVENTS=1` the boot crashes at frame 1362 in `CISet__GetBlock(sys->resource, 1)`
   (`FireAnimEvents` -> `ImpactEvent_Send` -> `Actor_SpawnParticle` -> `Spawn`): the
   `EffectSystem` view (the resource at N64 0xEC64, which must be `sptr`), then the
   Effect/Owner/Actor views and the particle table entries (`entry->params`). Port it on host
   types (hostview.py for the views), then turn the events on by default.
2. **The weapon switch**: the wheel requests weapon 2 but the switch waits for the weapon
   actor's animation (events off, in the water); retest on land once (1) is in.
3. **Bosses and the remaining actors**: `TotemDefense_InitFight` is all N64 offsets; the boss
   draw paths are untested.
4. **Warp ids**: typed this round (C21-3); test a real warp (a level exit) end to end.
5. **ABI**: abicheck's 11 harmful direct sites (ptr_from_int 4, void_used 2, missing_args 5) and
   4 indirect stores.
6. **macOS**: rerun the merged tree on the Mac (u2's actors and u3's input were not on u4's
   branch) with `--offscreen-gl` screenshots.
7. The remaining N64 views on the player path (GAMEPLAY.md section 5):
   `CPlayerPool__UpdateMatchClock`, `Action_CheckRequiredItem`, func_00271694.

## Round 6, part u1: gameplay draws the level (branch names-R6_u1)

[RENDER.md](RENDER.md) section 4. Level 51 now **draws in gameplay**: the Port of Adia start
(the ramp, brick and panelled walls, the arch row, crates, sky, fog) from frame 12903 on, with the
health HUD. Attract demos 2 and 3 draw their levels; demo 1 draws its geometry, very dark (maybe its
real look). "NEW CHEAT ENABLED" no longer appears. Four causes:
the gameplay camera's visibility mask read at an N64 offset of the host `CGameRegion` (0: every
object culled), **the cache hash lookups' N64 bucket stride** (0x10 vs the host's 32-byte
`HashEntry`: no asynchronous request ever hit, so gameplay never got lit vertices; cinemas load
synchronously and got through), the preview-options field written at an N64 offset of `gScene`
(the cheat message), and the `COverlay` constructors/script start on N64 views (no HUD). Boot
26,000 frames (158 s) and the new game to frame 15,000 (82 s) run without a crash. The player
still stops at (-1266.6, -779.5, 260.0) (u3).

Merge notes: `src/us/code/func_00203CA0.c`, `func_002034F4.c`, `func_00205A2C.c`,
`func_0027D160.c`, `func_0042C3BC.c`, `func_0027ACC4.c`, `func_0027AD00.c` (all `TARGET_PORT`
blocks, asm_diff 0), `port/game/t2debug.c` (`T2_SCENE_LOG`). The archive and gendata are unchanged
(no header edits).

## Round 6, part u5: ABI and pointer width (branch names-R6_u5)

- `tools/port/abicheck.py` checks **indirect calls** (function-pointer tables/fields, gendata
  initializers included) and **asm-label aliases**; new `tools/port/abiptrfix.py` retypes the
  declarations behind `ptr_from_int`. Harmful sites **193 -> 12** (ptr_from_int 185 -> 4, a
  literal 0; void_used 2 and missing_args 6 left, ABI.md section 4); indirect 210 call + 4 store
  -> 0 call + 4 store (declaration-only). ABI.md section 5.
- Gameplay-relevant ABI fixes: the six AI step functions now pass (actor, ai) to the mode
  callbacks (doors, turrets, destructibles, throwables, actions, interactive anims: MIPS
  pass-through), `CAI__CallHurt` passes (actor, ai, damage) to the hurt handler, the
  Destructible view writes its callbacks at host CAI offsets, impact handlers PlaySound /
  func_00239350 read the sound id from the right register.
- **Linux `--high-memory` runs**: the boot, both cinemas, the title, attract demo 1, demo 2's
  start (14,000 frames) and a **new game to level 51 identical to low memory** (was: crash at
  boot). 64BIT.md part 4 lists the six crash sites fixed on the way (texture image words
  truncated by `(unsigned int)(GBI_ADDR(p))`, the menu file's `s32` asm aliases, the lit-vertex
  model pointer, `GameObject_GetRegionInfo`, `Sort_Insertion`). audit64: pointer-width warnings
  1,580 -> 1,465, disagreeing declarations 158 -> 93 functions.
- Low memory unchanged: 26,000-frame boot loop with 3 attract demos, new game to level 51
  (player stops at the water at (-1266.6, -779.5, 260.0) as before). asm_diff 0 over every
  touched file (152 src files).
- Merge notes: many one-line declaration retypes (s32 -> sptr) spread over ~150 src files; a
  conflict with another part is a declaration line: keep both changes. func_002034F4/00205A2C
  (osSendMesg declaration) also carry u1's hash-stride fix on other lines.

## Round 6, part u4: macOS runs the merged tree, with sound and pixels (branch names-R6_u4)

Details in [MACOS.md](MACOS.md) section 8. On the Mac (arm64, full 64-bit pointers, headless, no
window of any kind):
- **The boot to frame 26,000** (cinemas, title, all three attract demos, the loop) and **the new game
  into level 51** run with every mode switch at the same frame as Linux and the same player states.
- **`--audio --wav`**: peak 21,304, RMS 1,996.4 over 6,000 frames, the same as Linux.
- **Pixels without a window**: `--offscreen-gl` (port/app/offscreen_gl.cpp) runs LUS's OpenGL
  renderer on a CGL context with no drawable, and T2_SHOT reads the game framebuffer. Mac against
  Linux screenshots: all 29 new-game frames match (at most 0.37% of the pixels off), and 15 of 17
  boot frames match.
- dlcheck (game.cpp): display lists with truncated host addresses are reported and not drawn. None
  is left on the boot or new-game path.
- Fixes, all asm_diff 0 against 6d04fabb: lit-vertex callbacks, GameObject_GetRegionInfo,
  Sort_Insertion, three audio truncations (ALLowPass, the A_SAVEBUFF output handle, the ALDMAproc),
  the font/glyph G_SETTIMG images, and **Cinema_DrawFade's camera viewport at the N64 offset**.
  The last one is a bug on all hosts: the cinema fades now draw on Linux too.
- `-ffp-contract=off` for the game (game.cmake).
- **Lead for u1**: in attract demo 2 the Mac draws the level's static objects and Linux draws none.
  On Linux the camera's `m_ViewParams.m_VisBits` is 0 (CCamera__UpdateVisBits), so
  CScene__DrawGeometry rejects every CStaticObject.

Files others may touch: port/app/game.cpp (WriteShot, the draw call, dlcheck, T2_DLDUMP_MAX),
port/app/main.cpp (`--offscreen-gl`), port/app/CMakeLists.txt, port/cmake/game.cmake,
port/audio/t2audio_mgr.c, port/runtime/t2ultra.c (T2_V2P_TRAP), and src func_00213A70,
func_00214F14, func_00220208, func_00268A2C, func_00227184, func_00295304, func_002971BC,
func_00278188, func_0027836C, func_0027854C, func_0027B0FC, func_00279158, func_0027A2C4,
func_0027A4DC, func_0040F240, func_00448240.

## Round 6, part u2: animated actors (branch names-R6_u2)

[RENDER.md](RENDER.md) section 5. The actor draw and animation chain (`CGameObjectInstance__Draw`
-> `BuildBoneMatrices` -> `PoseBone` -> `CAnimationState__*`, `AdvanceAnimation`,
`CScene__AdvanceInstances`, `CAI__UpdateModeAnim`, the shadow and draw callbacks) runs on host
types. Linux, headless, this branch alone:

- mode 4 (frame 1362-1846): **the Acclaim logo renders** and animates (letters flip in, settle
  into "Acclaim entertainment, inc."); bone matrices = `tools/assets/anim.py` pose (2 units of
  s15.16).
- the Iguana cinema as before; the title; demo 1 runs to its end and the boot loops (8,800
  frames, no crash).
- new game (`--input START@4400,A@4500,A@4700,A@4800`): the mode-11/12 cinemas draw their
  characters (Adon, Turok, enemies, the caged prisoner) and the run reaches level 51 (frame
  12899) and runs to 14,500 without a crash (before: a stack overrun in `CScene__GetLightsAt`
  crashed the cinemas once actors drew).
- with u1's branch merged in a scratch branch (not committed): level 51 draws the level **and the
  Talon in Turok's hand** (frames 13001-13601).

Merge notes: `include/structs/model.h` (`CDrawArgs.m_hModel` sptr), new
`include/structs/pose_host.h` and `port/game/t2actors.c`. Files others may touch:
func_0021A150 (u3 has the same m_nAnims fix uncommitted: take one), func_00243170
(`CAI__UpdateModeAnim`: changes enemy animation timing), func_00218B10 (`GetRootMotion` flags),
func_00252ADC/func_00254B04 (player pool), func_00214F14 (t3's static lit vertices: prototype
pointer-wide). Anim events/hot points are **off** (`T2_ANIM_EVENTS=1` to enable) until
`CParticleSystem__Spawn` (func_0022D2FC) is ported.

## Round 6, part u3: player input, the water, the weapon wheel (branch names-R6_u3)

Details and the fix table u3-1..5: [GAMEPLAY.md](GAMEPLAY.md) section 5, [INPUT.md](INPUT.md)
section 6. `--input` now takes held ranges (`CU@13000..13440`), the stick (`SX=80@13450..13474`,
`SY=...`) and `@FILE` scripts. The player turns (the region-info word bug, u3-1), the water was
a corner, not a merge regression: steered, he swims across the level-51 basin to x 358. The
weapon wheel opens with A/B and the stick picks the weapon (u3-2); the weapon actor's AI runs
(u3-3); switching and firing wait for animations: `m_nAnims` is read as the wrong half of a
swapped word (fix known, not committed: it exposes a crash in `CParticleSystem__Spawn`'s N64
views at frame 1362; GAMEPLAY.md section 5). Files the merge may meet: `port/app/game.cpp`,
`port/app/main.cpp` (usage line), `port/game/t2debug.c` (state log: weapon/wheel fields; u1
also edits it: auto-merged in my scratch test), func_0024EF70, func_0024BAD4, func_00252ADC,
func_00254B04, func_00250BF0, func_0021A658, func_0021ABC8 (not touched by u1 or u2's
branches as of their tips; u2 changes func_0021A344/0021A500/0021A7EC/00218B10 nearby).
Merged with u1's branch (scratch, not committed) the gameplay frames show the level.

# Round 5, merged state (history)

Round 5 merged five branches, in this order: t2 (the ABI checker and the ABI sweep), t5 (macOS to
the title: the virtual pad on `--no-window`, pointer-wide prototypes and parameters), t3 (the 3D
scene: the cinemas draw), t4 (audio part 3: sfx, music, speech) and t1 (gameplay: the attract
demos and a new game into level 51). Then it fixed what only the merged tree reaches. This page
says how to build and run the result, how far `turok2 --game` gets, what stopped it on the way and
what comes next. The part sections further down keep each branch's own notes (t2, t5, t3, t1 here;
t4 at the end; detail pages [ABI.md](ABI.md), [MACOS.md](MACOS.md) section 7,
[RENDER.md](RENDER.md), [AUDIO.md](AUDIO.md) section 10, [GAMEPLAY.md](GAMEPLAY.md)); the round 4
and round 3 states follow as history.

The matching build is unchanged. `tools/asm_diff.py --base 98574f3c -j 8` reports 0 differences
over all 3,938 files after every merge and fix. A clean `make clean && make setup && make build &&
make verify` gives the identical ROM (CRCs good, 123 shared .rodata objects agree).

## Round 5: build and run (Linux x86_64, the box)

```sh
# 0. the matching build first: gendata and asm_diff read its ELF; keep a copy as build/ref.elf
#    (the asm_diff/gendata tools expect it, and `make clean` deletes it)
make setup && make -j8 build && make verify && cp build/turok2.us.elf build/ref.elf

# 1. the archive, format 2.0. REBUILD IT this round: tools/assets/swap_spec.json (collision
#    table 3, the region grid) and include/structs/region.h (CRegionInfo typed) changed the plans.
python tools/assets/mkarchive.py baserom.us.z64 -o OUT/turok2.o2r -j 4     # ~12 s
python tools/assets/archive_check.py OUT/turok2.o2r baserom.us.z64          # OK, format 2.0

# 2. configure; the game library AND the runtime/audio libraries (gendata reads all three)
cmake -S port -B OUT/build -G Ninja -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ \
      -DCMAKE_BUILD_TYPE=Debug
ninja -C OUT/build -j8 turok2_game t2runtime t2audio

# 3. the game data as C into port/gen/ (gitignored). Drop the cache after ANY header change.
rm -f OUT/decls.json
python3 tools/port/gendata.py --build OUT/build --cache OUT/decls.json -j 4      # ~1 min

# 4. re-run cmake so data.cmake sees port/gen/*.c, then the executable
cmake OUT/build && ninja -C OUT/build -j8 turok2

# 5. run headless (SDL's offscreen driver: no window). --pak-blank: a blank formatted
#    Controller Pak in memory (--pak-dir DIR: turok2_pak<N>.mpk files; --no-pak: none).
cd OUT && SDL_VIDEODRIVER=offscreen T2_SHOT=shot.ppm T2_SHOT_EVERY=300 \
    build/app/turok2 --archive turok2.o2r --rom ../baserom.us.z64 \
    --game --draw --pak-blank --frames 26000
#    a new game: --input START@4400,A@4500,A@4700,A@4800 (title -> main menu -> single player
#    -> New game -> the pak message OK), then walking: CU@13000,CU@13004,... (--input presses a
#    button for one frame; repeat it every 4 frames to hold it). Buttons: A B Z START DU DD DL
#    DR L R CU CD CL CR; no stick yet.
#    --no-window (macOS: always): a virtual pad on port 1 (its pak per --pak-*, the buttons from
#    --input); with an offscreen window LUS's ControlDeck serves the same scripted buttons.
#    --audio / --wav out.wav: sound (sfx, music, speech). T2_STATE_LOG=60: the level, the
#    player (position, state, region, floor) every 60 frames. Debug: --skip-pak-check,
#    --start-mode N, T2_MENU_LOG=1, T2_DLDUMP_AT=<frame> (+ T2_DLDUMP_VTX=1),
#    T2_TEST_SFX/T2_TEST_SONG/T2_TEST_SPEECH; gdb -batch -ex run -ex bt --args ... for a trace.
```

gendata on this tree: 1,537 data symbols the C needs (1,531 typed from a C declaration), 93
pointer targets typed as tables, 386 zero .bss symbols, 4,041 pointer slots (2,932 relocated,
1,077 null, 32 unresolved). The archive: 26,124 units, sha1
`ae9b88e69adc541a2cd9accee142da0d3ea8207c` (the plans changed, so `turok2` without `--game` now
reports fnv1a64 `ac3d498348be24c2` on all 3 backends).

## Round 5: how far `turok2 --game` gets (Linux, headless)

The boot, the cinemas, the title and **all three attract demos** run on their own, and the boot
loops as on the console:

```
--game --draw --pak-blank --frames 26000
frame 2:     mode 0    pak check, the virtual pak passes
frame 298:   mode 1    legal screen
frame 1070:  mode 3    "EXPANSION PAK FOUND!" with the 3D pak model
frame 1362:  mode 4    cinema 100 (level 162, Acclaim logo): runs, black (an animated actor)
frame 1852:  mode 5    cinema 90 (level 44, Iguana jungle): DRAWS the scene
frame 4214:  mode 7    TITLE SCREEN
frame 5469:  mode 13   attract demo 1 (level 81), plays to its end
frame 8802:  mode 1 -> 4 (9577) -> 5 (10067) -> 7 (12429): the boot again
frame 13684: mode 13   attract demo 2 (level 131)
frame 16073: the boot again, title at 19697
frame 20952: mode 13   attract demo 3 (level 109): loads and plays (round 4/5 crash fixed, C20-3)
frame 22160: the boot again, title at 25786; no crash to frame 26000 (158 s)
```

A new game:

```
--game --draw --pak-blank --input START@4400,A@4500,A@4700,A@4800,CU@13000,...  T2_STATE_LOG=60
frame 4455:  mode 8    main menu
frame 4555:  mode 13   single-player menu (New game / Load / Go back)
frame 4855:  mode 11   new-game intro cinemas (levels 50, 49): they DRAW (Port of Adia, the boat)
frame 8977:  mode 12   (levels 47, 45, 46, 48, 49), then the level-51 warp-portal cinema: draws
frame 12903: mode 13   level 51, GAMEPLAY. The player stands at (-1540.7, -566.4, -1481.6);
                       with C-up held he walks (state 3) down the slope to y -735.8, into the
                       water (state 9) at (-1266.6, -779.5, 260.0) by frame 13600, where he stays
                       (C-left, C-up + Z: no further movement; state 8 once the input stops).
                       No crash to frame 15000 (82 s).
```

Screenshots (`T2_SHOT_EVERY`, 640x480, checked by eye): frame 2401 the Iguana cinema (the IGUANA
letters on the jungle floor, tree trunks, ferns, letterbox); frame 4500 the title as in round 4;
the new-game cinemas (frame 9302: the Port of Adia, buildings, the boat, fog; frame 12902: the
level-51 warp portal between the walls); demo 1 frames are a dark grey fill, with the weapon
wheel's 3D weapon models drawn at frame 7202; demo 2 frames show a full-screen texture close-up;
**gameplay frames (level 51) draw the sky, the fog colour and the HUD text** ("NEW CHEAT
ENABLED") but no level geometry: the gameplay display list has no `G_VTX` and no `G_MTX`
(`T2_DLDUMP_AT=13001`), while `CScene__DrawGeometry` and `CStaticObject__Draw` run every frame
(section 4).

With `--audio --wav` (6,000 frames: the boot, the title and into demo 1): no crash, 100 s of
sound, peak 21,304, RMS 1,996 (silent for the pak/legal seconds, then 750-3,700 RMS per 10 s
window through the cinemas and the title).

The port tests pass on this tree: `rnc_test` (14,426 streams, 0 failed), `t2asset_test` (26,124
units, 0 mismatches; ROM backend + the archive's plans identical), `t2asset_typed_test` (1,585
checks x3, 0 errors), `gbi_test` (0 mismatches), `t2os_test` (trace hash `2b2775877e0d226b`,
deterministic), `t2audio_test --direct 0` (19 frames, peak 32,433), `t2audio_test --adec
0xC29AB4 7464` (27,136 samples), the swoosh self test (`turok2 --game`: OK, 0 failed).

ABI checker (`tools/port/abicheck.py --build OUT/build -j 4`, 3,065 TUs, 10,762 calls checked):
harmful sites 193 (102 callees): ptr_from_int 185, missing_args 6, void_used 2; struct_class,
struct_scalar, float_int 0 (t2 alone: 510 harmful, 498 of them ptr_from_int: t5's pointer-wide
prototypes took that class from 498 to 185). `ptrdecls.py` and `ptrargs.py` rerun on the merged
tree report nothing left in `src/` (the gitignored, stale `include/functions.h` still lists
names; it is not compiled).

macOS: t5 ran its own tree to the title; the merged tree was not rerun on the Mac this round.

## Round 5: merge conflicts and crash/fix table (round 5 merge)

Conflicts and how they were resolved (asm_diff 0 after each merge):

- t2 x t5 (38 files): t2's `TARGET_PORT` host copies against t5's s32 -> sptr retypes of the same
  headers: kept t2's bodies and applied t5's parameter retypes to both branches of each `#ifdef`
  (a script over the diff3 hunks), then `ptrdecls.py` (2 returns) and `ptrargs.py` (1 parameter).
- `Actors_GetLightVolumeFalloff` was fixed three times (t2: adapters `_f3`/`_v` that pass the floats
  as words; t3: a float definition over a union; t1: a typed host body). Kept t1's body and the
  float call sites, dropped t2's two adapters and their redirects.
- `gWarmBoot = 1` (t3 and t4): one assignment in `t2boot.c`, both reasons in its comment.
- The audio heap/bus files (func_002912F0, 00294E70, 00295304, 002953B4, 00295410, 0029546C):
  pointer-wide in both t5 (sptr) and t4 (`TARGET_PORT` typedefs): kept t4's, with t5's
  pointer-wide `alHeapDBAlloc` file and `alFilterNew` handler parameters.
- Grid section record sizes (t3 `SIZEOF_*`, t1 `GRID_*_SIZE`): t1's names. The CDLightSet functions
  (t2 pass-through copies, t1 typed bodies): t1's. `CGameRegion__GetNormalRaw` (t2: `_byval` rename
  + wrapper; t1: a host body taking the result pointer) and `Automap_SetPlayer` (t2: adapter `_f`
  from f32 to words; t1: a host body with the caller's types and `m_pHostRegion`): t1's, t2's
  adapters and redirects dropped. t2's host copy of `CScene__LoadLevel` now passes
  `&w->m_DLightSet` (t1's member) where it had `w->unk_18EBC`.
- `ptrdecls.py` crashed on a negative brace depth (`#ifdef`-split function heads): clamped.

Paths only the merged tree reached (all `TARGET_PORT`, asm_diff 0):

| # | where | cause | fix |
|---|---|---|---|
| C20-1 | `Cinema_Update` (func_00283CD4): **no cinema ended** (mode 4 for ever) | its `GameState` view reads a second word at N64 offset 0x1450 from `m_PlayerList.m_bCinema` (= `gEngineApp_m_Settings.match.unk44`); after t1's `CAutomap.m_pHostRegion` that host offset landed on a non-zero word | by member: `gEngineApp_m_Settings.match.unk44` |
| C20-2 | abort `T2Port_ReadAsset(0x01CB021F)` 120 frames into level 51 (`CLevelActor__LoadGeometry`) | with the scene drawing, the level actors' geometry is requested; the key was read at N64 offset 0x34 (`m_Resource`) of a host-layout `CLevelActor` | `CLevelActor.m_Resource` (LoadGeometry and TouchGeometry) |
| C20-3 | demo 3 (level 109) SIGSEGV in `CCache__RequestDecompression` <- `CStaticObject__RequestLitVertices` <- `CScene__StreamAllSections` (t1's "host pointer as cache key") | `CScene__StreamAllSections` walked the grid section records with the N64 stride and wrote the section handle at N64 offset 0x24: the static objects' `m_pOwner` stayed garbage | `ObjA`/`ObjB` as host twins of `CStaticObject`/`CLevelActor` |

The class to look for next (C20-1 is one): views that read engine globals through an alias symbol
plus an N64 offset (`extern <LocalView> gEngineApp_m_Game_...`). Without a `TARGET_PORT` twin:
`CPlayerPool__UpdateMatchClock` (func_0025492C, `Globals` off `m_MenuPool`: the multiplayer match
clock), `Action_CheckRequiredItem` (func_00265F90, the camera pool), func_00271694
(`MenuMatchGlobals`), `EngineMode03_ExpansionPak_Draw` (func_00285CF0), func_00459374.

## Round 5: next

1. (done in round 6 u1, see its section) **Gameplay frames draw no level geometry**: the sky, the fog fill and the HUD text are there, the
   gameplay display list has no `G_VTX`/`G_MTX`, `CStaticObject__Draw` runs ~3 times a frame (the
   level-51 cinema, same level, draws everything). The player camera's draw setup (view/projection
   matrices, the viewer box `CScene__LoadVisibleGridSections` culls with) is the place to start;
   "NEW CHEAT ENABLED" at level start suggests the cheat flags read garbage too.
2. **Animated actors** (the Acclaim logo, every enemy, pickup and weapon): `CGameObjectInstance__Draw`
   -> `BuildBoneMatrices` (func_0021CDAC) -> `PoseBone` (func_0021B95C) -> `CAnimationState__*`
   (func_00218B10) on N64 views; t3's draft `/root/R5_t3/actor_draw_0021D39C.patch` covers the
   first step only (RENDER.md section 3).
3. **The player stops in the water** at (-1266.6, -779.5, 260.0) in state 9 (t1's run crossed the
   water to x 772): collision against the now-loaded static objects, or a state-9 view at N64
   offsets.
4. **Rotation input**: `--input` has no stick (an `SX`/`SY` entry), so scripts cannot turn the
   player; the d-pad does not turn in Turok 2.
5. **Weapon wheel**: pressing A in gameplay crashed in `CBarrel__OpenWheel` before t1-18; untested
   on the merged tree (the wheel's models draw in demo 1).
6. **macOS**: rerun the merged tree on the Mac (t5 stopped in the second demo's
   `CGameRegion__ContainsXZ`, which t1 fixed); audio above 4 GB (`--high-memory`): t4/t5's heap
   fixes are merged, untested together; pixel screenshots there need a Metal offscreen target.
7. **ABI**: abicheck's 185 `ptr_from_int` sites (the macOS truncations), 6 missing_args, 2
   void_used; function-pointer calls (184 sites, the handler tables) are not checked yet.

## Round 5, part t2: the ABI checker and sweep

[ABI.md](ABI.md). `tools/port/abicheck.py --build OUT/build -j 4` parses the 3,099 linked C files
with libclang (about 15 s) and compares every direct call (10,724) with the callee's definition
under the x86-64 SysV and AArch64 rules: struct classes, struct vs scalars and hidden result
pointers, float vs int, f32 vs f64, pointer width, void results used, MIPS pass-through
(missing) arguments, variadic mismatches. Harmful sites 779 (294 callees) -> 510 (214), of which
498 are the 64-bit width class (`ptr_from_int`, the macOS truncations): struct_class 26 -> 0,
struct_scalar 62 -> 0, float_int 43 -> 0, void_used 85 -> 4, missing_args 67 -> 7. The fixes are
host-only (adapters in `port/game/t2abi.c` reached by a `#define` in the caller, by-value
definitions wrapped for hidden-pointer callers, `Triple` as three floats in the impact handlers,
host copies that return the last call's value or pass the pass-through registers); asm_diff 0
over all 3,938 files.

With these fixes the boot runs the same sequence to the title and then **into the attract demo
without the `EnemyAI_Update` crash**: `--game --draw --pak-blank --frames 7000` reaches mode 13
at frame 5469 and runs to frame 7000 (the gdb run exits normally); the frames are black (the 3D
scene draw, t3). The checker does not see function-pointer calls (184 sites: the handler
tables); that is the next thing to add (ABI.md section 4).

## Round 5, part t5: macOS runs the merged tree to the title screen (branch names-R5_t5)

The Mac (arm64, full 64-bit pointers) now boots the merged tree on its own, headless, with the
pak flags, through the **same mode sequence at the same frames as Linux**: 0 (2), 1 (298), 3
(1070), 4 (1362), 5 (1852), **7 title (4214)**, 13 attract demo (5469). Display lists match Linux
at frames 601, 1201, 1501, 2401, 4500 and 6000 (same command count, op and w0; w1 differs only in
host addresses). Details, the commands and the fix table: [MACOS.md](MACOS.md) section 7.

What changed (all `src/` changes are s32 -> sptr / u32 -> uptr, the same types on N64: asm_diff
0 over 3,938 files at every commit):

- `port/app/game.cpp`: `--no-window` runs get a virtual pad on port 1 (pak per `--pak-*`,
  buttons from `--input`). Without it the Mac (no headless GL/Metal, so always `--no-window`)
  saw no controller and stayed in mode 0. **Linux `--no-window` runs change the same way.**
- `tools/port/ptrdecls.py`: prototypes that declare s32/u32 where the definition takes or returns a
  pointer (385 parameters + 27 returns). `tools/port/ptrargs.py`: definitions whose s32 parameter
  receives a pointer, from the port build's `-Wint-conversion` notes (331 parameters of 127
  functions, with all their declarations). Both re-runnable; they report 0 left on this branch.
- AI mode/event callbacks (`Action_*`, `Door_*`, `Enemy*`, `PlayerDeath*`, `Turret_*`, `Weapon*`,
  the bosses...): the actor/AI/event parameters as sptr (151 parameters in 120 files).
- Hand fixes on the path: pak note names, cache handles read as `s32 *` (`CISet__GetBlock(*res)`,
  13 files), `CPlayer__GetPickupCountLow/High` host bodies, event-record tables, the audio heap
  and filters (`alHeapInit`, `alHeapDBAlloc`, bus/resampler constructors, `alFxParam`).

**Side effect on Linux:** with this branch the Linux build (rebuilt from it on the box,
`/root/R5_t5mac`) no longer stops at the first attract demo either: windowed and `--no-window`
both run demo 1 to its end (mode 13, frames 5469-8794), loop the boot (legal 8802, cinemas,
title 12429) and stop in the **second** attract demo's level load, the same place as the Mac:
`CGameRegion__ContainsXZ(tri = garbage) <- CScene__FindRegionAt <- CScene__DecompressAnimatedInstances`.
`EnemyAI_Update` still reads the enemy AI at N64 offsets (t1's item); its garbage just does not
crash any more.

Merge notes: 351+ files in `src/` change, almost all on declaration/parameter lines
(`include/rw/abu_protos.h` too). Conflicts are likely with any branch that edits the same
function headers (t1's enemy AI files, t2's ABI sweep: rerun `ptrdecls.py`/`ptrargs.py` after
merging instead of resolving by hand), t4's audio files (`func_0029*.c`).

## Round 5, part t3: cinemas and the 3D scene ([RENDER.md](RENDER.md))

The second intro cinema (mode 5, level 44) now **draws**: the Iguana logo in the jungle, the
camera moving through trees, ferns and the vine, letterboxed (frames 2401-3901). The boot still
reaches the title (frame 4214) and the attract demo (frame 5465, mode 13, level 81), where it stops
in `EnemyAI_Update` as before (t1's). 11 fixes, RENDER.md section 1: the region attributes typed
(far clip / FOV were big-endian: **rebuild the archive**), the grid sections loaded (camera view at
N64 offsets), `CStaticObject` on the host layout in 9 files, `gWarmBoot = 1` (the settings defaults:
the difficulty mask was 0), a float/word ABI mismatch in the light volumes, the main camera's frame
clear over the scene (`m_MainCamera - 0x40`), `sLitVertexOffsets` sized 0x9000 (it overran into
`gAudioWorld`/`gLoopSounds`). The first cinema (Acclaim logo, level 162) stays black: its logo is an
animated instance and the actor path (`CGameObjectInstance__Draw` -> `BuildBoneMatrices` ->
`PoseBone` -> `CAnimationState`) is still on N64 views (RENDER.md section 3; a host
`CGameObjectInstance__Draw` is ready as a patch). New debug aid: `T2_DLDUMP_VTX=1`.

## Round 5, part t1: gameplay runner (branch names-R5_t1)

Details, fix table t1-1..t1-20 and the debug aids: [GAMEPLAY.md](GAMEPLAY.md).

**Rebuild the archive**: `tools/assets/swap_spec.json` changed (collision table 3, the region
grid, is swapped now: t1-3). An archive from round 4 still loads but the grid counts are
byte-reversed (`CScene__FindRegionAt` walks garbage).

```
--game --draw --pak-blank --frames 26000                      (no input: the attract demos)
frame 5469:  mode 13, level 81   demo 1 plays to its end (8794), the player moves
frame 13684: mode 13, level 131  demo 2 plays, the player moves
frame 20948: level 109 (demo 3) loading: abort in T2Port_ReadAsset(0x40291210), a host pointer
             as a cache key <- CStaticObject__LoadGeometry <- CScene__StreamAllSections

--game --draw --pak-blank --input START@4400,A@4500,A@4700,A@4800 (+ CU@13300,CU@13304,...)
frame 4455:  mode 8 main menu; 4555 mode 13 (single-player menu D_0043916C, New game)
frame 4855:  mode 11 new-game intro cinemas (levels 50, 49); 8977 mode 12 (47, 45, 46, 48, 49)
frame 12903: mode 13, level 51: GAMEPLAY. The player stands on the floor (y -566.4) and walks
             with C-up (state 3), down to y -1228.8, into the water (states 11/10), across a
             dozen regions; C-left strafes; Z presses run. No crash to frame 15000.
```

Every gameplay frame is black (no vertices in the display list; the scene draw is t3's), so the
progress is shown by `T2_STATE_LOG` (player position, state, region, floor height). Four ABI
mismatches fixed locally (t1-6 CVector3 by value vs three u32, t1-13 s32/Triple vs f32/CVector3,
t1-16 struct return vs hidden result pointer, t1-20 three s32 vs CVector3): t2's checker should
find their siblings. Files the merge may meet: `include/structs/{particle,world,game,weapon}.h`
(CDLightSet, `CScene.m_DLightSet`, `CAutomap.m_pHostRegion`, `gWeaponInfoTable[35]`),
`port/game/t2debug.c`, `port/app/game.cpp` (the state log), `func_0024C47C.c` (CPlayer__Update
host views), `func_00220530.c` (CScene__Update), `tools/assets/swap_spec.json`.

# Round 4, merged state (history)

Round 4 merged five branches, in this order: s5 (the swoosh/effect system typed, the N64-offset
sweep), s1 (the virtual PIF and Controller Pak), s2 (the boot runner to the title screen), s3
(macOS parity to mode 0) and s4 (audio part 2). Then it fixed what only the merged tree reaches
and followed the attract demo a few steps. This page says how to build and run the result, how
far `turok2 --game` gets, what stopped it on the way and what comes next. The part sections
further down keep each branch's own notes and crash tables (s2-1..16 here, s3-1..19 in
[MACOS.md](MACOS.md) section 6, s4-1..10 below); the round 3 state follows as history. Details:
[INPUT.md](INPUT.md) (PIF and pak), [AUDIO.md](AUDIO.md), [MACOS.md](MACOS.md),
[DATA.md](DATA.md), [64BIT.md](64BIT.md), [RUNTIME.md](RUNTIME.md).

The matching build is unchanged. `tools/asm_diff.py --base 474e40af -j 8` reports 0 differences
over all 3,938 files after every merge and fix. A clean `make clean && make setup && make build &&
make verify` gives the identical ROM (CRCs good, 123 shared .rodata objects agree).

## Round 4: build and run (Linux x86_64, the box)

```sh
# 0. the matching build first: gendata and asm_diff read its ELF; keep a copy as build/ref.elf
#    (the asm_diff/gendata tools expect it, and `make clean` deletes it)
make setup && make -j8 build && make verify && cp build/turok2.us.elf build/ref.elf

# 1. the archive, format 2.0 (t2/swap byte-swap plans); the port refuses 1.x archives.
#    Round 4 changed no asset tool: an archive from round 3 still checks OK.
python tools/assets/mkarchive.py baserom.us.z64 -o OUT/turok2.o2r -j 4
python tools/assets/archive_check.py OUT/turok2.o2r baserom.us.z64     # OK, format 2.0

# 2. configure; the game library AND the runtime/audio libraries (gendata reads all three)
cmake -S port -B OUT/build -G Ninja -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ \
      -DCMAKE_BUILD_TYPE=Debug
ninja -C OUT/build -j8 turok2_game t2runtime t2audio

# 3. the game data as C into port/gen/ (gitignored). Drop the cache after ANY header change.
rm -f OUT/decls.json
python3 tools/port/gendata.py --build OUT/build --cache OUT/decls.json -j 4

# 4. re-run cmake so data.cmake sees port/gen/*.c, then the executable
cmake OUT/build && ninja -C OUT/build -j8 turok2

# 5. run headless (SDL's offscreen driver: no window). --pak-blank: a blank formatted
#    Controller Pak in memory (--pak-dir DIR: turok2_pak<N>.mpk files; --no-pak: none).
cd OUT && SDL_VIDEODRIVER=offscreen T2_SHOT=shot.ppm T2_SHOT_EVERY=300 \
    build/app/turok2 --archive turok2.o2r --rom ../baserom.us.z64 \
    --game --draw --pak-blank --frames 5400
#    --audio / --wav out.wav: the sound world (section 2); gdb -batch -ex run -ex bt --args ...
#    for a backtrace. Debug only: --skip-pak-check, --start-mode N (7: title at frame 3),
#    --input START@300,A@400, T2_MENU_LOG=1, T2_DLDUMP_AT=<frame>, T2_TEST_SFX/T2_TEST_SONG.
```

A fresh `turok2.cfg.json` is fine now: `main.cpp` writes a 640x480 windowed default into a config
that lacks the window keys, and headless runs always start windowed (SDL's offscreen driver
reports its window as fullscreen, so LUS saved `Window.Fullscreen.Enabled = true` at exit and
the next run opened a fullscreen window at the driver's size with the game in its lower-left
640x480).

gendata on this tree: 1,543 data symbols the C needs (1,535 typed from a C declaration), 279
pointer targets, 391 zero .bss symbols, 3,881 pointer slots (2,852 relocated, 997 null, 32
unresolved).

## Round 4: how far `turok2 --game` got

With the virtual pak and no debug switch, the N64 boot runs on its own to the title screen and
into the attract demo:

```
--game --draw --pak-blank --frames 9000          (Linux, headless)
game: frame 2:    engine mode 17 -> 0     pak check: "ACCESSING THE CONTROLLER PAK", the pak passes
game: frame 298:  engine mode 17 -> 1     legal screen, the full text
game: frame 1070: engine mode 17 -> 3     "EXPANSION PAK FOUND!" with the 3D pak model
game: frame 1362: engine mode 17 -> 4     intro cinema 100 (level 162): runs, draws black
game: frame 1852: engine mode 17 -> 5     intro cinema 90 (level 44): runs to its end, draws black
game: frame 4214: engine mode 17 -> 7     TITLE SCREEN
game: frame 5465: engine mode 7 -> 17     the title's timeout: attract demo, mode 13, level 81
SIGSEGV in EnemyAI_Update (func_0025C5F8.c) <- CAI__Update <- func_0021F1B4 <- CScene__LoadLevel(81)
game: PIF 812 SI DMAs, 1 status, 236 pak reads, 169 pak writes   (the first 300 frames)
```

Screenshots (`T2_SHOT_EVERY=300`, 640x480, checked by eye): frame 601 the legal text (beige game
font, centred, every line); frame 1201 the Expansion Pak screen (the rotating pak model, red top,
grille, "EXPANSION PAK FOUND!"); frames 1501 and 2401 (the cinemas) are uniformly black; frame
4500 the title (the TUROK logo with the eye in the O, "SEEDS OF EVIL", the red "2" splash on the
scale background).

With `--audio --wav`: the same sequence up to mode 5. The first cinema's music (`SndSeq_Update`
-> `__CSPVoiceHandler`) stops in `__CSPSetUsptFromTempo` (func_00290D5C) at the start of the
second cinema: s4's open music item (the compressed sequence player is still m2c views). The
mixer runs every retrace before that.

The port tests pass on this tree: `rnc_test` (14,426 streams, 0 failed), `t2asset_test`
(26,124 units, 0 mismatches), `t2asset_typed_test` (1,585 checks x3, 0 errors), `gbi_test`
(0 mismatches), `t2os_test` (trace hash `2b2775877e0d226b`), `t2audio_test --direct 0` (19
frames, peak 32,433), the swoosh self test (`turok2 --game`: OK, 0 failed), `turok2` without
`--game` (26,124 units on 3 backends, fnv1a64 `e9b9d3e7c226d5aa`).

macOS: s3 ran mode 0 headless with display lists identical to Linux (s3 alone; the merged tree
was not rerun on the Mac in this round). The merge fixes below are host-layout fixes that apply
there too.

## Round 4: crash and fix table (merge)

Paths no single branch reached. Every fix is under `TARGET_PORT` or port-only, asm_diff 0.

| # | where | cause | fix |
|---|---|---|---|
| M1 | `WeaponActor_SwitchToRequestedInline` (mode 3 -> 4, `CPlayer__SetupActors`) | the weapon actor's owner read at +0x1A8 (N64 `m_AI.m_pParent`) and the player's requested weapon at +0xB14 | `m_AI.m_pParent` / `m_RequestedWeapon` in 10 weapon files; `WeaponActor_InitAI` host body (CAI members, the actor pointer-wide) |
| M2 | `CGameRegion__GetHeight(region = a stack address)` from `CCamera__SetViewParams` | **ABI**: the definition takes a `Shared_Quad` (union of s32/f32: integer registers on x86-64), the caller a `CQuaternion` (floats: SSE registers), so every later argument arrived in the wrong register; the GameplayCamera view at N64 offsets | host body on `CCamera.m_ViewParams` (`CCameraViewParams`) |
| M3 | `CScene__GetRegionInfo(0xffffffff402bfb18)` | `ConstructFromRecord` passed the region as `s32` to a pointer parameter (upper half garbage) | s3's 0d572c47 (merged) |
| M4 | `CList__Construct(0xe58)` from `CCameraPool__ResetEmitterPools` | camera table/count at N64 offsets 4/8, stride 0xEC0 | host body on `CCameraPool`/`CCamera.m_SkySystem` |
| M5 | `CMtxF__GetTranslation(0x14c)` from `WeaponActor_BuildMatrix` | the weapon model view and the camera's 0x12C/0x14C | the actor's members and `m_mfViewOrient`/`m_qViewAlign` |
| M6 | `CGameRegion__GetNormal(0x8006ff5df)` from `CalculateOrientationMatrix` | **ABI**: `GameObject_GetFloorTilt` returns a `CQuaternion` by value (MIPS: a hidden result pointer, which the caller's prototype passed; x86-64: registers); the LevelActor view has the N64 header size | host body on `CGameObjectInstance`, the quaternion by value |
| M7 | `CCache__Unlock(NULL)` from `CScene__ReleaseSectors` | the scene's sector table at N64 offsets | host body on `CScene` (`m_nSectors`, `m_Sectors[].m_hData`, counters) |
| M8 | `CAI__SetMode` calls garbage (attract demo, enemies) | the base mode table `D_800B0700`, which every AI table chains to through its header row's integer `m_Mode`, was used by no C, so gendata copied it as N64 bytes | declared `CAIModeInfo D_800B0700[27]` (globals/ai.h) and named from the host `CAI__SetMode`: gendata lays it out (3,798 -> 3,881 pointer slots) |
| M9 | `GameObject_TraceMove(outPos NULL, outRegion 7)` from impact event 7 (`Door_CloseEnter`) | **ABI**: the Rage Wars `Triple` (three s32) by value vs the callers' `CVector3` | `func_00237FAC` takes a `CVector3`, `func_00238858` the actors pointer-wide |
| M10 | `CScene__LoadLevel` | the s5/s2 conflict resolution kept s5's whole file and lost s2's `sizeof` stride of the second actor loop | restored |
| open | `EnemyAI_Update` (func_0025C5F8.c:110) | the enemy AI read through an `AI`/`Object` view at N64 offsets (`modeIndex` at 0x133, `age` 0xCB): the behaviour index is garbage | next |

Three of these (M2, M6, M9) are one class of bug the offset sweep does not see: a struct passed
or returned **by value** whose declarations disagree between an integer-class struct (`Triple`,
`Shared_Quad`) and a float struct (`CVector3`, `CQuaternion`), or a by-value return
that a Rage Wars prototype turned into a hidden pointer. MIPS passes all of them the same way,
x86-64 and arm64 do not. 31 files use `Triple`; the impact-event handlers
(`ImpactEvent_*`, func_0023870C..func_00238FD4) take one and are called with a `CVector3`.

## Round 4: next (as it stood)

1. **The attract demo / levels run**: `EnemyAI_Update` and the rest of the enemy AI on `CAI`
   (s5's counter: func_002568CC 31 sites, func_0043EC68 19, func_00258964 12, ...).
2. **By-value struct ABI audit**: a checker (libclang over all declarations of each function:
   parameter/return classes on x86-64 and arm64) and then the `Triple` handlers; cheaper than
   finding them one crash at a time.
3. **Black cinemas**: done for static geometry in round 5 (t3, RENDER.md); animated instances
   (the Acclaim logo, every actor) need the bone/animation path on the host layout.
4. **Audio**: a sound effect without gdb crashes in `t2asp.c` `cmd_adpcm` (the ADPCM state
   address, `osVirtualToPhysical`); music stops in `__CSPSetUsptFromTempo` (the `__CSP*`/`alCSeq*`
   player is still m2c views); speech is stubbed (s4).
5. **macOS beyond mode 0**: rerun the merged tree on the Mac (the merge fixed several of the
   paths s3 listed); then the AI action callbacks that take the actor as `s32`
   (`Action_IdleEnter` -> `CScene__SetInstanceSavedBit`), and `MtxPool_Alloc` declared `s32` in
   func_0021CDAC.c.
6. **Pixel screenshots on macOS**: no headless Metal/GL there; the Mac compares display lists
   (`T2_DLDUMP_AT`). A Metal offscreen target (or a window run by a person) for pixels.
7. Input: drive the title menu into a level (`--input` scripted presses, s1) once the AI runs.

# Round 3, merged state (history)

Round 3 merged five branches, in this order: r2 (archive format 2.0, the loader through the
archive in host byte order), r1 (the boot runner to mode 0), r3 (the 64-bit typing sweep), r5
(macOS arm64) and r4 (audio behind `--audio`). Then it fixed what the merge broke and followed
the black frame to the text and the menu. This page says how to build and run the result, how
far `turok2 --game` gets, what stopped it on the way (one table for the whole round) and what
comes next. The part sections further down keep each branch's own notes. Details:
[DATA.md](DATA.md) (gendata), [64BIT.md](64BIT.md) (part 2: retype.py), [AUDIO.md](AUDIO.md),
[MACOS.md](MACOS.md), [RUNTIME.md](RUNTIME.md) (13: the typed scheduler),
[../assets/PORT.md](../assets/PORT.md) 10.4/10.6 and [../assets/ARCHIVE.md](../assets/ARCHIVE.md)
(`t2/swap`).

The matching build is unchanged. `tools/asm_diff.py --base 3798e01e -j 8` reports 0 differences
over all 3,938 files after every merge and fix. A clean `make clean && make build && make verify`
gives the identical ROM (CRCs good, 123 shared .rodata objects agree).

## Round 3: build and run (Linux x86_64, the box)

```sh
# 0. the matching build first: gendata reads its ELF (build/turok2.us.elf)
make setup && make -j8 build && make verify

# 1. the archive, format 2.0 (t2/swap byte-swap plans); the port refuses 1.x archives
#    (sha1 99cbe76733bfab1d7235e4894cec2b91be3965f7 with zlib 1.3.1, any -j)
python tools/assets/mkarchive.py baserom.us.z64 -o OUT/turok2.o2r -j 4
python tools/assets/archive_check.py OUT/turok2.o2r baserom.us.z64     # OK, format 2.0

# 2. configure; build the game library AND the runtime/audio libraries: gendata reads the
#    undefined symbols of all three (t2audio_mgr.c reads gAudioFrameSize, gAudioGlobals, ...)
cmake -S port -B OUT/build -G Ninja -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ \
      -DCMAKE_BUILD_TYPE=Debug
ninja -C OUT/build -j8 turok2_game t2runtime t2audio

# 3. the game data as C, into port/gen/ (gitignored: ROM content); libclang 19 python bindings.
#    Drop the cache whenever a header changes (it holds the types): sizes and aliases follow them.
rm -f OUT/decls.json
python3 tools/port/gendata.py --build OUT/build --cache OUT/decls.json -j 4

# 4. re-run cmake so data.cmake sees port/gen/*.c, then the executable
cmake OUT/build && ninja -C OUT/build -j8 turok2

# 5. run headless and draw (SDL's offscreen driver: no window; --no-window would skip Fast3D)
SDL_VIDEODRIVER=offscreen T2_SHOT=OUT/frame.ppm gdb -batch -ex run -ex "bt 30" \
    --args OUT/build/app/turok2 --archive OUT/turok2.o2r --rom baserom.us.z64 \
    --game --draw --frames 300            # add --audio for the sound world (section 2)
```

`T2_SHOT=<file.ppm>` writes the last frame Fast3D drew (640x480). `T2_DLDUMP=1` prints the last
display list; it is read after the run, when the game may already be filling that buffer again,
so trust a breakpoint over it for single commands.

gendata on this tree: 1,545 data symbols that module-1 C and the runtime need (1,539 typed from
a C declaration), 393 zero .bss symbols (29 byte views doubled for host pointers), 3,794
pointer slots (2,770 relocated, 994 null in the ROM, 30 unresolved).

## Round 3: how far `turok2 --game` got

```
t2: boot: arena 32 MB at 0x40200000 (low memory), ROM baserom.us.z64, mainproc started (pri 13)
game: booted, 300 frames
t2: Sound__Initialize: audio off (run with --audio)
t2: __osSiRawStartDma: PIF not modelled (no Controller Pak yet)
game: 300 frames in 2.78 s, 1414 thread switches, 151 gfx tasks (151 drawn), 0 audio tasks,
      0 PI DMAs, 0 missing-function calls
OK (0 failed checks)
```

Boot -> `CEngineApp__Main` -> the engine, camera pool, player pool, `CScene__Construct` and
`CScene__LoadLevel(0)` -> mode 17 -> **mode 0 (pak check)**, which draws every retrace through the
game's scheduler into Fast3D: 300 frames, 151 display lists drawn, no crash.

What the frames show now (they were black with a 4-pixel red bar at the end of r1):
- frames up to ~120: **"ACCESSING THE / CONTROLLER PAK"** in the game's small font, beige,
  centred at the right place on the 480x360 screen;
- from frame ~120, the timer opens the pak menu (`gPakSaveGameAnyPakMenuDesc`, then
  `gDamagedPakAtBootMenuDesc`, because no Controller Pak is modelled): the menu's **background
  image** (the reptile-scale texture, full screen, darker bands at top and bottom) and the
  **red highlight bar** of the selected line, centred. The menu lines themselves draw no
  glyphs yet (below).

With `--audio`: the sound bank is converted (2 bank files, 34 instruments, 1,115 sounds, 866
wave tables), the sound data file arrives in host order through the archive (the audio hook
sees that and leaves it alone), the audio manager starts. Mode 17's `Sound__StopAllExcept` now
works (the voice pool and request queue typed, rows I5/I6). It then stops in
`SndQueue_CancelHandle` from `CAudioWorld__StopHandle` <- `Sound__Stop` <- `LoopSounds_StopAll`
<- `CScene__LoadLevel` (mode 0's init): `CAudioWorld__StopHandle` reaches the queue through the
Rage Wars `ContextRecAt2BC0` view of `CAudioWorld` at N64 offsets.

macOS arm64 (r5, [MACOS.md](MACOS.md)): the whole port builds and the tests pass. The game runs
headless with full 64-bit pointers (a low-memory mode is impossible there). With r1+r3+r5 it gets
past the camera pool and stops in `Model_Decompress` -> `Material_Unpack` (section 4).

## Round 3: crash and fix table (all parts)

Rows r1-* and r4-* were found on the branches; I-* during the merge (C18). Every fix is under
`TARGET_PORT` or in a port-only file, asm_diff 0.

| # | where | cause | fix |
|---|---|---|---|
| r1-1 | `CScene__GetRegionInfo` via `CCamera__AdvanceAttributes` | `CEngineApp`/`CGame` held the scene, camera pool, onscreen as N64-sized bytes | `structs/game.h` under TARGET_PORT: typed one-element-array members; literal CEngineApp/CGame offsets by `T2_OFFSETOF` |
| r1-2 | same | gendata translated offsets inside a union through its last member | a union prefers the member that types the offset |
| r1-3 | `CCamera__AdvanceAttributes` | `CCameraViewParams__Construct` wrote through a 4-byte-pointer view | host body |
| r1-4 | `Math_AcosLookup(NaN)` | libultra's `du` doubles generated by their words | gendata: a union's whole-size scalar member lays out the data |
| r1-5 | `CHeap__Free(NULL)` | `gPoolBuffers` generated at its N64 0x20 bytes | gendata doubles byte-only .bss; `CCache__AllocateAt` through `CMemEntry` |
| r1-6/7 | player / loader lists | link offsets 0x131C/0x1320, 0x18/0x1C | `T2_OFFSETOF`; host `PlayerList` |
| r1-8 | `CCache__DetachCacheEntry`, `__FreeCacheEntry` | m2c offsets | typed bodies |
| r1-9 | `CCache__LoadPersistantIndex` | `s32 cart` received a pointer | `sptr cart` |
| r1-10 | `Model_Decompress` | decompression blocks written at N64 offsets | r2's typed `CCache__AllocDecompressFlags` |
| r1-11 | `SwooshSystem_UpdateSwooshes` | swoosh/effect system as N64 bytes | **stub** (left empty on the host) |
| r1-12/13 | CScene views, `CGameObjectInstance__SetupAnimation` | N64 offsets, region stride | by field / typed |
| r1-14 | `__scMain(sc=NULL)` | scheduler views overran on LP64 | `port/game/t2sched.c` (17 functions typed, originals out of the link) |
| r1-15 | `SndVoicePool_*` from `Sound__StopAllExcept` | no sound bank | `Sound__*` return early while `gAudioWorld.bank` is NULL (they stay: they only fire without `--audio`) |
| r1-16/17 | delayed level events, actor lists | bytes / m2c offsets | typed |
| r1-18 | `CCameraPool__AdvanceText` | camera text/shake/emitter lists, sky system | links by `T2_OFFSETOF`, `CEmitterPool m_SkySystem[1]` |
| r1-19/20 | Fast3D `G_MOVEMEM` | `DlCmd` 32-bit words; static display lists with N64 addresses | `DlCmd` host words; gendata relocates w1 in static lists |
| r1-21 | `Menu_Construct` & co. | menu sizes, links, desc tables, desc text addresses | `MENU_SIZE`...; pointer-target tables; `t2_n64_to_host` |
| r4-A1..A5 | `alCSPNew`, heap, `alSndpAllocate`, `alEvtqPostEvent`, `CAudioWorld__Lock` | views at N64 offsets | typed (AUDIO.md) |
| r4-A6 | `_sndpVoiceHandler` spins | `player.evtq` overwritten | open on r4; not reached now (I6 stops first) |
| I1 | link: `gAudioFrameSize`, `gAudioGlobals`, `gAudioMaxRSPCmds` undefined | r4's audio manager files left the game library, so gendata no longer saw these names | gendata also reads `libt2runtime.a`/`libt2audio.a`'s undefined data names (build them before gendata) |
| I2 | merge r1+r3 does not compile | r3's `sptr` prototypes vs r1's `s32` port bodies | r3's prototypes kept, r1's bodies adapted (`CCache__AllocateAt`, `..ForDecompression[Top]`, `Sound__Play`/`SetListener` with the guard, `MenuPool_Construct`) |
| I3 | `VM_Alloc` (r2 vs r4) | r2 reads the unit through the archive in host order; r4 copied ROM bytes and swapped | r2's `T2Port_AssetWindow` + r4's `T2Audio_FixRegion` (detects host order) |
| I4 | pak menu lines measured 0 wide | the menu strings (`D_800B50C0` "damaged controller pak found in" ...) are address-only labels (SHN_ABS) in `.code_rodata`; gendata made every SHN_ABS symbol zero .bss | gendata reads SHN_ABS labels inside `.code_rodata`/`.code_rodata_tail` as data (`ABS_DATA_SECTIONS`) |
| I5 | `--audio`: `SndVoicePool_StopAllExcept` (mode 17) | Rage Wars views of the voice pool (0xCC-byte slots, 4-byte owner) | `SndVoicePool_Construct`, `SndVoicePool_StopAllExcept` typed on `SndVoicePool`/`SndVoice` |
| I6 | `--audio`: `SndQueue_CancelHandle` | the same for the request queue | `SndQueue_Construct` and `SndQueue_Cancel{AllExcept,Handle,SoundId,Owner,Flags}` typed on `SndQueue`; now stops in `CAudioWorld__StopHandle` (open) |
| I7 | no text, no font texture | `CScene__LoadGraphicSet` read the scene through an N64-offset view (0x1C, 0x48, 0x7C): the font graphic set was never found, `sOnScreenFontTexture` stayed NULL | typed on `CScene` (`m_GraphicSetsKey`, `m_pGraphicSets`, `m_pGraphicSetTypes`) |
| I8 | texture rectangles of size 0, black text | `COnScreen__DrawString`/`DrawText` read colours, scale and spacing through `FontStyle { u8 pad0[0xA0]; ... }` | pad by `T2_OFFSETOF(COnScreen, m_TopR)` |
| I9 | wrong glyphs ("!## 33)" for "access") | `COnScreen__Construct` built the character tables through a `Hud` view at N64 offsets (0xC8/0x148) | typed on `COnScreen` (`m_CharacterSmallLookUpPos`, +0x80) |
| I10 | everything drawn 1.5x too large, off to the right | Fast3D's native size is 320x240; the game draws at 480x360 | `game.cpp` sets the interpreter's native size from `gScreenWidth`/`gScreenHeight` before each frame |
| I11 | menu at the left edge, no background | `MenuPool_GetCamera` = pool - 0x53C (N64 `offsetof(CCamera, m_MenuPool)`): the menu's camera and viewport were 44 bytes into the camera | `pool - T2_OFFSETOF(CCamera, m_MenuPool)` (`sptr`) |

## Round 3: next (as it stood)

1. **The pak menu's lines.** They are laid out and highlighted but draw no glyphs: the items of
   `gDamagedPakAtBootMenuDesc` reach `MenuItem_DrawString` with `text` pointing into the desc
   table itself (`D_0043BF18 + 0x38`, an empty string), while the first menu's items map their
   N64 text addresses correctly (`t2_n64_to_host`). Follow the item text of this desc (its update
   callbacks `DamagedPakMenu_UpdateLine1/2` and the `data` words of the .virtual desc table).
2. **Controller Pak / input** (done in round 4 s1, below): `__osSiRawStartDma` was not modelled, so the game always sees a
   damaged pak and never gets past mode 0. Model the PIF (controllers, then a pak file) to reach
   the title screen.
3. **Audio**: `CAudioWorld__StopHandle` and the rest of the `CAudioWorld` API (about 40 functions:
   `SndVoicePool_*`, `SndVoice_*`, `SndQueue_RequeueVoice`) go through Rage Wars views of
   `CAudioWorld` at N64 offsets (`ContextRecAt2BC0`, `RecordCode8025A3EC`, ...). Type them on
   `CAudioWorld`/`SndVoicePool`/`SndQueue` as I5/I6 did; then r4's open `evtq` corruption.
4. **macOS** (full 64-bit, r5 found on r1+r3+r5): `Material_Unpack` (func_00207914) writes the
   texture handle into `Material.texture` (an `s32` inside the model block) and `texture + 4`:
   needs a host-typed `Material` or a side table. Then five more truncations listed in
   [MACOS.md](MACOS.md) (decompress callbacks with `s32` arguments, the
   `LoadPersistantSubBlock` callback argument, the CAI view in `DeallocateInternal`,
   `LoadBitSections` with an `s32` destination; r3's e9b1dfcc covers part of these: re-check).
5. The swoosh/effect system (CEngineApp 0x19608-0x22BA0, r1-11) typed instead of left empty.
6. 64-bit: 1,727 pointer-width warnings, **1,019 truncating** (`retype.py --dry --rules ""` on
   this tree; 2,828 before r3), 999 disagreeing declarations. Each crash above was one of them.

## Round 4, part s5: the swoosh/effect system typed, N64-offset sweep (menus, cameras, level load)

Branch `names-R4_s5` (from 474e40af). Every commit: `tools/asm_diff.py --base 474e40af -j 8` = 0
differences (over the whole tree, 3,938 files, for the commits that touch `include/`).

**The swoosh/effect system is typed and runs on the host (r1-11's stub is gone).**
`include/structs/swoosh.h` lays out `CSwooshSystem` (`gEngineApp_m_SwooshSystem`, CEngineApp +
0x19608, N64 size 0x95B4: it runs 0x1C bytes into `unk22BA0`): the ribbon vertex pool (2 x 300
`SwooshVtx`, the count at 0x2580, the cursor at 0x2584), 96 `SwooshNode` points (0xB8) and their
free list at 0x6A88, 32 `Swoosh` trails (0x54) with their NodePool at 0x7514 (live swooshes =
`m_SwooshPool.used`, head 0x7520), 3 `SwooshTimer`s (0x1C) with their pool at 0x7580, 100
`SwooshDecal`s (0x50, ground shadows/scorches) in 10 kind `CList`s at 0x94D8 and the free list at
0x95A0. mips `_Static_assert`s keep the N64 offsets. `structs/game.h` (TARGET_PORT) embeds it as
`union { CSwooshSystem m_SwooshSystem[1]; u8 _SwooshBytes[0x22BC0 - 0x19608]; }`, so gendata's
alias `gEngineApp_m_SwooshSystem` and everything after it follow the host size.

The owner of a swoosh is a `CGameObjectInstance` when `flags & 1` (count `m_ActiveSwooshes` at
0x10F) and a `CParticle` when `flags & 2` (0x1B5); `Trail_AddNode` takes a `CParticle` (effect
flags, `m_pOwner` as the key with `flags & 8`, `header.m_nObjType` as the id).

| what | files |
|---|---|
| on the shared types (same C for N64 and host) | `func_0042D410` (init: the host stub is gone), `SwooshSystem_Update`, `_UpdateSwooshes`, `_UpdateTimers`, `RibbonVtxPool_Reset`, `Swoosh_TakeNode`, `SwooshGroup_Remove`, `Swoosh_NewNode`, `Swoosh_DetachOwner`, `func_002368B0`, `func_002360A0`, `Swoosh_AddParticleNode`, `func_002364A4` (decal), `SwooshSet_FreeOwner`, `func_00236444` (timer), `func_00235C30` (decal draw), `Trail_AddNode`, `Trail_DrawAll`, `Trail_Draw` |
| views with their pointer members (host-correct, N64 identical) | `Swoosh_Update`, `Trail_DetachOwner` (CParticle, owner counts by type; its `register asm` hack is N64-only), `Trail_PlaceNodes`, `Trail_ComputeNodeFrame`, `Trail_ComputeFrames`, `Trail_DrawTextured`, `Morph_BlendVertices(Tex)`, `func_00236D10`, `func_00236D50`; `rw/abu_types.h`: `Actor_func_802A274C_de`, `ObjRecAt28V1`, `State802A6DF8`, `Node438C`, `Morph` (its pointer/float unions are floats on the host) |
| host bodies (`#ifdef TARGET_PORT`) | the generic lists `DList_AddHead`, `DList_AddTail`, `NodePool_Alloc`, `NodePool_Free`, `func_0026D230` (they wrote 4-byte words at N64 offsets); `RibbonVtxPool_Alloc` returns the vertex pointer at host width; the decal draw reads the matrix translation as host words |

Check: `port/game/t2swoosh.c` (`T2Swoosh_SelfTest`, run by `turok2 --game` after the frames; one
line in `port/app/main.cpp`) builds a private `CSwooshSystem` with the game's code and checks the
pools (96/32/3/100, links both ways), a timer firing twice and freed, a swoosh of three points
(lengths 5 + 12, fractions, removing a point), a decal placed and freed by its owner, the ribbon
pool (host pointers, the 300 limit): `t2: swoosh self test: OK (0 failed)`. In the game the
system is built twice (mode 17 and mode 0's `CScene__LoadLevel`) and updated every frame; mode 0
spawns no swoosh, so the draw functions have only run on empty lists so far.

```
game: 300 frames in 2.78 s, 1414 thread switches, 151 gfx tasks (151 drawn), 0 audio tasks, 0 PI DMAs, 0 missing-function calls
t2: swoosh self test: OK (0 failed)
OK (0 failed checks)
```

**N64-offset sweep** (boot/menu/camera path). `tools/port/n64_offsets.py` counts the sites
(`M2C_FIELD`, `((s8 *)x) + 0xNN`, `*(T *)(x + 0xNN)`, `P_S32(p, 0xNN)` macros):
474e40af **378 sites in 135 files** -> **345 in 118** (m2c 62 -> 62, casts 161 -> 139, derefs
87 -> 80, macros 68 -> 64). Not in that count: the 34 swoosh files above, whose local pad views
were the larger part. Fixed here:
- the camera's menu pool, `(u8 *)player->m_pCamera + 0x53C` (I11's offset, used by every
  `MenuPool_OpenMenu`/`CountOpen` caller: pak menus, save portal, character/player setup,
  rumble, messages): `+ T2_OFFSETOF(CCamera, m_MenuPool)` in 21 files, the module-2 copies (func_0045xxxx)
  included;
- `CPlayer` views: `m_pCamera` (0x51C), `m_ContState.m_pContState` (0xA50),
  `m_Data.m_CheckPointWarpID` (0x92C) in `ShowPlayerMessage` (func_004233E8), func_00255C80,
  `SavePortalMenu_*` (func_00423210/278/364);
- `CScene__LoadLevel` (mode 0's init): the actor stride 0x288, `m_WeaponActor` (+0x288) and the
  player link 0x1320;
- `CCamera__GetViewOrigin` (0x24C, `m_vTCorners`), `CCameraPool__ReleaseAllTexts` (main camera
  0x40, `m_ActiveTextList` 0xE28/0xE2C, `m_FreeTextList` 0xF0C).

For the merge:
- `include/structs/game.h` (the CEngineApp swoosh member, TARGET_PORT only), new
  `include/structs/swoosh.h`, `include/rw/abu_types.h` (5 structs: pads -> the same-size
  pointer members), `port/app/main.cpp` (one CHECK line + one extern), new
  `port/game/t2swoosh.c`, new `tools/port/n64_offsets.py`.
- s2 territory touched by one-token edits only: the `+ 0x53C` lines of the pak/menu files
  (func_0041DF50, func_0041E1D8, func_0041EA84, func_0042019C, func_004206B8, func_0042085C,
  func_00423768, func_00423880, func_004183C0 and the `_2` copies); `func_00413230.c` itself is
  untouched.
- Rerun gendata with a fresh cache (CEngineApp's host layout changed after +0x19608).

Next (s5's area):
1. Exercise the trail draw path in the game (a level with swooshes: `Trail_DrawTextured` /
   `Morph_BlendVertices` write vertices and GBI through views that are now host-sized, but the
   textured path also reads `Texture_Request(trail->model)` handles (`s32 texture[2]` at 0x14,
   func_00207914's handle: s3's `Material` work on macOS changes it, keep the two in step).
2. `D_80119850` (`B21World`, func_00289F30) is a view from CEngineApp + 0x22BA0 into CGame at N64
   offsets (`sub5DC` = the menu pool): retype on `gEngineApp_m_Game`.
3. The 345 counted sites (top: func_002568CC 31 (player), func_0043EC68 19, func_00258964 12,
   func_00255F04, func_00275450 (`alCSeqNew`) 8), and the local pad views that the counter does not
   see (the swoosh files were 34 of them): the same recipe, `T2_OFFSETOF`/typed members, asm_diff
   per batch.

## Round 4, part s1: controllers and the Controller Pak (mode 0 passes)

Details: [INPUT.md](INPUT.md). Branch `names-R4_s1`, 4 commits on 474e40af.

- **Virtual PIF** (`port/runtime/t2pif.c`) behind `__osSiRawStartDma`: joybus status/reset,
  pak block read/write with address and data CRCs, over an emulator-standard 32 KB `.mpk` per
  port (`turok2_pak<N>.mpk` in the user data dir; blank formatted when missing; written block by
  block). The game's own libultra pfs C runs unchanged: the PIF converts the pack ID, inode and
  directory blocks between the file's big-endian layout and host order. Flags: `--pak-dir DIR`,
  `--pak-blank` (in memory, tests), `--no-pak`; debug: `--input START@300,A@400` (scripted
  presses), `T2_PIF_LOG=1`, `T2_MENU_LOG=1`. The run logs engine mode changes and PIF counters.
  Rumble Pak: "not present" (unchanged).
- **Game-side fixes** (TARGET_PORT only; asm_diff 0 over 3,938 files): pak address high byte
  first in `__osPackRam{Read,Write}Data`; queue as `OSMesgQueue *` in `__osContRam{Read,Write}`;
  typed `__osPfsSelectBank(_2)`; `__OSInodeUnit` `{page, bank}` on little-endian hosts
  (`include/ultra_types.h`); `MenuRecAt24` (`include/rw/abu_types.h`) and
  `PakConfirmMenu_SelectNo`'s `Item` lead with the host `Menu` fields (both crashed in the
  repair/continue confirm menus).

How far it gets now (Linux, headless, `--game --draw`):

```
--pak-blank : mode 0 -> f297 mode 1 (legal screen, full text) -> f1069 mode 3 ("EXPANSION PAK
              FOUND!", 3D model) -> f1357 mode 4 (cinema) -> SIGSEGV
--no-pak    : pak menu, any button -> mode 1
zeroed .mpk : damaged-pak menu -> attempt repair -> yes -> repaired (the file is a valid
              big-endian formatted pak) -> continue without saving -> mode 1
```

Where it stops (mode 4, not in s1's scope):

```
CCameraViewParams__Construct (o=0x31)        func_00280C28.c:12
CCamera__Construct (view=0x1)                func_0042F9B0.c:37
CCameraPool__AllocCamera (gEngineApp_m_Game_m_CameraPool) func_0042FF58.c:21
CPlayer__NewGame (character=0)               func_0042DC18.c:33
CPlayerPool__NewGame (count=1)               func_0042E5AC.c:96
CEngineApp__StartWarp (.., 100)              func_00288DD0.c:26
EngineMode04_Cinema_Init                     func_00289554.c:6
EngineMode17_Switch_Retrace                  func_00289DFC.c:42
```

For the merge: files others may touch: `include/ultra_types.h` (inode union),
`include/rw/abu_types.h` (`MenuRecAt24` only), `src/us/code/func_00420F34.c`,
`port/app/game.cpp` / `main.cpp` / `game.h` (flags, mode log, scripted input),
`port/runtime/t2ultra.c` (`__osSiRawStartDma` moved to t2pif.c), `port/runtime/t2port.h`
(`T2PortConfig.pakMode/pakDir/pakPorts`), `port/app/CMakeLists.txt` (+t2pif.c),
new `port/game/t2modelog.c`. Header changes: drop `decls.json` before gendata.

Next: the mode 4 camera crash; a swap plan for the save records so notes move between the
port and emulators (the pak filesystem already does); a Rumble Pak mode; macOS: the same code
is LP64-clean (the queue truncation is fixed) but untested there.
## Round 4, part s2: boot runner part 2 (mode 0 to the title screen)

Branch `names-R4_s2` (from 474e40af). Every commit has `tools/asm_diff.py --base 474e40af -j 8` at
0 differ or fail over the files it touches; the one include change (`structs/world.h`, under
`TARGET_PORT`) was checked over the whole tree (3,938 files, 0 differ).

### How far it gets

```
SDL_VIDEODRIVER=offscreen T2_SHOT=out.ppm T2_SHOT_EVERY=300 build/app/turok2 --archive turok2.o2r \
    --rom baserom.us.z64 --game --draw --frames 5000 --skip-pak-check
game: frame 2: engine mode 17 -> 0          pak check: "ACCESSING THE CONTROLLER PAK"
game: frame 16: engine mode 17 -> 1         legal screen, the full text
game: frame 788: engine mode 17 -> 3        "EXPANSION PAK FOUND!" with the 3D pak model
game: frame 1080: engine mode 17 -> 4       intro cinema 100 ("acclaimlogo", level 162): runs 486 frames, draws black
game: frame 1570: engine mode 17 -> 5       intro cinema 90 (level 44): runs to its end, draws black
game: frame 3932: engine mode 17 -> 7       TITLE SCREEN (logo, "SEEDS OF EVIL", the scale background)
game: 5000 frames in 32.31 s, 1911 gfx tasks (1911 drawn), 0 missing-function calls
```

The whole boot sequence of the N64 game now runs on its own, timers and cinemas included, from
the pak check to the title. `--start-mode 7` reaches the title at frame 3 for quick checks.
With s1's PIF/pak the debug switch is not needed for mode 0 (s1 reported mode 0 -> 1 -> 3 -> 4
and the crash in mode 4's `CCameraPool__AllocCamera`, fixed here: row s2-3).

After the title's timeout (frame 5183) the game starts the attract demo (mode 13, level 81) and
stops in an enemy's mode setup:
```
#0 CAI__SetMode (arg2=0)  func_00243414.c:203   entry->unk_04 (the mode's enter function) is garbage
#1 EnemyAI_InitDispatch                          func_0025BE88.c:52   the enemy's mode table, set through N64 views
#2 func_0021E5D4 <- CScene__LoadLevel(81) <- CScene__UpdateWarp <- EngineMode13_Game_Draw
```
That is the start of "levels run" (the enemy AI, s5's sweep: ~400 offset sites).

Screenshots (`T2_SHOT_EVERY`): the damaged-pak menu with its lines ("PROBLEM!", "DAMAGED
CONTROLLER PAK FOUND IN CONTROLLER #1.", "ATTEMPT REPAIR" highlighted, "RETRY", "CONTINUE WITHOUT
SAVING"), the legal text, the Expansion Pak screen with the rotating pak model, the title. The two
cinemas draw black frames: their scene draws no geometry yet (no `G_VTX` in the frame's list).

### Debug switches (DEBUG ONLY, port/game/t2debug.c)

| switch | effect |
|---|---|
| `--skip-pak-check` | once mode 0 has started, sets `gPakCheckDone` ("continue without saving"): `EngineMode00_PakCheck_Retrace` takes its own exit to mode 1 |
| `--start-mode N` | once mode 0 has started, `CEngineApp__SetMode(&gEngineApp, N)` (7: title) |
| `T2_SHOT_EVERY=n` | with `T2_SHOT=<f.ppm>`: also `<f.ppm>.<frame>.ppm`, the first drawn frame of every n |

Every run logs the engine mode changes (`game: frame F: engine mode A -> B`). The two switches go
away once s1's pak model is merged (keep them until the demo path runs, they save 1000 frames).

### Crash and fix table (round 4, s2)

| # | where | cause | fix |
|---|---|---|---|
| s2-1 | pak menu lines draw nothing | `MenuItem_DrawText` read the item as a `Menu` and passed `->desc` (N64 0x14 = `MenuItem.text`; host: `MenuItem.def`, the desc row, an empty string) | typed on `MenuItem`, passes `text` |
| s2-2 | mode 0 never ends | no PIF/pak (s1) | debug switches above |
| s2-3 | `CCameraViewParams__Construct(0x31)` from `CCameraPool__AllocCamera` (mode 4 `CPlayer__NewGame`) | pool view with the free/active lists at N64 0xC/0x20 | `CCameraPool__AllocCamera(_2)` typed on `CCameraPool` |
| s2-4 | `MenuPool_CountOpen` from `Automap_Update` | camera menu pool as `m_pCamera + 0x53C` (23 files; three through raw player offsets 0x51C/0xA50) | `T2_OFFSETOF(CCamera, m_MenuPool)`, `player->m_pCamera`, `m_ContState.m_pContState`; `MenuPool_GetCamera_2` as I11 |
| s2-5 | `func_00267DBC`, `func_0021E5D4` (actor init), `func_0021F1B4`, `CScene__LoadLevel` | N64 views: intelligence at 0x14, AI at 0x140, flags 0xD4, 0x288/0x1328 strides, player link 0x1320 | typed / host body / `sizeof`, `T2_OFFSETOF` |
| s2-6 | `InteractiveAnim_Advance` | AI vars at 0x10C | typed on `CAI.m_VarS32/F32` |
| s2-7 | `CScene__BuildActorLists`: `m_pActors` = 0x190_402B4900 | `CScene.m_Heap` is `u8[0x10]` but holds a pool of pointers (24 host bytes): `MtxPool_Reset` wrote over `m_pActors` | `MtxPool` (structs/world.h), `CScene.m_Heap` typed under TARGET_PORT; `MatrixHeap_Construct`, `MtxPool_Reset`, `MtxPool_Alloc` typed |
| s2-8 | `CCache__Deallocate(0x50)` from the cinema spline | `TempAlloc` kept the handle as an s32, `TempFree` read `((s32 **)data)[-4]` (32 bytes back on the host) | the whole pointer, 0x10 bytes back (TARGET_PORT; both cinema copies) |
| s2-9 | archive abort: grid section `0x01F748E4` not a unit | `CScene__DecompressPaths` wrote the paths through a view at 0x58/0x68 (N64 `m_pRegionData`/`unk_68`): on the host that was `m_pCollision` | typed on `CScene` |
| s2-10 | Fast3D `G_VTX` with w1 `0x00800080020000D7` | the swoosh display list `D_800ACFC8` declared `u8[]`: gendata emitted ROM bytes | declared `Gfx[]` (gendata relocates it) |
| s2-11 | Fast3D runs matrices as commands (`G_DL` to a freed model) | `CCache__LockForDisplayList`, `Begin/EndDisplayList` and the cache thread `CCache__Main` used `D_800D7BE0` (gCache - 0x5F0) through `B12Mgr`/`B12Node`/`B12Msg` N64 views: the lock bit went into `CMemEntry.lockCount`; `CCache__Main` waited on a queue never created on the host (async loads and display-list releases never completed) | the four typed on `gCache`/`CMemEntry`/`CCacheEntry` (TARGET_PORT bodies) |
| s2-12 | Fast3D runs stale commands (the frame's list 8 bytes off) | `CCamera__DrawGlares` wrote one command as two u32 and stepped 8 bytes | one host `Gfx` (TARGET_PORT) |
| s2-13 | intro cinema never ends (`clipStart` -4761, 9007 cues) | `Pack_Add` called the memcpy `func_0029E460(data)`: m2c dropped the source and size (still in a1/a2 on the N64): every cinema resource block was left uninitialised on the host | `func_0029E460(data, prepare, size)` (matches); `proutSprintf` the same; `prepare` is `sptr` |
| s2-14 | `CAI__Update` callback | Rage Wars Source/Dest views | typed on `CGameObjectInstance`/`CAI` (matches) |
| s2-15 | `CAI__SetMode` | the same, plus the mode-table chain | host body on `CAI`/`CAIModeInfo` |
| s2-16 | `LevelEvent_Fire` and 11 other files | camera pool at `&multiplayer - 0x1201/0x1221`, actor stride 0x288, AI at +0x140, weapon actor +0x288/+0x3C8 | by name / `sizeof` / `T2_OFFSETOF` |

### For the merge

- Files others may touch: `port/app/{game.cpp,main.cpp,game.h}` (s1 adds flags there too: keep
  both sets of options; the mode log and `WriteShot` are mine), `include/structs/world.h`
  (`MtxPool`, `m_Heap`), `src/us/code/func_00413230.c` (menus: `MenuItem_DrawText` only),
  `func_0044C230.c` (`MenuPool_GetCamera_2`), the cache files `func_002041E4.c`,
  `func_0020548C.c`, `func_00205574.c`, `func_0020565C.c`, cinema `func_0040F240.c`/
  `func_00448240.c`, `func_00218590.c` (`Pack_Add`), the AI files `func_00243414.c`,
  `func_00245848.c`, `func_00266794.c`, and the 23 menu-pool files of s2-4 (s5's offset sweep may
  touch the same lines: they compute the same offsets).
- Rerun gendata with a fresh cache (`rm decls.json`): `world.h` and `D_800ACFC8`'s type changed.
- A fresh `turok2.cfg.json` makes LUS open 1024x768 and the game then draws into the lower-left
  640x480 of it (seen once with a new output directory): copy a 640x480 config or fix the
  default; screenshots assume 640x480.
- macOS (s3): s2-7, s2-8 and s2-11 are host-layout fixes that matter there too; several s32
  pointer arguments remain on this path (`CEngineApp__StartWarp(s32 app)`, `Action_EventGo`,
  `CHeap__Free(s32)` in `CCache__Main`, the cinema path items' `ref`).

### Next

1. The attract demo (mode 13): `EnemyAI_InitDispatch` and the enemy mode tables (N64 views of
   `CAI`), then the level renderer for the demo and the cinemas (they draw no geometry: follow
   `CScene__Draw`/the actor draw on level 162, whose acclaim-logo actor should show).
2. Merge s1 (PIF/pak) and drop `--skip-pak-check` from the default runs.
3. The rest of the Rage Wars views on the AI path (`AI_UpdateRegionFlags`, `CAI__UpdateModeAnim`,
   `EnemyAI_*`): s5's sweep, by the crash order above.

## Round 2 (history)


### How far `turok2 --game` got after round 2

```
t2: boot: arena 32 MB at 0x40200000 (low memory), ROM baserom.us.z64, mainproc started (pri 13)
game: booted, 120 frames
t2: Sound__Initialize: audio off on the host (sound bank not converted yet)
Thread 5 "turok2" received signal SIGSEGV
#0 CISet__GetBlock (arg0=0x0, arg1=0)                         func_002017D4.c:6
#1 CScene__GetRegionInfo (arg0=&gScene, arg1=0x3e32b8c400000000) func_0022425C.c:26
#2 CCamera__AdvanceAttributes (cam=gEngineApp_m_Game_m_CameraPool_m_MainCamera+76) func_0027CAB0.c:73
#3 CCamera__Advance                                            func_0027D160.c:102
#4 CCameraPool__Construct (pool=gEngineApp_m_Game_m_CameraPool_m_ActiveList_m_pHead+4) func_0042FE4C.c:30
#5 CEngineApp__Main (e=&gEngineApp)                            func_00286674.c:81
#6 mainproc                                                    func_00288860.c:13
other threads: CLoader__Main blocked in t2_osRecvMesg (as on hardware), the cache and
decompressor threads created and waiting; platform thread in t2os_run_until_idle
```

What now runs, in order: boot -> `CEngineApp__Main` -> `InitializeSystemResources` (CHeap on
the arena, decompressor and loader threads, `CCache__Construct` with its pools, hash table and
thread) -> `GfxPool_Alloc` (frame buffers, depth buffer, display lists, RSP FIFO, audio heap)
-> `CEngineApp__Construct` (the engine object, the synthesizer up to `amCreateAudioMgr`; the
sound bank is skipped) -> `CCameraPool__Construct` -> the main camera's first `Advance`, where
it stops. Mode: none yet (`CEngineApp__SetMode(0)` comes after the pools); 0 frames drawn,
so `--draw` (q4's GBI is in) has nothing to show yet.

### Crash log (round 2)

| # | where | cause | fix |
|---|---|---|---|
| 1 | `CHashTable__Construct` (func_00202860) | CHeap hard-coded the N64 block header (0x20; host `CHeapBlock` is 48): free list wrong from the first block; `HashEntry` 16 bytes on the N64, 32 on the host | header as `sizeof(CHeapBlock)` in CHeap Construct/Alloc/AllocTop/AllocAt, addresses through `uptr` (Free too); hash table by `sizeof(HashEntry)` (Construct, Insert). asm_diff 0 |
| 2 | `CHashTable__Construct` again | heap corrupted by `CCache__Construct`: the free stack sized `n * 4` holds 8-byte pointers; pools by N64 strides 0x28/0x2C; CList link offsets 0x18/0x1C/0x20/0x24/0x28 | pools by `sizeof(CMemEntry)`/`sizeof(CCacheEntry)`, stack by `sizeof(CMemEntry *)`, offsets by `T2_OFFSETOF` (new in common.h); `CCacheEntry` gains its `last` link at 0x28 (size 0x2C). asm_diff 0, check_structs 0 wrong |
| 3 | `GfxPool_Alloc` (func_00425220) | frame/depth buffers claimed at fixed RDRAM addresses (`GetMemSize() \| 0x80000000` - 1 MB): outside the host arena, `CCache__AllocateAt` returns NULL | `#ifdef TARGET_PORT`: `T2Port_RdramTop()` (t2boot.c: arena end + 512 KB, the console's geometry) |
| 4 | `alFilterNew` from `alSynAllocFX` (func_00297420) | m2c view: `s32 records` at 0x34 + `bus * 0x4C + 0x20` | rewritten as libultra's synallocfx.c with `ALSynth`/`ALAuxBus` (matches) |
| 5 | `alMainBusParam` (func_002964A0) | source list as `s32 *` | typed as libultra's mainbus.c (matches) |
| 6 | `alAuxBusParam` (func_00294E3C) | the same | typed as auxbus.c (matches) |
| 7 | `CAudioWorld__Initialize` (func_00270240:95) | the sound bank is a `VM_Alloc` copy of ROM 0x1E0000: big-endian, N64 layout, pointers that are VM addresses | **not fixed**: `Sound__Initialize` replaced by an empty host function (link.cmake, t2boot.c). Needs the bank converted (T2Asset + swap spec) |
| 8 | `CCameraPool__Construct` (func_0042FE4C) | local m2c struct with pads at N64 offsets, `memset(0x11F0)` | typed with `CCameraPool` (structs/camera.h), list links at 0 and `sizeof(void *)` (matches) |
| 9 | `CScene__GetRegionInfo` via `CCamera__AdvanceAttributes` | `cam->m_ViewParams.m_pCurrentRegion` is garbage: `CEngineApp`/`CGame` keep the camera pool, the scene and other parts as **byte arrays of N64 size** (structs/game.h), so on the host the typed `CCameraPool` (larger) overruns its slot, and the alias symbols (`gEngineApp_m_Game_m_CameraPool`, ...) disagree with `e->m_Game.m_CameraPool` (pool at `...m_ActiveList_m_pHead+4`, not even 8-aligned) | **open**, next round (below) |

### What round 3 was given to do

1. **`gEngineApp` host layout.** `CEngineApp` and `CGame` (structs/game.h) hold `CScene`,
   `CCameraPool`, the menu pool and more as `u8[N64 size]`, and `CGameFromCameraPool` views the
   same memory from the camera pool. On the host those members must be the real types (under
   `TARGET_PORT` at least), and gendata's aliases (`gEngineApp_m_Game_m_CameraPool`,
   `gEngineApp_m_Settings*`, `gScene_*`) then follow, since it computes their host offsets through
   the declared type. This is the next crash and blocks everything after the camera pool.
2. **Audio bank**: convert the sound bank (ALBankFile at ROM 0x1E0000) for the host and restore
   `Sound__Initialize`; `alAuxBusNew` points its handler at `func_00294D54 + 0xC` (asm in the
   middle of a word): a host `alAuxBusPull` is needed before any audio frame runs.
3. The ROM data the loader DMAs is big-endian: `ProPackDecompress` tests `'RNC'` as a native
   word, so on the host it will not see RNC streams; route the loader/decompressor through
   T2Asset (assets/PORT.md §10.4) instead of raw PI DMA.
4. q2's known gaps: the 403 unresolved pointer slots in 38 runtime structs (mark them .bss),
   the `*ModeTable` integers that hold function addresses (type them, rerun gendata).
5. Remaining 64-bit work (64BIT.md part 2): 1,064 mismatched declarations, ~3,000
   pointer-width warnings; each fix above was found by running, the audit lists the rest.

## Round 3, part r2: the loader through the archive, assets in host byte order

Details: [../assets/PORT.md](../assets/PORT.md) 10.4 and 10.6, [../assets/ARCHIVE.md](../assets/ARCHIVE.md)
(`t2/swap`, format 2.0), [../assets/BYTEORDER.md](../assets/BYTEORDER.md).

- **Archive format 2.0** (`tools/assets/mkarchive.py`): new entry `t2/swap`, one byte-swap plan per
  unit, computed from `swap_spec.json` (`swapcheck.unit_plan`). Rebuild the archive: the port
  (`T2Asset_OpenLus`) refuses 1.x. Deterministic (sha1 `99cbe767...` with zlib 1.3.1, any `-j`).
  `archive_check.py` recomputes and checks the plans.
- **Reader** (`port/assets/t2asset.c`): `T2Asset_SwapUnit` applies the plan after every backend
  load (whole unit, then the prefix), so `T2Asset_Read` / `T2Asset_ReadInto` return host-order
  fields on a little-endian host; VM-window residents are swapped once; the wave table stays raw.
- **Engine** (`#ifdef TARGET_PORT`, asm_diff 0 over the 9 changed files): `CLoader__LoadSync`,
  `CLoader__LoadAsync` (one entry per request), `CLoader__Main` (no PI DMA) read through
  `T2Port_ReadAsset` (port/runtime/t2loader.c); `CCache__Request` sizes with
  `T2Port_AssetHostSize`; `VM_Alloc` returns the whole unit in host order; `ProPackDecompress`
  keeps only the plain-data branch (no 'RNC' path can be taken on the host).
- **Cache on the host** (found while checking the path in the game): `CCache__Construct` wrote
  through `D_800D7BE0`, which gendata places inside `gDecompressor`, so `gCache`'s free stack
  stayed NULL and the first `CCache__AllocMemEntry` crashed; now typed on `gCache`. Also the free
  stack by index (func_00203068, func_002044A8) and a typed `CCache__AllocDecompressBlock`
  (func_00205D7C) for `ProPackDecompress`.

Tests: `t2asset_test` (26,124 units = ROM bytes through the plans, double swap, probes, 20,000
audio reads, ROM backend + plans: 0 errors), `t2asset_typed_test` (1,585 field checks on level
headers, geometry incl. transposed vertices, texture banks against the area tools' big-endian
parsers: 0 errors), `turok2` self test (LUS = miniz = ROM+plans), `archive_check.py` OK.

How far `turok2 --game` gets: unchanged (`CCameraPool__Construct` -> ... -> `CISet__GetBlock(NULL)`,
r1's crash); no asset read happens before it. In gdb at `GfxPool_Alloc`, `CCache__Request` (direct,
RNC unit, `ProPack__Decompress` callback, 4-byte probe) returns correct host-order bytes.

Open, found on the way (not fixed here):
- `gPoolBuffers` is 0x20 bytes of .bss in port/gen (untyped), the host `GfxPool` is 64:
  `GfxPool_Alloc` writes `pool->gfx`/`vtx` over `gCFB[1]` (becomes a heap pointer), and
  `CEngineApp__Main` then memsets 150 KB there, over the cache tables. Needs a typed
  declaration so gendata sizes it.
- Other m2c views at N64 offsets on the cache path (`CCache__AllocateMemoryForDecompressionTop`,
  B12Mgr users) remain; `CHeap__Free`/`CCache__HeapAlloc` callers that pass pointers as `s32` work
  only in low-memory mode (not on macOS).
- `Texture_DecompressCallback` recolours RGBA5551 palettes with native u16 loads: needs
  big-endian accessors now that units arrive host-order but texels/TLUTs stay big-endian.
- The 0.5 % `unk` bytes of BYTEORDER.md arrive in ROM order (collision regions, particles, swooshes).

## Round 3, part r1: the boot runner (to the first frames of mode 0)

Branch `names-R3_r1` (merges r2's `names-R3_r2` at 691aad0a for the loader). Every commit:
`tools/asm_diff.py --base 3798e01e -j 8` = **3938 files, 0 differ**; `check_structs.py` on
game.h, camera.h, world.h, frame.h, sched.h: 0 wrong.

### Where it was at the end of r1

```
SDL_VIDEODRIVER=offscreen T2_SHOT=out.ppm build/app/turok2 --archive <r2's format-2 turok2.o2r> \
    --rom baserom.us.z64 --game --draw --frames 300
game: 300 frames in 2.66 s, 1404 thread switches, 151 gfx tasks (151 drawn), 0 audio tasks,
      0 PI DMAs, 0 missing-function calls
OK (0 failed checks)
```

Boot -> `CEngineApp__Construct` (scheduler thread up) -> camera pool, player pool, `CScene__Construct`
(cartridge, levels, texture sets, object types, objects indexes) -> `CScene__LoadLevel(0)` (level
0 geometry, collision, instances) -> the message loop -> mode 17 switches to **mode 0 (pak check)**:
`EngineMode00_PakCheck_Init` loads level 0 again, every retrace runs `CEngineApp__Update` ->
`CEngineApp__Draw` (`CScene__Update`, the camera pool draw, the menus) ->
`CEngineApp__SubmitGfxTask` -> the scheduler -> `osSpTaskStartGo` -> Fast3D. At frame 120 mode 0's
timer opens the pak menu (`gPakSaveGameAnyPakMenuDesc`, then `gDamagedPakAtBootMenuDesc`: no
Controller Pak is modelled) and its windows are drawn. 300 frames run to the end without a crash;
**151 display lists drawn by Fast3D** (2 retraces per frame, as on hardware).

The picture (frame 300, `T2_SHOT`): black, with the menu's frame rectangles but no text: the
window widths come out ~0 (a 4-pixel red bar at the left), so the text measuring/drawing (fonts:
`COnScreen` font loads, `Menu_TextWidth`, the HUD text path) is the next thing to follow. The game
renders at 480x360 (`gDPSetColorImage` width 480; `Video_SetMode(480, 360, ...)`). `T2_DLDUMP=1`
prints the last frame's display list (166 commands: init lists, scissor, colour/depth image, fill
rectangles, prim colours; no texture or vertex commands yet).

### What was done (crash table, round 3)

| # | where | cause | fix |
|---|---|---|---|
| 1 | `CScene__GetRegionInfo` via `CCamera__AdvanceAttributes` | `CEngineApp`/`CGame` held the scene, camera pool, onscreen as N64-sized bytes | `structs/game.h` under TARGET_PORT: `CScene m_Scene[1]`, `COnScreen unk19440[1]`, `union { CCameraPool m_CameraPool[1]; struct { pad; u8 m_MenuPool[0x18]; }; }` (one-element arrays keep `x.m_CameraPool` a pointer); mips `_Static_assert`s keep the N64 offsets (gendata parses mips too). 8 files with literal CEngineApp/CGame offsets (0x3C8, 0x22C00, 0x23DF0, 0x1268, 0x1228) by `T2_OFFSETOF` |
| 2 | same | gendata translated offsets inside a union through its *last* member (the byte pad) | `_dtranslate`: a union prefers the member that types the offset |
| 3 | `CCamera__AdvanceAttributes` | `CCameraViewParams__Construct` wrote through an ObjInit view (4-byte region pointer) | host body on `CCameraViewParams` |
| 4 | `Math_AcosLookup(NaN)` | libultra's `du` (`{ struct { u32 hi, lo; } w; f64 d; }`) generated by its words: doubles with swapped halves | gendata: a union's whole-size scalar member lays out the data |
| 5 | `CHeap__Free(NULL)` from `CCache__Flush` | `gPoolBuffers` (`extern u8 gPoolBuffers[]`, a GfxPool of pointers) generated at its N64 0x20 bytes: `GfxPool_Alloc` overwrote `gCFB`, then the frame-buffer memset hit the cache tables | gendata: a .bss symbol the C only declares as bytes gets twice its N64 size (409 symbols); `CCache__AllocateAt` through `CMemEntry` (its B12Node view put `flags` in `lockCount`); cache free stack by name (= r2's 6abc08a0) |
| 6 | `CPlayer__LevelStart` | player list links `0x131C/0x1320`, `PlayerList` view with N64 list offsets | `T2_OFFSETOF(CPlayer, m_pLast/m_pNext)`, host `PlayerList` (both pool files) |
| 7 | `CLoader__AllocLoaderEntry` | loader lists `0x18/0x1C` | merged r2 (loader through T2Asset) + links by `T2_OFFSETOF` |
| 8 | `CCache__DetachCacheEntry`, `CCache__FreeCacheEntry` | m2c offsets 0x14/8/0x10, list 0x14 before | typed bodies |
| 9 | `CCache__LoadPersistantIndex` (levels index) | `CScene__Construct`'s `s32 cart` received an 8-byte pointer, clobbering `file` | `sptr cart` |
| 10 | `Model_Decompress` | `CCache__AllocateMemoryForDecompression(Top)` wrote CMemEntry at N64 offsets | both through r2's typed `CCache__AllocDecompressBlock` (now `..Flags`) |
| 11 | `SwooshSystem_UpdateSwooshes` | the swoosh/effect system (0x9598 bytes in CEngineApp) is N64 bytes with 4-byte links | **stub**: `func_0042D410` leaves it empty on the host (no swooshes/effect slots). Needs typing |
| 12 | `CScene__DecompressCollision`, `CScene__LoadLightSettings`, `CScene__GetIntelligence`, `CScene__GetRegion` | local CScene views at N64 offsets; region stride 0x20 | views by `__builtin_offsetof(CScene, ...)`; host `CGameRegion` stride; `CGameRegion__Unpack` typed |
| 13 | `CGameObjectInstance__SetupAnimation` | Actor view at N64 offsets (called a data address) | typed on `CGameObjectInstance` |
| 14 | `__scMain(sc=NULL)` | the scheduler's views (Panel, Sched, Vid, ...) overran each other on LP64 | **`port/game/t2sched.c`**: the 17 scheduler functions typed on `T2Sched`/`T2ScTask` (`structs/sched.h`), the originals left out of the link (`link.cmake`); `CFrameData.m_Task/m_LineTask` typed; `CEngineApp__SubmitGfxTask`, `CEngineApp__AdvanceFrameData`, `Video_SetMode` typed; `gRefreshRate` (`&gCurrentFB + 1`) is a variable of its own; `__amMain`'s client is two pointers |
| 15 | `SndVoicePool_*` from `Sound__StopAllExcept` | no sound bank (audio off) | `Sound__*` return early while `T2Port_AudioReady()` is false (`port/game/t2audio.c`: `gAudioWorld.bank != NULL`), plus a guard in `CAudioWorld__StopAllSounds` |
| 16 | `LevelEvent_Fire(NULL)` | the delayed level events at CScene+0x1180 (CLists + 0x30 nodes) as bytes | `CDelayedEvent` + typed fields in `CScene` (TARGET_PORT); the 4 users typed |
| 17 | `CPlayerPool__RegisterActors`, `CScene__BuildActorLists` | m2c offsets (0x54, 0x2DC, 0x8EC...), settings-relative 0x1270/0x1210, camera 0x2D8 | by field |
| 18 | `CCameraPool__AdvanceText` | camera text/shake/camera/emitter lists `0, 4`; texts block 0x400; `m_SkySystem` was `u8[0x8D0]` but holds a `CEmitterPool` of pointers | links by `T2_OFFSETOF`, `16 * sizeof(CCameraText)`, `CEmitterPool m_SkySystem[1]` (camera.h, TARGET_PORT) |
| 19 | Fast3D `G_MOVEMEM` with data 0 | `CEngineApp__BeginDisplayList` wrote Gfx through junk_types' `DlCmd {u32 w0, w1}` | `DlCmd` is two host words under TARGET_PORT |
| 20 | Fast3D `G_MOVEMEM` at 0x800AB0D8 | static display lists (gRspInitDL...) hold N64 addresses in w1 | gendata: Gfx data (`GfxWord` pairs) get host pointers in w1 for G_VTX/MTX/MOVEMEM/DL/RDPHALF_1/SETxIMG and G_MOVEWORD segment (10 lists) |
| 21 | `Menu_Construct`, `Menu_Update`, `Menu_TextWidth` | menu windows/items/descs/model blocks by N64 sizes (0x1D8/0x28/0x24/0x330), colours by offset, pool links 0x1D0/0x1D4; desc item tables generated as one element; desc `data` (s32) holding N64 text addresses | macros `MENU_SIZE`... (func_00413230.c); gendata types a pointer target as a table (the count stored next to the pointer, e.g. MenuDesc.items/count; never over a C-declared symbol; 88 tables); `Menu_Construct` maps an N64 address in `data` with `t2_n64_to_host` |

### For the merge

- Files others touch: `src/us/code/func_00203068.c`, `func_002044A8.c` (identical to r2's; this
  branch takes r2's version), `func_00205D7C.c` (r2's typed allocation renamed
  `CCache__AllocDecompressFlags` + wrapper), CList literal offsets in `func_0042E5AC.c`,
  `func_0042E834.c`, `func_00201CC0.c`, `func_0042F9B0.c`, `func_0042FBA4.c`, `func_0042FFF0.c`,
  `func_0042B3E4.c`, `func_00413230.c` (r3's CList sweep does the same lines: take either, they
  compute the same offsets), `include/rw/junk_types.h` (DlCmd), `include/structs/{game,camera,
  world,frame,sched}.h`, `include/globals/framebuffer.h`, `tools/port/gendata.py` (unions, bss
  slack, Gfx relocations, pointer tables), `port/cmake/{game,link}.cmake` (port/game/*.c and the
  scheduler files), `port/app/game.cpp` (T2_SHOT, T2_DLDUMP), 24 `Sound__*` files (guard).
- Run with r2's archive (format 2.0, t2/swap): `/root/R3_r2/out/turok2.o2r` or rebuild it with
  r2's mkarchive. The C17 archive (format 1) no longer loads with r2's reader.
- After changing a header, rerun gendata with a fresh cache (`rebuild.sh --fresh`): the aliases
  and sizes come from the types.
- Several fixes truncate pointers through `s32` prototypes on purpose (low-memory Linux):
  `CCache__AllocateMemoryForDecompression`, `CScene__GetRegion`, `gDisplayList0/1` (s32), `CFrameData.m_pFrameBuffer`
  (u32). r5 (macOS, no low memory) will hit those; they are r3's prototype typing.

### Next

1. Fonts and text: why the menu's text measures 0 and draws nothing (COnScreen fonts, the HUD text
   path, texture loads: the first frame with textures). Then the pak menu with input (PIF/SI:
   `__osSiRawStartDma` is not modelled, so the game sees a damaged pak).
2. The swoosh/effect system (CEngineApp 0x19608-0x22BA0) typed, instead of left empty.
3. Audio (r4): the `Sound__*` guard goes away by itself once `gAudioWorld.bank` is set.
4. The remaining m2c views on the draw path (`T2_DLDUMP` shows what the frame lacks).

## Round 3, r3: 64-bit part 2 (the typing sweep)

Branch `names-R3_r3`. Every commit has `tools/asm_diff.py --base 3798e01e` at
`3938 files: 0 differ or fail`, `check_structs.py structs/game_object.h` at 0 wrong, and
`turok2_game` / `turok2_data` building with 0 errors. Details and numbers are in 64BIT.md, "Part 2".

- `tools/port/retype.py` (new) does the mechanical work at the columns clang reports.
  It turns `(s32)ptr` into `(sptr)`, retypes locals that carry pointers, drops casts in
  front of pointer parameters, and retypes the `s32` parameters and returns of functions
  that callers fill with pointers (rule P, every declaration). 880 files changed.
- Truncating pointer-width warnings went from 2,828 to 1,048 (clang, x86_64). The
  disagreeing declarations went from 1,059 to 999.
- The GfxPool path is pointer-wide: `CCache__AllocateAt`, `B12Node.key`, `MemHandle.addr`,
  and `gDepthBuffer`/`gDisplayList*`/`gLineList*`/`gAudioHeapBase` are now `sptr`. r5 saw the
  macOS run pass `GfxPool_Alloc`.
- All `CList__Construct` link offsets are now `T2_OFFSETOF`/`sizeof(void *)`.
  `MenuPool_Construct` is typed, and `CInstancePool__Construct` has a typed host body. r5
  saw the macOS run pass `MenuPool_Construct`. It next stops in
  `CCamera__ConstructSkySystem(+0x558)`, which is r1's area.
- `CAIModeInfo`: `m_Mode` (the header row's next-table link), `unk_04` (enter) and `unk_10`
  (anim id or function) are now `sptr`. gendata relocates a pointer-wide integer whose value
  is a known address. 27 runtime structs inside `.libultra` are now `RUNTIME_BSS`.
  Unresolved pointer slots went from 403 to 24, and address-holding integers from 334 to 125.
  All `*ModeTable`s are now relocated.

Merging: `include/globals/frame.h`, `include/rw/junk_types.h`,
`include/structs/game_object.h` (CAIModeInfo only, not CEngineApp/CGame), several
`include/protos/*.h` and about 880 files in `src/us/code` changed in one-token edits.
Conflicts with r1/r5 should be small and local. Rerun gendata with a fresh `--cache`,
because global and struct sizes changed.

## r5 (round 3): macOS arm64

Details: [MACOS.md](MACOS.md). On the Mac (M5, Apple clang 21, Homebrew; no new packages) the
whole port builds natively: `turok2_game` (3,917 files, 0 errors), gendata (identical output
to the box), `turok2` on LUS 2.0. Runtime and asset tests give the same hashes as Linux.
`turok2 --game` (headless) stops in `GfxPool_Alloc` (func_00425220.c:86) on a truncated `s32`
address. Linux `--high-memory` stops at the same line, and macOS cannot have a low-memory mode
(checked). A probe branch (`r5-probe`, not for merge) fixes five truncations and reaches
`CCameraPool__Construct` → `CCamera__ConstructSkySystem` (`arg0 + 0x558`), the CCamera layout
work. It also found a host-layout bug that affects Linux too: `CCache__AllocateAt` writes a
`CMemEntry` through junk_types' `B12Node` view, so `flags |= 3` lands in `lockCount`.

## Round 4, part s3: macOS parity (mode 0, then legal and the first cinema)

Details and the crash table (s3-1 to s3-19): [MACOS.md](MACOS.md) section 6. Branch
`names-R4_s3` in the Mac clone, 21 commits on 474e40af. asm_diff 0 over all 3,938 files,
checked on the box after every change.

- **macOS now runs mode 0 headless for 300 frames**: 1,414 thread switches and 151 gfx tasks,
  as on Linux. The display lists of frames 101 and 301 are command-for-command identical to
  Linux's (headless on both; only host addresses differ).
- With a local end-the-pak-check probe (not committed), the Mac goes through mode 1 (legal),
  mode 3 (expansion pak) and into mode 4 (intro cinema, level 162). It stops in the AI action
  callbacks that take the actor as `s32` (`Action_IdleEnter` -> `CScene__SetInstanceSavedBit`),
  and in `CAI__SetMode`'s N64-offset views.
- **Fixes that matter on Linux too:**
  - s3-13: the damaged-pak menu lines now draw (this is section 4 item 1).
    `MenuItem_DrawText` read `Menu.desc`, which on the host is the item's desc pointer.
  - s3-19: `CFrameData.m_pFrameBuffer` is `uptr`. Every `G_SETCIMG` used to carry a truncated
    address, and `t2sched.c`'s `frameOf` compared 32 bits.
  - s3-16: `cam + 0x53C` -> `cam->m_MenuPool` (29 sites).
  - s3-18: `CScene__LoadLevel` steps over actors by `sizeof(CGameObjectInstance)`.
- New: `T2_DLDUMP_AT=<frame>` (game.cpp) dumps a frame's display list without a window. macOS
  has no headless Metal/GL, so the Mac compares display lists, not pixels.

For the merge (files others may touch): `src/us/code/func_00413230.c` (menus: MenuPool_Open*,
MenuItem_DrawText/Layout; s2), `func_0044C230.c`, `protos/menu.h`, the 19 `+ 0x53C` files
(s5's offset sweep), `func_002718A0.c`/`func_00275A08.c` (audio, s4), `func_004263DC.c`
(pak directory, s1), `structs/world.h` (MtxPool), `structs/frame.h`, `port/game/t2sched.c`,
`port/app/game.cpp`, the 28 `EngineModeNN_*` files (s2's mode runner). `include/` changed, so
the whole tree must be rerun through asm_diff after the merge.

## Round 3, part r4: audio on the host

Details: [AUDIO.md](AUDIO.md). Behind `--audio` (off by default: runs without it are unchanged).

- Sound bank (ROM 0x1E0000) rebuilt as host libaudio structs (2 bank files, 34 instruments,
  1,115 sounds, 866 wave tables), sound data file swapped to host order (or left alone when it
  already is: r2's archive path), host audio manager (one 368-sample frame per retrace), an ABI1
  interpreter for all 16 commands, AI queue -> LUS `AudioPlayerPlayFrame` (game.cpp).
- `t2audio_test --direct K` + `tools/port/audio_compare.py`: a sound effect through
  LOADADPCM/LOADBUFF/ADPCM/RESAMPLE/ENVMIXER/INTERLEAVE/SAVEBUFF equals audio.py's VADPCM decode
  with **0** difference (sfx 0: 6,704 samples, sfx 40: 1,088).
- Game path: `Sound__Initialize` (`CAudioWorld__Initialize`) completes on the host; the first
  audio frame spins in `_sndpVoiceHandler` because `gAudioWorld.player.evtq` is corrupted
  (open, AUDIO.md §4). `turok2 --game --audio` stops at the same `CCameraPool__Construct` crash
  as without `--audio`.

| # | where | cause | fix |
|---|---|---|---|
| A1 | `alCSPNew` from `SndSeq_Construct` | `AudioStateD844`/`AudioRootD844` views: heap at N64 offset 0x1DA8 | `SndSeq_Construct` typed with `SndSeq`/`CAudioWorld` (`TARGET_PORT`), `SndSeq.csp` as `sptr`, `alCSPNew` sizes by `sizeof` |
| A2 | (heap) | `alHeapInit`/`alHeapDBAlloc` 32-bit views of `ALHeap` | host layout, `uptr` (`TARGET_PORT`) |
| A3 | `alSndpAllocate` | `PoolRecAt44`/`SlotRecAt2F` views | the 10 sound player API calls typed in port/audio/t2audio_sndp.c (files out of the link) |
| A4 | `alEvtqPostEvent` | `alEvtqNew`/`alEvtqNextEvent` 0x1C strides, s32 links | typed (`TARGET_PORT`) |
| A5 | `osRecvMesg` from `CAudioWorld__Lock` | lock at `arg0 + 0x110` | `&arg0->lock` (matches) |
| A6 | `_sndpVoiceHandler` spins | `gAudioWorld.player.evtq` overwritten with stack-like data | **open** (AUDIO.md §4) |

## Round 4, part s4: audio part 2 (the sound world typed, --audio through mode 0, --wav)

`turok2 --game --audio --frames 300` now runs mode 0 to the end (300 frames, no crash; it used
to stop in `SndQueue_CancelHandle`). The synthesizer mixes a frame every retrace; mode 0 plays
nothing itself (the volume options are still 0 until the settings are loaded), so its output is
silent. Through the game's own API a sound effect plays: with `T2_TEST_SFX=300` (debug only,
`Sound__Play2D(300)` at frame 30, volume options forced to 0xC0) the WAV has the sound
(peak 1037, rms 88 over 5 s, 197,534 non-zero samples) under gdb. Details: AUDIO.md section 8.

```sh
# the mixed output as a WAV (implies --audio) and its summary line
SDL_VIDEODRIVER=offscreen OUT/build/app/turok2 --archive OUT/turok2.o2r --rom baserom.us.z64 \
    --game --wav OUT/m0.wav --frames 300
# game: wav OUT/m0.wav: 109664 stereo frames at 22050 Hz (4.97 s), peak 0, rms 0.0, 0 non-zero samples
T2_TEST_SFX=300 ... --wav OUT/sfx.wav     # one sound effect through Sound__Play2D (debug hook)
T2_TEST_SONG=0x10 ...                     # music through Music_SetTrack: stops in the CSP (below)
```

| # | where | cause | fix |
|---|---|---|---|
| s4-1 | `SndQueue_CancelHandle` from `CAudioWorld__StopHandle` | the sound world's C is Rage Wars' (ContextRecAt2BC0, Voice, M04Emitter, NodeRecAtC8, ... views at N64 offsets, 0xCC voices) | 81 functions (CAudioWorld, SndVoice, SndVoicePool, SndQueue, SndSeq, the speech player, LoopSounds, Sound__IsPlaying) typed in `port/audio/t2audio_world.c`, originals out of the link |
| s4-2 | r4's `_sndpVoiceHandler` evtq corruption | `SpeechPlayer_Construct` allocated its sound on `CAudioWorld__GetSndPlayer(m)` = m + 0x84 (the N64 offset of the sound player: the bank pointers on the host) | typed (s4-1) |
| s4-3 | `alAudioFrame` spins | `__nextSampleTime` read the client's time at 0x10 (`ALPlayer.handler` on the host); `alSynAddPlayer`, `_timeToSamples` the same | `port/audio/t2audio_lib.c`: the synthesizer driver, voice calls (`alSynAllocVoice` helpers, `alSynSet*`, `alSynStart/StopVoice`, `alSynFreeVoice`), `alEvtqFlush*`, `_removeEvents`, `alCSPGetState`, then (s4-4..s4-7) the filter constructors, `alResampleParam`, the CSP API posts and the codebook cache (30 functions) |
| s4-4 | `alAudioFrame`: `output->setParam` = 3 | `alFilterNew` stored handler/setParam as s32 at N64 offsets; `alSaveNew`/`alSaveParam` the same | typed in t2audio_lib.c; `ALSave.dramout` is `sptr` |
| s4-5 | SAVEBUFF of 11,168 bytes over the save filter | `alFxPull`/`_doModFunc`: `&r->input[-d->input]` with a u32 delay wraps only with 32-bit pointers | `-(s32)` under `TARGET_PORT` (func_00296750.c, func_00296CD0.c) |
| s4-6 | `alAdpcmPull` -> `alResampleParam` calls a handler as setParam | `alResampleParam` (IntegerState34 view) | typed |
| s4-7 | `AudioDMA_CacheBlock`: heap NULL, entries 0x90 bytes | `gAudioHeap` set by the replaced `amCreateAudioMgr`; CacheEntry holds pointers | t2audio_mgr.c sets `gAudioHeap`; typed host cache + ageing (`T2Audio_DmaCacheAge`) |
| s4-8 | `ALParam` too small for `ALStartParamAlt` | the update casts carry a pointer at 0x20 on the host | `ALParam` gets a host pad (`TARGET_PORT`) |
| s4-9 | (silent) pitch/env halves | `FloatToHalf` took the first u16 of the float: the low half on a little-endian host | `TARGET_PORT`: the top 16 bits |
| s4-10 | (silent) `LoopSounds` paused flag past the struct | `LoopSounds` had no field at 0x28 | `m_Paused` added (structs/sound.h); `SndSource.flags` and `SndSeq.unkC` are `sptr` (pointers) |

Open (next): (1) without gdb, a playing sound effect crashes in `t2asp.c` `cmd_adpcm`
(`memcpy(ram(w1), ...)`: the ADPCM state address in the command; under gdb, no ASLR, it plays:
an address above 4 GB or a stale state pointer, look at `alAdpcmPull`/`_decodeChunk`'s
`osVirtualToPhysical` arguments first); (2) music: `Music_SetTrack` -> `SndSeq_Update` (typed)
-> `__CSPVoiceHandler` -> `__CSPSetUsptFromTempo` (func_00290D5C) crashes: the compressed
sequence player (`__CSP*`, `alCSeq*`, func_0028F1F0..func_00290D5C) is still m2c views;
(3) speech (`SpeechPlayer_Update` state 1, the adec stream) is stubbed: it ends the request and
says so once.

For the merge: new files `port/audio/t2audio_world.c`, `port/audio/t2audio_lib.c`; edits in
`port/cmake/link.cmake` (112 files added to `T2_LINK_REPLACED`), `port/app/CMakeLists.txt`
(t2audio sources), `port/app/main.cpp` / `game.h` / `game.cpp` (`--wav`, the T2_TEST_* hooks:
s2 also adds a flag in main.cpp), `port/audio/t2audio_mgr.c`, `port/audio/t2audio_sndp.c`
(`alSndpSetPriority`), `include/structs/sound.h`, `include/structs/libaudio.h`, three src files.
Drop OUT/decls.json (header changes). asm_diff 0 (see the commit).

## Round 5, part t4: audio part 3 (sfx without gdb, music, speech)

Details, crash table t4-1..6 and verification commands: [AUDIO.md](AUDIO.md) sections 9-11.

`turok2 --game --draw --pak-blank --audio --wav boot.wav --frames 5400` runs the boot to the
title screen (mode 7 at frame 4214, as without `--audio`) with sound: the first cinema from 26 s,
music song 22 over the second cinema, song 16 (the title theme) from 70 s; WAV 90 s, peak
20,820, rms 2,088, no clipping. Every note on of songs 22 and 16 matches the ROM sequence in
time (0 us error), key and channel (`tools/port/audio_notes_check.py`, 510 and 251 notes); the
ADPCM voice path is bit-exact against tools/assets/audio.py; speech streams decode with 0
difference against audio.py (`t2audio_test --adec`) and play in game (`T2_TEST_SPEECH=311`:
correlation 0.986 with the reference stream).

| # | fix |
|---|---|
| t4-1 | `osVirtualToPhysical` names any host address (region handle table, `T2Port_PhysToVirtual`); `_decodeChunk` used K0_TO_PHYS (`& 0x1FFFFFFF`) for the ADPCM state: the sfx crash without gdb |
| t4-2 | the compressed sequence player, cseq.c and the shared seqplayer.c helpers typed (port/audio/t2audio_csp.c, 31 functions, their game files out of the link) |
| t4-3 | **`gWarmBoot = 1` in `T2Port_Boot`**: `CEngineApp__Construct` runs `GameSettings_SetDefaults` only then (on the N64: power-on). Every setting was 0 before (music/sfx volume, difficulty 0 instead of 0x10, HUD alpha, multiplayer rules) |
| t4-4 | the resident wave data copy is the whole ROM (it was taken before `gAudioRomBase` was set: every wave decoded garbage) |
| t4-5 | `A_ADPCM` writes the previous frame before the new ones, as the microcode does |
| t4-6 | the audio start-up runs above 4 GB (`--high-memory`): heap/block/source pointers pointer-wide in `alHeapInit`, `alLoadNew`, `alResampleNew`, `alFxNew`, `alMainBusNew`, `alAuxBusNew` |
| t4-7 | speech: port/audio/t2audio_adec.c (the adec decoder from audio.py, `alAdecPull`), `SpeechPlayer_Update` as func_002768D0 with a host wave table |

For the merge: t4-3 changes the game state of every run, not only audio: the other parts' runs
(t1's demo and new game, t3's cinemas) start from the default settings after it. Files others
may touch: port/runtime/t2boot.c (one line and a comment), port/runtime/t2ultra.c
(`osVirtualToPhysical`), port/runtime/t2port.h, port/app/game.cpp (the `T2_TEST_SPEECH` hook),
port/app/CMakeLists.txt (two sources in t2audio), port/cmake/link.cmake (36 files added to the
exclusion list, in two blocks before the scheduler's). src/ (asm_diff 0 against 98574f3c):
func_0029FCF4.c, func_002912F0.c, func_00295304.c, func_0029546C.c, func_00294E70.c,
func_002953B4.c, func_00295410.c. `--high-memory` on Linux still stops in `CHeap__Free` from
the decompressor, with or without `--audio` (not audio).
