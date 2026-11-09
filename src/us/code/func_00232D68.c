#include "common.h"
#include "structs/game_object.h"
#include "structs/particle.h"
#include "protos/list.h"


/* Insert arg1 into the list at 0xEC14: after the node with the same key at
 * 0xF8 if there is one, else at the end or the front by a flag. */
void func_00232D68(CParticleSystem *arg0, CParticle *arg1) {
    CParticle *p;

    for (p = arg0->m_DrawList.m_pHead; p != NULL; p = p->m_pDrawNext) {
        if (p->m_pEffect == arg1->m_pEffect) {
            break;
        }
    }
    if (p != NULL) {
        CList__InsertBefore(&arg0->m_DrawList, p, arg1);
    } else if (arg1->m_pEffect->m_dwFlags & 0x2000) {
        CList__AddHead(&arg0->m_DrawList, arg1);
    } else {
        CList__AddTail(&arg0->m_DrawList, arg1);
    }
    arg1->m_dwFlags |= 0x01000000;
}
