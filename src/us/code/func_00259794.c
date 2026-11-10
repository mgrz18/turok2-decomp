#include "common.h"
#include "structs/game_object.h"
#include "structs/player.h"
#include "structs/weapon.h"


#ifdef TARGET_PORT
/* Host body: the State view's 0x64 is m_ConditionTimer (cleared as a word), 0x118 m_Weapon.m_FireDelay. */
void CPlayer__StartWeaponFiringSound(CPlayer *player, s32 id);

void WeaponMode_AE714_Enter(CGameObjectInstance *actor, CAI *state) {
    CPlayer *player = ((CPlayer *)actor->m_AI.m_pParent);

    *(s32 *) &state->m_ConditionTimer = 0;
    state->m_Weapon.m_FireDelay = gWeaponInfoTable[((CPlayer *)actor->m_AI.m_pParent)->m_Data.m_CurrentWeapon]->fireDelay * 15.0f;
    CPlayer__StartWeaponFiringSound(player, 0x1A9);
}
#else
typedef struct {
    /* 0x000 */ u8 pad0[0x64];
    /* 0x064 */ s32 unk64;
    /* 0x068 */ u8 pad68[0x118 - 0x68];
    /* 0x118 */ f32 unk118;
} State;

void CPlayer__StartWeaponFiringSound(CPlayer *player, s32 id);

void WeaponMode_AE714_Enter(CGameObjectInstance *actor, State *state) {
    CPlayer *player = ((CPlayer *)actor->m_AI.m_pParent);

    state->unk64 = 0;
    state->unk118 = gWeaponInfoTable[((CPlayer *)actor->m_AI.m_pParent)->m_Data.m_CurrentWeapon]->fireDelay * 15.0f;
    CPlayer__StartWeaponFiringSound(player, 0x1A9);
}
#endif
