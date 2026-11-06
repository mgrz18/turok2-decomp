/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80446F60_de_2.c, func_8044799C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "globals/libultra.h"

void __osPfsRequestData(u8 cmd) {
    u8 *ptr;
    __OSContRequesFormat requestformat;
    s32 i;
    u8 *count;
    __osContLastCmd = cmd;
    ptr = (u8 *)&__osPfsPifRam;
    __osPfsPifRam.pifstatus = 1;
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
