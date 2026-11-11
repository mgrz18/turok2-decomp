#include "common.h"
#include "structs/level_actor.h"
#include "globals/scene.h"
#include "protos/scene.h"

s32 CPooledLevelActor__Release(sptr);
s32 CPlayer__CollectPickup(sptr, sptr, s32);
s32 GameObject_GetTypeId(sptr);

s32 CGameRegion__FireTriggers(sptr, s32, sptr);           /* extern */

void CPlayer__TouchPickup(sptr arg0, CPooledLevelActor *arg1) {
    u16 temp_v0;
    u16 temp_v1;

    temp_v1 = arg1->m_Actor.m_wFlags;
    if (temp_v1 & 8) {
        if (arg1->m_PoolFlags & 1) {
            goto block_4;
        }
    } else if (!(temp_v1 & 1)) {
block_4:
#ifdef TARGET_PORT
        /* the port: DEBUG-ONLY T2_PICKUP_LOG=1 logs each touch (port/game/t2debug.c) */
        s32 id = GameObject_GetTypeId((sptr) arg1);
        s32 got = CPlayer__CollectPickup(arg0, (sptr) arg1, id);
        extern void T2Port_LogPickup(sptr player, void *actor, s32 id, s32 got);
        T2Port_LogPickup(arg0, arg1, id, got);
        if (got != 0) {
#else
        if (CPlayer__CollectPickup(arg0, (sptr) arg1, GameObject_GetTypeId((sptr) arg1)) != 0) {
#endif
            temp_v0 = arg1->m_Actor.m_wFlags | 1;
            arg1->m_Actor.m_wFlags = temp_v0;
            if (temp_v0 & 8) {
                CPooledLevelActor__Release((sptr) arg1);
                return;
            }
            CScene__SetLevelActorKilled(&gScene, arg1, 1);
            CGameRegion__FireTriggers(arg1->m_Actor.header.m_pCurrentRegion, 0x400, arg0);
        }
    }
}
