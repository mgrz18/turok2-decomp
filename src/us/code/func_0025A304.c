#include "common.h"
#include "m2c_macros.h"

s32 WeightedTable_Add(sptr, s32, s32);
s32 WeightedTable_Pick(sptr);

s16 EnemyAnim_Combat_AFE28(void *, sptr);                 /* extern */
s16 EnemyAnim_Leap_AFEB8(void *, sptr);                 /* extern */
s16 EnemyAnim_Weapon_B0068(void *, sptr);                 /* extern */
M2C_UNK WeightedTable_Clear(M2C_UNK *);                   /* extern */

s16 EnemyAnim_PickAttack_B0A60(void *arg0, M2C_UNK arg1) {
    u8 sp10[0x68];
    void *temp_s0;

    temp_s0 = M2C_FIELD(arg0, s32 *, 0x14) + 0x14;
    WeightedTable_Clear(sp10);
    if (M2C_FIELD(temp_s0, u16 *, 0x6A) & 0x1F) {
        WeightedTable_Add((sptr) sp10, (s32) EnemyAnim_Combat_AFE28(arg0, arg1), 0xA);
    }
    if (M2C_FIELD(temp_s0, u16 *, 0x6C) & 0x1F) {
        WeightedTable_Add((sptr) sp10, (s32) EnemyAnim_Leap_AFEB8(arg0, arg1), 0xA);
    }
    if (M2C_FIELD(temp_s0, u16 *, 0x72) & 0x1F) {
        WeightedTable_Add((sptr) sp10, (s32) EnemyAnim_Weapon_B0068(arg0, arg1), 0xA);
    }
    return WeightedTable_Pick((sptr) sp10);
}
