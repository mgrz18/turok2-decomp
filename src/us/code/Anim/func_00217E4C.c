/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802609AC_de.c, func_802609AC_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#ifdef TARGET_PORT
/* bit addresses are offsets from the stream base on the host (func_00217B04.c) */
extern unsigned long gT2BitStreamBase;
#endif

extern f32 Bits_PeekFloat(s32 bitAddress, Func802608ECResult range);
f32 Bits_PeekFloat(s32 bitAddress, Func802608ECResult range) {
    s32 shift;
    u32 *word;
    u32 value;
    u32 next;
    f64 converted;
    f32 result;
#ifdef TARGET_PORT
    word = (u32 *) ((gT2BitStreamBase + (((u32) bitAddress >> 3) & ~3u)));
#else
    word = (u32 *) ((bitAddress & 0xF0000000) |
                    ((u32) (bitAddress & 0x0FFFFFE0) >> 3));
#endif
    shift = bitAddress & 0x1F;
    value = word[0];
    next = word[1];
    if (shift != 0) {
        value >>= shift;
        next <<= 0x20 - shift;
        value |= next;
    }
    if (range.value < 0x20U) {
        value &= (1 << range.value) - 1;
    }
    converted = (s32) value;
    if ((s32) value < 0) {
        converted += 4294967296.0;
    }
    result = (f32) converted;
    result /= (f32) ((1 << range.value) - 1);
    result *= range.delta;
    result += range.start;
    return result;
}
