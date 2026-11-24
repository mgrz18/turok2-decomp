#include "common.h"
#include "structs/game_object.h"

void CMtxF__TransformBox(void *m, void *in, void *out);

void CGameObjectInstance__UpdateBounds(CGameObjectInstance *obj, sptr arg1) {
    ((void (*)(void *, void *)) obj->m_AI.m_pfCalculateOrientationMatrix)(obj, &obj->m_AI);
    CMtxF__TransformBox(obj->m_mfOrient, (void *) arg1, &obj->m_vMin);
}
