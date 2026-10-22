/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_00409824.c, func_00409824.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed int s32;
extern char D_800B45D8[];
extern char D_800B45D4[];
s32 func_00426D8C(char *s);
s32 func_00426D54(char *a, char *b);
s32 func_00421260(char *a, char *b) {
    if (func_00426D8C(b) == 4 && func_00426D8C(a) == 2 && func_00426D54(b, D_800B45D8) == 0 && func_00426D54(a, D_800B45D4) == 0) {
        return 1;
    }
    return 0;
}
