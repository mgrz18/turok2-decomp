/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80265350_de.c, func_80265444_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

s32 func_00266C80(u8 *arg0, u8 *arg1, s32 arg2) {
    u8 *end = arg0 + arg2;
    if (arg2 >= 4 && !((u32)arg0 & 3) && !((u32)arg1 & 3)) {
        end -= 4;
        if (end < arg0) {
            end += 4;
            goto byte_loop;
        }
word_loop:
        if (*(u32 *)arg0 != *(u32 *)arg1) {
            goto word_mismatch;
        }
        arg0 += 4;
        goto word_continue;
word_mismatch:
        arg0 -= 4;
        arg1 -= 4;
        goto word_done;
word_continue:
        arg1 += 4;
        if (arg0 <= end) {
            goto word_loop;
        }
word_done:
        end += 4;
    }
byte_loop:
    while (arg0 < end) {
        u8 left = *arg0;
        u8 right = *arg1;
        if (left != right) {
            if (left < right) {
                return -1;
            }
            return 1;
        }
        arg0++;
        arg1++;
    }
    return 0;
}
