#include "common.h"

sptr CGameObjectInstance__PickRandomAnim(s32, sptr, sptr);
s32 WeightedTable_Add(s32, s32, s32);
s32 WeightedTable_Pick(s32);

s32 WeightedTable_Clear(void *);                   /* extern */

s16 CGameObjectInstance__PickAvailableAction(s32 arg0, s16 *arg1, s32 arg2)
{
  s32 new_var2;
  u8 sp10[0x68];
  s16 *var_s0;
  int new_var;
  u16 var_a1;
  WeightedTable_Clear(sp10);
  var_s0 = arg1;
  var_a1 = (u16) (*var_s0);
  if ((*var_s0) != (-1))
  {
    do
    {
      if (CGameObjectInstance__PickRandomAnim(arg0, (s32) ((s16) var_a1), arg2) != (-1))
      {
        new_var = 2;
        new_var2 = (s32) (*((s16 *) (((s8 *) var_s0) + 0)));
        WeightedTable_Add((sptr) (sp10), new_var2, (s32) (*((s16 *) (((s8 *) var_s0) + new_var))));
      }
      var_s0 = (typeof(var_s0)) ((s8 *) ((typeof(var_s0)) (((s8 *) var_s0) + 4)));
      var_a1 = (u16) (*var_s0);
    }
    while ((*var_s0) != (-1));
  }
  return WeightedTable_Pick((sptr) (sp10));
}
