#include "common.h"
#include "m2c_macros.h"
#include "protos/cache.h"
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
