#include "common.h"
#include "m2c_macros.h"

s32 CCache__ReleaseAndReplaceData(s32, s32, s32);
s32 Lighting_BuildLitVertices(s32, s32, s32, s32, s32);

void CGameObjectInstance__ConstructLitVertices(void *arg0, s32 **arg1)
{
  s32 new_var;
  new_var = *(*arg1);
  CCache__ReleaseAndReplaceData(0, (s32) arg1, Lighting_BuildLitVertices(new_var, 0, (s32) (arg0 + 0xBC), *((s32 *) (((s8 *) arg0) + 0x98)), 1));
}
