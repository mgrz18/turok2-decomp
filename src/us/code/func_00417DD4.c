#include "common.h"
#include "structs/menu.h"

extern char D_800B42D8[];
extern char D_800C0E70[]; /* "%d"-style format */
extern s32 D_8011ACDC;
extern s32 func_004160F0(MenuItem *a);
extern void func_0029E3E0(char *dst, char *fmt, s32 value);

/* Prints D_8011ACDC over the last four characters of the label. */
s32 func_00417DD4(MenuItem *a) {
    a->text = D_800B42D8;
    func_0029E3E0(a->text + (func_004160F0(a) - 4), D_800C0E70, D_8011ACDC);
    return 0;
}
