/* do { } while (0) blocks around the region setup and the second table lookup raise those pseudos' priority (GCC weights refs by loop depth) without a scheduling barrier at the call's argument moves; RW-shaped object loop (off = k), do/while loops with explicit k = 0 before the count load. */
/* Loop shapes after thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/game/func_00289054.c, func_00289054, with Turok 2's layouts. */
#include "common.h"
#include "structs/table.h"
#include "gfx.h"
#include "structs/world.h"

/* Builds (once per frame) the display list of the level geometry: for every
 * grid region seen by one of the views, every static object and every level
 * actor shown in this view and inside a view's frustum is drawn, or, when
 * translucent, queued on the world's sorted list (0x1064, drawn later by
 * func_0022077C). The list is built in place behind a branch command that
 * jumps over it, and later views just call it (0xDE000000). */

typedef struct View View;
struct View {
    u8 pad0[4];
    View *next;             /* 0x004 */
    u8 pad8[0x2D0];
    u8 frustum[0x60];       /* 0x2D8 */
    f32 boxMin[3];          /* 0x338 */
    f32 boxMax[3];          /* 0x344 */
};

typedef struct {
    s32 kind;               /* 0x00 */
    s32 flags;              /* 0x04: views it shows in */
} Info;

typedef struct {
    u8 pad0[0x14];
    Info *info;             /* 0x14 */
    u8 pad18[0xC];
    void **owner;           /* 0x24 */
    u8 pad28[0x40];
    f32 boxMin[3];          /* 0x68 */
    f32 boxMax[3];          /* 0x74 */
    u8 pad80[4];
    s32 viewMask;           /* 0x84 */
    u16 flags;              /* 0x88: 4 translucent */
    u8 pad8A[6];
} Object;                   /* 0x90 */

typedef struct {
    u8 pad0[0x14];
    Info *info;             /* 0x014 */
    u8 pad18[0x134];
    f32 boxMin[3];          /* 0x14C */
    f32 boxMax[3];          /* 0x158 */
    u8 pad164[8];
    u16 flags;              /* 0x16C: 1 dead, 2 shown, 0x20 translucent */
    u8 pad16E[0x22];
    s32 viewMask;           /* 0x190 */
    u8 pad194[4];
} Actor;                    /* 0x198 */

typedef struct {
    f32 minX, minZ;         /* 0x0 */
    f32 maxX, maxZ;         /* 0x8 */
} Cell;

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
s32 func_0026F60C(void *, f32 *);
void func_00214844(Object *, View *);
void func_002152AC(Actor *);

void func_00221D3C(CWorld *w) {
    s32 r;
    s32 n;
    u8 *objs;
    View *views;
    s32 mask;
    s32 vmask;
    View *v;
    CWorldSector *regions;
    CWorldSector *region;
    Cell *cell;
    void *grp;
    StrideTable *t;
    Object *o;
    Actor *actors;
    Actor *a;
    s32 k;
    s32 off;
    s32 cnt;
    s32 cnt2;
    s32 found;
    s32 c;

    mask = D_8011AAE0.mask;
    vmask = D_800F5A40;
    if (D_800ACDE0 == NULL) {
        views = *(View **)((u8 *)&D_8011AAE0 - 0x1210);
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
                    if (cell->maxX > v->boxMin[0] && cell->minX < v->boxMax[0]
                        && cell->maxZ > v->boxMin[2] && cell->minZ < v->boxMax[2]) {
                        found = 1;
                        break;
                    }
                    v = v->next;
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
                            o = (Object *)(objs + off);
                            if (o->info->flags & mask) {
                                if (vmask & o->viewMask) {
                                    v = views;
                                    found = 0;
                                    while (v != NULL) {
                                        if (v->boxMax[0] > o->boxMin[0] && v->boxMin[0] < o->boxMax[0]
                                            && v->boxMax[2] > o->boxMin[2] && v->boxMin[2] < o->boxMax[2]
                                            && v->boxMax[1] > o->boxMin[1] && v->boxMin[1] < o->boxMax[1]
                                            && func_0026F60C(v->frustum, o->boxMin)) {
                                            found = 1;
                                            break;
                                        }
                                        v = v->next;
                                    }
                                    if (found) {
                                        o->owner = region->m_hData;
                                        if (o->flags & 4) {
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
                            off += sizeof(Object);
                        } while (k < cnt);
                    }
                    do {
                        t = func_002017D4(grp, 1);
                    } while (0);
                    k = 0;
                    cnt2 = t->count;
                    actors = (Actor *)t->entries;
                    if (cnt2 > 0) {
                        do {
                            a = &actors[k];
                            if (!(a->info->flags & mask)) {
                                goto next;
                            }
                            if (a->flags & 1) {
                                goto next;
                            }
                            if (!(a->flags & 2)) {
                                goto next;
                            }
                            if (vmask & a->viewMask) {
                                v = views;
                                found = 0;
                                while (v != NULL) {
                                    if (v->boxMax[0] > a->boxMin[0] && v->boxMin[0] < a->boxMax[0]
                                        && v->boxMax[2] > a->boxMin[2] && v->boxMin[2] < a->boxMax[2]
                                        && v->boxMax[1] > a->boxMin[1] && v->boxMin[1] < a->boxMax[1]
                                        && func_0026F60C(v->frustum, a->boxMin)) {
                                        found = 1;
                                        break;
                                    }
                                    v = v->next;
                                }
                                if (found) {
                                    if (a->flags & 0x20) {
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
