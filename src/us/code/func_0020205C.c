/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80256280_de_4.c, func_8025661C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern u32 __osDisableInt(void);
extern void __osRestoreInt(u32);
extern void CList__Remove(void *, s32);
extern s32 CList__AddHead(void *, s32);
void *func_0020205C(CartridgeReadWorker *arg0) {
    u32 temp_s2;
    void *temp_s0;
    temp_s2 = __osDisableInt();
    temp_s0 = arg0->available.m_pHead;
    if (temp_s0 != 0) {
        CList__Remove(&arg0->available, (s32) temp_s0);
        ((CartridgeReadRequest *)temp_s0)->active = 1;
        CList__AddHead(&arg0->pending, (s32) temp_s0);
    }
    __osRestoreInt(temp_s2);
    return temp_s0;
}
extern u32 __osDisableInt(void);
extern void __osRestoreInt(u32);
extern void CList__Remove(void *, s32);
extern s32 CList__AddHead(void *, s32);
