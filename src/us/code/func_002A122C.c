/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802BE0D0_de_15.c, func_802C016C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern void adecDecodeFrame(void *arg0);
extern void adecImdctFrame(void *arg0, void *arg1);
extern unsigned short D_800B8BB2;
extern DecodeState D_80125C38;
extern char D_80125EF0;
void *adecNextFrame(void) {
    struct DecodeState *base = &D_80125C38;
    unsigned short *counter;
    char *slots;
    unsigned short temp;
    if (base->count == 0) {
        return 0;
    }
    adecDecodeFrame(base);
    counter = &D_800B8BB2;
    temp = *counter + 1;
    *counter = temp;
    if ((unsigned int)(temp & 0xFFFF) >= 6) {
        *counter = 0;
    }
    slots = &D_80125EF0;
    adecImdctFrame(slots + (*counter << 9), base);
    base->count = base->count - 1;
    return slots + (*counter << 9);
}
