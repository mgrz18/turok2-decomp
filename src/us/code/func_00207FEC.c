/* func_00207FEC, func_002086BC and func_00208810, one source file: the
 * file's .rodata opens at 0x800A52B8 with func_00207FEC's jump table, and
 * func_00208810's pool (2^31 floats for float-to-unsigned and three jump
 * tables) follows at 0x800A5334, 4 past an 8-byte boundary, which it only
 * reaches as a continuation of that pool. func_002086BC sits between the two
 * in .text, so it is in the file too. func_002086BC and func_00208810 are the
 * Rage Wars imports. */
/* func_002086BC: from AbuN64/ragewars-decomp (Turok: Rage Wars), src/func_80268CE0_de.c, func_8026992C_de. */
/* func_00208810: after AbuN64/ragewars-decomp (Turok: Rage Wars), src/RENDER_3/80268160_12.CPP, func_80269A80_de, with Turok 2's layouts. */
#include "common.h"
#include "gfx.h"

/* The libultra GBI (F3DEX2) commands this function writes, as in PR/gbi.h. */
#define _SHIFTL(v, s, w) (((unsigned int) (v) & (0xFFFFFFFFU >> (32 - (w)))) << (s))
#define _GBI_CMD(pkt, a, b) { Gfx *_gbi = (Gfx *) (pkt); _gbi->w0 = (a); _gbi->w1 = (b); }

#define G_TX_WRAP 0
#define G_TX_MIRROR 1
#define G_TX_CLAMP 2
#define G_SETOTHERMODE_H 0xE3
#define G_MDSFT_TEXTLOD 16
#define G_TL_LOD (1 << G_MDSFT_TEXTLOD)

#define gSPSetOtherMode(pkt, cmd, sft, len, data) \
    _GBI_CMD(pkt, _SHIFTL(cmd, 24, 8) | _SHIFTL(32 - (sft) - (len), 8, 8) | _SHIFTL((len) - 1, 0, 8), (unsigned int) (data))
#define gDPSetTextureLOD(pkt, data) gSPSetOtherMode(pkt, G_SETOTHERMODE_H, G_MDSFT_TEXTLOD, 1, data)
#define gDPPipeSync(pkt) _GBI_CMD(pkt, _SHIFTL(0xE7, 24, 8), 0)
#define gDPSetPrimColor(pkt, m, l, r, g, b, a)                                         \
    _GBI_CMD(pkt, _SHIFTL(0xFA, 24, 8) | _SHIFTL(m, 8, 8) | _SHIFTL(l, 0, 8),          \
             _SHIFTL(r, 24, 8) | _SHIFTL(g, 16, 8) | _SHIFTL(b, 8, 8) | _SHIFTL(a, 0, 8))
#define gDPSetEnvColor(pkt, r, g, b, a) \
    _GBI_CMD(pkt, _SHIFTL(0xFB, 24, 8), _SHIFTL(r, 24, 8) | _SHIFTL(g, 16, 8) | _SHIFTL(b, 8, 8) | _SHIFTL(a, 0, 8))
#define gSPTexture(pkt, s, t, level, tile, on) \
    _GBI_CMD(pkt, _SHIFTL(0xD7, 24, 8) | _SHIFTL(level, 11, 3) | _SHIFTL(tile, 8, 3) | _SHIFTL(on, 1, 7), _SHIFTL(s, 16, 16) | _SHIFTL(t, 0, 16))
#define gSPGeometryMode(pkt, clear, set) _GBI_CMD(pkt, _SHIFTL(0xD9, 24, 8) | _SHIFTL(~(clear), 0, 24), (unsigned int) (set))

typedef struct Material {
    /* 0x00 */ u32 flags;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 texture; /* resource handle, func_0020798C resolves it */
    /* 0x0C */ s32 unkC;
    /* 0x10 */ u8 prim[4];
    /* 0x14 */ u8 env[4];
    /* 0x18 */ u16 maskS;
    /* 0x1A */ u16 maskT;
} Material;

typedef struct TextureFrame {
    /* 0x0 */ u8 unk0;
    /* 0x1 */ u8 speed;
    /* 0x2 */ u8 shiftS;
    /* 0x3 */ u8 shiftT;
    /* 0x4 */ u8 lod;
    /* 0x5 */ u8 kind;
} TextureFrame;

typedef struct TextureInfo {
    /* 0x0 */ s32 count;
    /* 0x4 */ s32 palettes;
    /* 0x8 */ TextureFrame *frame;
} TextureInfo;


#define gDPSetRenderMode(pkt, mode) _GBI_CMD(pkt, 0xE200001C, mode) /* G_SETOTHERMODE_L, G_MDSFT_RENDERMODE */
#define gDPLoadSync(pkt) _GBI_CMD(pkt, _SHIFTL(0xE6, 24, 8), 0)
#define gDPSetTextureImage(pkt, fmt, siz, width, img) \
    _GBI_CMD(pkt, _SHIFTL(0xFD, 24, 8) | _SHIFTL(fmt, 21, 3) | _SHIFTL(siz, 19, 2) | _SHIFTL((width) - 1, 0, 12), (unsigned int) (img))
#define gDPSetTile(pkt, fmt, siz, line, tmem, tile, palette, cmt, maskt, shiftt, cms, masks, shifts)                     \
    _GBI_CMD(pkt, _SHIFTL(0xF5, 24, 8) | _SHIFTL(fmt, 21, 3) | _SHIFTL(siz, 19, 2) | _SHIFTL(line, 9, 9) | _SHIFTL(tmem, 0, 9), \
             _SHIFTL(tile, 24, 3) | _SHIFTL(palette, 20, 4) | _SHIFTL(cmt, 18, 2) | _SHIFTL(maskt, 14, 4) |                 \
             _SHIFTL(shiftt, 10, 4) | _SHIFTL(cms, 8, 2) | _SHIFTL(masks, 4, 4) | _SHIFTL(shifts, 0, 4))
#define gDPLoadBlock(pkt, tile, uls, ult, lrs, dxt) \
    _GBI_CMD(pkt, _SHIFTL(0xF3, 24, 8) | _SHIFTL(uls, 12, 12) | _SHIFTL(ult, 0, 12), \
             _SHIFTL(tile, 24, 3) | _SHIFTL(lrs, 12, 12) | _SHIFTL(dxt, 0, 12))
#define gDPSetTileSize(pkt, t, uls, ult, lrs, lrt) \
    _GBI_CMD(pkt, _SHIFTL(0xF2, 24, 8) | _SHIFTL(uls, 12, 12) | _SHIFTL(ult, 0, 12), \
             _SHIFTL(t, 24, 3) | _SHIFTL(lrs, 12, 12) | _SHIFTL(lrt, 0, 12))
/* G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH */
#define gSPMatrixLoad(pkt, m) _GBI_CMD(pkt, _SHIFTL(0xDA, 24, 8) | _SHIFTL(7, 19, 5) | _SHIFTL(2 ^ 1, 0, 8), (unsigned int) (m))
#define gSPDisplayList(pkt, dl) _GBI_CMD(pkt, _SHIFTL(0xDE, 24, 8), (unsigned int) (dl))
#define G_IM_FMT_I 4
#define G_IM_SIZ_8b 1
#define G_IM_SIZ_16b 2
#define G_TX_LOADTILE 7
#define G_TX_RENDERTILE 0
#define G_TX_NOMASK 0
#define G_TX_NOLOD 0

extern u32 D_800AB884;          /* current render mode index (func_00207FEC) */
extern s32 D_800AB890;          /* anti-aliasing enabled */
extern u32 D_800AB8A8;          /* extra blender flags */
extern u32 D_800AB8AC;          /* extra blender flags (z-buffered modes) */
extern u32 D_800AB880;
extern u8 D_800AB580[];         /* 16x16 I8 texture */
extern Gfx D_800AB458[], D_800AB4E0[];

void func_00207A54(void);

/* Switches the RDP render mode; returns 0 when it was already active. */
s32 func_00207FEC(u32 mode) {
    if (D_800AB884 == mode) {
        return 0;
    }
    D_800AB884 = mode;
    switch (mode) {
    case 0:
        if (D_800AB890) {
            gDPSetRenderMode(D_800D8E24++, 0xC1849D8);
        } else {
            gDPSetRenderMode(D_800D8E24++, 0xC184A50);
        }
        break;
    case 1:
        if (D_800AB890) {
            gDPSetRenderMode(D_800D8E24++, D_800AB8AC | 0x1049D8);
        } else {
            gDPSetRenderMode(D_800D8E24++, D_800AB8AC | 0x104A50);
        }
        break;
    case 2:
        gDPSetRenderMode(D_800D8E24++, 0xC184A50);
        break;
    case 3:
        gDPSetRenderMode(D_800D8E24++, 0xC184B50);
        break;
    case 4:
        if (D_800AB890) {
            gDPSetRenderMode(D_800D8E24++, D_800AB8AC | 0x104DD8);
        } else {
            gDPSetRenderMode(D_800D8E24++, D_800AB8AC | 0x104E50);
        }
        break;
    case 5:
        if (D_800AB890) {
            gDPSetRenderMode(D_800D8E24++, D_800AB8AC | 0x1041F8);
        } else {
            gDPSetRenderMode(D_800D8E24++, D_800AB8AC | 0x1041F0);
        }
        break;
    case 6:
        if (D_800AB890) {
            gDPSetRenderMode(D_800D8E24++, D_800AB8AC | 0x113078);
        } else {
            gDPSetRenderMode(D_800D8E24++, D_800AB8AC | 0x113078);
        }
        break;
    case 7:
        gDPSetRenderMode(D_800D8E24++, D_800AB8AC | 0x112E10);
        break;
    case 8:
        if (D_800AB890) {
            gDPSetRenderMode(D_800D8E24++, D_800AB8AC | 0x112478);
        } else {
            gDPSetRenderMode(D_800D8E24++, D_800AB8AC | 0x112478);
        }
        break;
    case 9:
        gDPSetRenderMode(D_800D8E24++, D_800AB8AC | 0x112438);
        break;
    case 10:
        if (D_800AB890) {
            gDPSetRenderMode(D_800D8E24++, D_800AB8AC | 0x112078);
        } else {
            gDPSetRenderMode(D_800D8E24++, D_800AB8AC | 0x112230);
        }
        break;
    case 11:
        gDPSetRenderMode(D_800D8E24++, D_800AB8AC | 0x112038);
        break;
    case 12:
        if (D_800AB890) {
            gDPSetRenderMode(D_800D8E24++, 0x5045D8);
        } else {
            gDPSetRenderMode(D_800D8E24++, 0x4045D0);
        }
        break;
    case 13:
        gDPSetRenderMode(D_800D8E24++, 0x504B50);
        break;
    case 14:
        if (D_800AB890) {
            gDPSetRenderMode(D_800D8E24++, 0x5049D8);
        } else {
            gDPSetRenderMode(D_800D8E24++, 0x504A50);
        }
        break;
    case 15:
        gDPSetRenderMode(D_800D8E24++, 0x504E50);
        break;
    case 16:
        gDPSetRenderMode(D_800D8E24++, 0x504B53);
        break;
    case 17:
        if (D_800AB890) {
            gDPSetRenderMode(D_800D8E24++, 0x553078);
        } else {
            gDPSetRenderMode(D_800D8E24++, 0x553078);
        }
        break;
    case 18:
        if (D_800AB890) {
            gDPSetRenderMode(D_800D8E24++, 0x553048);
        } else {
            gDPSetRenderMode(D_800D8E24++, 0xF0A7008);
        }
        break;
    case 19:
        gDPSetRenderMode(D_800D8E24++, 0x504340);
        break;
    case 20:
        gDPSetRenderMode(D_800D8E24++, 0);
        break;
    case 21:
        gDPSetRenderMode(D_800D8E24++, 0x504240);
        break;
    case 22:
        gDPSetRenderMode(D_800D8E24++, 0xFA54040);
        break;
    case 23:
        gDPSetRenderMode(D_800D8E24++, D_800AB8AC | 0x104B50);
        break;
    case 24:
        gDPSetRenderMode(D_800D8E24++, D_800AB8AC | 0x104E50);
        break;
    case 25:
        gDPSetRenderMode(D_800D8E24++, 0xC184240);
        break;
    case 26:
        gDPSetRenderMode(D_800D8E24++, 0xC184340);
        break;
    case 27:
        gDPSetRenderMode(D_800D8E24++, 0xF0A4000);
        break;
    case 28:
        if (D_800AB890) {
            gDPSetRenderMode(D_800D8E24++, 0x507048);
        } else {
            gDPSetRenderMode(D_800D8E24++, 0x507040);
        }
        break;
    case 29:
        gDPSetRenderMode(D_800D8E24++, D_800AB8AC | 0x104F50);
        break;
    case 30:
        gDPSetRenderMode(D_800D8E24++, D_800AB8A8 | 0x104F50);
        break;
    }
    return 1;
}

/* Draws a 16x16 intensity texture (D_800AB580) with `matrix` and the display
 * list for `pass`, white at `alpha`; then forces the next render and combine
 * modes to be set again. */
void func_002086BC(void *matrix, s32 pass, u32 alpha) {
    Gfx *list;

    /* gDPLoadTextureBlock(D_800AB580, G_IM_FMT_I, G_IM_SIZ_8b, 16, 16, 0, G_TX_CLAMP, G_TX_CLAMP, 0, 0, ...) */
    gDPSetTextureImage(D_800D8E24++, G_IM_FMT_I, G_IM_SIZ_16b, 1, D_800AB580);
    gDPSetTile(D_800D8E24++, G_IM_FMT_I, G_IM_SIZ_16b, 0, 0, G_TX_LOADTILE, 0, G_TX_CLAMP, G_TX_NOMASK, G_TX_NOLOD,
               G_TX_CLAMP, G_TX_NOMASK, G_TX_NOLOD);
    gDPLoadSync(D_800D8E24++);
    gDPLoadBlock(D_800D8E24++, G_TX_LOADTILE, 0, 0, 127, 1024);
    gDPPipeSync(D_800D8E24++);
    gDPSetTile(D_800D8E24++, G_IM_FMT_I, G_IM_SIZ_8b, 2, 0, G_TX_RENDERTILE, 0, G_TX_CLAMP, G_TX_NOMASK, G_TX_NOLOD,
               G_TX_CLAMP, G_TX_NOMASK, G_TX_NOLOD);
    gDPSetTileSize(D_800D8E24++, G_TX_RENDERTILE, 0, 0, 15 << 2, 15 << 2);
    gDPSetPrimColor(D_800D8E24++, 0, 0, 255, 255, 255, alpha);
    gSPMatrixLoad(D_800D8E24++, matrix);
    list = D_800AB458;
    gSPDisplayList(D_800D8E24++, ({ if (pass != 0) list = D_800AB4E0; list; }));
    D_800AB884 = -1;
    D_800AB880 = -1;
    func_00207A54();
}

extern s32 D_8011AAD8;
extern u32 D_800B6D20;
/* Scene colour mode: 0 none; otherwise an environment colour (and, for 6, a prim alpha). */
extern s32 D_800AB8B0;
extern f32 D_800AB8B4[5];
extern s32 D_800AB898;
extern s32 D_800AB89C;
extern s32 D_800AB8A4;
extern u8 D_800AB8C8;
extern u8 D_800AB8C9;
extern u8 D_800AB8CA;
extern u8 D_800AB8CB;
extern u8 D_800AB8CC;
extern u8 D_800AB8CD;

extern void *func_0020798C(s32 *texture);
extern void func_002079D8(void *node, TextureInfo *info);
extern void func_00206B30(s32 *, void *, s32, s32, s32, s32, s32, s32, s32);
extern void func_00207A70(s32 combine);
extern void func_002051F4(s32 unused, void *node);

/* Sets up the RDP for drawing with `mat`: its texture frame `pass` (or the
 * animated one when negative), colours, combiner, render and geometry modes.
 * Returns 0 when the material is invisible or its texture is not loaded. */
s32 func_00208810(Material *mat, s32 pass) {
    TextureInfo info;
    void *node;
    s32 tailEnum;
    s32 a;
    s32 b;
    u32 maskS;
    u32 maskT;
    u32 extra;
    u32 extra2;
    u32 mode;

    if ((mat->flags & 0x100) && mat->prim[3] == 0) {
        return 0;
    }
    tailEnum = -1;
    if (mat->texture != 0 && !(D_8011AAD8 & 0x400)) {
        node = func_0020798C(&mat->texture);
        if (node == 0) {
            return 0;
        }
        func_002079D8(node, &info);
        if (pass < 0) {
            pass = D_800B6D20 / info.frame->speed;
        }
        a = pass % info.count;
        if (info.palettes != 0) {
            b = pass % info.palettes;
        } else {
            b = 0;
        }
        if (mat->flags & 0x10) {
            maskS = 1 << (info.frame->shiftS + 6);
            maskT = 1 << (info.frame->shiftT + 6);
        } else {
            maskS = mat->maskS;
            maskT = mat->maskT;
        }
        if (mat->flags & 0x20) {
            extra = G_TX_CLAMP;
            extra2 = G_TX_CLAMP;
        } else {
            extra = G_TX_WRAP;
            extra2 = G_TX_WRAP;
        }
        if (mat->flags & 0x800) {
            extra |= G_TX_MIRROR;
            extra2 |= G_TX_MIRROR;
        }
        func_00206B30(&mat->texture, node, a, b, maskS, maskT, extra, extra2, 0);
        if (mat->flags & 0x2000) {
            gDPSetTextureLOD(D_800D8E24++, G_TL_LOD);
        }
        gDPSetPrimColor(D_800D8E24++, info.frame->lod, 0, mat->prim[0], mat->prim[1], mat->prim[2], mat->prim[3]);
        if (mat->flags & 0x400) {
            gDPSetEnvColor(D_800D8E24++, mat->env[0], mat->env[1], mat->env[2],
                           D_800AB8B0 == 0 ? mat->prim[3] : D_800AB8B4[3]);
            if (mat->flags & 2) {
                if (!(mat->flags & 0x1000)) {
                    func_00207A70(2);
                } else if (D_800AB8B0 == 0) {
                    func_00207A70(0);
                } else {
                    func_00207A70(1);
                }
            } else if (!(mat->flags & 0x1000)) {
                func_00207A70(4);
            } else {
                func_00207A70(3);
            }
            func_002051F4(0, node);
        } else if (mat->flags & 2) {
            if (mat->flags & 0x2000) {
                switch (info.frame->kind) {
                case 4:
                case 5:
                    gDPSetEnvColor(D_800D8E24++, mat->env[0], mat->env[1], mat->env[2], mat->env[3]);
                    func_00207A70(0x23);
                    break;
                default:
                    func_00207A70(5);
                    break;
                }
            } else if (D_800AB8B0 == 0) {
                func_00207A70(6);
            } else {
                gDPSetEnvColor(D_800D8E24++, D_800AB8B4[0], D_800AB8B4[1], D_800AB8B4[2],
                               D_800AB8B4[3]);
                switch (D_800AB8B0) {
                case 1:
                case 2:
                    func_00207A70(7);
                    break;
                case 3:
                    func_00207A70(8);
                    break;
                case 7:
                    func_00207A70(9);
                    break;
                case 4:
                case 5:
                    func_00207A70(10);
                    break;
                case 6:
                    gDPSetPrimColor(D_800D8E24++, 0, 0, mat->prim[0], mat->prim[1], mat->prim[2], D_800AB8B4[4]);
                    func_00207A70(0xB);
                    break;
                default:
                    break;
                }
            }
            func_002051F4(0, node);
        } else {
            if (mat->flags & 0x2000) {
                switch (info.frame->kind) {
                case 4:
                    gDPSetEnvColor(D_800D8E24++, mat->env[0], mat->env[1], mat->env[2], mat->env[3]);
                    func_00207A70(0x25);
                    break;
                case 5:
                    gDPSetEnvColor(D_800D8E24++, mat->env[0], mat->env[1], mat->env[2], mat->env[3]);
                    func_00207A70(0x24);
                    break;
                default:
                    func_00207A70(0xC);
                    break;
                }
            } else if (D_800AB8B0 == 0) {
                func_00207A70(0xD);
            } else {
                gDPSetEnvColor(D_800D8E24++, D_800AB8B4[0], D_800AB8B4[1], D_800AB8B4[2],
                               D_800AB8B4[3]);
                switch (D_800AB8B0) {
                case 1:
                case 2:
                    func_00207A70(0xE);
                    break;
                case 3:
                    func_00207A70(0xF);
                    break;
                case 7:
                    func_00207A70(9);
                    break;
                case 4:
                case 5:
                    func_00207A70(0x10);
                    break;
                case 6:
                    gDPSetPrimColor(D_800D8E24++, 0, 0, mat->prim[0], mat->prim[1], mat->prim[2], D_800AB8B4[4]);
                    func_00207A70(0x11);
                    break;
                default:
                    break;
                }
            }
            func_002051F4(0, node);
        }
    } else {
        gDPPipeSync(D_800D8E24++);
        if (D_800AB8C8 != 0) {
            if (D_800AB8CC != 0) {
                gDPSetPrimColor(D_800D8E24++, 0, 0, D_800AB8C9, D_800AB8CA, D_800AB8CB, D_800AB8CD);
            } else {
                gDPSetPrimColor(D_800D8E24++, 0, 0, D_800AB8C9, D_800AB8CA, D_800AB8CB, mat->prim[3]);
            }
        } else if (D_800AB8CC != 0) {
            gDPSetPrimColor(D_800D8E24++, 0, 0, mat->prim[0], mat->prim[1], mat->prim[2], D_800AB8CD);
        } else {
            gDPSetPrimColor(D_800D8E24++, 0, 0, mat->prim[0], mat->prim[1], mat->prim[2], mat->prim[3]);
        }
        gSPTexture(D_800D8E24++, 0, 0, 0, 0, 0);
        if (mat->flags & 2) {
            func_00207A70(0x12);
        } else if (D_800AB8B0 == 0) {
            func_00207A70(0x13);
        } else {
            gDPSetEnvColor(D_800D8E24++, D_800AB8B4[0], D_800AB8B4[1], D_800AB8B4[2],
                           D_800AB8B4[3]);
            switch (D_800AB8B0) {
            case 1:
            case 2:
                func_00207A70(0x14);
                break;
            case 3:
                func_00207A70(0x15);
                break;
            case 7:
                func_00207A70(0x16);
                break;
            case 4:
            case 5:
                func_00207A70(0x17);
                break;
            case 6:
                gDPSetPrimColor(D_800D8E24++, 0, 0, mat->prim[0], mat->prim[1], mat->prim[2], D_800AB8B4[4]);
                func_00207A70(0x11);
                break;
            default:
                break;
            }
        }
    }
    if ((mat->flags & 0x100) || D_800AB8B0 == 1 || D_800AB8B0 == 4 || D_800AB8B0 == 6) {
        if (D_800AB8B0 == 0) {
            tailEnum = 3;
            if (!(mat->flags & 0x1000)) {
                tailEnum = 1;
                if (mat->flags & 0x200) {
                    tailEnum = 4;
                }
            }
        } else {
            tailEnum = 5;
            if (mat->flags & 0x1000) {
                tailEnum = 3;
            }
        }
    } else if (mat->flags & 0x80) {
        func_00207FEC(6);
    } else {
        tailEnum = 7;
        if (!(mat->flags & 0x200)) {
            if (D_800AB898 != 0) {
                tailEnum = 9;
                if (D_800AB89C != 0) {
                    tailEnum = 8;
                }
            } else {
                tailEnum = 0xB;
                if (D_800AB89C != 0) {
                    tailEnum = 10;
                }
            }
        }
    }
    if (tailEnum != -1) {
        func_00207FEC(tailEnum);
    }
    mode = 0x260404;
    if (mat->flags & 0x1000) {
        mode |= D_800AB8A4;
    }
    gSPGeometryMode(D_800D8E24++, mode, 0);
    mode = 0;
    if (!(mat->flags & 0x1000)) {
        mode = D_800AB8A4;
    }
    if (D_800AB8B0 == 7) {
        mode |= 0x20000;
    }
    if (!(mat->flags & 0x42)) {
        mode |= 0x20000;
    }
    mode |= 0x200004;
    if (!(mat->flags & 4)) {
        mode |= 0x400;
    }
    if (mat->flags & 0x10) {
        mode |= 0x60000;
    }
    gSPGeometryMode(D_800D8E24++, 0, mode);
    return 1;
}
