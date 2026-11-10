/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8025E590_de_9.c, func_802606C4_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#ifdef TARGET_PORT
/* bit addresses are offsets from the stream base on the host (func_00217B04.c) */
extern unsigned long gT2BitStreamBase;
#endif

int Bits_Peek(int bitAddress, unsigned int width) {
    int shift;
    int *word;
    unsigned int value;
    unsigned int next;
#ifdef TARGET_PORT
    word = (int *) ((gT2BitStreamBase + (((u32) bitAddress >> 3) & ~3u)));
#else
    word = (int *) ((bitAddress & 0xF0000000) |
                    ((unsigned int) (bitAddress & 0x0FFFFFE0) >> 3));
#endif
    shift = bitAddress & 0x1F;
    value = word[0];
    next = word[1];
    if (shift != 0) {
        value >>= shift;
        next <<= 0x20 - shift;
        value |= next;
    }
    if (width < 0x20U) {
        value &= (1 << width) - 1;
    }
    return value;
}
