# The port's game archive (`turok2.o2r`)

Tools: `tools/assets/mkarchive.py` (builder, Python reference), `tools/assets/archive_check.py`
(checks an archive against the ROM), `tools/assets/romfs.py --manifest` (the table of
contents on its own). Design background: [PORT.md](PORT.md) (option C: one archive entry per
engine load unit, keyed by ROM address); the unit census: [ROMFS.md](ROMFS.md).

```
python tools/assets/mkarchive.py baserom.us.z64 -o turok2.o2r [--folder DIR] [--store] [--level N] [-j N]
python tools/assets/archive_check.py turok2.o2r baserom.us.z64 [--folder DIR]
python tools/assets/romfs.py baserom.us.z64 --manifest units.tsv      # or units.json
```

`mkarchive.py` runs `romid.identify` first: it accepts `.z64`, `.v64` and `.n64` images
(normalized to big-endian; all three give the same archive) and refuses any image whose
SHA1 is not the US release (`fb0400f2...`). `--folder` also writes the same tree as plain
files (libultraship `FolderArchive`, handy for development and for mods). Archives and
folders are game data: write them outside the repository (`*.o2r` is in `.gitignore`).

## Numbers (US ROM)

| | |
|---|---|
| entries | 26,129 = 26,124 units + 5 metadata files |
| units | 14,426 RNC streams stored unpacked (14,261 method 1, 165 method 2), 11,698 stored as in the ROM |
| unit bytes | 45,890,836 (ROM side: 31,554,284 bytes read from the ROM) |
| archive | 33,875,898 bytes (deflate level 6); 33,805,951 at `--level 9`; 53,795,943 with `--store` |
| build time | 7.5-9.5 s with `-j 8` (22-23 s with `-j 1`); RNC unpacking is the only real work |
| check time | 32 s (`archive_check.py` unpacks every RNC stream again) |

Units by kind: 14,261 `rnc1`, 165 `rnc2`, 6,437 `iset`, 4,885 `empty`, 332 `raw`, 41 `uset`,
3 `audio-region`. Flags: 14,426 `RNC_UNPACKED`, 5,747 `INDEX_HEADER`, 2,906 `CONTAINER`
(2 of them also `LOADSYNC`), 2 `VM_WINDOW`, 1 `RAW_RANGE`, 1 `NO_READER`, 0 `RNC_CHILDREN`.
46 types.

## Entries

| path | content |
|---|---|
| `version` | libultraship's version file: `u8 byteOrder (0 = little)`, `u32 gameVersion` = ROM header CRC1 (`0x49088A11`). Then a T2 extension libultraship does not read: `'T2AV'`, `u16 major`, `u16 minor` (archive format), 20-byte SHA1 of the (z64) ROM. |
| `t2/info.json` | ROM name, SHA1, tool, format version, unit / type / byte counts, zlib version. For people; the port reads `version` and `t2/units`. |
| `t2/units` | binary table of contents, little-endian: `'T2UT'`, `u16 major`, `u16 minor`, `u32 count`, `u32 typeCount`, then `count` records `{u32 key, u32 romSize, u32 hostSize, u16 type, u16 flags}` sorted by key (16 bytes each). |
| `t2/units.tsv` | the same as text (`romfs.manifest_tsv`): key, size, host size, kind, type, flags, flag names, CCache descriptions, decompress callbacks, romfs path. |
| `t2/types` | `id \t type \t descriptions` per line; type ids are the index in the sorted list of type names. |
| `t2/u/XXXXXXXX` | one load unit; `XXXXXXXX` = ROM key, 8 uppercase hex digits (`t2/u/00FCEA9C` = object 0's geometry). |

Unit entry layout (all header fields little-endian; the unit bytes after them are not):

| offset | size | field |
|---|---|---|
| 0x00 | 1 | byte order of the header fields: 0 (`Ship::Endianness::Little`) |
| 0x01 | 1 | is custom: 0 |
| 0x02 | 2 | unused |
| 0x04 | 4 | resource type `0x4F424C42` (`Ship::ResourceType::Blob`, "OBLB") |
| 0x08 | 4 | resource version 0 (`ResourceFactoryBinaryBlobV0`) |
| 0x0C | 8 | id = ROM key |
| 0x14 | 4 | `'T2LU'` (start of the T2 block; libultraship stops reading the header at 0x14) |
| 0x18 | 4 | ROM key |
| 0x1C | 4 | ROM size (the slot: what the engine's `CCache__Request` asks for) |
| 0x20 | 4 | host size (bytes that follow) |
| 0x24 | 2 | type id (`t2/types`) |
| 0x26 | 2 | flags (below) |
| 0x28 | 4 | archive format major, minor (u16 each) |
| 0x2C | 20 | zero |
| 0x40 | 4 | data size (= host size): the Blob factory's `u32` |
| 0x44 | n | the unit's bytes: ROM byte order, RNC streams unpacked |

Flags (`romfs.UF_*`, also in `t2/units`):

| bit | name | meaning |
|---|---|---|
| 0x001 | `RNC_UNPACKED` | one RNC stream in the ROM; stored unpacked (`romSize` = slot, `hostSize` = unpacked size) |
| 0x002 | `INDEX_HEADER` | header of an index set (`count*4 + 15 & ~7` bytes); its entries are units of their own. Read with prefix probes (4 bytes, then the header) |
| 0x004 | `CONTAINER` | raw ISet read whole; the engine parses its entries in RAM (`CISet__GetBlock`) |
| 0x008 | `RAW_RANGE` | read piecemeal at inner addresses (the audio wave table: `__amDMA`, 0x300-byte windows) |
| 0x010 | `VM_WINDOW` | mapped by `VM_Alloc` and paged in 4 KB pages (audio control block, sound data file) |
| 0x020 | `LOADSYNC` | read by `CLoader__LoadSync` straight into a global (persistent defaults, cartridge 6 / 7) |
| 0x040 | `NO_READER` | no code reads it (cartridge entry 20) |
| 0x080 | `RNC_CHILDREN` | container whose entries are RNC streams: none in the US ROM; the builder refuses one |

## How libultraship reads it (and why Blob)

Studied in Kenix3/libultraship at `759b134` (2026-09-02):

- `O2rArchive` is libzip over the file: `Open()` indexes every non-directory entry by
  `CRC64(path)`, `LoadFile(path)` returns a `File` whose buffer is the whole entry (entries
  of size 0 are treated as missing; ours are never smaller than 0x44 bytes). Stored and
  deflated entries both work.
- `Archive::Load()` reads two reserved names: `version` (`u8` byte order, then `u32` game
  version; checked against `ArchiveManager`'s valid game versions when the port registers
  any) and `manifest.json` (archive metadata; when present libultraship validates a blake2b
  checksum over the files). We write `version` only; the unit table is `t2/units`, so the
  reserved `manifest.json` name stays free for a later signed/described archive.
- A resource file starts with the 64-byte header (`ReadResourceInitDataBinary` reads byte
  order, is-custom, two unused bytes, `u32` type, `u32` version, `u64` id: bytes 0x00-0x13;
  the rest is "reserved"). The factory for `Blob` (`ResourceFactoryBinaryBlobV0`) reads
  `u32 dataSize` and `dataSize` bytes and appends 16 zero bytes (padding for code that
  over-reads).

So every unit is a valid **Blob** resource and needs no custom factory:
`ResourceGetDataByName("t2/u/00FCEA9C")` returns the unit's bytes. Two caveats, both handled
by reading `t2/units`: `ResourceGetSizeByName` returns `dataSize + 16` (the padding), and
`ResourceManager` keeps its own cached copy of every loaded resource. Because the engine's
`CCache` is the real cache, the port's `T2Asset_Read` should rather call
`ArchiveManager::LoadFile(path)` (raw file, no resource cache), skip 0x44 bytes and copy
`hostSize` bytes into the cache block. Both routes see the same bytes. The T2 block sits in the
header's reserved bytes, which libultraship never reads, so a unit is self-describing (key,
sizes, type, flags) for tools and for mods without the table.

Why not a custom resource type (`'T2LU'` in PORT.md 5.1): it would need a factory
registered in the port just to return bytes the Blob factory already returns, and tools
that list or extract O2R contents already understand Blob. Why not header-less raw files:
`ResourceLoad` on one would fail (libultraship would treat byte 0 as a byte order), and
`FolderArchive`/mod tooling expects resource headers.

## Decisions

### RNC units are stored unpacked

Every RNC stream in the ROM (14,426) is a load unit of its own, and every reader of an RNC
unit runs `ProPackDecompress` (`func_00200B00.c`) before touching the block:
`Model_Decompress` (`func_00209AF4.c:107`, behind `Model_DecompressCallback`/`Terminated`/`Extra`,
which are one-line wrappers), `Texture_DecompressCallback` (`func_00206990.c:36`),
`Anim_Decompress` (`func_00216A9C.c:133`), `CScene__DecompressGridSection` (`func_00220D00.c:97`),
`Cinema_PrepareResource` (`func_0040F240.c:242`), `CScene__DecompressIntelligence`
(`func_002261A8.c:71`), `CScene__DecompressParticles` (`func_0042CAB4.c:16`),
`CScene__DecompressCollision` (`func_0042B6B0.c:110`), `CScene__DecompressPaths`
(`func_0042CD04.c:42`), `CScene__DecompressAnimatedInstances` (`func_0042BA40.c:84`),
`CScene__DecompressLinks` (`func_0042B3E4.c:86`), `ProPack__Decompress` (`func_00200AE0.c`).
None reads the block before that call. `ProPackDecompress` on a non-RNC block either leaves
it in place (`arg1 = 1`) or copies the cache entry's size (`arg1 = 0`), so the block the
callback continues with is the same bytes it would have unpacked, provided the cache entry
is `hostSize` long. That is the one engine change this needs: `CCache__Request` asks
`T2Asset_HostSize(key, size)` (PORT.md 5.4). The 0-7 bytes of slot padding after a stream are
not stored. The port therefore never needs the RNC unpacker except in the bring-up ROM
backend.

The unpacker (builder: `tools/assets/rnc.py`; ROM backend: `port/assets/rnc.c`) is original code of this repository (CC0, like the rest), written from Turok 2's own routines (`ProPackDecompress`, `RNC_UnpackMethod1` 0x200C40, `RNC_UnpackMethod2` 0x2010C0) and the public RNC header layout.
It replaced the unlicensed Rage Wars-derived `propack.py` / `port/third_party/rnc` (docs/port/RAGEWARS_PORT.md
section 0) without changing a byte: same archive sha1, all 14,426 outputs identical to the old tool's.

### Containers with RNC children: there are none (no KEEP_PACKED, no table rewrite)

PORT.md 5.2 left open what to do with a raw container whose entries are RNC streams that the
engine unpacks from memory (store it `KEEP_PACKED`, or rewrite its offset table after
unpacking the children). The census answers it: walking all 2,906 `CONTAINER` units (anim
object info, Cinema, static events, persistent bit sections, warp dests, swooshes, sound
data, ...) finds **zero** RNC streams inside them, and the 14,426 RNC units are exactly all
RNC streams in the ROM. Every RNC stream is an entry of an *index* set, and index entries are
addressed in ROM space (`CISet__GetROMAddress[AndSize](table, setKey, i)` = `setKey +
offset[i]`), never through a loaded parent. Unpacking a child therefore changes no offset
the engine reads: index headers are stored as in the ROM and still give the ROM keys, and
the size the engine computes from them (`offset[i+1] - offset[i]`, the packed slot) is
mapped to the host size by `T2Asset_HostSize`. The engine stays unchanged. `mkarchive.py`
(`check_rows`) refuses to build if a unit ever carries `RNC_CHILDREN`, so another release
with such a container would be caught rather than silently mis-stored.

### Units read partially or at an offset

All `CCache__Request` keys in the C come from `CISet__GetROMAddress[AndSize]` or from fields
saved from them (`m_rpObject`, `animKey`, `animFile`, `key`, ...), i.e. unit starts. Reads
that are not "whole unit at its key":

| reads | units | what the port does |
|---|---|---|
| 4-byte count probe, then `count*4 + 15 & ~7` (`CCache__LoadPersistantIndex`, `func_00205B94.c`; "animations index" `+4`) | `INDEX_HEADER` | prefix of the unit: `T2Asset_HostSize` returns the asked size when it is smaller than the ROM size |
| 0x18-byte "object index" requests | `INDEX_HEADER` (count 4: the whole header) | whole unit |
| `__amDMA` (`func_002700D0.c`): `CLoader__LoadAsync(0x20F194 + offset, 0x300)` | `audiotbl`, `RAW_RANGE` | find the unit containing the address, copy from inside it |
| `VM_Alloc` + TLB pager (4 KB pages) | `audioctl`, `sound`, `VM_WINDOW` | load the whole unit once, keep it resident |
| `CLoader__LoadSync(unk_28, unk_30)` (`func_0042C418.c`, `func_0042C46C.c`) | cartridge 6 / 7, `LOADSYNC` | whole unit (key and size come from `GetROMAddressAndSize(cart, 6/7)`) |

The units do not overlap (checked by the builder), so the containing unit of an address is
unique. Whether some call site asks for *more* than a unit's ROM size is a run-time question;
PORT.md step 7 (log every request against `t2/units`) answers it.

### Byte order

Unit bytes stay in ROM (big-endian) order; swapping per type happens in the port's
`T2Asset_Read` (PORT.md 5.3, BYTEORDER.md). The libultraship header and the T2 metadata are
little-endian because their own byte-order byte says so; that byte governs the header and
the Blob's `u32` size only, not the unit bytes.

### Naming

`t2/u/` + 8 uppercase hex digits (PORT.md 5.1 sketched 7; 8 keeps names fixed-width for any
32-bit key). The engine never sees names: `T2Asset_Read` formats the name from the key.

### Determinism

Entries are written in a fixed order (`version`, `t2/info.json`, `t2/units`, `t2/units.tsv`,
`t2/types`, then units by key) with fixed zip metadata (date 1980-01-01 00:00:00, unix
attributes 0100644 (regular file, rw-r--r--), `create_system` = unix, no extra fields). Checked: builds with `-j 8` and
`-j 1` and from `.z64`, `.v64` and `.n64` images are byte-identical
(sha1 `bb77f744594dd036be4d6738eecbb6f1ef25d8fc` with Python 3.13 / zlib 1.3.1). Deflate
output depends on the zlib version, which `t2/info.json` records; `--store` gives an archive
that depends on nothing but the ROM. The port should compare `version` (format + ROM SHA1),
not the archive's hash.

## Verification

`archive_check.py` opens the archive with the stdlib `zipfile` (zip CRCs via `testzip`),
walks the ROM afresh with `romfs`, and checks: `version` (byte order, CRC1, format, SHA1);
`t2/units` equal to `RomFS.manifest()` record by record and sorted; every unit entry's
libultraship header, T2 block and size; the bytes against `ROM[key:key+size]` for stored
units and against a fresh unpack of the RNC stream (length = header size, CRC16 = header CRC)
for RNC units; no missing or extra entries; the `--folder` tree byte for byte. A tampered
archive (one flipped byte in a unit, one unit removed) fails with both errors reported.

## Mods and the Torch module

- A mod O2R that contains `t2/u/XXXXXXXX` in the same layout shadows the unit (libultraship
  mounts it later). Its size may differ: the port should take the size from the entry it
  actually loads (the Blob `u32` at 0x40) rather than from the base archive's `t2/units`.
- The shipping extractor (a Torch `T2` module, PORT.md 5.5) must produce byte-identical
  entries; `archive_check.py` (and a straight comparison with `mkarchive.py --store` output)
  is the oracle.

## Open questions

- libultraship's `IsGameVersionValid`: the port should register `0x49088A11` (ROM CRC1) as
  its valid game version, the way Ghostship/Starship register theirs.
- Not exercised with libzip itself on the build box (no libzip there); the zip is a plain
  deflate/stored zip, and the port-side reader prototype (b3, miniz) reads it.
- Larger-than-unit requests (if any) only show at run time (PORT.md step 7).
