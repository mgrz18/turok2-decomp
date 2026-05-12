# VMASM — Acclaim Studios London Overlay Dispatcher

## What VMASM is

The Turok 2 SN64 LNK records contain an explicit reference to a source file
named `VMASM.S`. That assembly file implements a tiny overlay dispatcher
that hot-loads chunks of code from ROM into a fixed VRAM bank, in the
classic N64 style of `osPiStartDma(rom_offset, dest_vram, size)`.

The Turok 1 source leak at
`references/Turok-N64-Source-code/src/PR/tengine/dosvers/overlay/`
shows the same idiom (structural reference only — we do **not** copy code).

The same engine is used in Turok 3, Rage Wars, and Armorines, so any work
done here transfers directly to the Armorines port.

## What lives in ROM

| ROM range            | size      | content                                      |
|---                   |---        |---                                           |
| 0x13B5F0 – 0x17A600  | 0x3F010   | `virtual_text_0`   (statically resident)     |
| 0x17A600 – 0x186000  | 0xBA00    | `virtual_rodata_0`                           |
| 0x186000 – 0x1B3400  | 0x2D400   | `virtual_text_1`   (overlay)                 |
| 0x1B3400 – 0x1BFA00  | 0xC600    | `virtual_rodata_1`                           |
| 0x1BFA00 – 0x1DEE00  | 0x1F400   | `virtual_text_2`   (overlay)                 |
| 0x1DEE00 – 0x20E334  | 0x2F534   | `virtual_rodata_2`                           |

## Adopted VRAM layout (Round 5)

Pass 5 (Agent O) reached the following layout by **gap-analysis of the
1820 unresolved references** that splat produced when the whole `virtual`
segment was assumed to be sequentially placed in VRAM from 0x8013AAF0:

| module                | VRAM load      | rationale                              |
|---                    |---             |---                                     |
| `virtual_text_0`      | 0x8013AAF0     | statically resident, ROM-natural VRAM  |
| `virtual_rodata_0`    | 0x80179B00     | follows text_0 (in-place)              |
| `virtual_text_1`      | 0x80210000     | covers first unresolved cluster        |
| `virtual_rodata_1`    | 0x8023D400     | follows text_1 in same bank            |
| `virtual_text_2`      | 0x80249A00     | follows rodata_1 in same bank          |
| `virtual_rodata_2`    | 0x80268E00     | follows text_2; ends at 0x80298334     |

`virtual_text_0` is its own segment; modules 1 and 2 share a single splat
segment named `virtual_overlay_bank` that begins at VRAM 0x80210000.

## Dispatch table — search status

The structural goal of the round was to **locate the dispatch table** in
the ROM and read the actual VRAMs directly. We did not find it in the
likely locations:

* `virtual_rodata_0` (ROM 0x17A600 – 0x186000) does not contain a
  table shaped like `(rom_offset, vram_load, size, entry_point)`.
* The engine `.rodata` block (ROM 0xA5FD8 – 0xBDF2C) does not contain
  literal BE encodings of `0x00186000` / `0x001BFA00` / `0x80210000` /
  `0x80280000`.
* A whole-ROM 4-uint32-BE row-scan returned **zero** plausible candidates
  whose first field equals a known overlay rom_start and whose second
  field is in the kernel VRAM range.

This is consistent with the table being **built at runtime** from the
LNK section-relative records at ROM 0x107000 – 0x13B5F0 — VMASM walks
those records and computes the load addresses dynamically. To extract it
properly would require simulating the dispatcher boot path on a CPU
trace; that is left for Pass 6.

In the meantime `tools/vmasm_decode.py` prints the round-5 adopted layout
and supports `--scan` / `--table-offset <hex>` so future passes can drop
in a discovered table without rewriting tooling.

## Outstanding mystery: the 0x80400000 cluster

After fixing the YAML to use the bank layout above, ~620 references in
the 0x80403BF0 – 0x8042F3D0 range remained unresolved. The cluster is
contiguous and roughly the size of `virtual_text_2` plus its rodata,
which suggests **module 2 may load to a second VRAM slot at runtime**
(double-banked overlay). Splat can only express one VRAM per ROM region,
so this cluster cannot be resolved without either:

1. Duplicating the ROM region into a second pseudo-segment with the
   alternate VRAM (requires splat patch or post-link symbol aliasing),
   or
2. Stubbing the unresolved targets with shim functions that trampoline
   to the primary VRAM at runtime.

Pass 6 should pick one of these strategies and run with it.

## Round 5 link status

`make rom` still fails to link: **2,414 unresolved references remain**
(down from ~1,820 in the assumed-sequential layout — the rise comes
from the new 0x80400000 cluster, which was previously masked when
splat absorbed those bytes into a continuous segment). The bank-layout
fix is structurally correct (text_0 has been separated, the
unresolved cluster between 0x80210000 and 0x80298334 is now genuinely
resolvable bytes), but the dual-VRAM problem blocks the final link
this round.

**SHA1 verify:** not attempted — no `.z64` produced.

## Regenerating the table read-out

```bash
python3 tools/vmasm_decode.py             # pretty-print the table
python3 tools/vmasm_decode.py --scan      # rerun the row-scan
python3 tools/vmasm_decode.py --yaml-snippet  # emit YAML lines
```

## Implication for Armorines

Armorines uses the same engine and almost certainly the same VMASM
dispatcher. The layout will differ in ROM offsets and probably in the
VRAM bank addresses, but the **shape** is the same: one static text
module plus N overlays sharing one or two banks. Re-using
`tools/vmasm_decode.py` for that title should be a 30-minute job once
the bank VRAMs are identified.
