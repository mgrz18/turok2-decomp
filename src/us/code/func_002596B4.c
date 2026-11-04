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
extern s32 D_800F7078;
extern u8 D_801198B0[];
void func_0027FC2C(void *buf, s32 a, s32 b);
void func_00253DE0(CPlayer *player, s32 id);

void func_002596B4(CGameObjectInstance *actor, State *state) {
    CPlayer *player = ((CPlayer *)actor->m_AI.m_pParent);

    if (player->m_Data.m_CurrentWeapon != player->m_RequestedWeapon || D_800F7078 != 4) {
        player->m_Data.m_CurrentWeapon = player->m_RequestedWeapon;
        state->unk2C = D_800AF618[player->m_Data.m_CurrentWeapon]->modeTable;
        state->unk108 = D_800AF618[player->m_Data.m_CurrentWeapon]->effects;
        if (D_800B6D20 >= 5) {
            func_0027FC2C(D_801198B0, player->m_pCamera, D_800AF618[player->m_Data.m_CurrentWeapon]->name);
        }
        state->unk10C = 0;
        state->unk110 = 0;
        state->unk128 = 0;
        state->unk118 = 0;
    }
    func_00253DE0(player, 0x1A8);
}
