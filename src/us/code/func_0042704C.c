/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8029F888_de_2.c, func_802A0444_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
extern u8 *func_0042704C(u8 *s, int c);
u8 *func_0042704C(u8 *s, int c) {
    u8 *start = s;
    while (*s++ != 0) {
    }
    s--;
    for (; s != start; s--) {
        if (*s == (u8)c) {
            break;
        }
    }
    if (*s == (u8)c) {
        return s;
    }
    return 0;
}
