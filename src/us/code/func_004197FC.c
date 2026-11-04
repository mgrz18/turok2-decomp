#include "common.h"
#include "structs/game.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MULTI/8043E9A8_4.CPP, func_8043EBE4_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */


/* Turok 2 has 4 player-settings records of 0x40 bytes (Rage Wars: 8 of 0x96);
 * the base of the array is a guess, only `enabled` (0xA8 for player 0) is known. */

extern char D_00439094[];
extern void func_00416894(void *, void *, void *);

s32 func_004197FC(void *first, void *second, void *third) {
    GameSettings *options = &D_8011AAD8;
    s32 i;
    options->multiplayer = 1;
    options->playerCount = 4;
    for (i = 0; i < 4; i++) {
        options->slots[i].active = 0;
    }
    func_00416894(third, second, D_00439094);
    return 1;
}
