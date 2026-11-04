#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ s32 flags;
    /* 0x08 */ u8 pad8[0x10 - 0x8];
    /* 0x10 */ s32 state;
} Stream;

/* Only the field read here. */
typedef struct {
    /* 0x000 */ u8 pad0[0x2F4];
    /* 0x2F4 */ Stream *stream;
} StreamOwner;

void func_0029A4F0(Stream *);                       /* extern */

void func_0028B274(StreamOwner *arg0) {
    Stream *temp_a0;

    temp_a0 = arg0->stream;
    if (temp_a0->state == 1) {
        temp_a0->flags = (s32) (temp_a0->flags | 0x10);
        func_0029A4F0(temp_a0);
    }
}
