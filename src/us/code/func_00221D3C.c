/* do { } while (0) blocks around the region setup and the second table lookup raise those pseudos' priority (GCC weights refs by loop depth) without a scheduling barrier at the call's argument moves; RW-shaped object loop (off = k), do/while loops with explicit k = 0 before the count load. */
/* Loop shapes after thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/game/func_00289054.c, func_00289054, with Turok 2's layouts. */
#include "common.h"
#include "structs/table.h"
#include "gfx.h"
#include "structs/world.h"
#include "structs/level_actor.h"
#include "structs/camera.h"
#include "structs/rect.h"

/* Builds (once per frame) the display list of the level geometry: for every
 * grid region seen by one of the views, every static object and every level
 * actor shown in this view and inside a view's frustum is drawn, or, when
 * translucent, queued on the world's sorted list (0x1064, drawn later by
 * func_0022077C). The list is built in place behind a branch command that
 * jumps over it, and later views just call it (0xDE000000). */

/* D_8011AAD8 (structs/game.h) seen from its difficulty, D_8011AAE0: the
 * ROM bases the CGame and camera-pool reads below on this symbol, and with
 * D_8011AAD8 / D_80119870 the code no longer matches. */
typedef struct {
    s32 mask;               /* 0x00 */
} ViewState;

extern ViewState D_8011AAE0;
extern s32 D_800F5A40;
extern Gfx *D_800ACDE0;     /* the level's display list, once built this frame */

void func_0020B5D4(void);
void func_0020B624(void);
void func_0020565C(s32, void **);
void *func_002017D4(void *, s32);
s32 func_0026F60C(void *, CBox *);
void func_00214844(CStaticObject *, CCamera *);
void func_002152AC(CLevelActor *);

void func_00221D3C(CWorld *w) {
    s32 r;
    s32 n;
    u8 *objs;
    CCamera *views;
    s32 mask;
    s32 vmask;
    CCamera *v;
    CWorldSector *regions;
    CWorldSector *region;
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

    mask = D_8011AAE0.mask;
    vmask = D_800F5A40;
    if (D_800ACDE0 == NULL) {
        views = *(CCamera **)((u8 *)&D_8011AAE0 - 0x1210);
        if (views == NULL) {
            return;
        }
        D_800ACDE0 = D_800D8E24;
        {
            Gfx *g = D_800D8E24++;
            g->w0 = 0xDE010000;
            g->w1 = 0;
        }
        {
            Gfx *g = D_800D8E24++;
            g->w0 = 0xE7000000;
            g->w1 = 0;
        }
        {
            Gfx *g = D_800D8E24++;
            g->w0 = 0xE3000A01;
            g->w1 = 0x100000;
        }
        func_0020B5D4();
        w->m_nDeferred = 0;
        {
            Gfx *g = D_800D8E24++;
            g->w0 = 0xDB040004;
            g->w1 = 2;
        }
        {
            Gfx *g = D_800D8E24++;
            g->w0 = 0xDB04000C;
            g->w1 = 2;
        }
        {
            Gfx *g = D_800D8E24++;
            g->w0 = 0xDB040014;
            g->w1 = 0xFFFE;
        }
        {
            Gfx *g = D_800D8E24++;
            g->w0 = 0xDB04001C;
            g->w1 = 0xFFFE;
        }
        r = 0;
        do {
            n = w->m_nSectors;
            regions = w->m_Sectors;
        } while (0);
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
                    func_0020565C(0, region->m_hData);
                    k = 0;
                    grp = *region->m_hData;
                    t = func_002017D4(grp, k);
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
                                            && func_0026F60C(v->m_ViewVolume, &o->m_Bounds)) {
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
                                            func_00214844(o, views);
                                        }
                                    }
                                }
                            }
                            k++;
                            off += sizeof(CStaticObject);
                        } while (k < cnt);
                    }
                    do {
                        t = func_002017D4(grp, 1);
                    } while (0);
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
                                        && func_0026F60C(v->m_ViewVolume, &a->m_Bounds)) {
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
                                        func_002152AC(a);
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
        func_0020B624();
        {
            Gfx *g = D_800D8E24++;
            g->w0 = 0xDF000000;
            g->w1 = 0;
        }
        {
            Gfx *g = D_800ACDE0++;
            g->w0 = 0xDE010000;
            g->w1 = (u32)D_800D8E24;
        }
    }
    {
        Gfx *g = D_800D8E24++;
        g->w0 = 0xDE000000;
        g->w1 = (u32)D_800ACDE0;
    }
}
