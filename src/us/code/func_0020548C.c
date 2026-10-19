/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0025398C.c, func_0025398C.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/junk_types.h"

u32 func_0029DFF0(void);
void func_0029E010(u32 mask);
s32 osSendMesg(OSMesgQueue *mq, OSMesg msg, s32 flag);
s32 osRecvMesg(OSMesgQueue *mq, OSMesg *msg, s32 flag);
extern B12Mgr D_800D7BE0;
extern s32 D_800D8DCC;
void func_0020548C(s32 unused, s32 idx) {
    u32 mask;
    u32 m2;
    mask = func_0029DFF0();
    if (++D_800D8DCC != 1) {
        func_0029E010(mask);
        osRecvMesg(&D_800D7BE0.q11D0, 0, 1);
    } else {
        func_0029E010(mask);
    }
    D_800D7BE0.f638 = idx;
    D_800D7BE0.tbl628[idx] = 1;
    m2 = func_0029DFF0();
    if (--D_800D8DCC != 0) {
        func_0029E010(m2);
        osSendMesg(&D_800D7BE0.q11D0, 0, 1);
    } else {
        func_0029E010(m2);
    }
}
