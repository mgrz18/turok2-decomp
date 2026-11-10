#include "common.h"
#include "m2c_macros.h"
#include "protos/cache.h"

sptr Lighting_BuildLitVertices(sptr, sptr, sptr, sptr, s32);

void CGameObjectInstance__ConstructLitVertices(void *arg0, s32 **arg1)
{
  sptr new_var;
#ifdef TARGET_PORT
  /* the block's first word is a host pointer (8 bytes): read it whole */
  new_var = *(sptr *)(*arg1);
#else
  new_var = *(*arg1);
#endif
  CCache__ReleaseAndReplaceData(0, arg1, Lighting_BuildLitVertices(new_var, 0, (sptr) (arg0 + 0xBC), *((s32 *) (((s8 *) arg0) + 0x98)), 1));
}
