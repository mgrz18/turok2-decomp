/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802BD218_us_padding_BDE18_18.c, func_802BD320_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern void * proutSprintf(void * arg0, const char * arg1, int arg2);
extern s32 _Printf(void *(*)(void *, const char *, s32), void *, const char *, char *);
s32 sprintf(char *buffer, const char *format, ...) {
    s32 length = _Printf(proutSprintf, buffer, format, __builtin_next_arg(format));
    if (length >= 0) {
        buffer[length] = 0;
    }
    return length;
}
