#include "common.h"
#include "m2c_macros.h"

s32 AI_HearNoise(sptr, sptr, sptr, s32);

#ifdef TARGET_PORT
#include "structs/world.h"
#include "structs/game_object.h"

/* Host body: every kind-1 actor (CScene::m_pKind1, N64 0xDFC; count m_nKind1, 0xEFC) hears a
 * noise at arg2. The N64 text reads both at N64 offsets with s32 actor pointers, and tests an
 * uninitialised local (its FAKEMATCH `if (temp_s2)`), which clang -O2 compiles to a trap
 * (round 7, v4: the optimized macOS build stopped here on Turok's first landing). */
void func_00225C08(void *arg0, s32 arg1, sptr arg2) {
    CScene *scene = (CScene *) arg0;
    s32 n = scene->m_nKind1;
    s32 i;

    for (i = 0; i < n; i++) {
        CGameObjectInstance *actor = scene->m_pKind1[i];
        AI_HearNoise((sptr) actor, (sptr) &actor->m_AI, arg2, arg1);
    }
}
#else

void func_00225C08(void *arg0, s32 arg1, sptr arg2)
{
  s32 temp_a0;
  s32 temp_s2;
  s32 var_s0;
  void *var_s1;
  if (temp_s2)
  {
    temp_s2 = *((s32 *) (((s8 *) arg0) + 0xEFC));
    var_s0 = 0;
  }
  else
  {
    temp_s2 = *((s32 *) (((s8 *) arg0) + 0xEFC));
    var_s0 = 0;
  }
  if (temp_s2 > 0)
  {
    var_s1 = arg0;
    do
    {
      temp_a0 = *((s32 *) (((s8 *) var_s1) + 0xDFC));
      var_s1 += 4;
      var_s0 += 1;
      AI_HearNoise(temp_a0, temp_a0 + 0x140, arg2, arg1);
    }
    while (var_s0 < temp_s2);
  }
}
#endif
