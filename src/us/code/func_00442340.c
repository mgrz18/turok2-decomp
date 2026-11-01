#include "common.h"

/* Module init: find the module actors (0x1DB1) and the controller (0x1DB2), hook the level callbacks,
 * then attach every slot to the object. */

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ u8 data[0x68 - 0x4];
} Slot;

typedef struct {
    /* 0x000 */ u8 pad0[0x250];
    /* 0x250 */ s32 unk250;
} LevelActor;

typedef struct {
    /* 0x000 */ void *obj;
    /* 0x004 */ s32 count;
    /* 0x008 */ LevelActor *actors[4];
    /* 0x018 */ Slot slots[15];
    /* 0x630 */ LevelActor *parts[7];
    /* 0x64C */ s32 partCount;
    /* 0x650 */ LevelActor *controller;
    /* 0x654 */ u8 pad654[0x658 - 0x654];
    /* 0x658 */ void *handle;
    /* 0x65C */ u8 pad65C[0x670 - 0x65C];
    /* 0x670 */ s32 partsLeft;
    /* 0x674 */ u8 pad674[0x680 - 0x674];
    /* 0x680 */ s32 unk680;
    /* 0x684 */ s32 unk684;
} State;

typedef struct {
    /* 0x000 */ u8 pad0[0x2C];
    /* 0x02C */ void *data;
    /* 0x030 */ u8 pad30[0xF0 - 0x30];
    /* 0x0F0 */ void *onReset;
    /* 0x0F4 */ void *onEvent;
    /* 0x0F8 */ void *onHit;
    /* 0x0FC */ void *onDamage;
    /* 0x100 */ u8 pad100[0x108 - 0x100];
    /* 0x108 */ void *onDraw;
} Level;

typedef struct {
    /* 0x00 */ u8 pad0[0xD4];
    /* 0xD4 */ s32 flags;
} Obj;

extern State D_8012FB70;
extern s32 D_80110048;
extern s32 D_800F1F10[];
extern u8 D_800F7078[];
extern u8 D_00431E20[];
extern s32 func_00225F6C(void *world, s32 key0, s32 key1, s32 key2, void *out, s32 max);
extern void *func_00225EBC(void *world, s32 a1, s32 id);
extern void func_002362E4(void *slot, void *obj);
extern void func_00243414(void *obj, Level *level, s32 event);
extern void func_0025E4B8();
extern void func_00408FF8();
extern void func_0040BC64();
extern void func_0040BEE4();
extern void func_0040BFF0();

void func_00442340(Obj *obj, Level *level) {
    State *state = &D_8012FB70;
    s32 i;

    D_80110048 = 0x1DB1;
    state->unk680 = 0;
    state->unk684 = 0;
    state->partCount = func_00225F6C(D_800F7078, 0x650, 10, 0x1DB1, state->parts, 6);
    for (i = 0; i < state->partCount; i++) {
        state->parts[i]->unk250 = 1;
    }
    state->partsLeft = state->partCount;
    func_00225F6C(D_800F7078, 0x650, 10, 0x1DB2, &state->controller, 1);
    state->controller->unk250 = 1;
    state->handle = func_00225EBC(D_800F7078, 1, 0x1DB1);
    level->data = D_00431E20;
    level->onReset = func_0040BC64;
    level->onHit = (u8 *)func_00408FF8 + 8;
    level->onDraw = func_0025E4B8;
    level->onDamage = func_0040BFF0;
    level->onEvent = func_0040BEE4;
    func_00243414(obj, level, 0);
    D_8012FB70.obj = obj;
    D_800F1F10[0] &= ~0x300;
    obj->flags |= 0x04000000;
    for (i = 0; i < 15; i++) {
        func_002362E4(D_8012FB70.slots[i].data, obj);
    }
}
