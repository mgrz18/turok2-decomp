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
    u8 data[0x68];
} Slot00403660;

typedef struct {
    /* 0x000 */ u8 pad0[0x54];
    /* 0x054 */ s32 unk54;
    /* 0x058 */ u8 pad1[0x14C - 0x58];
    /* 0x14C */ s32 slotMask;
    /* 0x150 */ u8 pad2[0x174 - 0x150];
    /* 0x174 */ s8 unk174;
} Boss00403660;

typedef struct {
    /* 0x000 */ Boss00403660 *boss;
    /* 0x004 */ u8 pad0[0xC - 0x4];
    /* 0x00C */ Slot00403660 slots[5];
} State00403660;

extern State00403660 D_8012F3A0;
extern s32 D_800B6D1C;
extern s32 D_800AB8B0;
void func_0021DC84(void *actor, void *state, AnimRequest *req);
void func_00236324(Slot00403660 *slot);
void func_00235898(void);
void func_0020A33C(s32 *anim, s32 model, s32 one, AnimSlot *slot, s32 zero, s32 index, s32 second);

static __inline__ s32 hasBit(s32 mask, s32 bit) {
    return mask & bit;
}

void func_00403660(u8 *actor, u8 *state, AnimRequest *req) {
    State00403660 *s;
    Boss00403660 *boss;
    s32 mask;
    Slot00403660 *slot;
    s32 i;
    s32 bit;

    i = 0;
    bit = 1;
    s = &D_8012F3A0;
    boss = s->boss;
    mask = boss->slotMask;
    slot = s->slots;
    do {
        if (hasBit(mask, bit << i)) {
            req->first = i;
            req->current = i;
            func_0021DC84(actor, state, req);
            func_00236324(slot);
            func_00235898();
            func_0020A33C(req->anim, *(s32 *) (actor + 0x98), 1, &((AnimSlot *) (actor + 0x110))[D_800B6D1C], 0, i,
                          *(s8 *) (actor + 0x151));
            D_800AB8B0 = 0;
        }
        i++;
        slot++;
    } while (i < 5);
    if (boss->unk174 != 2) {
        boss->unk54 = 0;
    }
}
