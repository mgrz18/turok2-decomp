#include "common.h"
#include "structs/demo.h"

/* Reads the next bit. */
s32 DemoStream_ReadBit(DemoStream *r) {
    u32 bit = r->pos++;

    return (r->bits[bit >> 3] & (0x80 >> (bit & 7))) != 0;
}
