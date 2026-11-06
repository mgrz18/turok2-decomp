#include "common.h"
#include "structs/emitter.h"

void CList__Construct(s32, s32, s32);
s32 CList__AddTail(s32, s32);

void CEmitterPool__Construct_2(CEmitterPool *arg0) {
    s32 var_s0;
    s32 var_s1;

    CList__Construct((s32) &arg0->m_ActiveList, 0, 4);
    CList__Construct((s32) &arg0->m_FreeList, 0, 4);
    var_s1 = 0;
    var_s0 = (s32) arg0;
    do {
        CList__AddTail((s32) &arg0->m_FreeList, var_s0);
        var_s1 += 1;
        var_s0 += 0x228;
    } while (var_s1 < 4);
    arg0->m_ActiveMask = 0;
}
