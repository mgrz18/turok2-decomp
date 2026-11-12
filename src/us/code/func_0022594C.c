#ifdef TARGET_PORT
/* host body (round 10 y2): the ROM reads gScene's m_pList6/m_nList6 at their N64 offsets (0xFCC,
 * 0x104C) with 4-byte slots; on the host the list sits elsewhere and holds 8-byte pointers: the
 * Blind One's group actors (cover flags) took garbage as a cover target and crashed in
 * AI_FindBestTarget. The nearest actor of the list by squared distance, as the ROM does. */
#include "common.h"
#include "structs/game_object.h"
#include "structs/world.h"

void *func_0022594C(void *arg0, void *arg1) {
    CScene *scene = arg0;
    CGameObjectInstance *self = arg1;
    CGameObjectInstance *best = NULL;
    f32 bestD = 3.4028235e38f;
    s32 i;

    for (i = 0; i < scene->m_nList6; i++) {
        CGameObjectInstance *a = scene->m_pList6[i];
        f32 dx = self->header.m_vPos.x - a->header.m_vPos.x;
        f32 dy = self->header.m_vPos.y - a->header.m_vPos.y;
        f32 dz = self->header.m_vPos.z - a->header.m_vPos.z;
        f32 d = dx * dx + dy * dy + dz * dz;
        if (d < bestD) {
            bestD = d;
            best = a;
        }
    }
    return best;
}
#else

typedef signed char s8;
typedef signed int s32;
typedef float f32;
void *func_0022594C(void *arg0, void *arg1)
{
  f32 temp_f0;
  f32 temp_f0_2;
  f32 temp_f1;
  f32 temp_f2;
  f32 var_f3;
  s32 temp_a3;
  s32 var_v1;
  void **var_a0;
  void *temp_v0;
  void *var_a2;
 do { var_v1 = 0; var_a2 = 0; } while (0);
  temp_a3 = *((s32 *) (((s8 *) arg0) + 0x104C));
  var_f3 = 3.4028235e38f;
  var_a0 = arg0 + 0xFCC;
  if (temp_a3 > 0)
  {
    do
    {
      temp_v0 = *var_a0;
      temp_f0 = (*((f32 *) (((s8 *) arg1) + 4))) - (*((f32 *) (((s8 *) temp_v0) + 4)));
      temp_f2 = (*((f32 *) (((s8 *) arg1) + 8))) - (*((f32 *) (((s8 *) temp_v0) + 8)));
      temp_f1 = (*((f32 *) (((s8 *) arg1) + 0xC))) - (*((f32 *) (((s8 *) temp_v0) + 0xC)));
      temp_f0_2 = ((temp_f0 * temp_f0) + (temp_f2 * temp_f2)) + (temp_f1 * temp_f1);
      var_v1 += 1;
      if (temp_f0_2 < var_f3)
      {
        var_f3 = temp_f0_2;
        var_a2 = temp_v0;
      }
      var_a0 = (typeof(var_a0)) ((s8 *) ((typeof(var_a0)) (((s8 *) var_a0) + 4)));
    }
    while (var_v1 < temp_a3);
  }
  return var_a2;
}
#endif
