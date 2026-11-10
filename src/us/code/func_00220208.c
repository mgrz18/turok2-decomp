#include "common.h"
#include "m2c_macros.h"
#include "protos/cache.h"
#ifdef TARGET_PORT
#include "structs/game_object.h"
#include "structs/cache.h"

sptr Lighting_BuildLitVertices(sptr, sptr, sptr, sptr, s32);

/* host: the box (m_vMin/m_vMax) and the draw matrices at the host offsets; arg1 is the CCacheEntry */
void CGameObjectInstance__ConstructLitVertices(void *arg0, s32 **arg1)
{
  CGameObjectInstance *o = arg0;
  sptr new_var = (sptr) ((CCacheEntry *) arg1)->memEntry->data;
  CCache__ReleaseAndReplaceData(0, arg1, Lighting_BuildLitVertices(new_var, 0, (sptr) &o->m_vMin, (sptr) o->m_pmtDrawMtxs, 1));
}
#else
sptr Lighting_BuildLitVertices(s32, s32, s32, sptr, s32);

void CGameObjectInstance__ConstructLitVertices(void *arg0, s32 **arg1)
{
  s32 new_var;
  new_var = *(*arg1);
  CCache__ReleaseAndReplaceData(0, arg1, Lighting_BuildLitVertices(new_var, 0, (sptr) (arg0 + 0xBC), *((s32 *) (((s8 *) arg0) + 0x98)), 1));
}
#endif
