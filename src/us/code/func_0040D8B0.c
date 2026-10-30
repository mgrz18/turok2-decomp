#include "common.h"

/* AI init for the module's minions (types 0x456 and others): sets the anim table and callbacks, targets the
 * current player and starts anim state 1. */

typedef struct {
    /* 0x000 */ u8 pad0[0xB8];
    /* 0x0B8 */ u16 type;
    /* 0x0BA */ u8 padBA[0xD4 - 0xBA];
    /* 0x0D4 */ u32 flags;
} Actor;

typedef struct {
    /* 0x000 */ u32 flags;
    /* 0x004 */ s32 unk4;
    /* 0x008 */ u8 pad8[0x2C - 0x8];
    /* 0x02C */ void *anims;
    /* 0x030 */ u8 pad30[0x80 - 0x30];
    /* 0x080 */ void *target;
    /* 0x084 */ u8 pad84[0x88 - 0x84];
    /* 0x088 */ void *target2;
    /* 0x08C */ u8 pad8C[0xF0 - 0x8C];
    /* 0x0F0 */ void *callbackF0;
    /* 0x0F4 */ void *callbackF4;
    /* 0x0F8 */ void *callbackF8;
    /* 0x0FC */ u8 padFC[0x108 - 0xFC];
    /* 0x108 */ void *callback108;
    /* 0x10C */ s32 unk10C;
    /* 0x110 */ u8 pad110[0x118 - 0x110];
    /* 0x118 */ s32 unk118;
    /* 0x11C */ s32 unk11C;
} CAI;

extern u8 D_800C1A90[];
extern void *D_80119890; /* current player */
extern void func_0040DF70(void);
extern void func_0040DDB4(void);
extern void func_0025E4B8(void);
extern void func_0040EEA0(void);
extern void func_0040EDB0(void);
extern void func_00243414(Actor *obj, CAI *ai, s32 state);

void func_0040D8B0(Actor *obj, CAI *ai) {
    void *player;

    ai->anims = D_800C1A90;
    ai->callbackF0 = func_0040DF70;
    ai->callbackF8 = func_0040DDB4;
    ai->callback108 = func_0025E4B8;
    if (obj->type == 0x456) {
        ai->callbackF4 = func_0040EEA0;
    } else {
        ai->callbackF4 = func_0040EDB0;
    }
    ai->unk10C = 0;
    ai->unk118 = 0;
    ai->unk11C = 0;
    ai->unk4 = 0;
    obj->flags &= ~0x10000000;
    if (obj->type == 0x456) {
        obj->flags |= 0x20000;
    } else {
        obj->flags &= ~0x20000;
    }
    player = D_80119890;
    if (player != NULL) {
        ai->target = player;
        ai->target2 = player;
        ai->flags |= 0x60000000;
    }
    func_00243414(obj, ai, 1);
    obj->flags &= ~0x100;
}
