#include "common.h"
#include "structs/game_object.h"
#include "globals/scene.h"

extern s32 CScene__GetObjectTypeID(void *, s32);

/* The type id of an object (kind 1) or of a particle. */
s32 GameObject_GetTypeId(CGameObjectInstance *arg0) {
    if (arg0->header.m_Type == 1) {
        return (u16) arg0->m_nTypeFlag;
    }
    return CScene__GetObjectTypeID(&gScene, arg0->header.m_nObjType);
}
