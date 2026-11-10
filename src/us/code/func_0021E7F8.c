#include "common.h"
#include "structs/game_object.h"

extern s32 CMtxF__TransformBox(sptr, sptr, sptr);

void CGameObjectInstance__UpdateBounds(CGameObjectInstance *obj, sptr arg1) {
    ((void (*)(void *, void *)) obj->m_AI.m_pfCalculateOrientationMatrix)(obj, &obj->m_AI);
    CMtxF__TransformBox((sptr) obj->m_mfOrient, arg1, (sptr) &obj->m_vMin);
}
