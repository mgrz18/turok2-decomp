# Where the code actually lives

The single most consequential fact about this ROM, and the one the project had
wrong from the first splat config until pass 6.

## TL;DR

The address space is **mixed**: code runs TLB-mapped in useg, data and libultra
run direct in KSEG0.

| region | ROM | address | space |
|---|---|---|---|
| boot stub | 0x1000 | `0x80000400` | KSEG0 |
| engine `.text` | 0x1100 | `0x00200500` | useg |
| engine `.rodata` | 0xA5FD8 | `0x800A53D8` | KSEG0 |
| `code_rodata_tail` | 0xC0950 | `0x800BFD50` | KSEG0 |
| libultra | 0xC3074 | `0x800C2474` | KSEG0 |
| `libultra_tail` | 0xF7F50 | `0x800F7350` | KSEG0 |
| `virtual` | 0x14A000 | `0x00400000` | useg |

The engine's `.text` was configured at `0x80000500`, off by `0x200000` and in
the wrong space. Correcting it took the disassembly from 48.8% to 95.2%.

An earlier revision of this document put everything in KSEG0 and wrote the
engine at `0x80200500`. That was half right: the offset was correct, the space
was not. A `jal` takes its top nibble from the PC, so code executing in useg
produces `0x002xxxxx` targets, never `0x802xxxxx`. The tell was sitting in the
boot stub the whole time — `main` decodes out of the ROM as `0x0028D380`, with
no leading 8.

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

## Locating the `virtual` segment

The same method placed it, and is worth spelling out because it needs no prior
knowledge of the game.

The `0x00400000`–`0x00430B20` cluster held 364 `jal` targets that no segment
covered. Sweeping candidate deltas and scoring each by how many targets land
exactly on an `addiu sp, sp, -X`:

| delta | implied ROM start | prologue hits |
|---|---|---|
| `0x2B6000` | **0x14A000** | 170 / 364 — **46.7%** |
| `0x27D000` | 0x183000 | 140 / 364 — 38.5% |

Chance is ~1%. Of the 170 hits, **147** are preceded by `jr ra` plus its delay
slot, i.e. genuine function starts rather than coincidental prologues. A ±0x400
sweep in 4-byte steps shows an isolated maximum, not a plateau. ROM `0x14A000`
itself opens on `27BDFFE8`.

Prologue density corroborates it: `0x14A000` onward runs at 12.4 per 4 KB,
matching known-good code exactly, while `0x13B5F0`–`0x14A000` sits at 7.2 —
mixed content, neither code nor pure data. That stretch is parked as
`pre_virtual` (bin) until it is identified.

## Still open

- The `code`/`rodata` boundary at ROM `0xA5FD8` is where the strings start
  (`"simp obj"`), but code resumes after them — `jr ra` at `0xA643C` and
  `0xA6968` with no prologues in between, so leaf functions interleaved with
  rodata. The segment needs splitting to cover them.
- What `0x13B5F0`–`0x14A000` actually is.
