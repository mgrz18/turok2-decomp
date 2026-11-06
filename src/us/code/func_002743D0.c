#include "structs/vector.h"
#include "globals/audio.h"


/* Distance falloff: 1 - d^2 / sSoundHearingRangeSq inside, 0 outside. */
f32 Sound_DistanceFalloff(CVector3 *arg0, CVector3 *arg1)
{
  f32 temp_f0;
  f32 temp_f0_2;
  f32 temp_f1;
  f32 temp_f3;
  temp_f1 = arg0->x - arg1->x;
  temp_f3 = arg0->y - arg1->y;
  temp_f0_2 = arg0->z - arg1->z;
  temp_f0 = ((temp_f1 * temp_f1) + (temp_f3 * temp_f3)) + (temp_f0_2 * temp_f0_2);
  if (!(sSoundHearingRangeSq <= temp_f0))
  {
    return 1.0f - (temp_f0 / sSoundHearingRangeSq);
  }
  if (arg1)
  {
    return 0.0f;
  }
  else
    if (arg1)
  {
    return 0.0f;
  }
  else
  {
    return 0.0f;
  }
}
