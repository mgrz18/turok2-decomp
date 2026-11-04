#include "common.h"
#include "structs/game_object.h"

extern u8 D_800F7078[];
extern s32 func_002241C0(void *, s32);

/* The type id of an object (kind 1) or of a particle. */
s32 func_00268C04(CGameObjectInstance *arg0) {
    if (arg0->header.m_Type == 1) {
        return (u16) arg0->m_nTypeFlag;
    }
    return func_002241C0(D_800F7078, arg0->header.m_nObjType);
}
