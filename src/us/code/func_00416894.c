/* WRONG CUT: real function 0x00416894, size 0x134 (through 0x004169C8): splat cut its
 * epilogue off as func_004169A0. */
/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/ovl_a/func_00442020.c, func_00442020, with Turok 2's layouts
 * (the template is passed directly instead of read from the caller). */
#include "common.h"

typedef struct {
    /* 0x00 */ s16 type;
    /* 0x02 */ u8 pad2[0x22];
} MenuItemTmpl; /* 0x24 bytes */

typedef struct {
    /* 0x00 */ MenuItemTmpl *items;
    /* 0x04 */ s16 count;
} MenuTmpl;

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ s16 opened;
} MenuPool;

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ s32 unk14;
    /* 0x18 */ u8 pad18[4];
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ s32 unk20;
    /* 0x24 */ s32 unk24;
} MenuCaller;

/* Each item of type 3 gets a 0x330-byte extension after the item array. */
static __inline__ s32 ItemExtSize(s16 *type) {
    if (*type == 3) {
        return 0x330;
    }
    return 0;
}

extern s32 *func_00204EDC(s32, s32, s32, void *);
extern void func_00414EA8(s32 menu, s32 *mem, MenuTmpl *t, s32, s32, s32, s32, MenuPool *pool);
extern void CList__AddHead(MenuPool *a, s32 b);
extern u8 D_800C0C54[]; /* "menu" */

s32 func_00416894(MenuPool *pool, MenuCaller *c, MenuTmpl *t) {
    s32 size, i;
    s32 *r;
    s32 s;
    s32 p1, p2, p3, p4;

    p1 = c->unk14;
    p2 = c->unk1C;
    p3 = c->unk20;
    p4 = c->unk24;
    size = t->count * 40 + 0x1D8;
    for (i = 0; i < t->count; i++) {
        size += ItemExtSize(&t->items[i].type);
    }
    r = func_00204EDC(0, size, 0x3B, D_800C0C54);
    if (r == 0) {
        return 0;
    }
    s = *r;
    if (s == 0) {
        return 0;
    }
    func_00414EA8(s, r, t, p1, p2, p3, p4, pool);
    CList__AddHead(pool, s);
    {
        s16 n = pool->opened + 1;
        pool->opened = n;
        if (n >= 4) {
            pool->opened = 0;
        }
    }
    return s;
}
