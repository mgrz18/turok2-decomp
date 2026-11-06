#include "common.h"
#include "gfx.h"
#include "structs/gbi.h"
#include "structs/vector.h"
#include "structs/region.h"
#include "structs/world.h"
#include "globals/scene.h"
#include "globals/render.h"

/* The automap: the level's navigation regions are drawn as lines in a 2D
 * orthographic view around the player, with an arrow for the player and a
 * compass in the corner. */

#define GFX(a, b)                       \
    {                                   \
        Gfx *_g = gDList++;         \
        _g->w0 = (a);                   \
        _g->w1 = (b);                   \
    }

/* gSPVertex(pkt, v, n, 0) and gSPLine3D(pkt, v0, v1, 0) (F3DEX2). */
#define VTX_W0(n) (0x01000000 | (((n) & 0xFF) << 12) | (((n) & 0x7F) << 1))
#define LINE_W0(a, b) (0x08000000 | (((((a) * 2) & 0xFF) << 16) | ((((b) * 2) & 0xFF) << 8)))

typedef struct MapVtx {
    /* 0x0 */ s16 ob[3];
    /* 0x6 */ u16 flag;
    /* 0x8 */ s16 tc[2];
    /* 0xC */ u8 cn[4];
} MapVtx; /* 0x10 bytes */

typedef struct {
    /* 0x00 */ u8 pad0[8];
    /* 0x08 */ f32 fade;
    /* 0x0C */ s32 visible;
    /* 0x10 */ f32 angle;
    /* 0x14 */ CVector3 pos;
    /* 0x20 */ CVector3 offset;
} MapView;

typedef struct {
    /* 0x00 */ f32 minX;
    /* 0x04 */ f32 minZ;
    /* 0x08 */ f32 maxX;
    /* 0x0C */ f32 maxZ;
    /* 0x10 */ u32 count;
} Area; /* 0x14 bytes */

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 count;
    /* 0x08 */ Area areas[1];
} AreaList;

typedef struct {
    /* 0x000 */ u8 pad0[0x110];
    /* 0x110 */ void *colorImage;
} FrameBuffers;

extern s32 gEvenOdd;      /* buffer index */
extern s32 gPersistentDataLoaded;
extern FrameBuffers *gEngineApp_m_pCurrentFrameData[]; /* CFrameData * (structs/frame.h); declared as an array here: as a scalar the code is scheduled differently (gEngineApp.m_pCurrentFrameData would need game.h, whose gEngineApp_m_Settings is not this file's s32) */
extern s32 gEngineApp_m_Settings;
extern s32 gEngineApp_m_Settings_nextMode;
extern Vp gScreenViewport[];
extern Mtx sAutomapWorldMtx[2];
extern Mtx sAutomapScreenMtx[2];
extern u16 sAutomapLineIndices[];    /* line vertex indices */
extern Mtx sAutomapOrthoMtx;
extern MapVtx sAutomapLineVtx[2][0x200];
extern MapVtx sAutomapMagentaVtx[2][8];
extern MapVtx sAutomapYellowVtx[2][3];
extern u8 gRspInitDL[];
extern u8 gRdpInitDL[];

extern char *CISet__GetBlock(s32 *data, s32 index);
extern void CMtxF__InvertAffine(f32 *out, f32 *m);
extern void CMtxF__ToMtxAffine(f32 *m, Mtx *out);
extern void CMtxF__TransformPoint(f32 *m, CVector3 *in, CVector3 *out);
extern void CMtxF__SetTranslate(f32 *m, f32 x, f32 y, f32 z);
extern void CMtxF__SetScale(f32 *m, f32 x, f32 y, f32 z);
extern void CMtxF__Translate(f32 *m, f32 x, f32 y, f32 z);
extern void CMtxF__ScaleColumns(f32 *m, f32 x, f32 y, f32 z);
extern void CMtxF__RotateZ(f32 *m, f32 angle);
extern f32 func_002974C0(f32 x); /* cos */
extern void guOrtho(Mtx *m, f32 l, f32 r, f32 b, f32 t, f32 n, f32 f, f32 scale);
extern f32 func_002982F0(f32 x); /* sin */

#define MIN(a, b) ((b) < (a) ? (b) : (a))
#define MAX(a, b) ((a) < (b) ? (b) : (a))

static __inline__ s32 TestFlag(s32 flags, s32 bit) {
    return flags & bit;
}

static __inline__ s32 EdgeVisible(CVector3 *corners, CVector3 *p0, CVector3 *p1) {
    CVector3 *a;
    CVector3 *b;
    f32 dx;
    f32 dz;
    s32 k;

    for (k = 0; k < 4; k++) {
        a = &corners[k];
        b = (k != 3) ? &corners[k + 1] : &corners[0];
        dz = b->y - a->y;
        dx = a->x - b->x;
        if (dz * (p0->x - a->x) + dx * (p0->z - a->y) > 0.0f &&
            dz * (p1->x - a->x) + dx * (p1->z - a->y) > 0.0f) {
            return 0;
        }
    }
    return 1;
}

void Automap_Draw(MapView *view) {
    s32 codes[3];
    f32 world[16];
    f32 inverse[16];
    f32 screen[16];
    struct {
        f32 minX, minZ, maxX, maxZ;
    } box;
    CVector3 corners[4];
    CVector3 viewCorners[4];
    CGameRegion *regions;
    s32 areaCount;
    Vp *vp;
    s32 showAll;
    Area *areas;
    Area *area;
    CGameRegion *region;
    CRegionInfo *infos;
    CRegionInfo *info;
    MapVtx *vtx;
    CVector3 *p0;
    CVector3 *p1;
    CVector3 *a;
    char *data;
    CScene *w;
    f32 s;
    f32 c;
    f32 cx;
    f32 scaleX;
    f32 scaleZ;
    f32 cz;
    f32 alpha;
    f32 len;
    f32 radius;
    f32 stem;
    f32 spread;
    f32 wid;
    s32 i;
    s32 n;
    s32 e;
    s32 next;
    s32 newVtx;
    s32 nVtx;
    s32 nIdx;
    s32 base;
    s32 height;
    s32 bright;
    s32 fade;
    s32 remaining;
    s32 count;
    s32 k;
    s32 *code;

    vp = &gScreenViewport[gEvenOdd];
    if (gEngineApp_m_Settings_nextMode == 1) {
        return;
    }
    guOrtho(&sAutomapOrthoMtx, 0.0f, gScreenWidth - 1, 0.0f, gScreenHeight - 1, -200.0f, 200.0f, 32.0f);
    if (view->visible == 0 || gPersistentDataLoaded == 0) {
        return;
    }
    GFX(0xDB060000, 0);                                 /* gSPSegment(0, 0) */
    GFX(0xDE000000, (u32) gRspInitDL);                  /* gSPDisplayList */
    GFX(0xDE000000, (u32) gRdpInitDL);
    GFX(0xED000000, ((((s32) ((f32) (gScreenWidth - 1) * 4.0f)) & 0xFFF) << 12) |
                        (((s32) ((f32) (gScreenHeight - 1) * 4.0f)) & 0xFFF)); /* gDPSetScissor */
    GFX(0xFF100000 | ((gScreenWidth - 1) & 0xFFF), (u32) gEngineApp_m_pCurrentFrameData[0]->colorImage); /* gDPSetColorImage */
    GFX(0xDC080008, (u32) vp);                          /* gSPViewport */
    GFX(0xE7000000, 0);                                 /* gDPPipeSync */
    GFX(0xE3000A01, 0);                                 /* gDPSetCycleType(G_CYC_1CYCLE) */
    GFX(0xE200001C, 0x507048);                          /* gDPSetRenderMode */
    GFX(0xFCFFFFFF, 0xFFFE793C);                        /* gDPSetCombineMode(G_CC_SHADE, G_CC_SHADE) */
    GFX(0xD9000000, 0);                                 /* gSPClearGeometryMode(~0) */
    GFX(0xD9FFFFFF, 0x200004);                          /* gSPSetGeometryMode(G_SHADE | G_SHADING_SMOOTH) */
    GFX(0xD7000000, 0);                                 /* gSPTexture(0, 0, 0, 0, G_OFF) */
    GFX(0xDB040004, 6);                                 /* gSPClipRatio(FRUSTRATIO_6) */
    GFX(0xDB04000C, 6);
    GFX(0xDB040014, 0xFFFA);
    GFX(0xDB04001C, 0xFFFA);
    GFX(0xDA380007, (u32) &sAutomapOrthoMtx);                 /* gSPMatrix(projection, load) */

    cx = view->pos.x;
    cz = view->pos.z;
    s = func_002982F0(view->angle);
    c = func_002974C0(view->angle);
    cx += view->offset.z * s - view->offset.x * c;
    cz += view->offset.z * c + view->offset.x * s;
    scaleX = scaleZ = 0.1f;
    cx = MAX(MIN(cx, 32000.0f), -32000.0f);
    cz = MAX(MIN(cz, 32000.0f), -32000.0f);
    CMtxF__SetTranslate(world, -cx, -cz, 0.0f);
    CMtxF__ScaleColumns(world, scaleX, scaleZ, 1.0f);
    CMtxF__RotateZ(world, view->angle + 3.1415929f);
    {
        f32 centreY = gScreenHeight / 2;

        centreY -= 16.0f;
        CMtxF__Translate(world, gScreenWidth / 2, centreY, 0.0f);
    }
    CMtxF__InvertAffine(inverse, world);

    corners[0].x = 0.0f;
    corners[0].y = 0.0f;
    corners[0].z = 0.0f;
    corners[1].x = gScreenWidth;
    corners[1].y = 0.0f;
    corners[1].z = 0.0f;
    corners[2].x = gScreenWidth;
    corners[2].y = gScreenHeight;
    corners[2].z = 0.0f;
    corners[3].x = 0.0f;
    corners[3].y = gScreenHeight;
    corners[3].z = 0.0f;
    for (i = 0; i < 4; i++) {
        CMtxF__TransformPoint(inverse, &corners[i], &viewCorners[i]);
    }
    box.minX = box.maxX = viewCorners[0].x;
    box.minZ = box.maxZ = viewCorners[0].y;
    for (i = 1; i < 4; i++) {
        a = &viewCorners[i];
        box.minX = !(a->x <= box.minX) ? box.minX : a->x;
        box.minZ = !(a->y <= box.minZ) ? box.minZ : a->y;
        box.maxX = !(box.maxX <= a->x) ? box.maxX : a->x;
        box.maxZ = !(box.maxZ <= a->y) ? box.maxZ : a->y;
    }

    CMtxF__ToMtxAffine(world, &sAutomapWorldMtx[gEvenOdd]);
    GFX(0xDA380003, (u32) &sAutomapWorldMtx[gEvenOdd]);    /* gSPMatrix(modelview, load) */

    vtx = sAutomapLineVtx[gEvenOdd];
    w = &gScene;
    regions = (CGameRegion *) (CISet__GetBlock(w->m_pRegionData, 2) + 8);
    data = CISet__GetBlock(w->m_pRegionData, 3);
    areas = (Area *) (data + 8);
    areaCount = *(s32 *) (data + 4);
    infos = (CRegionInfo *) (CISet__GetBlock(w->m_pRegionData, 0) + 8);
    nIdx = 0;
    base = 0;
    nVtx = 0;
    showAll = TestFlag(gEngineApp_m_Settings, 0x2000) != 0;
    for (n = 0; n < areaCount; n++) {
        area = &areas[n];
        if (area->maxX > box.minX && area->minX < box.maxX && area->maxZ > box.minZ && area->minZ < box.maxZ) {
            for (i = 0; i < area->count; i++) {
                region = &regions[base];
                info = &infos[region->m_nInfo];
                if (info->m_dwFlags & 0x200) {
                    goto next;
                }
                /* Outcodes of the region's corners against the view box. */
                for (k = 0; k < 3; k++) {
                    code = &codes[k];
                    p0 = region->m_pVertices[k];
                    if (p0->x < box.minX) {
                        *code = 1;
                    } else if (box.maxX <= p0->x) {
                        *code = 4;
                    } else {
                        *code = 0;
                    }
                    if (p0->z < box.minZ) {
                        *code |= 8;
                    } else if (box.maxZ <= p0->z) {
                        *code |= 2;
                    }
                }
                if (codes[0] & codes[1] & codes[2]) {
                    goto next;
                }
                if (!showAll && !(region->m_wFlags & 0x400)) {
                    goto next;
                }
                for (e = 0; e < 3; e++) {
                    newVtx = 0;
                    if (region->m_pNeighbours[e] == NULL) {
                        newVtx = 1;
                    } else if (region->m_nInfo < region->m_pNeighbours[e]->m_nInfo) {
                        newVtx = 1;
                    }
                    if (!newVtx) {
                        continue;
                    }
                    next = 0;
                    if (e != 2) {
                        next = e + 1;
                    }
                    if (codes[e] & codes[next]) {
                        continue;
                    }
                    p0 = region->m_pVertices[e];
                    p1 = region->m_pVertices[next];
                    if (!EdgeVisible(viewCorners, p0, p1)) {
                        continue;
                    }
                    newVtx = 1;
                    if (nVtx != 0 && vtx[nVtx - 1].ob[0] == (s16) p0->x) {
                        newVtx = vtx[nVtx - 1].ob[1] != (s16) p0->z;
                    }
                    if (newVtx) {
                        sAutomapLineIndices[nIdx++] = nVtx;
                        vtx[nVtx].ob[0] = p0->x;
                        vtx[nVtx].ob[1] = p0->z;
                        height = (p0->y - view->pos.y) * 0.390625f;
                        bright = height + 200;
                        fade = 255;
                        if (bright < 150) {
                            bright = 150;
                        } else if (bright >= 256) {
                            bright = 255;
                        }
                        if (height > 50) {
                            fade -= height - 50;
                            if (fade < 75) {
                                fade = 75;
                            }
                        } else if (height < -50) {
                            fade += height - 50;
                            if (fade < 75) {
                                fade = 75;
                            }
                        }
                        switch (info->m_MapColour) {
                            case 0:
                                vtx[nVtx].cn[0] = bright;
                                vtx[nVtx].cn[1] = bright;
                                vtx[nVtx].cn[2] = bright;
                                break;
                            case 1:
                                vtx[nVtx].cn[0] = 0;
                                vtx[nVtx].cn[1] = bright;
                                vtx[nVtx].cn[2] = 0;
                                break;
                            case 2:
                                vtx[nVtx].cn[0] = 0;
                                vtx[nVtx].cn[1] = 0;
                                vtx[nVtx].cn[2] = bright;
                                break;
                            case 3:
                                vtx[nVtx].cn[0] = bright;
                                vtx[nVtx].cn[1] = 0;
                                vtx[nVtx].cn[2] = 0;
                                break;
                            case 4:
                                vtx[nVtx].cn[0] = bright;
                                vtx[nVtx].cn[1] = 0;
                                vtx[nVtx].cn[2] = bright / 2;
                                break;
                            case 5:
                                vtx[nVtx].cn[0] = bright;
                                vtx[nVtx].cn[1] = 0;
                                vtx[nVtx].cn[2] = bright;
                                break;
                            case 6:
                                vtx[nVtx].cn[0] = bright / 2;
                                vtx[nVtx].cn[1] = bright / 4;
                                vtx[nVtx].cn[2] = 0;
                                break;
                            case 7:
                                vtx[nVtx].cn[0] = 0;
                                vtx[nVtx].cn[1] = bright;
                                vtx[nVtx].cn[2] = bright;
                                break;
                            case 8:
                                vtx[nVtx].cn[0] = bright;
                                vtx[nVtx].cn[1] = bright;
                                vtx[nVtx].cn[2] = 0;
                                break;
                            case 9:
                                vtx[nVtx].cn[0] = bright;
                                vtx[nVtx].cn[1] = bright / 2;
                                vtx[nVtx].cn[2] = 0;
                                break;
                            case 10:
                                vtx[nVtx].cn[0] = 0;
                                vtx[nVtx].cn[1] = 0;
                                vtx[nVtx].cn[2] = 0;
                                break;
                        }
                        vtx[nVtx].cn[3] = fade * view->fade;
                        nVtx++;
                    } else {
                        sAutomapLineIndices[nIdx++] = nVtx - 1;
                    }
                    sAutomapLineIndices[nIdx++] = nVtx;
                    vtx[nVtx].ob[0] = p1->x;
                    vtx[nVtx].ob[1] = p1->z;
                    height = (p0->y - view->pos.y) * 0.390625f;
                    bright = height + 200;
                    fade = 255;
                    if (bright < 150) {
                        bright = 150;
                    } else if (bright >= 256) {
                        bright = 255;
                    }
                    if (height > 50) {
                        fade -= height - 50;
                        if (fade < 75) {
                            fade = 75;
                        }
                    } else if (height < -50) {
                        fade += height - 50;
                        if (fade < 75) {
                            fade = 75;
                        }
                    }
                    switch (info->m_MapColour) {
                        case 0:
                            vtx[nVtx].cn[0] = bright;
                            vtx[nVtx].cn[1] = bright;
                            vtx[nVtx].cn[2] = bright;
                            break;
                        case 1:
                            vtx[nVtx].cn[0] = 0;
                            vtx[nVtx].cn[1] = bright;
                            vtx[nVtx].cn[2] = 0;
                            break;
                        case 2:
                            vtx[nVtx].cn[0] = 0;
                            vtx[nVtx].cn[1] = 0;
                            vtx[nVtx].cn[2] = bright;
                            break;
                        case 3:
                            vtx[nVtx].cn[0] = bright;
                            vtx[nVtx].cn[1] = 0;
                            vtx[nVtx].cn[2] = 0;
                            break;
                        case 4:
                            vtx[nVtx].cn[0] = bright;
                            vtx[nVtx].cn[1] = 0;
                            vtx[nVtx].cn[2] = bright / 2;
                            break;
                        case 5:
                            vtx[nVtx].cn[0] = bright;
                            vtx[nVtx].cn[1] = 0;
                            vtx[nVtx].cn[2] = bright;
                            break;
                        case 6:
                            vtx[nVtx].cn[0] = bright / 2;
                            vtx[nVtx].cn[1] = bright / 4;
                            vtx[nVtx].cn[2] = 0;
                            break;
                        case 7:
                            vtx[nVtx].cn[0] = 0;
                            vtx[nVtx].cn[1] = bright;
                            vtx[nVtx].cn[2] = bright;
                            break;
                        case 8:
                            vtx[nVtx].cn[0] = bright;
                            vtx[nVtx].cn[1] = bright;
                            vtx[nVtx].cn[2] = 0;
                            break;
                        case 9:
                            vtx[nVtx].cn[0] = bright;
                            vtx[nVtx].cn[1] = bright / 2;
                            vtx[nVtx].cn[2] = 0;
                            break;
                        case 10:
                            vtx[nVtx].cn[0] = 0;
                            vtx[nVtx].cn[1] = 0;
                            vtx[nVtx].cn[2] = 0;
                            break;
                    }
                    vtx[nVtx].cn[3] = fade * view->fade;
                    nVtx++;
                    if (nVtx >= 0x1FF) {
                        goto draw;
                    }
                }
            next: /* skipped regions still advance base */
                base++;
            }
        } else {
            base += area->count;
        }
    }
draw:
    /* Draw the lines in batches of up to 32 vertices; nVtx now counts the
     * batch's first vertex. */
    remaining = nVtx;
    nVtx = 0;
    i = 0;
    while (remaining >= 2) {
        count = MIN(remaining, 32);
        GFX(VTX_W0(count), (u32) &vtx[nVtx]);
        while (sAutomapLineIndices[i + 1] - nVtx < count && i < nIdx - 1) {
            GFX(LINE_W0(sAutomapLineIndices[i] - nVtx, sAutomapLineIndices[i + 1] - nVtx), 0);
            i += 2;
        }
        if (sAutomapLineIndices[i + 1] - nVtx == count) {
            count--;
        }
        remaining -= count;
        nVtx += count;
    }

    /* The player's arrow. */
    vtx = sAutomapYellowVtx[gEvenOdd];
    len = 40.0f;
    wid = 25.0f;
    alpha = view->fade * 255.0f;
    vtx[0].ob[0] = view->pos.x - s * len;
    vtx[0].ob[1] = view->pos.z - c * len;
    vtx[1].ob[0] = view->pos.x + c * wid + s * len;
    vtx[1].ob[1] = view->pos.z - s * wid + c * len;
    vtx[2].ob[0] = view->pos.x - c * wid + s * len;
    vtx[2].ob[1] = view->pos.z + s * wid + c * len;
    vtx[0].cn[3] = alpha;
    vtx[1].cn[3] = alpha;
    vtx[2].cn[3] = alpha;
    GFX(VTX_W0(3), (u32) vtx);
    GFX(LINE_W0(0, 1), 0);
    GFX(LINE_W0(0, 2), 0);

    /* The compass. */
    vtx = sAutomapMagentaVtx[gEvenOdd];
    CMtxF__SetScale(screen, 0.5f, 0.5f, 1.0f);
    CMtxF__Translate(screen, gScreenWidth / 2, 32.0f, 0.0f);
    CMtxF__ToMtxAffine(screen, &sAutomapScreenMtx[gEvenOdd]);
    GFX(0xDA380003, (u32) &sAutomapScreenMtx[gEvenOdd]);
    alpha = view->fade * 255.0f;
    radius = 15.0f;
    vtx[0].ob[0] = func_002982F0(view->angle) * radius;
    vtx[0].ob[1] = func_002974C0(view->angle) * -15.0f;
    stem = 40.0f;
    vtx[1].ob[0] = vtx[0].ob[0] - func_002982F0(-view->angle) * stem;
    vtx[1].ob[1] = vtx[0].ob[1] - func_002974C0(-view->angle) * stem;
    spread = 0.4f;
    vtx[2].ob[0] = vtx[1].ob[0] + func_002982F0(-view->angle - spread) * radius;
    vtx[2].ob[1] = vtx[1].ob[1] + func_002974C0(-view->angle - spread) * radius;
    vtx[3].ob[0] = vtx[1].ob[0] + func_002982F0(spread - view->angle) * radius;
    vtx[3].ob[1] = vtx[1].ob[1] + func_002974C0(spread - view->angle) * radius;
    vtx[4].ob[0] = -7;
    vtx[4].ob[1] = -7;
    vtx[5].ob[0] = vtx[4].ob[0];
    vtx[5].ob[1] = vtx[4].ob[1] + 15;
    vtx[6].ob[0] = vtx[5].ob[0] + 15;
    vtx[6].ob[1] = vtx[5].ob[1] - 15;
    vtx[7].ob[0] = vtx[6].ob[0];
    vtx[7].ob[1] = vtx[6].ob[1] + 15;
    vtx[0].cn[3] = alpha;
    vtx[1].cn[3] = alpha;
    vtx[2].cn[3] = alpha;
    vtx[3].cn[3] = alpha;
    vtx[4].cn[3] = alpha;
    vtx[5].cn[3] = alpha;
    vtx[6].cn[3] = alpha;
    vtx[7].cn[3] = alpha;
    GFX(VTX_W0(8), (u32) vtx);
    GFX(LINE_W0(0, 1), 0);
    GFX(LINE_W0(1, 2), 0);
    GFX(LINE_W0(1, 3), 0);
    GFX(LINE_W0(4, 5), 0);
    GFX(LINE_W0(5, 6), 0);
    GFX(LINE_W0(6, 7), 0);
}
