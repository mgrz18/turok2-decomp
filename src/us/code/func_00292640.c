/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/sdk/libgultra/part_800C7F20/code_802B66A0_2.c, func_802B16E0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern char D_800AA380;
extern char D_800AA384;
extern void func_0029B020(void *arg0, void *arg1, s32 arg2);
void *func_00292640(void *arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 high;
    s32 low;
    s32 middle;
    u8 x;
    u8 y;
    ALInstrument *table;
    ALSound_s *entry;
    u8 *bounds;
    table = ((ALSeqPlayer *)arg0)->chanState[arg3 & 0xFF].instrument;
    high = table->soundCount;
    low = 1;
    if (table == 0) {
        func_0029B020(&D_800AA380, &D_800AA384, 0x376);
    }
    if (high > 0) {
        x = arg1;
        y = arg2;
        do {
            middle = (low + high) / 2;
            entry = table->soundArray[middle - 1];
            bounds = (u8 *)entry->keyMap;
            if (x >= bounds[2] && x <= bounds[3] &&
                y >= bounds[0] && y <= bounds[1]) {
                return entry;
            }
            if (x < bounds[2] ||
                (y < bounds[0] && x <= bounds[3])) {
                high = middle - 1;
            } else {
                low = middle + 1;
            }
        } while (low <= high);
    }
    return 0;
}
