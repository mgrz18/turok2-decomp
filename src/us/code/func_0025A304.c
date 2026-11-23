#include "common.h"
#include "structs/game_object.h"

s32 WeightedTable_Add(sptr, s32, s32);
s16 WeightedTable_Pick(sptr); /* ABI: returns s16 */
s16 EnemyAnim_Combat_AFE28(void *, sptr);
s16 EnemyAnim_Leap_AFEB8(void *, sptr);
s16 EnemyAnim_Weapon_B0068(void *, sptr);
void WeightedTable_Clear(void *);

/* host body (round 8 w2): the mode-table picker of the index-0 attacks; the N64 text below reads
 * the definition at the N64 0x14 of the instance (the first enemy melee crashed here). The
 * combat, leap and weapon animation flags are the body's 0x6A, 0x6C and 0x72. */
s16 EnemyAnim_PickAttack_B0A60(CGameObjectInstance *arg0, sptr arg1) {
    u8 sp10[0x68];
    CEnemyIntelligence *info = &arg0->header.m_pIntelligence->m_Enemy;

    WeightedTable_Clear(sp10);
    if (info->m_wCombatAnimFlags & 0x1F) {
        WeightedTable_Add((sptr) sp10, (s32) EnemyAnim_Combat_AFE28(arg0, arg1), 0xA);
    }
    if (info->m_wLeapAnimFlags & 0x1F) {
        WeightedTable_Add((sptr) sp10, (s32) EnemyAnim_Leap_AFEB8(arg0, arg1), 0xA);
    }
    if (info->m_wWeaponAnimFlags & 0x1F) {
        WeightedTable_Add((sptr) sp10, (s32) EnemyAnim_Weapon_B0068(arg0, arg1), 0xA);
    }
    return WeightedTable_Pick((sptr) sp10);
}
