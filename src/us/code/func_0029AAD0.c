/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802BA210_de_29.c, func_802BA700_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern void func_0029AAD0(s32 arg0);
extern __OSViContext *D_800B7AC4;
extern u32 __osDisableInt(void);
extern void __osRestoreInt(u32 arg0);
void func_0029AAD0(s32 arg0) {
    u32 token;
    token = __osDisableInt();
    if (arg0 & 1) {
        D_800B7AC4->control |= 8;
    }
    if (arg0 & 2) {
        D_800B7AC4->control &= ~8;
    }
    if (arg0 & 4) {
        D_800B7AC4->control |= 4;
    }
    if (arg0 & 8) {
        D_800B7AC4->control &= ~4;
    }
    if (arg0 & 0x10) {
        D_800B7AC4->control |= 0x10;
    }
    if (arg0 & 0x20) {
        D_800B7AC4->control &= ~0x10;
    }
    if (arg0 & 0x40) {
        D_800B7AC4->control = (D_800B7AC4->control | 0x10000) & ~0x300;
    }
    if (arg0 & 0x80) {
        D_800B7AC4->control &= ~0x10000;
        D_800B7AC4->control |= D_800B7AC4->modep->comRegs.ctrl & 0x300;
    }
    D_800B7AC4->state |= 8;
    __osRestoreInt(token);
}
