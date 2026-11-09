# Display lists on the host (Phase 7 round 2, q4)

The engine builds its display lists at run time, one command at a time through `gDList`
(F3DEX2, the "F3DEX.NoN fifo 2.05" microcode: `0xDE` G_DL, `0xDA` G_MTX, `0x01` G_VTX ...).
libultraship's Fast3D interprets F3DEX2 (`GBI_UCODE=F3DEX_GBI_2`), so the command words can stay
what they are; what has to change on a 64-bit host is the *size of a word*, the *addresses* in
`w1`, and the *segmented* addresses. This round makes those three host-correct in `src/` with
zero change to the N64 build (`tools/asm_diff.py --base 55932a88`: 3938 files, 0 differ, after
every commit).

## 1. Gfx

`include/gfx.h`: `Gfx` is `{ GfxWord w0, w1; }`, `GfxWord` = `u32` on the N64 and
`__UINTPTR_TYPE__` under `TARGET_PORT`. That is the layout of LUS's `Gfx` (`union { Gwords
{ uintptr_t w0, w1; } ... }`, 16 bytes; no `USE_GBI_TRACE` in our LUS build), so a game display
list is handed to `Fast3dWindow::DrawAndRunGraphicsCommands` with a cast. The game never includes
LUS's `gbi.h` (and must not: both define `Gfx`); port glue that sees both casts.

`GFX_SIZE` (gfx.h) = bytes per command: 8 on the N64, `2 * sizeof(GfxWord)` on the host. Code
that sizes or measures display lists in bytes uses it.

## 2. include/gbi_t2.h: one GBI for the game

Before: 25 files defined their own copies (`GFX_CMD`, `GFX_W`, `GFX`, `GFX_WORDS`,
`_GBI_CMD`, `_SHIFTL`, `gDP*`/`gSP*` in three different calling conventions: packet-first,
implicit-`gDList`, and raw words). Now they all include `gbi_t2.h`, which has

| what | N64 | TARGET_PORT |
|---|---|---|
| `GFX_W(pkt, w0, w1)` / `GFX_CMD(w0, w1)` (= `GFX_W(gDList++, ...)`) | the engine's raw-word idiom | same |
| `GBI_ADDR(p)`: an address in a word | `(unsigned int)(p)` (what every site had) | `(GfxWord)(p)`: the full host pointer |
| `GBI_SEGADDR(a)`: a segment-relative address `0x0S00XXXX` | `a` | `a \| 1` (Fast3D's `Interpreter::SegAddr` only resolves segments for odd `w1`) |
| `_SHIFTL`, opcodes, `G_*` parameters, `CALC_DXT`... | libultra's | same |
| `gDPPipeSync/LoadSync/TileSync/FullSync`, `gDPSetTextureImage`, `gDPSetTile`, `gDPLoadBlock` (+ `gDPLoadBlockN` without the `MIN` clamp, for the two Rage Wars files that clamp themselves), `gDPSetTileSize`, `gDPLoadTLUTCmd`, `gDPSetPrimColor/EnvColor`, `gDPFillRectangle`, `gSPSetOtherMode` + `gDPSetTextureLOD/Detail/LUT`, `gDPSetCycleType1Cycle`, `gDPSetRenderMode`, `gSPTexture`, `gSPGeometryMode`, `gSPDisplayList`, `gSPMatrixLoad`, `gImmp1`, `gSPTextureRectangle`, `gSPScisTextureRectangle`, `gDPLoadTextureBlock` (generic `siz`), `gDPLoadMultiBlock_4b` | libultra's macros, packet first | address arguments through `GBI_ADDR` |

The three HUD mask blitters (`Hud_DrawMaskedBlock*`, func_00279AC4/00279528/00279EEC) and
`Hud_DrawBoxOutline` used implicit-`gDList` variants with fewer parameters; their call sites are
now the libultra calls (`gDPLoadTextureBlock(gDList++, image, G_IM_FMT_RGBA, G_IM_SIZ_16b, w, h,
0, G_TX_WRAP, G_TX_WRAP, 5, 5, G_TX_NOLOD, G_TX_NOLOD)`), with identical code. `MIN`/`MAX` are
`GBI_MIN`/`GBI_MAX` inside the header so they never clash with a file's own.

Not converted (cosmetic): ~30 Rage Wars imports carry *expanded* gbi.h macros inline
(`{ Gfx *_gbi = ...; _gbi->w0 = ((((unsigned int)(0xFD) & ...`). They are correct on the host
now (their address slots go through `GBI_ADDR`); turning them back into `gDP*` calls is a
readability job that asm_diff can check file by file.

## 3. Addresses in w1

94 sites in 29 files cast an address to `u32`/`unsigned int` into `w1` (matrices, vertices,
lights, viewports, look-at, textures, sub-display lists, colour image): all are `GBI_ADDR(...)`
now. They were found from the host build's `-Wpointer-to-int-cast` warnings whose line or macro
writes a Gfx word (the script is described in §7). Remaining, not fixable at the write site
because the *variable* holds the address in an integer (q3's 64-bit typing work):

| site | value | fix needed |
|---|---|---|
| func_0027854C, func_0027B0FC, func_00277F6C `g->w1 = texture` | font/HUD texture held in an int | type the local/callee as a pointer |
| func_00286E14, func_00287248 `w1 = gDepthBuffer`, `->m_pFrameBuffer` | colour/depth image (`gDPSetColorImage/DepthImage`) | `gDepthBuffer`, `gCFB[]`, `m_pFrameBuffer` as pointers (RUNTIME.md: the frame buffers are cache allocations; Fast3D only compares the colour and depth image to detect depth clears) |
| func_0020A33C/0020AF08/0020B258/0022D2FC/0027B320 `((u32)gDList - (u32)start) / sizeof(Gfx)` | a command count | correct on the host (difference modulo 2^32); leave or rewrite as `gDList - start` under `TARGET_PORT` |
| frame.h `m_pDisplayList`, frame.h globals `gDisplayList0/1`, `gLineList0/1` (s32) | the frame's display-list buffers | pointers (q3/q5) |

## 4. Segments

Fast3D keeps a segment table (`G_MOVEWORD`/`G_MW_SEGMENT` stores the full `w1`, so the game's
`gSPSegment(n, hostPtr)` words, `0xDB06000n*4`, work unchanged once `w1` is a host pointer) but
treats `w1` as segmented only when **bit 0 is set**. The only producer of segmented addresses in
the engine is `Model_Decompress` (func_00209AF4): matrices from segment 1 (`(mtx << 6) +
0x01000000`, the per-object matrix block set with `gSPSegment(1, matrix)`) and vertices from
segment 2 (`&vtx - vtxTbl + 0x02000000`, the frame's vertex table, `gSPSegment(2, base)`). Both
go through `GBI_SEGADDR` now. Offsets are multiples of 16/64, so bit 0 is free; Fast3D masks it
off (`w1 & 0x00FFFFFE`).

The only CPU reader of those lists, `ModelSweep_TestGroup` (func_0023D7CC, collision sweep
against a model), read the opcode as `b[0]` of a big-endian 8-byte union and stepped 8 bytes.
Under `TARGET_PORT` its `DLCmd` is `Gfx` and it reads `w0 >> 24` and `w1 & 0xFFFFFE`.

Static display lists in `.data` (`gRspInitDL`, `gRdpInitDL`, `sShadowQuadDL`,
`sTextureLoadDL_I16`, `sHudGlyph16x16LoadDL`, ...) are q2's: generated C must emit them as
`Gfx[]` with host-sized words and real pointers (or `GBI_SEGADDR` for segment-relative ones).

## 5. Sizes

| code | before | now |
|---|---|---|
| `Model_Decompress` size pass | `len += 8` per command, `(len + 0xF) & ~7` (end command + 8-align) | `len += GFX_SIZE`, `(len + GFX_SIZE + 7) & ~7`; the part's DL (at +0x30 of an 8-aligned part) stays 8-aligned |
| `GfxPool_Alloc` (func_00425220 and its module copy func_0045E140) | `(gDisplayListSize + gLineListSize) * 16`, strides `* 8` | `* (2 * GFX_SIZE)`, `* GFX_SIZE` |
| list-full checks func_0020BBE4, func_0027B320 | `bytes >> 3` | `bytes / GFX_SIZE` |
| others already used `sizeof(Gfx)` (func_0020A33C, 0020AF08, 0020B258, 0022D2FC) | | |

`gDisplayListSize`/`gLineListSize` are counts of commands, so nothing else changes.
`m_DisplayListSize` (func_00287248) is a byte count for the OSTask: right on both.

## 6. Acmd (audio command lists): left for the audio round

Nine libaudio files (func_00294D60, 00296360, 002964D0, 00296CD0, 0029704C, 002972E0,
0029F190, 0029F958, 0029FCF4) build `Acmd` words with their own `_SHIFTL`/`a*` macros and put
`osVirtualToPhysical(ptr)` or `(u32)ptr & 0x1FFFFFFF` in `w1`. The same treatment applies (a
shared `abi_t2.h`, `Acmd` with host words, the address through a macro, no KSEG mask on the
host), but only matters once an HLE audio interpreter exists (LIBULTRASHIP.md §4.5); it was not
done this round.

## 7. Tooling, verification

- Every commit: `python tools/asm_diff.py --base 55932a88 -j 16` = 0 differ (whole tree).
- Host: `ninja turok2_game` builds, 0 errors.
- Finding address-in-word sites: build the game library from clean, then keep the
  `-Wpointer-to-int-cast` / `-Wvoid-pointer-to-int-cast` warnings whose source line contains
  `w1` / a `GFX_*`/`gDP*`/`gSP*` call or whose note says "expanded from macro 'GFX..'"; at the
  reported column there is the cast; the operand (unary + postfix chain) is wrapped in
  `GBI_ADDR`. Audio files and command-count expressions were excluded by hand.

- **Encoding check against libultraship** (`port/tests/gbi/`): `game_side.c` + the game's own
  `Hud_DrawBoxOutline` (func_00277668.c) and `Hud_DrawMaskedBlockAt` (func_00279AC4.c), compiled
  with turok2_game's flags, plus `gSPDisplayList`/`gSPMatrixLoad`/a segment and a segmented
  `G_VTX` through `gbi_t2.h`; `lus_side.c` writes the same commands with LUS's `gbi.h`
  (`F3DEX_GBI_2`; note LUS turned `gSPDisplayList`/`gSPSegment`/`gSPVertex` into functions for
  resource paths, the test uses `__gSPDisplayList` etc.). Pointers are heap addresses above 4 GB.
  Box run (2026-11-09): `sizeof(Gfx)` 16 = 16, **30 of 30 commands identical** (both words),
  including the 64-bit texture/DL/matrix/segment addresses and the `0x02000041` segmented
  vertex address.

  ```
  make -C port/tests/gbi LUS=<build>/_deps/libultraship-src BUILD=<out>/gbi && <out>/gbi/gbi_test
  ```

  Not done yet: running a game-built list through Fast3D and reading pixels (the app's
  `testdl.c` path); the encoding identity above is what Fast3D consumes, so the remaining risk
  is data (matrices, vertices, textures), not command words.

## 8. Next

1. Draw a game-built list (port/tests/gbi's) through `DrawAndRunGraphicsCommands` in the app and
   read pixels back, as `testdl.c` does.
2. The int-held addresses of §3 (with q3's typing).
3. Static DLs as C (q2).
4. Matrices: Fast3D reads `Mtx` as `int32_t[16]` in host order (integer halves then fractions);
   the game's matrix writers (`guMtxF2L`-style, `CMtxF__ToMtx*`) write it through 32-bit words,
   so it should hold; verify once a model draws. Vertex data from assets is swapped by
   `swap_spec.json` (BYTEORDER.md).
5. L3DEX line lists (automap, func_004004D8: `G_LINE3D` 0x08): Fast3D has no handler.
6. Acmd (§6).
