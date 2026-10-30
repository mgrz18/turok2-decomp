#include "common.h"

/* A timer whose durations are read from a bit stream (see func_0042EC30). */
typedef struct Timer {
    /* 0x00 */ s32 value;
    /* 0x04 */ u8 *data;
    /* 0x08 */ u32 bitPos;
    /* 0x0C */ s32 count;
    /* 0x10 */ s32 index;
    /* 0x14 */ void (*callback)(struct Timer *);
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 unk1C;
} Timer; /* size 0x20 */

/* Reads count bits, most significant first. */
s32 func_0042F700(Timer *r, s32 count) {
    s32 v = 0;
    u32 pos;

    for (count--; count != -1; count--) {
        v <<= 1;
        pos = r->bitPos++;
        v |= (r->data[pos >> 3] & (0x80 >> (pos & 7))) != 0;
    }
    return v;
}
