#include "common.h"
#include "structs/emitter.h"

void CList__Construct(CList *, s32, s32);
s32 CList__AddTail(CList *, CEmitter *);

void func_0042FFF0(CEmitterPool *arg0) {
    CEmitter *var_s0;
    s32 var_s1;

    CList__Construct(&arg0->m_ActiveList, 0, 4);
    CList__Construct(&arg0->m_FreeList, 0, 4);
    var_s1 = 0;
    var_s0 = arg0->m_Emitters;
    do {
        CList__AddTail(&arg0->m_FreeList, var_s0);
        var_s1 += 1;
        var_s0++;
    } while (var_s1 < 4);
    arg0->m_ActiveMask = 0;
}
