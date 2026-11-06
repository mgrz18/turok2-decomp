/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802BA210_de_29.c, func_802BA700_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern void osViSetSpecialFeatures(s32 arg0);
extern __OSViContext *__osViNext;
extern u32 __osDisableInt(void);
extern void __osRestoreInt(u32 arg0);
void osViSetSpecialFeatures(s32 arg0) {
    u32 token;
    token = __osDisableInt();
    if (arg0 & 1) {
        __osViNext->control |= 8;
    }
    if (arg0 & 2) {
        __osViNext->control &= ~8;
    }
    if (arg0 & 4) {
        __osViNext->control |= 4;
    }
    if (arg0 & 8) {
        __osViNext->control &= ~4;
    }
    if (arg0 & 0x10) {
        __osViNext->control |= 0x10;
    }
    if (arg0 & 0x20) {
        __osViNext->control &= ~0x10;
    }
    if (arg0 & 0x40) {
        __osViNext->control = (__osViNext->control | 0x10000) & ~0x300;
    }
    if (arg0 & 0x80) {
        __osViNext->control &= ~0x10000;
        __osViNext->control |= __osViNext->modep->comRegs.ctrl & 0x300;
    }
    __osViNext->state |= 8;
    __osRestoreInt(token);
}
