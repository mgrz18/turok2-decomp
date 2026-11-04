#include "common.h"

/* The bank a channel plays from: a voice at 0x84, sound ids at 0xDC. */
typedef struct {
    /* 0x000 */ u8 pad0[0x84];
    /* 0x084 */ u8 voice[0xDC - 0x84];
    /* 0x0DC */ s16 sounds[(0x104 - 0xDC) / 2];
    /* 0x104 */ s32 current;
} Bank;

typedef struct {
    /* 0x00 */ s32 index;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ u8 pad8[0x10 - 0x8];
    /* 0x10 */ s32 id;
    /* 0x14 */ u8 pad14[0x50 - 0x14];
    /* 0x50 */ s32 state;
    /* 0x54 */ u8 pad54[0xAC - 0x54];
    /* 0xAC */ s32 unkAC;
    /* 0xB0 */ Bank *bank;
} Channel;

s32 func_00293EC0(s32);
s32 func_00293580(void *);                          /* extern */
void func_00293E60(void *, s16);                    /* extern */

void func_00274850(Channel *arg0) {
    Bank *temp_a0;
    void *temp_s0;

    temp_a0 = arg0->bank;
    arg0->unkAC = 1;
    arg0->state = 0;
    temp_s0 = temp_a0->voice;
    if (arg0->id != temp_a0->current) {
        func_00293E60(temp_s0, temp_a0->sounds[arg0->index]);
        if (func_00293580(temp_s0) != 0) {
            func_00293EC0((s32) temp_s0);
        }
        arg0->unk4 = -1;
    }
}
