/* MATCHES at its real start (checked with the symbol moved to 0x42EB50, size 0xC0). */
/* WRONG CUT: the real function starts at 0x00467B50 (size 0xC0 = 192); the word
 * at 0x00467B4C (lw v1,0x20(sp)) is junk between objects. */
#include "common.h"
#include "structs/demo.h"

/* Bit stream: read a length code (3 bits giving a bit count n, then n bits). */

#define READ_BITS(br, n, value)                                              \
    {                                                                        \
        s32 _n = (n);                                                        \
        u32 _pos;                                                            \
        value = 0;                                                           \
        while (--_n != -1) {                                                  \
            _pos = (br)->pos++;                                              \
            value <<= 1;                                                     \
            value |= ((br)->bits[_pos >> 3] & (0x80 >> (_pos & 7))) != 0;   \
        }                                                                    \
    }

void Demo_ReadRunLength_2(DemoStream *br) {
    s32 n;
    s32 bits;

    READ_BITS(br, 3, n);
    if (n != 0) {
        READ_BITS(br, n, bits);
        br->frames = bits + 1;
    } else {
        br->frames = 1;
    }
}
