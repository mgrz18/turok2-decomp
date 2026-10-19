/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802BD218_us_padding_BDE18_18.c, func_802BD3C8_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

unsigned char *func_0029E488(unsigned char *string, unsigned char value) {
    while (*string != value) {
        if (*string == 0) {
            return 0;
        }
        ++string;
    }
    return string;
}
