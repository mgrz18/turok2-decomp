#include "common.h"
#include "m2c_macros.h"

#ifdef TARGET_PORT
/* The N64 packs a KSEG0 pointer into a bit address: the segment nibble, then the byte
 * offset times 8 in the low 28 bits. A host heap pointer does not fit (above 32 MB of
 * offset the bits run into the nibble; 64-bit pointers lose their top half), so on the
 * host the stream is a bit offset from a base kept here, and Bits_Read
 * (func_00217C58.c) adds it back. One stream at a time: Anim_DecodeTracks (the
 * decompressor thread) is the only caller. */
unsigned long gT2BitStreamBase;

s32 BitAddr_FromPointer(sptr arg0) {
    gT2BitStreamBase = (unsigned long) arg0 & ~3ul;
    return (arg0 & 3) * 8;
}
#else
s32 BitAddr_FromPointer(sptr arg0) {
    return (arg0 & 0xF0000000) | ((arg0 & 0x0FFFFFFF) * 8);
}
#endif
