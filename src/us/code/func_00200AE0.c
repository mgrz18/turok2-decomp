/* WRONG CUT: splat named this D_00200AD4 (0x200AD4, 44 bytes): its first 12 bytes are junk fill
 * ("Color" text), the real function starts at the 16-aligned 0x200AE0 (ROM symbol D_00200AE0), 32 bytes.
 * A load callback (CScene__LoadBinaryBlock passes &D_00200AE0): loads a resource without the flag. */

#include "common.h"

s32 ProPackDecompress(void **res, s32 flag);

s32 ProPack__Decompress(s32 unused, void **res) {
    return ProPackDecompress(res, 0);
}
