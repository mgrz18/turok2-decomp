/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/ITEMS/80444030_6.CPP, func_804446D8_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed char s8;
typedef unsigned char u8;
typedef signed int s32;
struct GlobalOptions {
    u8 pad0[0x21];
    s8 option21; /* Turok 2 layout */
};
extern struct GlobalOptions D_8011AAD8;
extern s32 func_0041648C(void *, s32, s32, s32, s32, s32);
s32 func_00418B10(void *first, void *second) {
    struct GlobalOptions *options = &D_8011AAD8;
    options->option21 = func_0041648C(second, options->option21, 1, 0, 0x63, 1);
    return 0;
}
