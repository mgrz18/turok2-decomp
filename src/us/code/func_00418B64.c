#include "common.h"
#include "structs/game.h"
#include "structs/menu.h"

extern char D_800C0E84[]; /* "  %02d" */
extern char D_800C0E8C[]; /* "none" */
extern s32 func_004160F0(MenuItem *a);
extern void func_0029E3E0(char *dst, char *fmt, ...);

/* Prints the setting at D_8011AAD8+0x21 into the label, or "none". */
s32 func_00418B64(MenuItem *a) {
    GameSettings *setup = &D_8011AAD8;

    if (setup->dmScoreLimit != 0) {
        func_0029E3E0(a->text + (func_004160F0(a) - 4), D_800C0E84, setup->dmScoreLimit);
    } else {
        func_0029E3E0(a->text + (func_004160F0(a) - 4), D_800C0E8C);
    }
    return 0;
}
