#include "common.h"

s32 WeightedTable_Add(s32, s32, s32);
s32 WeightedTable_Pick(s32);

s16 EnemyAnim_Combat_AFE88(s32, s32);                    /* extern */
s16 EnemyAnim_Leap_AFF18(s32, s32);                    /* extern */
s16 EnemyAnim_Weapon_B00F0(s32, s32);                    /* extern */
s32 WeightedTable_Clear(s32 *);                   /* extern */

s16 EnemyAnim_PickAttack_B1200(s32 arg0, s32 arg1) {
    u8 sp10[0x68];

    WeightedTable_Clear(sp10);
    WeightedTable_Add((sptr) sp10, (s32) EnemyAnim_Combat_AFE88(arg0, arg1), 0xA);
    WeightedTable_Add((sptr) sp10, (s32) EnemyAnim_Leap_AFF18(arg0, arg1), 0xA);
    WeightedTable_Add((sptr) sp10, (s32) EnemyAnim_Weapon_B00F0(arg0, arg1), 0xA);
    return WeightedTable_Pick((sptr) sp10);
}
