/* WRONG CUT: splat cut a symbol at 0x206984 (428 bytes): its first 12 bytes are junk fill
 * ("m_YScale|" text), the real function starts at the 16-aligned 0x206990 (Texture_DecompressCallback), 416 bytes.
 * A load callback (Texture_RequestSync/Texture_Request pass its address): loads a block of RGBA5551 textures and,
 * by the mode in the top two bits of the request word 2, recolours every pixel:
 * 1 swaps red and green, 2 copies red into green, 3 copies green into red. */

#include "common.h"
#include "protos/iset.h"

s32 ProPackDecompress(void **res, s32 flag);
#ifdef TARGET_PORT
#include "structs/cache.h"
/* the request is the CCacheEntry being completed (CDecompressor__Main passes it): the view at
 * its host offsets (round 19 h2). The N64 view read the flags at 0x8, which on the host is the
 * high half of referencedMemEntry (0): no texture was ever recoloured on the port. */
typedef struct {
    union {
        struct { s32 **file; }; /* memEntry: its first word is the data pointer (N64 0x0) */
        struct { u8 _hp_flags[__builtin_offsetof(CCacheEntry, requestId)]; u32 flags; }; /* N64 0x8 */
    };
} LoadRequest;
#else
typedef struct {
    s32 **file;   /* 0x0 */
    s32 unk4;     /* 0x4 */
    u32 flags;    /* 0x8: top two bits = recolour mode */
} LoadRequest;
#endif

void Texture_DecompressCallback(s32 unused, LoadRequest *req) {
    s32 *file;
    s32 *table;
    u16 *pixels;
    u16 *pixel;
    s32 mode;
    u32 size;
    s32 count;
    s32 i;
    s32 j;
    s32 n;
    s32 type;
    u32 r;
    u32 g;
    u32 b;
    u32 a;

    if (ProPackDecompress((void **) req, 0) == 0) {
        return;
    }
    if ((req->flags >> 30) == 0) {
        return;
    }
    file = *req->file;
    mode = req->flags >> 30;
    if (*file == 0) {
        return;
    }
    type = *(u8 *)CISet__GetBlock(file, 0);
    if (type >= 2) {
        return;
    }
    if (type < 0) {
        return;
    }
    table = (s32 *) CISet__GetBlock(file, 2);
    count = *table;
    for (i = 0; i < count; i++) {
        pixels = (u16 *)CISet__GetBlockAndSize(table, i, (s32 *)&size);
        n = size >> 1;
        for (j = 0; j < n; j++) {
            pixel = &pixels[j];
#ifdef TARGET_PORT
            /* round 19 h2: the TLUT entries stay big-endian in the port's memory
             * (tools/assets/swap_spec.json "rdp": Fast3D reads them as bytes), so the recolour
             * reads and writes each RGBA5551 entry big-endian; a host u16 access would take the
             * green/blue byte for red and write the entry back byte-swapped. */
            {
                u8 *be = (u8 *) pixel;
                u32 v = ((u32) be[0] << 8) | be[1];
                r = (v >> 8) & 0xF8;
                g = (v >> 3) & 0xF8;
                b = (v << 2) & 0xF8;
                a = v & 1;
                switch (mode) {
                case 1:
                    v = (g << 8) | (r << 3) | (b >> 2) | a;
                    break;
                case 2:
                    v = (r << 8) | (r << 3) | (b >> 2) | a;
                    break;
                case 3:
                    v = (g << 8) | (g << 3) | (b >> 2) | a;
                    break;
                }
                be[0] = (u8) (v >> 8);
                be[1] = (u8) v;
            }
#else
            r = (*pixel >> 8) & 0xF8;
            g = (*pixel >> 3) & 0xF8;
            b = (*pixel << 2) & 0xF8;
            a = *pixel & 1;
            switch (mode) {
            case 1:
                *pixel = (g << 8) | (r << 3) | (b >> 2) | a;
                break;
            case 2:
                *pixel = (r << 8) | (r << 3) | (b >> 2) | a;
                break;
            case 3:
                *pixel = (g << 8) | (g << 3) | (b >> 2) | a;
                break;
            }
#endif
        }
    }
}

#include "gfx.h"
#include "gbi_t2.h"
#include "globals/render.h"
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
#ifdef TARGET_PORT
const void *T2Port_TextureCI4IA16(const void *texels, const void *palette, int w, int h);
#endif

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
#ifdef TARGET_PORT
            /* round 19 h2: CI4 with an IA16 palette, which Fast3D decodes as RGBA5551 (random
             * colours): load the texels expanded to IA16 instead (port/game/t2tex.c) */
            {
                const void *ia = T2Port_TextureCI4IA16(texture, palette, WIDTH, HEIGHT);
                if (ia != NULL) {
                    if (changed || paletteChanged) {
                        gDPSetTextureImage(gDList++, G_IM_FMT_IA, G_IM_SIZ_16b, 1, ia);
                        gDPSetTile(gDList++, G_IM_FMT_IA, G_IM_SIZ_16b, 0, 0, G_TX_LOADTILE, 0, wrapT, maskT, mode,
                                   wrapS, maskS, mode);
                        gDPLoadSync(gDList++);
                        gDPLoadBlock(gDList++, G_TX_LOADTILE, 0, 0, WIDTH * HEIGHT - 1, LOAD_DXT(2));
                        gDPPipeSync(gDList++);
                        gDPSetTile(gDList++, G_IM_FMT_IA, G_IM_SIZ_16b, ((WIDTH * 2) + 7) >> 3, 0, G_TX_RENDERTILE, 0,
                                   wrapT, maskT, mode, wrapS, maskS, mode);
                        gDPSetTileSize(gDList++, G_TX_RENDERTILE, 0, 0, (WIDTH - 1) << 2, (HEIGHT - 1) << 2);
                        gDPSetTextureLUT(gDList++, G_TT_NONE);
                    }
                    break;
                }
            }
#endif
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

#include "structs/texture.h"

void func_00207908(TextureRef *arg0, s32 arg1, s32 arg2) {
    arg0->unk0 = arg1;
    arg0->unk4 = arg2;
}

#include "m2c_macros.h"
#include "globals/scene.h"
#include "protos/scene.h"

/* arg0: &Material.texture (the ROM address, then its size at +4): an address, sptr on the host. */
void Material_GetTextureSet(sptr arg0, s32 arg1) {
    CScene__GetTextureSet(&gScene, arg1, (s32 *)arg0, arg0 + 4);
}

#include "globals/strings.h"

/* ABI (docs/port/ABI.md, abicheck void_used): the callers use the result, on MIPS v0 of
 * the last call; the host returns it. */
sptr Texture_RequestSync(TextureRef *arg0) {
    return CCache__Request(0, arg0->unk0, arg0->unk4, 0, 0, &Texture_DecompressCallback, &sTextureCacheDesc_Texture, 1);
}

/* ABI (docs/port/ABI.md, abicheck void_used): the callers use the result, on MIPS v0 of
 * the last call; the host returns it. */
void * Texture_Request(TextureRef *arg0) {
    return CCache__Request(0, arg0->unk0, arg0->unk4, 0, 0, &Texture_DecompressCallback, &sTextureCacheDesc_Texture, 0);
}

void Texture_GetInfo(s32 **arg0, TextureInfo *arg1) {
    s32 *temp_s0;

    temp_s0 = *arg0;
    if (*temp_s0 != 0) {
        arg1->frame = CISet__GetBlock(temp_s0, 0);
        arg1->count = (s32) *(s32 *)CISet__GetBlock(temp_s0, 1);
        arg1->palettes = (s32) *(s32 *)CISet__GetBlock(temp_s0, 2);
        return;
    }
    arg1->frame = NULL;
    arg1->count = 0;
    arg1->palettes = 0;
}

void Texture_InvalidateCache(void) {
    sLoadedTexture = 0;
}
