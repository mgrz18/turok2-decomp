/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/sdk/libgultra/part_800C7F20/code_802B7FA0_2.c, func_802B2F10_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern s32 func_00291034(void *, s16 *, s32);
void func_00293E70(ALSndPlayer *arg0, s16 arg1) {
    ParamsRecAt8V1 p;
    s32 acc;
    acc = (s32)arg0->sndState;
    p.f0 = 3;
    acc += arg0->target * 0x30;
    p.f8 = arg1;
    p.f4 = acc;
    func_00291034(&arg0->evtq, (s16 *)&p, 0);
}
