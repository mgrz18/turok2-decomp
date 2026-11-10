/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8025E590_de_9.c, func_802607B8_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

u32 Bits_Read(u32 *stream, u32 width) {
    u32 bitAddress;
    u32 shift;
    u32 *word;
    u32 value;
    u32 next;
    bitAddress = *stream;
#ifdef TARGET_PORT
    /* a bit offset from the stream's base (BitAddr_FromPointer, func_00217B04.c) */
    {
        extern unsigned long gT2BitStreamBase;
        word = (u32 *) (gT2BitStreamBase + ((bitAddress >> 3) & ~3u));
    }
#else
    word = (u32 *) ((bitAddress & 0xF0000000) |
                    ((bitAddress & 0x0FFFFFE0) >> 3));
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
    *stream = bitAddress + width;
    return value;
}
