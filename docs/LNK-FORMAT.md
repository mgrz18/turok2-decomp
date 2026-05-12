# SN64 LNK debug records in Turok 2 (US)

Turok 2: Seeds of Evil ships with three intact SN64 LNK object records embedded
in the ROM. They are a leftover of the Acclaim build pipeline — almost
certainly the output of the SN64 assembler/compiler for a handful of debug-
enabled translation units that made it into the final image. Decoding them
recovers original Windows source paths, function names, and some line-number
tables.

## Where they live (US 1.0 ROM)

```
0x00107000  LNK  - engine/PTIMER.CPP                 (size ~0x1870, clean END)
0x00137000  LNK  - engine/Virtual/VMEMORY.C          (size ~0xe34,  clean END)
0x00139000  LNK  - engine/Virtual/VMASM.S            (truncated mid-RELOCATION)
```

`grep -boaP '\x4c\x4e\x4b\x02' baserom.us.z64` will list them.

## File layout

```
0x00  4 bytes    Magic "LNK\x02"   (the \x02 is the format version)
0x04  ...        Bytecode stream of opcode records, terminated by OP_END (0)
```

The format is a direct descendant of Sony's PSY-Q LNK (.OBJ) used by the
PlayStation 1 toolchain. SN64 reuses the same opcode numbers; the
differences we observed are purely the target architecture (MIPS R4300 with
N64 ABI) and a small number of opcodes the SN64 build seems to emit that
PSY-Q did not (TBD — opcode 60 inside an expression tree appears at the end
of the VMASM.S record and the public PSY-Q parser does not know it).

## Opcode table (status in Turok 2 dump)

| Code | Name                       | Layout                                          | Decoded? |
|-----:|----------------------------|-------------------------------------------------|----------|
|    0 | END                        | (none)                                          | yes      |
|    2 | BYTES                      | u16 size; size bytes                            | yes      |
|    6 | SWITCH                     | u16 section_index                               | yes      |
|    8 | ZEROES                     | u32 size                                        | yes      |
|   10 | RELOCATION                 | u8 type; u16 offset; expression*                | partial  |
|   12 | EXPORTED_SYMBOL            | u16 idx; u16 sec; u32 off; pstring name         | yes      |
|   14 | IMPORTED_SYMBOL            | u16 idx; pstring name                           | yes      |
|   16 | SECTION                    | u16 idx; u16 group; u8 align; pstring name      | yes      |
|   18 | LOCAL_SYMBOL               | u16 sec; u32 off; pstring name                  | yes      |
|   28 | FILENAME                   | u16 file_id; pstring path                       | yes      |
|   46 | PROGRAMTYPE                | u8 type (always 9 in Turok 2)                   | yes      |
|   48 | UNINITIALIZED              | u16 idx; u16 sec; u32 size; pstring name        | yes      |
|   50 | INC_SLD_LINENUM            | u16                                             | yes      |
|   52 | INC_SLD_LINENUM_BY_BYTE    | u16; u8                                         | yes      |
|   54 | INC_SLD_LINENUM_BY_WORD    | u16; u16                                        | yes      |
|   56 | SET_SLD_LINENUM            | u16 offset; u32 line                            | yes      |
|   58 | SET_SLD_LINENUM_FILE       | u16 offset; u32 line; u16 file_id               | yes      |
|   60 | END_SLD                    | u16 (must be 0)                                 | yes      |
|   74 | FUNCTION                   | section header (see source)                     | yes      |
|   76 | FUNCTION_END               | u16; u32; u32                                   | yes      |
|   78 | BLOCK_START                | u16; u32; u32                                   | yes      |
|   80 | BLOCK_END                  | u16; u32; u32                                   | yes      |
|   82 | SECTION_DEF                | u16; u32; u16; u16; u32; pstring                | yes      |
|   84 | SECTION_DEF2               | u16; u32; u16; u16; u32; u16 dims; ...; 2 pstr  | yes      |
|   86 | FUNCTION_START2            | section header + 2 extra u32s                   | yes      |
|    ? | (in expression) op 60      | unknown - aborts VMASM.S record                 | TODO     |

Expressions inside RELOCATION use the PSY-Q sub-language: VALUE/SYMBOL/
SECTION_BASE/SECTION_START/SECTION_END/ADD/SUB/DIV. The one tail byte we
fail on is an `0x3c` (60) where a leaf opcode is expected. That same value
is `END_SLD` at the outer level, which suggests SN64 may overload codes
slightly differently here; PRs welcome.

`pstring` = single byte length followed by ASCII bytes (no NUL).

## Hand-trace, first 80 bytes of record at 0x107000

```
4c 4e 4b 02                      "LNK\x02" magic + version 2
2e 09                            PROGRAMTYPE=46, type=9
10 01 00 00 00 00 00 08 06 .rdata    SECTION id=1 group=0 align=8 name=.rdata
10 02 00 00 00 00 00 08 05 .text     SECTION id=2 ...
10 03 00 00 00 00 00 08 05 .data
10 04 00 00 00 00 00 08 06 .sdata
10 05 00 00 00 00 00 08 05 .sbss
10 06 00 00 00 00 00 08 04 .bss
10 07 00 00 00 00 00 08 06 .ctors
10 08 00 00 00 00 00 08 06 .dtors
1c 3b 00 26 D:\turok2\n64\source\engine\PTIMER.CPP    FILENAME id=0x3b len=0x26
1c 3a 00 24 D:\turok2\n64\source\engine\PTIMER.H
...
```

## How to regenerate

```bash
# Human-readable summary:
python3 tools/lnk_decoder.py --rom baserom.us.z64

# JSON for downstream tooling:
python3 tools/lnk_decoder.py --rom baserom.us.z64 --json > versions/lnk.us.json

# Splat-style symbol stubs (gitignored generated output):
python3 tools/lnk_decoder.py --rom baserom.us.z64 --symbols > versions/symbols_from_lnk.us.txt
```

The default behaviour scans the whole ROM for the `LNK\x02` magic; the
`--start` flag only reorders the hint to come first, it does not gate the
search.

## What we get (US 1.0)

- 3 LNK records, 2 cleanly terminate with `OP_END`.
- 51 + 7 + 1 = **59 source-path file entries** (many are duplicates of common
  headers; 24 unique paths).
- 6 unique exported symbols: `InitPTimers`, `DoInit`, `UpdatePTimers`,
  `UpdateProfileTimer`, `DoUpdate`, `VM_PhysicalPool`.
- 4 unique imported symbols (libgcc helpers + `frame_number`):
  `__udivdi3`, `__floatdisf`, `__cmpdi2`, `frame_number`.
- 15 + 2 = 17 line-table entries (mostly `SET_SLD_LINENUM` + bytes of
  `INC_SLD_LINENUM` deltas inside VMASM.S).

These records cover only three translation units, so the catalogue is small
but the format is now decoded and applies one-for-one to any further LNK
blobs we find in sister titles (Armorines, Turok 3) since they ship the
same Acclaim London toolchain.

## Overlap with `psyq-obj-parser`

The opcode dispatcher above is a stripped Python port of
`tools/psyq-obj-parser/psyq-obj-parser.cc` from `grumpycoders/pcsx-redux`.
We dropped relocation reassembly and ELF emission (we don't need those
yet) and kept everything that produces human/JSON output. If we later
want full ELF rehydration of these objects, that C++ parser is the
upstream to mirror.
