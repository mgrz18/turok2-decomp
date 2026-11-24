/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8028DF90_de_7.c, func_8028FA0C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
#include "common.h"
#include "protos/sched.h"
#include "protos/ultra.h"

s32 __scTaskComplete(OSSched *arg0, OSScTask *arg1) {
    if (((s32)arg1->state) & 3) {
        return 0;
    }
    if ((arg1->list.t.type) != 1) {
        return 1;
    }
    if ((((s32)arg1->flags) & 0x60) != 0x60) {
        return 1;
    }
    osViSwapBuffer(arg1->framebuffer);
    return 1;
}
