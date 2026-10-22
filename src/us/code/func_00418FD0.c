/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/ITEMS/80444030_6.CPP, func_804446D8_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
typedef signed char s8;
typedef unsigned char u8;
typedef signed int s32;
typedef struct GameOptions {
    u8 pad0[0x2B];
    s8 option2B; /* 0x2B, range 0..99 */
} GameOptions;
extern GameOptions D_8011AAD8;
extern s32 func_0041648C(void *, s32, s32, s32, s32, s32);
s32 func_00418FD0(void *first, void *second) {
    GameOptions *game = &D_8011AAD8;
    game->option2B = func_0041648C(second, game->option2B, 1, 0, 99, 1);
    return 0;
}
