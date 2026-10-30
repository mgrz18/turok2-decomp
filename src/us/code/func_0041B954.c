/* WRONG CUT: real function 0x0041B954, size 0x6C: splat cut it into func_0041B954 and func_0041B9B0 at branch targets. */
#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[8];
    /* 0x08 */ u32 flags;
    /* 0x0C */ u8 padC[8];
    /* 0x14 */ char *text;
} MenuItemTmpl;

extern u32 D_8011AAD8; /* game option bits */
extern u32 D_8011AADC; /* unlocked option bits */
extern char D_800B5ABC[];
extern char D_800B5AD8[];

/* On/off option item for bit 0x4000: shown only when unlocked. */
s32 func_0041B954(MenuItemTmpl *a) {
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
