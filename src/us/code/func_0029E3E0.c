/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802BD218_us_padding_BDE18_18.c, func_802BD320_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed int s32;
extern void * func_0029E438(void * arg0, const char * arg1, int arg2);
extern s32 func_0029E4F0(void *(*)(void *, const char *, s32), void *, const char *, char *);
s32 func_0029E3E0(char *buffer, const char *format, ...) {
    s32 length = func_0029E4F0(func_0029E438, buffer, format, __builtin_next_arg(format));
    if (length >= 0) {
        buffer[length] = 0;
    }
    return length;
}
