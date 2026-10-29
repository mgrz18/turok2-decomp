/* NON-MATCHING: 206 of 385 words differ: same size and structure; the region counter is spilled and the object-table base kept in fp (ROM: counter in fp, base spilled), so the region pointer increment lands in bc1fl delay slots and registers shift. */
/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/game/func_00289054.c, func_00289054, with Turok 2's layouts. */
#include "common.h"
#include "gfx.h"

/* Draws the level geometry seen by one view: every static object and level
 * actor of the grid regions inside the view's box that is shown in this view
 * and inside its frustum is drawn, or queued on the world's sorted list
 * (translucent ones, drawn later by func_0022077C). Objects flagged 8 also
 * go on a second list, except the type found at 0x33 in the world's id table
 * unless the view's player passes the func_00253720(0x111F) test. With the
 * shared flag set, the cached list of func_00221D3C is used instead. */

typedef struct View View;
struct View {
    u8 pad0[0x64];
    s32 mask;               /* 0x064 */
    u8 pad68[0x270];
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
    s32 stride;             /* 0x00 */
    s32 count;              /* 0x04 */
    u16 ids[1];             /* 0x08 */
} IdTable;

typedef struct {
    u8 pad0[2];
    u16 id;                 /* 0x02 */
    u8 pad4[0x10];
    Info *info;             /* 0x14 */
    u8 pad18[0xC];
    void **owner;           /* 0x24 */
    u8 pad28[0x40];
    f32 boxMin[3];          /* 0x68 */
    f32 boxMax[3];          /* 0x74 */
    u8 pad80[4];
    s32 viewMask;           /* 0x84 */
    u16 flags;              /* 0x88: 4 translucent, 8 listed */
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
    u8 pad0[0x74];
    IdTable *ids;           /* 0x0074 */
    u8 pad78[0xAF4 - 0x78];
    void *listed[0x40];     /* 0x0AF4 */
    s32 listedCount;        /* 0x0BF4 */
    u8 padBF8[0x1064 - 0xBF8];
    s32 sortedCount;        /* 0x1064 */
    void *sorted[0x40];     /* 0x1068 */
    u8 pad1168[0x14A8 - 0x1168];
    s32 regionCount;        /* 0x14A8 */
    Region regions[128];    /* 0x14AC */
} World;

typedef struct {
    u8 pad0[0xA1C];
    s32 flags;              /* 0xA1C */
} Player;

typedef struct {
    s32 mask;               /* 0x00 */
    u8 pad4[0xD];
    u8 unk11;               /* 0x11 */
} ViewState;

extern ViewState D_8011AAE0;
extern u8 D_8011AAF1;       /* D_8011AAE0.unk11 */
extern s32 D_800B6D20;

void func_00221D3C(World *);
Player *func_00281194(View *);
s32 func_00253720(Player *, s32);
void func_0020B5D4(void);
void func_0020B624(void);
void func_0020565C(s32, void **);
void *func_002017D4(void *, s32);
s32 func_0026F60C(void *, f32 *);
void func_00214D04(Object *, View *);
void func_00214844(Object *, View *);
void func_002152AC(Actor *);

void func_002222E0(World *w, View *v_) {
    register View *v asm("$18") = v_;
    u8 *base;
    void *grp;
    s32 i;
    s32 n;
    s32 mask;
    s32 vmask;
    s32 idx;
    s32 flag;
    Player *player;
    IdTable *tbl;
    s32 cnt;
    s32 si;
    u16 *ip;
    Table *t;
    Region *regions;
    Region *region;
    Cell *cell;
    Object *a;
    Actor *p;
    Actor *actors;
    s32 k;
    s32 cnt1;
    s32 cnt2;
    s32 off;
    s32 c;

    mask = D_8011AAE0.mask;
    vmask = v->mask;
    if (D_8011AAE0.unk11 != 0) {
        func_00221D3C(w);
        return;
    }
    player = func_00281194(v);
    si = 0;
    tbl = w->ids;
    cnt = tbl->count;
    ip = tbl->ids;
    if (cnt > 0) {
        do {
            if (ip[si] == 0x33) {
                idx = si;
                goto found;
            }
            si++;
        } while (si < cnt);
    }
    idx = -1;
found:
    if (player != NULL && idx != -1 && func_00253720(player, 0x111F) != 0 && D_8011AAF1 == 0) {
        flag = 1;
    } else {
        flag = 0;
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
    if (player != NULL && (player->flags & 0x100)) {
        {
            Gfx *g = D_800D8E24++;
            g->w0 = 0xDB040004;
            g->w1 = 1;
        }
        {
            Gfx *g = D_800D8E24++;
            g->w0 = 0xDB04000C;
            g->w1 = 1;
        }
        {
            Gfx *g = D_800D8E24++;
            g->w0 = 0xDB040014;
            g->w1 = 0xFFFF;
        }
        {
            Gfx *g = D_800D8E24++;
            g->w0 = 0xDB04001C;
            g->w1 = 0xFFFF;
        }
    } else {
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
    }
    i = 0;
    n = w->regionCount;
    regions = w->regions;
    if (n > 0) {
        do {
            region = regions + i;
            cell = region->cell;
            if (cell->maxX > v->boxMin[0] && cell->minX < v->boxMax[0]
                && cell->maxZ > v->boxMin[2] && cell->minZ < v->boxMax[2]) {
                func_0020565C(0, region->data);
                k = 0;
                grp = *region->data;
                t = func_002017D4(grp, k);
                cnt1 = t->count;
                base = t->entries;
                if (cnt1 > 0) {
                    off = k;
                    do {
                        a = (Object *)(base + off);
                        if (a->info->flags & mask) {
                            a->owner = region->data;
                            if (D_800B6D20 == 0) {
                                func_00214D04(a, v);
                            }
                            if ((vmask & a->viewMask)
                                && v->boxMax[0] > a->boxMin[0] && v->boxMin[0] < a->boxMax[0]
                                && v->boxMax[2] > a->boxMin[2] && v->boxMin[2] < a->boxMax[2]
                                && v->boxMax[1] > a->boxMin[1] && v->boxMin[1] < a->boxMax[1]
                                && func_0026F60C(v->frustum, a->boxMin) != 0) {
                                if ((a->flags & 8) && (a->id != idx || flag != 0)) {
                                    c = w->listedCount;
                                    if (c != 0x40) {
                                        w->listed[c] = a;
                                        w->listedCount = c + 1;
                                    }
                                }
                                if (a->flags & 4) {
                                    c = w->sortedCount;
                                    if (c != 0x40) {
                                        w->sorted[c] = a;
                                        w->sortedCount = c + 1;
                                    }
                                } else {
                                    func_00214844(a, v);
                                }
                            }
                        }
                        k++;
                        off += sizeof(Object);
                    } while (k < cnt1);
                }
                t = func_002017D4(grp, 1);
                k = 0;
                cnt2 = t->count;
                actors = (Actor *)t->entries;
                if (cnt2 > 0) {
                    do {
                        p = actors + k;
                        if (p->info->flags & mask) {
                            u16 fl = p->flags;

                            if (!(fl & 1) && (vmask & p->viewMask)
                                && v->boxMax[0] > p->boxMin[0] && v->boxMin[0] < p->boxMax[0]
                                && v->boxMax[2] > p->boxMin[2] && v->boxMin[2] < p->boxMax[2]
                                && v->boxMax[1] > p->boxMin[1] && v->boxMin[1] < p->boxMax[1]
                                && (fl & 2) && func_0026F60C(v->frustum, p->boxMin) != 0) {
                                if (p->flags & 0x20) {
                                    c = w->sortedCount;
                                    if (c != 0x40) {
                                        w->sorted[c] = p;
                                        w->sortedCount = c + 1;
                                    }
                                } else {
                                    func_002152AC(p);
                                }
                            }
                        }
                        k++;
                    } while (k < cnt2);
                }
            }
            i++;
        } while (i < n);
    }
    func_0020B624();
}
