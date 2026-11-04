#include "common.h"
#include "structs/game.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MULTI/8043E9A8_4.CPP, func_8043EBE4_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

extern char D_00438FB4[];
extern void func_00416894(void *, void *, void *);

s32 func_0045275C(void *first, void *second, void *third) {
    GameSettings *options = &D_8011AAD8;
    s32 i;
    options->multiplayer = 1;
    options->playerCount = 4;
    for (i = 0; i < 4; i++) {
        options->slots[i].m_bActive = 0;
    }
    func_00416894(third, second, D_00438FB4);
    return 1;
}
