/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/ITEMS/80444030_6.CPP, func_804446D8_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed char s8;
typedef signed int s32;
/* game options block; Turok 2 keeps this option as a signed byte at +0x25 */
typedef struct { s8 pad0[0x25]; s8 option; } Options;
extern Options D_8011AAD8;
extern s32 func_0041648C(void *, s32, s32, s32, s32, s32);
s32 func_00418454(void *first, void *second) {
    Options *opts = &D_8011AAD8;
    opts->option = func_0041648C(second, opts->option, 5, 0, 0x1E, 1);
    return 0;
}
