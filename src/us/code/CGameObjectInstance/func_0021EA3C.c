#include "common.h"
struct CGameObjectInstance;

sptr CGameObjectInstance__PickRandomAnim(struct CGameObjectInstance *actor, sptr action, sptr skip);
/* The entries passed as words (the halves sign-extended already): the definition (code/DList,
 * (table, s16, s16)) under its own name on the N64. */
#ifdef TARGET_PORT
s32 WeightedTable_Add(void *table, s16 id, s16 weight);
#define WeightedTable_Add_w(t, id, w) WeightedTable_Add((void *) (t), id, w)
#else
s32 WeightedTable_Add_w(sptr table, s32 id, s32 weight) __asm__("WeightedTable_Add");
#endif
s16 WeightedTable_Pick(void *table); /* ABI: returns s16 (the host leaves the upper bits of the return register undefined) */

void WeightedTable_Clear(void *table);

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
        if (CGameObjectInstance__PickRandomAnim((struct CGameObjectInstance *) arg0, (sptr) T2_BE16(entry), arg2) != -1) {
            WeightedTable_Add_w((sptr) sp10, (s32) T2_BE16(entry), (s32) T2_BE16(entry + 1));
        }
    }
    return (sptr) (s16) WeightedTable_Pick(sp10);
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
      if (CGameObjectInstance__PickRandomAnim((struct CGameObjectInstance *) arg0, (s32) ((s16) var_a1), arg2) != (-1))
      {
        new_var = 2;
        new_var2 = (s32) (*((s16 *) (((s8 *) var_s0) + 0)));
        WeightedTable_Add_w((sptr) (sp10), new_var2, (s32) (*((s16 *) (((s8 *) var_s0) + new_var))));
      }
      var_s0 = (typeof(var_s0)) ((s8 *) ((typeof(var_s0)) (((s8 *) var_s0) + 4)));
      var_a1 = (u16) (*var_s0);
    }
    while ((*var_s0) != (-1));
  }
  return WeightedTable_Pick(sp10);
}
#endif
