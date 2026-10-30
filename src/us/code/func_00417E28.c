#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ char *text;
} MenuItemTmpl;

extern char D_800B42F0[];
extern char D_800C0E70[]; /* "%d"-style format */
extern s32 D_8011ACE0;
extern s32 func_004160F0(MenuItemTmpl *a);
extern void func_0029E3E0(char *dst, char *fmt, s32 value);

/* Prints D_8011ACE0 over the last four characters of the label. */
s32 func_00417E28(MenuItemTmpl *a) {
    a->text = D_800B42F0;
    func_0029E3E0(a->text + (func_004160F0(a) - 4), D_800C0E70, D_8011ACE0);
    return 0;
}
