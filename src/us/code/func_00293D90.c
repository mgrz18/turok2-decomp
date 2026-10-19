/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/sdk/libgultra/part_800C7F20/sndpsetpan.c, alSndpSetPan.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern s32 func_00291034(void *, s16 *, s32);
s32 func_00293D90(struct ObjRecAt40V2 *arg0, s8 arg1) {
    ParamsRecAt8V2 p;
    s32 acc;
    acc = arg0->field40;
    p.f0 = 2;
    p.f8 = arg1;
    acc += arg0->field3C * 0x30;
    p.f4 = acc;
    func_00291034(&arg0->events, (s16 *)&p, 0);
}
