/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/TEXTURE/80294C64_13.CPP, func_80295FB4_us_rev1, with Turok 2's layouts. */
#include "common.h"
#include "gfx.h"
#include "gbi_t2.h"
#include "globals/render.h"
#include "protos/iset.h"
#include "protos/cache.h"

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
        gDPSetTextureImage(gDList++, fmt, loadSiz, 1, texture);                                                     \
        gDPSetTile(gDList++, fmt, loadSiz, 0, 0, G_TX_LOADTILE, 0, wrapT, maskT, mode, wrapS, maskS, mode);         \
        gDPLoadSync(gDList++);                                                                                      \
        gDPLoadBlockN(gDList++, G_TX_LOADTILE, 0, 0, LOAD_LRS(incr, shift), LOAD_DXT(bytes));                        \
        gDPPipeSync(gDList++);                                                                                      \
        gDPSetTile(gDList++, fmt, renderSiz, line, 0, G_TX_RENDERTILE, 0, wrapT, maskT, mode, wrapS, maskS, mode);  \
        gDPSetTileSize(gDList++, G_TX_RENDERTILE, 0, 0, (WIDTH - 1) << 2, (HEIGHT - 1) << 2);                       \
    }
#define LOAD_TLUT(count)                                                             \
    {                                                                                \
        gDPSetTextureImage(gDList++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, palette);   \
        gDPTileSync(gDList++);                                                   \
        gDPSetTile(gDList++, 0, 0, 0, 256, G_TX_LOADTILE, 0, 0, 0, 0, 0, 0, 0); \
        gDPLoadSync(gDList++);                                                   \
        gDPLoadTLUTCmd(gDList++, G_TX_LOADTILE, count);                          \
        gDPPipeSync(gDList++);                                                   \
    }


/* Loads frame `frame` of the texture `resource` (and its palette `secondary`)
 * into TMEM, skipping what the last call already loaded, then turns texturing on. */
void Texture_Load(s32 *output, void **resource, s32 frame, s32 secondary, s32 scaleS, s32 scaleT, s32 wrapS, s32 wrapT,
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

    CCache__LockForDisplayList(0, resource);
    header = *resource;
    info = (u8 *) CISet__GetBlock(header, 0);
    texture = CISet__GetBlock(CISet__GetBlock(header, 1), frame);
    entries = CISet__GetBlock(header, 2);
    if (*(s32 *) entries != 0) {
        palette = CISet__GetBlock(entries, secondary);
        paletteChanged = 0;
        if (sLoadedTexture != *output || sTexLastPalette != secondary) paletteChanged = 1;
    } else {
        paletteChanged = 0;
        palette = 0;
    }
    changed = 0;
    if (sLoadedTexture != *output || sTexLastFrame != frame || sTexLastWrapS != wrapS || sTexLastWrapT != wrapT) {
        changed = 1;
    }
    if (changed || paletteChanged) {
        sLoadedTexture = *output;
        sTexLastFrame = frame;
        sTexLastPalette = secondary;
        sTexLastWrapS = wrapS;
        sTexLastWrapT = wrapT;
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
                gDPSetTextureLUT(gDList++, G_TT_RGBA16);
            }
            if (paletteChanged) {
                LOAD_TLUT(255);
            }
            break;
        case 1:
            if (changed) {
                LOAD_TEXTURE_BLOCK(G_IM_FMT_CI, G_IM_SIZ_16b, 3, 2, 0, G_IM_SIZ_4b, ((WIDTH >> 1) + 7) >> 3);
                gDPSetTextureLUT(gDList++, G_TT_RGBA16);
            }
            if (paletteChanged) {
                LOAD_TLUT(15);
            }
            break;
        case 3:
            if (changed) {
                LOAD_TEXTURE_BLOCK(G_IM_FMT_CI, G_IM_SIZ_16b, 3, 2, 0, G_IM_SIZ_4b, ((WIDTH >> 1) + 7) >> 3);
                gDPSetTextureLUT(gDList++, G_TT_IA16);
            }
            if (paletteChanged) {
                LOAD_TLUT(15);
            }
            break;
        case 2:
            if (changed) {
                LOAD_TEXTURE_BLOCK(G_IM_FMT_RGBA, G_IM_SIZ_32b, 0, 0, 4, G_IM_SIZ_32b, ((2 << info[2]) + 7) >> 3);
                gDPSetTextureLUT(gDList++, G_TT_NONE);
            }
            break;
        case 4:
            if (changed) {
                gDPPipeSync(gDList++);
                gDPSetTextureImage(gDList++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, texture);
                switch (info[5]) {
                case 2:
                default:
                    list = sTextureLoadDL_Mode2;
                    break;
                case 3:
                    list = sTextureLoadDL_Mode3;
                    break;
                case 1:
                    gDPSetTextureDetail(gDList++, G_TD_SHARPEN);
                    list = sTextureLoadDL_Detail;
                    if (wrapS & 1) list = sTextureLoadDL_DetailMirror;
                    break;
                case 0:
                    gDPSetTextureDetail(gDList++, G_TD_CLAMP);
                    list = sTextureLoadDL_Detail;
                    if (wrapS & 1) list = sTextureLoadDL_DetailMirror;
                    break;
                }
                gSPDisplayList(gDList++, list);
            }
            break;
        case 5:
            if (changed) {
                gDPPipeSync(gDList++);
                gDPSetTextureImage(gDList++, G_IM_FMT_I, G_IM_SIZ_16b, 1, texture);
                gSPDisplayList(gDList++, sTextureLoadDL_I16);
            }
            break;
        }
    } else {
        gDPPipeSync(gDList++);
    }
    switch (info[0]) {
    case 4: level = 5; tile = 0; break;
    case 5: level = 6; tile = 0; break;
    default: level = 0; tile = 0; break;
    }
    gSPTexture(gDList++, scaleS, scaleT, level, tile, G_ON);
}
