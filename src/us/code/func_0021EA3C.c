#include "common.h"

sptr CGameObjectInstance__PickRandomAnim(sptr, sptr, sptr);
s32 WeightedTable_Add(sptr, s32, s32);
#ifdef TARGET_PORT
s16 WeightedTable_Pick(sptr); /* ABI: returns s16 (the host leaves the upper bits of the return register undefined) */
#else
s32 WeightedTable_Pick(sptr);
#endif

s32 WeightedTable_Clear(void *);                   /* extern */

#ifdef TARGET_PORT
/* Host body (round 7 v2). The table (id, weight) s16 pairs ending in -1 is reached only through
 * a mode row's animation word (CAIModeInfo.unk_10, an sptr: CAI__UpdateModeAnim), so gendata
 * has no type for it and copies it as bytes in ROM order: read the halves big-endian. The
 * caller declares the result as a pointer: return it sign-extended and pointer-wide (an s16
 * return left the high bits of the register undefined). */
static s16 T2_BE16(const s16 *p) {
    const u8 *b = (const u8 *) p;
    return (s16) ((b[0] << 8) | b[1]);
}
sptr CGameObjectInstance__PickAvailableAction(sptr arg0, s16 *arg1, s32 arg2) {
    u8 sp10[0x68];
    s16 *entry;

    WeightedTable_Clear(sp10);
    for (entry = arg1; T2_BE16(entry) != -1; entry += 2) {
        if (CGameObjectInstance__PickRandomAnim(arg0, (sptr) T2_BE16(entry), arg2) != -1) {
            WeightedTable_Add((sptr) sp10, (s32) T2_BE16(entry), (s32) T2_BE16(entry + 1));
        }
    }
    return (sptr) (s16) WeightedTable_Pick((sptr) sp10);
}
#else
s16 CGameObjectInstance__PickAvailableAction(sptr arg0, s16 *arg1, s32 arg2)
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
#endif
