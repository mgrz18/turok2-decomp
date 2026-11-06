/* WRONG CUT: real function 0x0041B954, size 0x6C: splat cut it into CheatMenu_BlackoutText and func_0041B9B0 at branch targets. */
#include "common.h"
#include "structs/menu.h"

extern u32 D_8011AAD8; /* game option bits */
extern u32 D_8011AADC; /* unlocked option bits */
extern char D_800B5ABC[];
extern char D_800B5AD8[];

/* On/off option item for bit 0x4000: shown only when unlocked. */
s32 CheatMenu_BlackoutText(MenuItem *a) {
    if (D_8011AADC & 0x4000) {
        a->flags |= 0x1000000;
    } else {
        a->flags &= ~0x1000000;
    }
    if (D_8011AAD8 & 0x4000) {
        a->text = D_800B5ABC;
    } else {
        a->text = D_800B5AD8;
    }
    return 0;
}
