#include "common.h"

/* Clears the 0x250/0x254 fields of every actor of the module, resets the module (func_0040A5B4) and queues func_0040C6F4. */

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ u8 data[0x68 - 0x4];
} Slot;

typedef struct {
    /* 0x000 */ u8 pad0[0x234];
    /* 0x234 */ void *callback234;
    /* 0x238 */ u8 pad238[0x248 - 0x238];
    /* 0x248 */ void *callback248;
    /* 0x24C */ u8 pad24C[0x250 - 0x24C];
    /* 0x250 */ s32 unk250;
    /* 0x254 */ s32 unk254;
} LevelActor;

typedef struct {
    /* 0x000 */ void *obj;
    /* 0x004 */ s32 count;
    /* 0x008 */ LevelActor *actors[4];
    /* 0x018 */ Slot slots[15];
    /* 0x630 */ u8 pad630[0x65C - 0x630];
    /* 0x65C */ s32 phase;
    /* 0x660 */ s32 activeMask;
    /* 0x664 */ s32 unk664;
    /* 0x668 */ u8 pad668[0x674 - 0x668];
    /* 0x674 */ s32 unk674;
    /* 0x678 */ s32 unk678;
    /* 0x67C */ s32 counter;
    /* 0x680 */ u8 pad680[0x688 - 0x680];
    /* 0x688 */ s32 unk688;
    /* 0x68C */ s32 unk68C;
} State;

extern State D_8012FB70;
extern void func_0040A5B4(void *obj, void *ai);
extern void func_0040C6F4(void);
extern s32 func_00283FF8(s32 id, void *arg1, void *arg2);

void func_0040B180(void *obj, void *ai) {
    State *state = &D_8012FB70;
    LevelActor *actor;
    s32 i;

    for (i = 0; i < state->count; i++) {
        actor = state->actors[i];
        actor->unk254 = 0;
        actor->unk250 = 0;
    }
    func_0040A5B4(obj, ai);
    func_00283FF8(0xD2, func_0040C6F4, NULL);
}
