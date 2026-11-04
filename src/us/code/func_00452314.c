#include "common.h"
#include "structs/game.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MULTI/8043E9A8.CPP, func_8043E830_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

/* Menu callback: toggle the option selected by arg0[0]. */
s32 func_00452314(s32 *arg0) {
    GameSettings *base;
    switch (arg0[0]) {
        case 2:
            base = &D_8011AAD8;
            base->weaponEnabled[1] ^= 1;
            break;
        case 3:
            base = &D_8011AAD8;
            base->weaponEnabled[2] ^= 1;
            break;
        case 4:
            base = &D_8011AAD8;
            base->weaponEnabled[3] ^= 1;
            break;
        case 5:
            base = &D_8011AAD8;
            base->weaponEnabled[4] ^= 1;
            break;
        case 6:
            base = &D_8011AAD8;
            base->weaponEnabled[5] ^= 1;
            break;
        case 7:
            base = &D_8011AAD8;
            base->weaponEnabled[6] ^= 1;
            break;
        case 8:
            base = &D_8011AAD8;
            base->weaponEnabled[7] ^= 1;
            break;
        case 9:
            base = &D_8011AAD8;
            base->weaponEnabled[9] ^= 1;
            break;
    }
    return 0;
}
