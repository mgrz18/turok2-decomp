#include "common.h"

/* AI init dispatcher: the 0x44E..0x456 types have their own init in the paged
 * VM code; everything else gets the generic init. */

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ u32 flags;
} ActorDef;

typedef struct {
    /* 0x000 */ u8 pad0[0x4];
    /* 0x004 */ f32 pos[3];
    /* 0x010 */ u8 pad10[0x14 - 0x10];
    /* 0x014 */ ActorDef *def;
    /* 0x018 */ u8 pad18[0xB8 - 0x18];
    /* 0x0B8 */ u16 type;
    /* 0x0BA */ u8 padBA[0xD4 - 0xBA];
    /* 0x0D4 */ u32 flags;
} Actor;

typedef struct {
    /* 0x00 */ u8 pad0[0x37];
    /* 0x37 */ s8 unk37;
} CAI;

extern u8 D_800F7078[];

extern void func_00402888(Actor *actor, CAI *ai);
extern void func_00409340(Actor *actor, CAI *ai);
extern void func_00403CF8(Actor *actor, CAI *ai);
extern void func_0040D5C4(Actor *actor, CAI *ai);
extern void func_0040D6A0(Actor *actor, CAI *ai);
extern void func_0040D7A8(Actor *actor, CAI *ai);
extern void func_0040D8B0(Actor *actor, CAI *ai);
extern void func_00243414(Actor *actor, CAI *ai, s32 anim);
extern void func_0025E52C(Actor *actor, CAI *ai, f32 *pos, s32 arg3);
extern s32 func_00220408(void *mgr, Actor *actor);

void func_0025BE88(Actor *actor, CAI *ai) {
    u32 *defFlags = &actor->def->flags;

    switch (actor->type) {
    case 0x451:
        func_00402888(actor, ai);
        break;
    case 0x44E:
        func_00409340(actor, ai);
        break;
    case 0x453:
        func_00403CF8(actor, ai);
        break;
    case 0x450:
        func_0040D5C4(actor, ai);
        break;
    case 0x454:
        func_0040D6A0(actor, ai);
        break;
    case 0x455:
        func_0040D7A8(actor, ai);
        break;
    case 0x456:
        func_0040D8B0(actor, ai);
        break;
    default:
        if (actor->type == 0x467) {
            func_00243414(actor, ai, 0x31);
        } else if (actor->flags & 0x80000) {
            func_00243414(actor, ai, 2);
        } else {
            func_00243414(actor, ai, 0);
        }
        ai->unk37 = 0;
        break;
    }
    if (!(*defFlags & 0x8000)) {
        ai->unk37 = 1;
        func_0025E52C(actor, ai, actor->pos, 3);
    }
    if (func_00220408(D_800F7078, actor) == 1) {
        actor->flags &= ~0x100;
    }
}
