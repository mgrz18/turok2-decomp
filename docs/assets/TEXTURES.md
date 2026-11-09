# Textures, palettes, HUD images and fonts

Tool: `tools/assets/textures.py baserom.us.z64 --out DIR` (stdlib only; ~12 s for everything).
It writes `DIR/textures/NNNN.png` (one strip per texture: every animation step), `DIR/graphics/III_idNNNN/FFF.png`
(one PNG per graphic-set frame), `DIR/textures.json` and `DIR/graphics.json` (per-asset metadata).
`--split` also writes one PNG per animation step, `--no-png` only the JSON, `--only textures|graphics`, `--limit N`.

## Where they live

`CEngineApp__Main` stores the constant **0xFCBFB4** in `m_CartridgeIndex` and `CScene__Construct` opens it as the
*cartridge index*: a CISet (`u32 count`, `count+1` offsets relative to the set's own start, the last one the end).
`CISet__GetROMAddress(cart, file, i)` = `file + offset[i]`. The entries this area uses:

| cart entry | ROM range | what | loaded by |
|---|---|---|---|
| 3  | 0x1766AF4..0x1B31794 (3,976,352 B) | **texture sets**: CISet of 1839 texture banks | `CScene__Construct` -> `m_pTextureSets` ("texture sets index") |
| 16 | 0x1F845DC..0x1FDE0F4 (367,384 B) | **graphic sets**: CISet of 114 tiled 2D images (HUD, fonts, menus, full-screen pictures) | `m_pGraphicSets` ("graphicset index") |
| 17 | 0x1FDE0F4..0x1FDE2C4 (464 B) | graphic-set type ids: StrideTable (`s32 stride`=4, `s32 count`=114, then u32 ids, sorted) | `m_pGraphicSetTypes` ("graphicset type") |

Every one of the 1839 texture entries and the 114 graphic sets is an RNC ProPack method-1 block (`RNC\x01`,
all CRCs verified). Blocks in a set are 8-aligned (gaps of 0..7 bytes). Together these cover 4,344,200 bytes,
**13.9 %** of the asset region 0x20F178..0x1FF7AEC.

`textures.py` locates through `romfs.py` (`cart/3`, `cart/16`, `cart/17`) and unpacks through `propack.py`
(the first pass's private locator and RNC1 port are gone). The parsers (`parse_texture_bank`, `decode_texture_frame`, `parse_graphic_set`,
`decode_graphic_frame`) only take decompressed bytes.

## Texture banks ("texture" requests)

### How a texture is referenced

A model material on disk (`Material_Unpack`, called from `Model_Decompress`) holds `s32 texture` = an **index
into the texture set** (-1 = none). At unpack time `func_00207914` -> `CScene__GetTextureSet` turns it into
`(ROM address, size)` with `CISet__GetROMAddressAndSize(m_pTextureSets, m_TextureSetsKey, index)` and stores
the pair in the runtime material (+0x08/+0x0C). `Texture_Request` / `Texture_RequestSync` hand that pair to
`CCache__Request(..., Texture_DecompressCallback, "texture", ...)`; particles (`particle->model`), trails and
other effects keep the same kind of pair. **The texture id that crosses areas is the texture-set index**
(models, particles, levels: whatever stores a texture stores this index).

### Layout (after RNC unpacking)

A CISet of 3 blocks (`u32 3`, 4 offsets, padded so block 0 starts at 0x18):

| block | contents |
|---|---|
| 0 | 8-byte info: `u8 format, u8 speed, u8 log2 width, u8 log2 height, u8 lod, u8 kind, 0, 0` |
| 1 | CISet of **frames**: each the raw texels of one frame |
| 2 | CISet of **palettes** (count 0 when the format has none): RGBA5551 or IA16 TLUTs |

Fields (from `Texture_Load`, `Texture_GetInfo` and the material code in `func_00207FEC.c`):

- `format`, decoded by `Texture_Load`'s switch:

  | fmt | texels | TLUT | count in ROM |
  |---|---|---|---|
  | 0 | CI8 | RGBA16 (256 entries) | 1241 |
  | 1 | CI4 | RGBA16 (16 entries) | 434 |
  | 3 | CI4 | **IA16** (16 entries; high byte intensity, low byte alpha) | 164 |
  | 2 | RGBA32, `2^w x 2^h` | none | 0 |
  | 4 | RGBA16 mip chain, top level 32x32, loaded by the fixed `sTextureLoadDL_Mode2/Mode3/Detail/DetailMirror` (rows padded to 8 bytes; levels at TMEM words 0, 0x100, 0x140, 0x150, 0x154, 0x156; 2744 bytes) | none | 0 |
  | 5 | I4 mip chain, top level 64x64, loaded by `sTextureLoadDL_I16` (levels at 0, 0x100, 0x140, 0x150, 0x158, 0x15C, 0x15E; 2808 bytes) | none | 0 |

  Formats 2, 4 and 5 are supported by the engine (and by the decoder) but no asset in the US ROM uses them.
- `width = 1 << info[2]`, `height = 1 << info[3]` (16..128; most are 32x64, 64x32, 64x64, 32x32). The masks
  default to these (material flag 0x10: `maskS = 1 << (shiftS + 6)`, otherwise the material's own masks).
- `speed`: animation rate. Frame shown = `(gFrameCountGameplay / speed) % frames`, palette =
  `(same) % palettes` — frames and palettes animate **independently** (79 banks have different counts, e.g.
  1 frame x 14 palettes = palette-cycling). The tool exports `lcm(frames, palettes)` steps (capped at 64).
  Values: 1 (1403 banks), 2 (230), 3..30 for the rest.
- `lod`: the `m` (min level / LOD fraction) of `gDPSetPrimColor`; 0xFF in 1831 banks, a few use 0, 25, 240, 244, 248.
- `kind`: for format 4 it selects the detail/mip display list (0 detail clamp, 1 detail sharpen, 2 Mode2, 3 Mode3);
  for formats 4/5 with material flag 0x2000 it selects the combiner. Non-zero in 8 banks (all CI8/CI4, where
  `Texture_Load` ignores it).

`Texture_DecompressCallback` (after unpacking) can recolour the RGBA5551 palettes of formats 0/1 by the top two
bits of the request's flags word: 1 swaps red/green, 2 copies red into green, 3 copies green into red (used for
colour variants of the same bank). The export shows the stored colours.

Totals: 1839 banks, 3441 frames, 3,968,984 packed / 6,840,256 unpacked bytes.

## Graphic sets (HUD images, fonts, menus, pictures)

`CScene__LoadGraphicSet(scene, id, flags, ...)` binary-searches `id` in the type table (entry 17) and requests
the block of the same index with `ProPack__Decompress`, "graphicset". Drawn by `Hud_DrawSprite` /
`Hud_DrawImageFrame` / `Hud_DrawNumber` / `COnScreen__DrawText` (Rage Wars' `GFX/TEXT` code).

Layout (after RNC unpacking), from `Hud_GetImageFrame` and `Hud_DrawImageFrame`:

```
0x0  s16 frameCount     0x2  u16 id (same as the type table)     0x4  pad
0x8  frameCount x { u32 offset (from the set start), u8 opaque, 3 pad }
frame:  u16 columns, u16 rows, u16 width, u16 height     (image size; edge blocks are padded)
        columns*rows blocks, row by row: { u32 w, u32 h, texels }
          opaque != 0 : w*h/2 bytes of I4 (drawn I4, coloured by prim/env: fonts, masks)
          opaque == 0 : w*h*2 bytes of RGBA5551, then w*h/2 bytes of I4 opacity mask
```

Blocks are 32x32 (or smaller at the right/bottom edges); a 320x240 picture is 10x8 blocks. The PNG export puts
RGBA5551 colour with the I4 mask as alpha (masked images) or grey with I4 as alpha (opaque/I4 images).

114 sets, 600 frames (452 I4, 148 RGBA16+mask). Ids (decimal) and what they are, by inspection:

| id | frames | content |
|---|---|---|
| 1 | 65 | large font (I4), glyphs A-Z 0-9 punctuation, accented letters |
| 2 | 62 | medium font (I4) |
| 4 | 10 | HUD digits 0-9 (RGBA16+mask, gold) — `Hud_DrawNumber` |
| 5 | 236 | full character-set font (I4) |
| 6 | 65 | small font (I4) |
| 100 | 1 | "GAME OVER" plate |
| 103 | 24 | screen-wipe / transition masks (I4, 64x64) |
| 150..221 | 1 (221: 9) | HUD icons: weapons, keys, items, health/ammo bars (221 = 9-step bar gauge) |
| 300..740 | 1..20 | HUD panels and menu pieces (400 "AIR" bar, 401 bar, 508 = 20-frame animation) |
| 810..821 | 1 | small icons |
| 900, 901, 902 | 1 | full-screen pictures (900 = 320x240 dinosaur eye title/legal screen) |
| 2000..2080 | 1 | 16x16 icons (map/menu markers) |

`gLoadedFont` (`COnScreen__DrawText`) advances by font: 0 -> 24 px, 1 -> 12, 3 -> 16, 4 -> 8, 5 -> 6;
`COnScreen__SelectNormalFont/SmallFont/TinyFont/Font2/Font3` pick the set; the character -> glyph lookups are built
by `COnScreen__BuildCharacterLookUp` from `sCharacterLookUpPairs` / `sCharacterSmallLookUpPairs` (code data,
not assets). `COnScreen__LoadFontTexture` copies 0x90 bytes of set id 2, frame 42 (the texels after the
grid and block headers) into `D_800F5618` for `sOnScreenFontTexture`.

## Verified visually

- Texture sheet of 24 banks across the set (indices 0..1838, all three formats): stone, dirt, grass, metal,
  bark, a leaf with transparency, a skull, a gradient, a gold relief — clean images, correct palettes, no noise.
- Graphic sets: picture 900 (320x240 dinosaur eye) perfect across all 80 blocks; fonts 1/2/6 legible
  (A-Z, 0-9, `©`, `Ä Ö Ü`, punctuation); digits 0-9 in gold; 9-step bar gauge; the 24-frame wipe mask;
  "GAME OVER"; the "AIR" bar.

## Open questions

- Type-table index 14 and 15 both carry id 157 (two different blocks); the binary search only reaches one of
  them. Both are exported (`014_id0157`, `015_id0157`).
- Which material/actor/menu refers to which texture-set index and graphic-set id (cross-area tables: models
  (a3), levels/particles (a4)).
- Meaning of `lod` values other than 0xFF and of `kind` on CI banks; who uses the palette-recolour modes 1-3
  (the flags word of `Texture_Request` callers).
- Textures embedded elsewhere (sky, level-specific, cinema) were not found: a scan of every RNC block in the
  ROM found no other texture-bank headers, and no raw (uncompressed) ones.

## For other areas

- Texture references are **texture-set indices** (0..1838), resolved through cart entry 3; the material record
  is `{s32 texture (-1 none), s32 unk4, s32 flags, u8 prim[4], u8 env[4], u16 maskS, u16 maskT}` (`Material_Unpack`).
  Material flags seen in the render path: 0x2 translucent, 0x10 masks from the texture, 0x20 clamp,
  0x40 (unk4 bit), 0x100 skip when prim alpha 0, 0x400 env colour, 0x800 mirror, 0x1000, 0x2000 LOD.
- Graphic sets are referenced by **type id** (u32 in cart entry 17), not by index.
- Cartridge index 0xFCBFB4 (21 entries) is the root for everything in 0xFCC014..0x1FF7AEC; see ROMFS.md (a1).
