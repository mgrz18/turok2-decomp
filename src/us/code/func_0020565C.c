/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00253B5C.c, func_00253B5C.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/junk_types.h"

u32 __osDisableInt(void);
void __osRestoreInt(u32 mask);
s32 osSendMesg(OSMesgQueue *mq, OSMesg msg, s32 flag);
s32 osRecvMesg(OSMesgQueue *mq, OSMesg *msg, s32 flag);
extern B12Mgr D_800D7BE0;
extern OSMesgQueue D_800D8DB0;
extern s32 D_800D8DCC;
void func_0020565C(s32 unused, B12Node *n) {
    u32 mask;
    u32 m2;
    s32 idx = D_800D7BE0.f638;
    u32 bit;
    mask = __osDisableInt();
    if (++D_800D8DCC != 1) {
        __osRestoreInt(mask);
        osRecvMesg(&D_800D7BE0.lock.queue, 0, 1);
    } else {
        __osRestoreInt(mask);
    }
    if (D_800D7BE0.tbl628[idx]) {
        if (idx) {
            bit = 0x400;
        } else {
            bit = 0x200;
        }
        if (!(n->flags & bit)) {
            n->flags |= bit;
            D_800D7BE0.tbl11C4[idx]++;
        }
    }
    m2 = __osDisableInt();
    if (--D_800D8DCC != 0) {
        __osRestoreInt(m2);
        osSendMesg(&D_800D8DB0, 0, 1);
    } else {
        __osRestoreInt(m2);
    }
}
