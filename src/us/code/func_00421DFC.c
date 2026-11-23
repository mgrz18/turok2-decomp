#include "common.h"
#include "structs/menu.h"
#include "globals/pak.h"

#define T2_TEXT(s) ((s32 *) (s))

extern char D_800B490C[]; /* the port: a string (gendata copies it as bytes, not as a swapped word) */

s32 PakMenu_UpdateCancel(MenuItem *arg0) {
    if (gPakMenuAnyPak != 0) {
        arg0->table = T2_TEXT(D_800B490C);
    }
    return 0;
}
