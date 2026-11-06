#include "common.h"
#include "structs/game_object.h"
#include "structs/player.h"
#include "structs/weapon.h"


typedef struct {
    /* 0x000 */ u8 pad0[0x2C];
    /* 0x02C */ s32 unk2C;
    /* 0x030 */ u8 pad30[0x108 - 0x30];
    /* 0x108 */ s32 unk108;
    /* 0x10C */ s32 unk10C;
    /* 0x110 */ s32 unk110;
    /* 0x114 */ u8 pad114[4];
    /* 0x118 */ s32 unk118;
    /* 0x11C */ u8 pad11C[0x128 - 0x11C];
    /* 0x128 */ s32 unk128;
} State;

extern u32 D_800B6D20;
extern s32 gScene;
extern u8 D_801198B0[];
void CCameraPool__AddText(void *buf, s32 a, s32 b);
void CPlayer__StartWeaponBackgroundSound(CPlayer *player, s32 id);

void WeaponActor_EquipRequested(CGameObjectInstance *actor, State *state) {
    CPlayer *player = ((CPlayer *)actor->m_AI.m_pParent);

    if (player->m_Data.m_CurrentWeapon != player->m_RequestedWeapon || gScene != 4) {
        player->m_Data.m_CurrentWeapon = player->m_RequestedWeapon;
        state->unk2C = D_800AF618[player->m_Data.m_CurrentWeapon]->modeTable;
        state->unk108 = D_800AF618[player->m_Data.m_CurrentWeapon]->effects;
        if (D_800B6D20 >= 5) {
            CCameraPool__AddText(D_801198B0, player->m_pCamera, D_800AF618[player->m_Data.m_CurrentWeapon]->name);
        }
        state->unk10C = 0;
        state->unk110 = 0;
        state->unk128 = 0;
        state->unk118 = 0;
    }
    CPlayer__StartWeaponBackgroundSound(player, 0x1A8);
}
