#include "common.h"
#include "structs/game_object.h"
#include "globals/scene.h"
#include "protos/scene.h"

/* The type id of an object (kind 1) or of a particle. The ROM does not mask the u16 that
 * CScene__GetObjectTypeID returns (its caller's prototype returned a word); the host must
 * (docs/port/ABI.md: the upper bits of the return register are undefined). */
s32 GameObject_GetTypeId(CGameObjectInstance *arg0) {
    if (arg0->header.m_Type == 1) {
        return (u16) arg0->m_nTypeFlag;
    }
#ifdef TARGET_PORT
    return CScene__GetObjectTypeID(&gScene, arg0->header.m_nObjType);
#else
    return ((s32 (*)(struct CScene *, s32)) CScene__GetObjectTypeID)(&gScene, arg0->header.m_nObjType);
#endif
}
