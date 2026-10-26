/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/game/func_002A577C.c, func_002A577C, with Turok 2's layouts. */

#include "common.h"
#include "gfx.h"

#define GFX_CMD(a, b) { Gfx *_g = D_800D8E24++; _g->w0 = (a); _g->w1 = (b); }

typedef struct Trail Trail;
struct Trail {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ Trail *next;
    /* 0x08 */ u8 pad8[0x2C];
    /* 0x34 */ s32 layer;
    /* 0x38 */ u8 pad38[4];
    /* 0x3C */ u32 flags;
    /* 0x40 */ u8 pad40[8];
    /* 0x48 */ s32 nodeCount;
};

typedef struct TrailMgr {
    /* 0x0000 */ u8 pad0[0x7520];
    /* 0x7520 */ Trail *trails;
} TrailMgr;

void func_0020B5B8(void);
void func_00207FEC(s32);
void func_00207A70(s32);
void func_00207A54(void);
void func_0023417C(Trail *);
void func_00233FCC(Trail *, s32);
void func_00234348(TrailMgr *, Trail *, s32);
void func_00234928(TrailMgr *, Trail *, s32);

/* Draws the trails: the negative layers first, then the positive ones. */
void func_002352F8(TrailMgr *mgr, s32 arg) {
    Trail *e;

    GFX_CMD(0xE7000000, 0)
    func_0020B5B8();
    func_00207FEC(0xE);
    func_00207A70(0x1C);
    GFX_CMD(0xD9F8FB7F, 0)
    GFX_CMD(0xD9FFFFFF, 0x200004)
    GFX_CMD(0xE3000A01, 0)
    GFX_CMD(0xE3000C00, 0x80000)
    GFX_CMD(0xD7000000, 0)
    func_00207A54();
    for (e = mgr->trails; e != NULL; e = e->next) {
        if (e->layer < 0) {
            if (e->flags & 8) {
                func_0023417C(e);
            }
            if (e->flags & 4) {
                func_00233FCC(e, arg);
            }
            if (e->nodeCount >= 2) {
                if (e->layer >= 0) {
                    func_00234348(mgr, e, arg);
                } else {
                    func_00234928(mgr, e, arg);
                }
            }
        }
    }
    for (e = mgr->trails; e != NULL; e = e->next) {
        if (e->layer > 0) {
            if (e->flags & 8) {
                func_0023417C(e);
            }
            if (e->flags & 4) {
                func_00233FCC(e, arg);
            }
            if (e->nodeCount >= 2) {
                if (e->layer >= 0) {
                    func_00234348(mgr, e, arg);
                } else {
                    func_00234928(mgr, e, arg);
                }
            }
        }
    }
}
