#include "common.h"

typedef struct {
    s32 animate;  /* 0x0 */
    s32 hasEvent; /* 0x4 */
    u8 pad8[0x4];
    s32 *anim;    /* 0xC */
    u8 pad10[0x8];
    s32 first;    /* 0x18 */
    s32 current;  /* 0x1C */
} AnimRequest;

typedef struct {
    u8 data[0x18];
} AnimSlot;

typedef struct {
    u8 data[0x64];
} Slot00407DC0;

typedef struct {
    /* 0x000 */ u8 pad0[0x14C];
    /* 0x14C */ s32 slotMask;
    /* 0x150 */ u8 pad150[0x174 - 0x150];
    /* 0x174 */ s8 unk174;
} Boss00407DC0;

extern Boss00407DC0 *D_8012F970;
extern Slot00407DC0 D_8012F62C[];
extern s32 D_800B6D1C;
extern s32 D_800AB8B0;
s32 Cinema_PathPlaying(void);
void func_0021DC84(void *actor, void *state, AnimRequest *req);
void func_00236324(Slot00407DC0 *slot);
void func_00235898(void);
void func_0020A33C(s32 *anim, s32 model, s32 one, AnimSlot *slot, s32 zero, s32 index, s32 second);

static __inline__ s32 hasBit(s32 mask, s32 bit) {
    return mask & bit;
}

/* Draws each of the boss's 8 parts present in its mask; part 1 shows mesh 8 in state 29. */
void func_00407DC0(u8 *actor, u8 *state, AnimRequest *req) {
    Boss00407DC0 *boss = D_8012F970;
    s32 mask;
    s32 i;

    mask = boss->slotMask;
    for (i = 0; i < 8; i++) {
        if (hasBit(mask, 1 << i)) {
            if (i == 1 && boss->unk174 == 29) {
                req->first = 8;
                req->current = 8;
            } else {
                req->first = i;
                req->current = i;
            }
            func_0021DC84(actor, state, req);
            if (!Cinema_PathPlaying()) {
                func_00236324(&D_8012F62C[i]);
                func_00235898();
            }
            func_0020A33C(req->anim, *(s32 *) (actor + 0x98), 1, &((AnimSlot *) (actor + 0x110))[D_800B6D1C], 0,
                          req->first, *(s8 *) (actor + 0x151));
            D_800AB8B0 = 0;
        }
    }
}
