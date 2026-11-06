/* Render_SetRenderMode, Render_DrawShadowQuad and Render_SetupMaterial, one source file: the
 * file's .rodata opens at 0x800A52B8 with Render_SetRenderMode's jump table, and
 * Render_SetupMaterial's pool (2^31 floats for float-to-unsigned and three jump
 * tables) follows at 0x800A5334, 4 past an 8-byte boundary, which it only
 * reaches as a continuation of that pool. Render_DrawShadowQuad sits between the two
 * in .text, so it is in the file too. Render_DrawShadowQuad and Render_SetupMaterial are the
 * Rage Wars imports. */
/* Render_DrawShadowQuad: from AbuN64/ragewars-decomp (Turok: Rage Wars), src/func_80268CE0_de.c, func_8026992C_de. */
/* Render_SetupMaterial: after AbuN64/ragewars-decomp (Turok: Rage Wars), src/RENDER_3/80268160_12.CPP, func_80269A80_de, with Turok 2's layouts. */
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
    /* 0x08 */ s32 texture; /* resource handle, Texture_Request resolves it */
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

extern u32 gCurrentRenderMode;          /* current render mode index (Render_SetRenderMode) */
extern s32 gRenderAntiAlias;          /* anti-aliasing enabled */
extern u32 gFogRenderModeFogA;          /* extra blender flags */
extern u32 gFogRenderModeShadeA;          /* extra blender flags (z-buffered modes) */
extern u32 gCurrentCombineMode;
extern u8 gShadowTexture[];         /* 16x16 I8 texture */
extern Gfx sShadowQuadDL[], sShadowQuadDL2[];

void Texture_InvalidateCache(void);

/* Switches the RDP render mode; returns 0 when it was already active. */
s32 Render_SetRenderMode(u32 mode) {
    if (gCurrentRenderMode == mode) {
        return 0;
    }
    gCurrentRenderMode = mode;
    switch (mode) {
    case 0:
        if (gRenderAntiAlias) {
            gDPSetRenderMode(gDList++, 0xC1849D8);
        } else {
            gDPSetRenderMode(gDList++, 0xC184A50);
        }
        break;
    case 1:
        if (gRenderAntiAlias) {
            gDPSetRenderMode(gDList++, gFogRenderModeShadeA | 0x1049D8);
        } else {
            gDPSetRenderMode(gDList++, gFogRenderModeShadeA | 0x104A50);
        }
        break;
    case 2:
        gDPSetRenderMode(gDList++, 0xC184A50);
        break;
    case 3:
        gDPSetRenderMode(gDList++, 0xC184B50);
        break;
    case 4:
        if (gRenderAntiAlias) {
            gDPSetRenderMode(gDList++, gFogRenderModeShadeA | 0x104DD8);
        } else {
            gDPSetRenderMode(gDList++, gFogRenderModeShadeA | 0x104E50);
        }
        break;
    case 5:
        if (gRenderAntiAlias) {
            gDPSetRenderMode(gDList++, gFogRenderModeShadeA | 0x1041F8);
        } else {
            gDPSetRenderMode(gDList++, gFogRenderModeShadeA | 0x1041F0);
        }
        break;
    case 6:
        if (gRenderAntiAlias) {
            gDPSetRenderMode(gDList++, gFogRenderModeShadeA | 0x113078);
        } else {
            gDPSetRenderMode(gDList++, gFogRenderModeShadeA | 0x113078);
        }
        break;
    case 7:
        gDPSetRenderMode(gDList++, gFogRenderModeShadeA | 0x112E10);
        break;
    case 8:
        if (gRenderAntiAlias) {
            gDPSetRenderMode(gDList++, gFogRenderModeShadeA | 0x112478);
        } else {
            gDPSetRenderMode(gDList++, gFogRenderModeShadeA | 0x112478);
        }
        break;
    case 9:
        gDPSetRenderMode(gDList++, gFogRenderModeShadeA | 0x112438);
        break;
    case 10:
        if (gRenderAntiAlias) {
            gDPSetRenderMode(gDList++, gFogRenderModeShadeA | 0x112078);
        } else {
            gDPSetRenderMode(gDList++, gFogRenderModeShadeA | 0x112230);
        }
        break;
    case 11:
        gDPSetRenderMode(gDList++, gFogRenderModeShadeA | 0x112038);
        break;
    case 12:
        if (gRenderAntiAlias) {
            gDPSetRenderMode(gDList++, 0x5045D8);
        } else {
            gDPSetRenderMode(gDList++, 0x4045D0);
        }
        break;
    case 13:
        gDPSetRenderMode(gDList++, 0x504B50);
        break;
    case 14:
        if (gRenderAntiAlias) {
            gDPSetRenderMode(gDList++, 0x5049D8);
        } else {
            gDPSetRenderMode(gDList++, 0x504A50);
        }
        break;
    case 15:
        gDPSetRenderMode(gDList++, 0x504E50);
        break;
    case 16:
        gDPSetRenderMode(gDList++, 0x504B53);
        break;
    case 17:
        if (gRenderAntiAlias) {
            gDPSetRenderMode(gDList++, 0x553078);
        } else {
            gDPSetRenderMode(gDList++, 0x553078);
        }
        break;
    case 18:
        if (gRenderAntiAlias) {
            gDPSetRenderMode(gDList++, 0x553048);
        } else {
            gDPSetRenderMode(gDList++, 0xF0A7008);
        }
        break;
    case 19:
        gDPSetRenderMode(gDList++, 0x504340);
        break;
    case 20:
        gDPSetRenderMode(gDList++, 0);
        break;
    case 21:
        gDPSetRenderMode(gDList++, 0x504240);
        break;
    case 22:
        gDPSetRenderMode(gDList++, 0xFA54040);
        break;
    case 23:
        gDPSetRenderMode(gDList++, gFogRenderModeShadeA | 0x104B50);
        break;
    case 24:
        gDPSetRenderMode(gDList++, gFogRenderModeShadeA | 0x104E50);
        break;
    case 25:
        gDPSetRenderMode(gDList++, 0xC184240);
        break;
    case 26:
        gDPSetRenderMode(gDList++, 0xC184340);
        break;
    case 27:
        gDPSetRenderMode(gDList++, 0xF0A4000);
        break;
    case 28:
        if (gRenderAntiAlias) {
            gDPSetRenderMode(gDList++, 0x507048);
        } else {
            gDPSetRenderMode(gDList++, 0x507040);
        }
        break;
    case 29:
        gDPSetRenderMode(gDList++, gFogRenderModeShadeA | 0x104F50);
        break;
    case 30:
        gDPSetRenderMode(gDList++, gFogRenderModeFogA | 0x104F50);
        break;
    }
    return 1;
}

/* Draws a 16x16 intensity texture (gShadowTexture) with `matrix` and the display
 * list for `pass`, white at `alpha`; then forces the next render and combine
 * modes to be set again. */
void Render_DrawShadowQuad(void *matrix, s32 pass, u32 alpha) {
    Gfx *list;

    /* gDPLoadTextureBlock(gShadowTexture, G_IM_FMT_I, G_IM_SIZ_8b, 16, 16, 0, G_TX_CLAMP, G_TX_CLAMP, 0, 0, ...) */
    gDPSetTextureImage(gDList++, G_IM_FMT_I, G_IM_SIZ_16b, 1, gShadowTexture);
    gDPSetTile(gDList++, G_IM_FMT_I, G_IM_SIZ_16b, 0, 0, G_TX_LOADTILE, 0, G_TX_CLAMP, G_TX_NOMASK, G_TX_NOLOD,
               G_TX_CLAMP, G_TX_NOMASK, G_TX_NOLOD);
    gDPLoadSync(gDList++);
    gDPLoadBlock(gDList++, G_TX_LOADTILE, 0, 0, 127, 1024);
    gDPPipeSync(gDList++);
    gDPSetTile(gDList++, G_IM_FMT_I, G_IM_SIZ_8b, 2, 0, G_TX_RENDERTILE, 0, G_TX_CLAMP, G_TX_NOMASK, G_TX_NOLOD,
               G_TX_CLAMP, G_TX_NOMASK, G_TX_NOLOD);
    gDPSetTileSize(gDList++, G_TX_RENDERTILE, 0, 0, 15 << 2, 15 << 2);
    gDPSetPrimColor(gDList++, 0, 0, 255, 255, 255, alpha);
    gSPMatrixLoad(gDList++, matrix);
    list = sShadowQuadDL;
    gSPDisplayList(gDList++, ({ if (pass != 0) list = sShadowQuadDL2; list; }));
    gCurrentRenderMode = -1;
    gCurrentCombineMode = -1;
    Texture_InvalidateCache();
}

extern s32 gEngineApp_m_Settings;
extern u32 gFrameCountGameplay;
/* Scene colour mode: 0 none; otherwise an environment colour (and, for 6, a prim alpha). */
extern s32 gBlendState;
extern f32 gBlendState_m_Color[5];
extern s32 gRenderZModeInter;
extern s32 gRenderImageRead;
extern s32 gFogGeometryMode;
extern u8 gPrimColorOverride;
extern u8 gPrimColorOverrideR;
extern u8 gPrimColorOverrideG;
extern u8 gPrimColorOverrideB;
extern u8 gPrimAlphaOverride;
extern u8 gPrimAlphaOverrideValue;

extern void *Texture_Request(s32 *texture);
extern void Texture_GetInfo(void *node, TextureInfo *info);
extern void Texture_Load(s32 *, void *, s32, s32, s32, s32, s32, s32, s32);
extern void Render_SetCombineMode(s32 combine);
extern void CCache__Unlock(s32 unused, void *node);

/* Sets up the RDP for drawing with `mat`: its texture frame `pass` (or the
 * animated one when negative), colours, combiner, render and geometry modes.
 * Returns 0 when the material is invisible or its texture is not loaded. */
s32 Render_SetupMaterial(Material *mat, s32 pass) {
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
    if (mat->texture != 0 && !(gEngineApp_m_Settings & 0x400)) {
        node = Texture_Request(&mat->texture);
        if (node == 0) {
            return 0;
        }
        Texture_GetInfo(node, &info);
        if (pass < 0) {
            pass = gFrameCountGameplay / info.frame->speed;
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
        Texture_Load(&mat->texture, node, a, b, maskS, maskT, extra, extra2, 0);
        if (mat->flags & 0x2000) {
            gDPSetTextureLOD(gDList++, G_TL_LOD);
        }
        gDPSetPrimColor(gDList++, info.frame->lod, 0, mat->prim[0], mat->prim[1], mat->prim[2], mat->prim[3]);
        if (mat->flags & 0x400) {
            gDPSetEnvColor(gDList++, mat->env[0], mat->env[1], mat->env[2],
                           gBlendState == 0 ? mat->prim[3] : gBlendState_m_Color[3]);
            if (mat->flags & 2) {
                if (!(mat->flags & 0x1000)) {
                    Render_SetCombineMode(2);
                } else if (gBlendState == 0) {
                    Render_SetCombineMode(0);
                } else {
                    Render_SetCombineMode(1);
                }
            } else if (!(mat->flags & 0x1000)) {
                Render_SetCombineMode(4);
            } else {
                Render_SetCombineMode(3);
            }
            CCache__Unlock(0, node);
        } else if (mat->flags & 2) {
            if (mat->flags & 0x2000) {
                switch (info.frame->kind) {
                case 4:
                case 5:
                    gDPSetEnvColor(gDList++, mat->env[0], mat->env[1], mat->env[2], mat->env[3]);
                    Render_SetCombineMode(0x23);
                    break;
                default:
                    Render_SetCombineMode(5);
                    break;
                }
            } else if (gBlendState == 0) {
                Render_SetCombineMode(6);
            } else {
                gDPSetEnvColor(gDList++, gBlendState_m_Color[0], gBlendState_m_Color[1], gBlendState_m_Color[2],
                               gBlendState_m_Color[3]);
                switch (gBlendState) {
                case 1:
                case 2:
                    Render_SetCombineMode(7);
                    break;
                case 3:
                    Render_SetCombineMode(8);
                    break;
                case 7:
                    Render_SetCombineMode(9);
                    break;
                case 4:
                case 5:
                    Render_SetCombineMode(10);
                    break;
                case 6:
                    gDPSetPrimColor(gDList++, 0, 0, mat->prim[0], mat->prim[1], mat->prim[2], gBlendState_m_Color[4]);
                    Render_SetCombineMode(0xB);
                    break;
                default:
                    break;
                }
            }
            CCache__Unlock(0, node);
        } else {
            if (mat->flags & 0x2000) {
                switch (info.frame->kind) {
                case 4:
                    gDPSetEnvColor(gDList++, mat->env[0], mat->env[1], mat->env[2], mat->env[3]);
                    Render_SetCombineMode(0x25);
                    break;
                case 5:
                    gDPSetEnvColor(gDList++, mat->env[0], mat->env[1], mat->env[2], mat->env[3]);
                    Render_SetCombineMode(0x24);
                    break;
                default:
                    Render_SetCombineMode(0xC);
                    break;
                }
            } else if (gBlendState == 0) {
                Render_SetCombineMode(0xD);
            } else {
                gDPSetEnvColor(gDList++, gBlendState_m_Color[0], gBlendState_m_Color[1], gBlendState_m_Color[2],
                               gBlendState_m_Color[3]);
                switch (gBlendState) {
                case 1:
                case 2:
                    Render_SetCombineMode(0xE);
                    break;
                case 3:
                    Render_SetCombineMode(0xF);
                    break;
                case 7:
                    Render_SetCombineMode(9);
                    break;
                case 4:
                case 5:
                    Render_SetCombineMode(0x10);
                    break;
                case 6:
                    gDPSetPrimColor(gDList++, 0, 0, mat->prim[0], mat->prim[1], mat->prim[2], gBlendState_m_Color[4]);
                    Render_SetCombineMode(0x11);
                    break;
                default:
                    break;
                }
            }
            CCache__Unlock(0, node);
        }
    } else {
        gDPPipeSync(gDList++);
        if (gPrimColorOverride != 0) {
            if (gPrimAlphaOverride != 0) {
                gDPSetPrimColor(gDList++, 0, 0, gPrimColorOverrideR, gPrimColorOverrideG, gPrimColorOverrideB, gPrimAlphaOverrideValue);
            } else {
                gDPSetPrimColor(gDList++, 0, 0, gPrimColorOverrideR, gPrimColorOverrideG, gPrimColorOverrideB, mat->prim[3]);
            }
        } else if (gPrimAlphaOverride != 0) {
            gDPSetPrimColor(gDList++, 0, 0, mat->prim[0], mat->prim[1], mat->prim[2], gPrimAlphaOverrideValue);
        } else {
            gDPSetPrimColor(gDList++, 0, 0, mat->prim[0], mat->prim[1], mat->prim[2], mat->prim[3]);
        }
        gSPTexture(gDList++, 0, 0, 0, 0, 0);
        if (mat->flags & 2) {
            Render_SetCombineMode(0x12);
        } else if (gBlendState == 0) {
            Render_SetCombineMode(0x13);
        } else {
            gDPSetEnvColor(gDList++, gBlendState_m_Color[0], gBlendState_m_Color[1], gBlendState_m_Color[2],
                           gBlendState_m_Color[3]);
            switch (gBlendState) {
            case 1:
            case 2:
                Render_SetCombineMode(0x14);
                break;
            case 3:
                Render_SetCombineMode(0x15);
                break;
            case 7:
                Render_SetCombineMode(0x16);
                break;
            case 4:
            case 5:
                Render_SetCombineMode(0x17);
                break;
            case 6:
                gDPSetPrimColor(gDList++, 0, 0, mat->prim[0], mat->prim[1], mat->prim[2], gBlendState_m_Color[4]);
                Render_SetCombineMode(0x11);
                break;
            default:
                break;
            }
        }
    }
    if ((mat->flags & 0x100) || gBlendState == 1 || gBlendState == 4 || gBlendState == 6) {
        if (gBlendState == 0) {
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
        Render_SetRenderMode(6);
    } else {
        tailEnum = 7;
        if (!(mat->flags & 0x200)) {
            if (gRenderZModeInter != 0) {
                tailEnum = 9;
                if (gRenderImageRead != 0) {
                    tailEnum = 8;
                }
            } else {
                tailEnum = 0xB;
                if (gRenderImageRead != 0) {
                    tailEnum = 10;
                }
            }
        }
    }
    if (tailEnum != -1) {
        Render_SetRenderMode(tailEnum);
    }
    mode = 0x260404;
    if (mat->flags & 0x1000) {
        mode |= gFogGeometryMode;
    }
    gSPGeometryMode(gDList++, mode, 0);
    mode = 0;
    if (!(mat->flags & 0x1000)) {
        mode = gFogGeometryMode;
    }
    if (gBlendState == 7) {
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
    gSPGeometryMode(gDList++, 0, mode);
    return 1;
}
