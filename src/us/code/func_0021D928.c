/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/particle/80246E34_6.CPP, func_8024A7A0_de, with Turok 2's layouts. */
#include "common.h"

/* As func_0021D7BC, drawing through func_0020A33C and without setting the
 * actor's mask; clears D_800AB8B0 afterwards. */

typedef struct {
    s32 type;               /* 0x00 */
    u8 pad4[0xA];
    s8 set0[2];             /* 0x0E (+ params->index) */
    s8 set1[2];             /* 0x10 (+ params->index) */
    u8 pad12[0x51];
    s8 stateSet[6];         /* 0x63: by AI state 0x36..0x3B */
} Region;

typedef struct {
    s32 draw;               /* 0x00 */
    s32 shadow;             /* 0x04 */
    u8 pad8[0x4];
    s32 model;              /* 0x0C */
    u8 pad10[0x4];
    s32 index;              /* 0x14 */
    s32 set0;               /* 0x18 */
    s32 set1;               /* 0x1C */
} DrawParams;

typedef struct {
    u8 pad0[0x14];
    Region *region;         /* 0x014 */
    u8 pad18[0x80];
    s32 unk98;              /* 0x098 */
    u8 pad9C[0x74];
    u8 lights[2][0x18];     /* 0x110 */
    u8 pad140[0xC];
    s32 mask;               /* 0x14C (AI +0x0C) */
    u8 pad150;
    s8 unk151;              /* 0x151 (AI +0x11) */
    u8 pad152[0x22];
    s8 state;               /* 0x174 (AI +0x34) */
    u8 pad175;
    s8 nextState;           /* 0x176 (AI +0x36) */
} Actor;

extern s32 D_800B6D1C;      /* current view */
extern s32 D_800AB8B0;

void func_0021DC84(Actor *, s32, DrawParams *);
void func_0020A33C(s32, s32, s32, void *, s32, s32, s32);

static __inline__ void pickSets(Actor *actor, s32 index, s32 *set0, s32 *set1) {
    Region *region = actor->region;

    if (region->type == 1) {
        s32 state;

        if (actor->state == 0x3C) {
            state = actor->nextState;
        } else {
            state = actor->state;
        }
        switch (state) {
            case 0x36:
                *set0 = *set1 = region->stateSet[0];
                break;
            case 0x37:
                *set0 = *set1 = region->stateSet[1];
                break;
            case 0x38:
                *set0 = *set1 = region->stateSet[2];
                break;
            case 0x39:
                *set0 = *set1 = region->stateSet[3];
                break;
            case 0x3a:
                *set0 = *set1 = region->stateSet[4];
                break;
            case 0x3b:
                *set0 = *set1 = region->stateSet[5];
                break;
            default:
                *set0 = (&region->set0[0])[index];
                *set1 = (&region->set1[0])[index];
                break;
        }
    } else {
        *set0 = (&region->set0[0])[index];
        *set1 = (&region->set1[0])[index];
    }
}

void func_0021D928(Actor *actor, s32 arg1, DrawParams *params) {
    pickSets(actor, params->index, &params->set0, &params->set1);
    if (params->shadow != 0) {
        func_0021DC84(actor, arg1, params);
    }
    if (params->draw != 0) {
        func_0020A33C(params->model, actor->unk98, 1, actor->lights[D_800B6D1C], 0, params->set0, actor->unk151);
    }
    D_800AB8B0 = 0;
}
