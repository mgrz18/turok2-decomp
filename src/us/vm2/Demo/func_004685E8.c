/* The second VM module's copy of src/us/vm1/Demo/func_0042F6C8.c (byte for byte, at the same link address or 0xE0 below
 * it, docs/VMASM.md), with the second module's names. */
#include "common.h"
#include "structs/demo.h"

/* Reads the next bit. */
s32 func_004685E8(DemoStream *r) {
    u32 bit = r->pos++;

    return (r->bits[bit >> 3] & (0x80 >> (bit & 7))) != 0;
}
