#include "common.h"
#include "structs/game.h"
#include "structs/menu.h"

extern char D_800C0E84[]; /* "  %02d" */
extern char D_800C0E8C[]; /* "none" */
extern s32 func_004160F0(MenuItem *a);
extern void func_0029E3E0(char *dst, char *fmt, ...);

/* Prints the time limit into the label, or "none". */
s32 func_004184A8(MenuItem *a) {
    GameSettings *setup = &D_8011AAD8;

    if (setup->tagTimeLimit != 0) {
        func_0029E3E0(a->text + (func_004160F0(a) - 4), D_800C0E84, setup->tagTimeLimit);
    } else {
        func_0029E3E0(a->text + (func_004160F0(a) - 4), D_800C0E8C);
    }
    return 0;
}
