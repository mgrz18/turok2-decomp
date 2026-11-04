#include "common.h"
#include "structs/player.h"
#include "structs/game_object.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ f32 unk18;
} WeaponDef;

typedef struct {
    /* 0x000 */ u8 pad0[0x118];
    /* 0x118 */ f32 unk118;
} WeaponState;

extern f32 D_800A7294;
extern WeaponDef *D_800AF618[];        /* per weapon */

void func_00258C20(CGameObjectInstance *arg0, WeaponState *arg1) {
    arg1->unk118 = (f32) (D_800AF618[((CPlayer *) arg0->m_AI.m_pParent)->m_Data.m_CurrentWeapon]->unk18 * D_800A7294);
}
