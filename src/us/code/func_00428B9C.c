/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80446F60_de_2.c, func_8044799C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern u8 D_80121420;
void func_00428B9C(u8 cmd) {
    extern OSPifRam D_80130E20;
    extern u8 D_801213DC;
    u8 *ptr;
    __OSContRequesFormat requestformat;
    s32 i;
    u8 *count;
    D_80121420 = cmd;
    ptr = (u8 *)&D_80130E20;
    D_80130E20.pifstatus = 1;
    requestformat.dummy = 0xFF;
    requestformat.txsize = 1;
    requestformat.rxsize = 3;
    requestformat.cmd = cmd;
    requestformat.typeh = 0xFF;
    requestformat.typel = 0xFF;
    requestformat.status = 0xFF;
    requestformat.dummy1 = 0xFF;
    count = &D_801213DC;
    for (i = 0; i < *count; i++) {
        *(__OSContRequesFormat *)ptr = requestformat;
        ptr += sizeof(requestformat);
    }
    *ptr = 0xFE;
}
