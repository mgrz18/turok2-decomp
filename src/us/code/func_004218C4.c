/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80409A5C_de_4.c, func_80409E10_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed int s32;
extern s32 func_0028591C(s32);
extern void func_00284CE0();
extern s32 D_8011F148;
extern s32 D_8011F13C;
extern s32 D_801309BC;
extern s32 D_8011B108;
extern s32 D_8011F144;
extern s32 D_800C2028;
extern s32 D_800F6CB0;
extern void func_002886AC(void *arg0);
s32 func_004218C4(s32 arg0) {
    extern s32 func_0041DD90(s32, s32, s32);
    if (D_8011B108 == 0) {
        func_002886AC(&D_800F6CB0);
        func_00284CE0();
        if (D_8011B108 == 0) {
            if (D_8011F144 != 0) {
                D_8011F148 = 1;
            }
            D_8011F13C = 0;
            D_801309BC = 1;
        }
    }
    if (D_801309BC == 0) {
        if ((D_800C2028 != -1) && (func_0028591C(D_800C2028) != 0) && (func_0041DD90(arg0, D_800C2028, 0) != 0)) {
            D_8011F13C = 0;
            D_801309BC = 1;
        }
    }
    return D_801309BC;
}
