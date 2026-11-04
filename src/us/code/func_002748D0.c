#include "common.h"

/* The owner of the voice: a free list at 0x84 and a slot table at 0xDC. */
typedef struct {
    /* 0x00 */ u8 pad0[0x84];
    /* 0x84 */ u8 freeList[0xDC - 0x84];
    /* 0xDC */ s16 slots[1];
} VoiceOwner;

typedef struct {
    /* 0x00 */ s32 index;
    /* 0x04 */ u8 pad4[4];
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ u8 pad10[0x38 - 0x10];
    /* 0x38 */ s16 unk38;
    /* 0x3A */ s16 unk3A;
    /* 0x3C */ u8 pad3C[0xA8 - 0x3C];
    /* 0xA8 */ s32 unkA8;
    /* 0xAC */ u8 padAC[4];
    /* 0xB0 */ VoiceOwner *owner;
    /* 0xB4 */ s32 unkB4;
} Voice;

s32 func_00293530(void *, s32);

/* Releases the voice slot and resets the voice. */
void func_002748D0(Voice *arg0) {
    s32 temp_a0;
    u8 *var_s1;

    temp_a0 = (s32) arg0->owner;
    var_s1 = (u8 *) (temp_a0 + 0x7C);       /* slots - 0x60 */
    func_00293530((void *) (temp_a0 + 0x84), *(s16 *) (var_s1 + (arg0->index << 1) + 0x60));
    var_s1 += arg0->index * 2;
    *(s16 *) (var_s1 + 0x60) = -1;          /* owner->slots[index] */
    arg0->unk38 = 0;
    arg0->unkC = -1;
    arg0->unk8 = -1;
    arg0->unkA8 = -1;
    arg0->unk3A = -1;
    arg0->unkB4 = -1;
}
