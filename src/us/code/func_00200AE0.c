/* WRONG CUT: splat named this D_00200AD4 (0x200AD4, 44 bytes): its first 12 bytes are junk fill
 * ("Color" text), the real function starts at the 16-aligned 0x200AE0 (ROM symbol D_00200AE0), 32 bytes.
 * A load callback (func_00224D54 passes &D_00200AE0): loads a resource without the flag. */

#include "common.h"

s32 func_00200B00(void **res, s32 flag);

s32 func_00200AE0(s32 unused, void **res) {
    return func_00200B00(res, 0);
}
