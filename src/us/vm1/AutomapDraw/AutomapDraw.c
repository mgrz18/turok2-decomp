#include "common.h"
#include "structs/region.h"
#include "globals/scene.h"
#include "protos/scene.h"
#include "globals/game.h"

extern u8 D_800C894C; /* bottom of the stack */
#ifdef TARGET_PORT
/* the N64 compares the stack pointer with its main stack's bottom; the host asks its own thread's
 * stack (t2os_stack_low, port/runtime/t2os.c: the address comparison depended on the layout) */
int t2os_stack_low(const void *local);
#define T2_STACK_LOW(local, bottom) t2os_stack_low(local)
#else
#define T2_STACK_LOW(local, bottom) ((uptr) (local) < (uptr) (bottom))
#endif
#define PLAYER_POS gEngineApp_m_Game.m_Automap.m_vPlayerPos
#define NEAR(p) (((p)->x - PLAYER_POS.x) * ((p)->x - PLAYER_POS.x) + ((p)->z - PLAYER_POS.z) * ((p)->z - PLAYER_POS.z) < 167772.14f)

/* Marks the regions around the camera, recursing into neighbours that have
 * a corner within ~410 units of it. */
void Automap_RevealRegion(CGameRegion *region) {
    if (region == NULL || T2_STACK_LOW(&region, &D_800C894C)) {
        return;
    }
    if (!(region->m_wFlags & 0x400)) {
        CScene__SetLevelBit(&gScene, CScene__GetRegionIndex(&gScene, region), 1);
    }
    region->m_wFlags |= 0x8400;
    if (!(NEAR(region->m_pVertices[0]) || NEAR(region->m_pVertices[1]) || NEAR(region->m_pVertices[2]))) {
        return;
    }
    if (region->m_pNeighbours[0] != NULL && !(region->m_pNeighbours[0]->m_wFlags & 0x8000) && (REGION_WORD(region->m_pNeighbours[0]) & 6) != 2) {
        Automap_RevealRegion(region->m_pNeighbours[0]);
    }
    if (region->m_pNeighbours[1] != NULL && !(region->m_pNeighbours[1]->m_wFlags & 0x8000) && (REGION_WORD(region->m_pNeighbours[1]) & 6) != 2) {
        Automap_RevealRegion(region->m_pNeighbours[1]);
    }
    if (region->m_pNeighbours[2] != NULL && !(region->m_pNeighbours[2]->m_wFlags & 0x8000) && (REGION_WORD(region->m_pNeighbours[2]) & 6) != 2) {
        Automap_RevealRegion(region->m_pNeighbours[2]);
    }
}

#include "structs/camera.h"
#include "structs/player.h"
#include "structs/game.h"
#include "globals/engine.h"
#include "protos/player.h"

/* Free-look camera driven by the C buttons. */
typedef struct {
    /* 0x00 */ s32 mode;
    /* 0x04 */ s32 state;
    /* 0x08 */ f32 blend;
    /* 0x0C */ u8 pad0C[0x20 - 0xC];
    /* 0x20 */ f32 yaw;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 pitch;
    /* 0x2C */ f32 yawTarget;
    /* 0x30 */ f32 unk30;
    /* 0x34 */ f32 pitchTarget;
} Look004001D0;
s32 MenuPool_CountOpen(u8 *a0);
void Math_EaseToward(f32 *value, f32 target, f32 rate);

void Automap_Update(Look004001D0 *look) {
    CPlayer *player;
    s32 held;
    GameSettings *sub = &gEngineApp_m_Game.m_Settings;

    if (sub->multiplayer) {
        return;
    }
    player = CPlayerPool__GetFirstActive(&gEngineApp_m_Game.m_PlayerList);
    if (player == NULL) {
        return;
    }
    if (look->mode == 0) {
        if (!MenuPool_CountOpen((u8 *)player->m_pCamera->m_MenuPool) && (player->m_ContState.m_dwButtonsSingle & 0x20) && player->m_SniperMode == 0) {
            look->mode = 1;
        }
    } else if (player->m_SniperMode != 0) {
        look->mode = 0;
    } else if (look->mode == 1) {
        if ((player->m_ContState.m_dwButtonsSingleNoHold & 0x20) || !(player->m_ContState.m_dwButtons & 0x20)) {
            look->mode = 2;
        }
    } else if (player->m_ContState.m_dwButtonsSingleNoHold & 0x20) {
        look->mode = 0;
    }
    if (look->mode != 0 && gEngineApp_m_Settings.mode == 13 && gEngineApp_m_Settings.paused == 0) {
        if (look->state != 1 && look->state != 2) {
            look->state = 1;
        }
    } else if (look->state != 0 && look->state != 3) {
        look->state = 3;
    }
    if (look->state != 0) {
        held = player->m_ContState.m_dwButtons;
        if (held & 0x20) {
            if (held & 0x200) {
                look->yawTarget += gFrameIncrement * 153.59999f;
            } else if (held & 0x100) {
                look->yawTarget -= gFrameIncrement * 153.59999f;
            }
            held = player->m_ContState.m_dwButtons;
            if (held & 0x800) {
                look->pitchTarget += gFrameIncrement * 153.59999f;
            } else if (held & 0x400) {
                look->pitchTarget -= gFrameIncrement * 153.59999f;
            }
        } else {
            look->yawTarget = 0.0f;
            look->unk30 = 0.0f;
            look->pitchTarget = 0.0f;
        }
        Math_EaseToward(&look->yaw, look->yawTarget, 0.33333334f);
        Math_EaseToward(&look->pitch, look->pitchTarget, 0.25f);
    }
    switch (look->state) {
        case 2:
            break;
        case 1:
            look->blend += gFrameIncrement * 0.13333334f;
            if (look->blend >= 1.0f) {
                look->blend = 1.0f;
                look->state = 2;
            }
            break;
        case 3:
            look->blend -= gFrameIncrement * 0.26666668f;
            if (look->blend <= 0.0f) {
                look->blend = 0.0f;
                look->state = 0;
                look->yaw = 0.0f;
                look->unk24 = 0.0f;
                look->pitch = 0.0f;
                look->yawTarget = 0.0f;
                look->unk30 = 0.0f;
                look->pitchTarget = 0.0f;
            }
            break;
    }
}

#include "gfx.h"
#include "gbi_t2.h"
#include "structs/gbi.h"
#include "structs/vector.h"
#include "structs/world.h"
#include "globals/render.h"
#include "globals/automap.h"
#include "protos/iset.h"
/* The automap: the level's navigation regions are drawn as lines in a 2D
 * orthographic view around the player, with an arrow for the player and a
 * compass in the corner. */


/* gSPVertex(pkt, v, n, 0) and gSPLine3D(pkt, v0, v1, 0) (F3DEX2). */
#define VTX_W0(n) (0x01000000 | (((n) & 0xFF) << 12) | (((n) & 0x7F) << 1))
#define LINE_W0(a, b) (0x08000000 | (((((a) * 2) & 0xFF) << 16) | ((((b) * 2) & 0xFF) << 8)))

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
extern Vp gScreenViewport[];
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

#ifdef TARGET_PORT
/* Round 20 i1: Fast3D has no G_LINE3D (the N64 draws the automap with the L3DEX2 line microcode:
 * libultraship logs the 0x08 commands as unhandled and draws nothing, so the map was never seen
 * on the port). Each line becomes a quad, two triangles `half` model units to each side of it
 * (half a screen pixel of the N64's frame under the line's modelview), drawn with the same
 * vertex colours and modes; quads go out 8 to a gSPVertex (32 vertices). The quads live until
 * the frame is drawn: one buffer per gEvenOdd, as the line vertices. */
#define T2_MAP_QUADS 1536
static Vtx_t sT2MapQuadVtx[2][T2_MAP_QUADS * 4];
static s32 sT2MapQuadCount;  /* quads in this frame's buffer */
static s32 sT2MapQuadFirst;  /* the first one not yet sent */

static void T2Automap_Flush(void) {
    Vtx_t *buf = sT2MapQuadVtx[gEvenOdd];
    s32 n;
    s32 q;

    while (sT2MapQuadFirst < sT2MapQuadCount) {
        n = GBI_MIN(sT2MapQuadCount - sT2MapQuadFirst, 8);
        GFX_CMD(VTX_W0(n * 4), GBI_ADDR(&buf[sT2MapQuadFirst * 4]));
        for (q = 0; q < n; q++) {
            s32 v = q * 4 * 2;
            GFX_CMD(0x06000000 | ((v + 0) << 16) | ((v + 2) << 8) | (v + 4),
                    ((v + 0) << 16) | ((v + 4) << 8) | (v + 6)); /* gSP2Triangles */
        }
        sT2MapQuadFirst += n;
    }
}

static void T2Automap_Line(Vtx_t *a, Vtx_t *b, f32 half) {
    Vtx_t *q;
    f32 dx = (f32) (b->ob[0] - a->ob[0]);
    f32 dy = (f32) (b->ob[1] - a->ob[1]);
    f32 len = __builtin_sqrtf(dx * dx + dy * dy);
    s32 nx;
    s32 ny;
    s32 k;

    if (sT2MapQuadCount >= T2_MAP_QUADS) {
        return;
    }
    if (len < 0.001f) {
        dx = 1.0f, dy = 0.0f, len = 1.0f;
    }
    nx = (s32) (-dy / len * half + (dy <= 0.0f ? 0.5f : -0.5f));
    ny = (s32) (dx / len * half + (dx >= 0.0f ? 0.5f : -0.5f));
    if (nx == 0 && ny == 0) {
        nx = 1;
    }
    q = &sT2MapQuadVtx[gEvenOdd][sT2MapQuadCount * 4];
    q[0] = *a, q[1] = *a, q[2] = *b, q[3] = *b;
    q[0].ob[0] += nx, q[0].ob[1] += ny;
    q[1].ob[0] -= nx, q[1].ob[1] -= ny;
    q[2].ob[0] -= nx, q[2].ob[1] -= ny;
    q[3].ob[0] += nx, q[3].ob[1] += ny;
    for (k = 0; k < 4; k++) {
        q[k].flag = 0;
    }
    sT2MapQuadCount++;
}
#define T2_MAP_LINE(vtxs, i0, i1, half) T2Automap_Line(&(vtxs)[i0], &(vtxs)[i1], half)
#endif

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
    Vtx_t *vtx;
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
#ifdef TARGET_PORT
    sT2MapQuadCount = 0;
    sT2MapQuadFirst = 0;
#endif
    GFX_CMD(0xDB060000, 0);                                 /* gSPSegment(0, 0) */
    GFX_CMD(0xDE000000, GBI_ADDR(gRspInitDL));                  /* gSPDisplayList */
    GFX_CMD(0xDE000000, GBI_ADDR(gRdpInitDL));
    GFX_CMD(0xED000000, ((((s32) ((f32) (gScreenWidth - 1) * 4.0f)) & 0xFFF) << 12) |
                        (((s32) ((f32) (gScreenHeight - 1) * 4.0f)) & 0xFFF)); /* gDPSetScissor */
    GFX_CMD(0xFF100000 | ((gScreenWidth - 1) & 0xFFF), GBI_ADDR(gEngineApp.m_pCurrentFrameData->m_pFrameBuffer)); /* gDPSetColorImage */
    GFX_CMD(0xDC080008, GBI_ADDR(vp));                          /* gSPViewport */
    GFX_CMD(0xE7000000, 0);                                 /* gDPPipeSync */
    GFX_CMD(0xE3000A01, 0);                                 /* gDPSetCycleType(G_CYC_1CYCLE) */
    GFX_CMD(0xE200001C, 0x507048);                          /* gDPSetRenderMode */
    GFX_CMD(0xFCFFFFFF, 0xFFFE793C);                        /* gDPSetCombineMode(G_CC_SHADE, G_CC_SHADE) */
    GFX_CMD(0xD9000000, 0);                                 /* gSPClearGeometryMode(~0) */
    GFX_CMD(0xD9FFFFFF, 0x200004);                          /* gSPSetGeometryMode(G_SHADE | G_SHADING_SMOOTH) */
    GFX_CMD(0xD7000000, 0);                                 /* gSPTexture(0, 0, 0, 0, G_OFF) */
    GFX_CMD(0xDB040004, 6);                                 /* gSPClipRatio(FRUSTRATIO_6) */
    GFX_CMD(0xDB04000C, 6);
    GFX_CMD(0xDB040014, 0xFFFA);
    GFX_CMD(0xDB04001C, 0xFFFA);
    GFX_CMD(0xDA380007, GBI_ADDR(&sAutomapOrthoMtx));                 /* gSPMatrix(projection, load) */

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
    GFX_CMD(0xDA380003, GBI_ADDR(&sAutomapWorldMtx[gEvenOdd]));    /* gSPMatrix(modelview, load) */

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
    showAll = TestFlag(gEngineApp_m_Settings_cheatFlags, 0x2000) != 0;
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
#ifdef TARGET_PORT
    /* the same lines (every index pair), as quads: the world matrix scales by 0.1, so half a
     * pixel is 5 units */
    for (i = 0; i < nIdx - 1 && nVtx >= 2; i += 2) {
        T2_MAP_LINE(vtx, sAutomapLineIndices[i], sAutomapLineIndices[i + 1], 5.0f);
    }
#else
    remaining = nVtx;
    nVtx = 0;
    i = 0;
    while (remaining >= 2) {
        count = MIN(remaining, 32);
        GFX_CMD(VTX_W0(count), GBI_ADDR(&vtx[nVtx]));
        while (sAutomapLineIndices[i + 1] - nVtx < count && i < nIdx - 1) {
            GFX_CMD(LINE_W0(sAutomapLineIndices[i] - nVtx, sAutomapLineIndices[i + 1] - nVtx), 0);
            i += 2;
        }
        if (sAutomapLineIndices[i + 1] - nVtx == count) {
            count--;
        }
        remaining -= count;
        nVtx += count;
    }
#endif

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
#ifdef TARGET_PORT
    T2_MAP_LINE(vtx, 0, 1, 5.0f);
    T2_MAP_LINE(vtx, 0, 2, 5.0f);
    T2Automap_Flush(); /* before the compass's matrix */
#else
    GFX_CMD(VTX_W0(3), GBI_ADDR(vtx));
    GFX_CMD(LINE_W0(0, 1), 0);
    GFX_CMD(LINE_W0(0, 2), 0);
#endif

    /* The compass. */
    vtx = sAutomapMagentaVtx[gEvenOdd];
    CMtxF__SetScale(screen, 0.5f, 0.5f, 1.0f);
    CMtxF__Translate(screen, gScreenWidth / 2, 32.0f, 0.0f);
    CMtxF__ToMtxAffine(screen, &sAutomapScreenMtx[gEvenOdd]);
    GFX_CMD(0xDA380003, GBI_ADDR(&sAutomapScreenMtx[gEvenOdd]));
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
#ifdef TARGET_PORT
    T2_MAP_LINE(vtx, 0, 1, 1.0f);
    T2_MAP_LINE(vtx, 1, 2, 1.0f);
    T2_MAP_LINE(vtx, 1, 3, 1.0f);
    T2_MAP_LINE(vtx, 4, 5, 1.0f);
    T2_MAP_LINE(vtx, 5, 6, 1.0f);
    T2_MAP_LINE(vtx, 6, 7, 1.0f);
    T2Automap_Flush();
#else
    GFX_CMD(VTX_W0(8), GBI_ADDR(vtx));
    GFX_CMD(LINE_W0(0, 1), 0);
    GFX_CMD(LINE_W0(1, 2), 0);
    GFX_CMD(LINE_W0(1, 3), 0);
    GFX_CMD(LINE_W0(4, 5), 0);
    GFX_CMD(LINE_W0(5, 6), 0);
    GFX_CMD(LINE_W0(6, 7), 0);
#endif
}

typedef struct {
    s32 v[3];
} Triple;
typedef struct {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ s32 a;
    /* 0x10 */ s32 b;
    /* 0x14 */ Triple c;
} Obj00401B30;
void Automap_RevealAroundPlayer(void *automap);
#ifdef TARGET_PORT
/* The port: the caller's types (CPlayerPool__Update: the region, the facing as f32 and the
 * position as a CVector3 by value; as s32 and the Rage Wars Triple they arrived in other
 * registers on x86-64). The region goes to CAutomap.m_pHostRegion, the N64 word keeps
 * whether there is one (Automap_Draw tests it). */
#include "structs/game.h"
#include "structs/vector.h"
void Automap_SetPlayer(Obj00401B30 *obj, struct CGameRegion *a, f32 b, CVector3 c) {
    obj->a = a != NULL;
    *(f32 *) &obj->b = b;
    *(CVector3 *) &obj->c = c;
    ((CAutomap *) obj)->m_pHostRegion = a;
    Automap_RevealAroundPlayer(obj);
}
#else

void Automap_SetPlayer(Obj00401B30 *obj, sptr a, s32 b, Triple c) {
    obj->a = a;
    obj->b = b;
    obj->c = c;
    Automap_RevealAroundPlayer(obj);
}
#endif
#include "structs/game.h"
/* The region Automap_SetPlayer keeps: on the N64 the word at 0xC (m_bHasRegion's: a region pointer is
 * non-zero), on the host CAutomap.m_pHostRegion (no room for a host pointer there). */
#ifdef TARGET_PORT
#define AUTOMAP_REGION(a) ((a)->m_pHostRegion)
#else
#define AUTOMAP_REGION(a) (*(CGameRegion **) &(a)->m_bHasRegion)
#endif

void Automap_ClearVisited(CGameRegion *arg0);

/* The caller's CAutomap (structs/game.h): around the region Automap_SetPlayer keeps (AUTOMAP_REGION). */
void Automap_RevealAroundPlayer(void *automap) {
    CAutomap *arg0 = automap;
    CGameRegion *temp_s0;
    CGameRegion *temp_a0;
    CGameRegion *temp_a0_2;
    CGameRegion *temp_a0_3;

    Automap_RevealRegion((sptr) AUTOMAP_REGION(arg0));
    temp_s0 = AUTOMAP_REGION(arg0);
    if (temp_s0 != 0) {
        temp_a0 = temp_s0->m_pNeighbours[0];
        temp_s0->m_wFlags = (u16) (temp_s0->m_wFlags & 0x7FFF);
        if ((temp_a0 != NULL) && (temp_a0->m_wFlags & 0x8000)) {
            Automap_ClearVisited((sptr) temp_a0);
        }
        temp_a0_2 = temp_s0->m_pNeighbours[1];
        if ((temp_a0_2 != NULL) && (temp_a0_2->m_wFlags & 0x8000)) {
            Automap_ClearVisited((sptr) temp_a0_2);
        }
        temp_a0_3 = temp_s0->m_pNeighbours[2];
        if ((temp_a0_3 != NULL) && (temp_a0_3->m_wFlags & 0x8000)) {
            Automap_ClearVisited((sptr) temp_a0_3);
        }
    }
}

#include "m2c_macros.h"

s32 Cinema_PathPlaying();                                /* extern */

s32 Automap_IsShown(void *arg0)
{
  s32 var_s0;
  s32 var_v0;
  var_s0 = 0;
  if ((*((s32 *) (((s8 *) arg0) + 4))) != 0)
  {
    var_v0 = 0;
    if (gPersistentDataLoaded != 0)
    {
      var_v0 = 0;
      if (Cinema_PathPlaying() == 0)
      {
        var_v0 = gEngineApp_m_Settings.nextMode ^ 1;
        {
          var_s0 = ((u32) var_v0) > 0U;
          goto block_4;
        }
      }
    }
  }
  else
  {
    block_4:
    ;

  }
  return var_s0;
}

typedef struct {
    /* 0x0 */ u8 pad0[4];
    /* 0x4 */ s32 state;
} Unk00401C78;

s32 Automap_IsOpen(Unk00401C78 *arg0) {
    return (u32) (arg0->state - 1) < 2U;
}

void Automap_Open(Unk00401C78 *arg0) {
    if ((u32) (arg0->state - 1) >= 2U) {
        arg0->state = 1;
    }
}

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 state;
} Phase;

void Automap_Close(Phase *arg0) {
    s32 temp_v0;

    temp_v0 = arg0->state;
    if ((temp_v0 != 0) && (temp_v0 != 3)) {
        arg0->state = 3;
    }
}

typedef struct {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ u8 padC[0x20 - 0xC];
    /* 0x20 */ s32 unk20[6];
} Module;

void Automap_Reset(Module *arg0) {
    arg0->unk8 = 0;
    arg0->unk4 = 0;
    arg0->unk20[0] = 0;
    arg0->unk20[1] = 0;
    arg0->unk20[2] = 0;
    arg0->unk20[3] = 0;
    arg0->unk20[4] = 0;
    arg0->unk20[5] = 0;
}

void Automap_ClearVisited(CGameRegion *arg0) {
    CGameRegion *temp_a0;
    CGameRegion *temp_a0_2;
    CGameRegion *temp_a0_3;

    if (arg0 != NULL) {
        temp_a0 = arg0->m_pNeighbours[0];
        arg0->m_wFlags &= 0x7FFF;
        if ((temp_a0 != NULL) && (temp_a0->m_wFlags & 0x8000)) {
            Automap_ClearVisited(temp_a0);
        }
        temp_a0_2 = arg0->m_pNeighbours[1];
        if ((temp_a0_2 != NULL) && (temp_a0_2->m_wFlags & 0x8000)) {
            Automap_ClearVisited(temp_a0_2);
        }
        temp_a0_3 = arg0->m_pNeighbours[2];
        if ((temp_a0_3 != NULL) && (temp_a0_3->m_wFlags & 0x8000)) {
            Automap_ClearVisited(temp_a0_3);
        }
    }
}

/* Returns 0 when both points are strictly outside the same edge of the quad (x/z plane). */
s32 Automap_EdgeVisible(f32 (*quad)[3], f32 *a, f32 *b) {
    s32 i;
    f32 *p;
    f32 *q;
    f32 dz;
    f32 dx;

    for (i = 0; i < 4; i++) {
        p = quad[i];
        q = (i == 3) ? quad[0] : quad[i + 1];
        dz = q[1] - p[1];
        dx = p[0] - q[0];
        if (dz * (a[0] - p[0]) + dx * (a[2] - p[1]) > 0.0f) {
            if (dz * (b[0] - p[0]) + dx * (b[2] - p[1]) > 0.0f) {
                return 0;
            }
        }
    }
    return 1;
}
