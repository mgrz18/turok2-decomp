#include "common.h"
#include "structs/game.h"
#include "structs/menu.h"

extern char D_800C0E84[]; /* "  %02d" */
extern char D_800C0E8C[]; /* "none" */
extern s32 MenuItem_TextLength(MenuItem *a);
extern void sprintf(char *dst, char *fmt, ...);

/* Prints the time limit into the label, or "none". */
s32 FragTagMenu_TimeLimitLabel(MenuItem *a) {
    GameSettings *setup = &D_8011AAD8;

    if (setup->tagTimeLimit != 0) {
        sprintf(a->text + (MenuItem_TextLength(a) - 4), D_800C0E84, setup->tagTimeLimit);
    } else {
        sprintf(a->text + (MenuItem_TextLength(a) - 4), D_800C0E8C);
    }
    return 0;
}
