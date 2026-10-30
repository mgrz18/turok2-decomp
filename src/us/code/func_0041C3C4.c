/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/ovl_a/func_004449FC.c, func_004449FC, with Turok 2's layouts. */
#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ char *text;
} MenuItemTmpl;

extern u16 D_8011AAE8; /* D_8011AAD8 + 0x10 */
extern char D_800B4008[];
extern char D_800B4020[];
extern char D_800B4038[];
extern char D_800C1310[]; /* number format */
extern s32 func_004160F0(MenuItemTmpl *a);
extern void func_0029E3E0(char *dst, char *fmt, ...);

s32 func_0041C3C4(MenuItemTmpl *a) {
    u32 h = D_8011AAE8 >> 4;

    if (h == 0) {
        a->text = D_800B4008;
    } else if (h == 0x10) {
        a->text = D_800B4020;
    } else {
        a->text = D_800B4038;
        func_0029E3E0(a->text + (func_004160F0(a) - 2), D_800C1310, h);
    }
    return 0;
}
