#include "common.h"
#include "structs/emitter.h"
#include "protos/list.h"


void CEmitterPool__Construct(CEmitterPool *arg0) {
    CEmitter *var_s0;
    s32 var_s1;

#ifdef TARGET_PORT
    CList__Construct(&arg0->m_ActiveList, T2_OFFSETOF(CEmitter, m_pLast), T2_OFFSETOF(CEmitter, m_pNext));
#else
    CList__Construct(&arg0->m_ActiveList, 0, 4);
#endif
#ifdef TARGET_PORT
    CList__Construct(&arg0->m_FreeList, T2_OFFSETOF(CEmitter, m_pLast), T2_OFFSETOF(CEmitter, m_pNext));
#else
    CList__Construct(&arg0->m_FreeList, 0, 4);
#endif
    var_s1 = 0;
    var_s0 = arg0->m_Emitters;
    do {
        CList__AddTail(&arg0->m_FreeList, var_s0);
        var_s1 += 1;
        var_s0++;
    } while (var_s1 < 4);
    arg0->m_ActiveMask = 0;
}
