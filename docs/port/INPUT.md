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

**Open (saves):** the note data is whatever the game writes: its save records in host order
today. Notes written by the port and by an emulator are not interchangeable until the save
records get a swap plan (like the archive's `t2/swap`). The pak's filesystem itself is
compatible both ways.

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
| `--input BUTTON@FRAME[,...]` | **debug**: scripted presses on port 1 for headless runs, each held 4 host frames. Buttons: `A B Z START DU DD DL DR L R CU CD CL CR` |

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

- Save records: a swap plan for the notes Turok 2 writes (profile, save game), so .mpk saves
  move between the port and emulators; then test save/load through the game's menus.
- Rumble: a Rumble Pak mode (answer the 0x8000 probe with 0x80, route `osMotorStart/Stop` to
  LUS's rumble) instead of "not present".
- Ports 2-4 with paks (`pakPorts` is there; no flag yet), multi-bank paks (not needed: Turok 2
  formats 1-bank paks).
- `--input` is a bring-up aid; real input is LUS's ControlDeck (keyboard/gamepad mapping in
  `turok2.cfg.json`).
