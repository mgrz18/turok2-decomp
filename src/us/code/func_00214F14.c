#include "common.h"
#include "m2c_macros.h"
#include "protos/cache.h"
#ifdef TARGET_PORT
#include "structs/level_actor.h"
#include "structs/cache.h"
#endif

#ifdef TARGET_PORT /* host: the model, regions and box are pointers */
sptr Lighting_BuildLitVertices(sptr, sptr, sptr, sptr, s32);
#else
sptr Lighting_BuildLitVertices(s32, s32, s32, sptr, s32);
#endif

void CStaticObject__BuildLitVerticesCallback(void *arg0, s32 **arg1)
{
#ifdef TARGET_PORT /* the static object's members (structs/level_actor.h); arg1 is the CCacheEntry */
  sptr new_var = (sptr) ((CCacheEntry *) arg1)->memEntry->data;
  CStaticObject *o = arg0;
  CCache__ReleaseAndReplaceData(0, arg1, Lighting_BuildLitVertices(new_var, (sptr) *o->m_pOwner, (sptr) &o->m_Bounds, (sptr) o->m_Matrix, 0));
#else
  s32 new_var;
  new_var = *(*arg1);
  CCache__ReleaseAndReplaceData(0, arg1, Lighting_BuildLitVertices(new_var, *(*((s32 **) (((s8 *) arg0) + 0x24))), (sptr) (arg0 + 0x68), (sptr) (arg0 + 0x28), 0));
#endif
}
