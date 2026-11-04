#include "common.h"

/* Saves the current values (0x18-0x28, 0xC4-0xC5) into the saved copy
 * (0xB0-0xC0, 0xC6-0xC7) and clears four of them. */
typedef struct {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ s32 cur[5];
    /* 0x2C */ u8 pad2C[0xB0 - 0x2C];
    /* 0xB0 */ s32 saved[5];
    /* 0xC4 */ u8 curByte[2];
    /* 0xC6 */ u8 savedByte[2];
} SaveState;

void func_0028582C(SaveState *arg0) {
    arg0->savedByte[0] = arg0->curByte[0];
    arg0->savedByte[1] = arg0->curByte[1];
    arg0->saved[0] = arg0->cur[0];
    arg0->saved[1] = arg0->cur[1];
    arg0->saved[2] = arg0->cur[2];
    arg0->saved[3] = arg0->cur[3];
    arg0->saved[4] = arg0->cur[4];
    arg0->cur[1] = 0;
    arg0->cur[2] = 0;
    arg0->cur[3] = 0;
    arg0->cur[4] = 0;
}
