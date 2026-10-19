/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_002533DC.c, func_002533DC.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/junk_types.h"

u32 func_0029DFF0(void);
void func_0029E010(u32 mask);
s32 osSendMesg(OSMesgQueue *mq, OSMesg msg, s32 flag);
s32 osRecvMesg(OSMesgQueue *mq, OSMesg *msg, s32 flag);
extern OSMesgQueue D_800D8DB0;
extern s32 D_800D8DCC;
B12Node *func_002060E0(s32 a, s32 b, u32 c, s32 d);
B12Node *func_00204EDC(s32 unused, s32 a1, u32 a2, s32 a3) {
    u32 mask;
    u32 m2;
    B12Node *r;
    mask = func_0029DFF0();
    if (++D_800D8DCC != 1) {
        func_0029E010(mask);
        osRecvMesg(&D_800D8DB0, 0, 1);
    } else {
        func_0029E010(mask);
    }
    r = func_002060E0(0, a1, a2, a3);
    m2 = func_0029DFF0();
    if (--D_800D8DCC != 0) {
        func_0029E010(m2);
        osSendMesg(&D_800D8DB0, 0, 1);
    } else {
        func_0029E010(m2);
    }
    return r;
}
