/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8043C688_de_11.c, func_8043CCD0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern s32 D_80130920;
s32 func_00453BA4(void) {
    extern Shared_Entry D_800C1F80;
    s32 index;
    s32 next;
    D_80130920 = -1;
    D_800C1F80.timer = 0;
    if (D_800C1F80.count == 0) {
        return 0;
    }
    if (D_800C1F80.value == 0) {
        return 0;
    }
    index = D_800C1F80.value - 1;
    if (index < 0x17) {
        do {
            next = index + 1;
            D_800C1F80.text[index] = D_800C1F80.text[next];
            index = next;
        } while (index < 0x17);
    }
    D_800C1F80.value--;
    D_800C1F80.count--;
    return 0;
}
