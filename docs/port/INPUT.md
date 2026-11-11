# Input: controllers and the Controller Pak on the host

Round 4, part s1. Code: `port/runtime/t2pif.c` (the virtual PIF), `port/runtime/t2ultra.c`
(controllers), `port/app/game.cpp` (LUS ControlDeck hooks, scripted input, flags).

## 1. What runs where

| layer | on the host |
|---|---|
| `osContInit`, `osContStartQuery/GetQuery`, `osContStartReadData/GetReadData`, `osContReset` | `t2ultra.c` (the game's libultra files are out of the link). Pads come from LUS's `ControlDeck` through `T2PortContHooks` (keyboard = port 1). Each start posts `OS_EVENT_SI`. The query status byte says `CONT_CARD_ON` when a pak is in the slot. |
| `osMotorInit/Start/Stop` | `t2ultra.c`: `PFS_ERR_NOPACK` (no rumble pak) |
| `osPfsInitPak`, `osPfsIsPlug`, `osPfsChecker`, `osPfsFindFile`, `osPfsReadWriteFile`, `osPfsAllocateFile`, `osPfsDeleteFile`, `osPfsFileState`, `osPfsFreeBlocks`, `osPfsRepairId`, ... (and the module-2 `_2` copies) | **the game's own C, unchanged**: they build joybus commands in `__osPfsPifRam` and call `__osSiRawStartDma` |
| `__osSiRawStartDma` | `t2pif.c`: the PIF. A write DMA copies the 64 bytes in and runs every channel's command; a read DMA copies the answers back; both post `OS_EVENT_SI` |

Joybus commands the PIF answers (channels 0-3; a channel without a connected controller gets the
"no response" bit 0x80 in its rx byte):

- `0x00` status / `0xFF` reset: type `05 00`, status bit 0 = pak in the slot;
- `0x01` read buttons: zeros (the game reads pads through `osContGetReadData`, not this path);
- `0x02` pak read: 32 bytes at `address & 0xFFE0` + data CRC;
- `0x03` pak write: 32 bytes, answers the data CRC.

The address CRC is checked (a mismatch is logged once). Addresses >= 0x8000 (bank select and
the rumble probe) read zeros and ignore writes, as a 32 KB pak does. With no pak in the slot the
controller still answers, with the data CRC inverted, so libultra asks the status and returns
`PFS_ERR_NOPACK`, as on hardware. A stray byte after the last command (libultra's read/write
packets are 0x28 bytes with an uninitialized last byte) is not taken as a command: a command
that would run past the PIF RAM ends the parse.

## 2. The pak file

One emulator-standard raw 32 KB image per port, big-endian (the console's layout):
`<pak dir>/turok2_pak<N>.mpk`, N = 1..4. Default pak dir: `$XDG_DATA_HOME/turok2` or
`~/.local/share/turok2` (Linux), `~/Library/Application Support/turok2` (macOS); never the repo.
A missing file is a blank formatted 1-bank pak (label, the pack ID in blocks 1/3/4/6 with
deviceid 1, inode table and mirror with every page free, empty directory); the file is created
only when the game writes a block that differs. Every changed block goes to the file at once.

Only port 1 has a pak by default (`T2PortConfig.pakPorts`, bit per port; 0 = port 1).

### Byte order

The game's pfs C reads the pak's structured blocks with native loads: the pack ID's u16 checksum
words and `deviceid`, the directory's `game_code`/`company_code`/`data_sum`, and the inode
units both as bytes (`inode_t.bank/.page`) and as a u16 (`ipage`). So the PIF converts those
blocks as they cross the DMA, and the game's C runs unchanged on a standard file:

- ID blocks 1, 3, 4, 6: every `__OSPackId` field swapped; the checksum pair is recomputed in the
  destination byte order when it was valid in the source order (a damaged ID stays damaged);
- blocks 8-23 (pages 1-2, inode table and mirror): each `__OSInodeUnit` swapped as a u16. On a
  little-endian host `include/ultra_types.h` orders the unit `{page, bank}` (TARGET_PORT only),
  so `inode_t.bank`/`.page` and `ipage` agree as on the console;
- blocks 24-39 (pages 3-4, the 16 `__OSDir`): `game_code`, `company_code`, `start_page`,
  `data_sum` swapped;
- everything else (label, note pages) is bytes.

These are 1-bank paks (the standard 32 KB pak); the block numbers above assume it.

**Saves (round 8):** the note data is big-endian too now: the save game and the profile are
swapped field by field as they cross `Pak_SaveNoteVerified`/`Pak_ReadNote` (section 7), so .mpk
files move between the port and emulators both ways.

Game-side fixes that the PIF needed (`TARGET_PORT` only, asm_diff 0):
`__osPackRamReadData`/`__osPackRamWriteData` stored the address word with a u16 store (low byte
first on the host): now high byte first; `__osContRamRead`/`__osContRamWrite` took the queue as
`s32` (truncates on LP64): `OSMesgQueue *`; `__osPfsSelectBank(_2)` was an m2c view reading
queue/channel at N64 offsets: typed on `OSPfs`.

## 3. Flags

| flag | effect |
|---|---|
| (none) | port 1 has the pak `turok2_pak1.mpk` in the user data dir |
| `--pak-dir DIR` | the .mpk files live in DIR |
| `--pak-blank` | a blank formatted pak in memory, never saved (tests, headless runs) |
| `--no-pak` | no pak in any slot |
| `--input SPEC` | **debug**: scripted input on port 1 for headless runs (section 6). `BUTTON@F` (held 4 frames), `BUTTON@F1..F2`, `SX=V@F1..F2` / `SY=V@...` (stick), `@FILE` |

Env: `T2_PIF_LOG=1` logs every pak block read/write (host-order view); `T2_MENU_LOG=1` logs the
newest open menu (desc address and cursor: resolve the address with `nm`); the run always logs
engine mode changes and, at the end, `PIF n SI DMAs, n status, n pak reads, n pak writes`.

## 4. Results (Linux, the box, headless)

| case | what happens |
|---|---|
| `--pak-blank` (or a missing .mpk) | mode 0 mounts the pak (236 block reads, the mirror rewritten with identical bytes), no menu; frame 297 mode 1 (legal screen), frame 1069 mode 3 ("EXPANSION PAK FOUND!", the 3D pak model drawn), frame 1357 mode 4 (cinema): crash there (STATUS.md) |
| `--no-pak` | the pak menu opens at frame ~120; any button (`--input A@300`) -> frame 357 mode 1 |
| a zeroed .mpk (damaged) | `gDamagedPakAtBootMenuDesc`; "attempt repair" (`--input A@300,DD@400,A@450`) -> `gPakConfirmRepairDesc` yes -> the game's `osPfsRepairId` + re-init -> `gPakMsgRepairedDesc`; OK (`A@550`) -> `gPakSaveGameAnyPakMenuDesc`; "continue without saving" (`DD@650,A@700`) -> frame 759 mode 1. The written file passes `mpkcheck`-style checks in big-endian: 4 valid ID copies (deviceid 1, 1 bank), inode checksums 0x71 |

Before the repair worked, `PakConfirmMenu_SelectYes/SelectNo` crashed: their menu views
(`MenuRecAt24`, a local `Item`) put `player`/`slot`/`prompt` at N64 offset 0x1C; on the host they
lead with the host `Menu` fields now (TARGET_PORT).

## 5. Next

- Saves: done in round 8 (section 7); still to do: load a save made on a console or an emulator.
- Rumble: a Rumble Pak mode (answer the 0x8000 probe with 0x80, route `osMotorStart/Stop` to
  LUS's rumble) instead of "not present".
- Ports 2-4 with paks (`pakPorts` is there; no flag yet), multi-bank paks (not needed: Turok 2
  formats 1-bank paks).
- `--input` is a bring-up aid; real input is LUS's ControlDeck (keyboard/gamepad mapping in
  `turok2.cfg.json`).

## 6. Scripted input (round 6, u3)

`--input` (`port/app/game.cpp`) takes entries separated by commas or whitespace, or `@FILE`
(one or more entries per line, `#` comments):

| entry | effect |
|---|---|
| `BUTTON@F` | press BUTTON for 4 host frames from frame F |
| `BUTTON@F1..F2` | hold BUTTON from F1 to F2 inclusive |
| `SX=V@F1..F2`, `SY=V@F1..F2` | stick X/Y = V (-128..127; X + = right, Y + = up) over the range; `SX=V@F` for 4 frames; overlapping stick entries: the last wins |

Buttons: `A B Z START DU DD DL DR L R CU CD CL CR` (the N64 bits). The stick goes into the
6-byte pad record on both paths (the LUS ControlDeck path when a window exists, and the
`--no-window` virtual pad). A script file for a new game and a walk:

```
START@4400 A@4500 A@4700 A@4800     # title -> main menu -> single player -> new game -> OK
CU@13000..13440                     # level 51 from frame 12903: walk forward (C-up)
SX=80@13450..13474                  # turn right about 90 degrees
CU@13480..14000
A@14050..14090 SX=80@14060..14090   # hold A (the weapon wheel), stick right: weapon 2
Z@14150..14250                      # fire
```

A journey that also fires (the bow from the wheel, two arrows on the dock; round 7, v2) is in
[GAMEPLAY.md](GAMEPLAY.md) section 6: the bow is drawn while Z is held and fires on release;
the land weapons do not fire in the water.

Turok 2's default controls as the port sees them (control type 0, `m_WeaponSelect` 1, the
eight-slot stick wheel): **stick** turns (X, about 3.8 rad per second at 80) and looks (Y);
**C-up/C-down** forward/back, **C-left/C-right** strafe; **R** jumps (pressed bit 0x10:
`PlayerJump`, state 4); **Z** fires; **A** / **B** hold the weapon wheel (two button groups;
the stick picks the slot, 8 directions, 0 = up, clockwise, dead zone 60); **L** is the map.
The game folds the stick into its own button word too (`m_dwButtons` 0x10000 up, 0x40000
right, 0x20000 left), as the state log shows. Stick 80 reads as 77 after the game's dead zone.

## 7. Save notes in the console's byte order (round 8, w5)

The pak's filesystem was already the console's (section 2); now the **note data** is too, so a
.mpk written by the port loads on an emulator or a console with a pak, and the other way round.
Code: `port/game/t2save.c`, called from `Pak_SaveNoteVerified` (func_00426164, the only note
writer) and `Pak_ReadNote` (func_004265B0) under TARGET_PORT.

Turok 2 writes two records (game code `NT2E`, company `51`):

| note | record | layout | checksum |
|---|---|---|---|
| "TUROK 2" (ext A, B...) | the save game, `SaveGame` (structs/save.h): size, level-flags size, the play clock (4 f32), `CPlayerLoadSaveData`, the first 0x184 bytes of `GameSettings`, the level's persistent bit sections, a checksum | 0x688 + the level flags (0x5310 in level 51: 90 pages) | `Crc32_Compute(save, size - 4, 0xB)` in the last word |
| "T2 MULTI" | `PakProfile` (structs/pak.h): 4 u16 counts, character, name | 0x18 | `Crc32_Compute(p, 0x14, 4)` at 0x14 |

The swap plan (`t2save.c`): the header words; in `CPlayerLoadSaveData` the s32/s16 fields
(health, life force, lives, warps, ammo list, weapons, Adon and totem/boss flags, the intro and
first-time masks; the 1024 pickup counts and the nuke/key bytes stay bytes); in the settings the
cheat/difficulty words, the HUD alpha u16, the speed f32 and the 20 u16 counts of each of the
five `CPlayerOptions`; the level flags by their ISet structure (`swap_spec.json` "load/save
persistent bit sections": the ISet headers and each section's u32 bit count; the flag bits are
bytes). The archive swapped those ISet words to host order when the level loaded, so the note
swaps them back.

- **Write**: the record (host order, its checksum computed by the game in host order) is copied,
  swapped and its checksum recomputed over the big-endian bytes, stored big-endian (what the
  console computes). The verify read-back compares that image.
- **Read**: when the big-endian checksum holds the record is swapped to host order and its
  checksum recomputed in host order, so `PakMenu_SelectNote`'s checks and every reader run
  unchanged. A note whose host-order checksum holds (written by a port build before round 8) is
  taken as it is; anything else is left alone (the game calls it damaged).
- `Pak_FindNote` now spells the note's game/company codes high byte first (the console copies
  the bytes of its big-endian integers): before, no Turok 2 note was recognised on the host.

`python3 tools/port/mpk_saves.py PAK.mpk [--dump-save out.bin]` lists the notes and checks them
the console's way (standard CRC-32 with the game's seeds: `zlib.crc32(data, seed)`), decodes the
save's health, lives, warps, clock and walks the level-flag ISets; exit 1 if a Turok 2 note does
not check big-endian. A save written by the port:

```
note  1: NT2E 51 "TUROK 2" ext "A" 90 pages: big-endian, 22936 bytes, checksum OK; health 100
lives 3 life force 0 weapon 0 difficulty 16 checkpoint warp 1000 save warp 0 clock 0:02:20;
level flags: 663 ISets
```

Not tested here: a save from a real console or an emulator (none at hand); the checker reads
port saves the console's way, which is the same code path in reverse.

Scripted save and load (GAMEPLAY.md section 9): `T2_SAVE_PORTAL=13100 --pak-dir DIR --input
"START@4400 A@4500 A@4700 A@4800 A@13250 A@13400"`, then boot again with the same DIR and
`--input "START@4400 A@4500 DD@4700 A@4800 DU@4900 A@5000 A@5200 A@5400"`.
