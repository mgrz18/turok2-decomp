/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/SCHED/8023A284_7.CPP, func_8023B3F8_de, with Turok 2's layouts. */

#include "common.h"
#include "structs/camera.h"

typedef struct {
    u32 w0;
    u32 w1;
} Gfx;

#define GFX_CMD(a, b) { Gfx *_g = D_800D8E24++; _g->w0 = (a); _g->w1 = (b); }

/* A running emitter (see func_00282E4C), as the sort reads it. */
typedef struct Emitter {
    /* 0x000 */ struct Emitter *last;
    /* 0x004 */ struct Emitter *next;
    /* 0x008 */ u8 pad8[0x208];
    /* 0x210 */ f32 depth;
} Emitter;

typedef struct {
    /* 0x000 */ u8 pad0[0x8B4];
    /* 0x8B4 */ CList activeList;
} EmitterOwner;

typedef struct {
    /* 0x000 */ u8 pad0[0x118];
    /* 0x118 */ f32 depth;
} EmitterView;

extern Gfx *D_800D8E24;             /* display list head */
extern u8 D_8011B0C8[];             /* the matrix loaded for them */

void func_00207FEC(s32 mode);
void func_00226F90(void *items, s32 count, s32 size, void *compare, void *swap);
s32 func_00283784(void *a, void *b);
s32 func_002837B0(void *a, void *b);
void func_00283770(void *a, void *b);
void func_00282340(Emitter *e, EmitterView *view);

/* Draws the running emitters: sets up the display list, splits them into
 * those deeper than the view and the others (four of each at most), sorts
 * each group and draws the deeper group first. */
void func_00283238(EmitterOwner *owner, EmitterView *view) {
    Emitter *near[4];
    Emitter *far[4];
    Emitter *e;
    s32 nearCount;
    s32 farCount;
    s32 i;

    if (owner->activeList.m_nCount == 0) {
        return;
    }
    GFX_CMD(0xE7000000, 0)
    GFX_CMD(0xDA380003, (u32)D_8011B0C8)
    GFX_CMD(0xDB040004, 2)
    GFX_CMD(0xDB04000C, 2)
    GFX_CMD(0xDB040014, 0xFFFE)
    GFX_CMD(0xDB04001C, 0xFFFE)
    GFX_CMD(0xD9FEFFFF, 0)
    GFX_CMD(0xE3000A01, 0)
    func_00207FEC(0xD);
    GFX_CMD(0xD9F9FBFF, 0)
    GFX_CMD(0xD9FFFFFF, 0x200004)
    GFX_CMD(0xE3000C00, 0x80000)
    GFX_CMD(0xE3001201, 0x2000)
    nearCount = 0;
    farCount = 0;
    for (e = owner->activeList.m_pHead; e != NULL; e = e->next) {
        if (e->depth > view->depth) {
            near[nearCount++] = e;
        } else {
            far[farCount++] = e;
        }
    }
    func_00226F90(near, nearCount, 4, func_00283784, func_00283770);
    func_00226F90(far, farCount, 4, func_002837B0, func_00283770);
    for (i = 0; i < nearCount; i++) {
        func_00282340(near[i], view);
    }
    for (i = 0; i < farCount; i++) {
        func_00282340(far[i], view);
    }
}
