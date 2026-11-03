#include "common.h"

/* Boss fight setup: finds the six level actors with id 0x1DB1 and the one
 * with 0x1DB2, installs the AI callbacks and resets the 15 attack slots. */

typedef struct {
    /* 0x000 */ u8 pad0[0x250];
    /* 0x250 */ s32 active;
} LevelActor;

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ u8 data[0x68 - 0x4];
} Slot;

typedef struct {
    /* 0x000 */ void *obj;
    /* 0x004 */ u8 pad4[0x18 - 0x4];
    /* 0x018 */ Slot slots[15];
    /* 0x630 */ LevelActor *actors[7];
    /* 0x64C */ s32 count;
    /* 0x650 */ LevelActor *core[2];
    /* 0x658 */ s32 coreCount;
    /* 0x65C */ u8 pad65C[0x670 - 0x65C];
    /* 0x670 */ s32 alive;
    /* 0x674 */ u8 pad674[0x680 - 0x674];
    /* 0x680 */ s32 unk680;
    /* 0x684 */ s32 unk684;
} State;

typedef struct {
    /* 0x000 */ u8 pad0[0x2C];
    /* 0x02C */ void *anims;
    /* 0x030 */ u8 pad30[0xF0 - 0x30];
    /* 0x0F0 */ void *cbF0;
    /* 0x0F4 */ void *cbF4;
    /* 0x0F8 */ void *cbF8;
    /* 0x0FC */ void *cbFC;
    /* 0x100 */ u8 pad100[0x108 - 0x100];
    /* 0x108 */ void *cb108;
} CAI;

typedef struct {
    /* 0x000 */ u8 pad0[0xD4];
    /* 0x0D4 */ s32 flags;
} Obj;

extern State D_8012FB70;
extern s32 D_80110048;
extern u8 D_800F7078[];
extern u32 D_800F1F10[];
extern u8 D_00431F00[];

s32 func_00225F6C(void *, s32, s32, s32, LevelActor **, s32);
s32 func_00225EBC(void *, s32, s32);
void func_00243414(Obj *, CAI *, s32);
void func_002362E4(void *, Obj *);
void func_0040BC64();
void func_0040BEE4();
void func_0040BFF0();
void func_0025E4B8();
extern u8 func_00408FF8[];

void func_00409340(Obj *obj, CAI *ai) {
    State *s = &D_8012FB70;
    s32 i;

    D_80110048 = 0x1DB1;
    s->unk680 = 0;
    s->unk684 = 0;
    s->count = func_00225F6C(D_800F7078, 0x650, 10, 0x1DB1, s->actors, 6);
    for (i = 0; i < s->count; i++) {
        s->actors[i]->active = 1;
    }
    s->alive = s->count;
    func_00225F6C(D_800F7078, 0x650, 10, 0x1DB2, s->core, 1);
    s->core[0]->active = 1;
    s->coreCount = func_00225EBC(D_800F7078, 1, 0x1DB1);
    ai->anims = D_00431F00;
    ai->cbF0 = func_0040BC64;
    ai->cbF8 = func_00408FF8 + 8;
    ai->cb108 = func_0025E4B8;
    ai->cbFC = func_0040BFF0;
    ai->cbF4 = func_0040BEE4;
    func_00243414(obj, ai, 0);
    D_8012FB70.obj = obj;
    D_800F1F10[0] &= ~0x300;
    obj->flags |= 0x04000000;
    for (i = 0; i < 15; i++) {
        func_002362E4(D_8012FB70.slots[i].data, obj);
    }
}
