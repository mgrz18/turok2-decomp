/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_00409824.c, func_00409824.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed int s32;
extern char D_800B45D8[];
extern char D_800B45D4[];
s32 Str_Length_2(char *s);
s32 Str_Compare_2(char *a, char *b);
s32 Pak_IsTurok2Note_2(char *a, char *b) {
    if (Str_Length_2(b) == 4 && Str_Length_2(a) == 2 && Str_Compare_2(b, D_800B45D8) == 0 && Str_Compare_2(a, D_800B45D4) == 0) {
        return 1;
    }
    return 0;
}
