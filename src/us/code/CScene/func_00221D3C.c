/* do { } while (0) blocks around the region setup and the second table lookup raise those pseudos' priority (GCC weights refs by loop depth) without a scheduling barrier at the call's argument moves; RW-shaped object loop (off = k), do/while loops with explicit k = 0 before the count load. */
/* Loop shapes after thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/game/func_00289054.c, func_00289054, with Turok 2's layouts. */
#include "common.h"
#include "structs/table.h"
#include "gfx.h"
#include "gbi_t2.h"
#include "structs/world.h"
#include "structs/level_actor.h"
#include "structs/camera.h"
#include "structs/rect.h"
#include "globals/camera.h"
#include "globals/scene.h"
#include "protos/iset.h"
#include "protos/cache.h"
#include "structs/vector.h"
/* protos/scene.h's prototypes of what this file calls (the module defines them all; the
 * merged file declares them from the definitions). */
struct CCamera;
struct CGameObjectInstance;
struct CScene;
struct CSceneRecord;
struct LevelEvent;
void CScene__BuildGeometryDisplayList(struct CScene *);
void CScene__DrawSortedList(void *, void *);

/* Builds (once per frame) the display list of the level geometry: for every
 * grid region seen by one of the views, every static object and every level
 * actor shown in this view and inside a view's frustum is drawn, or, when
 * translucent, queued on the world's sorted list (0x1064, drawn later by
 * CScene__DrawSortedList). The list is built in place behind a branch command that
 * jumps over it, and later views just call it (0xDE000000). */

/* gEngineApp_m_Settings (structs/game.h) seen from its difficulty, gSettingsDifficultyView_00221D3C: the
 * ROM bases the CGame and camera-pool reads below on this symbol, and with
 * gEngineApp_m_Settings / gEngineApp_m_Game the code no longer matches. */
typedef struct {
    s32 mask;               /* 0x00 */
} ViewState_00221D3C;

extern ViewState_00221D3C gSettingsDifficultyView_00221D3C __asm__("gEngineApp_m_Settings_difficulty"); /* local type: the field's real type (or the member path) changes the ROM's code here */

void Render_ResetBatches(void);
void Render_FlushBatches(void);
s32 Frustum_TestBox(void *frustum, void *box);
void CStaticObject__Draw(void *object, void *camera);
void CLevelActor__Draw(void *actor);

void CScene__BuildGeometryDisplayList(CScene *w) {
    s32 r;
    s32 n;
    u8 *objs;
    CCamera *views;
    s32 mask;
    s32 vmask;
    CCamera *v;
    CSceneSector *regions;
    CSceneSector *region;
    CRect *cell;
    void *grp;
    StrideTable *t;
    CStaticObject *o;
    CLevelActor *actors;
    CLevelActor *a;
    s32 k;
    s32 off;
    s32 cnt;
    s32 cnt2;
    s32 found;
    s32 c;

    mask = gSettingsDifficultyView_00221D3C.mask;
    vmask = gCameraVisBits;
    if (gSceneGeometryDL == NULL) {
#ifdef TARGET_PORT
        /* the N64 reads gEngineApp_m_Game_m_CameraPool_m_ActiveList_m_pHead 0x1210 bytes before
         * the difficulty; on the host the two are 0x1358 apart (round 18 g2, n64viewcheck gptr) */
        views = (CCamera *) gEngineApp_m_Game_m_CameraPool_m_ActiveList_m_pHead;
#else
        views = *(CCamera **)((u8 *)&gSettingsDifficultyView_00221D3C - 0x1210);
#endif
        if (views == NULL) {
            return;
        }
        gSceneGeometryDL = gDList;
        {
            Gfx *g = gDList++;
            g->w0 = 0xDE010000;
            g->w1 = 0;
        }
        {
            Gfx *g = gDList++;
            g->w0 = 0xE7000000;
            g->w1 = 0;
        }
        {
            Gfx *g = gDList++;
            g->w0 = 0xE3000A01;
            g->w1 = 0x100000;
        }
        Render_ResetBatches();
        w->m_nDeferred = 0;
        {
            Gfx *g = gDList++;
            g->w0 = 0xDB040004;
            g->w1 = 2;
        }
        {
            Gfx *g = gDList++;
            g->w0 = 0xDB04000C;
            g->w1 = 2;
        }
        {
            Gfx *g = gDList++;
            g->w0 = 0xDB040014;
            g->w1 = 0xFFFE;
        }
        {
            Gfx *g = gDList++;
            g->w0 = 0xDB04001C;
            g->w1 = 0xFFFE;
        }
        r = 0;
        n = w->m_nSectors;
        regions = w->m_Sectors;
        if (n > 0) {
            do {
                region = regions + r;
                v = views;
                found = 0;
                while (v != NULL) {
                    cell = region->m_pBounds;
                    if (cell->maxX > v->m_ViewMin.x && cell->minX < v->m_ViewMax.x
                        && cell->maxZ > v->m_ViewMin.z && cell->minZ < v->m_ViewMax.z) {
                        found = 1;
                        break;
                    }
                    v = v->m_pNext;
                }
                if (found) {
                    CCache__LockForDisplayList(0, region->m_hData);
                    k = 0;
                    grp = *region->m_hData;
                    t = CISet__GetBlock(grp, k);
                    cnt = t->count;
                    objs = t->entries;
                    if (cnt > 0) {
                        off = k;
                        do {
                            o = (CStaticObject *)(objs + off);
                            if (o->header.m_pIntelligence->m_dwFlags & mask) {
                                if (vmask & o->m_ViewMask) {
                                    v = views;
                                    found = 0;
                                    while (v != NULL) {
                                        if (v->m_ViewMax.x > o->m_Bounds.m_vMin.x && v->m_ViewMin.x < o->m_Bounds.m_vMax.x
                                            && v->m_ViewMax.z > o->m_Bounds.m_vMin.z && v->m_ViewMin.z < o->m_Bounds.m_vMax.z
                                            && v->m_ViewMax.y > o->m_Bounds.m_vMin.y && v->m_ViewMin.y < o->m_Bounds.m_vMax.y
                                            && Frustum_TestBox(v->m_ViewVolume, &o->m_Bounds)) {
                                            found = 1;
                                            break;
                                        }
                                        v = v->m_pNext;
                                    }
                                    if (found) {
                                        o->m_pOwner = region->m_hData;
                                        if (o->m_wFlags & 4) {
                                            c = w->m_nDeferred;
                                            if (c != 0x40) {
                                                w->m_pDeferred[c] = o;
                                                w->m_nDeferred = c + 1;
                                            }
                                        } else {
                                            CStaticObject__Draw(o, views);
                                        }
                                    }
                                }
                            }
                            k++;
                            off += sizeof(CStaticObject);
                        } while (k < cnt);
                    }
                    t = CISet__GetBlock(grp, 1);
                    k = 0;
                    cnt2 = t->count;
                    actors = (CLevelActor *)t->entries;
                    if (cnt2 > 0) {
                        do {
                            a = &actors[k];
                            if (!(a->header.m_pIntelligence->m_dwFlags & mask)) {
                                goto next;
                            }
                            if (a->m_wFlags & 1) {
                                goto next;
                            }
                            if (!(a->m_wFlags & 2)) {
                                goto next;
                            }
                            if (vmask & a->m_ViewMask) {
                                v = views;
                                found = 0;
                                while (v != NULL) {
                                    if (v->m_ViewMax.x > a->m_Bounds.m_vMin.x && v->m_ViewMin.x < a->m_Bounds.m_vMax.x
                                        && v->m_ViewMax.z > a->m_Bounds.m_vMin.z && v->m_ViewMin.z < a->m_Bounds.m_vMax.z
                                        && v->m_ViewMax.y > a->m_Bounds.m_vMin.y && v->m_ViewMin.y < a->m_Bounds.m_vMax.y
                                        && Frustum_TestBox(v->m_ViewVolume, &a->m_Bounds)) {
                                        found = 1;
                                        break;
                                    }
                                    v = v->m_pNext;
                                }
                                if (found) {
                                    if (a->m_wFlags & 0x20) {
                                        c = w->m_nDeferred;
                                        if (c != 0x40) {
                                            w->m_pDeferred[c] = a;
                                            w->m_nDeferred = c + 1;
                                        }
                                    } else {
                                        CLevelActor__Draw(a);
                                    }
                                }
                            }
                        next:
                            k++;
                        } while (k < cnt2);
                    }
                }
                r++;
            } while (r < n);
        }
        Render_FlushBatches();
        {
            Gfx *g = gDList++;
            g->w0 = 0xDF000000;
            g->w1 = 0;
        }
        {
            Gfx *g = gSceneGeometryDL++;
            g->w0 = 0xDE010000;
            g->w1 = GBI_ADDR(gDList);
        }
    }
    {
        Gfx *g = gDList++;
        g->w0 = 0xDE000000;
        g->w1 = GBI_ADDR(gSceneGeometryDL);
    }
}
