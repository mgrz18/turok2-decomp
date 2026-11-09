
#include "protos/scene.h"
void CScene__MarkGeneratorsSaved(void *arg0)
{
  s32 temp_s2;
  int new_var;
  s32 var_s0;
  s32 var_s1;
  void *temp_a1;
  temp_s2 = *((s32 *) (((s8 *) arg0) + 0xE8));
  var_s0 = 0;
  var_s1 = 0;
  if (temp_s2 > 0)
  {
    new_var = 3;
    var_s1 = 0;
    do
    {
      temp_a1 = (*((s32 *) (((s8 *) arg0) + 0xE0))) + var_s1;
      if ((*(*((s32 **) (((s8 *) temp_a1) + 0x14)))) == new_var)
      {
        CScene__SetInstanceSavedBit(arg0, temp_a1, 1);
      }
      var_s0 += 1;
      var_s1 += 0x288;
    }
    while (var_s0 < temp_s2);
  }
}
