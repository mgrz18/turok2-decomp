/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/sdk/libgultra/part_800C7F20/sndpsetpitch.c, alSndpSetPitch.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern s32 func_00291034(void *, s16 *, s32);
void func_00293DE0(struct ObjRecAt40V2 *arg0, s32 arg1) {
    ParamsRecAt8V3 p;
    s32 acc;
    acc = arg0->field40;
    p.f0 = 4;
    acc += arg0->field3C * 0x30;
    p.f8 = arg1;
    p.f4 = acc;
    func_00291034(&arg0->events, (s16 *)&p, 0);
}
