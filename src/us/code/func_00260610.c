#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0xD4];
    /* 0xD4 */ u32 flags;
} Actor;

typedef struct {
    /* 0x00 */ u8 pad0[0x6C];
    /* 0x6C */ s32 unk6C;
    /* 0x70 */ s32 unk70;
    /* 0x74 */ s32 unk74;
} AI;

/* AI callback: clears the AI's 0x6C-0x74 words and the actor's flag 0x20000. */
void func_00260610(Actor *obj, AI *ai) {
    ai->unk6C = 0;
    ai->unk70 = 0;
    ai->unk74 = 0;
    obj->flags &= ~0x20000;
}
