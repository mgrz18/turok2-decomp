/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/ITEMS/80444030_6.CPP, func_804446D8_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed int s32;
typedef signed char s8;
typedef struct {
    u8 pad0[0x2A];
    s8 option2A; /* 0x2A */
} OptionsBlock;
extern OptionsBlock D_8011AAD8;
extern s32 func_0041648C(void *, s32, s32, s32, s32, s32);
s32 func_00418EF4(void *first, void *second) {
    OptionsBlock *opts = &D_8011AAD8;
    opts->option2A = func_0041648C(second, opts->option2A, 5, 0, 30, 1);
    return 0;
}
