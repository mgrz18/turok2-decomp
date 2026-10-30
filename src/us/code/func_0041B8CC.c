/* WRONG CUT: real function 0x0041B8CC, size 0x6C: splat cut it into func_0041B8CC, func_0041B8EC and func_0041B920 at branch targets. */
#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[8];
    /* 0x08 */ u32 flags;
    /* 0x0C */ u8 padC[8];
    /* 0x14 */ char *text;
} MenuItemTmpl;

extern u32 D_8011AAD8; /* game option bits */
extern u32 D_8011AADC; /* unlocked option bits */
extern char D_800B5A84[];
extern char D_800B5AA0[];

/* On/off option item for bit 0x2000: shown only when unlocked. */
s32 func_0041B8CC(MenuItemTmpl *a) {
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
