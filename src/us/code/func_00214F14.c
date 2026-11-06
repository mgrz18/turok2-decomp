#include "common.h"
#include "m2c_macros.h"

s32 CCache__ReleaseAndReplaceData(s32, s32, s32);
s32 Lighting_BuildLitVertices(s32, s32, s32, s32, s32);

void CStaticObject__BuildLitVerticesCallback(void *arg0, s32 **arg1)
{
  s32 new_var;
  new_var = *(*arg1);
  CCache__ReleaseAndReplaceData(0, (s32) arg1, Lighting_BuildLitVertices(new_var, *(*((s32 **) (((s8 *) arg0) + 0x24))), (s32) (arg0 + 0x68), (s32) (arg0 + 0x28), 0));
}
