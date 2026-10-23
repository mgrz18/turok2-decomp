/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/ITEMS/80444030_6.CPP, func_804446D8_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed char s8;
typedef signed int s32;
/* Options block; Turok 2 keeps this option as a signed byte at 0x20. */
typedef struct OptionsBlock { s8 pad0[0x20]; s8 option20; } OptionsBlock;
extern OptionsBlock D_8011AAD8;
extern s32 func_0041648C(void *, s32, s32, s32, s32, s32);
s32 func_00418A34(void *first, void *second) {
    OptionsBlock *opts = &D_8011AAD8;
    opts->option20 = func_0041648C(second, opts->option20, 5, 0, 0x1E, 1);
    return 0;
}
