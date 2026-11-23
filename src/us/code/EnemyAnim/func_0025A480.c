#include "common.h"

s32 WeightedTable_Add(sptr, s32, s32);
s16 WeightedTable_Pick(sptr); /* ABI: returns s16 (the host leaves the upper bits of the return register undefined) */

s16 EnemyAnim_Combat_AFE88(sptr, sptr);                    /* extern */
s16 EnemyAnim_Leap_AFF18(sptr, sptr);                    /* extern */
s16 EnemyAnim_Weapon_B00F0(sptr, sptr);                    /* extern */
void WeightedTable_Clear(void *);

s16 EnemyAnim_PickAttack_B1200(sptr arg0, sptr arg1) {
    u8 sp10[0x68];

    WeightedTable_Clear(sp10);
    WeightedTable_Add((sptr) sp10, (s32) (s16) EnemyAnim_Combat_AFE88(arg0, arg1), 0xA);
    WeightedTable_Add((sptr) sp10, (s32) (s16) EnemyAnim_Leap_AFF18(arg0, arg1), 0xA);
    WeightedTable_Add((sptr) sp10, (s32) (s16) EnemyAnim_Weapon_B00F0(arg0, arg1), 0xA);
    return WeightedTable_Pick((sptr) sp10);
}
