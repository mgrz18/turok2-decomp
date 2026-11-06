#include "common.h"

s32 WeightedTable_Add(s32, s32, s32);
s32 WeightedTable_Pick(s32);

s16 EnemyAnim_Combat_AFE58(s32, s32);                    /* extern */
s16 EnemyAnim_Leap_AFEE8(s32, s32);                    /* extern */
s16 EnemyAnim_Weapon_B00C0(s32, s32);                    /* extern */
s32 WeightedTable_Clear(s32 *);                   /* extern */

s16 EnemyAnim_PickAttack_B0E80(s32 arg0, s32 arg1) {
    u8 sp10[0x68];

    WeightedTable_Clear(sp10);
    WeightedTable_Add((s32) sp10, (s32) EnemyAnim_Combat_AFE58(arg0, arg1), 0xA);
    WeightedTable_Add((s32) sp10, (s32) EnemyAnim_Leap_AFEE8(arg0, arg1), 0xA);
    WeightedTable_Add((s32) sp10, (s32) EnemyAnim_Weapon_B00C0(arg0, arg1), 0xA);
    return WeightedTable_Pick((s32) sp10);
}
