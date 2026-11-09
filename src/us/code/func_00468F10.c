#include "common.h"
#include "structs/emitter.h"
#include "protos/list.h"


void CEmitterPool__Construct_2(CEmitterPool *arg0) {
    sptr var_s0;
    s32 var_s1;

    CList__Construct(&arg0->m_ActiveList, 0, 4);
    CList__Construct(&arg0->m_FreeList, 0, 4);
    var_s1 = 0;
    var_s0 = (sptr) arg0;
    do {
        CList__AddTail(&arg0->m_FreeList, var_s0);
        var_s1 += 1;
        var_s0 += 0x228;
    } while (var_s1 < 4);
    arg0->m_ActiveMask = 0;
}
