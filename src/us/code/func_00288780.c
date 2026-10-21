/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00293424.c, func_00293424.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed int s32;
s32 func_00288780(u8 *a, u8 *b) {
    if (a == ((void *)0) || b == ((void *)0)) {
        return -1;
    }
    while (*a != 0 && *b != 0) {
        if (*a < *b) {
            return -1;
        }
        if (*b < *a) {
            return 1;
        }
        a++;
        b++;
    }
    if (*a != 0) {
        return 1;
    }
    if (*b != 0) {
        return -1;
    }
    return 0;
}
