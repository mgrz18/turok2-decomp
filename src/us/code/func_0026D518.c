/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80279198_de_8.c, func_80279798_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern s16 func_0026D518(s16 *arg0);
extern s32 func_002113A4(void);
s16 func_0026D518(s16 *arg0) {
    s16 *entry;
    s16 *start;
    s32 random;
    s32 total;
    s32 value;
    entry = arg0;
    total = 0;
    start = entry;
    if (*entry != -1) {
        do {
            total += entry[1];
            entry += 2;
        } while (*entry != -1);
    }
    random = func_002113A4();
    entry = start;
    value = random % total;
    total = 0;
    while (*entry != -1) {
        total += entry[1];
        if (total >= value) {
            break;
        }
        entry += 2;
    }
    return *entry;
}
extern s32 func_002113A4(void);
