#include "common.h"
#include "structs/vector.h"
#include "structs/collision.h"

extern CCollisionResults *D_800F1ADC;     /* current hit buffer */
extern s8 D_800F1CC6[];           /* collision info D_800F1CC0 + 6 */

void func_00239F00(CInstanceHeader *actor, CVector3 pos, void *info);

/* Moves an actor to pos with collision, without moving it: returns where it would end up and in which region. */
void func_002688C8(CInstanceHeader *actor, CVector3 pos, CVector3 *outPos, void **outRegion, s32 flagA, s32 flagB) {
    CInstanceHeader saved;
    CVector3 end;
    CCollisionResults hits;
    CCollisionResults *oldHits;
    s32 oldA;
    s32 oldB;
    void *region;
    s8 *flags;

    saved = *actor;
    oldHits = D_800F1ADC;
    D_800F1ADC = &hits;
    flags = D_800F1CC6;
    oldA = flags[0];
    oldB = flags[-2];
    flags[0] = flagA;
    flags[-2] = flagB;
    func_00239F00(actor, pos, flags - 6);
    D_800F1ADC = oldHits;
    actor->m_vPos.y = pos.y;
    end.x = actor->m_vPos.x;
    end.y = pos.y;
    end.z = actor->m_vPos.z;
    region = actor->m_pCurrentRegion;
    *actor = saved;
    *outPos = end;
    *outRegion = region;
    flags[0] = oldA;
    flags[-2] = oldB;
}
