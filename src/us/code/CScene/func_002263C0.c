#include "common.h"
#include "m2c_macros.h"
#include "globals/engine.h"
#include "protos/scene.h"

s32 CStaticObject__SpawnEvents(sptr, s32);


#ifdef TARGET_PORT
/* Host body (round 10 y3, docs/port/FIDELITY.md 5): the same by member. The N64 view read the
 * timer (0x18EB8), the count (0xBF4) and the s32 list (0xAF4) at N64 offsets of CScene, so on
 * the host no listed static object ever fired its events: demo 1's ambient particle (type 103,
 * kind 8 at gf 201, every second after) never spawned. */
#include "structs/world.h"
void CScene__AdvanceListedInstances(void *arg0) {
    CScene *world = (CScene *) arg0;
    f32 *timer = (f32 *) &world->unk_18EB8; /* a float: the second's fraction */
    f32 t;
    s32 all;
    s32 i;

    t = *timer + gFrameIncrement;
    *timer = t;
    all = t > 0.0f;
    if (all) {
        *timer = t - (f32) ((s32) t + 1);
    }
    for (i = 0; i < world->m_nListed; i++) {
        CStaticObject__SpawnEvents((sptr) world->m_pListed[i], all);
    }
}
#else
void CScene__AdvanceListedInstances(void *arg0)
{
  f32 temp_f1;
  int new_var;
  s32 temp_a0;
  s32 var_s0;
  s32 var_s3;
  void *var_s1;
  temp_f1 = (*((f32 *) (((s8 *) arg0) + 0x18EB8))) + gFrameIncrement;
  *((f32 *) (((s8 *) arg0) + 0x18EB8)) = temp_f1;
  var_s3 = 1;
  if (!(temp_f1 > 0.0f))
  {
    var_s3 = 0;
  }
  if (var_s3 != 0)
  {
    *((f32 *) (((s8 *) arg0) + 0x18EB8)) = (f32) (temp_f1 - ((f32) (((s32) temp_f1) + 1)));
  }
  var_s0 = 0;
  new_var = 0;
  if ((*((s32 *) (((s8 *) arg0) + 0xBF4))) > new_var)
  {
    var_s1 = arg0;
    do
    {
      temp_a0 = *((s32 *) (((s8 *) var_s1) + 0xAF4));
      var_s1 += 4;
      CStaticObject__SpawnEvents(temp_a0, var_s3);
      var_s0 += 1;
    }
    while (var_s0 < (*((s32 *) (((s8 *) arg0) + 0xBF4))));
  }
}
#endif
