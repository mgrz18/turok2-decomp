/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80444CF8_de_3.c, func_804447A0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed int s32;
extern u8 D_8011AAF3;
extern s32 func_0041648C(void *, s32, s32, s32, s32, s32);
s32 func_0041C2E8(void *first, void *second) {
    u8 *option = &D_8011AAF3;
    *option = func_0041648C(second, *option, 1, 0, 1, 1);
    return 0;
}
