/* WRONG CUT: real function 0x0041B8CC, size 0x6C: splat cut it into CheatMenu_AllMapText, func_0041B8EC and func_0041B920 at branch targets. */
#include "common.h"
#include "structs/menu.h"

extern u32 D_8011AAD8; /* game option bits */
extern u32 D_8011AADC; /* unlocked option bits */
extern char D_800B5A84[];
extern char D_800B5AA0[];

/* On/off option item for bit 0x2000: shown only when unlocked. */
s32 CheatMenu_AllMapText(MenuItem *a) {
    if (D_8011AADC & 0x2000) {
        a->flags |= 0x1000000;
    } else {
        a->flags &= ~0x1000000;
    }
    if (D_8011AAD8 & 0x2000) {
        a->text = D_800B5A84;
    } else {
        a->text = D_800B5AA0;
    }
    return 0;
}
