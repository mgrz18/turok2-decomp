/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80255C4C_de_8.c, func_80256190_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern s32 func_002113A4(void);
void *func_002009F0(void *arg0) {
    s32 index;
    s32 offset;
    void *node;
    node = ((IntrusiveList *)arg0)->head;
    if (node == 0) {
        return 0;
    }
    index = func_002113A4() % (u32)((IntrusiveList *)arg0)->count;
    index--;
    if (index != -1) {
        offset = ((IntrusiveList *)arg0)->nextOffset;
        do {
            { u8 *cursor = (u8 *)node; cursor += offset; node = *(void **)cursor; }
            index--;
        } while (index != -1);
    }
    return node;
}
