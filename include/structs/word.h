#ifndef STRUCTS_WORD_H
#define STRUCTS_WORD_H

/* A 4-byte value read as whatever its record says it is: the AI / event parameter
 * words of the imported Rage Wars level and AI code (they were the local unions
 * M1W4 / M1IF / B01W4). */

#include "common.h"

typedef union CWord {
    s32 i;
    f32 f;
    s16 h[2];
    u16 hu[2];
    u8 b[4];
    u32 u;
} CWord; /* size 0x4 */

#endif
