/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802B7E2C_eu_padding_A25BC_15.c, func_802B7A20_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "globals/libultra.h"

void __osPackRequestData(u8 cmd) {
    u8 *ptr;
    __OSContRequesFormat requestformat;
    s32 i;
    u8 *count;
    OSPifRam *pifram;
    for (i = 0; i < 15; i++) {
        ((u32 *)&__osContBuffer)[i] = 0;
    }
    pifram = &__osContBuffer;
    pifram->pifstatus = 1;
    ptr = (u8 *)pifram->ramarray;
    requestformat.dummy = 0xFF;
    requestformat.txsize = 1;
    requestformat.rxsize = 3;
    requestformat.cmd = cmd;
    requestformat.typeh = 0xFF;
    requestformat.typel = 0xFF;
    requestformat.status = 0xFF;
    requestformat.dummy1 = 0xFF;
    count = &__osContNumControllers;
    for (i = 0; i < *count; i++) {
        *(__OSContRequesFormat *)ptr = requestformat;
        ptr += sizeof(requestformat);
    }
    *ptr = 0xFE;
}
