#include "common.h"
#include "structs/menu.h"

extern char D_800B42F0[];
extern char D_800C0E70[]; /* "%d"-style format */
extern s32 D_8011ACE0;
extern s32 func_004160F0(MenuItem *a);
extern void func_0029E3E0(char *dst, char *fmt, s32 value);

/* Prints D_8011ACE0 over the last four characters of the label. */
s32 func_00417E28(MenuItem *a) {
    a->text = D_800B42F0;
    func_0029E3E0(a->text + (func_004160F0(a) - 4), D_800C0E70, D_8011ACE0);
    return 0;
}
