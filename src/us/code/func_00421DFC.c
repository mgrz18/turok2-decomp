#include "common.h"
#include "structs/menu.h"
#include "globals/pak.h"

#ifdef TARGET_PORT
#define T2_TEXT(s) ((s32 *) (s))
#else
#define T2_TEXT(s) (&s)
#endif

#ifdef TARGET_PORT
extern char D_800B490C[]; /* the port: a string (gendata copies it as bytes, not as a swapped word) */
#else
extern s32 D_800B490C;
#endif

s32 PakMenu_UpdateCancel(MenuItem *arg0) {
    if (gPakMenuAnyPak != 0) {
        arg0->table = T2_TEXT(D_800B490C);
    }
    return 0;
}
