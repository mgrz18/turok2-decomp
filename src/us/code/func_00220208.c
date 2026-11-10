#include "common.h"
#include "m2c_macros.h"
#include "protos/cache.h"

#ifdef TARGET_PORT
#include "structs/game_object.h"
sptr Lighting_BuildLitVertices(sptr, sptr, sptr, sptr, s32);

/* Host body: the model data pointer read pointer-wide, the instance's box (N64 0xBC
 * m_vMin) and bone matrices (0x98 m_pmtDrawMtxs) by member. */
void CGameObjectInstance__ConstructLitVertices(void *arg0, s32 **arg1)
{
  CGameObjectInstance *obj = arg0;

  CCache__ReleaseAndReplaceData(0, arg1, Lighting_BuildLitVertices(*(sptr *) *arg1, 0, (sptr) &obj->m_vMin,
                                                                   (sptr) obj->m_pmtDrawMtxs, 1));
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
