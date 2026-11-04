#include "common.h"
#include "structs/player.h"
#include "structs/game_object.h"
#include "structs/weapon.h"

typedef struct {
    /* 0x000 */ u8 pad0[0x118];
    /* 0x118 */ f32 unk118;
} WeaponState;

extern f32 D_800A7294;

void func_00258C20(CGameObjectInstance *arg0, WeaponState *arg1) {
    arg1->unk118 = (f32) (D_800AF618[((CPlayer *) arg0->m_AI.m_pParent)->m_Data.m_CurrentWeapon]->fireDelay * D_800A7294);
}
