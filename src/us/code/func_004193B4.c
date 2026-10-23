/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MULTI/8043E9A8.CPP, func_8043E830_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed int s32;

/* Turok 2: the multiplayer option toggles sit at 0x2C (Rage Wars 0x28). */
typedef struct Shared_Game {
    char pad0[0x2C];
    u8 options[0x10];          /* 0x2C */
} Shared_Game;

extern Shared_Game D_8011AAD8;

/* Menu callback: toggle the option selected by arg0[0]. */
s32 func_004193B4(s32 *arg0) {
    Shared_Game *base;
    switch (arg0[0]) {
        case 2:
            base = &D_8011AAD8;
            base->options[5] ^= 1;
            break;
        case 3:
            base = &D_8011AAD8;
            base->options[6] ^= 1;
            break;
        case 4:
            base = &D_8011AAD8;
            base->options[7] ^= 1;
            break;
        case 5:
            base = &D_8011AAD8;
            base->options[8] ^= 1;
            break;
        case 6:
            base = &D_8011AAD8;
            base->options[9] ^= 1;
            break;
        case 7:
            base = &D_8011AAD8;
            base->options[10] ^= 1;
            break;
        case 8:
            base = &D_8011AAD8;
            base->options[11] ^= 1;
            break;
        case 9:
            base = &D_8011AAD8;
            base->options[13] ^= 1;
            break;
    }
    return 0;
}
