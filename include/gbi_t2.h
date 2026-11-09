#ifndef GBI_T2_H
#define GBI_T2_H

/* The part of libultra's PR/gbi.h (F3DEX2, "F3DEX.NoN fifo 2.05") that Turok 2
 * writes, in one place. The engine builds its display lists at run time, one
 * 8-byte command at a time through gDList; the matching sources used to carry
 * their own copy of these macros in every file that needed them.
 *
 * On the N64 every macro expands to exactly what the per-file copies did
 * (tools/asm_diff.py: 0 differences). Under TARGET_PORT:
 *   - Gfx is libultraship's layout (two uintptr_t words: include/gfx.h);
 *   - GBI_ADDR(p) puts a full host pointer in w1 (on the N64 it is the
 *     (unsigned int) cast the game always had);
 *   - GBI_SEGADDR(a) marks a segment-relative address (0x0S00XXXX) with bit 0,
 *     which is how Fast3D tells a segmented address from a raw pointer
 *     (Interpreter::SegAddr). Segment offsets of vertices and matrices are even.
 * See docs/port/GBI.md. */

#include "gfx.h"

#ifdef TARGET_PORT
#define GBI_ADDR(p) ((GfxWord) (p))
#define GBI_SEGADDR(a) ((GfxWord) (a) | 1)
#else
#define GBI_ADDR(p) ((unsigned int) (p))
#define GBI_SEGADDR(a) (a)
#endif

/* ---- encoding helpers ---------------------------------------------------------------------- */

#define _SHIFTL(v, s, w) ((unsigned int) (((unsigned int) (v) & ((0x01 << (w)) - 1)) << (s)))

/* One raw command: the engine's own idiom (w0 and w1 spelled out, often as hex). */
#define GFX_W(pkt, a, b) { Gfx *_g = (Gfx *) (pkt); _g->w0 = (a); _g->w1 = (b); }
/* ... appended at gDList. */
#define GFX_CMD(a, b) GFX_W(gDList++, a, b)
#define _GBI_CMD(pkt, a, b) GFX_W(pkt, a, b)

#define GBI_MIN(a, b) ((a) < (b) ? (a) : (b))
#define GBI_MAX(a, b) ((a) > (b) ? (a) : (b))

/* ---- opcodes (F3DEX2) ---------------------------------------------------------------------- */

#define G_VTX 0x01
#define G_MODIFYVTX 0x02
#define G_TRI1 0x05
#define G_TRI2 0x06
#define G_LINE3D 0x08
#define G_TEXTURE 0xD7
#define G_POPMTX 0xD8
#define G_GEOMETRYMODE 0xD9
#define G_MTX 0xDA
#define G_MOVEWORD 0xDB
#define G_MOVEMEM 0xDC
#define G_DL 0xDE
#define G_ENDDL 0xDF
#define G_SETOTHERMODE_L 0xE2
#define G_SETOTHERMODE_H 0xE3
#define G_TEXRECT 0xE4
#define G_RDPLOADSYNC 0xE6
#define G_RDPPIPESYNC 0xE7
#define G_RDPTILESYNC 0xE8
#define G_RDPFULLSYNC 0xE9
#define G_SETSCISSOR 0xED
#define G_LOADTLUT 0xF0
#define G_SETTILESIZE 0xF2
#define G_LOADBLOCK 0xF3
#define G_SETTILE 0xF5
#define G_FILLRECT 0xF6
#define G_SETPRIMCOLOR 0xFA
#define G_SETENVCOLOR 0xFB
#define G_SETTIMG 0xFD
#define G_SETZIMG 0xFE
#define G_SETCIMG 0xFF
#define G_RDPHALF_1 0xE1
#define G_RDPHALF_2 0xF1

/* ---- parameters ---------------------------------------------------------------------------- */

#define G_ON 1
#define G_OFF 0

#define G_IM_FMT_RGBA 0
#define G_IM_FMT_YUV 1
#define G_IM_FMT_CI 2
#define G_IM_FMT_IA 3
#define G_IM_FMT_I 4

#define G_IM_SIZ_4b 0
#define G_IM_SIZ_8b 1
#define G_IM_SIZ_16b 2
#define G_IM_SIZ_32b 3

#define G_TX_LOADTILE 7
#define G_TX_RENDERTILE 0
#define G_TX_NOMIRROR 0
#define G_TX_WRAP 0
#define G_TX_MIRROR 1
#define G_TX_CLAMP 2
#define G_TX_NOMASK 0
#define G_TX_NOLOD 0
#define G_TX_DXT_FRAC 11
#define G_TX_LDBLK_MAX_TXL 2047

#define G_MDSFT_TEXTLOD 16
#define G_MDSFT_TEXTDETAIL 17
#define G_MDSFT_TEXTLUT 14
#define G_TL_TILE (0 << G_MDSFT_TEXTLOD)
#define G_TL_LOD (1 << G_MDSFT_TEXTLOD)
#define G_TT_NONE (0 << G_MDSFT_TEXTLUT)
#define G_TT_RGBA16 (2 << G_MDSFT_TEXTLUT)
#define G_TT_IA16 (3 << G_MDSFT_TEXTLUT)
#define G_TD_CLAMP (0 << G_MDSFT_TEXTDETAIL)
#define G_TD_SHARPEN (1 << G_MDSFT_TEXTDETAIL)

/* texture-loading sizes per pixel size (gDPLoadTextureBlock's siz##_LOAD_BLOCK etc.) */
#define G_IM_SIZ_4b_BYTES 0
#define G_IM_SIZ_4b_LINE_BYTES G_IM_SIZ_4b_BYTES
#define G_IM_SIZ_8b_BYTES 1
#define G_IM_SIZ_8b_LINE_BYTES G_IM_SIZ_8b_BYTES
#define G_IM_SIZ_16b_BYTES 2
#define G_IM_SIZ_16b_LINE_BYTES G_IM_SIZ_16b_BYTES
#define G_IM_SIZ_32b_BYTES 4
#define G_IM_SIZ_32b_LINE_BYTES 2
#define G_IM_SIZ_4b_LOAD_BLOCK G_IM_SIZ_16b
#define G_IM_SIZ_8b_LOAD_BLOCK G_IM_SIZ_16b
#define G_IM_SIZ_16b_LOAD_BLOCK G_IM_SIZ_16b
#define G_IM_SIZ_32b_LOAD_BLOCK G_IM_SIZ_32b
#define G_IM_SIZ_4b_SHIFT 2
#define G_IM_SIZ_8b_SHIFT 1
#define G_IM_SIZ_16b_SHIFT 0
#define G_IM_SIZ_32b_SHIFT 0
#define G_IM_SIZ_4b_INCR 3
#define G_IM_SIZ_8b_INCR 1
#define G_IM_SIZ_16b_INCR 0
#define G_IM_SIZ_32b_INCR 0
#define G_TEXTURE_IMAGE_FRAC 2

/* texture loading arithmetic (gDPLoadTextureBlock) */
#define TXL2WORDS(txls, b_txl) GBI_MAX(1, ((txls) * (b_txl) / 8))
#define CALC_DXT(width, b_txl) (((1 << G_TX_DXT_FRAC) + TXL2WORDS(width, b_txl) - 1) / TXL2WORDS(width, b_txl))
#define TXL2WORDS_4b(txls) GBI_MAX(1, ((txls) / 16))
#define CALC_DXT_4b(width) (((1 << G_TX_DXT_FRAC) + TXL2WORDS_4b(width) - 1) / TXL2WORDS_4b(width))

/* ---- commands (libultra's gDP* / gSP*, packet first) --------------------------------------- */

#define gDPNoParam(pkt, cmd) GFX_W(pkt, _SHIFTL(cmd, 24, 8), 0)
#define gDPPipeSync(pkt) gDPNoParam(pkt, G_RDPPIPESYNC)
#define gDPLoadSync(pkt) gDPNoParam(pkt, G_RDPLOADSYNC)
#define gDPTileSync(pkt) gDPNoParam(pkt, G_RDPTILESYNC)
#define gDPFullSync(pkt) gDPNoParam(pkt, G_RDPFULLSYNC)

#define gImmp1(pkt, c, p0) GFX_W(pkt, _SHIFTL(c, 24, 8), (unsigned int) (p0))

#define gSPSetOtherMode(pkt, cmd, sft, len, data) \
    GFX_W(pkt, _SHIFTL(cmd, 24, 8) | _SHIFTL(32 - (sft) - (len), 8, 8) | _SHIFTL((len) - 1, 0, 8), (unsigned int) (data))
#define gDPSetTextureLOD(pkt, type) gSPSetOtherMode(pkt, G_SETOTHERMODE_H, G_MDSFT_TEXTLOD, 1, type)
#define gDPSetTextureDetail(pkt, type) gSPSetOtherMode(pkt, G_SETOTHERMODE_H, G_MDSFT_TEXTDETAIL, 2, type)
#define gDPSetTextureLUT(pkt, type) gSPSetOtherMode(pkt, G_SETOTHERMODE_H, G_MDSFT_TEXTLUT, 2, type)
/* gDPSetCycleType(pkt, G_CYC_1CYCLE) */
#define gDPSetCycleType1Cycle(pkt) GFX_W(pkt, _SHIFTL(G_SETOTHERMODE_H, 24, 8) | _SHIFTL(10, 8, 8) | _SHIFTL(1, 0, 8), 0)
/* G_SETOTHERMODE_L, G_MDSFT_RENDERMODE */
#define gDPSetRenderMode(pkt, mode) GFX_W(pkt, 0xE200001C, mode)

#define gDPSetColor(pkt, c, d) GFX_W(pkt, _SHIFTL(c, 24, 8), (unsigned int) (d))
#define gDPSetPrimColor(pkt, m, l, r, g, b, a)                                              \
    GFX_W(pkt, (_SHIFTL(G_SETPRIMCOLOR, 24, 8) | _SHIFTL(m, 8, 8) | _SHIFTL(l, 0, 8)),      \
          (_SHIFTL(r, 24, 8) | _SHIFTL(g, 16, 8) | _SHIFTL(b, 8, 8) | _SHIFTL(a, 0, 8)))
#define gDPSetEnvColor(pkt, r, g, b, a) \
    GFX_W(pkt, _SHIFTL(G_SETENVCOLOR, 24, 8), _SHIFTL(r, 24, 8) | _SHIFTL(g, 16, 8) | _SHIFTL(b, 8, 8) | _SHIFTL(a, 0, 8))

#define gDPFillRectangle(pkt, ulx, uly, lrx, lry)                                           \
    GFX_W(pkt, (_SHIFTL(G_FILLRECT, 24, 8) | _SHIFTL((lrx), 14, 10) | _SHIFTL((lry), 2, 10)), \
          (_SHIFTL((ulx), 14, 10) | _SHIFTL((uly), 2, 10)))

#define gDPSetTextureImage(pkt, fmt, siz, width, img)                                           \
    GFX_W(pkt, _SHIFTL(G_SETTIMG, 24, 8) | _SHIFTL(fmt, 21, 3) | _SHIFTL(siz, 19, 2) |          \
                   _SHIFTL((width) - 1, 0, 12),                                                 \
          GBI_ADDR(img))
#define gDPSetTile(pkt, fmt, siz, line, tmem, tile, palette, cmt, maskt, shiftt, cms, masks, shifts)       \
    GFX_W(pkt, _SHIFTL(G_SETTILE, 24, 8) | _SHIFTL(fmt, 21, 3) | _SHIFTL(siz, 19, 2) |                     \
                   _SHIFTL(line, 9, 9) | _SHIFTL(tmem, 0, 9),                                              \
          _SHIFTL(tile, 24, 3) | _SHIFTL(palette, 20, 4) | _SHIFTL(cmt, 18, 2) | _SHIFTL(maskt, 14, 4) |    \
              _SHIFTL(shiftt, 10, 4) | _SHIFTL(cms, 8, 2) | _SHIFTL(masks, 4, 4) | _SHIFTL(shifts, 0, 4))
/* gDPLoadBlock without libultra's MIN(lrs, G_TX_LDBLK_MAX_TXL): for callers that clamp lrs themselves. */
#define gDPLoadBlockN(pkt, tile, uls, ult, lrs, dxt)                                            \
    GFX_W(pkt, _SHIFTL(G_LOADBLOCK, 24, 8) | _SHIFTL(uls, 12, 12) | _SHIFTL(ult, 0, 12),        \
          _SHIFTL(tile, 24, 3) | _SHIFTL(lrs, 12, 12) | _SHIFTL(dxt, 0, 12))
#define gDPLoadBlock(pkt, tile, uls, ult, lrs, dxt)                                             \
    GFX_W(pkt, (_SHIFTL(G_LOADBLOCK, 24, 8) | _SHIFTL(uls, 12, 12) | _SHIFTL(ult, 0, 12)),      \
          (_SHIFTL(tile, 24, 3) | _SHIFTL((GBI_MIN(lrs, G_TX_LDBLK_MAX_TXL)), 12, 12) | _SHIFTL(dxt, 0, 12)))
#define gDPSetTileSize(pkt, t, uls, ult, lrs, lrt)                                              \
    GFX_W(pkt, _SHIFTL(G_SETTILESIZE, 24, 8) | _SHIFTL(uls, 12, 12) | _SHIFTL(ult, 0, 12),      \
          _SHIFTL(t, 24, 3) | _SHIFTL(lrs, 12, 12) | _SHIFTL(lrt, 0, 12))
#define gDPLoadTLUTCmd(pkt, tile, count) GFX_W(pkt, _SHIFTL(G_LOADTLUT, 24, 8), _SHIFTL(tile, 24, 3) | _SHIFTL(count, 14, 10))

#define gDPLoadTextureBlock(pkt, timg, fmt, siz, width, height, pal, cms, cmt, masks, maskt, shifts, shiftt)        \
    {                                                                                                               \
        gDPSetTextureImage(pkt, fmt, siz##_LOAD_BLOCK, 1, timg);                                                    \
        gDPSetTile(pkt, fmt, siz##_LOAD_BLOCK, 0, 0, G_TX_LOADTILE, 0, cmt, maskt, shiftt, cms, masks, shifts);     \
        gDPLoadSync(pkt);                                                                                           \
        gDPLoadBlock(pkt, G_TX_LOADTILE, 0, 0, (((width) * (height) + siz##_INCR) >> siz##_SHIFT) - 1,              \
                     CALC_DXT(width, siz##_BYTES));                                                                 \
        gDPPipeSync(pkt);                                                                                           \
        gDPSetTile(pkt, fmt, siz, (((width) * siz##_LINE_BYTES) + 7) >> 3, 0, G_TX_RENDERTILE, pal, cmt, maskt,     \
                   shiftt, cms, masks, shifts);                                                                     \
        gDPSetTileSize(pkt, G_TX_RENDERTILE, 0, 0, ((width) - 1) << G_TEXTURE_IMAGE_FRAC,                           \
                       ((height) - 1) << G_TEXTURE_IMAGE_FRAC)                                                      \
    }
#define gDPLoadMultiBlock_4b(pkt, timg, tmem, rtile, fmt, width, height, pal, cms, cmt, masks, maskt, shifts, shiftt) \
    {                                                                                                               \
        gDPSetTextureImage(pkt, fmt, G_IM_SIZ_16b, 1, timg);                                                        \
        gDPSetTile(pkt, fmt, G_IM_SIZ_16b, 0, tmem, G_TX_LOADTILE, 0, cmt, maskt, shiftt, cms, masks, shifts);      \
        gDPLoadSync(pkt);                                                                                           \
        gDPLoadBlock(pkt, G_TX_LOADTILE, 0, 0, (((width) * (height) + 3) >> 2) - 1, CALC_DXT_4b(width));             \
        gDPPipeSync(pkt);                                                                                           \
        gDPSetTile(pkt, fmt, G_IM_SIZ_4b, ((((width) >> 1) + 7) >> 3), tmem, rtile, pal, cmt, maskt, shiftt, cms,   \
                   masks, shifts);                                                                                  \
        gDPSetTileSize(pkt, rtile, 0, 0, ((width) - 1) << G_TEXTURE_IMAGE_FRAC, ((height) - 1) << G_TEXTURE_IMAGE_FRAC) \
    }

#define gSPTexture(pkt, s, t, level, tile, on)                                                         \
    GFX_W(pkt, _SHIFTL(G_TEXTURE, 24, 8) | _SHIFTL(level, 11, 3) | _SHIFTL(tile, 8, 3) | _SHIFTL(on, 1, 7), \
          _SHIFTL(s, 16, 16) | _SHIFTL(t, 0, 16))
#define gSPGeometryMode(pkt, clear, set) \
    GFX_W(pkt, _SHIFTL(G_GEOMETRYMODE, 24, 8) | _SHIFTL(~(clear), 0, 24), (unsigned int) (set))
#define gSPDisplayList(pkt, dl) GFX_W(pkt, _SHIFTL(G_DL, 24, 8), GBI_ADDR(dl))
/* gSPMatrix(pkt, m, G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH) */
#define gSPMatrixLoad(pkt, m) GFX_W(pkt, _SHIFTL(G_MTX, 24, 8) | _SHIFTL(7, 19, 5) | _SHIFTL(2 ^ 1, 0, 8), GBI_ADDR(m))

#define gSPTextureRectangle(pkt, xl, yl, xh, yh, tile, s, t, dsdx, dtdy)                         \
    {                                                                                           \
        GFX_W(pkt, (_SHIFTL(G_TEXRECT, 24, 8) | _SHIFTL(xh, 12, 12) | _SHIFTL(yh, 0, 12)),      \
              (_SHIFTL(tile, 24, 3) | _SHIFTL(xl, 12, 12) | _SHIFTL(yl, 0, 12)));               \
        gImmp1(pkt, G_RDPHALF_1, (_SHIFTL(s, 16, 16) | _SHIFTL(t, 0, 16)));                     \
        gImmp1(pkt, G_RDPHALF_2, (_SHIFTL(dsdx, 16, 16) | _SHIFTL(dtdy, 0, 16)));               \
    }
/* libultra's gSPScisTextureRectangle: the rectangle clipped at the screen's top-left edge. */
#define gSPScisTextureRectangle(pkt, xl, yl, xh, yh, tile, s, t, dsdx, dtdy)                     \
    {                                                                                           \
        GFX_W(pkt, (_SHIFTL(G_TEXRECT, 24, 8) | _SHIFTL(GBI_MAX((s16)(xh), 0), 12, 12) |            \
                    _SHIFTL(GBI_MAX((s16)(yh), 0), 0, 12)),                                         \
              (_SHIFTL((tile), 24, 3) | _SHIFTL(GBI_MAX((s16)(xl), 0), 12, 12) |                    \
               _SHIFTL(GBI_MAX((s16)(yl), 0), 0, 12)));                                             \
        gImmp1(pkt, G_RDPHALF_1, (_SHIFTL(((s) - (((s16)(xl) < 0) ? (((s16)(dsdx) < 0)          \
                                       ? (GBI_MAX((((s16)(xl) * (s16)(dsdx)) >> 7), 0))             \
                                       : (GBI_MIN((((s16)(xl) * (s16)(dsdx)) >> 7), 0))) : 0)),     \
                              16, 16) |                                                         \
                      _SHIFTL(((t) - (((yl) < 0) ? (((s16)(dtdy) < 0)                           \
                                       ? (GBI_MAX((((s16)(yl) * (s16)(dtdy)) >> 7), 0))             \
                                       : (GBI_MIN((((s16)(yl) * (s16)(dtdy)) >> 7), 0))) : 0)),     \
                              0, 16)));                                                         \
        gImmp1(pkt, G_RDPHALF_2, (_SHIFTL((dsdx), 16, 16) | _SHIFTL((dtdy), 0, 16)));           \
    }

#endif
