/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/ITEMS/80444030_6.CPP, func_804446D8_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed char s8;
typedef signed int s32;
/* Turok 2: the option lives at +0x27 of this options block (s8). */
struct Options8011AAD8 {
    u8 pad0[0x27];
    s8 option27;
};
extern struct Options8011AAD8 D_8011AAD8;
extern s32 func_0041648C(void *, s32, s32, s32, s32, s32);
s32 func_0041860C(void *first, void *second) {
    struct Options8011AAD8 *options = &D_8011AAD8;
    options->option27 = func_0041648C(second, options->option27, 1, 0, 6, 1);
    return 0;
}
