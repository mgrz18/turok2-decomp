# Turok 2 assets (Phase 6)

Every byte of the asset region of the US ROM (`0x1E0000..0x1FF7AEC`, 31,554,284 bytes) is
mapped: the C says where each block is read, how it is decompressed and what parses it, and
`tools/assets/` turns all of it into ordinary files. Nothing extracted is ever committed: the
tools read the player's own ROM (SHA1 `fb0400f21e3f043939ab56500c7b12a3231006f1`).

| Document | Covers |
|----------|--------|
| [ROMFS.md](ROMFS.md) | The region map, the container formats (ISet / USet / RNC), the cartridge index tree, every `CCache__Request` call site, load units and their readers. Tools: `romfs.py`, `rnc.py`, `cache_calls.py`. |
| [TEXTURES.md](TEXTURES.md) | Texture banks (CI8/CI4 with RGBA16/IA16 palettes, RGBA32, mip chains; animated frames and palettes) and graphic sets (HUD, fonts, screens: I4 and RGBA5551 + I4 mask). Tool: `textures.py`. |
| [MODELS.md](MODELS.md) | Object geometry (`Model_Decompress`): LODs, vertex lists, packed command streams, materials, UVs, skeleton block. Tool: `models.py`. |
| [ANIMATION.md](ANIMATION.md) | Anim object info, skeletons, the three track encodings of the animations. Tool: `anim.py`. |
| [LEVELS.md](LEVELS.md) | Levels (header, collision regions, paths, instances, grid bounds and sections, sky layers, links, events), warp destinations, cinemas. Tools: `levels.py`, `cinemas.py`. |
| [AUDIO.md](AUDIO.md) | The two libaudio banks (VADPCM), the sound tree file (effects, envelopes, songs: alCSeq MIDI and Acclaim `adec` MDCT streams). Tool: `audio.py`. |
| [PORT.md](PORT.md) | How the native port gets these assets without shipping them: archive format, engine hooks, extractor plan. Tool: `romid.py`. |
| [ARCHIVE.md](ARCHIVE.md) | The game archive `turok2.o2r` as built: entries, unit layout (libultraship Blob), manifest and flags, decisions (RNC unpacked, no containers with RNC children, partial reads), determinism, verification. Tools: `mkarchive.py`, `archive_check.py`, `romfs.py --manifest`. |

## The four regions

The engine reaches every asset from four ROM addresses hard-coded in the C; everything else
is computed from them through offset tables (`CISet__GetROMAddress`). These are also the
splat segments in `versions/turok2.us.yaml` (all `bin`).

| ROM | Size | Segment | What | Reached by |
|-----|------|---------|------|------------|
| 0x01E0000 | 0x2F194 | `audio_ctl` | Audio control block: the music and sfx `ALBankFile`s, pointers pre-relocated to 0x480000 | `CAudioWorld__Initialize`: `VM_Alloc(0x2F194, 0, 0x1E0000)` |
| 0x020F194 | 0x9D2680 | `audio_wavetable` | VADPCM sample data (`ALWaveTable.base` is relative to it) | `gAudioRomBase = 0x20F194`, audio DMA callback |
| 0x0BE1814 | 0x3EA7A0 | `sound_data` | Sound tree file (ISet): effects, envelopes, 159 songs | `CAudioWorld__LoadSfxDataFile`: `VM_Alloc(0x3EA7A0, 0, 0xBE1814)` |
| 0x0FCBFB4 | 0x102BB38 | `cartridge` | Cartridge index: ISet of 21, every other asset | `m_CartridgeIndex = 0xFCBFB4` (`CEngineApp__Main`) -> `CScene__Construct` |
| 0x1FF7AEC | | `trailer` | 0xFF padding to 32 MB | |

What older notes called `assets_index` (a "913-entry monotonic offset table" at 0x20E334) is
the sfx instrument's `soundArray` inside the audio control block.

## The cartridge index (ROM 0xFCBFB4)

| # | ROM | Bytes | Entry | Contents | Doc / tool |
|---|-----|-------|-------|----------|------------|
| 0 | 0x0FCC014 | 7,952,552 | objects index | 2705 objects, each an ISet of 4: anim object info, geometry (RNC), animations index (1857 RNC animations over 490 animated objects), static events | MODELS, ANIMATION / `models.py`, `anim.py` |
| 1 | 0x17618BC | 15,624 | intelligence | RNC; AI parameters | ROMFS (not exported) |
| 2 | 0x17655C4 | 5,424 | object types | USet of u16 type ids, one per object | MODELS / `models.py` |
| 3 | 0x1766AF4 | 3,976,352 | texture sets index | 1839 texture banks (RNC) | TEXTURES / `textures.py` |
| 4 | 0x1B31794 | 24,616 | particle effects | RNC | ROMFS (not exported) |
| 5 | 0x1B377BC | 4,452,880 | levels index | 165 levels, each an ISet of 9 (6916 grid sections in all) | LEVELS / `levels.py` |
| 6 | 0x1F769CC | 21,264 | load/save persistent bit sections | per-level defaults (`CLoader__LoadSync`) | ROMFS (not exported) |
| 7 | 0x1F7BCDC | 19,360 | game persistent bit sections | per-level defaults (`CLoader__LoadSync`) | ROMFS (not exported) |
| 8 | 0x1F8087C | 7,184 | warp dests | sorted warp ids + `CWarpPoint`s | LEVELS / `levels.py` |
| 9 | 0x1F8248C | 8 | binaries index | empty in the US ROM | ROMFS |
| 10 | 0x1F82494 | 8 | object types 2 | empty in the US ROM | ROMFS |
| 11 | 0x1F8249C | 8,128 | particle swooshes | 91 raw records | ROMFS (not exported) |
| 12 | 0x1F8445C | 152 | dynamic lights | | ROMFS (not exported) |
| 13 | 0x1F844F4 | 128 | curves | RNC | ROMFS (not exported) |
| 14 | 0x1F84574 | 56 | rumbles | RNC | ROMFS (not exported) |
| 15 | 0x1F845AC | 48 | rumble types | RNC | ROMFS (not exported) |
| 16 | 0x1F845DC | 367,384 | graphicset index | 114 graphic sets (RNC): HUD, fonts, screens | TEXTURES / `textures.py` |
| 17 | 0x1FDE0F4 | 464 | graphicset type | graphic set id table | TEXTURES / `textures.py` |
| 18 | 0x1FDE2C4 | 104,056 | Cinemas index | cinema table + 197 cinemas | LEVELS / `cinemas.py` |
| 19 | 0x1FF793C | 424 | link group types | | ROMFS (not exported) |
| 20 | 0x1FF7AE4 | 8 | end marker | empty set | ROMFS |

## Running the extractor

```sh
python tools/assets/extract_all.py baserom.us.z64 --out assets_out
#   [--jobs N] [--only audio,anim,models,levels,textures,romfs,cinemas,romid]
#   [--no-streams] [--no-png]
```

It first runs `romid.py`: .v64 / .n64 images are normalised, any image other than the US
release is refused (exit 1). Then each area's exporter runs as its own process, in parallel
(each also works alone: `python tools/assets/<tool>.py ROM --out DIR`, see `--help`):

| Area | Output | Count (US ROM) |
|------|--------|----------------|
| textures | `textures/textures/NNNN.png` (animation steps side by side), `graphics/III_idNNNN/FFF.png`, JSON | 1839 banks, 114 graphic sets / 600 frames |
| models | `models/objNNNN.obj/.mtl/.gltf/.bin`, `index.json`; animated models posed at frame 0 of animation 0 (`--pose`) | 2705 models (2215 static, 490 animated) |
| anim | `anim/objNNNN.json/.gltf/.bin`, `index.json` | 490 skeletons, 1857 animations |
| levels | `levels/levels/NNN_name.json`, `levels/maps/*.png`, `warp_dests.json` | 165 levels, 6916 grid sections |
| cinemas | `cinemas/cinemas/NNN_name.json` | 197 cinemas |
| audio | `banks.json`, `soundtree.json`, `music_waves/`, `sfx_waves/` (WAV), `sequences/` (MIDI), `streams/` (WAV) | 188 + 678 waves, 1319 effects, 19 sequences, 140 streams |
| romfs | `romfs_tree.tsv/.json` (40,671 nodes), `romfs_types.tsv` | |
| romid | `romid.json` (offsets and hashes only) | |

Reference run (32-core box): 101 s wall clock (the audio streams are the long pole; the rest
finish within 70 s), 16,212 files, 338 MB. `assets_out/`, `*.o2r` and `*.otr` are in
`.gitignore`.

## Coverage

Every ROM-level node of the four regions, classified by area (`extract_all.py` prints this
table and writes it to `summary.json`):

| Area | Bytes | % | Exported by |
|------|------:|--:|-------------|
| audio: wave table | 10,299,008 | 32.64 | audio |
| levels (cart 5) | 4,415,056 | 13.99 | levels |
| animation: animations | 4,127,952 | 13.08 | anim |
| audio: sound data file | 4,078,408 | 12.93 | audio |
| textures: texture banks | 3,968,984 | 12.58 | textures |
| models: geometry | 3,607,320 | 11.43 | models |
| textures: graphic sets + types | 367,384 | 1.16 | textures |
| index structure (ISet headers + padding) | 241,320 | 0.76 | romfs |
| audio: control block | 192,916 | 0.61 | audio |
| cinemas | 86,280 | 0.27 | cinemas |
| animation: anim object info | 63,200 | 0.20 | anim |
| levels: warp dests | 7,168 | 0.02 | levels |
| models: object types | 5,424 | 0.02 | models |
| game data without an exporter yet: particle effects, intelligence, persistent bits, particle swooshes, static events, link group types, dynamic lights, curves, rumbles, empty sets | 93,864 | 0.30 | - |
| **total** | **31,554,284** | **100.00** classified, **99.70** exported | |

## Open questions (collected from the area docs)

- Formats only partly named: anim object info 0x1E and bone bytes 0x66/0x68; geometry block 1
  ("data") and blocks 4/5; object block 3 (static events); material `unk4`; level header
  bytes 4..9 / 0x14..0x17, collision table 3, light volume fields, `LevelEvent` first word;
  the sound tree's effect `cue` block and t0/t1; song header +4/+6.
- Cross-area naming: which material, actor or menu uses which texture set / graphic set id;
  song keys and effect indices to names (Music_SetTrack call sites, level and cinema data);
  action ids outside the enemy tables.
- Graphic set type index 14 and 15 both carry id 157 (only one is reachable by the search).
- Units of the cinema spline knots (arc length or frames).
- No exporter yet for the small game-data entries (intelligence, particles, rumbles, curves,
  dynamic lights, link groups, persistent bits): 0.3% of the region.
- Port: whether any decompress callback touches a block before `ProPackDecompress`, raw
  containers with RNC children (`KEEP_PACKED` or offset rewrite), `Bits_Read` fetch width
  (swap rule for animation streams), libaudio banks pointer-fixed in place (a 64-bit audio
  problem for Phase 7).

## Port plan (summary of [PORT.md](PORT.md))

- Turok 2 has no linked-in asset symbols: the engine is a cache over ROM addresses
  (`CCache__Request(key = ROM address, size, ...)`), so the archive is keyed by ROM address.
- Use libultraship. `turok2.o2r` is generated on first run from the player's ROM: one entry
  per load unit (26,124 units, from `romfs.load_units()`), RNC streams stored unpacked, big
  endian kept; byte swapping per type in one `T2Asset_Read`.
- Engine change: `CLoader__LoadSync`, `CLoader__LoadAsync` / `CLoader__Main`, `VM_Alloc` and
  the size in `CCache__Request` read from the archive instead of the cartridge.
- Done: `romfs.py --manifest`, `mkarchive.py` + `archive_check.py` ([ARCHIVE.md](ARCHIVE.md)).
- Next steps: per-type field-width tables (BYTEORDER.md), the port-side reader
  (`port/assets/t2asset.c`), the Torch `T2` module for distribution.
