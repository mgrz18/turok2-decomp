#include "common.h"
#include "structs/menu.h"

extern char D_800B42D8[];
extern char D_800C0E70[]; /* "%d"-style format */
extern s32 D_8011ACDC;
extern s32 MenuItem_TextLength(MenuItem *a);
extern void sprintf(char *dst, char *fmt, s32 value);

/* Prints D_8011ACDC over the last four characters of the label. */
s32 FinalScoreMenu_BlueScoreLabel(MenuItem *a) {
    a->text = D_800B42D8;
    sprintf(a->text + (MenuItem_TextLength(a) - 4), D_800C0E70, D_8011ACDC);
    return 0;
}
