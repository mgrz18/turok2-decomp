/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/ITEMS/80444030_6.CPP, func_804446D8_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed char s8;
typedef signed int s32;
typedef struct GameOptions {
    u8 pad0[0x26];
    s8 option26;
} GameOptions;
extern GameOptions D_8011AAD8;
extern s32 func_0041648C(void *, s32, s32, s32, s32, s32);
s32 func_00418530(void *first, void *second) {
    GameOptions *options = &D_8011AAD8;
    options->option26 = func_0041648C(second, options->option26, 1, 0, 0x63, 1);
    return 0;
}
