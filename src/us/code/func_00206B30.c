/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/TEXTURE/80294C64_13.CPP, func_80295FB4_us_rev1, with Turok 2's layouts. */
#include "common.h"
#include "gfx.h"

/* The libultra GBI (F3DEX2) commands this function writes, as in PR/gbi.h. */
#define _SHIFTL(v, s, w) (((unsigned int) (v) & (0xFFFFFFFFU >> (32 - (w)))) << (s))
#define _GBI_CMD(pkt, a, b) { Gfx *_gbi = (Gfx *) (pkt); _gbi->w0 = (a); _gbi->w1 = (b); }

#define G_IM_FMT_RGBA 0
#define G_IM_FMT_CI 2
#define G_IM_FMT_I 4
#define G_IM_SIZ_4b 0
#define G_IM_SIZ_8b 1
#define G_IM_SIZ_16b 2
#define G_IM_SIZ_32b 3
#define G_TX_LOADTILE 7
#define G_TX_RENDERTILE 0
#define G_TX_CLAMP 2
#define G_TX_DXT_FRAC 11
#define G_ON 1
#define G_SETOTHERMODE_H 0xE3
#define G_MDSFT_TEXTDETAIL 17
#define G_MDSFT_TEXTLUT 14
#define G_TT_NONE (0 << G_MDSFT_TEXTLUT)
#define G_TT_RGBA16 (2 << G_MDSFT_TEXTLUT)
#define G_TT_IA16 (3 << G_MDSFT_TEXTLUT)
#define G_TD_CLAMP (0 << G_MDSFT_TEXTDETAIL)
#define G_TD_SHARPEN (1 << G_MDSFT_TEXTDETAIL)

#define gSPSetOtherMode(pkt, cmd, sft, len, data) \
    _GBI_CMD(pkt, _SHIFTL(cmd, 24, 8) | _SHIFTL(32 - (sft) - (len), 8, 8) | _SHIFTL((len) - 1, 0, 8), (unsigned int) (data))
#define gDPSetTextureDetail(pkt, data) gSPSetOtherMode(pkt, G_SETOTHERMODE_H, G_MDSFT_TEXTDETAIL, 2, data)
#define gDPSetTextureLUT(pkt, data) gSPSetOtherMode(pkt, G_SETOTHERMODE_H, G_MDSFT_TEXTLUT, 2, data)
#define gDPPipeSync(pkt) _GBI_CMD(pkt, _SHIFTL(0xE7, 24, 8), 0)
#define gDPLoadSync(pkt) _GBI_CMD(pkt, _SHIFTL(0xE6, 24, 8), 0)
#define gDPTileSync(pkt) _GBI_CMD(pkt, _SHIFTL(0xE8, 24, 8), 0)
#define gDPSetTextureImage(pkt, fmt, siz, width, img) \
    _GBI_CMD(pkt, _SHIFTL(0xFD, 24, 8) | _SHIFTL(fmt, 21, 3) | _SHIFTL(siz, 19, 2) | _SHIFTL((width) - 1, 0, 12), (unsigned int) (img))
#define gDPSetTile(pkt, fmt, siz, line, tmem, tile, palette, cmt, maskt, shiftt, cms, masks, shifts) \
    _GBI_CMD(pkt, _SHIFTL(0xF5, 24, 8) | _SHIFTL(fmt, 21, 3) | _SHIFTL(siz, 19, 2) | _SHIFTL(line, 9, 9) | _SHIFTL(tmem, 0, 9), \
             _SHIFTL(tile, 24, 3) | _SHIFTL(palette, 20, 4) | _SHIFTL(cmt, 18, 2) | _SHIFTL(maskt, 14, 4) | _SHIFTL(shiftt, 10, 4) | \
             _SHIFTL(cms, 8, 2) | _SHIFTL(masks, 4, 4) | _SHIFTL(shifts, 0, 4))
#define gDPSetTileSize(pkt, tile, uls, ult, lrs, lrt) \
    _GBI_CMD(pkt, _SHIFTL(0xF2, 24, 8) | _SHIFTL(uls, 12, 12) | _SHIFTL(ult, 0, 12), _SHIFTL(tile, 24, 3) | _SHIFTL(lrs, 12, 12) | _SHIFTL(lrt, 0, 12))
#define gDPLoadBlock(pkt, tile, uls, ult, lrs, dxt) \
    _GBI_CMD(pkt, _SHIFTL(0xF3, 24, 8) | _SHIFTL(uls, 12, 12) | _SHIFTL(ult, 0, 12), _SHIFTL(tile, 24, 3) | _SHIFTL(lrs, 12, 12) | _SHIFTL(dxt, 0, 12))
#define gDPLoadTLUTCmd(pkt, tile, count) _GBI_CMD(pkt, _SHIFTL(0xF0, 24, 8), _SHIFTL(tile, 24, 3) | _SHIFTL(count, 14, 10))
#define gSPDisplayList(pkt, dl) _GBI_CMD(pkt, _SHIFTL(0xDE, 24, 8) | _SHIFTL(0, 16, 8), (unsigned int) (dl))
#define gSPTexture(pkt, s, t, level, tile, on) \
    _GBI_CMD(pkt, _SHIFTL(0xD7, 24, 8) | _SHIFTL(level, 11, 3) | _SHIFTL(tile, 8, 3) | _SHIFTL(on, 1, 7), _SHIFTL(s, 16, 16) | _SHIFTL(t, 0, 16))

/* gDPLoadTextureBlock's expressions, for a 2^info[2] x 2^info[3] texture. */
#define WIDTH (1 << info[2])
#define HEIGHT (1 << info[3])
#define LOAD_LRS(incr, shift) \
    ((((WIDTH * HEIGHT + (incr)) >> (shift)) - 1) < 2047 ? (((WIDTH * HEIGHT + (incr)) >> (shift)) - 1) : 2047)
#define LINE_WORDS(bytes) ((bytes) == 0 ? (WIDTH / 16) : ((WIDTH * (bytes)) / 8))
#define LOAD_DXT(bytes) \
    (((1 << G_TX_DXT_FRAC) + (1 > LINE_WORDS(bytes) ? 1 : LINE_WORDS(bytes)) - 1) / (1 > LINE_WORDS(bytes) ? 1 : LINE_WORDS(bytes)))
#define LOAD_TEXTURE_BLOCK(fmt, loadSiz, incr, shift, bytes, renderSiz, line)                                          \
    {                                                                                                                   \
        gDPSetTextureImage(D_800D8E24++, fmt, loadSiz, 1, texture);                                                     \
        gDPSetTile(D_800D8E24++, fmt, loadSiz, 0, 0, G_TX_LOADTILE, 0, wrapT, maskT, mode, wrapS, maskS, mode);         \
        gDPLoadSync(D_800D8E24++);                                                                                      \
        gDPLoadBlock(D_800D8E24++, G_TX_LOADTILE, 0, 0, LOAD_LRS(incr, shift), LOAD_DXT(bytes));                        \
        gDPPipeSync(D_800D8E24++);                                                                                      \
        gDPSetTile(D_800D8E24++, fmt, renderSiz, line, 0, G_TX_RENDERTILE, 0, wrapT, maskT, mode, wrapS, maskS, mode);  \
        gDPSetTileSize(D_800D8E24++, G_TX_RENDERTILE, 0, 0, (WIDTH - 1) << 2, (HEIGHT - 1) << 2);                       \
    }
#define LOAD_TLUT(count)                                                             \
    {                                                                                \
        gDPSetTextureImage(D_800D8E24++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, palette);   \
        gDPTileSync(D_800D8E24++);                                                   \
        gDPSetTile(D_800D8E24++, 0, 0, 0, 256, G_TX_LOADTILE, 0, 0, 0, 0, 0, 0, 0); \
        gDPLoadSync(D_800D8E24++);                                                   \
        gDPLoadTLUTCmd(D_800D8E24++, G_TX_LOADTILE, count);                          \
        gDPPipeSync(D_800D8E24++);                                                   \
    }

extern void func_0020565C(s32 unused, void **resource);
extern s32 *func_002017D4(void *table, s32 index);
extern s32 D_800AB570;
extern s32 D_800D8E00;
extern s32 D_800D8E04;
extern s32 D_800D8E08;
extern s32 D_800D8E0C;
extern Gfx D_800AB190[], D_800AB210[], D_800AB290[], D_800AB328[], D_800AB3C0[];

/* Loads frame `frame` of the texture `resource` (and its palette `secondary`)
 * into TMEM, skipping what the last call already loaded, then turns texturing on. */
void func_00206B30(s32 *output, void **resource, s32 frame, s32 secondary, s32 scaleS, s32 scaleT, s32 wrapS, s32 wrapT,
                   s32 mode) {
    s32 *texture;
    s32 *palette;
    void *header;
    void *entries;
    u8 *info;
    s32 changed;
    s32 paletteChanged;
    u8 maskS;
    u8 maskT;
    s32 level;
    s32 tile;
    Gfx *list;

    func_0020565C(0, resource);
    header = *resource;
    info = (u8 *) func_002017D4(header, 0);
    texture = func_002017D4(func_002017D4(header, 1), frame);
    entries = func_002017D4(header, 2);
    if (*(s32 *) entries != 0) {
        palette = func_002017D4(entries, secondary);
        paletteChanged = 0;
        if (D_800AB570 != *output || D_800D8E04 != secondary) paletteChanged = 1;
    } else {
        paletteChanged = 0;
        palette = 0;
    }
    changed = 0;
    if (D_800AB570 != *output || D_800D8E00 != frame || D_800D8E08 != wrapS || D_800D8E0C != wrapT) {
        changed = 1;
    }
    if (changed || paletteChanged) {
        D_800AB570 = *output;
        D_800D8E00 = frame;
        D_800D8E04 = secondary;
        D_800D8E08 = wrapS;
        D_800D8E0C = wrapT;
        maskS = 0;
        if (!(wrapS & G_TX_CLAMP)) {
            maskS = info[2];
        }
        maskT = 0;
        if (!(wrapT & G_TX_CLAMP)) {
            maskT = info[3];
        }
        switch (info[0]) {
        case 0:
            if (changed) {
                LOAD_TEXTURE_BLOCK(G_IM_FMT_CI, G_IM_SIZ_16b, 1, 1, 1, G_IM_SIZ_8b, (WIDTH + 7) >> 3);
                gDPSetTextureLUT(D_800D8E24++, G_TT_RGBA16);
            }
            if (paletteChanged) {
                LOAD_TLUT(255);
            }
            break;
        case 1:
            if (changed) {
                LOAD_TEXTURE_BLOCK(G_IM_FMT_CI, G_IM_SIZ_16b, 3, 2, 0, G_IM_SIZ_4b, ((WIDTH >> 1) + 7) >> 3);
                gDPSetTextureLUT(D_800D8E24++, G_TT_RGBA16);
            }
            if (paletteChanged) {
                LOAD_TLUT(15);
            }
            break;
        case 3:
            if (changed) {
                LOAD_TEXTURE_BLOCK(G_IM_FMT_CI, G_IM_SIZ_16b, 3, 2, 0, G_IM_SIZ_4b, ((WIDTH >> 1) + 7) >> 3);
                gDPSetTextureLUT(D_800D8E24++, G_TT_IA16);
            }
            if (paletteChanged) {
                LOAD_TLUT(15);
            }
            break;
        case 2:
            if (changed) {
                LOAD_TEXTURE_BLOCK(G_IM_FMT_RGBA, G_IM_SIZ_32b, 0, 0, 4, G_IM_SIZ_32b, ((2 << info[2]) + 7) >> 3);
                gDPSetTextureLUT(D_800D8E24++, G_TT_NONE);
            }
            break;
        case 4:
            if (changed) {
                gDPPipeSync(D_800D8E24++);
                gDPSetTextureImage(D_800D8E24++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, texture);
                switch (info[5]) {
                case 2:
                default:
                    list = D_800AB290;
                    break;
                case 3:
                    list = D_800AB328;
                    break;
                case 1:
                    gDPSetTextureDetail(D_800D8E24++, G_TD_SHARPEN);
                    list = D_800AB190;
                    if (wrapS & 1) list = D_800AB210;
                    break;
                case 0:
                    gDPSetTextureDetail(D_800D8E24++, G_TD_CLAMP);
                    list = D_800AB190;
                    if (wrapS & 1) list = D_800AB210;
                    break;
                }
                gSPDisplayList(D_800D8E24++, list);
            }
            break;
        case 5:
            if (changed) {
                gDPPipeSync(D_800D8E24++);
                gDPSetTextureImage(D_800D8E24++, G_IM_FMT_I, G_IM_SIZ_16b, 1, texture);
                gSPDisplayList(D_800D8E24++, D_800AB3C0);
            }
            break;
        }
    } else {
        gDPPipeSync(D_800D8E24++);
    }
    switch (info[0]) {
    case 4: level = 5; tile = 0; break;
    case 5: level = 6; tile = 0; break;
    default: level = 0; tile = 0; break;
    }
    gSPTexture(D_800D8E24++, scaleS, scaleT, level, tile, G_ON);
}
