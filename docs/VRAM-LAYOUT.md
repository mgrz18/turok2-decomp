# Where the code actually lives

The single most consequential fact about this ROM, and the one the project had
wrong from the first splat config until pass 6.

## TL;DR

The engine's `.text` starts at ROM `0x1100` and loads at VRAM **`0x80200500`**,
not `0x80000500`. The 256-byte SN64 stub at ROM `0x1000` / VRAM `0x80000400`
(`romMain`, the cartridge entry point) is what DMAs it there.

Correcting this took the disassembly from 48.8% to 94.9% decoded.

## How to prove a load address from the ROM alone

`jal` encodes its destination absolutely — bits [27:2] of the target sit in the
instruction word, and the top nibble comes from the delay slot's PC. For KSEG0
code that makes the full address recoverable with no assumptions:

```python
if (word >> 26) == 0x03:                       # jal
    target = 0x80000000 | ((word & 0x03FFFFFF) << 2)
```

Two tests fall out of that, both implemented in `tools/function_seed.py`:

**Does the call graph close?** Collect every `jal` target in the code area. If
the declared VRAM is right, the bulk of them land inside the declared range —
the engine calls itself constantly. Here they did not:

| destination | `jal` instructions |
|---|---|
| `0x802_____` | 14,208 |
| `0x804_____` | 1,470 |
| `0x800_____` | **1** |
| `0x8C0_____` | 1 |

15,680 calls and exactly one to the range the config claimed. That is the
smoking gun.

**Do the targets land on function starts?** Map each unique target back to a ROM
offset under a candidate delta and check how many hit an `addiu sp, sp, -X`
prologue:

| candidate | prologue hits |
|---|---|
| `vram 0x80000500` (as configured) | 0 / 1,391 — **0.0%** |
| `vram 0x80200500` | 912 / 1,391 — **65.6%** |

Chance is ~1% in that region (1,763 prologues across 168,662 words). The 34%
that miss are leaf functions, which never touch `sp` and so have no prologue.

A third, assumption-free variant: cross every target against every prologue in
the ROM and let the deltas vote. `0x801FF400` wins with 49.7%, mapping the
lowest target to ROM `0x001100` — the exact start of the code segment.

## Consequence: LibTEngine's addresses do not describe this ROM

`versions/symbol_addrs.us.txt` used to be seeded from
[LibTEngine](https://github.com/Drahsid/LibTEngine)'s `functions.csv`. Running
the same tests against its 225 functions in `0x800`–`0x801`:

- **0.9%** land on a prologue — indistinguishable from chance
- **0 of 225** are the target of any `jal` in the ROM

They are almost certainly from a different build or revision. The one case that
seemed to confirm them, `CHeap__AllocTop_80042cb0` sitting exactly at ROM
`0x438B0`, was circular: that subsegment boundary had been chosen *from* that
address.

Symbols are now recovered from the ROM itself instead
(`versions/symbols_from_scan.us.txt`, 4,111 entries). LibTEngine remains useful
for struct layouts and for what functions *mean* — just not for where they are.

## Still open

- `libultra` keeps the old `0x7FFFF400`-style delta (`vram: 0x800C2474`) and is
  the worst-decoding area left: `os_text_tail` at 0.0%, `os_text_thread` at
  6.3%, `os_text_exception` at 13.0%. Same method should locate it.
- The `0x80400000`–`0x80430B20` cluster (370 functions, 1,470 refs) is
  unplaced. Delta voting splits between two candidates inside the `virtual`
  region (46.7% vs 38.5%), so it needs a tiebreaker rather than a guess.
