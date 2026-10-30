/* NON-MATCHING: 182 of 362 words differ: r = 0 placed later and a do { } while (0) around the region setup (GCC weights refs by loop depth) fix the start and part of the allocation; still off: the region pointer and the object count swap s5/s6, the object loop takes its 0 for k/off from $a1/$a0 instead of k (one extra word), and the sorted-list branch gets a beql where the ROM has beq + j. */
#include "common.h"
#include "gfx.h"

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
    s32 stride;             /* 0x00 */
    s32 count;              /* 0x04 */
    u8 entries[1];          /* 0x08 */
} Table;

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

typedef struct {
    void **data;            /* 0x0 */
    Cell *cell;             /* 0x4 */
} Region;

typedef struct {
    u8 pad0[0x1064];
    s32 sortedCount;        /* 0x1064 */
    void *sorted[0x40];     /* 0x1068 */
    u8 pad1168[0x14A8 - 0x1168];
    s32 regionCount;        /* 0x14A8 */
    Region regions[128];    /* 0x14AC */
} World;

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

void func_00221D3C(World *w) {
    s32 r;
    s32 n;
    u8 *objs;
    View *views;
    s32 mask;
    s32 vmask;
    View *v;
    Region *regions;
    Region *region;
    Cell *cell;
    void *grp;
    Table *t;
    Object *o;
    Actor *actors;
    Actor *a;
    s32 k;
    s32 off;
    s32 cnt;
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
        w->sortedCount = 0;
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
        n = w->regionCount;
        regions = w->regions;
        if (n > 0) {
            do {
            region = regions + r;
            v = views;
            found = 0;
            while (v != NULL) {
                cell = region->cell;
                if (cell->maxX > v->boxMin[0] && cell->minX < v->boxMax[0]
                    && cell->maxZ > v->boxMin[2] && cell->minZ < v->boxMax[2]) {
                    found = 1;
                    break;
                }
                v = v->next;
            }
            if (found) {
            do {
            func_0020565C(0, region->data);
            k = 0;
            grp = *region->data;
            } while (0);
            t = func_002017D4(grp, k);
            cnt = t->count;
            objs = t->entries;
            for (k = 0, off = 0; k < cnt; k++, off += sizeof(Object)) {
                o = (Object *)(objs + off);
                if (!(o->info->flags & mask)) {
                    continue;
                }
                if (!(vmask & o->viewMask)) {
                    continue;
                }
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
                if (!found) {
                    continue;
                }
                o->owner = region->data;
                if (o->flags & 4) {
                    c = w->sortedCount;
                    if (c != 0x40) {
                        w->sorted[c] = o;
                        w->sortedCount = c + 1;
                    }
                } else {
                    func_00214844(o, views);
                }
            }
            t = func_002017D4(grp, 1);
            k = 0;
            cnt = t->count;
            actors = (Actor *)t->entries;
            for (; k < cnt; k++) {
                a = &actors[k];
                if (!(a->info->flags & mask)) {
                    continue;
                }
                if (a->flags & 1) {
                    continue;
                }
                if (!(a->flags & 2)) {
                    continue;
                }
                if (!(vmask & a->viewMask)) {
                    continue;
                }
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
                if (!found) {
                    continue;
                }
                if (a->flags & 0x20) {
                    c = w->sortedCount;
                    if (c != 0x40) {
                        w->sorted[c] = a;
                        w->sortedCount = c + 1;
                    }
                } else {
                    func_002152AC(a);
                }
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
