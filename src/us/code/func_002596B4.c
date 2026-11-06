#include "common.h"
#include "structs/game_object.h"
#include "structs/player.h"
#include "structs/weapon.h"
#include "globals/camera.h"
#include "globals/scene.h"
#include "globals/engine.h"

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

void CCameraPool__AddText(void *buf, s32 a, s32 b);
void CPlayer__StartWeaponBackgroundSound(CPlayer *player, s32 id);

void WeaponActor_EquipRequested(CGameObjectInstance *actor, State *state) {
    CPlayer *player = ((CPlayer *)actor->m_AI.m_pParent);

    if (player->m_Data.m_CurrentWeapon != player->m_RequestedWeapon || gScene.m_Mode != 4) {
        player->m_Data.m_CurrentWeapon = player->m_RequestedWeapon;
        state->unk2C = gWeaponInfoTable[player->m_Data.m_CurrentWeapon]->modeTable;
        state->unk108 = gWeaponInfoTable[player->m_Data.m_CurrentWeapon]->effects;
        if (gFrameCountGameplay >= 5) {
            CCameraPool__AddText(&gEngineApp_m_Game_m_CameraPool, player->m_pCamera, gWeaponInfoTable[player->m_Data.m_CurrentWeapon]->name);
        }
        state->unk10C = 0;
        state->unk110 = 0;
        state->unk128 = 0;
        state->unk118 = 0;
    }
    CPlayer__StartWeaponBackgroundSound(player, 0x1A8);
}
